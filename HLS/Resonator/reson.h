#include "hls_stream.h"
#include <ap_axi_sdata.h>
#include <ap_int.h>

typedef ap_axis<32, 2, 4, 1> stream;

#define CHANNELS 2

void reson(hls::stream<stream> &in, hls::stream<stream> &out,
           volatile float &scale, volatile float &b0, volatile float &b1, volatile float &a);