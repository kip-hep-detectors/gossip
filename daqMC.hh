/********** Version 2.0 **********/

#ifndef daqMC_hh
#define daqMC_hh

#include <string>
#include "HitMatrix.hh"
#include "PhotonSource.hh"
#include "sipmMC.hh"
#include <TRandom3.h>
#include <TGraph.h>
#include <TH1.h>
#include <TF1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <stdlib.h>
#include <vector>
#include <TROOT.h>
#include <RQ_OBJECT.h>

using namespace std;

class daqMC{
  RQ_OBJECT("daqMC")

 public:

   daqMC();
  ~daqMC();
  
  bool   cancel;
  int    selector;
  double progress;

  void      Statistic( int N );
  
  TH1D*     Scope( TH1D* waveform );
  TH1D*     QDCSpectrum( int N );
  TGraphErrors*   ThreshScan( double gate, double tstart, double tstop, double tstep );
  TH1D*     TDCSpectrum( int N );
  TGraphErrors*   DynamicRange( int N, double Ngamma_max, double Ngamma_step );
  
  
  void    QDC( double charge );
  void    SetQDCChannels( int nChannels ){ h_QDC->SetBins(nChannels,0,nChannels); };
  void    SetPedestal( double Pedestal ){ pedestal = Pedestal; };
  
  void    SetDiscriDeadTime( double deadtime ){ discriDeadTime = deadtime; };
  void    SetDiscriWidth( double width ){ discriWidth = width; };
  
  
  
  TH1D*   Discriminator( TH1D* waveform, double threshold );

  void      ScanCT( int );
  void      Progress( int ); //*SIGNAL*
  
  void      SetSiPM( sipmMC *mySiPM){ sipm = mySiPM; };
  void      SetPhotonSource( PhotonSource *myPhotonSource ){ photonSource = myPhotonSource; };

  TGraphErrors         *g_Response, *g_ResNgamma, *g_threshScan;
  TH1D                 *h_pe, *h_dr, *h_xt, *h_ap, *h_QDC, *h_TDC;
  
 private:

  bool  Check();
  
  double pedestal;
  
  int nTDC_channels;
  
  TRandom3             r;
  HitMatrix*           hitMatrix;
  vector<double>       hit;
  sipmMC*              sipm;
  PhotonSource*        photonSource;
  TH1D*                waveform;
  
  double discriDeadTime;
  double discriWidth;
  
  int                  plast;
  
};

#endif

