#ifndef QDC_h
#define QDC_h

#include "VDigitizer.h"
#include "Waveform.h"

class QDC : public VDigitizer
{
	public:
		QDC();
		~QDC();

		void		Run();
		int		GetQDCValue(){ return qdc_value; };
		void		SetConversion( double c ){ conv = c; };
		double		GetConversion(){ return conv; };
		void		SetPedestal( double ped ){ pedestal = ped; };
		double		GetPedestal(){ return pedestal; };
		void		SetNbins( int n ){ nbins = n; };
		int		GetNbins(){ return nbins; };

	private:
		void		AddENoise();

		double		conv;
		double		pedestal;
		int		nbins;
		int		qdc_value;
};

#endif
