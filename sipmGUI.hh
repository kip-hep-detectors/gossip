#ifndef sipmGUI_hh
#define sipmGUI_hh

#include <TApplication.h>
#include <TGButton.h>
#include <TGButtonGroup.h>
#include <TRootEmbeddedCanvas.h>
#include <TFile.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TGClient.h>
#include <TGNumberEntry.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGComboBox.h>
#include <TGProgressBar.h>
#include "sipmMC.hh"
#include "PhotonSource.hh"
#include "daqMC.hh"


class sipmGUI{

private:

    TGTextButton	*fButtonStart, *fButtonExit, *fButtonCancel;
    TGCheckButton       *fNgammaFluct;
    
    TGButtonGroup	*Mselect;
    
    TGNumberEntry 	*fNumberEntryPDE,
			*fNumberEntryGain,
			*fNumberEntryTau_dr,
			*fNumberEntryPap1,
			*fNumberEntryTau_ap1,
			*fNumberEntryPap2,
			*fNumberEntryTau_ap2,
			*fNumberEntryPx,
			*fNumberEntryENF,
			*fNumberEntryEN,
			*fNumberEntryNpx,
			*fNumberEntryNpy,
			*fNumberEntrySizeX,
			*fNumberEntrySizeY,
			*fNumberEntryTau_rec,
			*fNumberEntryTau_le,
			*fNumberEntryTau_te,
			*fNumberEntrySignalAmp,
			*fNumberEntryRMS,
			*fNumberEntryResolution,
			*fNumberEntryCutoff,
			*fNumberEntryNgamma,
			*fNumberEntryLEDtime,
			*fNumberEntryLEDtimeWidth,
			*fNumberEntryLEDxPos,
			*fNumberEntryLEDxWidth,
			*fNumberEntryLEDyPos,
			*fNumberEntryLEDyWidth,
			*fNumberEntryNentries,
			*fNumberEntryGate,
			*fNumberEntryPedestal,
			*fNumberEntryThreshStart,
			*fNumberEntryThreshStop,
			*fNumberEntryThreshStep,
			*fNumberEntryDiscriMinTime,
			*fNumberEntryDiscriWidth,
			*fNumberEntryNgammaMax,
			*fNumberEntryNgammaStep;
    
    TGLabel 		*fLabelPDE,
			*fLabelGain,
			*fLabelTau_dr,
			*fLabelPap1,
			*fLabelTau_ap1,
			*fLabelPap2,
			*fLabelTau_ap2,
			*fLabelPx,
			*fLabelENF,
			*fLabelEN,
			*fLabelNpx,
			*fLabelNpy,
			*fLabelSizeX,
			*fLabelSizeY,
			*fLabelTau_rec,
                  	*fLabelTau_le,
			*fLabelTau_te,
			*fLabelSignalAmp,
			*fLabelRMS,
			*fLabelResolution,
			*fLabelCutoff,
			*fLabelNgamma,
			*fLabelLEDtime,
			*fLabelLEDtimeWidth,
			*fLabelLEDxPos,
			*fLabelLEDxWidth,
			*fLabelLEDyPos,
			*fLabelLEDyWidth,
			*fLabelNentries,
			*fLabelGate,
			*fLabelPedestal,
			*fLabelLEDshape,
			*fLabelLEDpulse,
			*fLabelNgammaFluct,
			*fLabelThreshStart,
			*fLabelThreshStop,
			*fLabelThreshStep,
			*fLabelDiscriMinTime,
			*fLabelDiscriWidth,
			*fLabelNgammaMax,
			*fLabelNgammaStep;
    
    
    
    TGGroupFrame	*fGframeLightSource,
			*fGframeMeasurement,
			*fGframeParameters,
			*fGframeWaveform,
			*fGframeStart;
    
    TGCompositeFrame    *fHframeSettings,
			*fVframeParaEntries,
			*fHframeMeasurPara,
			*fVframeParaLabels,
			*fVframeWaveformEntries,
			*fVframeWaveformLabels,
			*fVframeLightSourceEntries,
			*fVframeLightSourceLabels,
			*fVframeMeasSelect,
			*fHframeMeasPara,
			*fVframeMeasParaLabels,
			*fVframeMeasParaEntries;
			
			
    TGLayoutHints   	*fLayout1, *fLayout2;
    
    TGComboBox 		*fComboBoxLEDShape,
			*fComboBoxLEDPulse;
    
    TGHProgressBar 	*fHProgressBar;
    
    int 		measurement;
    
public:
  
    sipmGUI();
    ~sipmGUI();
    
    TCanvas *c_main, *c;
    TPad *pad1,
         *pad2;
    sipmMC *sipm;
    daqMC *daq;
    PhotonSource *photonSource;
       
    void RunMeasurement();
    void CancelMeasurement();
    void SetParameters();
    void SetProgress(int);
    void EnableEntries( int );
    void ReadParaFile( const char* filename );
    
};

#endif

