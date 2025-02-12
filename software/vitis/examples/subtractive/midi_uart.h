#ifndef MIDI_UART_H
#define MIDI_UART_H

#include "config.h"
#include "xuartps.h"
#include "voice.h"
#include "reverb.h"
#include "fourpole_eight_voice.h"

#define UART_DEVICE_ID XPAR_XUARTPS_0_DEVICE_ID
#define UART_BAUD_RATE 31250

XUartPs Uart_PS;
XUartPs_Config *uConfig;
int uStatus;

void init_midi_uart()
{
	uConfig = XUartPs_LookupConfig(UART_DEVICE_ID);
	if (uConfig == NULL)
	{
		xil_printf("Error finding UART configuration\n\r");
		return;
	}

	uStatus = XUartPs_CfgInitialize(&Uart_PS, uConfig, uConfig->BaseAddress);
	if (uStatus != XST_SUCCESS)
	{
		xil_printf("Error initializing UART\n\r");
		return;
	}

	XUartPs_SetBaudRate(&Uart_PS, UART_BAUD_RATE);
	xil_printf("UART Initialized \n");
}

void process_midi_message(XUartPs *uart_instance)
{
	static uint8_t running_status = 0;
	static uint8_t data_bytes[2] = {0};
	static uint8_t data_count = 0;

	while (XUartPs_IsReceiveData(uart_instance->Config.BaseAddress))
	{
		uint8_t byte = XUartPs_ReadReg(uart_instance->Config.BaseAddress, XUARTPS_FIFO_OFFSET);

		if (byte & 0x80)
		{

			if ((byte >= 0x80) && (byte < 0xF0))
			{
				running_status = byte;
				data_count = 0;
			}
		}
		else
		{
			if (running_status == 0)
				continue;

			data_bytes[data_count++] = byte;

			int expected = 0;
			switch (running_status & 0xF0)
			{
			case 0x80:
			case 0x90:
			case 0xB0:
			case 0xE0:
				expected = 2;
				break;
			case 0xC0:
			case 0xD0:
				expected = 1;
				break;
			default:
				expected = 0;
				break;
			}

			if (expected > 0 && data_count >= expected)
			{
				uint8_t status = running_status & 0xF0;
				if (status == 0x90)
				{
					int note = data_bytes[0];
					int velocity = data_bytes[1];
					if (velocity > 0)
					{
						float amp = map_range(velocity, 0, 127, 0.000f, 0.300f);
						note_on(note, amp);
					}
					else
					{
						note_off(note);
					}
				}
				else if (status == 0x80)
				{
					int note = data_bytes[0];
					note_off(note);
				}
				else if (status == 0xB0)
				{
					int cc_number = data_bytes[0];
					int cc_value = data_bytes[1];

					if (cc_number == 74)
					{
						float normalized_value = (float)cc_value / 127.0f;
						float min_log_freq = log(20.0f);
						float max_log_freq = log(20000.0f);
						synth_state.cutoff_freq = exp(min_log_freq + normalized_value * (max_log_freq - min_log_freq));
						update_fp_coeffs(synth_state.cutoff_freq, synth_state.resonance);
					}
					else if (cc_number == 71)
					{
						synth_state.resonance = map_range(cc_value, 0, 127, 0, 0.9);
						update_fp_coeffs(synth_state.cutoff_freq, synth_state.resonance);
					}
					else if (cc_number == 76)
					{
						synth_state.filt_amt = map_range(cc_value, 0, 127, 0, 0.99);
						synth_state.filt_adsr[0] = map_range(cc_value, 0, 127, 0.0000001, 10.0000000);
						set_filt_env(synth_state.filt_adsr[0],
									 synth_state.filt_adsr[1],
									 synth_state.filt_adsr[2],
									 synth_state.filt_adsr[3]);
					}
					else if (cc_number == 80)
					{
						synth_state.adsr[0] = map_range(cc_value, 0, 127, 0.00000001, 10.000000);
						set_amp_env(synth_state.adsr[0],
									synth_state.adsr[1],
									synth_state.adsr[2],
									synth_state.adsr[3]);
					}
					else if (cc_number == 81)
					{
						synth_state.adsr[1] = map_range(cc_value, 0, 127, 0.0000001, 10.000000);
						set_amp_env(synth_state.adsr[0],
									synth_state.adsr[1],
									synth_state.adsr[2],
									synth_state.adsr[3]);
					}
					else if (cc_number == 82)
					{
						synth_state.adsr[2] = map_range(cc_value, 0, 127, 0.0000001, 1.000);
						set_amp_env(synth_state.adsr[0],
									synth_state.adsr[1],
									synth_state.adsr[2],
									synth_state.adsr[3]);
					}
					else if (cc_number == 83)
					{
						synth_state.adsr[3] = map_range(cc_value, 0, 127, 0.0000001, 5.000000);
						set_amp_env(synth_state.adsr[0],
									synth_state.adsr[1],
									synth_state.adsr[2],
									synth_state.adsr[3]);
					}
					else if (cc_number == 77)
					{
						synth_state.filt_adsr[1] = map_range(cc_value, 0, 127, 0.0000001, 10.0000000);
						set_filt_env(synth_state.filt_adsr[0],
									 synth_state.filt_adsr[1],
									 synth_state.filt_adsr[2],
									 synth_state.filt_adsr[3]);
						synth_state.rev_mix = map_range(cc_value, 0, 127, 0, 1);
						set_mix(VERB_BASE_ADDR, synth_state.rev_mix);
					}
					else if (cc_number == 78)
					{
						synth_state.filt_adsr[2] = map_range(cc_value, 0, 127, 0.00000001, 1.0000);
						set_filt_env(synth_state.filt_adsr[0],
									 synth_state.filt_adsr[1],
									 synth_state.filt_adsr[2],
									 synth_state.filt_adsr[3]);
					}
					else if (cc_number == 22)
					{
						synth_state.rev_cf = map_range(cc_value, 0, 127, 60, 20000);
						set_cf(VERB_BASE_ADDR, synth_state.rev_cf);
					}
					else if (cc_number == 23)
					{
						synth_state.rev_fb = map_range(cc_value, 0, 127, 0, 1);
						set_fb(VERB_BASE_ADDR, synth_state.rev_fb);
					}
				}
				data_count = 0;
			}
		}
	}
}

void process_midi_message_usb_ascii(XUartPs *uart_instance)
{
	static char buffer[100];
	static int buffer_index = 0;
	uint8_t data;

	if (XUartPs_IsReceiveData(uart_instance->Config.BaseAddress))
	{
		data = XUartPs_ReadReg(uart_instance->Config.BaseAddress, XUARTPS_FIFO_OFFSET);

		if (data == '\n' || data == '\r')
		{
			buffer[buffer_index] = '\0';

			int note, velocity, cc_number, cc_value;

			if (sscanf(buffer, "Note On: %d, Velocity: %d", &note, &velocity) == 2)
			{
				if (velocity > 0)
				{
					note_on(note, map_range(velocity, 0, 127, 0.000, 0.300));
				}
			}
			else if (sscanf(buffer, "Note Off: %d, Velocity: %d", &note, &velocity) == 2)
			{
				note_off(note);
			}
			else if (sscanf(buffer, "Control Change: Controller: %d, Value: %d", &cc_number, &cc_value) == 2)
			{
				if (cc_number == 74)
				{
					float normalized_value = (float)cc_value / 127.0f;
					float min_log_freq = log(20.0f);
					float max_log_freq = log(20000.0f);
					synth_state.cutoff_freq = exp(min_log_freq + normalized_value * (max_log_freq - min_log_freq));
					update_fp_coeffs(synth_state.cutoff_freq, synth_state.resonance);
				}
				if (cc_number == 71)
				{
					synth_state.resonance = map_range(cc_value, 0, 127, 0, 0.9);
					update_fp_coeffs(synth_state.cutoff_freq, synth_state.resonance);
				}
				if (cc_number == 76)
				{
					synth_state.filt_amt = map_range(cc_value, 0, 127, 0, 0.99);
				}
				if (cc_number == 80)
				{
					synth_state.adsr[0] = map_range(cc_value, 0, 127, 0.00000001, 10.000000);
					set_amp_env(synth_state.adsr[0], synth_state.adsr[1], synth_state.adsr[2], synth_state.adsr[3]);
				}
				if (cc_number == 81)
				{
					synth_state.adsr[1] = map_range(cc_value, 0, 127, 0.0000001, 10.000000);
					set_amp_env(synth_state.adsr[0], synth_state.adsr[1], synth_state.adsr[2], synth_state.adsr[3]);
				}
				if (cc_number == 82)
				{
					synth_state.adsr[2] = map_range(cc_value, 0, 127, 0.0000001, 1.000);
					set_amp_env(synth_state.adsr[0], synth_state.adsr[1], synth_state.adsr[2], synth_state.adsr[3]);
				}
				if (cc_number == 83)
				{
					synth_state.adsr[3] = map_range(cc_value, 0, 127, 0.0000001, 5.000000);
					set_amp_env(synth_state.adsr[0], synth_state.adsr[1], synth_state.adsr[2], synth_state.adsr[3]);
				}
				if (cc_number == 76)
				{
					synth_state.filt_adsr[0] = map_range(cc_value, 0, 127, 0.0000001, 10.0000000);
					set_filt_env(synth_state.filt_adsr[0], synth_state.filt_adsr[1], synth_state.filt_adsr[2], synth_state.filt_adsr[3]);
				}
				if (cc_number == 77)
				{
					synth_state.filt_adsr[1] = map_range(cc_value, 0, 127, 0.0000001, 10.0000000);
					set_filt_env(synth_state.filt_adsr[0], synth_state.filt_adsr[1], synth_state.filt_adsr[2], synth_state.filt_adsr[3]);
				}
				if (cc_number == 78)
				{
					synth_state.filt_adsr[2] = map_range(cc_value, 0, 127, 0.00000001, 1.0000);
					set_filt_env(synth_state.filt_adsr[0], synth_state.filt_adsr[1], synth_state.filt_adsr[2], synth_state.filt_adsr[3]);
				}
				if (cc_number == 22)
				{
					synth_state.rev_cf = map_range(cc_value, 0, 127, 60, 20000);
					set_cf(VERB_BASE_ADDR, synth_state.rev_cf);
				}
				if (cc_number == 23)
				{
					synth_state.rev_fb = map_range(cc_value, 0, 127, 0, 1);
					set_fb(VERB_BASE_ADDR, synth_state.rev_fb);
				}
				if (cc_number == 77)
				{
					synth_state.rev_mix = map_range(cc_value, 0, 127, 0, 1);
					set_mix(VERB_BASE_ADDR, synth_state.rev_mix);
				}
			}

			buffer_index = 0;
		}
		else
		{
			buffer[buffer_index++] = data;
			if (buffer_index >= sizeof(buffer) - 1)
				buffer_index = 0;
		}
	}
}

#endif
