/* Adpated from Zynq Book Example */

#ifndef __AUDIO_H_
#define __AUDIO_H_

#include "xiicps.h"
#include "xparameters.h"
#include "xil_printf.h"
#include "sleep.h"

/* Redefine audio controller base address from xparameters.h */
#define AUDIO_BASE XPAR_ZYBO_AUDIO_CTRL_0_BASEADDR

/* Slave address for the SSM audio controller */
#define IIC_SLAVE_ADDR 0b0011010

/* I2C Serial Clock frequency in Hertz */
#define IIC_SCLK_RATE 100000

/* SSM internal registers */
enum audio_regs
{
    R0_LEFT_CHANNEL_ADC_INPUT_VOLUME = 0x00,
    R1_RIGHT_CHANNEL_ADC_INPUT_VOLUME = 0x01,
    R2_LEFT_CHANNEL_DAC_VOLUME = 0x02,
    R3_RIGHT_CHANNEL_DAC_VOLUME = 0x03,
    R4_ANALOG_AUDIO_PATH = 0x04,
    R5_DIGITAL_AUDIO_PATH = 0x05,
    R6_POWER_MANAGEMENT = 0x06,
    R7_DIGITAL_AUDIO_I_F = 0x07,
    R8_SAMPLING_RATE = 0x08,
    R9_ACTIVE = 0x09,
    R15_SOFTWARE_RESET = 0x0F,
    R16_ALC_CONTROL_1 = 0x10,
    R17_ALC_CONTROL_2 = 0x11,
    R18_NOISE_GATE = 0x12,
};

/* Prototype Functions */
unsigned char IicConfig(unsigned int DeviceIdPS);
void AudioPllConfig();
void AudioWriteToReg(u8 u8RegAddr, u16 u16Data);
void init_audio_codec();

#endif
