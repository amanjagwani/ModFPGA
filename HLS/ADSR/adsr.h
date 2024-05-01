#ifndef _ADSR_H_
#define _ADSR_H_

#include "hls_stream.h"
#include <ap_int.h>
#include <ap_axi_sdata.h>

typedef ap_axis<32, 2, 5, 6> stream;
void adsr_gen(hls::stream<stream> &out, volatile bool &gate, volatile float &attack, volatile float &decay, volatile float &sustain, volatile float &release);

class ADSR
{

public:
	enum class State
	{
		IDLE,
		ATTACK,
		DECAY,
		SUSTAIN,
		RELEASE
	};

private:
	float attack_incr;
	float decay_incr;
	float sustain_level;
	float release_incr;

	float output;
	State state;

	float sr;

	void calc_incr(float attack, float decay, float release)
	{
		attack_incr = 1.0f / (sr * attack);
		decay_incr = (1.0f - sustain_level) / (sr * decay);
		release_incr = (sustain_level) / (sr * release);
	}

public:
	ADSR(float SR) : attack_incr(0), decay_incr(0), release_incr(0), sustain_level(0), output(0.0f), state(State::IDLE), sr(SR){

																														 };

	void set_params(float atk, float dec, float sus, float rel)
	{
		calc_incr(atk, dec, rel);
		sustain_level = sus;
	}

	float process(bool gate)
	{

		if (gate && (state != State::ATTACK && state != State::SUSTAIN))
		{
			state = State::ATTACK;
		}

		if (!gate && (state == State::ATTACK || state == State::SUSTAIN))
		{
			state = State::RELEASE;
		}

		switch (state)
		{
		case State::IDLE:
			output = 0.0f;
			break;

		case State::ATTACK:
			output += attack_incr;
			if (output >= 1.0f)
			{
				output = 1.0f;
				state = State::DECAY;
			}
			break;

		case State::DECAY:
			output -= decay_incr;
			if (output <= sustain_level)
			{
				output = sustain_level;
				state = State::SUSTAIN;
			}
			break;

		case State::SUSTAIN:
			break;

		case State::RELEASE:
			output -= release_incr;
			if (output <= 0.0f)
			{
				output = 0.0f;
				state = State::IDLE;
			}
			break;
		}

		return output;
	}
};

#endif
