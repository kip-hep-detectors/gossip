#ifndef VDigitizer_h
#define VDigitizer_h

#include "Waveform.h"
#include "sipmMC.h"

class VDigitizer
{
	public:
		VDigitizer(){};
		virtual ~VDigitizer(){};

		virtual void		Run() = 0;
		void			ConnectSiPM( sipmMC* SiPM ){ sipm = SiPM; };

	protected:
		virtual void		BWFilter() = 0;
		virtual void		AddENoise() = 0;

		sipmMC*			sipm;
		Waveform		waveform;
		double			charge;
};

#endif
