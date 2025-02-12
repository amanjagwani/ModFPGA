#include "FourPole.h"

// k is r * 4
float filter(float in, double *d, double *g, double a, float k)
{
	float out, u, w;
	float ss = d[3];
	for (int j = 0; j < 3; j++)
		ss += d[j] * g[2 - j];
	out = (g[3] * in + ss) / (1 + k * g[3]);
	u = g[0] * (in - k * out);
	for (int j = 0; j < 3; j++)
	{
		w = d[j] + u;
		d[j] = u - a * w;
		u = g[0] * w;
	}
	d[3] = g[0] * w - a * out;
	return out;
}

void fourpole(hls::stream<stream> &in, hls::stream<stream> &out, volatile double &g0, volatile double &g1,
			  volatile double &g2, volatile double &g3, volatile double &a, volatile float &k)
{
#pragma HLS INTERFACE axis port = in
#pragma HLS INTERFACE axis port = out
#pragma HLS INTERFACE s_axilite port = g0
#pragma HLS INTERFACE s_axilite port = g1
#pragma HLS INTERFACE s_axilite port = g2
#pragma HLS INTERFACE s_axilite port = g3
#pragma HLS INTERFACE s_axilite port = a
#pragma HLS INTERFACE s_axilite port = k
#pragma HLS INTERFACE ap_ctrl_none port = return

	static double del[CHANNELS][4] = {0};
	static double G[4] = {0};

	const float def_g0 = 0.0316989, def_g1 = 0.00100482, def_g2 = 3.18517e-05, def_g3 = 1.00966e-06, def_a = -0.936602;
	double temp_g0 = def_g0, temp_g1 = def_g1, temp_g2 = def_g2, temp_g3 = def_g3, temp_a = def_a;
	double local_g0 = def_g0, local_g1 = def_g1, local_g2 = def_g2, local_g3 = def_g3, local_a = def_a;
	float local_k;
	bool g0_recvd = false, g1_recvd = false, g2_recvd = false, g3_recvd = false, a_recvd = false;
	while (!in.empty())
	{
		// #pragma HLS LOOP_TRIPCOUNT max=1
#pragma HLS PIPELINE off

		local_k = k;
		if (g0 != temp_g0)
		{
			temp_g0 = g0;
			g0_recvd = true;
		}

		if (g1 != temp_g1)
		{
			temp_g1 = g1;
			g1_recvd = true;
		}

		if (g2 != temp_g2)
		{
			temp_g2 = g2;
			g2_recvd = true;
		}

		if (g3 != temp_g3)
		{
			temp_g3 = g3;
			g3_recvd = true;
		}

		if (a != temp_a)
		{
			temp_a = a;
			a_recvd = true;
		}

		if (g0_recvd && g1_recvd && g2_recvd && g3_recvd && a_recvd)
		{
			local_g0 = temp_g0;
			local_g1 = temp_g1;
			local_g2 = temp_g2;
			local_g3 = temp_g3;
			local_a = temp_a;

			g0_recvd = false;
			g1_recvd = false;
			g2_recvd = false;
			g3_recvd = false;
			a_recvd = false;
		}

		G[0] = local_g0;
		G[1] = local_g1;
		G[2] = local_g2;
		G[3] = local_g3;

		for (int c = 0; c < CHANNELS; c++)
		{
			ap_int<24> data_in = in.read().data(27, 4);
			float in_sample = static_cast<float>(data_in) * (1.0f / ((1 << 23) - 1));

			float out_sample = filter(in_sample, del[c], G, local_a, local_k);
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
