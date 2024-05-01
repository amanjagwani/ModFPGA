#include "hls_stream.h"
#include <ap_axi_sdata.h>
#include <ap_int.h>
#include "wavetables.h"
#include <math.h>


#define PI 3.14159265
#define AMPLITUDE ((1 << 23) - 1)
#define CHANNELS 2
#define SAMPLE_RATE 48000.0
#define LUT_SIZE 4096

typedef ap_axis<32, 2, 4, 1> stream;

void blsaw_bank(float freqs[NUM_PHASE_INPUTS], int waveforms[NUM_PHASE_INPUTS], float gains[NUM_PHASE_INPUTS],
                hls::stream<stream>& phase1, hls::stream<stream>& out1,
                hls::stream<stream>& phase2, hls::stream<stream>& out2,
                hls::stream<stream>& phase3, hls::stream<stream>& out3,
				hls::stream<stream>& phase4, hls::stream<stream>& out4
);
