// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2023 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xfourpole.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XFourpole_CfgInitialize(XFourpole *InstancePtr, XFourpole_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

u32 XFourpole_Get_coeffs_BaseAddress(XFourpole *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XFOURPOLE_CONTROL_ADDR_COEFFS_BASE);
}

u32 XFourpole_Get_coeffs_HighAddress(XFourpole *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XFOURPOLE_CONTROL_ADDR_COEFFS_HIGH);
}

u32 XFourpole_Get_coeffs_TotalBytes(XFourpole *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XFOURPOLE_CONTROL_ADDR_COEFFS_HIGH - XFOURPOLE_CONTROL_ADDR_COEFFS_BASE + 1);
}

u32 XFourpole_Get_coeffs_BitWidth(XFourpole *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XFOURPOLE_CONTROL_WIDTH_COEFFS;
}

u32 XFourpole_Get_coeffs_Depth(XFourpole *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XFOURPOLE_CONTROL_DEPTH_COEFFS;
}

u32 XFourpole_Write_coeffs_Words(XFourpole *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XFOURPOLE_CONTROL_ADDR_COEFFS_HIGH - XFOURPOLE_CONTROL_ADDR_COEFFS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Control_BaseAddress + XFOURPOLE_CONTROL_ADDR_COEFFS_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XFourpole_Read_coeffs_Words(XFourpole *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XFOURPOLE_CONTROL_ADDR_COEFFS_HIGH - XFOURPOLE_CONTROL_ADDR_COEFFS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Control_BaseAddress + XFOURPOLE_CONTROL_ADDR_COEFFS_BASE + (offset + i)*4);
    }
    return length;
}

u32 XFourpole_Write_coeffs_Bytes(XFourpole *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XFOURPOLE_CONTROL_ADDR_COEFFS_HIGH - XFOURPOLE_CONTROL_ADDR_COEFFS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Control_BaseAddress + XFOURPOLE_CONTROL_ADDR_COEFFS_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XFourpole_Read_coeffs_Bytes(XFourpole *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XFOURPOLE_CONTROL_ADDR_COEFFS_HIGH - XFOURPOLE_CONTROL_ADDR_COEFFS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Control_BaseAddress + XFOURPOLE_CONTROL_ADDR_COEFFS_BASE + offset + i);
    }
    return length;
}

