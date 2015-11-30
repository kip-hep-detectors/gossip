#ifndef Oscilloscope_h
#define Oscilloscope_h

#include "Waveform.h"

class Oscilloscope
{
	public:
		Oscilloscope();
		~Oscilloscope();

		void		SetWaveform();
		Waveform	Process();
		void		ApplyLPFilter();
};

#endif
