#ifndef CONFIG_H
#define CONFIG_H

#include "xparameters.h"
#include "xil_printf.h"
#include "xil_io.h"
#include "xstatus.h"
#include "math.h"
#include "string.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "xtime_l.h"

#define I2S_FS 48
#define I2S_MCLK 256 * I2S_FS

#define SAMPLE_RATE 48000
#define PI 3.14159265
#define piosr PI / SAMPLE_RATE

typedef struct
{
    float gains[4];
    float adsr[4];
    float current_freq;
    float target_freq;
    float rev_cf;
    float rev_fb;
    float rev_mix;
    float portamento_time;
    float portamento_step;
    float freqs[4];
    bool gate;
    float velocity;
    float env_value;
    float cutoff_freq;
    float resonance;
    float filt_amt;
    float filt_adsr[4];
    float mod_index;
    float mod_ratio;
    float mod_adsr[4];
} synth;

synth synth_state;

void write_array(int *data, uint32_t base_addr, int size)
{
    for (int i = 0; i < size; i++)
    {
        Xil_Out32(base_addr + (i * sizeof(int)), data[i]);
    }
}

void write_array_uint(uint32_t *data, uint32_t base_addr, int size)
{
    for (int i = 0; i < size; i++)
    {
        Xil_Out32(base_addr + (i * sizeof(int)), data[i]);
    }
}

void write_array_float(float *data, uint32_t base_addr, int size)
{
    for (int i = 0; i < size; i++)
    {
        uint32_t int_rep = *(uint32_t *)&data[i];
        Xil_Out32(base_addr + (i * sizeof(float)), int_rep);
    }
}

float map_range(int value, int minIn, int maxIn, float minOut, float maxOut)
{
    return (float)(value - minIn) * (maxOut - minOut) / (float)(maxIn - minIn) + minOut;
}

#endif
