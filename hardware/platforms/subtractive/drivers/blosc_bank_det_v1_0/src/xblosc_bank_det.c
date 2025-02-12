// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2023 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xblosc_bank_det.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XBlosc_bank_det_CfgInitialize(XBlosc_bank_det *InstancePtr, XBlosc_bank_det_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XBlosc_bank_det_Set_detune(XBlosc_bank_det *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XBlosc_bank_det_WriteReg(InstancePtr->Control_BaseAddress, XBLOSC_BANK_DET_CONTROL_ADDR_DETUNE_DATA, Data);
}

u32 XBlosc_bank_det_Get_detune(XBlosc_bank_det *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XBlosc_bank_det_ReadReg(InstancePtr->Control_BaseAddress, XBLOSC_BANK_DET_CONTROL_ADDR_DETUNE_DATA);
    return Data;
}

void XBlosc_bank_det_Set_waveform1(XBlosc_bank_det *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XBlosc_bank_det_WriteReg(InstancePtr->Control_BaseAddress, XBLOSC_BANK_DET_CONTROL_ADDR_WAVEFORM1_DATA, Data);
}

u32 XBlosc_bank_det_Get_waveform1(XBlosc_bank_det *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XBlosc_bank_det_ReadReg(InstancePtr->Control_BaseAddress, XBLOSC_BANK_DET_CONTROL_ADDR_WAVEFORM1_DATA);
    return Data;
}

void XBlosc_bank_det_Set_waveform2(XBlosc_bank_det *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XBlosc_bank_det_WriteReg(InstancePtr->Control_BaseAddress, XBLOSC_BANK_DET_CONTROL_ADDR_WAVEFORM2_DATA, Data);
}

u32 XBlosc_bank_det_Get_waveform2(XBlosc_bank_det *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XBlosc_bank_det_ReadReg(InstancePtr->Control_BaseAddress, XBLOSC_BANK_DET_CONTROL_ADDR_WAVEFORM2_DATA);
    return Data;
}

u32 XBlosc_bank_det_Get_freqs_BaseAddress(XBlosc_bank_det *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XBLOSC_BANK_DET_CONTROL_ADDR_FREQS_BASE);
}

u32 XBlosc_bank_det_Get_freqs_HighAddress(XBlosc_bank_det *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XBLOSC_BANK_DET_CONTROL_ADDR_FREQS_HIGH);
}

u32 XBlosc_bank_det_Get_freqs_TotalBytes(XBlosc_bank_det *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XBLOSC_BANK_DET_CONTROL_ADDR_FREQS_HIGH - XBLOSC_BANK_DET_CONTROL_ADDR_FREQS_BASE + 1);
}

u32 XBlosc_bank_det_Get_freqs_BitWidth(XBlosc_bank_det *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XBLOSC_BANK_DET_CONTROL_WIDTH_FREQS;
}

u32 XBlosc_bank_det_Get_freqs_Depth(XBlosc_bank_det *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XBLOSC_BANK_DET_CONTROL_DEPTH_FREQS;
}

u32 XBlosc_bank_det_Write_freqs_Words(XBlosc_bank_det *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XBLOSC_BANK_DET_CONTROL_ADDR_FREQS_HIGH - XBLOSC_BANK_DET_CONTROL_ADDR_FREQS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Control_BaseAddress + XBLOSC_BANK_DET_CONTROL_ADDR_FREQS_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XBlosc_bank_det_Read_freqs_Words(XBlosc_bank_det *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XBLOSC_BANK_DET_CONTROL_ADDR_FREQS_HIGH - XBLOSC_BANK_DET_CONTROL_ADDR_FREQS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Control_BaseAddress + XBLOSC_BANK_DET_CONTROL_ADDR_FREQS_BASE + (offset + i)*4);
    }
    return length;
}

u32 XBlosc_bank_det_Write_freqs_Bytes(XBlosc_bank_det *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XBLOSC_BANK_DET_CONTROL_ADDR_FREQS_HIGH - XBLOSC_BANK_DET_CONTROL_ADDR_FREQS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Control_BaseAddress + XBLOSC_BANK_DET_CONTROL_ADDR_FREQS_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XBlosc_bank_det_Read_freqs_Bytes(XBlosc_bank_det *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XBLOSC_BANK_DET_CONTROL_ADDR_FREQS_HIGH - XBLOSC_BANK_DET_CONTROL_ADDR_FREQS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Control_BaseAddress + XBLOSC_BANK_DET_CONTROL_ADDR_FREQS_BASE + offset + i);
    }
    return length;
}

u32 XBlosc_bank_det_Get_gains_BaseAddress(XBlosc_bank_det *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XBLOSC_BANK_DET_CONTROL_ADDR_GAINS_BASE);
}

u32 XBlosc_bank_det_Get_gains_HighAddress(XBlosc_bank_det *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XBLOSC_BANK_DET_CONTROL_ADDR_GAINS_HIGH);
}

u32 XBlosc_bank_det_Get_gains_TotalBytes(XBlosc_bank_det *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XBLOSC_BANK_DET_CONTROL_ADDR_GAINS_HIGH - XBLOSC_BANK_DET_CONTROL_ADDR_GAINS_BASE + 1);
}

u32 XBlosc_bank_det_Get_gains_BitWidth(XBlosc_bank_det *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XBLOSC_BANK_DET_CONTROL_WIDTH_GAINS;
}

u32 XBlosc_bank_det_Get_gains_Depth(XBlosc_bank_det *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XBLOSC_BANK_DET_CONTROL_DEPTH_GAINS;
}

u32 XBlosc_bank_det_Write_gains_Words(XBlosc_bank_det *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XBLOSC_BANK_DET_CONTROL_ADDR_GAINS_HIGH - XBLOSC_BANK_DET_CONTROL_ADDR_GAINS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Control_BaseAddress + XBLOSC_BANK_DET_CONTROL_ADDR_GAINS_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XBlosc_bank_det_Read_gains_Words(XBlosc_bank_det *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XBLOSC_BANK_DET_CONTROL_ADDR_GAINS_HIGH - XBLOSC_BANK_DET_CONTROL_ADDR_GAINS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Control_BaseAddress + XBLOSC_BANK_DET_CONTROL_ADDR_GAINS_BASE + (offset + i)*4);
    }
    return length;
}

u32 XBlosc_bank_det_Write_gains_Bytes(XBlosc_bank_det *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XBLOSC_BANK_DET_CONTROL_ADDR_GAINS_HIGH - XBLOSC_BANK_DET_CONTROL_ADDR_GAINS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Control_BaseAddress + XBLOSC_BANK_DET_CONTROL_ADDR_GAINS_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XBlosc_bank_det_Read_gains_Bytes(XBlosc_bank_det *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XBLOSC_BANK_DET_CONTROL_ADDR_GAINS_HIGH - XBLOSC_BANK_DET_CONTROL_ADDR_GAINS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Control_BaseAddress + XBLOSC_BANK_DET_CONTROL_ADDR_GAINS_BASE + offset + i);
    }
    return length;
}

