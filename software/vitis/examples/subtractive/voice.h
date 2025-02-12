#ifndef VOICE_H
#define VOICE_H

#include "config.h"
#include "ADSR.h"
#include <stdbool.h>

#define NUM_VOICES 8
#define A4_MIDI_NOTE 69
#define A4_FREQUENCY 440.0

#define FREQS_BASE_ADDR 0x40010020
#define GAINS_BASE_ADDR 0x40010040
#define OSC_BASE_ADDR XPAR_XBLOSC_BANK_DET_0_S_AXI_CONTROL_BASEADDR
#define WAVE_1_REG 0x18
#define WAVE_2_REG 0x60
#define DET_REG 0x10

float global_cf, global_res;

float midi_to_freq(int midi_note)
{
    return A4_FREQUENCY * pow(2.0, (midi_note - A4_MIDI_NOTE) / 12.0);
}

void write_freq(int index, float freq)
{
    uint32_t address = FREQS_BASE_ADDR + (index * sizeof(float));

    uint32_t int_rep = *(uint32_t *)&freq;

    Xil_Out32(address, int_rep);
}

void write_gain(int index, float gain)
{
    uint32_t address = GAINS_BASE_ADDR + (index * sizeof(float));

    float gain_scaled = gain * 0.125;

    uint32_t int_rep = *(uint32_t *)&gain_scaled;

    Xil_Out32(address, int_rep);
}

void write_wave1(int wave)
{
    Xil_Out32(OSC_BASE_ADDR + WAVE_1_REG, wave);
}

void write_wave2(int wave)
{
    Xil_Out32(OSC_BASE_ADDR + WAVE_2_REG, wave);
}

void write_det(int det)
{
    Xil_Out32(OSC_BASE_ADDR + DET_REG, *(uint32_t *)&det);
}

typedef struct
{
    ADSR adsr;
    ADSR filtAdsr;
    float frequency;
    float cutoff;
    float res;
    bool gate;
    float velocity;
    bool active;
} Voice;

Voice voices[NUM_VOICES];

void init_voices()
{
    for (int i = 0; i < NUM_VOICES; i++)
    {
        ADSR_init(&voices[i].adsr, 48000.0f, ADSR_LINEAR);
        ADSR_setParams(&voices[i].adsr, 0.01, 0.3, 0.9, 0.01);

        ADSR_init(&voices[i].filtAdsr, 48000.0f, ADSR_LINEAR);
        ADSR_setParams(&voices[i].filtAdsr, 0.1, 0.3, 0.4, 0.01);

        voices[i].frequency = 0.0f;
        voices[i].velocity = 0.0f;
        voices[i].gate = false;
        voices[i].active = false;
    }
}

void set_amp_env(float a, float d, float s, float r)
{
    for (int i = 0; i < NUM_VOICES; i++)
    {
        ADSR_setParams(&voices[i].adsr, a, d, s, r);
    }
}

void set_filt_env(float a, float d, float s, float r)
{
    for (int i = 0; i < NUM_VOICES; i++)
    {
        ADSR_setParams(&voices[i].filtAdsr, a, d, s, r);
    }
}

void set_amp_env_mode(int mode)
{
    for (int i = 0; i < NUM_VOICES; i++)
    {
        ADSR_setMode(&voices[i].adsr, mode);
    }
}

void set_filt_env_mode(int mode)
{
    for (int i = 0; i < NUM_VOICES; i++)
    {
        ADSR_setMode(&voices[i].filtAdsr, mode);
    }
}

int allocate_voice()
{
    for (int i = 0; i < NUM_VOICES; i++)
    {
        if (!voices[i].active)
        {
            return i;
        }
    }
    return -1;
}

void note_on(int note, float velocity)
{
    int voice_index = allocate_voice();
    if (voice_index != -1)
    {
        voices[voice_index].frequency = midi_to_freq(note);
        voices[voice_index].velocity = velocity;
        voices[voice_index].gate = true;
        voices[voice_index].active = true;
        ADSR_reset(&voices[voice_index].adsr);
        write_freq(voice_index, voices[voice_index].frequency);
    }
}

void note_off(int note)
{
    for (int i = 0; i < NUM_VOICES; i++)
    {
        if (voices[i].frequency == midi_to_freq(note) && voices[i].active)
        {
            voices[i].gate = false;
        }
    }
}

void process_voices()
{
    for (int i = 0; i < NUM_VOICES; i++)
    {
        if (voices[i].active)
        {
            float env_value = ADSR_process(&voices[i].adsr, voices[i].gate);
            float gain = voices[i].velocity * env_value;

            write_gain(i, gain);

            if (voices[i].adsr.state == ADSR_IDLE)
            {
                voices[i].active = false;
            }
        }
    }
}

#endif
