/********** Version 1.2 **********/

#ifndef SiPM_MC_h
#define SiPM_MC_h

#include <string>
#include "HitMatrix.h"
#include <TRandom1.h>
#include <TGraph.h>
#include <TH1.h>
#include <TF1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <stdlib.h>
#include <vector>
#include <RQ_OBJECT.h>

using namespace std;

class SiPM_MC{
  RQ_OBJECT("SiPM_MC")

 public:

	int tscale;
	double Npix, Npx, Npy, Ngamma, Ngamma_max, LED_width, gate, recovery, tau1, tau2, PDE, Px, Pas, Paf, tau_aps, tau_apf, tau_dr, G, ENF, EN, pedestal, d_spot;
	bool SeedHit, cancel, NgammaFluct;
	int selector;
	double progress;

	SiPM_MC();
	~SiPM_MC();

	int Generate( bool );
	void GeneratePhotons(string opt, bool NgammaFluct=true, int xHit=0, int yHit=0);
	TH1D* QDCSpectrum( int );
	TGraph* ThreshScan( int );
	TGraph* Res_vs_Ngamma( int );
	TGraph* Response( int );
	void ScanCT( int );
	TH1D* Histo(int N, string type);
	void TimeSpectrum( int );
	TGraph* Scope(double res);
	TGraph* ScopeSection(double res, double tstart, double thresh);
	void Progress( int ); //*SIGNAL*
	HitMatrix *matrix;

 private:

	TRandom1 r;
	int plast;
	vector<double> event;
	vector<vector<int> > PhotonCoord;
	HitMatrix *ctapFlag;
	void Reset( bool );
	void NumberOfHits();
	double BuildPulses(double x);
	double Charge();
	
	TH1D		*h_QDC, *h_TDC, *h_Histo;
	TGraph		*g_Thresh, *g_Temp;
	TGraphErrors	*g_Response, *g_ResNgamma;
};

#endif

