// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2023 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xreverbsc.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XReverbsc_CfgInitialize(XReverbsc *InstancePtr, XReverbsc_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XReverbsc_Set_fb(XReverbsc *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XReverbsc_WriteReg(InstancePtr->Control_BaseAddress, XREVERBSC_CONTROL_ADDR_FB_DATA, Data);
}

u32 XReverbsc_Get_fb(XReverbsc *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XReverbsc_ReadReg(InstancePtr->Control_BaseAddress, XREVERBSC_CONTROL_ADDR_FB_DATA);
    return Data;
}

void XReverbsc_Set_cf(XReverbsc *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XReverbsc_WriteReg(InstancePtr->Control_BaseAddress, XREVERBSC_CONTROL_ADDR_CF_DATA, Data);
}

u32 XReverbsc_Get_cf(XReverbsc *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XReverbsc_ReadReg(InstancePtr->Control_BaseAddress, XREVERBSC_CONTROL_ADDR_CF_DATA);
    return Data;
}

void XReverbsc_Set_mix(XReverbsc *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XReverbsc_WriteReg(InstancePtr->Control_BaseAddress, XREVERBSC_CONTROL_ADDR_MIX_DATA, Data);
}

u32 XReverbsc_Get_mix(XReverbsc *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XReverbsc_ReadReg(InstancePtr->Control_BaseAddress, XREVERBSC_CONTROL_ADDR_MIX_DATA);
    return Data;
}

