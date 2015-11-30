#ifndef Waveform_h
#define Waveform_h

#include "TGraph.h"

#include "vector"

using namespace std;

class Waveform
{
	public:
		Waveform();
		~Waveform();

		void		Clear();
		void		SetWaveform( vector<double> y, double Sampling, double t_start=0 );
		void		Invert();
		void		CutWaveform( double t1, double t2 );
		TGraph		GetGraph(){ return g_waveform; };

	private:

		int		nSamples;
		double		sampling;
		double		tStart;
		double		tStop;

		TGraph		g_waveform;

		vector<double>	v_time;
		vector<double>	v_amplitude;
};

#endif

