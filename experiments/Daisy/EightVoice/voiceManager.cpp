#include "voiceManager.h"
#include <math.h>

VoiceManager::VoiceManager() {}

void VoiceManager::Init(float samplerate)
{
    for(int i = 0; i < maxVoices; i++)
    {
        voices[i].Init(samplerate);
    }
}

void VoiceManager::NoteOn(float freq, float amp)
{
    for(int i = 0; i < maxVoices; i++)
    {
        if(!voices[i].IsActive())
        {
            voices[i].NoteOn(freq, amp * 0.125);
            break;
        }
    }
}

void VoiceManager::NoteOff(float freq)
{
    for(int i = 0; i < maxVoices; i++)
    {
        if(voices[i].IsActive() && fabs(voices[i].GetFreq() - freq) < 0.001f)
        {
            voices[i].NoteOff();
        }
    }
}

float VoiceManager::Process()
{
    float mix = 0.0f;
    for(int i = 0; i < maxVoices; i++)
    {
        mix += voices[i].Process();
    }
    return mix;
}

void VoiceManager::SetAmpEnv(float a, float d, float s, float r)
{
    for(int i = 0; i < maxVoices; i++)
    {
        voices[i].SetAmpEnv(a, d, s, r);
    }
}

void VoiceManager::SetFiltEnv(float a, float d, float s, float r)
{
    for(int i = 0; i < maxVoices; i++)
    {
        voices[i].SetFiltEnv(a, d, s, r);
    }
}

void VoiceManager::SetDetune(float det)
{
    for(int i = 0; i < maxVoices; i++)
    {
        voices[i].SetDetune(det);
    }
}

void VoiceManager::SetFiltEnvAmt(float amt)
{
    for(int i = 0; i < maxVoices; i++)
    {
        voices[i].SetFiltEnvAmt(amt);
    }
}

void VoiceManager::SetCutoff(float cf)
{
    for(int i = 0; i < maxVoices; i++)
    {
        voices[i].SetCutoff(cf);
    }
}

void VoiceManager::SetRes(float resonance)
{
    for(int i = 0; i < maxVoices; i++)
    {
        voices[i].SetRes(resonance);
    }
}
