
#ifndef gossipGUI_hh
#define gossipGUI_hh

#include "TGFrame.h"
#include "TGTab.h"
#include "TGNumberEntry.h"
#include "TGLabel.h"
#include "TGComboBox.h"
#include "TGProgressBar.h"
#include "TCanvas.h"

#include "sipmMC.h"
#include "PhotonSource.h"
#include "daqMC.h"

class gossipGUI{

	public:

		gossipGUI();
		~gossipGUI();

		void BuildMainFrame();
		void BuildLoadFrame();
		void BuildSiPMFrame( TGFrame *parentFrame );
		void BuildLightSourceFrame( TGFrame *parentFrame );
		void BuildDAQFrame( TGFrame *parentFrame );

		void onTabSwitch( int );
		void onRunButtonClicked();
		void onCancelButtonClicked();
		void SetParameters();
		void SetProgress( int );
		void SelectMeasurement( int );
		void ReadParaFile( const char* filename );

		sipmMC* GetSiPM(){ return sipm; };

	private:

		sipmMC		*sipm;
		PhotonSource		*led;
		daqMC			*daq;

		int 			measurement;

		TCanvas 		*c_main;

		TGMainFrame 		*mainFrame,
					*loadFrame;

		TGCompositeFrame 	*sipmFrame;
		TGCompositeFrame 	*lightSourceFrame;
		TGCompositeFrame 	*daqFrame;

		TGTab 		*tab;

		TGLayoutHints 	*layout1,
				*layout2;

		TGHProgressBar 	*progressBar;

		TGComboBox		*comboBoxLEDgeometry,
					*comboBoxLEDpulse;

		TGCheckButton 	*checkButtonPhotonFluct;

		TGNumberEntry 	*entryPDE,
				*entryGain,
				*entryTau_dr,
				*entryPap1,
				*entryTau_ap1,
				*entryPap2,
				*entryTau_ap2,
				*entryPx,
				*entryENF,
				*entryEN,
				*entryNpx,
				*entryNpy,
				*entrySizeX,
				*entrySizeY,
				*entryTau_rec,
				*entryTau_le,
				*entryTau_te,
				*entryJitter,
				*entrySignalAmp,
				*entryRMS,
				*entrySampling,
				*entryCutoff,
				*entryNgamma,
				*entryLEDtime,
				*entryLEDduration,
				*entryLEDriseTime,
				*entryLEDxPos,
				*entryLEDxWidth,
				*entryLEDyPos,
				*entryLEDyWidth,
				*entryNentries,
				*entryGate,
				*entryPreGate,
				*entryPedestal,
				*entryThreshStart,
				*entryThreshStop,
				*entryThreshStep,
				*entryDiscriMinTime,
				*entryDiscriWidth,
				*entryNgammaMax,
				*entryNgammaStep;

		TGLabel 		*labelPDE,
					*labelGain,
					*labelTau_dr,
					*labelPap1,
					*labelTau_ap1,
					*labelPap2,
					*labelTau_ap2,
					*labelPx,
					*labelENF,
					*labelEN,
					*labelNpx,
					*labelNpy,
					*labelSizeX,
					*labelSizeY,
					*labelTau_rec,
					*labelTau_le,
					*labelTau_te,
					*labelJitter,
					*labelSignalAmp,
					*labelRMS,
					*labelSampling,
					*labelCutoff,
					*labelNgamma,
					*labelLEDtime,
					*labelLEDduration,
					*labelLEDriseTime,
					*labelLEDxPos,
					*labelLEDxWidth,
					*labelLEDyPos,
					*labelLEDyWidth,
					*labelNentries,
					*labelGate,
					*labelPreGate,
					*labelPedestal,
					*labelLEDgeometry,
					*labelLEDpulse,
					*labelNgammaFluct,
					*labelThreshStart,
					*labelThreshStop,
					*labelThreshStep,
					*labelDiscriMinTime,
					*labelDiscriWidth,
					*labelNgammaMax,
					*labelNgammaStep;
};

#endif
