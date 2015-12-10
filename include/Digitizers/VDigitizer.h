#ifndef VDigitizer_h
#define VDigitizer_h

#include "Waveform.h"
#include "sipmMC.h"

class VDigitizer
{
	public:
		VDigitizer(){};
		virtual ~VDigitizer(){};

		void			ConnectSiPM( sipmMC* SiPM ){ sipm = SiPM; };
		void			SetBandwidth( double bw ){ bandwidth = bw; };
		void			ApplyLPFilter();
		virtual void		Run() = 0;


	protected:
		sipmMC*			sipm;
		double			bandwidth;
};

#endif
