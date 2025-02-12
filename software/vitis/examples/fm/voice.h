#ifndef VOICE_H
#define VOICE_H

#include "ADSR.h"
#include <stdbool.h>

#define NUM_VOICES 8
#define A4_MIDI_NOTE 69
#define A4_FREQUENCY 440.0

#define FMOSC_0_BASE_ADDR XPAR_FMOSC_0_S_AXI_CONTROL_BASEADDR
#define FMOSC_1_BASE_ADDR XPAR_FMOSC_1_S_AXI_CONTROL_BASEADDR
#define FMOSC_2_BASE_ADDR XPAR_FMOSC_2_S_AXI_CONTROL_BASEADDR
#define FMOSC_3_BASE_ADDR XPAR_FMOSC_3_S_AXI_CONTROL_BASEADDR
#define FMOSC_4_BASE_ADDR XPAR_FMOSC_4_S_AXI_CONTROL_BASEADDR
#define FMOSC_5_BASE_ADDR XPAR_FMOSC_5_S_AXI_CONTROL_BASEADDR
#define FMOSC_6_BASE_ADDR XPAR_FMOSC_6_S_AXI_CONTROL_BASEADDR
#define FMOSC_7_BASE_ADDR XPAR_FMOSC_7_S_AXI_CONTROL_BASEADDR

#define CAR_FREQ_REG 0x10
#define CAR_GAIN_REG 0x18
#define MOD_FREQ_REG 0x20
#define MOD_GAIN_REG 0x28
#define MOD_INDEX_REG 0x30


uint32_t fmosc_base[NUM_VOICES] = {
		FMOSC_0_BASE_ADDR,
		FMOSC_1_BASE_ADDR,
		FMOSC_2_BASE_ADDR,
		FMOSC_3_BASE_ADDR,
		FMOSC_4_BASE_ADDR,
		FMOSC_5_BASE_ADDR,
		FMOSC_6_BASE_ADDR,
		FMOSC_7_BASE_ADDR
};

float global_indx, global_ratio;


float midi_to_freq(int midi_note) {
    return A4_FREQUENCY * pow(2.0, (midi_note - A4_MIDI_NOTE) / 12.0);
}

void write_indx(float val){
	for(int i = 0; i < NUM_VOICES; i++){
	Xil_Out32(fmosc_base[i] + MOD_INDEX_REG, *(uint32_t*)&val);
	}
}

void write_car_freq(int i, float val){
	Xil_Out32(fmosc_base[i] + CAR_FREQ_REG, *(uint32_t*)&val);
}

void write_mod_freq(int i, float val){
	Xil_Out32(fmosc_base[i] + MOD_FREQ_REG, *(uint32_t*)&val);
}

void write_car_gain(int i, float val){
	Xil_Out32(fmosc_base[i] + CAR_GAIN_REG, *(uint32_t*)&val);
}

void write_mod_gain(int i, float val){
	Xil_Out32(fmosc_base[i] + MOD_GAIN_REG, *(uint32_t*)&val);
}



typedef struct{
	ADSR adsr;
	ADSR modAdsr;
	float carFreq;
	float modFreq;
	bool gate;
	float velocity;
	bool active;
} Voice;

Voice voices[NUM_VOICES];

void init_voices() {
    for (int i = 0; i < NUM_VOICES; i++) {
        ADSR_init(&voices[i].adsr, 48000.0f, ADSR_LINEAR);
        ADSR_setParams(&voices[i].adsr, 0.1, 0.9, 0.7, 1);

        ADSR_init(&voices[i].modAdsr, 48000.0f, ADSR_LINEAR);
        ADSR_setParams(&voices[i].modAdsr, 0.01, 0.1, 0.1, 0);

        voices[i].carFreq = 0.0f;
        voices[i].modFreq = 0.0f;
        voices[i].velocity = 0.0f;
        voices[i].gate = false;
        voices[i].active = false;
    }
}

void set_amp_env(float a, float d, float s, float r){
	for(int i = 0; i < NUM_VOICES; i++){
        ADSR_setParams(&voices[i].adsr, a, d, s, r);

	}
}

void set_mod_env(float a, float d, float s, float r){
	for(int i = 0; i < NUM_VOICES; i++){
        ADSR_setParams(&voices[i].modAdsr, a, d, s, r);

	}
}


void set_amp_env_mode(ADSR_Mode mode){
	for(int i = 0; i < NUM_VOICES; i++){
		ADSR_setMode(&voices[i].adsr, mode);
	}
}

void set_mod_env_mode(ADSR_Mode mode){
	for(int i = 0; i < NUM_VOICES; i++){
		ADSR_setMode(&voices[i].modAdsr, mode);
	}
}


int allocate_voice(){
	for(int i = 0;i < NUM_VOICES; i++){
		if(!voices[i].active){
			return i;
		}
	}
	return -1;
}

void note_on(int note, float velocity, float ratio) {
    int voice_index = allocate_voice();
    if (voice_index != -1) {
        voices[voice_index].carFreq = midi_to_freq(note);
        voices[voice_index].modFreq = voices[voice_index].carFreq * ratio;
        voices[voice_index].velocity = velocity;
        voices[voice_index].gate = true;
        voices[voice_index].active = true;
        ADSR_reset(&voices[voice_index].adsr);
        ADSR_reset(&voices[voice_index].modAdsr);
        write_car_freq(voice_index, voices[voice_index].carFreq);
        write_mod_freq(voice_index, voices[voice_index].modFreq);
    }
}


void note_off(int note){
	for(int i = 0; i < NUM_VOICES; i++){
		if (voices[i].carFreq == midi_to_freq(note) && voices[i].active){
			voices[i].gate = false;

		}
	}
}

void process_voices() {
    for (int i = 0; i < NUM_VOICES; i++) {
        if (voices[i].active) {
            float env_value = ADSR_process(&voices[i].adsr, voices[i].gate);
            float mod_env_value = ADSR_process(&voices[i].modAdsr, voices[i].gate);
            float gain = voices[i].velocity * env_value;

            write_car_gain(i, gain);
            write_mod_gain(i, mod_env_value);

            if (voices[i].adsr.state == ADSR_IDLE) {
                voices[i].active = false;
            }
        }
    }
}



#endif
