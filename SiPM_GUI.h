#ifndef SiPM_GUI_H
#define SiPM_GUI_H

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
#include <TROOT.h>
#include <TGProgressBar.h>
#include "SiPM_MC.h"


class SiPM_GUI{

private:
  
    TGTextButton	*fButtonStart, *fButtonExit, *fButtonCancel;
    
    TGButtonGroup	*Mselect;
    
    TGNumberEntry 	*fNumberEntryPDE, *fNumberEntryTau_dr, *fNumberEntryPap1,
			*fNumberEntryTau_ap1, *fNumberEntryPap2, *fNumberEntryTau_ap2,
                  	*fNumberEntryPx, *fNumberEntryNgamma, *fNumberEntryNpx,
                  	*fNumberEntryNpy, *fNumberEntryGate, *fNumberEntryTau_rec,
                  	*fNumberEntryTau_le, *fNumberEntryTau_te, *fNumberEntryNentries,
                  	*fNumberEntryGain, *fNumberEntryLEDwidth, *fNumberEntryENF,
			*fNumberEntryEN, *fNumberEntryPedestal, *fNumberEntryLEDspot;
    
    TGLabel		*fLabelPDE, *fLabelTau_dr, *fLabelPap1, *fLabelTau_ap1,
			*fLabelPap2, *fLabelTau_ap2, *fLabelPx, *fLabelNgamma,
			*fLabelNpx, *fLabelNpy, *fLabelGate, *fLabelTau_rec,
			*fLabelTau_le, *fLabelTau_te, *fLabelPedestal, *fLabelNentries,
			*fLabelGain, *fLabelLEDwidth, *fLabelENF, *fLabelEN, *fLabelLEDspot;
			
    TGGroupFrame	*fGframeMeasurement, *fGframeParameters, *fGframeStart;
    
    TGCompositeFrame    *fHframeSettings, *fVframeParaEntries, *fVframeParaLabels,
			*fVframeMeasSelect, *fHframeMeasPara, *fVframeMeasParaLabels, *fVframeMeasParaEntries;
			
    TGLayoutHints   	*fLayout1, *fLayout2;
    
    TGComboBox 		*fComboBoxSelect;
    
    TGHProgressBar 	*fHProgressBar;
    
    int 		measurement; 
    
    
public:
  
    SiPM_GUI();
    ~SiPM_GUI();

    TCanvas *c_main;
    SiPM_MC *sipm;
    void RunMeasurement();
    void CancelMeasurement();
    void SetParameters();
    void SetProgress(int);
    void EnableEntries( int );
 
};

#endif

