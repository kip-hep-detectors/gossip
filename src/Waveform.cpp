#include "Waveform.h"

#include <iostream>


using namespace std;


Waveform::Waveform()
{
	g_waveform = new TGraph();
	g_waveform->SetTitle(";Time [ns];Amplitude [mV]");
	g_waveform->SetMarkerStyle(1);

	sampling = 1;
}

Waveform::Waveform( vector<double> y, double Sampling )
{
	SetWaveform(y, Sampling);
}

Waveform::~Waveform()
{
}

void Waveform::Clear()
{
	v_amplitudes.clear();
}

void Waveform::Resize( int size )
{
	v_amplitudes.resize(size, 0);
}

void Waveform::SetWaveform( vector<double> y, double Sampling )
{
	v_amplitudes = y;
	sampling = Sampling;
}

void Waveform::SetSampling( double Sampling )
{
	sampling = Sampling;
}

void Waveform::SetSamples( vector<double> amps )
{
	v_amplitudes = amps;
}

void Waveform::SetSample( unsigned int i, double amp )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "Waveform::SetSample( unsigned int i, double amp )" << endl;

	if(v_amplitudes.size() <= i)
	{
		v_amplitudes.resize(i+1, 0);
	}
	v_amplitudes.at(i) = amp;
}

double Waveform::GetSample( unsigned int i )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "Waveform::GetSample( unsigned int i )" << endl;

	if(v_amplitudes.size() <= i) return 0;
	else return v_amplitudes.at(i);
}

TGraph* Waveform::GetGraph()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "Waveform::GetGraph()" << endl;

	g_waveform->Set(0);
	for(unsigned int i=0; i<v_amplitudes.size(); i++)
	{
		double time = i*sampling;
		double amplitude = v_amplitudes.at(i);
		g_waveform->SetPoint(i,time,amplitude);
	}

	return g_waveform;
}

