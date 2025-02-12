#include "hls_stream.h"
#include <ap_axi_sdata.h>
#include <ap_int.h>
#include "wavetables.h"
#include <math.h>
#include "blsaw_bank.h"

#define NUM_PHASE_INPUTS 4
#define NUM_OUTPUTS 4
#define SAWTOOTH 0
#define SQUARE 1
#define TRIANGLE 2

#define NUM_RANGES 5
const float frequency_thresholds[NUM_RANGES] = {220.0f, 440.0f, 880.0f, 1760.0f, 20000.0f};

int get_table_index(float frequency)
{
    int table_index = 0;
    while (frequency >= frequency_thresholds[table_index] && table_index < NUM_RANGES - 1)
    {
        table_index++;
    }
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
        return triangle_tables[table_index];
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
    float val2 = table[(index + 1) % LUT_SIZE];
    return val1 + (frac * (val2 - val1));
}

void process_phase_input(hls::stream<stream> &phase, hls::stream<stream> &out,
                         float freq, int waveform, float gain)
{

    ap_uint<24> phs_data = phase.read().data(23, 0);
    float phs_val = static_cast<float>((phs_data * LUT_SIZE) / ((1 << 24) - 1));

    float sample_val_left = get_waveform_value(phs_val, freq, waveform) * gain;
    float sample_val_right = get_waveform_value(phs_val, freq, waveform) * gain;

    stream sample_left, sample_right;
    sample_left.data(27, 4) = static_cast<ap_uint<24>>(sample_val_left * AMPLITUDE);
    sample_left.data(3, 0) = 0;
    sample_left.id = 0;
    sample_left.keep = -1;
    sample_left.strb = -1;
    sample_left.user = 0;
    sample_left.last = 0;

    sample_right.data(27, 4) = static_cast<ap_uint<24>>(sample_val_right * AMPLITUDE);
    sample_right.data(3, 0) = 1;
    sample_right.id = 0;
    sample_right.keep = -1;
    sample_right.strb = -1;
    sample_right.user = 0;
    sample_right.last = 0;

    out.write(sample_left);
    out.write(sample_right);
}

void blsaw_bank(float freqs[NUM_PHASE_INPUTS], int waveforms[NUM_PHASE_INPUTS], float gains[NUM_PHASE_INPUTS],
                hls::stream<stream> &phase1, hls::stream<stream> &out1,
                hls::stream<stream> &phase2, hls::stream<stream> &out2,
                hls::stream<stream> &phase3, hls::stream<stream> &out3,
                hls::stream<stream> &phase4, hls::stream<stream> &out4)
{
#pragma HLS INTERFACE s_axilite port = freqs
#pragma HLS INTERFACE s_axilite port = waveforms
#pragma HLS INTERFACE s_axilite port = gains
#pragma HLS INTERFACE axis port = phase1
#pragma HLS INTERFACE axis port = out1
#pragma HLS INTERFACE axis port = phase2
#pragma HLS INTERFACE axis port = out2
#pragma HLS INTERFACE axis port = phase3
#pragma HLS INTERFACE axis port = out3
#pragma HLS INTERFACE axis port = phase4
#pragma HLS INTERFACE axis port = out4
#pragma HLS INTERFACE ap_ctrl_none port = return

    while (true)
    {
        process_phase_input(phase1, out1, freqs[0], waveforms[0], gains[0]);

        process_phase_input(phase2, out2, freqs[1], waveforms[1], gains[1]);

        process_phase_input(phase3, out3, freqs[2], waveforms[2], gains[2]);

        process_phase_input(phase4, out4, freqs[3], waveforms[3], gains[3]);
    }
}
