// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2023 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xmixer.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XMixer_CfgInitialize(XMixer *InstancePtr, XMixer_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XMixer_Set_gain(XMixer *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMixer_WriteReg(InstancePtr->Control_BaseAddress, XMIXER_CONTROL_ADDR_GAIN_DATA, Data);
}

u32 XMixer_Get_gain(XMixer *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMixer_ReadReg(InstancePtr->Control_BaseAddress, XMIXER_CONTROL_ADDR_GAIN_DATA);
    return Data;
}

