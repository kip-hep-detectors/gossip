//TODO: move noise to Digitizers!!

#include "SiPM.h"

#include "gglobals.h"

#include <TROOT.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <TH1.h>
#include <TMath.h>
#include <stdio.h>
#include <iostream>

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


SiPM::SiPM()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::SiPM()" << endl;

	double pulse_shape_range = 1000;

	f_pulse_shape_intern = new TF1("f_pulse_shape_intern", GPulseShape, 0, pulse_shape_range, 2);

	g_spectral = NULL;

	NpixX = 10;
	NpixY = 10;
	xSipm = 1;
	ySipm = 1;
	gate = 300;
	pre_gate = 0;
	tau_recovery = 10;
	PDE = 0.1;
	Pxt = 0.1;
	Pap_s = 0.1;
	Pap_f = 0.1;
	tau_ap_s = 150;
	tau_ap_f = 40;
	tau_dr = 2000;
	gain = 20;
	ENF = 2;
	EN = 2;
	signalAmp = 20;
	noiseRMS = 2;
	jitter = 0.25;

	r.SetSeed(0);

	h_geometry = new TH2I();
	h_geometry->SetNameTitle("h_geometry", "h_geometry");

	update_pulse_shape = false;
	SetSampling(0.1);
	SetCutoff(0.001);
	SetPulseShape(1, 40);
	SetGeometry("square");
}


SiPM::~SiPM()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::~SiPM()" << endl;

	delete h_geometry;

	if(g_spectral!=NULL) delete g_spectral;
}


void SiPM::Reset()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::Reset()" << endl;

	charge.all = 0;
	charge.pe  = 0;
	charge.dr  = 0;
	charge.ap  = 0;
	charge.ct  = 0;
	charge.en  = 0;
	charge.enf = 0;

	photonList.clear();
}


int SiPM::GetParaFile( const char* filename )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::GetParaFile( const char* filename )" << endl;

	string para, value, dump;
	ifstream in_config(filename);

	///check if file exists
	if(!in_config.good())
	{
		cout << C_YELLOW << "WARNING: Parameter file '" << filename << "' does not exist!" << C_RESET << endl;
		in_config.close();
		return 1;
	}

	double tau1F = tau1;
	double tau2F = tau2;
	double gateF = gate;

	while(1)
	{
		///get rid of commented and empty lines
		while(1)
		{
			char c = in_config.peek();
			if(c=='\n') getline(in_config, para);
			else if(c=='#') getline(in_config, para);
			else break;
		}

		///stop at end of file
		if(in_config.eof()==true) break;

		///get config parameter (string before '=')
		getline(in_config, para, '=');

		///get config parameter value (string after '=')
		getline(in_config, value);

		///remove tabs and whitespaces
		para.erase(remove(para.begin(), para.end(), '\t'), para.end());
		para.erase(remove(para.begin(), para.end(), ' '), para.end());
		para.erase(remove(para.begin(), para.end(), '\n'), para.end());

		value.erase(remove(value.begin(), value.end(), '\t'), value.end());
		value.erase(remove(value.begin(), value.end(), ' '), value.end());
		value.erase(remove(value.begin(), value.end(), '\n'), value.end());

		printf(">> %s = %s\n", para.c_str(), value.c_str());

		///assign variable
		if(para == "PDE") PDE = atof(value.c_str());
		else if(para == "Gain") gain = atof(value.c_str());
		else if(para == "TauDR") tau_dr = atof(value.c_str());
		else if(para == "AP_s") Pap_s = atof(value.c_str());
		else if(para == "TauAP_s") tau_ap_s = atof(value.c_str());
		else if(para == "AP_f") Pap_f = atof(value.c_str());
		else if(para == "TauAP_f") tau_ap_f = atof(value.c_str());
		else if(para == "XT") Pxt = atof(value.c_str());
		else if(para == "ENF") ENF = atof(value.c_str());
		else if(para == "EN") EN = atof(value.c_str());
		else if(para == "TauRec") tau_recovery = atof(value.c_str());
		else if(para == "Jitter") jitter = atof(value.c_str());
		else if(para == "Npx") NpixX = atoi(value.c_str());
		else if(para == "Npy") NpixY = atoi(value.c_str());
		else if(para == "SizeX") xSipm = atof(value.c_str());
		else if(para == "SizeY") ySipm = atof(value.c_str());
		else if(para == "NoiseRMS") noiseRMS = atof(value.c_str());
		else if(para == "SignalAmp") signalAmp = atof(value.c_str());
		else if(para == "Tau1") tau1F = atof(value.c_str());
		else if(para == "Tau2") tau2F = atof(value.c_str());
		else if(para == "Gate") gateF = atof(value.c_str());
		else cout << C_YELLOW << "WARNING: Unknown parameter '" << para << "' in config file!" << C_RESET << endl;
	}

	SetGate(gateF);
	SetPulseShape(tau1F, tau2F);
	SetGeometry("square");

	in_config.close();

	return 0;
}


void SiPM::SetGate( double Gate, bool gateCut )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::SetGate( double Gate, bool gateCut )" << endl;

	gate = Gate;
	hitMatrix.SetGate(gate, gateCut);
	update_pulse_shape = true;
}


void SiPM::SetPreGate( double preGate )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::SetPreGate( double preGate )" << endl;

	pre_gate = preGate;
}


void SiPM::SetSpectralSensitivity( TGraph* sensitivity )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::SetSpectralSensitivity( TGraph* sensitivity )" << endl;

	g_spectral = (TGraph*)sensitivity->Clone();
}


void SiPM::SetSpectralSensitivity( const char* file )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::SetSpectralSensitivity( const char* file )" << endl;

	g_spectral = new TGraph(file);
}


void SiPM::SetGeometry(string Geometry)
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::SetGeometry( string Geometry )" << endl;

	h_geometry->Reset("M");

	if(Geometry=="square")
	{
		h_geometry->SetBins(NpixX, 0, NpixX, NpixY, 0, NpixY);

		for(int x=0; x<NpixX; ++x)
		{
			for(int y=0; y<NpixY; ++y)
			{
				h_geometry->Fill(x, y);
			}
		}
	}
	else cout << C_RED << "ERROR: Geometry not found" << C_RESET << endl;

	Npix = h_geometry->Integral();

	hitMatrix.SetGeometry(h_geometry);
}


void SiPM::SetGeometry( TH2I* hgeometry )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::SetGeometry( TH2I* hgeometry )" << endl;

	h_geometry->Reset("M");

	h_geometry = hgeometry;

	Npix = h_geometry->Integral();

	hitMatrix.SetGeometry(h_geometry);
}


void SiPM::SetSampling( double Sampling )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::SetSampling( double Sampling )" << endl;

	sampling = Sampling;
	update_pulse_shape = true;
}


void SiPM::SetCutoff( double Cutoff )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::SetCutoff( double Cutoff )" << endl;

	cutOff = Cutoff;
	update_pulse_shape = true;
}


void SiPM::SetPulseShape( double Tau1, double Tau2 )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::SetPulseShape( double Tau1, double Tau2 )" << endl;

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

	f_pulse_shape_intern->SetParameters(tau1, tau2);

	SetPulseShape(f_pulse_shape_intern);
}


void SiPM::SetPulseShape( TF1* f_shape )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::SetPulseShape( TF1* pulse_shape )" << endl;

	f_pulse_shape = f_shape;

	if(f_pulse_shape->GetXmin() != 0)
	{
		cout << C_YELLOW << "WARNING: TF1 used for pulse shape does not start at 0!" << C_RESET << endl;
	}

	pulse_shape.Clear();
	double psampling = sampling;
	pulse_shape.SetSampling(psampling);
	double xmin = f_pulse_shape->GetXmin();
	double xmax = f_pulse_shape->GetXmax();
	double range = xmax - xmin;

	for(int i=0; i<range/psampling; ++i)
	{
		double amp = f_pulse_shape->Eval(i*psampling);
		pulse_shape.SetSample(i, amp);
	}

	UpdatePulseShape();
}


void SiPM::UpdatePulseShape()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::UpdatePulseShape()" << endl;

	g_pulse_charge.Set(0);

	if(gPad!=0) gPad->SetLogx(false);

	///find pulse shape function amplitude and range
	pulse_shape_func_max = f_pulse_shape->GetMaximum();
	pulse_shape_func_range = f_pulse_shape->GetXmax();

	n_pulse_samples = pulse_shape_func_range/sampling+1;	//not 100% sure about the +1...

	///find pulse shape cutoff
	int i_max = f_pulse_shape->GetMaximumX()/sampling + 1;

	int i = i_max;
	if(cutOff > 0)
	{
		while(f_pulse_shape->Eval(i*sampling) > pulse_shape_func_max*cutOff)
		{
			if(i*sampling >= pulse_shape_func_range)
			{
				cout << C_YELLOW << "WARNING: No cutoff found for waveform!" << C_RESET << endl;
				break;
			}
			++i;	//not 100% sure of i++ has to come before or after the break;
		}
		n_pulse_samples = i;
	}

	///calculate pulse charge graph
	double flast_charge = 0;
	for(int i=0; i<pulse_shape_func_range/sampling; ++i)
	{
		double ftime = i*sampling;
		double fcharge = f_pulse_shape->Integral(0, ftime, 1e-3);
		g_pulse_charge.SetPoint(g_pulse_charge.GetN(), ftime, fcharge);

		if(fabs(fcharge-flast_charge)<1e-3 && i>0)
		{
			g_pulse_charge.SetPoint(g_pulse_charge.GetN(), gate+pre_gate, fcharge);
			break;
		}
		flast_charge = fcharge;
	}

	///normalize to 1
	//double *x = g_pulse_charge.GetX();
	double *y = g_pulse_charge.GetY();

	for(int i=0; i<g_pulse_charge.GetN(); ++i)
	{
		y[i] /= flast_charge;
	}

	update_pulse_shape = false;
	//   cout << "Done..." << endl;
}


void SiPM::ImportPhotons( PhotonList photons )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::ImportPhotons( PhotonList photons )" << endl;

	for(auto it : photons)
	{
		double x = it.pos_x;
		double y = it.pos_y;

		if( x>=-xSipm/2. && x<=xSipm/2. && y>=-ySipm/2. && y<=ySipm/2.)
		{
			it.pos_x = (int)(((x+0.5*xSipm))/xSipm*h_geometry->GetNbinsX());
			it.pos_y = (int)(((y+0.5*ySipm))/ySipm*h_geometry->GetNbinsY());
			photonList.push_back(it);
		}
	}
}


void SiPM::InitAvalancheList()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::InitAvalancheList()" << endl;

	hitMatrix.Init();
}


double SiPM::Generate( PhotonList photons )
{

	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::Generate()" << endl;

	Reset();
	ImportPhotons(photons);					//Translate photons from PhotonList to pixel basis
	InitAvalancheList();					//Pass parameters to AvalancheList class
	if(update_pulse_shape==true) UpdatePulseShape();	//Calculate pulse shape parameters if something has changes

	if(PDE>1 || PDE<0)
	{
		PDE=0;
		cout << C_YELLOW << "WARNING: PDE not in between 0 and 1. Setting PDE to 0!" << C_RESET << endl;
	}
	if(gain<0)
	{
		gain=0;
		cout << C_YELLOW << "WARNING: Gain smaller than zero! Setting Gain to 0!" << C_RESET << endl;
	}
	if(tau_dr<0)
	{
		tau_dr=0;
		cout << C_YELLOW << "WARNING: tau_dr smaller than zero! Setting tau_dr to 0!" << C_RESET << endl;
	}
	if(Pap_s>1 || Pap_s<0)
	{
		Pap_s=0;
		cout << C_YELLOW << "WARNING: Pap_s not in between 0 and 1. Setting Pap_s to 0!" << C_RESET << endl;
	}
	if(tau_ap_s<0)
	{
		tau_ap_s=0;
		cout << C_YELLOW << "WARNING: tau_ap_s smaller than zero! Setting tau_ap_s to 0!" << C_RESET << endl;
	}
	if(Pap_f>1 || Pap_f<0)
	{
		Pap_f=0;
		cout << C_YELLOW << "WARNING: Pap_f not in between 0 and 1. Setting Pap_f to 0!" << C_RESET << endl;
	}
	if(tau_ap_f<0)
	{
		tau_ap_f=0;
		cout << C_YELLOW << "WARNING: tau_ap_f smaller than zero! Setting tau_ap_f to 0!" << C_RESET << endl;
	}
	if(Pxt>1 || Pxt<0)
	{
		Pxt=0;
		cout << C_YELLOW << "WARNING: Pxt not in between 0 and 1. Setting Pxt to 0!" << C_RESET << endl;
	}
	if(ENF<0)
	{
		ENF=0;
		cout << C_YELLOW << "WARNING: ENF smaller than zero! Setting ENF to 0!" << C_RESET << endl;
	}
	if(EN<0)
	{
		EN=0;
		cout << C_YELLOW << "WARNING: EN smaller than zero! Setting EN to 0!" << C_RESET << endl;
	}

	///operation over-voltage:
	double Vop = 1;		///has to be 1!!! (only for custom voltage dependence of parameters important)

	///detected photons:
	for(auto it : photonList)
	{
		///apply spectral sensitivity if set
		if(g_spectral!=NULL)
		{
			double wavelength = it.wavelength;
			double spec_sens = g_spectral->Eval(wavelength);

			if(r.Rndm()>spec_sens) continue;	///photon is not detected
		}

		if(r.Rndm()<PDE)
		{
			double time = r.Gaus(it.time, jitter);		///No time offset...signal can appear before photon time stamp! =)
			int x = it.pos_x;
			int y = it.pos_y;
			hitMatrix.AddHit(x, y, time, PE);
		}
	}

	///NO JITTER FOR CT and AP implemented yet!

	///darkrate:
	if(tau_dr!=0)
	{
		double time = -pre_gate;
		int x, y;

		while(1)
		{
			time += r.Exp(tau_dr);
			while(1)
			{
				x = r.Rndm()*h_geometry->GetNbinsX();
				y = r.Rndm()*h_geometry->GetNbinsY();
				if(h_geometry->GetBinContent(x+1, y+1)==1) break;
			}
			hitMatrix.AddHit(x, y, time, DR);
			if(time >= gate) break;	//letzter hit liegt nach dem gate für time distribution
		}
	}

	/// crosstalk & afterpulses (& amplituden):
	//double ct_length = 80;
	//double pitch = (1000/Npx + 1000/Npy)*0.5;	//gemittelt über xpitch und ypitch
	double q = (1-TMath::Power(1-Pxt, 0.25));

	for(int i=0; i<hitMatrix.nHits(); ++i)
	{
		Avalanche& av = hitMatrix[i];
		int x=av.x;
		int y=av.y;
		double time=av.time;

		///pixel recovery:
		double Vover;
		double tlast=hitMatrix.GetPreviousTime(i);
		if(tlast!=-1 && tau_recovery>0) Vover = Vop*(1-TMath::Exp(-(time-tlast)/tau_recovery));
		else Vover = Vop;

		///PDE reduced due to pixel recovery:
		if(av.type==PE && r.Rndm()>=Vover/Vop)
		{
			hitMatrix.EraseHit(i);
			i--;
			continue;
		}

		///Dark-rate reduced due to pixel recovery:
		if(av.type==DR && r.Rndm()>=Vover/Vop)
		{
			hitMatrix.EraseHit(i);
			i--;
			continue;
		}

		///amplitude:
		double enfNoise = r.Gaus(0, ENF*Vover/Vop);
		double amplitude = gain + enfNoise;
		if(amplitude<0) amplitude=0;
		amplitude = amplitude*Vover/Vop;	///recovery
		av.amplitude = amplitude;

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
			if(av.type==PE) charge.pe+=gain*Vover/Vop*(1-overflow/amplitude);	///formular from amplitude = (gain + enf)*Vover/Vop - overflow
			if(av.type==CT) charge.ct+=gain*Vover/Vop*(1-overflow/amplitude);	///                        = (gain + enf)*Vover/Vop - (gain + enf)*Vover/Vop*overflow/amplitude
			if(av.type==AP) charge.ap+=gain*Vover/Vop*(1-overflow/amplitude);	///                        = gain*Vover/Vop*(1-overflow/amplitude) + enf*Vover/Vop*(1-overflow/amplitude)
			if(av.type==DR) charge.dr+=gain*Vover/Vop*(1-overflow/amplitude);
			charge.enf+=enfNoise*Vover/Vop*(1-overflow/amplitude);
		}

		///crosstalk:
		if(Pxt!=0)
		{
			//direct
			if(r.Rndm()<q*Vover/Vop) hitMatrix.AddHit(x+1, y, av.time, CT);
			if(r.Rndm()<q*Vover/Vop) hitMatrix.AddHit(x-1, y, av.time, CT);
			if(r.Rndm()<q*Vover/Vop) hitMatrix.AddHit(x, y+1, av.time, CT);
			if(r.Rndm()<q*Vover/Vop) hitMatrix.AddHit(x, y-1, av.time, CT);

			//diagonal
			// 	if(r.Rndm()<q*Vover/Vop*TMath::Exp(-sqrt(2)*pitch/ct_length)) hitMatrix.AddHit(x+1, y+1, av.time, CT);
			// 	if(r.Rndm()<q*Vover/Vop*TMath::Exp(-sqrt(2)*pitch/ct_length)) hitMatrix.AddHit(x+1, y-1, av.time, CT);
			// 	if(r.Rndm()<q*Vover/Vop*TMath::Exp(-sqrt(2)*pitch/ct_length)) hitMatrix.AddHit(x-1, y+1, av.time, CT);
			// 	if(r.Rndm()<q*Vover/Vop*TMath::Exp(-sqrt(2)*pitch/ct_length)) hitMatrix.AddHit(x-1, y-1, av.time, CT);
		}

		///afterpulses:
		//carrier stays trapped if new avalanche occurs before it's released
		if(Pap_s!=0)
		{
			int Nap=r.Poisson(Pap_s*Vover/Vop);
			for(int i_ap=0; i_ap<Nap; ++i_ap)
			{
				double time_ap = r.Exp(tau_ap_s);
				if(av.time<=gate) hitMatrix.AddHit(x, y, av.time+time_ap, AP);	//av.time<=gate: erlaube noch ein hit nach dem gate für time distribution
			}
		}
		if(Pap_f!=0)
		{
			int Nap=r.Poisson(Pap_f*Vover/Vop);
			for(int i_ap=0; i_ap<Nap; ++i_ap)
			{
				double time_ap = r.Exp(tau_ap_f);
				if(av.time<=gate) hitMatrix.AddHit(x, y, av.time+time_ap, AP);	//av.time<=gate: erlaube noch ein hit nach dem gate für time distribution
			}
		}
	}

	///Add electronic noise:
	double eNoise = r.Gaus(0, EN);
	charge.en = eNoise;
	charge.all += eNoise;

	return charge.all;
}


Waveform SiPM::GetWaveform()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "SiPM::GetWaveform()" << endl;

	int n_samples = gate/sampling+1;

	vector<double> v_amplitudes;
	v_amplitudes.resize(n_samples);

	vector<double> v_pulse = pulse_shape.GetSamples();

	int i_start;
	int i_ps_start;
	double dt;
	double time;
	double amplitude;
	double amp_norm;
	double amp;
	for(int n=0; n<hitMatrix.nHits(); ++n)
	{
		time = hitMatrix[n].time;
		amplitude = hitMatrix[n].amplitude;
		if(time >= 0)
		{
			i_start = time/sampling + 1;
			dt = i_start*sampling - time;
			i_ps_start = 0;
		}
		else
		{
			i_ps_start = -time/sampling;
			dt = -i_ps_start*sampling - time;
			i_start = 0;
		}

		//    ///risetime jitter
		//    double tau1_tmp = tau1;
		//    double risetime = r.Gaus(tau1, jitter);
		//    if(risetime<=0.2) risetime = 0.2;
		//    SetPulseShape(risetime, tau2);
		//    tau1 = tau1_tmp;

		for(int i=i_ps_start; i<n_pulse_samples; ++i)
		{
			if(i_start+i >= n_samples) break;

			amp_norm = v_pulse[i] + dt*(v_pulse[i+1]-v_pulse[i])/sampling;
			amp = signalAmp*amplitude/gain*amp_norm/pulse_shape_func_max;
			v_amplitudes[i_start+i-i_ps_start] += amp;
		}
	}

	///add random noise to waveform
	if(noiseRMS > 0)
	{
		for(int i=0; i<n_samples; ++i)
		{
			v_amplitudes[i] += r.Gaus(0, noiseRMS);
		}
	}

	///set g_waveform
	waveform.Clear();
	waveform.SetWaveform(v_amplitudes, sampling);

	return waveform;
}