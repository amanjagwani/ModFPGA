#include "fourPole.h"

FourPole::FourPole() : cutoff(0.0f), res(0.0f), piosr(0.0), A(0.0)
{
    for(int i = 0; i < 4; i++)
    {
        D[i] = 0.0;
        G[i] = 0.0;
    }
}

void FourPole::Init(float samplerate)
{
    piosr = PI_F / samplerate;
    for(int i = 0; i < 4; i++)
    {
        D[i] = 0.0;
    }
    cutoff = 1000.0f;
    coeffs(cutoff);
}

void FourPole::SetFreq(float f)
{
    if(f != cutoff)
    {
        coeffs(f);
    }
}

void FourPole::SetRes(float r)
{
    res = r;
}

float FourPole::Process(float s)
{
    return filter(s, res * 4.0f);
}

void FourPole::Reset()
{
    for(int i = 0; i < 4; i++)
    {
        D[i] = 0.0;
    }
    coeffs(cutoff);
}

void FourPole::coeffs(float f)
{
    float w = tanf(f * piosr);
    G[0]    = w / (1.0f + w);
    A       = (w - 1.0f) / (1.0f + w);
    G[1]    = G[0] * G[0];
    G[2]    = G[0] * G[1];
    G[3]    = G[0] * G[2];
    cutoff  = f;
}

float FourPole::filter(float s, float k)
{
    double o, u, w;
    double ss = D[3];
    for(int j = 0; j < 3; j++)
    {
        ss += D[j] * G[2 - j];
    }
    o = (G[3] * s + ss) / (1.0 + k * G[3]);
    u = G[0] * (s - k * o);
    for(int j = 0; j < 3; j++)
    {
        w    = D[j] + u;
        D[j] = u - A * w;
        u    = G[0] * w;
    }
    D[3] = G[0] * w - A * o;
    return static_cast<float>(o);
}
