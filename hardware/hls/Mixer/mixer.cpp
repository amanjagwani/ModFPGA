#include "hls_stream.h"
#include <ap_axi_sdata.h>
#include <ap_int.h>

typedef ap_axis<32, 2, 4, 1> stream;

#define CHANNELS 2

void mixer(hls::stream<stream> &ch1, hls::stream<stream> &ch2, hls::stream<stream> &ch3, hls::stream<stream> &ch4, hls::stream<stream> &out, volatile float &gain)
{
#pragma HLS INTERFACE axis port = ch1
#pragma HLS INTERFACE axis port = ch2
#pragma HLS INTERFACE axis port = ch3
#pragma HLS INTERFACE axis port = ch4
#pragma HLS INTERFACE axis port = out
#pragma HLS INTERFACE s_axilite port = gain
#pragma HLS INTERFACE ap_ctrl_none port = return

    float local_gain = 0.25;

    while (true)
    {

        local_gain = gain;

        for (int c = 0; c < CHANNELS; c++)
        {
            ap_int<24> data_ch1 = ch1.read().data(27, 4);
            ap_int<24> data_ch2 = ch2.read().data(27, 4);
            ap_int<24> data_ch3 = ch3.read().data(27, 4);
            ap_int<24> data_ch4 = ch4.read().data(27, 4);

            ap_int<24> mixed_sample = (data_ch1 + data_ch2 + data_ch3 + data_ch4) * local_gain;

            stream mixed_out;
            mixed_out.data(27, 4) = mixed_sample;

            mixed_out.data(3, 0) = c == 0 ? 1 : 2;
            mixed_out.id = c;

            mixed_out.keep = -1;
            mixed_out.strb = -1;
            mixed_out.user = 0;
            mixed_out.last = 0;

            out.write(mixed_out);
        }
    }
}
