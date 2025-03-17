#ifndef VOICEMANAGER_H
#define VOICEMANAGER_H

#include "voice.h"

class VoiceManager
{
    static const int maxVoices = 8;
    Voice            voices[maxVoices];

  public:
    VoiceManager();
    void  Init(float samplerate);
    void  NoteOn(float freq, float amp);
    void  NoteOff(float freq);
    float Process();
    void  SetAmpEnv(float a, float d, float s, float r);
    void  SetFiltEnv(float a, float d, float s, float r);
    void  SetDetune(float det);
    void  SetFiltEnvAmt(float amt);
    void  SetCutoff(float cf);
    void  SetRes(float resonance);
};

#endif
