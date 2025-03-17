#ifndef VOICE_H
#define VOICE_H

#include "daisysp.h"
#include "fourPole.h"

using namespace daisysp;

class Voice
{
    Adsr       ampEnv, filtEnv;
    Oscillator osc1, osc2;
    FourPole   filt;
    float      detune;
    bool       gate;
    bool       active;
    float      freq;
    float      amp;
    float      filtEnvAmt;
    float      cutoff;
    float      res;

  public:
    Voice();
    void  Init(float samplerate);
    void  NoteOn(float freq, float amp);
    void  NoteOff();
    void  SetAmpEnv(float a, float d, float s, float r);
    void  SetFiltEnv(float a, float d, float s, float r);
    void  SetDetune(float det);
    void  SetFiltEnvAmt(float amt);
    void  SetCutoff(float cf);
    void  SetRes(float resonance);
    float Process();
    bool  IsActive() const;
    float GetFreq() const;
};

#endif