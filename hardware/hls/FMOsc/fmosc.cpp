#include "hls_stream.h"
#include <ap_axi_sdata.h>
#include <math.h>
#include <ap_int.h>
#include "sine_table.h"

#define PI 3.14159265
#define LUT_SIZE 4096
#define AMPLITUDE ((1 << 23) - 1)
#define CHANNELS 2
#define DEFAULT_FREQUENCY 440.0
#define SAMPLE_RATE 48000.0

typedef ap_axis<32, 2, 4, 1> stream;

float get_table(float phase)
{
    int index = int(phase);
    float frac = phase - index;
    float val1 = sine_table[index];
    float val2 = sine_table[(index + 1) % LUT_SIZE];
    return val1 + (frac * (val2 - val1));
}

void initialize_array()
{
#pragma HLS bind_storage variable = sine_table type = ram_1p impl = bram
}

void oscillator(float frequency, hls::stream<stream> &envelope, hls::stream<stream> &modulator, float modulation_index, hls::stream<stream> &out)
{
#pragma HLS INTERFACE axis port = out
#pragma HLS INTERFACE axis port = envelope
#pragma HLS INTERFACE axis port = modulator

    static float phase = 0.0;
    ap_uint<24> env_data = envelope.read().data(23, 0);

    ap_int<24> mod_data = modulator.read().data(27, 4);
    float mod_val = static_cast<float>(mod_data) * (1.0f / AMPLITUDE);
    float mod_frequency = frequency + (frequency * mod_val * modulation_index);
    float mod_phase_step = float(LUT_SIZE * mod_frequency / SAMPLE_RATE);

    for (int c = 0; c < CHANNELS; c++)
    {
        float osc_sample_float = AMPLITUDE * get_table(phase);
        ap_int<24> osc_sample = static_cast<ap_int<24>>(std::round(osc_sample_float * env_data / ((1 << 24) - 1)));

        stream sample;
        sample.data(27, 4) = osc_sample;
        sample.data(3, 0) = c == 0 ? 1 : 2;
        sample.id = c;
        sample.keep = -1;
        sample.strb = -1;
        sample.user = 0;
        sample.last = 0;

        out.write(sample);
    }

    phase += mod_phase_step;
    if (phase >= LUT_SIZE)
    {
        phase -= LUT_SIZE;
    }
    if (phase < 0)
    {
        phase += LUT_SIZE;
    }
}

void table_lookup_fmosc_sine(volatile float &freq, hls::stream<stream> &envelope, hls::stream<stream> &modulator, volatile float &modulation_index, hls::stream<stream> &out)
{
#pragma HLS INTERFACE axis port = out
#pragma HLS INTERFACE axis port = envelope
#pragma HLS INTERFACE axis port = modulator
#pragma HLS INTERFACE s_axilite port = freq
#pragma HLS INTERFACE s_axilite port = modulation_index
#pragma HLS INTERFACE ap_ctrl_none port = return

    initialize_array();

    float local_freq;
    float local_modulation_index;
    while (true)
    {
        local_freq = freq;
        local_modulation_index = modulation_index;
        oscillator(local_freq, envelope, modulator, local_modulation_index, out);
    }
}
