#include "hls_stream.h"
#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <math.h>
#include "phase_gen.h"



static float phase = 0.0;
typedef ap_axis<32, 2, 4, 1> stream;



void phase_gen(volatile float &freq, hls::stream<stream> &out) {
    #pragma HLS INTERFACE s_axilite port=freq
    #pragma HLS INTERFACE axis port=out
    #pragma HLS INTERFACE ap_ctrl_none port=return

    static float phase = 0.0;
    float local_freq;

   while(true){
    local_freq = freq;
    float phase_step = float((LUT_SIZE) * (local_freq / SAMPLE_RATE));

    phase += phase_step;
    if (phase >= LUT_SIZE) {
        phase -= LUT_SIZE;
    }
    if (phase < 0) {
        phase += LUT_SIZE;
    }

    ap_uint<24> scaled_phase = static_cast<ap_uint<24>>((phase / LUT_SIZE) * ((1 << 24) - 1));
    stream sample_out;
    sample_out.data = scaled_phase;
    sample_out.keep = -1;
    sample_out.strb = -1;
    sample_out.last = 0;
    out.write(sample_out);

    }
}
