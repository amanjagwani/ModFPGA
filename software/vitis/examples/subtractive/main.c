
#include "i2s_setup.h"
#include "audio.h"
#include "ADSR.h"
#include "voice.h"
#include "config.h"
#include "midi_uart.h"
#include "mixer.h"
#include "reverb.h"
#include "fourpole_eight_voice.h"

int main()
{

	init_i2s_transmitter();

	init_audio_codec();

	init_midi_uart();

	set_mixer_gain(MIXER_BASE_ADDR, 1.0);

	update_fp_coeffs(20000, 0);
	synth_state.resonance = 0;
	synth_state.filt_adsr[3] = 0.0;

	init_voices();
	write_wave1(0);
	write_wave2(0);
	write_det(0.005);

	set_cf(VERB_BASE_ADDR, 12000);
	set_fb(VERB_BASE_ADDR, 0.8);
	set_mix(VERB_BASE_ADDR, 0.3);

	synth_state.filt_amt = 0.8;

	while (1)
	{
		process_midi_message(&Uart_PS);
		process_voices();
		process_cutoff(synth_state.filt_amt);
	}

	return 0;
}
