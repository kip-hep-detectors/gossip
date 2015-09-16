#include "sipmMC.h"
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

Double_t GPulseShape(Double_t *x, Double_t *par)
{
	Float_t t = x[0];
	Double_t F=0;
	Double_t tau1 = par[0];
	Double_t tau2 = par[1];

	F = exp(-t/tau2) - exp(-t/tau1);

	return F;
}


sipmMC::sipmMC()
{
	pulse_shape_func_range = 1000;

	f_pulse_shape_intern = new TF1("f_pulse_shape_intern",GPulseShape,0,pulse_shape_func_range,2);

	g_spectral = NULL;

	NpixX	= 10;
	NpixY	= 10;
	xSipm	= 1;
	ySipm	= 1;
	gate	= 300;
	pre_gate	= 0;
	tau_recovery= 10;
	PDE		= 0.1;
	Pxt		= 0.1;
	Pap_s	= 0.1;
	Pap_f	= 0.1;
	tau_ap_s	= 150;
	tau_ap_f	= 40;
	tau_dr	= 2000;
	gain	= 20;
	ENF		= 2;
	EN		= 2;
	signalAmp	= 20;
	noiseRMS	= 2;
	jitter	= 0.25;

	timeval time;
	gettimeofday(&time,NULL);
	r.SetSeed(time.tv_usec);

	hitMatrix = new HitMatrix();

	h_geometry = new TH2I();
	h_geometry->SetNameTitle("h_geometry","h_geometry");

	g_waveform = new TGraph();
	g_waveform->SetMarkerStyle(1);
	g_waveform->SetNameTitle("Waveform","Waveform");
	g_waveform->GetXaxis()->SetTitle("Time [ns]");
	g_waveform->GetYaxis()->SetTitle("Amplitude [mV]");

	update_pulse_shape = false;
	SetSampling(0.1);
	SetCutoff(0.001);
	SetPulseShape(1,40);
	SetGeometry("square");
}


sipmMC::~sipmMC()
{
	delete hitMatrix;
	delete h_geometry;
	delete g_waveform;

	if(g_spectral!=NULL) delete g_spectral;
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
		else if(para == "Jitter") in >> jitter;
		else if(para == "Npx") in >> NpixX;
		else if(para == "Npy") in >> NpixY;
		else if(para == "SizeX") in >> xSipm;
		else if(para == "SizeY") in >> ySipm;
		else if(para == "NoiseRMS") in >> noiseRMS;
		else if(para == "SignalAmp") in >> signalAmp;
		else if(para == "Tau1") in >> tau1F;
		else if(para == "Tau2") in >> tau2F;
		else getline(in, dump);

		SetPulseShape(tau1F, tau2F);
		SetGeometry("square");

		if(!in.good()) break;
	}

	cout << "PDE = " << PDE << "\n"
		<< "Gain = " << gain << "\n"
		<< "TauDR = " << tau_dr << "\n"
		<< "AP_s = " << Pap_s << "\n"
		<< "TauAP_s = " << tau_ap_s << "\n"
		<< "AP_f = " << Pap_f << "\n"
		<< "TauAP_f = " << tau_ap_f << "\n"
		<< "XT = " << Pxt << "\n"
		<< "ENF = " << ENF << "\n"
		<< "EN = " << EN << "\n"
		<< "TauRec = " << tau_recovery << "\n"
		<< "Jitter = " << jitter << "\n"
		<< "Npx = " << NpixX << "\n"
		<< "Npy = " << NpixY << "\n"
		<< "SizeX = " << xSipm << "\n"
		<< "SizeY = " << ySipm << "\n"
		<< "NoiseRMS = " << noiseRMS << "\n"
		<< "SignalAmp = " << signalAmp << "\n"
		<< "Tau1 = " << tau1F << "\n"
		<< "Tau2 = " << tau2F << "\n"
		<< endl;

	in.close();

}


void sipmMC::SetGate( double Gate, bool gateCut )
{
	gate = Gate;
	hitMatrix->SetGate(gate, gateCut);
	update_pulse_shape = true;
}


void sipmMC::SetPreGate( double preGate )
{
	pre_gate = preGate;
}


void sipmMC::SetSpectralSensitivity( TGraph* sensitivity )
{
	g_spectral = (TGraph*)sensitivity->Clone();
}


void sipmMC::SetSpectralSensitivity( const char* file )
{
	g_spectral = new TGraph("file");
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


void sipmMC::SetSampling( double Sampling )
{

	sampling = Sampling;
	update_pulse_shape = true;

}


void sipmMC::SetCutoff( double Cutoff )
{

	cutOff = Cutoff;
	update_pulse_shape = true;

}


void sipmMC::SetPulseShape( double Tau1, double Tau2 )
{
	if(Tau1<Tau2)
	{
		tau1 = Tau1;
		tau2 = Tau2;
	}
	else
	{
		tau1 = Tau2;
		tau2 = Tau1;
	}

	f_pulse_shape_intern->SetRange(0,pulse_shape_func_range);
	f_pulse_shape_intern->SetParameters(tau1,tau2);

	SetPulseShape(f_pulse_shape_intern);
}


void sipmMC::SetPulseShape( TF1* pulse_shape )
{
	f_pulse_shape = pulse_shape;

	update_pulse_shape = true;
}


void sipmMC::UpdatePulseShape()
{
	//   cout << "Calculating pulse charge..." << endl;

	g_pulse_charge.Set(0);

	f_pulse_shape->SetRange(0,pulse_shape_func_range);

	if(gPad!=0) gPad->SetLogx(false);

	///find pulse shape function amplitude
	pulse_shape_func_max = f_pulse_shape->GetMaximum();

	///find pulse shape cutoff
	int i_max = f_pulse_shape->GetMaximumX()/sampling + 1;

	int i = i_max;
	while(f_pulse_shape->Eval(i*sampling) > pulse_shape_func_max*cutOff)
	{
		i++;
	}
	n_pulse_samples = i;

	///calculate pulse charge graph
	double flast_charge = 0;
	for(int i=0;i<pulse_shape_func_range/sampling;i++)
	{
		double ftime = i*sampling;
		double fcharge = f_pulse_shape->Integral(0,ftime,(const double*)0,1e-3);
		g_pulse_charge.SetPoint(g_pulse_charge.GetN(),ftime,fcharge);

		if(fabs(fcharge-flast_charge)<1e-3 && i>0)
		{
			g_pulse_charge.SetPoint(g_pulse_charge.GetN(),gate+pre_gate,fcharge);
			break;
		}
		flast_charge = fcharge;
	}


	///normalize to 1
	double *x = g_pulse_charge.GetX();
	double *y = g_pulse_charge.GetY();

	for(int i=0;i<g_pulse_charge.GetN();i++)
	{
		y[i] /= flast_charge;
	}

	update_pulse_shape = false;
	//   cout << "Done..." << endl;
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


double sipmMC::Generate( PhotonList photons )
{
	Reset();
	ImportPhotons(photons);				//Übersetzte Potonen aus PhotonList auf Pixelbasis
	InitHitMatrix();					//Geben Parameter an HitMatrix weiter
	if(update_pulse_shape==true) UpdatePulseShape();	//Calculate pulse shape parameters if something has changes


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
		///apply spectral sensitivity if set
		if(g_spectral!=NULL)
		{
			double wavelength = photonList[i][PL_WL];
			double spec_sens = g_spectral->Eval(wavelength);

			if(r.Rndm()>spec_sens) continue;	///photon is not detected
		}

		if(r.Rndm()<PDE)
		{
			double time = r.Gaus(photonList[i][PL_TIME],jitter);		///No time offset...signal can appear before photon time stamp! =)
			hitMatrix->AddHit(photonList[i][PL_X],photonList[i][PL_Y],time,PE);
		}
	}

	///NO JITTER FOR CT and AP implemented yet!

	///darkrate:
	if(tau_dr!=0)
	{
		double time = -pre_gate;
		int x,y;

		while(1)
		{
			time += r.Exp(tau_dr);
			while(1)
			{
				x = r.Rndm()*h_geometry->GetNbinsX();
				y = r.Rndm()*h_geometry->GetNbinsY();
				if(h_geometry->GetBinContent(x+1,y+1)==1) break;
			}
			hitMatrix->AddHit(x,y,time,DR);
			if(time >= gate) break;	//letzter hit liegt nach dem gate für time distribution
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
		if(time>=0)
		{
			overflow = amplitude*(1-g_pulse_charge.Eval(gate-time));		///Not effected by jitter!
		}
		else ///events in pre_gate
		{
			overflow = amplitude*g_pulse_charge.Eval(-time);			///Not effected by jitter!
		}
		charge.all += amplitude - overflow;

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

	return charge.all;
}


TGraph* sipmMC::GetWaveform()
{
	//reset g_waveform
	g_waveform->Set(0);

	for(int i=0;i<gate/sampling+1;i++)
	{
		g_waveform->SetPoint(i,i*sampling,r.Gaus(0,noiseRMS));
	}

	double *wf_x = g_waveform->GetX();
	double *wf_y = g_waveform->GetY();

	for(int n=0;n<hitMatrix->nHits();n++)
	{
		double time = hitMatrix->GetHit(n)[TIME];
		double amplitude = hitMatrix->GetHit(n)[AMPLITUDE];

		int i_start;
		double tstart;
		if(time>=0)
		{
			i_start= time/sampling + 1;
			tstart = wf_x[i_start];
		}
		else
		{
			i_start = 0;
			tstart = 0;
		}

		//    ///risetime jitter
		//    double tau1_tmp = tau1;
		//    double risetime = r.Gaus(tau1,jitter);
		//    if(risetime<=0.2) risetime = 0.2;
		//    SetPulseShape(risetime,tau2);
		//    tau1 = tau1_tmp;

		for(int i=0;i<n_pulse_samples;i++)
		{
			if(i_start+i > g_waveform->GetN()) break;

			double t = i*sampling + (tstart - time);
			double amp = signalAmp*amplitude/gain*f_pulse_shape->Eval(t)/pulse_shape_func_max;
			wf_y[i_start+i] += amp;
		}
	}

	return g_waveform;
}
