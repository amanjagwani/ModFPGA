#include "FourPole.h"

// k is r * 4
float filter(float in, float *d, float *g, float a, float k)
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

void fourpole(hls::stream<stream> &in, hls::stream<stream> &out, float coeffs[6])
{
#pragma HLS INTERFACE axis port = in
#pragma HLS INTERFACE axis port = out
#pragma HLS INTERFACE s_axilite port = coeffs
#pragma HLS INTERFACE ap_ctrl_none port = return

	static float del[CHANNELS][4] = {0};
	static float G[4] = {0};

	while (!in.empty())
	{
#pragma HLS LOOP_TRIPCOUNT max = 1
#pragma HLS PIPELINE off

		G[0] = coeffs[0];
		G[1] = coeffs[1];
		G[2] = coeffs[2];
		G[3] = coeffs[3];

		for (int c = 0; c < CHANNELS; c++)
		{
			ap_int<24> data_in = in.read().data(27, 4);
			float in_sample = static_cast<float>(data_in) * (1.0f / ((1 << 23) - 1));

			float out_sample = filter(in_sample, del[c], G, coeffs[4], coeffs[5]);
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