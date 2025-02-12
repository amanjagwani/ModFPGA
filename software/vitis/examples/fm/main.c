
#include "i2s_setup.h"
#include "audio.h"
#include "ADSR.h"
#include "voice.h"

#define SYNTH_FM
#include "config.h"
#include "midi_uart.h"
#include "mixer.h"
#include "reverb.h"

int main()
{

    init_i2s_transmitter();

    init_audio_codec();

    init_midi_uart();

    float amp_adsr[4] = {0.1, 0.1, 0.7, 0.1};
    float mod_adsr[4] = {0.1, 0.1, 0.7, 0.1};

    set_mixer_gain(MIXER_BASE_ADDR, 0.3);

    synth_state.mod_ratio = 3;
    memcpy(synth_state.mod_adsr, mod_adsr, sizeof(mod_adsr));
    memcpy(synth_state.adsr, amp_adsr, sizeof(amp_adsr));
    init_voices();
    write_indx(8);

    set_cf(VERB_BASE_ADDR, 12000);
    set_fb(VERB_BASE_ADDR, 0.9);
    set_mix(VERB_BASE_ADDR, 0.4);

    while (1)
    {
        process_midi_message(&Uart_PS);
        process_voices();
    }

    return 0;
}
