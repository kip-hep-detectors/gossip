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
#include <string>

using namespace std;

struct GCharge /**<Structure of charge return value*/
{
  double all;
  double pe;
  double dr;
  double ct;
  double ap;
  double enf;
  double en;
};

/**
 * Main SiPM simulation class
 *
 * Generates the waveform and charge from a photon list
 */
class sipmMC{

  public:
    
    sipmMC();
    ~sipmMC();
    
    static sipmMC*	Instance() { return fInstance; };
    static sipmMC*	fInstance;
    
    int			NpixX;						/**<Number of pixels in x. Disregarded when SetGeometry( TH2I* hgeometry ) is used.*/
    int			NpixY;						/**<Number of pixels in y. Disregarded when SetGeometry( TH2I* hgeometry ) is used.*/
    double		xSipm;						/**<Size of active area in x*/
    double		ySipm;						/**<Size of active area in y*/
    double		tau_recovery;					/**<Pixel recovery time*/
    double		PDE;						/**<Photon detection efficiency*/
    double		Pxt;						/**<Cross-talk probability (probability for one ore more cross-talk events per avalanche)*/
    double		Pap_s;						/**<After-pulse probability (slow)*/
    double		Pap_f;						/**<After-pulse probability (fast)*/
    double		tau_ap_s;					/**<After-pulse time constant (slow)*/
    double		tau_ap_f;					/**<After-pulse time constant (fast)*/
    double		tau_dr;						/**<Thermal pulse time constant*/
    double		gain;						/**<Gain*/
    double		ENF;						/**<Excess noise (=gain fluctuations)*/
    double		EN;						/**<Electronic noise (=pedestal noise)*/
    double		signalAmp;					/**<Singel pixel signal amplitude*/
    double		noiseRMS;					/**<RMS of noise*/
    double		jitter;						/**<Jitter of rising edge tau1*/
    
    double		Generate( PhotonList photons );			/**<Generates SiPM response*/
    void		GetParaFile( const char* filename );		/**<Sets SiPM parameters from config file*/
    void		SetGeometry( string Geometry );			/**<Sets pixel arrangement. Only "square" implemented at the moment*/
    void		SetGeometry( TH2I* hgeometry );			/**<Sets custom pixel arrangement from TH2I*/
    void      		SetPulseShape( double Tau1 = 1,
				       double Tau2 = 40,
				       double Resolution = 0.1,
				       double cutOff = 0.0001 ); 	/**<Sets double exponential single pixel waveform with time constants "Tau1", "Tau2". "Resolution" is the sampling time of the simualtion in ns. Waveform is cut off at a fraction "cutOff" of the amplitude*/
    void      		SetPulseShape( TH1* PulseShape );		/**<Sets custom single pixel waveform from TH2I*/
    void		SetGate( double Gate, bool gateCut=true );	/**<Sets integration gate. (For information on "gateCut" see HitMatrix class)*/

    TH1D*		GetWaveform();					/**<Returns simulated signal waveform*/
									/**Returns signal charge from different components*/
    GCharge		GetCharge(){ return charge; };
									/**Returns length of integration gate in ns*/
    double		GetGate(){ return gate; };
									/**Returns single pixel waveform*/
    TH1D*		GetPulseShape(){ return h_pulseShape; };
									/**Returns hitmatrix*/
    HitMatrix*		GetHitMatrix(){ return hitMatrix; };
									/**Returns number of pixels of the SiPM*/
    int			GetNpix(){ return Npix; };
				   
    
  private:

    int			Npix;
    HitMatrix*		hitMatrix;
    TH1D*		waveform;
    TH1D*		h_pulseShape;
    double		pulseIntegral;
    GCharge		charge;
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
