#ifndef Waveform_h
#define Waveform_h

#include "TGraph.h"

#include "vector"

using namespace std;

class Waveform
{
	public:
		Waveform();
		Waveform( vector<double> amps, double Sampling);
		~Waveform();

		void		Clear();
		void		SetWaveform( vector<double> amps, double Sampling );
		void		SetSampling( double Sampling );
		double		GetSampling(){ return sampling; };
		void		SetSample( unsigned int i, double amp );
		double		GetSample( unsigned int i );
		void		SetSamples( vector<double> amp );
		vector<double>	GetSamples(){ return v_amplitudes; };
		int		GetNsamples(){ return v_amplitudes.size(); }
		TGraph*		GetGraph();

	private:

		double		sampling;

		TGraph*		g_waveform;
		vector<double>	v_amplitudes;
};

#endif

