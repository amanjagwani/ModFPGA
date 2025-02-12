// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2023 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XBLOSC_BANK_DET_H
#define XBLOSC_BANK_DET_H

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
#include "xblosc_bank_det_hw.h"

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
} XBlosc_bank_det_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XBlosc_bank_det;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XBlosc_bank_det_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XBlosc_bank_det_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XBlosc_bank_det_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XBlosc_bank_det_ReadReg(BaseAddress, RegOffset) \
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
int XBlosc_bank_det_Initialize(XBlosc_bank_det *InstancePtr, u16 DeviceId);
XBlosc_bank_det_Config* XBlosc_bank_det_LookupConfig(u16 DeviceId);
int XBlosc_bank_det_CfgInitialize(XBlosc_bank_det *InstancePtr, XBlosc_bank_det_Config *ConfigPtr);
#else
int XBlosc_bank_det_Initialize(XBlosc_bank_det *InstancePtr, const char* InstanceName);
int XBlosc_bank_det_Release(XBlosc_bank_det *InstancePtr);
#endif


void XBlosc_bank_det_Set_detune(XBlosc_bank_det *InstancePtr, u32 Data);
u32 XBlosc_bank_det_Get_detune(XBlosc_bank_det *InstancePtr);
void XBlosc_bank_det_Set_waveform1(XBlosc_bank_det *InstancePtr, u32 Data);
u32 XBlosc_bank_det_Get_waveform1(XBlosc_bank_det *InstancePtr);
void XBlosc_bank_det_Set_waveform2(XBlosc_bank_det *InstancePtr, u32 Data);
u32 XBlosc_bank_det_Get_waveform2(XBlosc_bank_det *InstancePtr);
u32 XBlosc_bank_det_Get_freqs_BaseAddress(XBlosc_bank_det *InstancePtr);
u32 XBlosc_bank_det_Get_freqs_HighAddress(XBlosc_bank_det *InstancePtr);
u32 XBlosc_bank_det_Get_freqs_TotalBytes(XBlosc_bank_det *InstancePtr);
u32 XBlosc_bank_det_Get_freqs_BitWidth(XBlosc_bank_det *InstancePtr);
u32 XBlosc_bank_det_Get_freqs_Depth(XBlosc_bank_det *InstancePtr);
u32 XBlosc_bank_det_Write_freqs_Words(XBlosc_bank_det *InstancePtr, int offset, word_type *data, int length);
u32 XBlosc_bank_det_Read_freqs_Words(XBlosc_bank_det *InstancePtr, int offset, word_type *data, int length);
u32 XBlosc_bank_det_Write_freqs_Bytes(XBlosc_bank_det *InstancePtr, int offset, char *data, int length);
u32 XBlosc_bank_det_Read_freqs_Bytes(XBlosc_bank_det *InstancePtr, int offset, char *data, int length);
u32 XBlosc_bank_det_Get_gains_BaseAddress(XBlosc_bank_det *InstancePtr);
u32 XBlosc_bank_det_Get_gains_HighAddress(XBlosc_bank_det *InstancePtr);
u32 XBlosc_bank_det_Get_gains_TotalBytes(XBlosc_bank_det *InstancePtr);
u32 XBlosc_bank_det_Get_gains_BitWidth(XBlosc_bank_det *InstancePtr);
u32 XBlosc_bank_det_Get_gains_Depth(XBlosc_bank_det *InstancePtr);
u32 XBlosc_bank_det_Write_gains_Words(XBlosc_bank_det *InstancePtr, int offset, word_type *data, int length);
u32 XBlosc_bank_det_Read_gains_Words(XBlosc_bank_det *InstancePtr, int offset, word_type *data, int length);
u32 XBlosc_bank_det_Write_gains_Bytes(XBlosc_bank_det *InstancePtr, int offset, char *data, int length);
u32 XBlosc_bank_det_Read_gains_Bytes(XBlosc_bank_det *InstancePtr, int offset, char *data, int length);

#ifdef __cplusplus
}
#endif

#endif
