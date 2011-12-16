/********** Version 1.2 **********/

#include "sipmGUI.hh"
#include <TMultiGraph.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>
#include <string>
#include <fstream>
#include <TGIcon.h>

ClassImp (sipmGUI);

sipmGUI::sipmGUI(){

  TGMainFrame *fMainFrame = new TGMainFrame(gClient->GetRoot(),100,100,kMainFrame | kVerticalFrame);

  sipm = new sipmMC();
  photonSource = new PhotonSource();
  daq = new daqMC();
  daq->SetSiPM(sipm);
  daq->SetPhotonSource(photonSource);

  //   TFile *file = new TFile("wf.root");
  //
  //   TCanvas *c1 = (TCanvas*)file->Get("c2");
  //   TH1D *h1 = (TH1D*)c1->GetListOfPrimitives()->FindObject("h");
  //
  //   c1->Close();

  //   sipm->SetPulseShape(h1);

  //Build GUI

  fLayout1 = new TGLayoutHints(kLHintsRight | kLHintsCenterY, 0, 0, 4, 4);
  fLayout2 = new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 0, 7, 7);

  //Canvas
  c_main = new TCanvas("c_main","c_main", 0, 0, 700, 500);

  //Frames
  fHframeSettings = new TGHorizontalFrame(fMainFrame, 400, 400);	//Settings: SiPM, LightSource und Measurement
  fMainFrame->AddFrame(fHframeSettings, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 5, 5));


  //SiPM Parameters

  fGframeParameters = new TGGroupFrame(fHframeSettings, "SiPM Parameters", kHorizontalFrame);	//SiPM Paramters
  fHframeSettings->AddFrame(fGframeParameters, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));

  fVframeParaEntries = new TGVerticalFrame(fGframeParameters, 400, 400);			//Entries
  fGframeParameters->AddFrame(fVframeParaEntries, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 8, 0));

  fVframeParaLabels = new TGVerticalFrame(fGframeParameters, 400, 400);			//Labels
  fGframeParameters->AddFrame(fVframeParaLabels, new TGLayoutHints(kLHintsRight | kLHintsTop, 5, 0, 8, 0));

  {
    fNumberEntryPDE = new TGNumberEntry(fVframeParaEntries,20,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0,100);
    fVframeParaEntries->AddFrame(fNumberEntryPDE, fLayout1);
    fLabelPDE = new TGLabel(fVframeParaLabels,"PDE [%]");
    fVframeParaLabels->AddFrame(fLabelPDE, fLayout2);
    
    fNumberEntryGain = new TGNumberEntry(fVframeParaEntries,20,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeParaEntries->AddFrame(fNumberEntryGain, fLayout1);
    fLabelGain = new TGLabel(fVframeParaLabels,"Gain [QDC-Channels]");
    fVframeParaLabels->AddFrame(fLabelGain, fLayout2);
    
    fNumberEntryTau_dr = new TGNumberEntry(fVframeParaEntries,2000,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
    fVframeParaEntries->AddFrame(fNumberEntryTau_dr, fLayout1);
    fLabelTau_dr = new TGLabel(fVframeParaLabels,"Dark-rate time constant [ns]");
    fVframeParaLabels->AddFrame(fLabelTau_dr, fLayout2);
    
    fNumberEntryPap1 = new TGNumberEntry(fVframeParaEntries,10,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0,100);
    fVframeParaEntries->AddFrame(fNumberEntryPap1, fLayout1);
    fLabelPap1 = new TGLabel(fVframeParaLabels,"After-pulse prob. (slow) [%]");
    fVframeParaLabels->AddFrame(fLabelPap1, fLayout2);
    
    fNumberEntryTau_ap1 = new TGNumberEntry(fVframeParaEntries,200,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeParaEntries->AddFrame(fNumberEntryTau_ap1, fLayout1);
    fLabelTau_ap1 = new TGLabel(fVframeParaLabels,"After-pulse time constant (slow) [ns]");
    fVframeParaLabels->AddFrame(fLabelTau_ap1, fLayout2);
    
    fNumberEntryPap2 = new
TGNumberEntry(fVframeParaEntries,10,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0,100);
    fVframeParaEntries->AddFrame(fNumberEntryPap2, fLayout1);
    fLabelPap2 = new TGLabel(fVframeParaLabels,"After-pulse prob. (fast) [%]");
    fVframeParaLabels->AddFrame(fLabelPap2, fLayout2);
    
    fNumberEntryTau_ap2 = new TGNumberEntry(fVframeParaEntries,50,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeParaEntries->AddFrame(fNumberEntryTau_ap2, fLayout1);
    fLabelTau_ap2 = new TGLabel(fVframeParaLabels,"After-pulse time constant (fast) [ns]");
    fVframeParaLabels->AddFrame(fLabelTau_ap2, fLayout2);
    
    fNumberEntryPx = new TGNumberEntry(fVframeParaEntries,10,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0,100);
    fVframeParaEntries->AddFrame(fNumberEntryPx, fLayout1);
    fLabelPx = new TGLabel(fVframeParaLabels,"Cross-talk prob. [%]");
    fVframeParaLabels->AddFrame(fLabelPx, fLayout2);
    
    fNumberEntryENF = new TGNumberEntry(fVframeParaEntries,2,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
    fVframeParaEntries->AddFrame(fNumberEntryENF, fLayout1);
    fLabelENF = new TGLabel(fVframeParaLabels,"Excess noise [QDC-Channles]");
    fVframeParaLabels->AddFrame(fLabelENF, fLayout2);
    
    fNumberEntryEN = new TGNumberEntry(fVframeParaEntries,2,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
    fVframeParaEntries->AddFrame(fNumberEntryEN, fLayout1);
    fLabelEN = new TGLabel(fVframeParaLabels,"Electronic noise [QDC-Channles]");
    fVframeParaLabels->AddFrame(fLabelEN, fLayout2);
    
    fNumberEntryNpx = new TGNumberEntry(fVframeParaEntries,10,6,-1, TGNumberFormat::kNESInteger,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeParaEntries->AddFrame(fNumberEntryNpx, fLayout1);
    fLabelNpx = new TGLabel(fVframeParaLabels,"Number of pixels in x");
    fVframeParaLabels->AddFrame(fLabelNpx, fLayout2);
    
    fNumberEntryNpy = new TGNumberEntry(fVframeParaEntries,10,6,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeParaEntries->AddFrame(fNumberEntryNpy, fLayout1);
    fLabelNpy = new TGLabel(fVframeParaLabels,"Number of pixels in y");
    fVframeParaLabels->AddFrame(fLabelNpy, fLayout2);
    
    fNumberEntrySizeX = new
TGNumberEntry(fVframeParaEntries,1,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeParaEntries->AddFrame(fNumberEntrySizeX, fLayout1);
    fLabelSizeX = new TGLabel(fVframeParaLabels,"SiPM size in x [mm]");
    fVframeParaLabels->AddFrame(fLabelSizeX, fLayout2);
    
    fNumberEntrySizeY = new TGNumberEntry(fVframeParaEntries,1,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeParaEntries->AddFrame(fNumberEntrySizeY, fLayout1);
    fLabelSizeY = new TGLabel(fVframeParaLabels,"SiPM size in y [mm]");
    fVframeParaLabels->AddFrame(fLabelSizeY, fLayout2);
    
    fNumberEntryTau_rec = new TGNumberEntry(fVframeParaEntries,10,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeParaEntries->AddFrame(fNumberEntryTau_rec, fLayout1);
    fLabelTau_rec = new TGLabel(fVframeParaLabels,"Recovery time [ns]");
    fVframeParaLabels->AddFrame(fLabelTau_rec, fLayout2);
  }


  //Waveform
  fGframeWaveform = new TGGroupFrame(fHframeSettings, "SiPM Waveform", kHorizontalFrame);	//SiPM Waveform
  fHframeSettings->AddFrame(fGframeWaveform, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));

  fVframeWaveformEntries = new TGVerticalFrame(fGframeWaveform, 400, 400);			//Entries
  fGframeWaveform->AddFrame(fVframeWaveformEntries, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 8, 0));

  fVframeWaveformLabels = new TGVerticalFrame(fGframeWaveform, 400, 400);			//Labels
  fGframeWaveform->AddFrame(fVframeWaveformLabels, new TGLayoutHints(kLHintsRight | kLHintsTop, 5, 0, 8, 0));

  {
    fNumberEntryTau_le = new TGNumberEntry(fVframeWaveformEntries,1,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeWaveformEntries->AddFrame(fNumberEntryTau_le, fLayout1);
    fLabelTau_le = new TGLabel(fVframeWaveformLabels,"Leading edge time constant [ns]");
    fVframeWaveformLabels->AddFrame(fLabelTau_le, fLayout2);
    
    fNumberEntryTau_te = new TGNumberEntry(fVframeWaveformEntries,40,6, -1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeWaveformEntries->AddFrame(fNumberEntryTau_te, fLayout1);
    fLabelTau_te = new TGLabel(fVframeWaveformLabels,"Trailing edge time constant [ns]");
    fVframeWaveformLabels->AddFrame(fLabelTau_te, fLayout2);
    
    fNumberEntrySignalAmp = new TGNumberEntry(fVframeWaveformEntries,20,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeWaveformEntries->AddFrame(fNumberEntrySignalAmp, fLayout1);
    fLabelSignalAmp = new TGLabel(fVframeWaveformLabels,"Signal amplitude");
    fVframeWaveformLabels->AddFrame(fLabelSignalAmp, fLayout2);
    
    fNumberEntryRMS = new TGNumberEntry(fVframeWaveformEntries,1.5,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
    fVframeWaveformEntries->AddFrame(fNumberEntryRMS, fLayout1);
    fLabelRMS = new TGLabel(fVframeWaveformLabels,"Noise RMS");
    fVframeWaveformLabels->AddFrame(fLabelRMS, fLayout2);
    
    fNumberEntryResolution = new TGNumberEntry(fVframeWaveformEntries,0.1,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeWaveformEntries->AddFrame(fNumberEntryResolution, fLayout1);
    fLabelResolution = new TGLabel(fVframeWaveformLabels,"Resolution [ns]");
    fVframeWaveformLabels->AddFrame(fLabelResolution, fLayout2);
    
    fNumberEntryCutoff = new TGNumberEntry(fVframeWaveformEntries,0.01,6,-1,TGNumberFormat::kNESRealThree,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
    fVframeWaveformEntries->AddFrame(fNumberEntryCutoff, fLayout1);
    fLabelCutoff = new TGLabel(fVframeWaveformLabels,"Cutoff [%]");
    fVframeWaveformLabels->AddFrame(fLabelCutoff, fLayout2);
  }


  //Measurement

  fGframeMeasurement = new TGGroupFrame(fHframeSettings, "Measurement", kVerticalFrame);	//Measurement
  fHframeSettings->AddFrame(fGframeMeasurement, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));

  Mselect = new TGButtonGroup(fGframeMeasurement,"Select Measurement");
  new TGRadioButton(Mselect, "QDC Spectrum");
  new TGRadioButton(Mselect, "Time Spectrum");
  new TGRadioButton(Mselect, "Threshold Scan");
  new TGRadioButton(Mselect, "Oscilloscope");
  new TGRadioButton(Mselect, "Statistic");
  new TGRadioButton(Mselect, "Dynamic Range");
  Mselect->SetButton(kTextLeft);
  Mselect->Connect("Pressed(Int_t)", "sipmGUI", this, "EnableEntries( int )");
  fGframeMeasurement->AddFrame(Mselect,new TGLayoutHints(kLHintsLeft, 0, 0, 5, 0));
  measurement=1;

  fHframeMeasurPara = new TGHorizontalFrame(fGframeMeasurement, 400, 400);	//Settings: LightSource und Measurement
  fGframeMeasurement->AddFrame(fHframeMeasurPara, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  fVframeMeasParaEntries = new TGVerticalFrame(fHframeMeasurPara, 400, 400);			//Entries
  fHframeMeasurPara->AddFrame(fVframeMeasParaEntries, new TGLayoutHints(kLHintsLeft | kLHintsTop, 5, 0, 8, 0));

  fVframeMeasParaLabels = new TGVerticalFrame(fHframeMeasurPara, 400, 400);			//Labels
  fHframeMeasurPara->AddFrame(fVframeMeasParaLabels, new TGLayoutHints(kLHintsRight | kLHintsTop, 5, 0, 8, 0));

  {
    fNumberEntryNentries = new TGNumberEntry(fVframeMeasParaEntries,1000,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeMeasParaEntries->AddFrame(fNumberEntryNentries, fLayout1);
    fLabelNentries = new TGLabel(fVframeMeasParaLabels,"Number of entries");
    fVframeMeasParaLabels->AddFrame(fLabelNentries, fLayout2);
    
    fNumberEntryGate = new TGNumberEntry(fVframeMeasParaEntries,300,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeMeasParaEntries->AddFrame(fNumberEntryGate, fLayout1);
    fLabelGate = new TGLabel(fVframeMeasParaLabels,"Integration gate [ns]   ");
    fVframeMeasParaLabels->AddFrame(fLabelGate, fLayout2);
    
    fNumberEntryPedestal = new TGNumberEntry(fVframeMeasParaEntries,50,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
    fVframeMeasParaEntries->AddFrame(fNumberEntryPedestal, fLayout1);
    fLabelPedestal = new TGLabel(fVframeMeasParaLabels,"Pedestal position");
    fVframeMeasParaLabels->AddFrame(fLabelPedestal, fLayout2);
    
    fNumberEntryThreshStart = new TGNumberEntry(fVframeMeasParaEntries,15,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeMeasParaEntries->AddFrame(fNumberEntryThreshStart, fLayout1);
    fLabelThreshStart = new TGLabel(fVframeMeasParaLabels,"Threshold start");
    fVframeMeasParaLabels->AddFrame(fLabelThreshStart, fLayout2);
    
    fNumberEntryThreshStop = new TGNumberEntry(fVframeMeasParaEntries,60,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeMeasParaEntries->AddFrame(fNumberEntryThreshStop, fLayout1);
    fLabelThreshStop = new TGLabel(fVframeMeasParaLabels,"Threshold stop");
    fVframeMeasParaLabels->AddFrame(fLabelThreshStop, fLayout2);
    
    fNumberEntryThreshStep = new TGNumberEntry(fVframeMeasParaEntries,1,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeMeasParaEntries->AddFrame(fNumberEntryThreshStep, fLayout1);
    fLabelThreshStep = new TGLabel(fVframeMeasParaLabels,"Threshold step");
    fVframeMeasParaLabels->AddFrame(fLabelThreshStep, fLayout2);
    
    fNumberEntryDiscriMinTime = new TGNumberEntry(fVframeMeasParaEntries,3,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
    fVframeMeasParaEntries->AddFrame(fNumberEntryDiscriMinTime, fLayout1);
    fLabelDiscriMinTime = new TGLabel(fVframeMeasParaLabels,"Discri. mintime");
    fVframeMeasParaLabels->AddFrame(fLabelDiscriMinTime, fLayout2);
    
    fNumberEntryDiscriWidth = new TGNumberEntry(fVframeMeasParaEntries,10,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
    fVframeMeasParaEntries->AddFrame(fNumberEntryDiscriWidth, fLayout1);
    fLabelDiscriWidth = new TGLabel(fVframeMeasParaLabels,"Discri. width");
    fVframeMeasParaLabels->AddFrame(fLabelDiscriWidth, fLayout2);
    
    fNumberEntryNgammaMax = new TGNumberEntry(fVframeMeasParaEntries,1000,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeMeasParaEntries->AddFrame(fNumberEntryNgammaMax, fLayout1);
    fLabelNgammaMax = new TGLabel(fVframeMeasParaLabels,"Nphotons max.");
    fVframeMeasParaLabels->AddFrame(fLabelNgammaMax, fLayout2);
    
    fNumberEntryNgammaStep = new TGNumberEntry(fVframeMeasParaEntries,10,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
    fVframeMeasParaEntries->AddFrame(fNumberEntryNgammaStep, fLayout1);
    fLabelNgammaStep = new TGLabel(fVframeMeasParaLabels,"Nphotons step");
    fVframeMeasParaLabels->AddFrame(fLabelNgammaStep, fLayout2);
  }


  //Light Source

  fGframeLightSource = new TGGroupFrame(fHframeSettings, "Light Source", kHorizontalFrame);	//Light Source Paramters
  fHframeSettings->AddFrame(fGframeLightSource, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));

  fVframeLightSourceEntries = new TGVerticalFrame(fGframeLightSource, 400, 400);		//Entries
  fGframeLightSource->AddFrame(fVframeLightSourceEntries, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 8, 0));

  fVframeLightSourceLabels = new TGVerticalFrame(fGframeLightSource, 400, 400);		//Labels
  fGframeLightSource->AddFrame(fVframeLightSourceLabels, new TGLayoutHints(kLHintsRight | kLHintsTop, 5, 0, 8, 0));

  {
    fNumberEntryNgamma = new TGNumberEntry(fVframeLightSourceEntries,10,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
    fVframeLightSourceEntries->AddFrame(fNumberEntryNgamma, fLayout1);
    fLabelNgamma = new TGLabel(fVframeLightSourceLabels,"Number of photons");
    fVframeLightSourceLabels->AddFrame(fLabelNgamma, fLayout2);
    
    fNumberEntryLEDtime = new TGNumberEntry(fVframeLightSourceEntries,5,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
    fVframeLightSourceEntries->AddFrame(fNumberEntryLEDtime, fLayout1);
    fLabelLEDtime = new TGLabel(fVframeLightSourceLabels,"Time [ns]");
    fVframeLightSourceLabels->AddFrame(fLabelLEDtime, fLayout2);
    
    fNumberEntryLEDtimeWidth = new TGNumberEntry(fVframeLightSourceEntries,5,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
    fVframeLightSourceEntries->AddFrame(fNumberEntryLEDtimeWidth, fLayout1);
    fLabelLEDtimeWidth = new TGLabel(fVframeLightSourceLabels,"Time width [ns]");
    fVframeLightSourceLabels->AddFrame(fLabelLEDtimeWidth, fLayout2);
    
    fNumberEntryLEDxPos = new TGNumberEntry(fVframeLightSourceEntries,0,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELNoLimits);
    fVframeLightSourceEntries->AddFrame(fNumberEntryLEDxPos, fLayout1);
    fLabelLEDxPos = new TGLabel(fVframeLightSourceLabels,"X position [mm]");
    fVframeLightSourceLabels->AddFrame(fLabelLEDxPos, fLayout2);
    
    fNumberEntryLEDyPos = new TGNumberEntry(fVframeLightSourceEntries,0,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELNoLimits);
    fVframeLightSourceEntries->AddFrame(fNumberEntryLEDyPos, fLayout1);
    fLabelLEDyPos = new TGLabel(fVframeLightSourceLabels,"Y position [mm]");
    fVframeLightSourceLabels->AddFrame(fLabelLEDyPos, fLayout2);
    
    fNumberEntryLEDxWidth = new TGNumberEntry(fVframeLightSourceEntries,1,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
    fVframeLightSourceEntries->AddFrame(fNumberEntryLEDxWidth, fLayout1);
    fLabelLEDxWidth = new TGLabel(fVframeLightSourceLabels,"X width [mm]");
    fVframeLightSourceLabels->AddFrame(fLabelLEDxWidth, fLayout2);
    
    fNumberEntryLEDyWidth = new TGNumberEntry(fVframeLightSourceEntries,1,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
    fVframeLightSourceEntries->AddFrame(fNumberEntryLEDyWidth, fLayout1);
    fLabelLEDyWidth = new TGLabel(fVframeLightSourceLabels,"Y width [mm]");
    fVframeLightSourceLabels->AddFrame(fLabelLEDyWidth, fLayout2);
    
    
    fComboBoxLEDShape = new TGComboBox(fVframeLightSourceEntries,-1, kHorizontalFrame | kSunkenFrame | kDoubleBorder | kOwnBackground);
    fVframeLightSourceEntries->AddFrame(fComboBoxLEDShape, fLayout1);
    fLabelLEDshape = new TGLabel(fVframeLightSourceLabels,"Shape");
    fVframeLightSourceLabels->AddFrame(fLabelLEDshape, fLayout2);
    
    fComboBoxLEDShape->AddEntry("square",1);
    fComboBoxLEDShape->AddEntry("elliptic",2);
    
    fComboBoxLEDShape->Resize(80,22);
    fComboBoxLEDShape->Select(1);
    
    fComboBoxLEDPulse = new TGComboBox(fVframeLightSourceEntries,-1, kHorizontalFrame | kSunkenFrame | kDoubleBorder | kOwnBackground);
    fVframeLightSourceEntries->AddFrame(fComboBoxLEDPulse, fLayout1);
    fLabelLEDpulse = new TGLabel(fVframeLightSourceLabels,"Pulse");
    fVframeLightSourceLabels->AddFrame(fLabelLEDpulse, fLayout2);
    
    fComboBoxLEDPulse->AddEntry("flat",1);
    fComboBoxLEDPulse->AddEntry("gaus",2);
    fComboBoxLEDPulse->AddEntry("exp",3);
    
    fComboBoxLEDPulse->Resize(80,22);
    fComboBoxLEDPulse->Select(1);

    
    fNgammaFluct = new TGCheckButton(fVframeLightSourceEntries,"");
    fVframeLightSourceEntries->AddFrame(fNgammaFluct, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, 6, 0));
    fLabelNgammaFluct = new TGLabel(fVframeLightSourceLabels,"Ngamma fluctuation");
    fVframeLightSourceLabels->AddFrame(fLabelNgammaFluct, fLayout2);
    fNgammaFluct->SetOn();
  }


  //Start Frame
  fGframeStart = new TGGroupFrame(fMainFrame, "GosSiP 2.0", kVerticalFrame);
  fMainFrame->AddFrame(fGframeStart, new TGLayoutHints(kLHintsTop | kLHintsCenterX, 0, 0, 0, 0));

  //Run Button
  fButtonStart = new TGTextButton(fGframeStart,"&Run");
  fGframeStart->AddFrame(fButtonStart, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 2, 2, 10, 2));
  fButtonStart->Connect("Clicked()", "sipmGUI", this, "RunMeasurement()");

  //Cancel Button
  fButtonCancel = new TGTextButton(fGframeStart,"&Cancel");
  fGframeStart->AddFrame(fButtonCancel, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 2, 2, 10, 2));
  fButtonCancel->Connect("Clicked()", "sipmGUI", this, "CancelMeasurement()");

  //Exit Button
  fButtonExit = new TGTextButton(fGframeStart, "&Exit", "gApplication->Terminate(0)");
  fGframeStart->AddFrame(fButtonExit, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 20, 2));

  //Progress Bar
  fHProgressBar = new TGHProgressBar(fMainFrame,kLHintsExpandX);
  fHProgressBar->SetMin(0);
  fHProgressBar->SetMax(100);
  fHProgressBar->SetBarColor("lightblue");
  fHProgressBar->ShowPosition();
  fMainFrame->AddFrame(fHProgressBar, new TGLayoutHints(kLHintsBottom | kLHintsExpandX, 5, 5, 10, 5));
  daq->Connect("Progress(int)", "sipmGUI", this, "SetProgress(int)");


  //

  TGIcon *fIconLogo = new TGIcon(fMainFrame,"gossip_logo.png");
  fIconLogo->SetName("fIconLogo");
  fMainFrame->AddFrame(fIconLogo, new TGLayoutHints(kLHintsRight | kLHintsBottom,10,10,10,5));
  fIconLogo->Resize(3764/18,1920/18);
  

  fMainFrame->SetWindowName("GosSiP 2.0 - Generic framework for the simulation of Silicon Photomultipliers");
  fMainFrame->SetIconName("GosSiP 2.0");

  fMainFrame->SetMWMHints(kMWMDecorAll, kMWMFuncAll, kMWMInputModeless);
  fMainFrame->MapSubwindows();
  fMainFrame->Resize(fMainFrame->GetDefaultSize());
  fMainFrame->MapWindow();

}


void sipmGUI::SetProgress(int p){
  fHProgressBar->SetPosition(p);
  gSystem->ProcessEvents();
}


void sipmGUI::EnableEntries(int meas){

  if(meas==1){
    measurement=1;
    fNumberEntryGate->SetState(true);
    fNumberEntryNentries->SetState(true);
    fNumberEntryPedestal->SetState(true);
    fNumberEntryNgamma->SetState(true);
    fNumberEntryNentries->SetNumber(10000);
  }
  
  if(meas==2){
    measurement=2;
    fNumberEntryGate->SetState(true);
    fNumberEntryNentries->SetState(true);
    fNumberEntryPedestal->SetState(false);
    fNumberEntryNgamma->SetState(false);
    fNumberEntryNentries->SetNumber(100000);
  }
  
  if(meas==3){
    measurement=3;
    fNumberEntryGate->SetState(true);
    fNumberEntryNentries->SetState(false);
    fNumberEntryPedestal->SetState(false);
    fNumberEntryNgamma->SetState(false);
    fLabelGate->SetText("Integration gate [mu s]");
  }
  
  if(meas!=3) fLabelGate->SetText("Integration gate [ns]   ");

  if(meas==4){
  measurement=4;
  fNumberEntryGate->SetState(true);
  fNumberEntryNentries->SetState(true);
  fNumberEntryPedestal->SetState(false);
  fNumberEntryNgamma->SetState(true);
  fNumberEntryNentries->SetNumber(1);
  }
  
  if(meas==5){
    measurement=5;
    fNumberEntryGate->SetState(true);
    fNumberEntryNentries->SetState(true);
    fNumberEntryPedestal->SetState(true);
    fNumberEntryNgamma->SetState(true);
    fNumberEntryNentries->SetNumber(10000);
  }
  
  if(meas==6){
    measurement=6;
    fNumberEntryGate->SetState(true);
    fNumberEntryNentries->SetState(true);
    fNumberEntryPedestal->SetState(true);
    fNumberEntryNgamma->SetState(true);
    fNumberEntryNentries->SetNumber(10000);
  }
}


void sipmGUI::RunMeasurement(){

  daq->cancel=false;
  SetParameters();

  c_main->cd();

  if(measurement==1) { daq->QDCSpectrum(fNumberEntryNentries->GetNumber()); c_main->SetLogy(0); c_main->SetLogx(0);}
  if(measurement==2) { daq->TDCSpectrum(fNumberEntryNentries->GetNumber()); c_main->SetLogy(); c_main->SetLogx(); }
  if(measurement==3) {
    daq->ThreshScan(fNumberEntryGate->GetNumber()*1000,fNumberEntryThreshStart->GetNumber(),fNumberEntryThreshStop->GetNumber(),fNumberEntryThreshStep->
    GetNumber()); c_main->SetLogy(); c_main->SetLogx(0);
  }
  if(measurement==4) {
    c_main->Clear();
    for(int i=0;i<fNumberEntryNentries->GetNumber();i++)
    {
      sipm->Generate(photonSource->GeneratePhotons());
      daq->Scope(sipm->GetWaveform())->Draw();
    }
    c_main->SetLogy(0); c_main->SetLogx(0);
    sipm->GetHitMatrix()->DrawMatrix();
  }
  if(measurement==5) { daq->Statistic(fNumberEntryNentries->GetNumber()); c_main->SetLogy(0); c_main->SetLogx(0); }
  if(measurement==6) { daq->DynamicRange(fNumberEntryNentries->GetNumber(),fNumberEntryNgammaMax->GetNumber(),fNumberEntryNgammaStep->GetNumber());
  c_main->SetLogy(0); c_main->SetLogx(0); }

  c_main->Update();
  fHProgressBar->Reset();
}


void sipmGUI::CancelMeasurement(){
  daq->cancel=true;
  fHProgressBar->Reset();
  cout << "Measurement Canceled..." << endl;
}


void sipmGUI::SetParameters(){

  //sipm
  sipm->PDE = fNumberEntryPDE->GetNumber()/100.0;
  sipm->tau_dr = fNumberEntryTau_dr->GetNumber();
  sipm->Pap_s = fNumberEntryPap1->GetNumber()/100.0;
  sipm->tau_ap_s = fNumberEntryTau_ap1->GetNumber();
  sipm->Pap_f = fNumberEntryPap2->GetNumber()/100.0;
  sipm->tau_ap_f = fNumberEntryTau_ap2->GetNumber();
  sipm->Pxt = fNumberEntryPx->GetNumber()/100.0;
  sipm->NpixX = fNumberEntryNpx->GetNumber();
  sipm->NpixY = fNumberEntryNpy->GetNumber();
  sipm->xSipm = fNumberEntrySizeX->GetNumber();
  sipm->ySipm = fNumberEntrySizeY->GetNumber();
  sipm->SetPulseShape(fNumberEntryTau_le->GetNumber(), fNumberEntryTau_te->GetNumber(), fNumberEntryResolution->GetNumber(), fNumberEntryCutoff->GetNumber()/100.);
  sipm->gain = fNumberEntryGain->GetNumber();
  sipm->ENF = fNumberEntryENF->GetNumber();
  sipm->EN = fNumberEntryEN->GetNumber();
  sipm->tau_recovery = fNumberEntryTau_rec->GetNumber();
  sipm->signalAmp = fNumberEntrySignalAmp->GetNumber();
  sipm->noiseRMS = fNumberEntryRMS->GetNumber();

  sipm->SetGeometry("square");

  //photon Source
  photonSource->SetNgamma(fNumberEntryNgamma->GetNumber());
  photonSource->SetTime(fNumberEntryLEDtime->GetNumber());
  photonSource->SetTimeWidth(fNumberEntryLEDtimeWidth->GetNumber());
  photonSource->SetXY(fNumberEntryLEDxPos->GetNumber(), fNumberEntryLEDyPos->GetNumber());
  photonSource->SetXYWidth(fNumberEntryLEDxWidth->GetNumber(), fNumberEntryLEDyWidth->GetNumber());
  if(fComboBoxLEDShape->GetSelected()==1) photonSource->SetShape("square");
  if(fComboBoxLEDShape->GetSelected()==2) photonSource->SetShape("elliptic");
  if(fComboBoxLEDPulse->GetSelected()==1) photonSource->SetPulse("flat");
  if(fComboBoxLEDPulse->GetSelected()==2) photonSource->SetPulse("gaus");
  if(fComboBoxLEDPulse->GetSelected()==3) photonSource->SetPulse("exp");
  photonSource->SetFluctuation(fNgammaFluct->IsOn());

  //daq
  sipm->SetGate(fNumberEntryGate->GetNumber());
  daq->SetPedestal(fNumberEntryPedestal->GetNumber());

  daq->SetDiscriMinTime(fNumberEntryDiscriMinTime->GetNumber());
  daq->SetDiscriWidth(fNumberEntryDiscriWidth->GetNumber());
  
}

void sipmGUI::ReadParaFile( const char* filename )
{

  string para, pm, dump;
  ifstream in(filename);
  
  double PDE,gain,tau_dr,Pap_s,tau_ap_s,Pap_f,tau_ap_f,Pxt,ENF,EN,tau_recovery,NpixX,NpixY,xSipm,ySipm,noiseRMS,signalAmp,tau1,tau2,gate;
  
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
    else if(para == "Tau1") in >> tau1;
    else if(para == "Tau2") in >> tau2;
    else if(para == "Gate") in >> gate;
    else getline(in, dump);
    
    if(!in.good()) break;
  }
  
  in.close();
  
  fNumberEntryPDE->SetNumber(PDE*100.0);
  fNumberEntryTau_dr->SetNumber(tau_dr);
  fNumberEntryPap1->SetNumber(Pap_s*100.0);
  fNumberEntryTau_ap1->SetNumber(tau_ap_s);
  fNumberEntryPap2->SetNumber(Pap_f*100.0);
  fNumberEntryTau_ap2->SetNumber(tau_ap_f);
  fNumberEntryPx->SetNumber(Pxt*100.0);
  fNumberEntryNpx->SetNumber(NpixX);
  fNumberEntryNpy->SetNumber(NpixY);
  fNumberEntrySizeX->SetNumber(xSipm);
  fNumberEntrySizeY->SetNumber(ySipm);
  fNumberEntryTau_le->SetNumber(tau1);
  fNumberEntryTau_te->SetNumber(tau2);
  fNumberEntryGain->SetNumber(gain);
  fNumberEntryENF->SetNumber(ENF);
  fNumberEntryEN->SetNumber(EN);
  fNumberEntryTau_rec->SetNumber(tau_recovery);
  fNumberEntrySignalAmp->SetNumber(signalAmp);
  fNumberEntryRMS->SetNumber(noiseRMS);
  fNumberEntryGate->SetNumber(gate);
}


sipmGUI::~sipmGUI(){
  delete sipm, c_main;

  gApplication->Terminate();
}


//------------------------------------
// int main(int argc,char** argv){
// 
//   //     gROOT->Macro("$(HOME)/RootStyle/ilcStylePatrick.C");
// 
//   TApplication app("sipmMC", 0, 0);
//   sipmGUI mainWin;
//   if(argc == 2) mainWin.ReadParaFile(argv[1]);
//   app.Run();
// 
//   
// 
//   return 0;
// }
//------------------------------------

