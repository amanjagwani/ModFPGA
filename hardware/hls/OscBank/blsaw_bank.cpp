#include "hls_stream.h"
#include <ap_axi_sdata.h>
#include <ap_int.h>
#include "wavetable.h"
#include <math.h>
#include "blsaw_bank.h"

#define NUM_PHASE_INPUTS 8 /
#define NUM_OUTPUTS 8

#define SAWTOOTH 0
#define SQUARE 1
#define TRIANGLE 2

#define NUM_TABLES 4
const float frequency_thresholds[NUM_TABLES - 1] = {440.0f, 880.0f, 1760.0f};

int get_table_index(float frequency)
{
    int table_index = 0;
    if (frequency >= frequency_thresholds[0])
        table_index = 1;
    if (frequency >= frequency_thresholds[1])
        table_index = 2;
    if (frequency >= frequency_thresholds[2])
        table_index = 3;
    return table_index;
}

float *select_table(float frequency, int waveform)
{
    int table_index = get_table_index(frequency);

    switch (waveform)
    {
    case SAWTOOTH:
        return sawtooth_tables[table_index];
    case SQUARE:
        return square_tables[table_index];
    case TRIANGLE:
        return triangle_table;
    default:
        return sawtooth_tables[table_index];
    }
}

float get_waveform_value(float phase, float frequency, int waveform)
{
    float *table = select_table(frequency, waveform);
    int index = int(phase);
    float frac = phase - index;
    float val1 = table[index];
    int index_next = (index + 1) & (LUT_SIZE - 1);
    float val2 = table[(index_next)];
    return val1 + (frac * (val2 - val1));
}

void process_phase_input(
    float freq,
    int waveform1,
    int waveform2,
    float gain,
    int channel,
    float det,
    stream &sample_left,
    stream &sample_right)
{
    static float phs_val[NUM_PHASE_INPUTS] = {0.0f};

    float phase_step = static_cast<float>(LUT_SIZE) * (freq / SAMPLE_RATE);

    float sample_val1 = get_waveform_value(phs_val[channel], freq * (1 + det), waveform1) * gain;
    float sample_val2 = get_waveform_value(phs_val[channel], freq * (1 - det), waveform2) * gain;
    int sample_int = static_cast<int>((sample_val1 + sample_val2) * AMPLITUDE_HALF);

    sample_left.data(27, 4) = sample_int;
    sample_left.data(3, 0) = 0;
    sample_left.id = 0;
    sample_left.keep = -1;
    sample_left.strb = -1;
    sample_left.user = 0;
    sample_left.last = 0;

    sample_right.data(27, 4) = sample_int;
    sample_right.data(3, 0) = 1;
    sample_right.id = 0;
    sample_right.keep = -1;
    sample_right.strb = -1;
    sample_right.user = 0;
    sample_right.last = 0;

    phs_val[channel] += phase_step;
    if (phs_val[channel] >= LUT_SIZE)
    {
        phs_val[channel] -= LUT_SIZE;
    }
    if (phs_val[channel] < 0)
    {
        phs_val[channel] += LUT_SIZE;
    }
}

void blosc_bank_det(
    float freqs[8],
    float gains[8],
    float detune,
    int waveform1,
    int waveform2,
    hls::stream<stream> &out1,
    hls::stream<stream> &out2,
    hls::stream<stream> &out3,
    hls::stream<stream> &out4,
    hls::stream<stream> &out5,
    hls::stream<stream> &out6,
    hls::stream<stream> &out7,
    hls::stream<stream> &out8)
{
#pragma HLS INTERFACE s_axilite port = freqs
#pragma HLS INTERFACE s_axilite port = gains
#pragma HLS INTERFACE s_axilite port = waveform1
#pragma HLS INTERFACE s_axilite port = waveform2
#pragma HLS INTERFACE s_axilite port = detune
#pragma HLS INTERFACE axis port = out1
#pragma HLS INTERFACE axis port = out2
#pragma HLS INTERFACE axis port = out3
#pragma HLS INTERFACE axis port = out4
#pragma HLS INTERFACE axis port = out5
#pragma HLS INTERFACE axis port = out6
#pragma HLS INTERFACE axis port = out7
#pragma HLS INTERFACE axis port = out8
#pragma HLS INTERFACE ap_ctrl_none port = return

    stream left_samples[8], right_samples[8];
#pragma HLS ARRAY_PARTITION variable = left_samples complete
#pragma HLS ARRAY_PARTITION variable = right_samples complete

    for (int i = 0; i < 8; i++)
    {
#pragma HLS PIPELINE II = 10
        process_phase_input(freqs[i], waveform1, waveform2, gains[i], i, detune,
                            left_samples[i], right_samples[i]);
    }

    out1.write(left_samples[0]);
    out1.write(right_samples[0]);
    out2.write(left_samples[1]);
    out2.write(right_samples[1]);
    out3.write(left_samples[2]);
    out3.write(right_samples[2]);
    out4.write(left_samples[3]);
    out4.write(right_samples[3]);
    out5.write(left_samples[4]);
    out5.write(right_samples[4]);
    out6.write(left_samples[5]);
    out6.write(right_samples[5]);
    out7.write(left_samples[6]);
    out7.write(right_samples[6]);
    out8.write(left_samples[7]);
    out8.write(right_samples[7]);
}
