// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2023 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XFOURPOLE_H
#define XFOURPOLE_H

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
#include "xfourpole_hw.h"

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
} XFourpole_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XFourpole;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XFourpole_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XFourpole_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XFourpole_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XFourpole_ReadReg(BaseAddress, RegOffset) \
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
int XFourpole_Initialize(XFourpole *InstancePtr, u16 DeviceId);
XFourpole_Config* XFourpole_LookupConfig(u16 DeviceId);
int XFourpole_CfgInitialize(XFourpole *InstancePtr, XFourpole_Config *ConfigPtr);
#else
int XFourpole_Initialize(XFourpole *InstancePtr, const char* InstanceName);
int XFourpole_Release(XFourpole *InstancePtr);
#endif


u32 XFourpole_Get_coeffs_BaseAddress(XFourpole *InstancePtr);
u32 XFourpole_Get_coeffs_HighAddress(XFourpole *InstancePtr);
u32 XFourpole_Get_coeffs_TotalBytes(XFourpole *InstancePtr);
u32 XFourpole_Get_coeffs_BitWidth(XFourpole *InstancePtr);
u32 XFourpole_Get_coeffs_Depth(XFourpole *InstancePtr);
u32 XFourpole_Write_coeffs_Words(XFourpole *InstancePtr, int offset, word_type *data, int length);
u32 XFourpole_Read_coeffs_Words(XFourpole *InstancePtr, int offset, word_type *data, int length);
u32 XFourpole_Write_coeffs_Bytes(XFourpole *InstancePtr, int offset, char *data, int length);
u32 XFourpole_Read_coeffs_Bytes(XFourpole *InstancePtr, int offset, char *data, int length);

#ifdef __cplusplus
}
#endif

#endif
