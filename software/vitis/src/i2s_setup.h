#ifndef I2S_SETUP_H
#define I2S_SETUP_H

#include "config.h"
#include "xi2stx.h"

XI2s_Tx I2sInstance;

XI2stx_Config *Config;
int Status;

void init_i2s_transmitter()
{
	xil_printf("Initializing I2S Transmitter\n\r");

	Config = XI2s_Tx_LookupConfig(XPAR_I2S_TRANSMITTER_0_DEVICE_ID);
	if (Config == NULL)
	{
		return;
	}

	Status = XI2s_Tx_CfgInitialize(&I2sInstance, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS)
	{
		return;
	}

	XI2s_Tx_SetSclkOutDiv(&I2sInstance, 256 * I2S_FS, I2S_FS);

	Status = XI2s_Tx_SetChMux(&I2sInstance, XI2S_TX_CHID0, XI2S_TX_CHMUX_AXIS_01);
	if (Status != XST_SUCCESS)
	{
		return;
	}

	XI2s_Tx_Enable(&I2sInstance, TRUE);

	xil_printf("I2s Transmitter initialized\n\r");
}

#endif
