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

struct GResonseCurve /**<Structure of response measurement return value*/
{
  TGraphErrors *response;
  TGraphErrors *resolution;
  TGraphErrors *responsePE;
  TGraphErrors *resolutionPE;
  TGraphErrors *responseDR;
  TGraphErrors *resolutionDR;
  TGraphErrors *responseAP;
  TGraphErrors *resolutionAP;
  TGraphErrors *responseCT;
  TGraphErrors *resolutionCT;
  TGraphErrors *responseENF;
  TGraphErrors *resolutionENF;
  TGraphErrors *responseEN;
  TGraphErrors *resolutionEN;
};

/**
 * Simple implementation of several characterization measurements
 */
class daqMC{
  RQ_OBJECT("daqMC")

 public:

   daqMC();
  ~daqMC();
  
														/**Connect daqMC class with sipmMC object*/
  void      		SetSiPM( sipmMC *mySiPM){ sipm = mySiPM; };							
														/**Connect daqMC class with PhotonSource object*/
  void      		SetPhotonSource( PhotonSource *myPhotonSource ){ photonSource = myPhotonSource; };		
  void      		Statistic( int N );									/**<Plots number of pde, ct, ap, dr events in "N" events*/
  TH1D*     		Scope( TH1D* waveform );								/**<Returns waveform (does nothing useful in this version)*/
  TH1D*     		QDCSpectrum( int N );									/**<Simulates charge spectrum with "N" events*/
  TGraphErrors*		ThreshScan( double gate, double tstart, double tstop, double tstep );			/**<Simulates theshold scan with "gate" integration time and thresholds from "tstart" to "tstop" with step size "tstep"*/
  TH1D*     		TDCSpectrum( int N );									/**<Simulated time spectrum with "N" events*/
  GResonseCurve 	DynamicRange( int N, double Ngamma_max, double Ngamma_step );				/**<Simulates saturation curve and RMS with "N" events per intensity from 0 to "Ngamma_max" photons with "Ngamma_step" step width*/
    
  double   		QDC( double charge );									/**<Returns charge in QDC channels*/
														/**Sets number of QDC channels*/
  void    		SetQDCChannels( int nChannels ){ h_QDC->SetBins(nChannels,0,nChannels); };		
														/**Sets pedestal of QDC*/
  void    		SetPedestal( double Pedestal ){	pedestal = Pedestal; };							
														/**Sets minimal time the input signal must be above the threshold for the discriminator to give a signal*/
  void    		SetDiscriMinTime( double mintime ){ discriMinTime = mintime; };					
														/**Sets minimal duration discriminator output pulse in ns*/
  void    		SetDiscriWidth( double width ){	discriWidth = width; };
  
  TH1D*   		Discriminator( TH1D* waveform, double threshold );					/**<Returns discriminated waveform (MAY STILL BE BUGGY!)*/

//   void      		ScanCT( int );										/**<Single pixel scan.*/

  void      		Progress( int ); //*SIGNAL*								

  GResonseCurve         responseCurve;										/**<Response curve and resolution graphs*/
  TGraphErrors  	*g_threshScan;										/**<Threshold scan graph*/
  TH1D          	*h_pe;											/**<Number of detected photons histogram*/
  TH1D          	*h_dr;											/**<Number of thermal pulses histogram*/
  TH1D          	*h_xt;											/**<SNumber of cross-talk histogram*/
  TH1D          	*h_ap;											/**<Number of after-pulses histogram*/
  TH1D          	*h_QDC;											/**<Charge spectrum histogram*/
  TH1D          	*h_TDC;											/**<Time spectrum histogram*/

  bool   		cancel;											/**<Cancels simulation process*/
  double 		progress;										/**<Progress of simulation*/
  
 private:

  bool  		Check();
  
  double 		pedestal;
  
  int 			nTDC_channels;
  
  TRandom3      	r;
  HitMatrix*    	hitMatrix;
  vector<double>	hit;
  sipmMC*              	sipm;
  PhotonSource*       	photonSource;
  TH1D*               	waveform;
  
  double 		discriMinTime;
  double 		discriWidth;
  
  int                  	plast;
  
};

#endif
