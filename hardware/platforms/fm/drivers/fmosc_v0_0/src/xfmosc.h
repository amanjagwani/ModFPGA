// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2023 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XFMOSC_H
#define XFMOSC_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xfmosc_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
    u16 DeviceId;
    u64 Control_BaseAddress;
} XFmosc_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XFmosc;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XFmosc_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XFmosc_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XFmosc_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XFmosc_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XFmosc_Initialize(XFmosc *InstancePtr, u16 DeviceId);
XFmosc_Config* XFmosc_LookupConfig(u16 DeviceId);
int XFmosc_CfgInitialize(XFmosc *InstancePtr, XFmosc_Config *ConfigPtr);
#else
int XFmosc_Initialize(XFmosc *InstancePtr, const char* InstanceName);
int XFmosc_Release(XFmosc *InstancePtr);
#endif


void XFmosc_Set_car_freq(XFmosc *InstancePtr, u32 Data);
u32 XFmosc_Get_car_freq(XFmosc *InstancePtr);
void XFmosc_Set_car_gain(XFmosc *InstancePtr, u32 Data);
u32 XFmosc_Get_car_gain(XFmosc *InstancePtr);
void XFmosc_Set_mod_freq(XFmosc *InstancePtr, u32 Data);
u32 XFmosc_Get_mod_freq(XFmosc *InstancePtr);
void XFmosc_Set_mod_gain(XFmosc *InstancePtr, u32 Data);
u32 XFmosc_Get_mod_gain(XFmosc *InstancePtr);
void XFmosc_Set_modulation_index(XFmosc *InstancePtr, u32 Data);
u32 XFmosc_Get_modulation_index(XFmosc *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
