#include <math.h>
#include "hls_stream.h"
#include <ap_axi_sdata.h>
#include <ap_int.h>

#define CHANNELS 2
typedef ap_axis<32, 2, 4, 1> stream;

void butterworth(hls::stream<stream> &in, hls::stream<stream> &out, volatile float &a0,
				 volatile float &a1, volatile float &a2, volatile float &b1, volatile float &b2);

#define SR 48000
