#include "voice.h"
#include <math.h>

Voice::Voice()
: detune(0.0f),
  gate(false),
  active(false),
  freq(0.0f),
  amp(0.0f),
  filtEnvAmt(0.6),
  cutoff(12000),
  res(0.6)
{
}

void Voice::Init(float samplerate)
{
    ampEnv.Init(samplerate);
    filtEnv.Init(samplerate);
    filt.Init(samplerate);
    osc1.Init(samplerate);
    osc2.Init(samplerate);

    osc1.SetWaveform(Oscillator::WAVE_SAW);
    osc2.SetWaveform(Oscillator::WAVE_SAW);

    ampEnv.SetTime(ADSR_SEG_ATTACK, 0.01f);
    ampEnv.SetTime(ADSR_SEG_DECAY, 0.1f);
    ampEnv.SetSustainLevel(0.7f);
    ampEnv.SetTime(ADSR_SEG_RELEASE, 0.2f);

    filtEnv.SetTime(ADSR_SEG_ATTACK, 0.01f);
    filtEnv.SetTime(ADSR_SEG_DECAY, 0.1f);
    filtEnv.SetSustainLevel(0.7f);
    filtEnv.SetTime(ADSR_SEG_RELEASE, 0.2f);

    filt.SetRes(res);
    filt.SetFreq(cutoff);
}

void Voice::NoteOn(float freq, float amp)
{
    this->freq = freq;
    this->amp  = amp;
    active     = true;
    gate       = true;
    osc1.SetFreq(freq * (1.0f - detune));
    osc2.SetFreq(freq * (1.0f + detune));
}

void Voice::NoteOff()
{
    gate = false;
}

void Voice::SetAmpEnv(float a, float d, float s, float r)
{
    ampEnv.SetTime(ADSR_SEG_ATTACK, a);
    ampEnv.SetTime(ADSR_SEG_DECAY, d);
    ampEnv.SetSustainLevel(s);
    ampEnv.SetTime(ADSR_SEG_RELEASE, r);
}

void Voice::SetFiltEnv(float a, float d, float s, float r)
{
    filtEnv.SetTime(ADSR_SEG_ATTACK, a);
    filtEnv.SetTime(ADSR_SEG_DECAY, d);
    filtEnv.SetSustainLevel(s);
    filtEnv.SetTime(ADSR_SEG_RELEASE, r);
}

void Voice::SetDetune(float det)
{
    detune = det;
    if(active)
    {
        osc2.SetFreq(freq * (1.0f + detune));
    }
}

void Voice::SetFiltEnvAmt(float amt)
{
    filtEnvAmt = amt;
}

void Voice::SetCutoff(float cf)
{
    cutoff = cf;
    filt.SetFreq(cf);
}

void Voice::SetRes(float resonance)
{
    res = resonance;
    filt.SetRes(resonance);
}

float Voice::Process()
{
    if(!active)
    {
        return 0.0f;
    }

    float envAmp  = ampEnv.Process(gate);
    float envFilt = filtEnv.Process(gate);
    float oscOut  = (osc1.Process() + osc2.Process()) * 0.5f;


    float filtEnvAdd = (20000 - cutoff) * envFilt * filtEnvAmt;
    filt.SetFreq(cutoff + filtEnvAdd);
    float filtered = filt.Process(oscOut);
    float out      = filtered * envAmp * amp;

    if(!ampEnv.IsRunning())
    {
        active = false;
    }
    return out;
}

bool Voice::IsActive() const
{
    return active;
}

float Voice::GetFreq() const
{
    return freq;
}
