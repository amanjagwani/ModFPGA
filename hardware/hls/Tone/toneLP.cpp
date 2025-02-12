#include "hls_stream.h"
#include <ap_axi_sdata.h>
#include <ap_int.h>

typedef ap_axis<32, 2, 4, 1> stream;

#define CHANNELS 2

static float y_prev[CHANNELS] = {0.0};

void tone_filt(hls::stream<stream> &in, hls::stream<stream> &out,
               volatile float &a, volatile float &b)
{
#pragma HLS INTERFACE axis port = in
#pragma HLS INTERFACE axis port = out
#pragma HLS INTERFACE s_axilite port = a
#pragma HLS INTERFACE s_axilite port = b
#pragma HLS INTERFACE ap_ctrl_none port = return
#pragma HLS bind_storage variable = y_prev type = ram_s2p impl = bram

    float local_a, local_b;
    while (true)
    {
        local_a = a;
        local_b = b;

        for (int c = 0; c < CHANNELS; c++)
        {
            ap_int<24> data_in = in.read().data(27, 4);
            float in_sample = static_cast<float>(data_in) * (1.0f / ((1 << 23) - 1));

            float out_sample = (local_a * in_sample) + (local_b * y_prev[c]);
            y_prev[c] = out_sample;

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
