#ifndef _ADSR_H_
#define _ADSR_H_

#include <math.h>
#include <stdbool.h>

typedef enum
{
    ADSR_IDLE,
    ADSR_ATTACK,
    ADSR_DECAY,
    ADSR_SUSTAIN,
    ADSR_RELEASE
} ADSR_State;

typedef enum
{
    ADSR_LINEAR,
    ADSR_EXPONENTIAL
} ADSR_Mode;

typedef struct
{
    float attack_rate;
    float decay_rate;
    float sustain_level;
    float release_rate;

    float output;
    ADSR_State state;
    ADSR_Mode mode;

    float sr;
} ADSR;

void ADSR_init(ADSR *adsr, float sr, ADSR_Mode mode)
{
    adsr->attack_rate = 0;
    adsr->decay_rate = 0;
    adsr->release_rate = 0;
    adsr->sustain_level = 0;
    adsr->output = 0.0f;
    adsr->state = ADSR_IDLE;
    adsr->mode = mode;
    adsr->sr = sr;
}

void ADSR_setParams(ADSR *adsr, float attack, float decay, float sustain, float release)
{
    adsr->attack_rate = 1.0f / (adsr->sr * attack);
    adsr->decay_rate = (1.0f - sustain) / (adsr->sr * decay);
    adsr->release_rate = sustain / (adsr->sr * release);
    adsr->sustain_level = sustain;
}

void ADSR_setMode(ADSR *adsr, ADSR_Mode mode)
{
    adsr->mode = mode;
}

void ADSR_reset(ADSR *adsr)
{
    adsr->output = 0.0f;
    adsr->state = ADSR_ATTACK;
}

float ADSR_process(ADSR *adsr, bool gate)
{
    switch (adsr->state)
    {
    case ADSR_IDLE:
        adsr->output = 0.0f;
        break;

    case ADSR_ATTACK:
        adsr->output += adsr->attack_rate;
        if (adsr->output >= 1.0f)
        {
            adsr->output = 1.0f;
            adsr->state = ADSR_DECAY;
        }
        break;

    case ADSR_DECAY:
        if (adsr->mode == ADSR_LINEAR)
        {
            adsr->output -= adsr->decay_rate;
        }
        else
        {
            adsr->output *= (1.0f - adsr->decay_rate);
        }
        if (adsr->output <= adsr->sustain_level)
        {
            adsr->output = adsr->sustain_level;
            adsr->state = ADSR_SUSTAIN;
        }
        break;

    case ADSR_SUSTAIN:
        break;

    case ADSR_RELEASE:
        if (adsr->mode == ADSR_LINEAR)
        {
            adsr->output -= adsr->release_rate;
        }
        else
        {
            adsr->output *= (1.0f - adsr->release_rate);
        }
        if (adsr->output <= 0.0f)
        {
            adsr->output = 0.0f;
            adsr->state = ADSR_IDLE;
        }
        break;
    }

    if (gate && adsr->state == ADSR_IDLE)
    {
        adsr->state = ADSR_ATTACK;
    }
    else if (!gate && adsr->state != ADSR_IDLE)
    {
        adsr->state = ADSR_RELEASE;
    }

    return adsr->output;
}

#endif
