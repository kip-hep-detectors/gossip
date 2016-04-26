#include "daqMC.h"

#include "HitMatrix.h"

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
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "daqMC::daqMC()" << endl;

	plast=0;
	cancel=false;

	r.SetSeed(0);

	h_QDC = new TH1D();
	h_QDC->SetNameTitle("Charge Spectrum","Charge Spectrum");
	h_QDC->GetXaxis()->SetTitle("QDC Channels");
	h_QDC->GetYaxis()->SetTitle("# Entries");

	SetQDCChannels(1024);
	SetPedestal(50);

	discriMinTime= 1.5;
	discriWidth = 5;

	h_TDC = new TH1D();
	h_TDC->SetNameTitle("TimeSpectrum","TimeSpectrum");
	h_TDC->GetXaxis()->SetTitle("Time [ns]");
	h_TDC->GetYaxis()->SetTitle("# Entries");

	//   nTDC_channels=1024;

	h_pe = new TH1D();
	h_pe->SetNameTitle("PE","PE");
	h_pe->GetXaxis()->SetTitle("Fired Pixels");
	h_pe->GetYaxis()->SetTitle("# Entries");
	h_pe->SetLineColor(1);
	h_pe->SetFillColor(0);
	h_dr = new TH1D();
	h_dr->SetNameTitle("DR","DR");
	h_dr->SetLineColor(2);
	h_dr->SetFillColor(0);
	h_dr->GetXaxis()->SetTitle("Fired Pixels");
	h_dr->GetYaxis()->SetTitle("# Entries");
	h_xt = new TH1D();
	h_xt->SetNameTitle("CT","CT");
	h_xt->SetLineColor(3);
	h_xt->SetFillColor(0);
	h_xt->GetXaxis()->SetTitle("Fired Pixels");
	h_xt->GetYaxis()->SetTitle("# Entries");
	h_ap = new TH1D();
	h_ap->SetNameTitle("AP","AP");
	h_ap->SetLineColor(4);
	h_ap->SetFillColor(0);
	h_ap->GetXaxis()->SetTitle("Fired Pixels");
	h_ap->GetYaxis()->SetTitle("# Entries");

	g_threshScan = new TGraph();
	g_threshScan->SetTitle("Threshold Scan");

	responseCurve.response = new TGraphErrors();
	responseCurve.responsePE = new TGraphErrors();
	responseCurve.responseDR = new TGraphErrors();
	responseCurve.responseAP = new TGraphErrors();
	responseCurve.responseCT = new TGraphErrors();
	responseCurve.responseEN = new TGraphErrors();
	responseCurve.responseENF = new TGraphErrors();

	responseCurve.resolution = new TGraphErrors();
	responseCurve.resolutionPE = new TGraphErrors();
	responseCurve.resolutionDR = new TGraphErrors();
	responseCurve.resolutionAP = new TGraphErrors();
	responseCurve.resolutionCT = new TGraphErrors();
	responseCurve.resolutionEN = new TGraphErrors();
	responseCurve.resolutionENF = new TGraphErrors();

	legend = NULL;
	legend2 = NULL;
}


daqMC::~daqMC()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "daqMC::~daqMC()" << endl;

	delete h_pe;
	delete h_dr;
	delete h_xt;
	delete h_ap;
	delete h_QDC;
	delete h_TDC;
	delete g_threshScan;
}


void daqMC::Progress( int p )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "daqMC::Progress( int p )" << endl;

	if(p!=plast) Emit("Progress(int)",p+1);	// p = Progress in %
	plast=p;
}


bool daqMC::Check()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "daqMC::Check()" << endl;

	if(sipm && photonSource) return true;
	else if (!sipm){ cout << "No SiPM loaded!" << endl; return false; }
	else if (!photonSource){ cout << "No PhotonSource loaded!" << endl; return false; }
	else return false;
}


void daqMC::Statistic( int N )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "daqMC::Statistic( int N )" << endl;

	if(!Check()) return;

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
		int nxt = hitMatrix->nHits(CT);
		int nap = hitMatrix->nHits(AP);

		h_pe->Fill(hitMatrix->nHits(PE));
		h_dr->Fill(hitMatrix->nHits(DR));
		h_xt->Fill(hitMatrix->nHits(CT));
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

	///draw legend
	if(legend!=0) delete legend;
	legend = new TLegend(0.65,0.65,0.85,0.85);
	legend->AddEntry(h_pe,"PE");
	legend->AddEntry(h_dr,"DR");
	legend->AddEntry(h_xt,"CT");
	legend->AddEntry(h_ap,"AP");

	legend->Draw();
}


TH1D* daqMC::QDCSpectrum( int N )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "daqMC::QDCSpectrum( int N )" << endl;

	if(!Check()) return h_QDC;

	h_QDC->Reset("M");

	for(int i=0;i<N;i++)
	{
		if(cancel==true) break;
		Progress(i*100/N);

		PhotonList photons = photonSource->GeneratePhotons();

		double charge = sipm->Generate(photons);
		h_QDC->Fill(QDC(charge));
	}

	//   h_QDC->Draw("HIST E0");
	//   h_QDC->Draw("");

	return h_QDC;
}


TH1D* daqMC::TDCSpectrum( int N )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "daqMC::TDCSpectrum( int N )" << endl;

	//Simple: Use timestaps

	double deadtime = 0;

	if(!Check()) return 0;

	h_TDC->Reset("M");

	h_TDC->SetBins(65000,0,65000);

	sipm->SetGate(65000/*,false*/);

	PhotonList empty;
	empty.clear();

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
			if(hit[TYPE]==CT || hit[TYPE]==PE || hit[TIME]-tlast<=deadtime) continue;
			h_TDC->Fill(hit[TIME]-tlast);
			tlast=hit[TIME];
			if(h_TDC->GetEntries()>=N) break;
		}
	}

	//   h_TDC->Draw("HIST E0");
	h_TDC->Draw("");

	//Fit

	//   Float_t tpfitstart = 2000;
	//   Float_t fitstart = 100;
	//   Float_t fitstop = 1000;
	//
	//   Int_t const n = 2.6e6;
	//
	//   ///fitte termische pulse
	//   TF1 *fittp = new TF1("fittp",tpTDCspec,0,n*0.025,2);
	//   fittp->SetParameters(1e7,1e3);
	//
	//   //finde rechte fitgrenze
	//   Int_t tpstop = tpfitstart;
	//   while(1)
	//   {
	//     if(h_TDC->GetBinContent(tpstop)<10) break;
	//     tpstop=tpstop+1;
	//   }
	//
	//   //fitte
	//   h_TDC->Fit("fittp","M0","",tpfitstart,tpstop);
	//
	//   Double_t A3 = fittp->GetParameter(0);
	//   Double_t tau3 = fittp->GetParameter(1);
	//   Double_t A3_err = fittp->GetParError(0);
	//   Double_t tau3_err = fittp->GetParError(1);
	//
	//
	//   //fitte afterpulse
	//   TF1 *fit = new TF1("fit",drTDCspec2,fitstart,n*0.025,6);
	//   fit->SetParNames("N","APslow","APfast","tauDR","tauSlow","taufast");
	//   fit->SetParameters(1e6,0.05,0.1,tau3,2000,200,50);
	//   // 	fit->FixParameter(6,0);
	//   fit->SetParLimits(0,0,1e9);
	//   fit->SetParLimits(1,0,1);
	//   fit->SetParLimits(2,0,1);
	//   fit->SetParLimits(3,0,1e5);
	//   fit->SetParLimits(4,100,1e4);
	//   fit->SetParLimits(5,0,100);
	//   fit->FixParameter(3,tau3);
	//
	//   fit->SetLineColor(2);
	//
	//   h_TDC->Fit("fit","M0","",fitstart,fitstop);
	//   Double_t chi2 = fit->GetChisquare();
	//   Double_t ndf = fit->GetNDF();
	//   cout << "Reduced ChiSquare: " << chi2 << " / " << ndf << " = " << chi2/ndf << endl;
	//
	//   fit->Draw("SAME");

	return h_TDC;
}


TGraph* daqMC::ThreshScan( double gate, double tstart, double tstop, double tstep )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "daqMC::ThreshScan( double gate, double tstart, double tstop, double tstep )" << endl;

	g_threshScan->Set(0);

	photonSource->SetNgamma(0);
	sipm->SetGate(gate);

	PhotonList empty = photonSource->GeneratePhotons();

	int nthresh = (tstop-tstart)/tstep+1;
	//const int N = 10000;//(tstop-tstart)/tstep+1;

	double thresh;
	double counts;
	double rate;
	//double tlast[N]={0};

	sipm->Generate(empty);
	TGraph *g_waveform = sipm->GetWaveform();
	hitMatrix = sipm->GetHitMatrix();

	//   int imax = sipm->GetPulseShape()->GetMaximumBin();

	//double *wf_x = g_waveform->GetX();
	double *wf_y = g_waveform->GetY();

	int nbins = g_waveform->GetN();
	double sampling = sipm->GetSampling();
	//double tmin = wf_x[0];
	//double tmax = wf_x[g_waveform->GetN()-1];

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
	//     double amp = wf_y[hit[TIME]/sampling+imax];
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
	//   TH1D *h = new TH1D("h","h",nbins,0,nbins*sampling);
	//
	//   for(int i=0;i<nbins;i++)
	//   {
	//     if(cancel==true) break;
	//
	//     if(i%(nbins/100)==0) Progress(i/(nbins/100));
	//
	//     double amp = wf_y[i];
	//     double time = wf_x[i];
	//
	//     n=0;
	//     for(thresh=tstart;thresh<=tstop;thresh+=tstep)
	//     {
	//       if(i==0) V[n]=thresh;
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
	//       if(thresh==tstart)h->SetBinContent(i+1,on[0]*tstart);
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
	//double toff[N]={0};
	//double ton[N]={0};

	//TH1D *h = new TH1D("h","h",nbins,0,nbins*sampling);

	n=0;
	for(thresh=tstart;thresh<=tstop;thresh+=tstep)
	{
		if(cancel==true) break;

		if((n*50)%nthresh==0) Progress(100*n/nthresh);

		for(int i=0;i<nbins;i++)
		{
			double amp = wf_y[i];
			//double time = wf_x[i];

			if(i==0)
			{
				on.clear();
				off.clear();
				on.push_back(-1);
				off.push_back(-1);
			}

			if(amp>=thresh && off.back()>=on.back())
			{
				on.push_back(i);
				if((on.back()-off.back())*sampling<=discriMinTime)
				{
					on.pop_back();
					off.pop_back();
				}
			}

			if(amp<thresh && off.back()<on.back())
			{
				off.push_back(i);
				if((off.back()-on.back())*sampling<=discriMinTime)
				{
					on.pop_back();
					off.pop_back();
				}

				if((i-on.back())*sampling<discriWidth)
				{
					off.pop_back();
				}
			}
		}
		counts = on.size();
		rate = counts/gate*1e9;

		g_threshScan->SetPoint(n,thresh,rate);

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

	g_threshScan->Draw("ALP");

	g_threshScan->GetXaxis()->SetTitle("Threshold [mV]");
	g_threshScan->GetYaxis()->SetTitle("Count Rate [Hz]");

	//   g_waveform->Draw();
	//   h->Draw("SAME");
	//   h->SetLineColor(2);
	return g_threshScan;
}


TGraph* daqMC::Scope()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "daqMC::Scope()" << endl;

	if(!Check()) return 0;

	PhotonList photons = photonSource->GeneratePhotons();

	sipm->Generate(photons);
	TGraph *g_waveform = sipm->GetWaveform();

	return g_waveform;
}


double daqMC::QDC( double charge )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "daqMC::QDC( double charge )" << endl;

	return charge+pedestal;
}


TGraph* daqMC::Discriminator( TGraph* g_waveform, double threshold )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "daqMC::Discriminator( TGraph* g_waveform, double threshold )" << endl;

	int ion=-1;
	int ioff=-1;
	double amp;
	bool on=false;

	double deadtime=5;
	double width=10;

	double tlast=0;
	double nCounts=0;

	double *wf_x = g_waveform->GetX();
	double *wf_y = g_waveform->GetY();

	int nbins = g_waveform->GetN();
	double sampling = sipm->GetSampling();
	double tmin = wf_x[0];
	double tmax = wf_x[g_waveform->GetN()-1];

	TH1D* h_discri = new TH1D("h_discri","h_discri",nbins,tmin,tmax);
	TH1D* h_timeSpec = new TH1D("h_timeSpec","h_timeSpec",nbins,tmin,tmax);

	for(int i=0;i<nbins;i++)
	{
		amp = wf_y[i];

		if(amp>=threshold)
		{
			if((i-ioff)*sampling>=deadtime || ioff==-1)
			{
				if(on==false)
				{
					h_timeSpec->Fill(i*sampling-tlast);
					tlast = i*sampling;
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
			if((i-ion)*sampling>=width || ion==-1)
			{
				if(on==true) ioff=i;
				h_discri->SetBinContent(i,0);
				on=false;
			}
			else h_discri->SetBinContent(i,threshold);
		}
	}

	h_discri->SetLineColor(2);

	return 0;	//fix!!!
}


GResonseCurve daqMC::DynamicRange( int N, double Ngamma_max, double Ngamma_step )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "daqMC::DynamicRange( int N, double Ngamma_max, double Ngamma_step )" << endl;

	int nQDC_temp = h_QDC->GetNbinsX();
	SetQDCChannels(1000000);

	const int Npoints = Ngamma_max/Ngamma_step+1;

	double Ndone=0;
	double Ntot=0;

	for(int i=0;i<Npoints;i++)
	{
		Ntot+=Ngamma_max/Npoints*i;
	}

	//const int Np = 100000;

	double Ngamma=0, Mean=0, Mean_err=0, RMS=0, RMS_err=0;

	for(int i=0; i<Npoints; i++)
	{
		Ngamma=i*Ngamma_step;
		photonSource->SetNgamma(Ngamma);

		h_QDC->Reset("M");

		TH1D h_QDC_pe("h_QDC_pe","h_QDC_pe",h_QDC->GetNbinsX(),0,h_QDC->GetNbinsX());
		TH1D h_QDC_dr("h_QDC_dr","h_QDC_dr",h_QDC->GetNbinsX(),0,h_QDC->GetNbinsX());
		TH1D h_QDC_ct("h_QDC_ct","h_QDC_ct",h_QDC->GetNbinsX(),0,h_QDC->GetNbinsX());
		TH1D h_QDC_ap("h_QDC_ap","h_QDC_ap",h_QDC->GetNbinsX(),0,h_QDC->GetNbinsX());
		TH1D h_QDC_enf("h_QDC_enf","h_QDC_enf",h_QDC->GetNbinsX(),0,h_QDC->GetNbinsX());
		TH1D h_QDC_en("h_QDC_en","h_QDC_en",h_QDC->GetNbinsX(),0,h_QDC->GetNbinsX());

		for(int n=0;n<N;n++)
		{
			if(cancel==true) break;
			sipm->Generate(photonSource->GeneratePhotons());
			GCharge charge = sipm->GetChargeDecomp();
			h_QDC->Fill(QDC(charge.all));
			h_QDC_pe.Fill(QDC(charge.pe));
			h_QDC_dr.Fill(QDC(charge.dr));
			h_QDC_ct.Fill(QDC(charge.ct));
			h_QDC_ap.Fill(QDC(charge.ap));
			h_QDC_enf.Fill(QDC(charge.enf));
			h_QDC_en.Fill(QDC(charge.en));
		}

		Mean=(h_QDC->GetMean()-pedestal)/sipm->gain;
		RMS=h_QDC->GetRMS()/sipm->gain;
		Mean_err=h_QDC->GetMeanError()/sipm->gain;
		RMS_err=h_QDC->GetRMSError()/sipm->gain;
		responseCurve.response->SetPoint(i,Ngamma,Mean);
		responseCurve.response->SetPointError(i,0,Mean_err);
		responseCurve.resolution->SetPoint(i,Ngamma,RMS);
		responseCurve.resolution->SetPointError(i,0,RMS_err);

		Mean=(h_QDC_pe.GetMean()-pedestal)/sipm->gain;
		RMS=h_QDC_pe.GetRMS()/sipm->gain;
		Mean_err=h_QDC_pe.GetMeanError()/sipm->gain;
		RMS_err=h_QDC_pe.GetRMSError()/sipm->gain;
		responseCurve.responsePE->SetPoint(i,Ngamma,Mean);
		responseCurve.responsePE->SetPointError(i,0,Mean_err);
		responseCurve.resolutionPE->SetPoint(i,Ngamma,RMS);
		responseCurve.resolutionPE->SetPointError(i,0,RMS_err);

		Mean=(h_QDC_dr.GetMean()-pedestal)/sipm->gain;
		RMS=h_QDC_dr.GetRMS()/sipm->gain;
		Mean_err=h_QDC_dr.GetMeanError()/sipm->gain;
		RMS_err=h_QDC_dr.GetRMSError()/sipm->gain;
		responseCurve.responseDR->SetPoint(i,Ngamma,Mean);
		responseCurve.responseDR->SetPointError(i,0,Mean_err);
		responseCurve.resolutionDR->SetPoint(i,Ngamma,RMS);
		responseCurve.resolutionDR->SetPointError(i,0,RMS_err);

		Mean=(h_QDC_ct.GetMean()-pedestal)/sipm->gain;
		RMS=h_QDC_ct.GetRMS()/sipm->gain;
		Mean_err=h_QDC_ct.GetMeanError()/sipm->gain;
		RMS_err=h_QDC_ct.GetRMSError()/sipm->gain;
		responseCurve.responseCT->SetPoint(i,Ngamma,Mean);
		responseCurve.responseCT->SetPointError(i,0,Mean_err);
		responseCurve.resolutionCT->SetPoint(i,Ngamma,RMS);
		responseCurve.resolutionCT->SetPointError(i,0,RMS_err);

		Mean=(h_QDC_ap.GetMean()-pedestal)/sipm->gain;
		RMS=h_QDC_ap.GetRMS()/sipm->gain;
		Mean_err=h_QDC_ap.GetMeanError()/sipm->gain;
		RMS_err=h_QDC_ap.GetRMSError()/sipm->gain;
		responseCurve.responseAP->SetPoint(i,Ngamma,Mean);
		responseCurve.responseAP->SetPointError(i,0,Mean_err);
		responseCurve.resolutionAP->SetPoint(i,Ngamma,RMS);
		responseCurve.resolutionAP->SetPointError(i,0,RMS_err);

		Mean=(h_QDC_enf.GetMean()-pedestal)/sipm->gain;
		RMS=h_QDC_enf.GetRMS()/sipm->gain;
		Mean_err=h_QDC_enf.GetMeanError()/sipm->gain;
		RMS_err=h_QDC_enf.GetRMSError()/sipm->gain;
		responseCurve.responseENF->SetPoint(i,Ngamma,Mean);
		responseCurve.responseENF->SetPointError(i,0,Mean_err);
		responseCurve.resolutionENF->SetPoint(i,Ngamma,RMS);
		responseCurve.resolutionENF->SetPointError(i,0,RMS_err);

		Mean=(h_QDC_en.GetMean()-pedestal)/sipm->gain;
		RMS=h_QDC_en.GetRMS()/sipm->gain;
		Mean_err=h_QDC_en.GetMeanError()/sipm->gain;
		RMS_err=h_QDC_en.GetRMSError()/sipm->gain;
		responseCurve.responseEN->SetPoint(i,Ngamma,Mean);
		responseCurve.responseEN->SetPointError(i,0,Mean_err);
		responseCurve.resolutionEN->SetPoint(i,Ngamma,RMS);
		responseCurve.resolutionEN->SetPointError(i,0,RMS_err);

		Ndone+=Ngamma;
		Progress(100*Ndone/Ntot);
	}

	responseCurve.response->Set(Npoints);
	responseCurve.responsePE->Set(Npoints);
	responseCurve.responseDR->Set(Npoints);
	responseCurve.responseAP->Set(Npoints);
	responseCurve.responseCT->Set(Npoints);
	responseCurve.responseENF->Set(Npoints);
	responseCurve.responseEN->Set(Npoints);
	responseCurve.resolution->Set(Npoints);
	responseCurve.resolutionPE->Set(Npoints);
	responseCurve.resolutionDR->Set(Npoints);
	responseCurve.resolutionAP->Set(Npoints);
	responseCurve.resolutionCT->Set(Npoints);
	responseCurve.resolutionENF->Set(Npoints);
	responseCurve.resolutionEN->Set(Npoints);

	responseCurve.response->SetMarkerStyle(20);
	responseCurve.response->SetLineColor(1);
	responseCurve.response->SetMarkerColor(1);
	responseCurve.response->SetFillColor(0);
	responseCurve.response->SetTitle("Response");
	responseCurve.response->Draw("ALP");

	responseCurve.response->GetXaxis()->SetTitle("Number of Photons");
	responseCurve.response->GetYaxis()->SetTitle("Number of Fired Pixels");


	responseCurve.responsePE->SetMarkerStyle(20);
	responseCurve.responsePE->SetLineColor(4);
	responseCurve.responsePE->SetMarkerColor(4);
	responseCurve.responsePE->SetFillColor(0);
	responseCurve.responsePE->SetTitle("PE");
	responseCurve.responsePE->Draw("SAMELP");
	responseCurve.responseDR->SetMarkerStyle(20);
	responseCurve.responseDR->SetLineColor(3);
	responseCurve.responseDR->SetMarkerColor(3);
	responseCurve.responseDR->SetFillColor(0);
	responseCurve.responseDR->SetTitle("DR");
	responseCurve.responseDR->Draw("SAMELP");
	responseCurve.responseCT->SetMarkerStyle(20);
	responseCurve.responseCT->SetLineColor(5);
	responseCurve.responseCT->SetMarkerColor(5);
	responseCurve.responseCT->SetFillColor(0);
	responseCurve.responseCT->SetTitle("CT");
	responseCurve.responseCT->Draw("SAMELP");
	responseCurve.responseAP->SetMarkerStyle(20);
	responseCurve.responseAP->SetLineColor(2);
	responseCurve.responseAP->SetMarkerColor(2);
	responseCurve.responseAP->SetFillColor(0);
	responseCurve.responseAP->SetTitle("AP");
	responseCurve.responseAP->Draw("SAMELP");
	responseCurve.responseENF->SetMarkerStyle(20);
	responseCurve.responseENF->SetLineColor(6);
	responseCurve.responseENF->SetMarkerColor(6);
	responseCurve.responseENF->SetFillColor(0);
	responseCurve.responseENF->SetTitle("ENF");
	responseCurve.responseENF->Draw("SAMELP");
	responseCurve.responseEN->SetMarkerStyle(20);
	responseCurve.responseEN->SetLineColor(7);
	responseCurve.responseEN->SetMarkerColor(7);
	responseCurve.responseEN->SetLineColor(7);
	responseCurve.responseEN->SetFillColor(0);
	responseCurve.responseEN->SetTitle("EN");
	responseCurve.responseEN->Draw("SAMELP");


	///draw legend
	if(legend!=0) delete legend;
	legend = new TLegend(0.15,0.55,0.35,0.85);
	legend->AddEntry(responseCurve.response,"Total");
	legend->AddEntry(responseCurve.responsePE,"PE");
	legend->AddEntry(responseCurve.responseDR,"DR");
	legend->AddEntry(responseCurve.responseCT,"CT");
	legend->AddEntry(responseCurve.responseAP,"AP");
	legend->AddEntry(responseCurve.responseENF,"ENF");
	legend->AddEntry(responseCurve.responseEN,"EN");

	legend->Draw();


	//   TF1 *fit = new TF1("fit","[0]*(1-TMath::Exp(-x*[1]/[0]))+[2]",0,Ngamma_max);
	//   fit->SetParameters(sipm->GetNpix(),sipm->PDE,1);
	//   g_Response->Fit("fit","M","",0,Ngamma_max);

	//   FOR(INT I=0;I<NPOINTS;I++)
	//   {
	//     SIGMA[I]=RMS[I]*FIT->GETPARAMETER(0)/FIT->GETPARAMETER(1)/(FIT->GETPARAMETER(0)-MEAN[I])/NGAMMA[I];
	//   }
	//   DELETE FIT;

	//   G_RESNGAMMA = NEW TGRAPH(NPOINTS,NGAMMA,SIGMA);

	TCanvas *cc2 = new TCanvas("cc2","cc2",750,0,700,500);

	responseCurve.resolution->SetMarkerStyle(20);
	responseCurve.resolution->SetLineColor(1);
	responseCurve.resolution->SetMarkerColor(1);
	responseCurve.resolution->SetFillColor(0);
	responseCurve.resolution->SetTitle("Resolution");
	responseCurve.resolution->Draw("ALP");

	responseCurve.resolution->GetXaxis()->SetTitle("Number of Photons");
	responseCurve.resolution->GetYaxis()->SetTitle("Number of Fired Pixels RMS");


	responseCurve.resolutionPE->SetMarkerStyle(20);
	responseCurve.resolutionPE->SetLineColor(4);
	responseCurve.resolutionPE->SetMarkerColor(4);
	responseCurve.resolutionPE->SetFillColor(0);
	responseCurve.resolutionPE->SetTitle("PE");
	responseCurve.resolutionPE->Draw("SAMELP");
	responseCurve.resolutionDR->SetMarkerStyle(20);
	responseCurve.resolutionDR->SetLineColor(3);
	responseCurve.resolutionDR->SetMarkerColor(3);
	responseCurve.resolutionDR->SetFillColor(0);
	responseCurve.resolutionDR->SetTitle("DR");
	responseCurve.resolutionDR->Draw("SAMELP");
	responseCurve.resolutionCT->SetMarkerStyle(20);
	responseCurve.resolutionCT->SetLineColor(5);
	responseCurve.resolutionCT->SetMarkerColor(5);
	responseCurve.resolutionCT->SetFillColor(0);
	responseCurve.resolutionCT->SetTitle("CT");
	responseCurve.resolutionCT->Draw("SAMELP");
	responseCurve.resolutionAP->SetMarkerStyle(20);
	responseCurve.resolutionAP->SetLineColor(2);
	responseCurve.resolutionAP->SetMarkerColor(2);
	responseCurve.resolutionAP->SetFillColor(0);
	responseCurve.resolutionAP->SetTitle("AP");
	responseCurve.resolutionAP->Draw("SAMELP");
	responseCurve.resolutionENF->SetMarkerStyle(20);
	responseCurve.resolutionENF->SetLineColor(6);
	responseCurve.resolutionENF->SetMarkerColor(6);
	responseCurve.resolutionENF->SetFillColor(0);
	responseCurve.resolutionENF->SetTitle("ENF");
	responseCurve.resolutionENF->Draw("SAMELP");
	responseCurve.resolutionEN->SetMarkerStyle(20);
	responseCurve.resolutionEN->SetLineColor(7);
	responseCurve.resolutionEN->SetMarkerColor(7);
	responseCurve.resolutionEN->SetFillColor(0);
	responseCurve.resolutionEN->SetTitle("EN");
	responseCurve.resolutionEN->Draw("SAMELP");


	///draw legend
	if(legend2!=0) delete legend2;
	legend2 = new TLegend(0.15,0.55,0.35,0.85);
	legend2->AddEntry(responseCurve.resolution,"Total");
	legend2->AddEntry(responseCurve.resolutionPE,"PE");
	legend2->AddEntry(responseCurve.resolutionDR,"DR");
	legend2->AddEntry(responseCurve.resolutionCT,"CT");
	legend2->AddEntry(responseCurve.resolutionAP,"AP");
	legend2->AddEntry(responseCurve.resolutionENF,"ENF");
	legend2->AddEntry(responseCurve.resolutionEN,"EN");

	legend2->Draw();

	cc2->Update();


	SetQDCChannels(nQDC_temp);

	return responseCurve;
}


// void daqMC::ScanCT(int N)
// {
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
// }

