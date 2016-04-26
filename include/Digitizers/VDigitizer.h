#ifndef VDigitizer_h
#define VDigitizer_h

#include "Waveform.h"
#include "SiPM.h"

class VDigitizer
{
	public:
		VDigitizer(){};
		virtual ~VDigitizer(){};

		virtual void		Run() = 0;
		void			ConnectSiPM( SiPM* SiPM ){ sipm = SiPM; };

	protected:
		virtual void		BWFilter() = 0;
		virtual void		AddENoise() = 0;

		SiPM*			sipm;
		Waveform		waveform;
		double			charge;
};

#endif
