#include "hls_stream.h"
#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <math.h>

#define PI 3.14159265
#define AMPLITUDE ((1 << 23) - 1)
#define AMPLITUDE_HALF (((1 << 23) - 1) / 2)
#define CHANNELS 2
#define SAMPLE_RATE 48000.0

typedef ap_axis<32, 2, 4, 1> stream;

void process_phase_input(
    float freq,
    int waveform1,
    int waveform2,
    float gain,
    int channel,
    float det,
    stream &sample_left,
    stream &sample_right);

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
    hls::stream<stream> &out8);
