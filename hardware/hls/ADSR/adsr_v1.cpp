#include "hls_stream.h"
#include <ap_axi_sdata.h>
#include <ap_int.h>
#include "adsr.h"
#include <math.h>


#define SAMPLE_RATE 48000.0

typedef ap_axis<32, 2, 5, 6> stream;

void adsr_gen(hls::stream<stream>& out, volatile bool& gate, volatile float& attack, volatile float& decay, volatile float& sustain, volatile float& release){
#pragma HLS INTERFACE axis port=out
#pragma HLS INTERFACE s_axilite port=gate
#pragma HLS INTERFACE s_axilite port=attack
#pragma HLS INTERFACE s_axilite port=decay
#pragma HLS INTERFACE s_axilite port=sustain
#pragma HLS INTERFACE s_axilite port=release
#pragma HLS INTERFACE ap_ctrl_none port =return


static ADSR adsr(SAMPLE_RATE);

float local_atk, local_dec, local_sus, local_rel;
bool local_gate;


while(true){
	local_atk = attack;
	local_dec = decay;
	local_sus = sustain;
	local_rel = release;
	local_gate = gate;

	adsr.set_params(local_atk, local_dec, local_sus, local_rel);
	float sample = adsr.process(local_gate);
	stream sample_out;
	sample_out.data(23, 0) = static_cast<ap_uint<24>>(std::round(sample * ((1 << 24) - 1)));
	sample_out.keep = -1;
	sample_out.strb = -1;
	sample_out.last = 0;
	out.write(sample_out);



}


}
