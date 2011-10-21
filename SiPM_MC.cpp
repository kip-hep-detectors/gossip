/********** Version 1.2 **********/

#include "SiPM_MC.h"
#include "drTDCspec.h"
#include <TRandom1.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <TH2.h>
#include <TMath.h>

using namespace std;


SiPM_MC::SiPM_MC(){
	Npx=10;
	Npy=10;
	Npix=Npx*Npy;
	tscale=1;
	Ngamma=10;
	Ngamma_max=3000;
	LED_width=4;
	gate=300*tscale;
	recovery=10*tscale;
	tau1=1*tscale;
	tau2=40*tscale;
	PDE=0.05717;
	Px=0.04;
	Pas=0.045;
	Paf=0.047;
	tau_aps=159*tscale;
	tau_apf=46.1*tscale;
	tau_dr=5529*tscale;
	G=35;
	ENF=4;
	EN=3.6;
	pedestal=20;
	d_spot=0;
	SeedHit=false;
	plast=0;
	cancel=false;
	NgammaFluct==true;
	matrix = new HitMatrix();
	ctapFlag = new HitMatrix();

	h_QDC = new TH1D("Charge Spectrum","Charge Spectrum",1024,0,1024);
	h_QDC->GetXaxis()->SetTitle("QDC Channels");
	h_QDC->GetYaxis()->SetTitle("# Entries");
	h_TDC = new TH1D();
	h_TDC->SetName("Time Spectrum");
	h_TDC->SetTitle("Time Spectrum");
	h_TDC->GetXaxis()->SetTitle("Time [ns]");
	h_TDC->GetYaxis()->SetTitle("# Entries");
	h_Histo = new TH1D("","",100,0,100);
	h_Histo->GetXaxis()->SetTitle("Number of Events");
	h_Histo->GetYaxis()->SetTitle("# Entries");
}


SiPM_MC::~SiPM_MC(){
	delete matrix, ctapFlag, h_QDC, h_TDC, h_Histo, g_Thresh;
}


//Generate Photon Hits
void SiPM_MC::GeneratePhotons(string opt, bool NgammaFluct, int xHit, int yHit){

	int Ngamma1;
  
	PhotonCoord.clear();
	if(opt=="rndm"){
		if(NgammaFluct==true) Ngamma1=r.Poisson(Ngamma);
		else Ngamma1=Ngamma;
    
		double Xcoord, Ycoord;
		double XCenter=500-1000/(2.0*Npx);
		double YCenter=500-1000/(2.0*Npy);
		int x,y;
    
		for(int i=0;i<Ngamma1;i++){
			x=r.Rndm()*Npx;
			y=r.Rndm()*Npy;
			Xcoord=x*1000/Npx;
			Ycoord=y*1000/Npy;
			if(pow(Xcoord-XCenter,2)+pow(Ycoord-YCenter,2)<=pow(d_spot/2,2) || d_spot==0){
				vector<int> Coord;
				Coord.push_back(r.Rndm()*Npx);
				Coord.push_back(r.Rndm()*Npy);
				PhotonCoord.push_back(Coord);
			}
		else i--;
		}
	}
  
	if(opt=="seed"){
		vector<int> Coord;
		Coord.push_back(r.Rndm()*Npx);
		Coord.push_back(r.Rndm()*Npy);
		PhotonCoord.push_back(Coord);
		}
		
	if(opt=="non"){
	}
  
	if(opt=="scan"){
		if(xHit<Npx && yHit<Npy){
			if(NgammaFluct==true) Ngamma1=r.Poisson(Ngamma);
			else Ngamma1=Ngamma;
			for(int i=0;i<Ngamma1;i++){
				vector<int> Coord;
				Coord.push_back(xHit);
				Coord.push_back(yHit);
				PhotonCoord.push_back(Coord);
			}
		}
		else cout << "Photon position not on SiPM" << endl;
	}
  
}


//-----------Calculate Number of Firing Pixels:-----------
int SiPM_MC::Generate(bool gcut){
	
	Reset(gcut);
	
//detected photons:
	
	int x,y;
	int iPhoton=0;
	double time;
	for(unsigned int i=0;i<PhotonCoord.size();i++){
		if(r.Rndm()<PDE){
			x=PhotonCoord[i][0];
			y=PhotonCoord[i][1];
			time=r.Rndm()*LED_width;
// 			time=r.Exp(LED_width);
			matrix->AddHit(x,y,0,time);
		}
	}
	PhotonCoord.clear();
	matrix->TSort(0);

//darkrate:

	if(tau_dr!=0){
		time=0;
		while(1){
			time += r.Exp(tau_dr);
			x=r.Rndm()*Npx;
			y=r.Rndm()*Npy;
			matrix->AddHit(x,y,1,time);
			if(time<=LED_width) matrix->TSortLastEvent(0);
			if(time>=matrix->gate) break; //letztes event liegt nach dem gate für time distribution
		}
	}

// crosstalk & afterpulses (& amplituden):


	//double ct_length = 80;
	//double pitch = (1000/Npx + 1000/Npy)*0.5;	//gemittelt über xpitch und ypitch
	double q = (1-TMath::Power(1-Px,0.25));
	double tap, tlast=-1, A=0;
	time=0;
	
	for(unsigned int i=0;i<matrix->nHits(-1);i++){
		
		if(Pas==0 && Paf==1 && Px==0) break;
		
		event.clear();
		event=matrix->GetEvent(i);
		x=event[0];
		y=event[1];
		time=event[3];
		
		//amplitude:
		A=r.Gaus(G,ENF);

		tlast=matrix->GetPreviousTime(event);
		if(tlast!=-1) A=A*(1-TMath::Exp(-(time-tlast)/recovery));
		matrix->matrix[i][4]=A;

		if(ctapFlag->FindEvent(event)==-1){
			//crosstalk;
			if(Px!=0){
				//direct
				if(r.Rndm()<q*A/G){
					if(matrix->AddHit(event[0]+1,event[1],2,event[3])==0) matrix->TSortLastEvent(i);
				}
				if(r.Rndm()<q*A/G){
					if(matrix->AddHit(event[0]-1,event[1],2,event[3])==0) matrix->TSortLastEvent(i);
				}
				if(r.Rndm()<q*A/G){
					if(matrix->AddHit(event[0],event[1]+1,2,event[3])==0) matrix->TSortLastEvent(i);
				}
				if(r.Rndm()<q*A/G){
					if(matrix->AddHit(event[0],event[1]-1,2,event[3])==0) matrix->TSortLastEvent(i);
				}
				//diagonal
				/*if(r.Rndm()<q*A/G*TMath::Exp(-sqrt(2)*pitch/ct_length)){
					matrix->AddHit(event[0]+1,event[1]+1,2,event[3]);
					matrix->TSortLastEvent(i);
				}
				if(r.Rndm()<q*A/G*TMath::Exp(-sqrt(2)*pitch/ct_length)){
					matrix->AddHit(event[0]+1,event[1]-1,2,event[3]);
					matrix->TSortLastEvent(i);
				}
				if(r.Rndm()<q*A/G*TMath::Exp(-sqrt(2)*pitch/ct_length)){
					matrix->AddHit(event[0]-1,event[1]+1,2,event[3]);
					matrix->TSortLastEvent(i);
				}
				if(r.Rndm()<q*A/G*TMath::Exp(-sqrt(2)*pitch/ct_length)){
					matrix->AddHit(event[0]-1,event[1]-1,2,event[3]);
					matrix->TSortLastEvent(i);
				}*/
			}

			//afterpulses
			//carrier stays trapped if new avalanche occurs before it's released
			if(Pas!=0){
				int Nap=r.Poisson(Pas*A/G);
				for(int i_ap=0;i_ap<Nap;i_ap++){
					tap = r.Exp(tau_aps);
					if(event[3]<=matrix->gate){ //event[3]<=gate: erlaube noch ein event nach dem gate für time distribution
						matrix->AddHit(event[0],event[1],3,event[3]+tap);
						matrix->TSortLastEvent(i);
					}
				}
			}
			if(Paf!=0){
				int Nap=r.Poisson(Paf*A/G);
				for(int i_ap=0;i_ap<Nap;i_ap++){
					tap = r.Exp(tau_apf);
					if(event[3]<=matrix->gate){ //event[3]<=gate: erlaube noch ein event nach dem gate für time distribution
						matrix->AddHit(event[0],event[1],3,event[3]+tap);
						matrix->TSortLastEvent(i);
					}
				}
			}
			
			ctapFlag->AddHit(event[0],event[1],event[2],event[3]);
		}
	}
	return matrix->nHits(-1);
}


void SiPM_MC::Progress(int p){
	if(p!=plast) Emit("Progress(int)",p+1);	// p = Progress in %
	plast=p;
}


//Reset HitMatrix:
void SiPM_MC::Reset(bool gcut){
	
	matrix->Init(-1);
	ctapFlag->Init(-1);
	event.clear();
	
	matrix->gcut=gcut;
	ctapFlag->gcut=gcut;
	matrix->gate=gate;
	matrix->Nx=Npx;
	matrix->Ny=Npy;
	ctapFlag->gate=gate;
	ctapFlag->Nx=Npx;
	ctapFlag->Ny=Npy;
}


//Show of Hits:
void SiPM_MC::NumberOfHits(){
	
	cout << "Detected: " << matrix->nHits(0) << endl;
	cout << "Darkrate: " << matrix->nHits(1) << endl;
	cout << "Crosstalk: " << matrix->nHits(2) << endl;
	cout << "Afterpulses: " << matrix->nHits(3) << endl;
	cout << "Total: " << matrix->nHits(-1) << endl;
	
}


//Calculate Pulseshape
double SiPM_MC::BuildPulses(double x){
	
	double t = x;
	double F=0;
	double A=0;
	double t0=0;
	
	for(int i=0;i<matrix->nHits(-1);i++){
		event.clear();
		event=matrix->GetEvent(i);
		t0=event[3];
		A=event[4];
// 		if(t-t0>=0) F+=(-A/(tau1-tau2)*(TMath::Exp(-(t-t0)/tau1) - TMath::Exp(-(t-t0)/tau2)));
		if(t-t0>=0) F+=(-A/G/(pow(tau1/tau2,-tau2/(tau1-tau2)) - pow(tau1/tau2,-tau1/(tau1-tau2)))*(TMath::Exp(-(t-t0)/tau1) - TMath::Exp(-(t-t0)/tau2)));

	
	}
	F+=r.Gaus(0,EN/sqrt(matrix->gate));
	
	return F;
}


//Scope
TGraph* SiPM_MC::Scope(double res){
	
	int xmax=matrix->gate/res;
	double Signal[xmax],time[xmax];
	
	for(int x=0;x<xmax;x++){
		Signal[x]=BuildPulses(x*res);
		time[x]=x*res;
	}

	TGraph *scope = new TGraph(xmax,time,Signal);

	return scope;
}

TGraph* SiPM_MC::ScopeSection(double res, double tstart, double thresh){
  
	unsigned int xmin=tstart/res;
	vector<double> VSignal;
	vector<double> Vtime;
	unsigned int x=xmin;
	bool above=false;
	unsigned int i=0;

	while(1){
		VSignal.push_back(BuildPulses(x*res));
		Vtime.push_back(x*res);
		if(VSignal[i]<=thresh) above=true;
		if(VSignal[i]>thresh && above==true) break;
		if(x>=matrix->gate/res && matrix->gcut==true) break;
		x++;
		i++;
	}

	const int n = VSignal.size();
	double Signal[n];
	double time[n];

	for(int i=0;i<n;i++){
		Signal[i]=VSignal[i];
		time[i]=Vtime[i];
	}

	TGraph *scope = new TGraph(x-xmin,time,Signal);
	
	return scope;
}


//Integrate Charge in Gate
double SiPM_MC::Charge(){
	
	double C=0, A=0;
	double t0=0;
	
	for(int i=0;i<matrix->nHits(-1);i++){
		event.clear();
		event=matrix->GetEvent(i);
		t0=event[3];
		A=event[4];
		C+=A/(tau1-tau2)*(tau1-tau2-tau1*TMath::Exp(-(matrix->gate-t0)/tau1)+tau2*TMath::Exp(-(matrix->gate-t0)/tau2));
	}
	C+=r.Gaus(pedestal,EN);
	
	return C;
}



//--------------------------------------------Measurements:--------------------------------------------


//Build Histograms:
TH1D* SiPM_MC::Histo(int N, string type ){

	h_Histo->Reset("M");
	//h_Histo->SetBins(Npix,-0.5,Npix-0.5);
	
	cout << "Simulating Events..." << endl;
	
	for(int i=0;i<N;i++){
		if(type=="PDE"){
			h_Histo->SetName("Detecte Photons");
			h_Histo->SetTitle("Detecte photons");
			GeneratePhotons("rndm");
			Generate(1);
			h_Histo->Fill(matrix->nHits(0));
		}
		if(type=="DR"){
			h_Histo->SetName("Dark-rate Events");
			h_Histo->SetTitle("Dark-rate Events");
			PDE=0;
			Generate(1);
			h_Histo->Fill(matrix->nHits(1));
		}
		if(type=="CT"){
			h_Histo->SetName("Cross-talk Events");
			h_Histo->SetTitle("Cross-talk Events");
			PDE=100;
			tau_dr=0;
			Pas=0;
			Paf=0;
			GeneratePhotons("seed");
			Generate(1);
			h_Histo->Fill(matrix->nHits(2));
		}
		if(type=="AP"){
			h_Histo->SetName("After-pulse Events");
			h_Histo->SetTitle("After-pulse Events");
			PDE=100;
			tau_dr=0;
			Px=0;
			GeneratePhotons("seed");
			Generate(1);
			h_Histo->Fill(matrix->nHits(3));
		}
		if(type=="CTAP"){
			h_Histo->SetName("Cross-talk & After-pulse Events");
			h_Histo->SetTitle("Cross-talk & After-pulse Events");
			PDE=100;
			tau_dr=0;
			GeneratePhotons("seed");
			Generate(1);
			h_Histo->Fill(matrix->nHits(2)+matrix->nHits(3));
		}
	}
	
	cout << "Done..." << endl;
	
	return h_Histo;
}


//Calculate QDC Spectrum
TH1D* SiPM_MC::QDCSpectrum(int N){

	h_QDC->Reset("M");
	
	cout << "Simulating Events..." << endl;

	for(int i=0;i<N;i++){
		if(cancel==true) break;
		Progress(100*i/N);
		GeneratePhotons("rndm");
		Generate(1);
		h_QDC->Fill(Charge());
	}
	
	cout << "Done..." << endl;
	
	return h_QDC;
}


//Time Distribution:
void SiPM_MC::TimeSpectrum(int N){

//Simple: Use timestaps

	PDE=0;
	Ngamma=0;
	gate=65000;
	bool fill=1;

	h_TDC->Reset("M");
	h_TDC->SetBins(gate,0,gate);

	if(tau_dr==0) GeneratePhotons("non");

	cout << "Simulating Events..." << endl;

	while(fill){
		if(cancel==true) break;
		double tlast=0;
		Generate(0);
		for(int j=0;j<matrix->nHits(-1);j++){
			event.clear();
			event=matrix->GetEvent(j);
			if(event[2]==2 || event[2]==0 || event[3]-tlast<=0) continue; //0ns dead-time
// 			h_TDC->Fill(event[3]-tlast);
			h_TDC->SetBinContent(event[3]-tlast+1,h_TDC->GetBinContent(event[3]-tlast+1)+event[4]);
			tlast=event[3];
			Progress(h_TDC->GetEntries()*100/N);
			if(h_TDC->GetEntries()>=N) { fill=0; break; }
		}
		
	}
	
//Advanced: Use Scope	
/*
	PDE=0;
	Ngamma=0;
	gate=65000;
	
	bool above = false;
	bool fill=1;
	double thresh=-1;
	double res=1;
	h_TDC->Reset("M");
	h_TDC->SetBins(gate,0,gate);

	if(tau_dr==0) GeneratePhotons("seed");
	
	Double_t *Signal = new Double_t();
	Double_t *time = new Double_t();
	TGraph *scope = new TGraph();
	Int_t Npoints;
	double tlast;
	double thit;
	

	while(fill){
		tlast=0;
		thit=0;
		Generate(0);
		
		for(int j=0; j<matrix->nHits(-1);j++){

			if(cancel==true) break;
			event.clear();
			event=matrix->GetEvent(j);
			
			if(event[3]<tlast) continue;
			
			scope = ScopeSection(res,event[3],-0.5);
			Signal=scope->GetY();
			time=scope->GetX();
			Npoints=scope->GetN();
			tlast=time[0]+Npoints*res;
			
			for(int i=0;i<Npoints;i++){
				if(Signal[i]<=thresh && above==false){ h_TDC->Fill(time[i]-thit); above=true; thit=time[i]; }
				if(Signal[i]>thresh && above==true && time[i]-thit>5) above=false;
				Progress(h_TDC->GetEntries()*100/N);
				if(h_TDC->GetEntries()>=N) { fill=0; break; }
			}
		}
	}
	
	delete Signal, time, scope;
	
	cout << "Process: " << 100 << "%" << endl;
	cout << "Fitting..." << endl;
*/
//Fit
	cout << "Fitting..." << endl;
	
	h_TDC->SetTitle("Time Distribution");
	h_TDC->Draw();
	TF1 *fit = new TF1("fit",drTDCspec,0,matrix->gate,6);
	fit->SetParNames("N","Pas","Paf","tau_dr","tau_aps","tau_apf","Px");
	fit->SetParameters(N,Pas,Paf,tau_dr,tau_aps,tau_apf,Px);
	fit->SetParLimits(0,0,1e8);
	fit->SetParLimits(1,0,1);
	fit->SetParLimits(2,0,1);
	fit->SetParLimits(3,0,1e5);
	fit->SetParLimits(3,0,1e4);
	fit->SetParLimits(3,0,1e4);
	
	//fit->FixParameter(0,N);
	//fit->FixParameter(1,Pas);
	//fit->FixParameter(2,0);
	//fit->FixParameter(3,tau_dr);
	//fit->FixParameter(4,tau_aps);
	//fit->FixParameter(5,tau_apf);
// 	fit->FixParameter(6,Px);

	fit->SetLineColor(4);
	h_TDC->Fit("fit","M","",2*recovery ,tau_dr*4);
	cout << "\n RedChi2: " << fit->GetChisquare()/fit->GetNDF() << endl;
	cout << "Done..." << endl;
}


//Threshold Scan
TGraph* SiPM_MC::ThreshScan(int N){

	PDE=0;
	Ngamma=0;	

	bool above = false;
	double thresh;
	double V[1000]={0}, count[1000]={0};
	double thit, tlast=0;
	int n=0;
	double res=1;
	Double_t *Signal, *time;
	TGraph *scope = new TGraph();
	Int_t Npoints;
	
	gate=N;
	cout << "Simulate Event..." << endl;
	GeneratePhotons("non");
	Generate(1);
	
	cout << "Threshold Scan..." << endl;
	
	for(int j=0; j<matrix->nHits(-1);j++){
		if(cancel==true) break;
		Progress(100*j/matrix->nHits(-1));
		event.clear();
		event=matrix->GetEvent(j);

		if(event[3]<tlast) continue;
		
		scope = ScopeSection(res,event[3],-0.5);
		Signal=scope->GetY();
		time=scope->GetX();
		Npoints=scope->GetN();
		tlast=time[0]+Npoints*res;
		
		n=0;
		for(thresh=-1;thresh>=-10;thresh-=0.05){
			thit=time[0];
			V[n]=-thresh;
			for(int i=0;i<Npoints;i++){
				if(Signal[i]<=thresh && above==false){ count[n]++; above=true; thit=time[i]; }
				if(Signal[i]>thresh && above==true && time[i]-thit>10) above=false;		//10ns dead-time
			}
			n++;
		}
	}
	
	delete Signal, time, scope;
	
	g_Thresh = new TGraph(n,V,count);
	
	cout << "Done..." << endl;
//Alt: full scope - noch nicht getestet!!!
/*
	PDE=0;
	Ngamma=0;

	bool above = false;
	double thresh;
	double V[1000]={0}, count[1000]={0};
	double thit;
	Double_t *Signal, *time;
	Int_t Npoints;

	Generate(0);
	TGraph *scope = Scope(1);
	Signal=scope->GetY();
	time=scope->GetX();
	Npoints=scope->GetN();

	int n=0;
	for(thresh=-0.5;thresh>-5;thresh-=0.1){
		thit=time[0];
		V[n]=-thresh;
		for(int i=0;i<Npoints;i++){
			if(Signal[i]<=thresh && above==false){ count[n]++; above=true; thit=time[i]; }
			if(Signal[i]>thresh && above==true && time[i]-thit>10) above=false;
		}
		n++;
	}

	g_Thresh = new TGraph(n-1,V,count);
	g_Thresh->Draw("ALP");
*/

	return g_Thresh;
}


TGraph* SiPM_MC::Res_vs_Ngamma(int N){
  
	h_QDC->Reset("M");
	h_QDC->SetBins(100000,0,100000);
	
	int Npoints=11;
	
	double Ndone=0;
	double Ntot=0;
	
	for(int jj=0;jj<Npoints;jj++)
	{
	  Ntot+=Ngamma_max/Npoints*jj;
	}
	
	double Ng[10000]={0}, Mean[10000]={0}, Mean_err[10000], RMS[10000]={0}, RMS_err[10000]={0}, Sigma[10000]={0};
	
	cout << "Simulating Events..." << endl;
	
	for(int j=0; j<Npoints; j++){
		Ng[j]=Ngamma_max/(Npoints-1)*j;
		Ngamma=Ng[j];
		h_QDC->Reset("M");
		for(int i=0;i<N;i++){
			if(cancel==true) break;
			GeneratePhotons("rndm");
			Generate(1);
			h_QDC->Fill(Charge());
		}
		Mean[j]=(h_QDC->GetMean()-pedestal)/G;
		RMS[j]=h_QDC->GetRMS()/G;
		Mean_err[j]=RMS[j]/sqrt(N)/G;
		RMS_err[j]=RMS[j]/sqrt(2*(N-1))/G;
		
		Ndone+=Ng[j];
		Progress(100*Ndone/Ntot);
	}

	h_QDC->SetBins(1024,0,1024);
	
	g_Response = new TGraphErrors(Npoints,Ng,Mean,0,Mean_err);
	g_Response->SetMarkerStyle(20);
	TF1 *fit = new TF1("fit","[0]*(1-TMath::Exp(-[1]*x/[0]))",0,Ngamma_max);
	fit->SetParameters(Npix,PDE);
	g_Response->Fit("fit","M","",0,Ngamma_max);
	
	TCanvas *cc = new TCanvas("cc","cc",0,0,700,500);
	g_Response->Draw("ALP");
	
	///Resolution

	for(int j=0; j<Npoints; j++){
		Sigma[j]=RMS[j]*fit->GetParameter(0)/fit->GetParameter(1)/(fit->GetParameter(0)-Mean[j])/Ng[j];
	}
	
	//g_ResNgamma = new TGraph(Npoints,Ng,Sigma);
	g_ResNgamma = new TGraphErrors(Npoints,Ng,RMS,0,RMS_err);
	g_ResNgamma->SetMarkerStyle(20);
	
	delete fit;
	
	TCanvas *cc2 = new TCanvas("cc2","cc2",750,0,700,500);
	g_ResNgamma->Draw("ALP");
	
	cout << "Done..." << endl;
	return g_ResNgamma;
}


TGraph* SiPM_MC::Response(int N){
  
	h_QDC->Reset("M");
	h_QDC->SetBins(100000,0,100000);
	
	int Npoints=15;
	
	double Ng[10000]={0}, Res[10000]={0}, Res_err[10000]={0};
	
	cout << "Simulating Events..." << endl;
	
	for(int j=1; j<=Npoints; j=j++){
		Ng[j-1]=Ngamma_max/Npoints*j;
		Ngamma=Ng[j-1];
		h_QDC->Reset("M");
		for(int i=0;i<N;i++){
			if(cancel==true) break;
			GeneratePhotons("rndm");
			Generate(1);
			h_QDC->Fill(Charge());
		}
		Res[j-1]=(h_QDC->GetMean()-pedestal);
		Res_err[j-1]=h_QDC->GetRMS()/sqrt(N);
		Progress(100*j/Npoints);
	}
	
	h_QDC->SetBins(1024,0,1024);
	
	g_Response = new TGraphErrors(Npoints,Ng,Res,0,Res_err);
	g_Response->SetMarkerStyle(21);
	
	TF1 *fit = new TF1("fit","[0]*(1-TMath::Exp(-[1]*x/[0]))",0,Ngamma_max);
	fit->SetParameters(Npix,PDE);
	fit->SetLineColor(3);
	g_Response->Fit("fit","M","",0,Ngamma_max);
	
	cout << "Done..." << endl;
		
	return g_Response;
}


void SiPM_MC::ScanCT(int N){
  
    TH2D *h_scan = new TH2D("h_scan","h_scan",Npx,0,Npx,Npy,0,Npy);
    for(int x=0;x<Npx;x++){
      for(int y=0;y<Npy;y++){
	Progress((Npy*x+y+1)*100/(Npx*Npy));
	int CTevents=0;
	for(int i=0; i<N; i++){
	  GeneratePhotons("scan",false,x,y);
	  Generate(1);
	  CTevents+=matrix->nHits(2);
	}
	h_scan->SetBinContent(x+1,y+1,CTevents);
      }
    }
    cout << "Done..." << endl;
    TCanvas *c_scan = new TCanvas("c_scan","c_scan",0,600,400,400);
    h_scan->Draw("colz");
}
