#include "butter_filt.h"

void butterworth(hls::stream<stream> &in, hls::stream<stream> &out, volatile float &a0,
				 volatile float &a1, volatile float &a2, volatile float &b1, volatile float &b2)
{
#pragma HLS INTERFACE axis port = in
#pragma HLS INTERFACE axis port = out
#pragma HLS INTERFACE s_axilite port = a0
#pragma HLS INTERFACE s_axilite port = a1
#pragma HLS INTERFACE s_axilite port = a2
#pragma HLS INTERFACE s_axilite port = b1
#pragma HLS INTERFACE s_axilite port = b2
#pragma HLS INTERFACE ap_ctrl_none port = return

	static float y_del[CHANNELS][2] = {0};
	static float x_del[CHANNELS][2] = {0};

	const float def_a0 = 0.003916, def_a1 = 0.007832, def_a2 = 0.007832, def_b1 = -1.815341, def_b2 = 0.831006;
	float temp_a0 = def_a0, temp_a1 = def_a1, temp_a2 = def_a2, temp_b1 = def_b1, temp_b2 = def_b2;
	float local_a0 = def_a0, local_a1 = def_a1, local_a2 = def_a2, local_b1 = def_b1, local_b2 = def_b2;
	bool a0_recvd = false, a1_recvd = false, a2_recvd = false, b1_recvd = false, b2_recvd = false;
	while (!in.empty())
	{

		if (a0 != temp_a0)
		{
			temp_a0 = a0;
			a0_recvd = true;
		}

		if (a1 != temp_a1)
		{
			temp_a1 = a1;
			a1_recvd = true;
		}

		if (a2 != temp_a2)
		{
			temp_a2 = a2;
			a2_recvd = true;
		}

		if (b1 != temp_b1)
		{
			temp_b1 = b1;
			b1_recvd = true;
		}

		if (b2 != temp_b2)
		{
			temp_b2 = b2;
			b2_recvd = true;
		}

		if (a0_recvd && a1_recvd && a2_recvd && b1_recvd && b2_recvd)
		{
			local_a0 = temp_a0;
			local_a1 = temp_a1;
			local_a2 = temp_a2;
			local_b1 = temp_b1;
			local_b2 = temp_b2;

			a0_recvd = false;
			a1_recvd = false;
			a2_recvd = false;
			b1_recvd = false;
			b2_recvd = false;
		}

		for (int c = 0; c < CHANNELS; c++)
		{
			ap_int<24> data_in = in.read().data(27, 4);
			float in_sample = static_cast<float>(data_in) * (1.0f / ((1 << 23) - 1));

			float out_sample = (local_a0 * in_sample) + (local_a1 * x_del[c][0]) + (local_a2 * x_del[c][1]) - (local_b1 * y_del[c][0]) - (local_b2 * y_del[c][1]);
			x_del[c][1] = x_del[c][0];
			x_del[c][0] = in_sample;

			y_del[c][1] = y_del[c][0];
			y_del[c][0] = out_sample;

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
