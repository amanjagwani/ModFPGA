/* Adpated from Zynq Book Example */

#include "audio.h"
/* Global variable for the IIC */
XIicPs Iic;

unsigned char IicConfig(unsigned int DeviceIdPS)
{
    XIicPs_Config *Config;
    int Status;

    Config = XIicPs_LookupConfig(DeviceIdPS);
    if (NULL == Config)
    {
        return XST_FAILURE;
    }

    Status = XIicPs_CfgInitialize(&Iic, Config, Config->BaseAddress);
    if (Status != XST_SUCCESS)
    {
        return XST_FAILURE;
    }

    Status = XIicPs_SelfTest(&Iic);
    if (Status != XST_SUCCESS)
    {
        xil_printf("IIC FAILED \r\n");
        return XST_FAILURE;
    }
    xil_printf("IIC Passed\r\n");

    Status = XIicPs_SetSClk(&Iic, IIC_SCLK_RATE);
    if (Status != XST_SUCCESS)
    {
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

void AudioPllConfig()
{

    AudioWriteToReg(R15_SOFTWARE_RESET, 0b000000000);
    usleep(75000);
    AudioWriteToReg(R6_POWER_MANAGEMENT, 0b000110000);
    AudioWriteToReg(R0_LEFT_CHANNEL_ADC_INPUT_VOLUME, 0b001010111);
    AudioWriteToReg(R1_RIGHT_CHANNEL_ADC_INPUT_VOLUME, 0b001010111);
    AudioWriteToReg(R2_LEFT_CHANNEL_DAC_VOLUME, 0b001111001);
    AudioWriteToReg(R3_RIGHT_CHANNEL_DAC_VOLUME, 0b001111001);
    AudioWriteToReg(R4_ANALOG_AUDIO_PATH, 0b000010010);
    AudioWriteToReg(R5_DIGITAL_AUDIO_PATH, 0b000000000);
    AudioWriteToReg(R7_DIGITAL_AUDIO_I_F, 0b000001010);
    AudioWriteToReg(R8_SAMPLING_RATE, 0b000000000);
    // AudioWriteToReg(R18_NOISE_GATE,						 0b000000001);
    usleep(75000);
    AudioWriteToReg(R9_ACTIVE, 0b000000001);
    AudioWriteToReg(R6_POWER_MANAGEMENT, 0b001100111);
}

void AudioWriteToReg(u8 u8RegAddr, u16 u16Data)
{

    unsigned char u8TxData[2];

    u8TxData[0] = u8RegAddr << 1;
    u8TxData[0] = u8TxData[0] | ((u16Data >> 8) & 0b1);

    u8TxData[1] = u16Data & 0xFF;

    XIicPs_MasterSendPolled(&Iic, u8TxData, 2, IIC_SLAVE_ADDR);
    while (XIicPs_BusIsBusy(&Iic))
        ;
}

void init_audio_codec()
{
    xil_printf("Initializing Audio Codec\n\r");

    unsigned int status = IicConfig(XPAR_XIICPS_0_DEVICE_ID);
    if (status != XST_SUCCESS)
    {
        xil_printf("I2C configuration failed\n\r");
        return;
    }

    AudioPllConfig();

    xil_printf("Audio Codec initialized\n\r");
}
