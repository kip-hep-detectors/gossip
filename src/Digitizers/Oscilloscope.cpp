#include "Digitizers/Oscilloscope.h"

#include "Digitizers/Filters/BWFilters.h"
#include "TRandom3.h"

Oscilloscope::Oscilloscope( double kbandwidth, double kenoise, double ksampling, double kenob )
{
	bwlp_order = 2;
	bandwidth = kbandwidth;
	enoise = kenoise;
	sampling = ksampling;
	enob = kenob;
}

Oscilloscope::~Oscilloscope()
{
}

void Oscilloscope::Run()
{
	waveform = sipm->GetWaveform();
	BWFilter();
	AddENoise();
}

void Oscilloscope::BWFilter()
{
	waveform = BWFilters::BWLP(waveform, bandwidth, bwlp_order);
}

void Oscilloscope::AddENoise()
{
	TRandom3 r(0);
	for(int i=0; i<waveform.GetNsamples(); i++)
	{
		double amp = waveform.GetSample(i);
		waveform.SetSample(i, amp+r.Gaus(0,enoise));
	}
}

