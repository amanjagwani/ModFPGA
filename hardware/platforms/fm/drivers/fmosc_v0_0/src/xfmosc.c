// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2023 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xfmosc.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XFmosc_CfgInitialize(XFmosc *InstancePtr, XFmosc_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XFmosc_Set_car_freq(XFmosc *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFmosc_WriteReg(InstancePtr->Control_BaseAddress, XFMOSC_CONTROL_ADDR_CAR_FREQ_DATA, Data);
}

u32 XFmosc_Get_car_freq(XFmosc *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFmosc_ReadReg(InstancePtr->Control_BaseAddress, XFMOSC_CONTROL_ADDR_CAR_FREQ_DATA);
    return Data;
}

void XFmosc_Set_car_gain(XFmosc *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFmosc_WriteReg(InstancePtr->Control_BaseAddress, XFMOSC_CONTROL_ADDR_CAR_GAIN_DATA, Data);
}

u32 XFmosc_Get_car_gain(XFmosc *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFmosc_ReadReg(InstancePtr->Control_BaseAddress, XFMOSC_CONTROL_ADDR_CAR_GAIN_DATA);
    return Data;
}

void XFmosc_Set_mod_freq(XFmosc *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFmosc_WriteReg(InstancePtr->Control_BaseAddress, XFMOSC_CONTROL_ADDR_MOD_FREQ_DATA, Data);
}

u32 XFmosc_Get_mod_freq(XFmosc *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFmosc_ReadReg(InstancePtr->Control_BaseAddress, XFMOSC_CONTROL_ADDR_MOD_FREQ_DATA);
    return Data;
}

void XFmosc_Set_mod_gain(XFmosc *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFmosc_WriteReg(InstancePtr->Control_BaseAddress, XFMOSC_CONTROL_ADDR_MOD_GAIN_DATA, Data);
}

u32 XFmosc_Get_mod_gain(XFmosc *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFmosc_ReadReg(InstancePtr->Control_BaseAddress, XFMOSC_CONTROL_ADDR_MOD_GAIN_DATA);
    return Data;
}

void XFmosc_Set_modulation_index(XFmosc *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFmosc_WriteReg(InstancePtr->Control_BaseAddress, XFMOSC_CONTROL_ADDR_MODULATION_INDEX_DATA, Data);
}

u32 XFmosc_Get_modulation_index(XFmosc *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFmosc_ReadReg(InstancePtr->Control_BaseAddress, XFMOSC_CONTROL_ADDR_MODULATION_INDEX_DATA);
    return Data;
}

