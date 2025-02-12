#include <fstream>
#include "hls_stream.h"
#include "ap_axi_sdata.h"
#include "blsaw_bank.h"

int main()
{
    const float freq = 60;
    const float gain = 1.0f;
    const float det = 0.0f;
    const int waveform1 = 0;
    const int waveform2 = 0;

    std::ofstream outfile("oscillator_cycle.txt");
    if (!outfile.is_open())
    {
        return -1;
    }

    for (int i = 0; i < 48000; i++)
    {
        stream sample_left, sample_right;

        process_phase_input(freq, waveform1, waveform2, gain, 0, det, sample_left, sample_right);

        ap_uint<32> data_val = sample_left.data;
        ap_uint<24> sample_int = data_val.range(27, 4);

        float sample_value = static_cast<float>(sample_int.to_uint()) / static_cast<float>(AMPLITUDE);

        outfile << sample_value << "\n";
    }

    outfile.close();
    return 0;
}
