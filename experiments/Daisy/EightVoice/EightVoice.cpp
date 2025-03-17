#include "daisy_seed.h"
#include "daisysp.h"
#include "voiceManager.h"

using namespace daisy;
using namespace daisysp;

DaisySeed                     hw;
VoiceManager                  voiceManager;
MidiUartHandler               midi;
static ReverbSc DSY_SDRAM_BSS verb;
float                         verbMix;
CpuLoadMeter                  loadMeter;


void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{
    loadMeter.OnBlockStart();
    for(size_t i = 0; i < size; i++)
    {
        float dry = voiceManager.Process();
        float wetL, wetR;
        verb.Process(dry, dry, &wetL, &wetR);
        out[0][i] = (wetL * verbMix) + (dry * (1 - verbMix));
        out[1][i] = (wetR * verbMix) + (dry * (1 - verbMix));
    }
    loadMeter.OnBlockEnd();
}

void HandleMidiMessage(MidiEvent m)
{
    switch(m.type)
    {
        case NoteOn:
        {
            NoteOnEvent note = m.AsNoteOn();
            if(note.velocity == 0)
                voiceManager.NoteOff(mtof(note.note));
            else
                voiceManager.NoteOn(mtof(note.note), note.velocity / 127.0f);
        }
        break;
        case NoteOff:
        {
            NoteOnEvent note = m.AsNoteOn();
            voiceManager.NoteOff(mtof(note.note));
        }
        break;
        default: break;
    }
}


int main(void)
{
    hw.Init();
    hw.SetAudioBlockSize(8);
    hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
    hw.StartLog();

    loadMeter.Init(hw.AudioSampleRate(), hw.AudioBlockSize());

    voiceManager.Init(hw.AudioSampleRate());
    voiceManager.SetAmpEnv(0.01, 0.1, 0.9, 0.01);
    voiceManager.SetFiltEnv(0.1, 0.3, 0.4, 0);
    voiceManager.SetDetune(0);
    voiceManager.SetFiltEnvAmt(0);
    voiceManager.SetCutoff(20000);
    voiceManager.SetRes(0);

    verb.Init(hw.AudioSampleRate());
    verb.SetFeedback(0.8);
    verb.SetLpFreq(14000);
    verbMix = 0.3;

    MidiUartHandler::Config midi_config;
    midi.Init(midi_config);
    midi.StartReceive();

    hw.StartAudio(AudioCallback);


    while(1)
    {
        const float avgLoad = loadMeter.GetAvgCpuLoad();
        const float maxLoad = loadMeter.GetMaxCpuLoad();
        const float minLoad = loadMeter.GetMinCpuLoad();
        hw.PrintLine("Processing Load %:");
        hw.PrintLine("Max: " FLT_FMT3, FLT_VAR3(maxLoad * 100.0f));
        hw.PrintLine("Avg: " FLT_FMT3, FLT_VAR3(avgLoad * 100.0f));
        hw.PrintLine("Min: " FLT_FMT3, FLT_VAR3(minLoad * 100.0f));

        midi.Listen();
        while(midi.HasEvents())
        {
            HandleMidiMessage(midi.PopEvent());
        }
    }
}
