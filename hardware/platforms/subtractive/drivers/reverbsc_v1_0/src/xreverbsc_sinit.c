// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2023 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xreverbsc.h"

extern XReverbsc_Config XReverbsc_ConfigTable[];

XReverbsc_Config *XReverbsc_LookupConfig(u16 DeviceId) {
	XReverbsc_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XREVERBSC_NUM_INSTANCES; Index++) {
		if (XReverbsc_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XReverbsc_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XReverbsc_Initialize(XReverbsc *InstancePtr, u16 DeviceId) {
	XReverbsc_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XReverbsc_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XReverbsc_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

