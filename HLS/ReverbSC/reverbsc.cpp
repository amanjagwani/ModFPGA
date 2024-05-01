#include "reverbsc.h"

// Delay line 0 buffer size: 2543 samples
// Delay line 1 buffer size: 2842 samples
// Delay line 2 buffer size: 3325 samples
// Delay line 3 buffer size: 3605 samples
// Delay line 4 buffer size: 3977 samples
// Delay line 5 buffer size: 4202 samples
// Delay line 6 buffer size: 2251 samples
// Delay line 7 buffer size: 1981 samples

void next_random_lineseg(delayLine *lp, int n, float pitchMod)
{
	float prvDel, nxtDel, phs_incVal;

	if (lp->seedVal < 0)
		lp->seedVal += 0x10000;
	lp->seedVal = (lp->seedVal * 15625 + 1) & 0xFFFF;
	if (lp->seedVal >= 0x8000)
		lp->seedVal -= 0x10000;

	lp->randLine_cnt = (int)((SR / reverbParams[n][2]) + 0.5);
	prvDel = (float)lp->writePos;
	prvDel -= ((float)lp->readPos + ((float)lp->readPosFrac / (float)DELAYPOS_SCALE));
	while (prvDel < 0.0)
	{
		prvDel += lp->bufferSize;
	}
	prvDel = prvDel / SR;
	nxtDel = (float)lp->seedVal * reverbParams[n][1] / 32768.0;
	nxtDel = reverbParams[n][0] + (nxtDel * (float)pitchMod);

	phs_incVal = (prvDel - nxtDel) / (float)lp->randLine_cnt;
	phs_incVal = phs_incVal * SR + 1.0;
	lp->readPosFrac_inc = (int)(phs_incVal * DELAYPOS_SCALE + 0.5);
}

void init_delay_line(SC_REVERB *p, delayLine *lp, int n)
{
	if (!lp->initialized)
	{
		float readPos;
		switch (n)
		{
		case 0:
			lp->bufferSize = DELAY_LINE_0_SIZE;
			break;

		case 1:
			lp->bufferSize = DELAY_LINE_1_SIZE;
			break;

		case 2:
			lp->bufferSize = DELAY_LINE_2_SIZE;
			break;

		case 3:
			lp->bufferSize = DELAY_LINE_3_SIZE;
			break;

		case 4:
			lp->bufferSize = DELAY_LINE_4_SIZE;
			break;

		case 5:
			lp->bufferSize = DELAY_LINE_5_SIZE;
			break;

		case 6:
			lp->bufferSize = DELAY_LINE_6_SIZE;
			break;

		case 7:
			lp->bufferSize = DELAY_LINE_7_SIZE;
			break;
		default:
			break;
		}

		lp->writePos = 0;
		lp->seedVal = (int)(reverbParams[n][3] + 0.5);

		readPos = (float)lp->seedVal * reverbParams[n][1] / 32768;
		readPos = reverbParams[n][0] + (readPos * (float)p->pitchMod);
		readPos = (float)lp->bufferSize - (readPos * SR);
		lp->readPos = (int)(readPos);
		readPos = (readPos - (float)lp->readPos) * (float)DELAYPOS_SCALE;
		lp->readPosFrac = (int)(readPos + 0.5);

		next_random_lineseg(lp, n, p->pitchMod);

		for (int i = 0; i < lp->bufferSize; i++)
		{
			lp->buf[i] = 0.0;
		}

		lp->filterState = 0.0;
		lp->initialized = true;
	}
}

void sc_reverb_init(SC_REVERB *p)
{
	if (!p->initialized)
	{
		p->feedback = 0.5;
		p->lpfreq = 10000.0;
		p->sampleRate = SR;
		p->pitchMod = 1.0;
		p->dampFact = 1.0;

		for (int i = 0; i < NUM_DELAY_LINES; i++)
		{
			init_delay_line(p, &(p->delayLines[i]), i);
		}
		p->initialized = true;
	}
}

void sc_reverb_perf(float inL, float inR, float *outL, float *outR, float fb, float cf, SC_REVERB *verb)
{
	float ainL, ainR, aoutL, aoutR, out;
	float vm1, v0, v1, v2, am1, a0, a1, a2, frac;
	delayLine *lp;
	int readPos;
	int bufferSize;
	float dampFact = verb->dampFact;

	if (verb->lpfreq != cf)
	{
		verb->lpfreq = cf;
		dampFact = 2.0f - cosf(verb->lpfreq * (2.0f * (float)PI) / SR);
		dampFact = verb->dampFact = dampFact - sqrtf(dampFact * dampFact - 1.0f);
	}

	ainL = aoutL = aoutR = 0.0;
	for (int n = 0; n < 8; n++)
	{
		ainL += verb->delayLines[n].filterState;
	}

	ainL *= jpScale;
	ainR = ainL + inR;
	ainL = ainL + inL;

	for (int n = 0; n < 8; n++)
	{
		lp = &verb->delayLines[n];
		bufferSize = lp->bufferSize;

		lp->buf[lp->writePos] = (float)((n & 1 ? ainR : ainL) - lp->filterState);
		if (++lp->writePos >= bufferSize)
		{
			lp->writePos -= bufferSize;
		}

		if (lp->readPosFrac >= DELAYPOS_SCALE)
		{
			lp->readPos += (lp->readPosFrac >> DELAYPOS_SHIFT);
			lp->readPosFrac &= DELAYPOS_MASK;
		}

		if (lp->readPos >= bufferSize)
		{
			lp->readPos -= bufferSize;
		}
		readPos = lp->readPos;
		frac = (float)lp->readPosFrac * (1.0 / (float)DELAYPOS_SCALE);

		a2 = frac * frac;
		a2 -= 1.0;
		a2 *= (1.0 / 6.0);
		a1 = frac;
		a1 += 1.0;
		a1 *= 0.5;
		am1 = a1 - 1.0;
		a0 = 3.0 * a2;
		a1 -= a0;
		am1 -= a2;
		a0 -= frac;

		if (readPos > 0 && readPos < (bufferSize - 2))
		{
			vm1 = (float)(lp->buf[readPos - 1]);
			v0 = (float)(lp->buf[readPos]);
			v1 = (float)(lp->buf[readPos + 1]);
			v2 = (float)(lp->buf[readPos + 2]);
		}
		else
		{
			if (--readPos < 0)
			{
				readPos += bufferSize;
			}
			vm1 = (float)lp->buf[readPos];
			if (++readPos >= bufferSize)
			{
				readPos -= bufferSize;
			}
			v0 = (float)lp->buf[readPos];
			if (++readPos >= bufferSize)
			{
				readPos -= bufferSize;
			}
			v1 = (float)lp->buf[readPos];
			if (++readPos >= bufferSize)
			{
				readPos -= bufferSize;
			}
			v2 = (float)lp->buf[readPos];
		}

		v0 = (am1 * vm1 + a0 * v0 + a1 * v1 + a2 * v2) * frac + v0;

		lp->readPosFrac += lp->readPosFrac_inc;

		v0 *= (float)fb;
		v0 = (lp->filterState - v0) * dampFact + v0;
		lp->filterState = v0;

		if (n & 1)
		{
			aoutR += v0;
		}
		else
		{
			aoutL += v0;
		}

		if (--(lp->randLine_cnt) <= 0)
		{
			next_random_lineseg(lp, n, verb->pitchMod);
		}
	}

	*outR = aoutR * outputGain;
	*outL = aoutL * outputGain;
}

void reverbsc(hls::stream<stream> &in, hls::stream<stream> &out, volatile float &fb, volatile float &cf)
{
#pragma HLS INTERFACE axis port = in
#pragma HLS INTERFACE axis port = out
#pragma HLS INTERFACE s_axilite port = fb
#pragma HLS INTERFACE s_axilite port = cf
#pragma HLS INTERFACE ap_ctrl_none port = return

	float local_fb, local_cf;
	static SC_REVERB reverbsc;
	sc_reverb_init(&reverbsc);

	if (reverbsc.initialized)
	{
		while (!in.empty())
		{
#pragma HLS LOOP_TRIPCOUNT max = 1
#pragma HLS PIPELINE off
			local_fb = fb;
			local_cf = cf;

			stream input_streamL = in.read();
			stream input_streamR = in.read();

			ap_int<24> data_inL = input_streamL.data(27, 4);
			float in_sampleL = static_cast<float>(data_inL) * (1.0f / ((1 << 23) - 1));

			ap_int<24> data_inR = input_streamR.data(27, 4);
			float in_sampleR = static_cast<float>(data_inR) * (1.0f / ((1 << 23) - 1));

			float out_sampleL = 0.0f, out_sampleR = 0.0f;

			sc_reverb_perf(in_sampleL, in_sampleR, &out_sampleL, &out_sampleR, local_fb, local_cf, &reverbsc);

			ap_int<24> out_dataL = static_cast<ap_int<24>>(out_sampleL * ((1 << 23) - 1));
			ap_int<24> out_dataR = static_cast<ap_int<24>>(out_sampleR * ((1 << 23) - 1));

			stream out_streamL;
			out_streamL.data(27, 4) = out_dataL;
			out_streamL.keep = input_streamL.keep;
			out_streamL.strb = input_streamL.strb;
			out_streamL.user = input_streamL.user;
			out_streamL.last = input_streamL.last;
			out_streamL.id = input_streamL.id;
			out.write(out_streamL);

			stream out_streamR;
			out_streamR.data(27, 4) = out_dataR;
			out_streamR.keep = input_streamR.keep;
			out_streamR.strb = input_streamR.strb;
			out_streamR.user = input_streamR.user;
			out_streamR.last = input_streamR.last;
			out_streamR.id = input_streamR.id;
			out.write(out_streamR);
		}
	}
}
