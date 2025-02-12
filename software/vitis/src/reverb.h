#ifndef REVERB_H
#define REVERB_H

#include "config.h"

#define VERB_BASE_ADDR XPAR_REVERBSC_0_S_AXI_CONTROL_BASEADDR
#define VERB_CF_REG 0x18
#define VERB_FB_REG 0x10
#define VERB_MIX_REG 0x20

void set_fb(int baseaddr, float fb)
{
    Xil_Out32(baseaddr + VERB_FB_REG, *(uint32_t *)&fb);
}

void set_cf(int baseaddr, float cf)
{
    Xil_Out32(baseaddr + VERB_CF_REG, *(uint32_t *)&cf);
}

void set_mix(int baseaddr, float cf)
{
    Xil_Out32(baseaddr + VERB_MIX_REG, *(uint32_t *)&cf);
}

#endif
