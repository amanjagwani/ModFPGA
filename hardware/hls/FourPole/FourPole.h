#include <math.h>
#include "hls_stream.h"
#include <ap_axi_sdata.h>
#include <ap_int.h>

#define CHANNELS 2
#define SR 48000
#define PI 3.14159265359
#define piosr PI / SR
typedef ap_axis<32, 2, 4, 1> stream;

void fourpole(hls::stream<stream> &in, hls::stream<stream> &out, volatile double &g0, volatile double &g1,
			  volatile double &g2, volatile double &g3, volatile double &a, volatile float &k);
