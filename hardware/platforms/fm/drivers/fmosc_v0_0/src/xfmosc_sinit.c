// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2023 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xfmosc.h"

extern XFmosc_Config XFmosc_ConfigTable[];

XFmosc_Config *XFmosc_LookupConfig(u16 DeviceId) {
	XFmosc_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XFMOSC_NUM_INSTANCES; Index++) {
		if (XFmosc_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XFmosc_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XFmosc_Initialize(XFmosc *InstancePtr, u16 DeviceId) {
	XFmosc_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XFmosc_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XFmosc_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

