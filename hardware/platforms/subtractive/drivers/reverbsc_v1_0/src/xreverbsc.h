// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2023 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XREVERBSC_H
#define XREVERBSC_H

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
#include "xreverbsc_hw.h"

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
} XReverbsc_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XReverbsc;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XReverbsc_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XReverbsc_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XReverbsc_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XReverbsc_ReadReg(BaseAddress, RegOffset) \
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
int XReverbsc_Initialize(XReverbsc *InstancePtr, u16 DeviceId);
XReverbsc_Config* XReverbsc_LookupConfig(u16 DeviceId);
int XReverbsc_CfgInitialize(XReverbsc *InstancePtr, XReverbsc_Config *ConfigPtr);
#else
int XReverbsc_Initialize(XReverbsc *InstancePtr, const char* InstanceName);
int XReverbsc_Release(XReverbsc *InstancePtr);
#endif


void XReverbsc_Set_fb(XReverbsc *InstancePtr, u32 Data);
u32 XReverbsc_Get_fb(XReverbsc *InstancePtr);
void XReverbsc_Set_cf(XReverbsc *InstancePtr, u32 Data);
u32 XReverbsc_Get_cf(XReverbsc *InstancePtr);
void XReverbsc_Set_mix(XReverbsc *InstancePtr, u32 Data);
u32 XReverbsc_Get_mix(XReverbsc *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
