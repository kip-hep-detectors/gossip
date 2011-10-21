/********** Version 1.2 **********/

#include "daqMC.hh"
#include "HitMatrix.hh"
#include "drTDCspec.h"
#include <TRandom1.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <TH2.h>
#include <TMath.h>
#include <TFile.h>
#include <ctime>
#include <vector>

using namespace std;


daqMC::daqMC() : sipm(0), photonSource(0)
{
  plast=0;
  cancel=false;
  
  r.SetSeed(time (NULL));

  h_QDC = new TH1D();
  h_QDC->SetNameTitle("Charge Spectrum","Charge Spectrum");
  h_QDC->GetXaxis()->SetTitle("QDC Channels");
  h_QDC->GetYaxis()->SetTitle("# Entries");
  
  SetQDCChannels(1024);
  SetPedestal(50);
  
  discriMinTime= 1.5;
  discriWidth = 5;
  
  h_TDC = new TH1D();
  h_TDC->SetNameTitle("Time Spectrum","Time Spectrum");
  h_TDC->GetXaxis()->SetTitle("Time [ns]");
  h_TDC->GetYaxis()->SetTitle("# Entries");
  
//   nTDC_channels=1024;
  
  h_pe = new TH1D();
  h_pe->SetNameTitle("PE","PE");
  h_pe->SetLineColor(1);
  h_dr = new TH1D();
  h_dr->SetNameTitle("DR","DR");
  h_dr->SetLineColor(2);
  h_xt = new TH1D();
  h_xt->SetNameTitle("XT","XT");
  h_xt->SetLineColor(3);
  h_ap = new TH1D();
  h_ap->SetNameTitle("AP","AP");
  h_ap->SetLineColor(4);
  
//   sipm = new sipmMC();
//   photonSource = new PhotonSource();
}


daqMC::~daqMC()
{
    delete hitMatrix;
    delete sipm;
    delete photonSource;
    delete waveform;
    delete h_pe;
    delete h_dr;
    delete h_xt;
    delete h_ap;
    delete h_QDC;
    delete h_TDC;
    delete g_threshScan;
    delete g_Response;
    delete g_ResNgamma;
}


void daqMC::Progress(int p)
{
  if(p!=plast) Emit("Progress(int)",p+1);	// p = Progress in %
  plast=p;
}


bool daqMC::Check()
{
  if(sipm && photonSource) return true;
  else if (!sipm){ cout << "No SiPM loaded!" << endl; return false; }
  else if (!photonSource){ cout << "No PhotonSource loaded!" << endl; false; }
}


void daqMC::Statistic( int N )
{
  
  if(!Check());
  
  h_pe->Reset("M");
  h_dr->Reset("M");
  h_xt->Reset("M");
  h_ap->Reset("M");
   
  h_pe->SetBins(1000,0,1000);
  h_dr->SetBins(1000,0,1000);
  h_xt->SetBins(1000,0,1000);
  h_ap->SetBins(1000,0,1000);
  
  int max=0;
  int ymax=0;
  
  for(int i=0;i<N;i++)
  {
    sipm->Generate(photonSource->GeneratePhotons());
    hitMatrix = sipm->GetHitMatrix();
    
    int npe = hitMatrix->nHits(PE);
    int ndr = hitMatrix->nHits(DR);
    int nxt = hitMatrix->nHits(XT);
    int nap = hitMatrix->nHits(AP);
    
    h_pe->Fill(hitMatrix->nHits(PE));
    h_dr->Fill(hitMatrix->nHits(DR));
    h_xt->Fill(hitMatrix->nHits(XT));
    h_ap->Fill(hitMatrix->nHits(AP));
    if(npe>=max) max=npe;
    if(ndr>=max) max=ndr;
    if(nxt>=max) max=nxt;
    if(nap>=max) max=nap; 
  }
  
  h_pe->SetBins(max,0,max);
  h_dr->SetBins(max,0,max);
  h_xt->SetBins(max,0,max);
  h_ap->SetBins(max,0,max);
  
  if(h_pe->GetMaximum()>=ymax) ymax = h_pe->GetMaximum();
  if(h_dr->GetMaximum()>=ymax) ymax = h_dr->GetMaximum();
  if(h_xt->GetMaximum()>=ymax) ymax = h_xt->GetMaximum();
  if(h_ap->GetMaximum()>=ymax) ymax = h_ap->GetMaximum();
  
  h_pe->SetMaximum(ymax*1.2);
  
  h_pe->Draw("HIST E0");
  h_dr->Draw("HIST E0 SAME");
  h_xt->Draw("HIST E0 SAME");
  h_ap->Draw("HIST E0 SAME");
   
}


TH1D* daqMC::QDCSpectrum( int N )
{
  
  if(!Check()) return h_QDC;
  
  h_QDC->Reset("M");

  for(int i=0;i<N;i++)
  {
    if(cancel==true) break;
    Progress(i*100/N);
    
    double charge = sipm->Generate(photonSource->GeneratePhotons());
    QDC(charge);
  }
  
  h_QDC->Draw("HIST E0");
  
  return h_QDC;
}


TH1D* daqMC::TDCSpectrum( int N )
{

//Simple: Use timestaps

  double deadtime = 0;

  if(!Check()) return h_TDC;
  
  h_TDC->Reset("M");
  h_TDC->SetBins(65000,0,65000);
  
  photonSource->SetNgamma(0);
  sipm->SetGate(65000);
  
  PhotonList empty = photonSource->GeneratePhotons();

  while(h_TDC->GetEntries()<N)
  {
    if(cancel==true) break;
    Progress(h_TDC->GetEntries()*100/N);
    
    double tlast=0;
    sipm->Generate(empty);
    hitMatrix = sipm->GetHitMatrix();
    
    for(int i=0;i<hitMatrix->nHits();i++)
    {
      hit.clear();
      hit=hitMatrix->GetHit(i);
      if(hit[TYPE]==XT || hit[TYPE]==PE || hit[TIME]-tlast<=deadtime) continue;
      h_TDC->Fill(hit[TIME]-tlast);
      tlast=hit[TIME];
      if(h_TDC->GetEntries()>=N) break;
    }
  }

//Fit

  TF1 *fit = new TF1("fit",drTDCspec,0,65000,6);
  fit->SetLineColor(2);
//   fit->SetParNames("N","Pap_s","Pap_f","tau_dr","tau_ap_s","tau_ap_f","Pxt");
//   fit->SetParameters(N,sipm->Pap_s,sipm->Pap_f,sipm->tau_dr,sipm->tau_ap_s,sipm->tau_ap_f,sipm->Pxt);
  fit->SetParLimits(0,1e4,1e8);
  fit->SetParLimits(1,1e4,1e8);
  fit->SetParLimits(2,1e4,1e8);
//   fit->SetParLimits(3,0,1e5);
//   fit->SetParLimits(4,0,1e4);
//   fit->SetParLimits(5,0,1e4);
//   fit->SetParLimits(6,0,1);

//   fit->FixParameter(0,N);
//   fit->FixParameter(1,sipm->Pap_s);
//   fit->FixParameter(2,sipm-Pap_f);
//   fit->FixParameter(3,sipm->tau_dr);
//   fit->FixParameter(4,sipm->tau_ap_s);
//   fit->FixParameter(5,sipm->tau_ap_f);
//   fit->FixParameter(6,0/*sipm->Pxt*/);

  fit->SetParameters(1e7,1e5,1e5,1e3,160,50);

  fit->SetLineColor(2);
  h_TDC->Fit("fit","M",""/*,2*sipm->tau_recovery,sipm->tau_dr*3*/);
//   cout << "\n RedChi2: " << fit->GetChisquare()/fit->GetNDF() << endl;

  double d = fit->GetParameter(0);
  double s = fit->GetParameter(1);
  double f = fit->GetParameter(2);
  
  cout << "slow: " << s/(s+d+f) << " - fast: " << f/(s+d+f) << endl;

  
  h_TDC->Draw("HIST E0");
  fit->Draw("SAME");
  
  return h_TDC;
}


TGraphErrors* daqMC::ThreshScan( double gate, double tstart, double tstop, double tstep )
{

  photonSource->SetNgamma(0);
  sipm->SetGate(gate);
  
  PhotonList empty = photonSource->GeneratePhotons();

  int nthresh = (tstop-tstart)/tstep+1;
  const int N = 10000;//(tstop-tstart)/tstep+1;
  
  double thresh;
  double V[N]={0}, count[N]={0};
  double V_err[N]={0}, count_err[N]={0};
  double tlast[N]={0};
  
  sipm->Generate(empty);
  waveform = sipm->GetWaveform();
  hitMatrix = sipm->GetHitMatrix();
  
  int imax = sipm->GetPulseShape()->GetMaximumBin();
  
  int nbins = waveform->GetNbinsX();
  double resolution = waveform->GetBinWidth(1);
  double tmin = waveform->GetBinCenter(1)-0.5*resolution;
  double tmax = tmin+resolution*nbins;
  
  int n;
  
  //----------------------------------------------------------------------------//
  
//   for(int i=0;i<hitMatrix->nHits();i++)
//   {
//     if(cancel==true) break;
//     Progress(100*i/hitMatrix->nHits());
//     
//     hit.clear();
//     hit=hitMatrix->GetHit(i);
// 
//     double amp = waveform->GetBinContent(hit[TIME]/resolution+imax+1);
// 
//     n=0;
//     for(thresh=tstart;thresh<=tstop;thresh+=tstep)
//     {
//       if(i==0) V[n]=thresh;
//       if(amp>=thresh && hit[TIME]-tlast[n]>=discriWidth+discriDeadTime)
//       {
// 	count[n]++;
// 	tlast[n]=hit[TIME];
//       }
//       n++;
//     }
//   }
    
  //----------------------------------------------------------------------------//
  
//   bool on[N]={0};
//   double toff[N]={0};
//   double ton[N]={0};
//   
//   TH1D *h = new TH1D("h","h",nbins,0,nbins*resolution);
//   
//   for(int i=1;i<=nbins;i++)
//   {
//     if(cancel==true) break;
// 
//     if((i-1)%(nbins/100)==0) Progress((i-1)/(nbins/100));
//     
//     double amp = waveform->GetBinContent(i);
//     double time = i*resolution;
//     
//     n=0;
//     for(thresh=tstart;thresh<=tstop;thresh+=tstep)
//     {
//       if(i==1) V[n]=thresh;
//       if(amp>=thresh && on[n]==false && time-toff[n]>=discriDeadTime)
//       {
// 	count[n]++;
// 	on[n]=true;
// 	ton[n]=time;
//       }
//       if(amp<thresh && on[n]==true && time-ton[n]>=discriWidth)
//       {
// 	on[n]=false;
// 	toff[n]=time;
//       }
//       if(thresh==tstart)h->SetBinContent(i,on[0]*tstart);
//       n++;
//     }
//   }
  
  //----------------------------------------------------------------------------//  
  
//   for(int i=0;i<n;i++)
//   {
//       count_err[i]=sqrt(count[i]);
//       V_err[i]=0;
//   }

  //----------------------------------------------------------------------------//

    vector<int> on, off;
    double toff[N]={0};
    double ton[N]={0};

    TH1D *h = new TH1D("h","h",nbins,0,nbins*resolution);
    
    n=0;
    for(thresh=tstart;thresh<=tstop;thresh+=tstep)
    {
      if(cancel==true) break;
      
      if((n*50)%nthresh==0) Progress(100*n/nthresh);
      
      for(int i=1;i<=nbins;i++)
      {
	double amp = waveform->GetBinContent(i);
	double time = i*resolution;
	
	if(i==1)
	{
	  on.clear();
	  off.clear();
	  on.push_back(-1);
	  off.push_back(-1);
	}
	
	if(amp>=thresh && off.back()>=on.back())
	{
	  on.push_back(i);
	  if((on.back()-off.back())*resolution<=discriMinTime)
	  {
	    on.pop_back();
	    off.pop_back();
	  }
	}
	
	if(amp<thresh && off.back()<on.back())
	{
	  off.push_back(i);
	  if((off.back()-on.back())*resolution<=discriMinTime)
	  {
	    on.pop_back();
	    off.pop_back();
	  }
	  
	  if((i-on.back())*resolution<discriWidth)
	  {
	    off.pop_back();
	  }
	}
      }
      count[n]=on.size()-1;
      V[n]=thresh;
      n++;
    }

// for(int i=1;i<on.size();i++)
// {
//   for(int j=on.at(i);j<off.at(i);j++)
//   {
//     h->SetBinContent(j,thresh);
//   }
// }

  //----------------------------------------------------------------------------//

  g_threshScan = new TGraphErrors(n,V,count,V_err,count_err);

  g_threshScan->SetTitle("Threshold Scan");
  g_threshScan->Draw("ALP");
//   waveform->Draw();
//   h->Draw("SAME");
//   h->SetLineColor(2);
  return g_threshScan;
}


TH1D* daqMC::Scope( TH1D* waveform )
{
  waveform->SetTitle("Waveform");
  return waveform;
}


void daqMC::QDC( double charge )
{
  h_QDC->Fill(charge+pedestal);
}


TH1D* daqMC::Discriminator( TH1D* waveform, double threshold )
{
  
  int ion=-1;
  int ioff=-1;
  double amp;
  bool on=false;
  
  double deadtime=5;
  double width=10;
  
  double tlast=0;
  double nCounts=0;
  
  int nbins = waveform->GetNbinsX();
  double resolution = waveform->GetBinWidth(1);
  double tmin = waveform->GetBinCenter(1)-0.5*resolution;
  double tmax = tmin+resolution*nbins;

  TH1D* h_discri = new TH1D("h_discri","h_discri",nbins,tmin,tmax);
  TH1D* h_timeSpec = new TH1D("h_timeSpec","h_timeSpec",nbins,tmin,tmax);
  
  for(int i=0;i<=nbins;i++)
  {
    amp = waveform->GetBinContent(i);
    if(amp>=threshold)
    {
      if((i-ioff)*resolution>=deadtime || ioff==-1)
      {
	if(on==false)
	{
	  h_timeSpec->Fill(i*resolution-tlast);
	  tlast = i*resolution;
	  nCounts++;
	  ion=i;
	}
	h_discri->SetBinContent(i,threshold);
	on=true;
      }
      else h_discri->SetBinContent(i,0);
    }
    
    if(amp<threshold)
    {
      if((i-ion)*resolution>=width || ion==-1)
      {
	if(on==true) ioff=i;
	h_discri->SetBinContent(i,0);
	on=false;
      }
      else h_discri->SetBinContent(i,threshold);
    }    
  }
  
  h_discri->SetLineColor(2);

  return h_discri;
}


TGraphErrors* daqMC::DynamicRange( int N, double Ngamma_max, double Ngamma_step )
{

  const int Npoints = Ngamma_max/Ngamma_step+1;

  double Ndone=0;
  double Ntot=0;

  for(int i=0;i<Npoints;i++)
  {
    Ntot+=Ngamma_max/Npoints*i;
  }

  const int Np = 100000;

  double Ngamma[Np]={0}, Mean[Np]={0}, Mean_err[Np], RMS[Np]={0}, RMS_err[Np]={0}, Sigma[Np]={0};

  for(int i=0; i<Npoints; i++)
  {
    Ngamma[i]=i*Ngamma_step;
    photonSource->SetNgamma(Ngamma[i]);
    
    h_QDC->Reset("M");
    
    for(int n=0;n<N;n++)
    {
      if(cancel==true) break;
      double charge = sipm->Generate(photonSource->GeneratePhotons());
      QDC(charge);
    }
    
    Mean[i]=(h_QDC->GetMean()-pedestal)/sipm->gain;
    RMS[i]=h_QDC->GetRMS()/sipm->gain;
    Mean_err[i]=h_QDC->GetMeanError()/sipm->gain;
    RMS_err[i]=h_QDC->GetRMSError()/sipm->gain;
    
    Ndone+=Ngamma[i];
    Progress(100*Ndone/Ntot);
  }

  g_Response = new TGraphErrors(Npoints,Ngamma,Mean,0,Mean_err);
  g_Response->SetMarkerStyle(20);

  TF1 *fit = new TF1("fit","[0]*(1-TMath::Exp(-[1]*x/[0]))",0,Ngamma_max);
  fit->SetParameters(sipm->Npix,sipm->PDE);
  g_Response->Fit("fit","M","",0,Ngamma_max);

  g_Response->SetTitle("Response");
  g_Response->Draw("ALP");

  ///Resolution

  for(int i=0;i<Npoints;i++)
  {
    Sigma[i]=RMS[i]*fit->GetParameter(0)/fit->GetParameter(1)/(fit->GetParameter(0)-Mean[i])/Ngamma[i];
  }

  //g_ResNgamma = new TGraph(Npoints,Ngamma,Sigma);
  g_ResNgamma = new TGraphErrors(Npoints,Ngamma,RMS,0,RMS_err);
  g_ResNgamma->SetMarkerStyle(20);

  delete fit;

  TCanvas *cc2 = new TCanvas("cc2","cc2",750,0,700,500);
  g_ResNgamma->SetTitle("RMS");
  g_ResNgamma->Draw("ALP");

  return g_ResNgamma;
}


void daqMC::ScanCT(int N){
  
//     TH2D *h_scan = new TH2D("h_scan","h_scan",sipm->Npx,0,sipm->Npx,sipm->Npy,0,sipm->Npy);
//     for(int x=0;x<sipm->Npx;x++){
//       for(int y=0;y<sipm->Npy;y++){
// 	Progress((sipm->Npy*x+y+1)*100/(sipm->Npx*sipm->Npy));
// 	int CThits=0;
// 	for(int i=0; i<N; i++){
// 	  sipm->GeneratePhotons("scan",false,x,y);
// 	  sipm->Generate(1);
// 	  CThits+=sipm->matrix->nHits(2);
// 	}
// 	h_scan->SetBinContent(x+1,y+1,CThits);
//       }
//     }
//     cout << "Done..." << endl;
//     TCanvas *c_scan = new TCanvas("c_scan","c_scan",0,600,400,400);
//     h_scan->Draw("colz");
}
