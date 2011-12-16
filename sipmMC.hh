
#ifndef sipmMC_hh
#define sipmMC_hh

#include "HitMatrix.hh"
#include "PhotonList.hh"
#include <TROOT.h>
#include <TRandom3.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <string>

using namespace std;

class sipmMC{

  public:
    
    sipmMC();
    ~sipmMC();
    
    static sipmMC*  Instance() { return fInstance; };
    static sipmMC*  fInstance;
    
    int			NpixX;			//Number of pixels in X ()
    int			NpixY;			//Number of pixels in Y
    double		xSipm;			//Size of active area in x
    double		ySipm;			//Size of active area in y
    double		tau_recovery;		//Recovery time
    double		PDE;			//Photon detection efficiency
    double		Pxt;			//Cross-talk probability
    double		Pap_s, Pap_f;		//After-pulse probability (slow & fast)
    double		tau_ap_s, tau_ap_f;	//After-pulse time constant (slow & fast)
    double		tau_dr;			//Thermal pulse time constant
    double		gain;			//Gain
    double		ENF;			//Pixel noise
    double		EN;			//Electronic noise
    double		signalAmp;		//Singel pixel signal amplitude
    double		noiseRMS;		//RMS of noise
    
    double		Generate( PhotonList photons );
    void		GetParaFile( const char* filename );
    void		SetGeometry( string Geometry );
    void		SetGeometry( TH2I* hgeometry );
    void      		SetPulseShape( double Tau1 = 1, double Tau2 = 40, double Resolution = 0.1, double cutOff = 0.0001 );
    void      		SetPulseShape( TH1* PulseShape );
    void		SetGate( double Gate, bool gateCut=true );
    double		GetGate(){ return gate; };
    TH1*		GetPulseShape(){ return h_pulseShape; };
    TH1D*		GetWaveform();
    HitMatrix*		GetHitMatrix(){ return hitMatrix; };
    int			GetNpix(){ return Npix; };
    
  private:

    int			Npix;
    HitMatrix*		hitMatrix;
    TH1D*		waveform;
    TH1*		h_pulseShape;
    double		pulseIntegral;
    double		charge;
    bool		customPulse;
    double		gate;
    
    TRandom3		r;
    vector<double>	hit;
    PhotonList		photonList;
    TH2I*		h_geometry;
    double		resolution;
    double		cutoff;
    double		tau1, tau2;
    int			nBinsPulseShape;
    
    void		Reset();
    void		ImportPhotons( PhotonList photons );
    void		InitHitMatrix();
};

#endif
