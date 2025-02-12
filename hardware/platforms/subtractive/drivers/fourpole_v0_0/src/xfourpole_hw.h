// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2023 Xilinx, Inc. All Rights Reserved.
// ==============================================================
// control
// 0x20 ~
// 0x3f : Memory 'coeffs' (6 * 32b)
//        Word n : bit [31:0] - coeffs[n]
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XFOURPOLE_CONTROL_ADDR_COEFFS_BASE 0x20
#define XFOURPOLE_CONTROL_ADDR_COEFFS_HIGH 0x3f
#define XFOURPOLE_CONTROL_WIDTH_COEFFS     32
#define XFOURPOLE_CONTROL_DEPTH_COEFFS     6

