// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2023 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xblosc_bank_det.h"

extern XBlosc_bank_det_Config XBlosc_bank_det_ConfigTable[];

XBlosc_bank_det_Config *XBlosc_bank_det_LookupConfig(u16 DeviceId) {
	XBlosc_bank_det_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XBLOSC_BANK_DET_NUM_INSTANCES; Index++) {
		if (XBlosc_bank_det_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XBlosc_bank_det_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XBlosc_bank_det_Initialize(XBlosc_bank_det *InstancePtr, u16 DeviceId) {
	XBlosc_bank_det_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XBlosc_bank_det_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XBlosc_bank_det_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

