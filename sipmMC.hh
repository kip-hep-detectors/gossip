/********** Version 2.0 **********/

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
    
    int    Npix;
    int    NpixX;
    int    NpixY;
    double xSipm;			//Size of active area in x
    double ySipm;			//Size of active area in y
    double tau_recovery;
    double PDE;
    double Pxt;
    double Pap_s, Pap_f;
    double tau_ap_s, tau_ap_f;
    double tau_dr;
    double gain;
    double ENF;
    double EN;
    double signalAmp;
    double noiseRMS;
    
    double     Generate( PhotonList photons );
    void       SetGeometry(string Geometry){ geometry = Geometry; };
    void       SetPulseShape( double Tau1 = 1, double Tau2 = 40, double Resolution = 0.1, double cutOff = 0.0001 );
    void       SetPulseShape( TH1* PulseShape );
    void       SetGate( double Gate );
    TH1*       GetPulseShape(){ return h_pulseShape; };
    TH1D*      GetWaveform();
    HitMatrix* GetHitMatrix(){ return hitMatrix; };
    void       BuildGeometry();
    
  private:
    
    HitMatrix*       hitMatrix;
    TH1D*            waveform;
    TH1*             h_pulseShape;
    //    vector<double>           pulseShape;
    double           pulseIntegral;
    double           charge;
    bool             customPulse;
    double gate;
    
    TRandom3              r;
    vector<double>        hit;
    PhotonList            photonList;
    TH2I*                 h_geometry;
    string                geometry;
    double                resolution;
    double                cutoff;
    double                tau1, tau2;
    int                   nBinsPulseShape;
    void      Reset();
    void      ImportPhotons( PhotonList photons );
    void      InitHitMatrix();
};

#endif

