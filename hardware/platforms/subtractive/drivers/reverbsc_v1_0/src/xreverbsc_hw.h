// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2023 Xilinx, Inc. All Rights Reserved.
// ==============================================================
// control
// 0x00 : reserved
// 0x04 : reserved
// 0x08 : reserved
// 0x0c : reserved
// 0x10 : Data signal of fb
//        bit 31~0 - fb[31:0] (Read/Write)
// 0x14 : reserved
// 0x18 : Data signal of cf
//        bit 31~0 - cf[31:0] (Read/Write)
// 0x1c : reserved
// 0x20 : Data signal of mix
//        bit 31~0 - mix[31:0] (Read/Write)
// 0x24 : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XREVERBSC_CONTROL_ADDR_FB_DATA  0x10
#define XREVERBSC_CONTROL_BITS_FB_DATA  32
#define XREVERBSC_CONTROL_ADDR_CF_DATA  0x18
#define XREVERBSC_CONTROL_BITS_CF_DATA  32
#define XREVERBSC_CONTROL_ADDR_MIX_DATA 0x20
#define XREVERBSC_CONTROL_BITS_MIX_DATA 32

