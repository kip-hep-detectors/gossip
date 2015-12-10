#ifndef Oscilloscope_h
#define Oscilloscope_h

#include "VDigitizer.h"
#include "Waveform.h"

class Oscilloscope : public VDigitizer
{
	public:
		Oscilloscope();
		~Oscilloscope();

		void		Run();
		Waveform	GetWaveform(){ return waveform; };
		void		SetSamping( double samp ){ sampling = samp; };
		double		GetSamping(){ return sampling; };
		void		SetENOB( double ENOB ){ enob = ENOB; };
		double		GetENOB(){ return enob; };

	private:
		Waveform	waveform;
		double		sampling;
		double		enob;
};

#endif
