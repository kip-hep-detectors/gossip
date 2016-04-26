#ifndef sipmMC_h
#define sipmMC_h

#include "HitMatrix.h"
#include "PhotonList.h"

#include <TROOT.h>
#include <TRandom.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <stdlib.h>
#include <vector>
#include <string>

using std::vector;
using std::cout;
using std::endl;
using std::string;

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

		int		NpixX;						/**<Number of pixels in x. Disregarded when SetGeometry( TH2I* hgeometry ) is used.*/
		int		NpixY;						/**<Number of pixels in y. Disregarded when SetGeometry( TH2I* hgeometry ) is used.*/
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
		int		GetParaFile( const char* filename );		/**<Sets SiPM parameters from config file. Returns 0 if successfull, returns 1 if file not found.*/
		void		SetGeometry( string Geometry );			/**<Sets pixel arrangement. Only "square" implemented at the moment*/
		void		SetGeometry( TH2I* hgeometry );			/**<Sets custom pixel arrangement from TH2I*/
		void		SetPulseShape( double Tau1, double Tau2 );	/**<Sets double exponential single pixel waveform with time constants "Tau1", "Tau2". "Sampling" is the sampling time of the simualtion in ns. Waveform is cut off at a fraction "cutOff" of the amplitude*/
		void		SetPulseShape( TF1* pulse_shape );		/**<Sets single pixel waveform from TF1*/
		void		UpdatePulseShape();				/**<Builds g_pulse_charge*/
		void		SetSampling( double Sampling );			/**<Sets waveform sampling*/
		void		SetCutoff( double Cutoff );			/**<Sets pixel waveform cutoff*/
		void		SetGate( double Gate, bool gateCut=true );	/**<Sets integration gate. (For information on "gateCut" see HitMatrix class)*/
		void		SetPreGate( double preGate );			/**<Sets time period before integration.*/
		void		SetSpectralSensitivity( TGraph* spect );	/**<Sets the spectral sensitivity via a TGraph*/
		void		SetSpectralSensitivity( const char* file );	/**<Sets the spectral sensitivity via a text file*/

		TGraph*		GetWaveform();					/**<Returns simulated signal waveform*/
		/**Returns signal charge*/
		double		GetCharge(){ return charge.all; };
		/**Returns decomposition of the signal charge*/
		GCharge		GetChargeDecomp(){ return charge; };
		/**Returns length of integration gate in ns*/
		double		GetGate(){ return gate; };
		/**Returns single pixel waveform*/
		TF1*		GetPulseShape(){ return f_pulse_shape; };
		/**Returns hitmatrix*/
		HitMatrix*	GetHitMatrix(){ return hitMatrix; };
		/**Returns number of pixels of the SiPM*/
		int		GetNpix(){ return Npix; };
		/**Returns sampling rate*/
		double		GetSampling(){ return sampling; };

	private:

		TF1*		f_pulse_shape;
		TF1*		f_pulse_shape_intern;
		double		pulse_shape_func_max;
		double		pulse_shape_func_range;
		bool		update_pulse_shape;

		TGraph*		g_spectral;

		int		Npix;
		HitMatrix*	hitMatrix;
		TGraph*		g_waveform;
		TGraph		g_pulse_charge;
		double		pulseIntegral;
		int		n_pulse_samples;
		GCharge		charge;
		double		gate;
		double		pre_gate;

		TRandom		r;
		vector<double>	hit;
		PhotonList	photonList;
		TH2I*		h_geometry;
		double		sampling;
		double		cutOff;
		double		tau1, tau2;

		void		Reset();
		void		ImportPhotons( PhotonList photons );
		void		InitHitMatrix();
};

#endif
