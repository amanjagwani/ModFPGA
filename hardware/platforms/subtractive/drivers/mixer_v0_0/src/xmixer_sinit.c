// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2023 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xmixer.h"

extern XMixer_Config XMixer_ConfigTable[];

XMixer_Config *XMixer_LookupConfig(u16 DeviceId) {
	XMixer_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XMIXER_NUM_INSTANCES; Index++) {
		if (XMixer_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XMixer_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XMixer_Initialize(XMixer *InstancePtr, u16 DeviceId) {
	XMixer_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XMixer_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XMixer_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

