#include "FourPole.h"

typedef struct
{
	float g0;
	float g1;
	float g2;
	float g3;
	float g4;
	float a;
} coefficients;

// ts is piosr
void coeffs(float f, coefficients &coeffs, float &of, double ts)
{
	float w = std::tan(f * ts);
	coeffs.g0 = w / (1 + w);
	coeffs.a = (w - 1) / (1 + w);
	coeffs.g1 = coeffs.g0 * coeffs.g0;
	coeffs.g2 = coeffs.g0 * coeffs.g1;
	coeffs.g3 = coeffs.g0 * coeffs.g2;
	of = f;
}

void print_output(hls::stream<stream> &out_stream, int num_samples)
{

	for (int i = 0; i < num_samples; i++)
	{

		ap_int<24> data_in = out_stream.read().data(27, 4);
		float data_float = static_cast<float>(data_in) * (1.0f / ((1 << 23) - 1));
		std::cout << "Output sample" << ":" << data_float << std::endl;
	}
}

void noise_input(hls::stream<stream> &in_stream, int num_samples)
{
	for (int i = 0; i < num_samples; i++)
	{

		float noise = 2.0f * ((float)rand() / RAND_MAX) - 1.0f;
		ap_int<24> noise_fixed = static_cast<ap_int<24>>(noise * ((1 << 23) - 1));

		stream noise_sample;
		noise_sample.data(27, 4) = noise_fixed;
		noise_sample.data(3, 0) = 1;
		noise_sample.id = 0;
		noise_sample.keep = -1;
		noise_sample.strb = -1;
		noise_sample.user = 0;
		noise_sample.last = 0;

		in_stream.write(noise_sample);
	}
}

int main()
{

	hls::stream<stream> in, out;
	coefficients coeffstruct;
	float of = 0;
	float r = 0.3 * 4.0;

	coeffs(500, coeffstruct, of, piosr);
	int num_samples = 2000;

	noise_input(in, num_samples);
	float coeff_array[6] = {coeffstruct.g0, coeffstruct.g1, coeffstruct.g2, coeffstruct.g3, coeffstruct.a, r};
	for (int i = 0; i < num_samples; i++)
	{

		fourpole(in, out, coeff_array);
	}

	print_output(out, num_samples);

	return 0;
}
