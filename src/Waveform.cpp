#include "Waveform.h"
#include "TMath.h"

#include "iostream"

#include "Digitizers/Filters/iir.h"


Waveform::Waveform()
{
}

Waveform::~Waveform()
{
}

void Waveform::Clear()
{
	v_time.clear();
	v_amplitude.clear();
	nSamples = 0;
	sampling = 0;
	tStart = 0;
	tStop = 0;
}

void Waveform::SetWaveform( vector<double> y, double Sampling, double Tstart )
{
	Clear();

	v_amplitude = y;
	sampling = Sampling;
	tStart = Tstart;

	for(int i=0;i<nSamples;i++)
	{
		double time = i*sampling+tStart;
		v_time.push_back(time);
	}

	SetWaveform(v_amplitude,v_time);
}

void Waveform::SetWaveform( TGraph* g_wf )
{
	Clear();

	double *x = g_wf->GetX();
	double *y = g_wf->GetY();

	for(int i=0;i<g_wf->GetN();i++)
	{
		double time = x[i];
		double amplitude = y[i];
		v_time.push_back(time);
		v_amplitude.push_back(amplitude);
	}

	SetWaveform(v_amplitude,v_time);
}

void Waveform::Invert()
{

	for(int i=0;i<nSamples;i++)
	{
		v_amplitude.at(i) *= -1;
	}

	SetWaveform(v_amplitude,v_time);
}

