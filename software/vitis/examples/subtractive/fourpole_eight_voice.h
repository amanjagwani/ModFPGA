#ifndef FOURPOLE_EIGHT_H
#define FOURPOLE_EIGHT_H

#include "config.h"

#define FP0_BASE_ADDR XPAR_FOURPOLE_0_S_AXI_CONTROL_BASEADDR
#define FP0_COEFFS_REG 0x40000020

#define FP1_BASE_ADDR XPAR_FOURPOLE_2_S_AXI_CONTROL_BASEADDR
#define FP1_COEFFS_REG 0x40020020

#define FP2_BASE_ADDR XPAR_FOURPOLE_3_S_AXI_CONTROL_BASEADDR
#define FP2_COEFFS_REG 0x40030020

#define FP3_BASE_ADDR XPAR_FOURPOLE_4_S_AXI_CONTROL_BASEADDR
#define FP3_COEFFS_REG 0x40040020

#define FP4_BASE_ADDR XPAR_FOURPOLE_5_S_AXI_CONTROL_BASEADDR
#define FP4_COEFFS_REG 0x40050020

#define FP5_BASE_ADDR XPAR_FOURPOLE_6_S_AXI_CONTROL_BASEADDR
#define FP5_COEFFS_REG 0x40060020

#define FP6_BASE_ADDR XPAR_FOURPOLE_7_S_AXI_CONTROL_BASEADDR
#define FP6_COEFFS_REG 0x40070020

#define FP7_BASE_ADDR XPAR_FOURPOLE_8_S_AXI_CONTROL_BASEADDR
#define FP7_COEFFS_REG 0x40080020

typedef struct
{
    float g0;
    float g1;
    float g2;
    float g3;
    float g4;
    float a;
} coefficients;

uint32_t fp_coeffs_base[NUM_VOICES] = {
    FP0_COEFFS_REG,
    FP1_COEFFS_REG,
    FP2_COEFFS_REG,
    FP3_COEFFS_REG,
    FP4_COEFFS_REG,
    FP5_COEFFS_REG,
    FP6_COEFFS_REG,
    FP7_COEFFS_REG};

void coeff_calc(float f, coefficients *coeffs, float *of, double ts)
{
    float w = tan(f * ts);
    coeffs->g0 = w / (1 + w);
    coeffs->a = (w - 1) / (1 + w);
    coeffs->g1 = coeffs->g0 * coeffs->g0;
    coeffs->g2 = coeffs->g0 * coeffs->g1;
    coeffs->g3 = coeffs->g0 * coeffs->g2;
    *of = f;
}

void update_fp_coeffs_voice(float f, float r, int voice_index)
{
    coefficients coeffstruct;
    float of;
    float k = r * 4;
    coeff_calc(f, &coeffstruct, &of, piosr);

    float coeff_array[6] = {coeffstruct.g0, coeffstruct.g1, coeffstruct.g2, coeffstruct.g3, coeffstruct.a, k};

    write_array_float(coeff_array, fp_coeffs_base[voice_index], 6);
}

void process_cutoff(float amount)
{
    float filt_add = (20000 - synth_state.cutoff_freq) * amount;

    for (int i = 0; i < NUM_VOICES; i++)
    {
        if (voices[i].active)
        {
            float filt_env = ADSR_process(&voices[i].filtAdsr, voices[i].gate);
            float new_cutoff = synth_state.cutoff_freq + (filt_add * filt_env);

            update_fp_coeffs_voice(new_cutoff, synth_state.resonance, i);
        }
    }
}

void update_fp_coeffs(float f, float r)
{
    coefficients coeffstruct;
    float of;
    float k = r * 4;
    coeff_calc(f, &coeffstruct, &of, piosr);

    float coeff_array[6] = {coeffstruct.g0, coeffstruct.g1, coeffstruct.g2, coeffstruct.g3, coeffstruct.a, k};
    write_array_float(coeff_array, FP0_COEFFS_REG, 6);
    write_array_float(coeff_array, FP1_COEFFS_REG, 6);
    write_array_float(coeff_array, FP2_COEFFS_REG, 6);
    write_array_float(coeff_array, FP3_COEFFS_REG, 6);
    write_array_float(coeff_array, FP4_COEFFS_REG, 6);
    write_array_float(coeff_array, FP5_COEFFS_REG, 6);
    write_array_float(coeff_array, FP6_COEFFS_REG, 6);
    write_array_float(coeff_array, FP7_COEFFS_REG, 6);
}

#endif
