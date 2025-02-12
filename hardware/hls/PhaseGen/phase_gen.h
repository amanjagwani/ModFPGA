#include "hls_stream.h"
#include <ap_axi_sdata.h>
#include <ap_int.h>



#define PI 3.14159265
#define AMPLITUDE ((1 << 23) - 1)
#define CHANNELS 2
#define LUT_SIZE 4096
#define SAMPLE_RATE 48000.0

typedef ap_axis<32, 2, 4, 1> stream;


void phase_gen(volatile float &freq, volatile float &detune, hls::stream<stream> &out);
