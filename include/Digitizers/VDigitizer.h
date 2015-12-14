#ifndef VDigitizer_h
#define VDigitizer_h

#include "Waveform.h"
#include "sipmMC.h"

class VDigitizer
{
	public:
		VDigitizer(){ enoise = 0; bandwidth = 0; };
		virtual ~VDigitizer(){};

		virtual void		Run() = 0;
		void			ConnectSiPM( sipmMC* SiPM ){ sipm = SiPM; };
		void			SetBandwidth( double bw ){ bandwidth = bw; };
		void			SetENoise( double noise ){ enoise = noise; };

	protected:
		void			ApplyLPFilter();
		virtual void		AddENoise() = 0;

		sipmMC*			sipm;
		double			bandwidth;
		double			enoise;
		Waveform		waveform;
		double			charge;
};

#endif
