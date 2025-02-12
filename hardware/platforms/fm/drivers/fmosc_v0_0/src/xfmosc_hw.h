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
// 0x10 : Data signal of car_freq
//        bit 31~0 - car_freq[31:0] (Read/Write)
// 0x14 : reserved
// 0x18 : Data signal of car_gain
//        bit 31~0 - car_gain[31:0] (Read/Write)
// 0x1c : reserved
// 0x20 : Data signal of mod_freq
//        bit 31~0 - mod_freq[31:0] (Read/Write)
// 0x24 : reserved
// 0x28 : Data signal of mod_gain
//        bit 31~0 - mod_gain[31:0] (Read/Write)
// 0x2c : reserved
// 0x30 : Data signal of modulation_index
//        bit 31~0 - modulation_index[31:0] (Read/Write)
// 0x34 : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XFMOSC_CONTROL_ADDR_CAR_FREQ_DATA         0x10
#define XFMOSC_CONTROL_BITS_CAR_FREQ_DATA         32
#define XFMOSC_CONTROL_ADDR_CAR_GAIN_DATA         0x18
#define XFMOSC_CONTROL_BITS_CAR_GAIN_DATA         32
#define XFMOSC_CONTROL_ADDR_MOD_FREQ_DATA         0x20
#define XFMOSC_CONTROL_BITS_MOD_FREQ_DATA         32
#define XFMOSC_CONTROL_ADDR_MOD_GAIN_DATA         0x28
#define XFMOSC_CONTROL_BITS_MOD_GAIN_DATA         32
#define XFMOSC_CONTROL_ADDR_MODULATION_INDEX_DATA 0x30
#define XFMOSC_CONTROL_BITS_MODULATION_INDEX_DATA 32

