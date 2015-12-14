#include "Digitizers/Oscilloscope.h"
#include "TRandom3.h"

Oscilloscope::Oscilloscope()
{
}

Oscilloscope::~Oscilloscope()
{
}

void Oscilloscope::Run()
{
	waveform = sipm->GetWaveform();
	ApplyLPFilter();
	AddENoise();
}

void Oscilloscope::AddENoise()
{
	TRandom3 r;
	for(int i=0; i<waveform.GetNsamples(); i++)
	{
		double amp = waveform.GetSample(i);
		waveform.SetSample(i, amp+r.Gaus(0,enoise));
	}
}

