// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2023 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xfourpole.h"

extern XFourpole_Config XFourpole_ConfigTable[];

XFourpole_Config *XFourpole_LookupConfig(u16 DeviceId) {
	XFourpole_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XFOURPOLE_NUM_INSTANCES; Index++) {
		if (XFourpole_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XFourpole_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XFourpole_Initialize(XFourpole *InstancePtr, u16 DeviceId) {
	XFourpole_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XFourpole_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XFourpole_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

