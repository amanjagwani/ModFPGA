#include "hls_stream.h"
#include <ap_axi_sdata.h>
#include <ap_int.h>

typedef ap_axis<32, 2, 4, 1> stream;

#define CHANNELS 2

void reson(hls::stream<stream> &in, hls::stream<stream> &out,
		   volatile float &scale, volatile float &b0, volatile float &b1, volatile float &a)
{
#pragma HLS INTERFACE axis port = in
#pragma HLS INTERFACE axis port = out
#pragma HLS INTERFACE s_axilite port = scale
#pragma HLS INTERFACE s_axilite port = b0
#pragma HLS INTERFACE s_axilite port = b1
#pragma HLS INTERFACE s_axilite port = a
#pragma HLS INTERFACE ap_ctrl_none port = return
	// #pragma HLS bind_storage variable=del type=ram_s2p impl=bram

	static float del[CHANNELS][2] = {0};
	static float in1 = 0;
	const float DEFAULT_SCALE = 0.061739;
	const float DEFAULT_B0 = -1.618850;
	const float DEFAULT_B1 = 0.877306;
	const float DEFAULT_A = -1.0;
	bool scl_recvd = false, b0_recvd = false, b1_recvd = false, a_recvd = false;
	float temp_scl = DEFAULT_SCALE, temp_b0 = DEFAULT_B0, temp_b1 = DEFAULT_B1, temp_a = DEFAULT_A;
	float local_scale = DEFAULT_SCALE, local_b0 = DEFAULT_B0, local_b1 = DEFAULT_B1, local_a = DEFAULT_A;
	while (true)
	{
		if (!in.empty())
		{
			if (scale != temp_scl)
			{
				temp_scl = scale;
				scl_recvd = true;
			}

			if (b0 != temp_b0)
			{
				temp_b0 = b0;
				b0_recvd = true;
			}

			if (b1 != temp_b1)
			{
				temp_b1 = b1;
				b1_recvd = true;
			}

			if (a != temp_a)
			{
				temp_a = a;
				a_recvd = true;
			}

			if (scl_recvd && b0_recvd && b1_recvd && a_recvd)
			{
				local_scale = temp_scl;
				local_b0 = temp_b0;
				local_b1 = temp_b1;
				local_a = temp_a;

				scl_recvd = false;
				b0_recvd = false;
				b1_recvd = false;
				a_recvd = false;
			}

			for (int c = 0; c < CHANNELS; c++)
			{
				ap_int<24> data_in = in.read().data(27, 4);
				float in_sample = static_cast<float>(data_in) * (1.0f / ((1 << 23) - 1));

				float out_sample = (in_sample * local_scale) - (local_b0 * del[c][0]) - (local_b1 * del[c][1]);
				float out_sample = w + (local_a * del[c][1]);

				del[c][1] = del[c][0];
				del[c][0] = out_sample;

				ap_int<24> out_data = static_cast<ap_int<24>>(out_sample * ((1 << 23) - 1));

				stream out_stream;
				out_stream.data(27, 4) = out_data;

				out_stream.data(3, 0) = c == 0 ? 1 : 2;
				out_stream.id = c;

				out_stream.keep = -1;
				out_stream.strb = -1;
				out_stream.user = 0;
				out_stream.last = 0;

				out.write(out_stream);
			}
		}
	}
}
