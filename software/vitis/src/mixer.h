#ifndef MIXER_H
#define MIXER_H

#include "config.h"

#define MIXER_BASE_ADDR XPAR_MIXER_0_S_AXI_CONTROL_BASEADDR
#define MIXER_GAIN_REG 0x10

void set_mixer_gain(int baseaddr, float gain)
{
    Xil_Out32(baseaddr + MIXER_GAIN_REG, *(uint32_t *)&gain);
}

#endif
