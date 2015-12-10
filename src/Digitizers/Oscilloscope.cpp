#include "Digitizers/Oscilloscope.h"

Oscilloscope::Oscilloscope()
{
}

Oscilloscope::~Oscilloscope()
{
}

void Oscilloscope::Run()
{
	waveform = sipm->GetWaveform();
}

