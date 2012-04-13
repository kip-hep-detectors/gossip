#include "sipmMC.hh"
#include <TROOT.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <TH1.h>
#include <TMath.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <sys/time.h>

using namespace std;

sipmMC* sipmMC::fInstance = NULL;

sipmMC::sipmMC()
{
  if(fInstance)
  {
    return;
  }
  else
  {
    this->fInstance = this;
    
    NpixX=10;
    NpixY=10;
    xSipm=1;
    ySipm=1;
    gate=300;
    tau_recovery=10;
    PDE=0.1;
    Pxt=0.1;
    Pap_s=0.1;
    Pap_f=0.1;
    tau_ap_s=150;
    tau_ap_f=40;
    tau_dr=2000;
    gain=20;
    ENF=2;
    EN=2;
    signalAmp = 20;
    noiseRMS = 2;

    timeval time;
    gettimeofday(&time,NULL);    
    r.SetSeed(time.tv_usec);
    
    hitMatrix = new HitMatrix();

    h_geometry = new TH2I();
    h_geometry->SetNameTitle("h_geometry","h_geometry");

    waveform = new TH1D();
    h_pulseShape = new TH1D();

    SetPulseShape();
    SetGeometry("square");
  }

}


sipmMC::~sipmMC()
{
  delete hitMatrix;
  delete h_geometry;
  delete waveform;
  delete h_pulseShape;
}


void sipmMC::Reset()
{
  charge.all = 0;
  charge.pe  = 0;
  charge.dr  = 0;
  charge.ap  = 0;
  charge.ct  = 0;
  charge.en  = 0;
  charge.enf = 0;
}


void sipmMC::GetParaFile( const char* filename )
{

  string para, pm, dump;
  ifstream in(filename);
  
  double tau1F, tau2F, gateF;
  
  while(1)
  {
    in >> para;
    if(para == "PDE") in >> PDE;
    else if(para == "Gain") in >> gain;
    else if(para == "TauDR") in >> tau_dr;
    else if(para == "AP_s") in >> Pap_s;
    else if(para == "TauAP_s") in >> tau_ap_s;
    else if(para == "AP_f") in >> Pap_f;
    else if(para == "TauAP_f") in >> tau_ap_f;
    else if(para == "XT") in >> Pxt;
    else if(para == "ENF") in >> ENF;
    else if(para == "EN") in >> EN;
    else if(para == "TauRec") in >> tau_recovery;
    else if(para == "Npx") in >> NpixX;
    else if(para == "Npy") in >> NpixY;
    else if(para == "SizeX") in >> xSipm;
    else if(para == "SizeY") in >> ySipm;
    else if(para == "NoiseRMS") in >> noiseRMS;
    else if(para == "SignalAmp") in >> signalAmp;
    else if(para == "Tau1") in >> tau1F;
    else if(para == "Tau2") in >> tau2F;
    else if(para == "Gate") in >> gateF;
    else getline(in, dump);
      
    SetPulseShape(tau1F, tau2F);
    SetGate(gateF);
    
    if(!in.good()) break;
  }

  in.close();

}


void sipmMC::SetGate( double Gate, bool gateCut )
{
  gate = Gate;
  if(customPulse==false) SetPulseShape(tau1, tau2, resolution, cutoff);
  hitMatrix->SetGate(gate, gateCut);
}


void sipmMC::SetGeometry(string Geometry)
{
  h_geometry->Reset("M");

  if(Geometry=="square")
  {
    h_geometry->SetBins(NpixX,0,NpixX,NpixY,0,NpixY);
    
    for(int x=0;x<NpixX;x++)
    {
      for(int y=0;y<NpixY;y++)
      {
	h_geometry->Fill(x,y);
      }
    }
  }
  else cout << "Error! Geometry not found" << endl;

  Npix = h_geometry->Integral();

  hitMatrix->SetGeometry(h_geometry);
}


void sipmMC::SetGeometry( TH2I* hgeometry )
{
  h_geometry->Reset("M");

  h_geometry = hgeometry;

  Npix = h_geometry->Integral();
  
  hitMatrix->SetGeometry(h_geometry);
}


void sipmMC::SetPulseShape( double Tau1, double Tau2, double Resolution, double cutOff )
{
  customPulse = false;
  resolution = Resolution;
  tau1 = Tau1;
  tau2 = Tau2;

  cutoff = cutOff;

  h_pulseShape->Reset("M");
  h_pulseShape->SetBins(gate/resolution,0,gate);
  double t;

  int i=1;
  while(i<=gate/resolution)
  {
    t=(i-1)*resolution;
    double amp = signalAmp/(pow(tau1/tau2,-tau2/(tau1-tau2))-pow(tau1/tau2,-tau1/(tau1-tau2)))*(exp(-t/tau1) -exp(-t/tau2));
    h_pulseShape->SetBinContent(i,amp);
    if(t>=log(tau1/tau2)/(1/tau2-1/tau1) && amp<signalAmp*cutoff) break;
    i++;
  }

  nBinsPulseShape = i-1;
  h_pulseShape->SetBins(nBinsPulseShape,0,nBinsPulseShape*resolution);
  pulseIntegral = h_pulseShape->Integral();
}


void sipmMC::SetPulseShape( TH1* PulseShape )
{
  customPulse = true;
  resolution = PulseShape->GetBinWidth(1);

  h_pulseShape = PulseShape;
  pulseIntegral = h_pulseShape->Integral();
  nBinsPulseShape = h_pulseShape->GetNbinsX();

  h_pulseShape->Scale(1/h_pulseShape->GetMaximum());
}


void sipmMC::ImportPhotons(PhotonList photons)
{
  
  photonList = photons;
  
  for(unsigned int i=0;i<photonList.size();i++)
  {
    double x = photonList[i][X];
    double y = photonList[i][Y];

    if( x>=-xSipm/2. && x<=xSipm/2. && y>=-ySipm/2. && y<=ySipm/2.)
    {
      photonList[i][X] = (int)(((x+0.5*xSipm))/xSipm*h_geometry->GetNbinsX());
      photonList[i][Y] = (int)(((y+0.5*ySipm))/ySipm*h_geometry->GetNbinsY());
    }
    else
    {
      photonList.erase(photonList.begin()+i);
      i--;
    }
  }
  
}


void sipmMC::InitHitMatrix()
{
  hitMatrix->Init();
}


GCharge sipmMC::Generate( PhotonList photons )
{
  Reset();
  ImportPhotons(photons);	//Übersetzte Potonen aus PhotonList auf Pixelbasis
  InitHitMatrix();		//Geben Parameter an HitMatrix weiter


  if(PDE>1 || PDE<0)
  {
    PDE=0;
    cout << "ERROR: PDE not in between 0 and 1. Set PDE to 0!" << endl;
  }
  if(gain<0)
  {
    gain=0;
    cout << "ERROR: Gain smaller than zero! Set Gain to 0!" << endl;
  }
  if(tau_dr<0)
  {
    tau_dr=0;
    cout << "ERROR: tau_dr smaller than zero! Set tau_dr to 0!" << endl;
  }
  if(Pap_s>1 || Pap_s<0)
  {
    Pap_s=0;
    cout << "ERROR: Pap_s not in between 0 and 1. Set Pap_s to 0!" << endl;
  }
  if(tau_ap_s<0)
  {
    tau_ap_s=0;
    cout << "ERROR: tau_ap_s smaller than zero! Set tau_ap_s to 0!" << endl;
  }
  if(Pap_f>1 || Pap_f<0)
  {
    Pap_f=0;
    cout << "ERROR: Pap_f not in between 0 and 1. Set Pap_f to 0!" << endl;
  }
  if(tau_ap_f<0)
  {
    tau_ap_f=0;
    cout << "ERROR: tau_ap_f smaller than zero! Set tau_ap_f to 0!" << endl;
  }
  if(Pxt>1 || Pxt<0)
  {
    Pxt=0;
    cout << "ERROR: Pxt not in between 0 and 1. Set Pxt to 0!" << endl;
  }  
  if(ENF<0)
  {
    ENF=0;
    cout << "ERROR: ENF smaller than zero! Set ENF to 0!" << endl;
  }
  if(EN<0)
  {
    EN=0;
    cout << "ERROR: EN smaller than zero! Set EN to 0!" << endl;
  }
  
  ///operation over-voltage:
  double Vop = 1;		///has to be 1!!! (only for custom voltage dependence of parameters important)

  ///detected photons:
  for(unsigned int i=0;i<photonList.size();i++)
  {
    if(r.Rndm()<PDE) hitMatrix->AddHit(photonList[i][X],photonList[i][Y],photonList[i][TIME],PE);
  }

  ///darkrate:
  if(tau_dr!=0)
  {
    double time=0;
    int x,y;
    
    while(1)
    {
      time+=r.Exp(tau_dr);
      while(1)
      {
	x=r.Rndm()*h_geometry->GetNbinsX();
	y=r.Rndm()*h_geometry->GetNbinsY();
	if(h_geometry->GetBinContent(x+1,y+1)==1) break;
      }
      hitMatrix->AddHit(x,y,time,DR);
      if(time>=gate) break;	//letzter hit liegt nach dem gate für time distribution
    }
  }

  /// crosstalk & afterpulses (& amplituden):
  //double ct_length = 80;
  //double pitch = (1000/Npx + 1000/Npy)*0.5;	//gemittelt über xpitch und ypitch
  double q = (1-TMath::Power(1-Pxt,0.25));

  for(int i=0;i<hitMatrix->nHits();i++)
  {    
    hit.clear();
    hit=hitMatrix->GetHit(i);
    int x=hit[X];
    int y=hit[Y];
    double time=hit[TIME];

    ///pixel recovery:
    double Vover;
    double tlast=hitMatrix->GetPreviousTime(i);
    if(tlast!=-1 && tau_recovery>0) Vover = Vop*(1-TMath::Exp(-(time-tlast)/tau_recovery));
    else Vover = Vop;

    ///PDE reduced due to pixel recovery:
    if(hit[TYPE]==PE && r.Rndm()>=Vover/Vop)
    {
      hitMatrix->EraseHit(hit);
      i--;
      continue;
    }
    
    ///Dark-rate reduced due to pixel recovery:
    if(hit[TYPE]==DR && r.Rndm()>=Vover/Vop)
    {
      hitMatrix->EraseHit(hit);
      i--;
      continue;
    }
   
    ///amplitude:
    double enfNoise = r.Gaus(0,ENF*Vover/Vop);
    double amplitude= gain + enfNoise;
    if(amplitude<0) amplitude=0;
    amplitude=amplitude*Vover/Vop;	///recovery
    hitMatrix->SetAmplitude(i,amplitude);
    
    double overflow;
    if(customPulse==true)  overflow = h_pulseShape->Integral((gate-time)/resolution+1,nBinsPulseShape)/pulseIntegral*amplitude;
    if(customPulse==false) overflow = amplitude/(tau1-tau2)*(tau1*exp(-(gate-time)/tau1)-tau2*exp(-(gate-time)/tau2));
    charge.all+=amplitude-overflow;

    if(amplitude>0)
    {
      if(hit[TYPE]==PE) charge.pe+=gain*Vover/Vop*(1-overflow/amplitude);	///formular from amplitude = (gain + enf)*Vover/Vop - overflow
      if(hit[TYPE]==CT) charge.ct+=gain*Vover/Vop*(1-overflow/amplitude);	///                        = (gain + enf)*Vover/Vop - (gain + enf)*Vover/Vop*overflow/amplitude
      if(hit[TYPE]==AP) charge.ap+=gain*Vover/Vop*(1-overflow/amplitude);	///                        = gain*Vover/Vop*(1-overflow/amplitude) + enf*Vover/Vop*(1-overflow/amplitude)
      if(hit[TYPE]==DR) charge.dr+=gain*Vover/Vop*(1-overflow/amplitude);
			charge.enf+=enfNoise*Vover/Vop*(1-overflow/amplitude);
    }
    
    if(hitMatrix->GetProcessed(i)==false)
    {
      ///crosstalk:
      if(Pxt!=0)
      {
	//direct
	if(r.Rndm()<q*Vover/Vop) hitMatrix->AddHit(x+1,y,hit[TIME],CT);
	if(r.Rndm()<q*Vover/Vop) hitMatrix->AddHit(x-1,y,hit[TIME],CT);
	if(r.Rndm()<q*Vover/Vop) hitMatrix->AddHit(x,y+1,hit[TIME],CT);
	if(r.Rndm()<q*Vover/Vop) hitMatrix->AddHit(x,y-1,hit[TIME],CT);

	//diagonal
	// 	if(r.Rndm()<q*Vover/Vop*TMath::Exp(-sqrt(2)*pitch/ct_length)) hitMatrix->AddHit(x+1,y+1,hit[TIME],CT);
	// 	if(r.Rndm()<q*Vover/Vop*TMath::Exp(-sqrt(2)*pitch/ct_length)) hitMatrix->AddHit(x+1,y-1,hit[TIME],CT);
	// 	if(r.Rndm()<q*Vover/Vop*TMath::Exp(-sqrt(2)*pitch/ct_length)) hitMatrix->AddHit(x-1,y+1,hit[TIME],CT);
	// 	if(r.Rndm()<q*Vover/Vop*TMath::Exp(-sqrt(2)*pitch/ct_length)) hitMatrix->AddHit(x-1,y-1,hit[TIME],CT);
      }
      
      ///afterpulses:
      //carrier stays trapped if new avalanche occurs before it's released
      if(Pap_s!=0)
      {
	int Nap=r.Poisson(Pap_s*Vover/Vop);
	for(int i_ap=0;i_ap<Nap;i_ap++)
	{
	  double time_ap = r.Exp(tau_ap_s);
	  if(hit[TIME]<=gate) hitMatrix->AddHit(x,y,hit[TIME]+time_ap,AP);	//hit[TIME]<=gate: erlaube noch ein hit nach dem gate für time distribution
	}
      }
      if(Pap_f!=0)
      {
	int Nap=r.Poisson(Pap_f*Vover/Vop);
	for(int i_ap=0;i_ap<Nap;i_ap++)
	{
	  double time_ap = r.Exp(tau_ap_f);
	  if(hit[TIME]<=gate) hitMatrix->AddHit(x,y,hit[TIME]+time_ap,AP);	//hit[TIME]<=gate: erlaube noch ein hit nach dem gate für time distribution
	}
      }
      
      ///mark hit as processed:
      hitMatrix->HitProcessed(i);
    }
  }

  ///Add electronic noise:
  double eNoise = r.Gaus(0,EN);
  charge.en = eNoise;
  charge.all += eNoise;

  return charge;
}


TH1D* sipmMC::GetWaveform()
{

  waveform->Reset("M");
  waveform->SetBins(gate/resolution,0,gate);

  for(int i=0;i<hitMatrix->nHits();i++)
  {
    double time = hitMatrix->GetHit(i)[TIME];
    double amplitude = hitMatrix->GetHit(i)[AMPLITUDE];
    
    if(customPulse==true)
    {
      int i_ps=1;
      while(i_ps<=nBinsPulseShape && i_ps*resolution+time<=gate)
      {
	int i_wf = time/resolution + i_ps;
	waveform->AddBinContent(i_wf,h_pulseShape->GetBinContent(i_ps)*amplitude/gain*signalAmp);
	i_ps++;
      }
    }
    if(customPulse==false)
    {
      int i_wf=time/resolution+1;
      int j=0;
      double t=0;
      while(i_wf<=gate/resolution)
      {
	t=j*resolution;
	double amp = amplitude/gain*signalAmp/(pow(tau1/tau2,-tau2/(tau1-tau2))-pow(tau1/tau2,-tau1/(tau1-tau2)))*(exp(-t/tau1) -exp(-t/tau2));
	waveform->AddBinContent(i_wf,amp);
	i_wf++;
	j++;
	if(t>=log(tau1/tau2)/(1/tau2-1/tau1) && amp<signalAmp*cutoff) break;
      }
    }
  }

  for(int i=1;i<=waveform->GetNbinsX();i++)
  {
    waveform->AddBinContent(i,r.Gaus(0,noiseRMS));
  }

  return waveform;
}
