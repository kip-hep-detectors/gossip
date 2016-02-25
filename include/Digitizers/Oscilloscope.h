#ifndef Oscilloscope_h
#define Oscilloscope_h

#include "VDigitizer.h"
#include "Waveform.h"

class Oscilloscope : public VDigitizer
{
	public:
		Oscilloscope( double bandwidth=0.2, double enoise=1, double sampling=0.1, double enob=6 );
		~Oscilloscope();

		void		Run();
		Waveform	GetWaveform(){ return waveform; };
		void		SetSamping( double samp ){ sampling = samp; };
		double		GetSamping(){ return sampling; };
		void		SetENOB( double ENOB ){ enob = ENOB; };
		double		GetENOB(){ return enob; };
		void		SetENoise( double noise ){ enoise = noise; };
		double		GetENoise(){ return enoise; };
		void		SetBW( double bw ){ bandwidth = bw; };
		double		GetBW(){ return bandwidth; };

	private:
		void		BWFilter();
		void		AddENoise();

		double		bandwidth;
		int		bwlp_order;
		double		enoise;
		double		sampling;
		double		enob;
};

#endif
