#include <math.h>
#include "hls_stream.h"
#include <ap_axi_sdata.h>
#include <ap_int.h>

typedef ap_axis<32, 2, 4, 1> stream;

#define SR 48000.0
#define MAX_PITCHMOD 20.0
#define DELAYPOS_SHIFT 28
#define DELAYPOS_SCALE 0x10000000
#define DELAYPOS_MASK 0x0FFFFFFF
#define PI 3.14159265358979323846
#define CHANNELS 2

#define DELAY_LINE_0_SIZE 2543
#define DELAY_LINE_1_SIZE 2842
#define DELAY_LINE_2_SIZE 3325
#define DELAY_LINE_3_SIZE 3605
#define DELAY_LINE_4_SIZE 3977
#define DELAY_LINE_5_SIZE 4202
#define DELAY_LINE_6_SIZE 2251
#define DELAY_LINE_7_SIZE 1981

#define NUM_DELAY_LINES 8
#define MAX_DELAY_BUFFER_SIZE 4250

static const float outputGain = 0.35;
static const float jpScale = 0.25;

static const float reverbParams[8][4] = {{(2473.0 / SR), 0.0010, 3.100, 1966.0},
                                         {(2767.0 / SR), 0.0011, 3.500, 29491.0},
                                         {(3217.0 / SR), 0.0017, 1.110, 22937.0},
                                         {(3557.0 / SR), 0.0006, 3.973, 9830.0},
                                         {(3907.0 / SR), 0.0010, 2.341, 20643.0},
                                         {(4127.0 / SR), 0.0011, 1.897, 22937.0},
                                         {(2143.0 / SR), 0.0017, 0.891, 29491.0},
                                         {(1933.0 / SR), 0.0006, 3.221, 14417.0}};

struct delayLine
{
    int writePos;
    int bufferSize;
    int readPos;
    int readPosFrac;
    int readPosFrac_inc;
    int dummy;
    int seedVal;
    int randLine_cnt;
    float filterState;
    float buf[MAX_DELAY_BUFFER_SIZE];
    bool initialized;
};

struct SC_REVERB
{
    float feedback;
    float lpfreq;
    float sampleRate;
    float pitchMod;
    float dampFact;
    bool initialized;
    delayLine delayLines[NUM_DELAY_LINES];
};

void reverbsc(hls::stream<stream> &in, hls::stream<stream> &out, volatile float &fb, volatile float &cf);
