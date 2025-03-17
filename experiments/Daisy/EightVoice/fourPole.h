#ifndef FOURPOLE_H
#define FOURPOLE_H

#include <cmath>
#include "daisysp.h"

class FourPole
{
    double D[4];
    double G[4];
    float  cutoff;
    float  res;
    double piosr;
    double A;


    void  coeffs(float f);
    float filter(float s, float k);

  public:
    FourPole();
    void  Init(float samplerate);
    void  SetFreq(float f);
    void  SetRes(float r);
    float Process(float s);
    void  Reset();
};

#endif
