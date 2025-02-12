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
// 0x10 : Data signal of detune
//        bit 31~0 - detune[31:0] (Read/Write)
// 0x14 : reserved
// 0x18 : Data signal of waveform1
//        bit 31~0 - waveform1[31:0] (Read/Write)
// 0x1c : reserved
// 0x60 : Data signal of waveform2
//        bit 31~0 - waveform2[31:0] (Read/Write)
// 0x64 : reserved
// 0x20 ~
// 0x3f : Memory 'freqs' (8 * 32b)
//        Word n : bit [31:0] - freqs[n]
// 0x40 ~
// 0x5f : Memory 'gains' (8 * 32b)
//        Word n : bit [31:0] - gains[n]
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XBLOSC_BANK_DET_CONTROL_ADDR_DETUNE_DATA    0x10
#define XBLOSC_BANK_DET_CONTROL_BITS_DETUNE_DATA    32
#define XBLOSC_BANK_DET_CONTROL_ADDR_WAVEFORM1_DATA 0x18
#define XBLOSC_BANK_DET_CONTROL_BITS_WAVEFORM1_DATA 32
#define XBLOSC_BANK_DET_CONTROL_ADDR_WAVEFORM2_DATA 0x60
#define XBLOSC_BANK_DET_CONTROL_BITS_WAVEFORM2_DATA 32
#define XBLOSC_BANK_DET_CONTROL_ADDR_FREQS_BASE     0x20
#define XBLOSC_BANK_DET_CONTROL_ADDR_FREQS_HIGH     0x3f
#define XBLOSC_BANK_DET_CONTROL_WIDTH_FREQS         32
#define XBLOSC_BANK_DET_CONTROL_DEPTH_FREQS         8
#define XBLOSC_BANK_DET_CONTROL_ADDR_GAINS_BASE     0x40
#define XBLOSC_BANK_DET_CONTROL_ADDR_GAINS_HIGH     0x5f
#define XBLOSC_BANK_DET_CONTROL_WIDTH_GAINS         32
#define XBLOSC_BANK_DET_CONTROL_DEPTH_GAINS         8

