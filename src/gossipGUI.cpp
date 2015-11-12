#include "gossipGUI.h"

#include "sipmMC.h"
#include "daqMC.h"
#include "PhotonSource.h"

#include <iostream>
#include <fstream>
#include <TRint.h>
#include <TSystem.h>
#include <TGButton.h>
#include <TGButtonGroup.h>
#include <TGTextEntry.h>
#include <TVirtualX.h>


ClassImp (gossipGUI);

using namespace std;

gossipGUI::gossipGUI()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "gossipGUI::gossipGUI()" << endl;

	layout1 = new TGLayoutHints(kLHintsRight | kLHintsCenterY, 0, 0, 4, 4);
	layout2 = new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 0, 7, 7);

	c_main = new TCanvas("c_main","c_main",0,0,700,500);

	sipm = new sipmMC();
	led = new PhotonSource();
	daq = new daqMC();
	daq->SetSiPM(sipm);
	daq->SetPhotonSource(led);
}

gossipGUI::~gossipGUI()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "gossipGUI::~gossipGUI()" << endl;

	delete sipm;
	delete led;
	delete daq;
	delete layout1;
	delete layout2;
	delete c_main;

	gApplication->Terminate();
}

void gossipGUI::BuildSiPMFrame( TGFrame *parentFrame )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "gossipGUI::BuildSiPMFrame( TGFrame *parentFrame )" << endl;

	sipmFrame = new TGCompositeFrame(parentFrame, 60, 20, kHorizontalFrame);

	///Parameters
	TGGroupFrame *parameterGroupFrame = new TGGroupFrame(sipmFrame, "Parameters", kHorizontalFrame);
	sipmFrame->AddFrame(parameterGroupFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));

	TGCompositeFrame *parametersEntriesFrame = new TGCompositeFrame(parameterGroupFrame, 400, 400);			//Entries
	parameterGroupFrame->AddFrame(parametersEntriesFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 8, 0));

	TGCompositeFrame *parametersLabelsFrame = new TGCompositeFrame(parameterGroupFrame, 400, 400);			//Labels
	parameterGroupFrame->AddFrame(parametersLabelsFrame, new TGLayoutHints(kLHintsRight | kLHintsTop, 5, 0, 8, 0));

	entryPDE = new TGNumberEntry(parametersEntriesFrame,20,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0,100);
	parametersEntriesFrame->AddFrame(entryPDE, layout1);
	labelPDE = new TGLabel(parametersLabelsFrame,"PDE [%]");
	parametersLabelsFrame->AddFrame(labelPDE, layout2);

	entryGain = new TGNumberEntry(parametersEntriesFrame,20,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryGain, layout1);
	labelGain = new TGLabel(parametersLabelsFrame,"Gain [1E4]");
	parametersLabelsFrame->AddFrame(labelGain, layout2);

	entryENF = new TGNumberEntry(parametersEntriesFrame,2,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryENF, layout1);
	labelENF = new TGLabel(parametersLabelsFrame,"Gain fluct. [1E4]");
	parametersLabelsFrame->AddFrame(labelENF, layout2);

	entryTau_dr = new TGNumberEntry(parametersEntriesFrame,2000,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryTau_dr, layout1);
	labelTau_dr = new TGLabel(parametersLabelsFrame,"DR time-const. [ns]");
	parametersLabelsFrame->AddFrame(labelTau_dr, layout2);

	entryPap1 = new TGNumberEntry(parametersEntriesFrame,10,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0,100);
	parametersEntriesFrame->AddFrame(entryPap1, layout1);
	labelPap1 = new TGLabel(parametersLabelsFrame,"AP prob. (slow) [%]");
	parametersLabelsFrame->AddFrame(labelPap1, layout2);

	entryTau_ap1 = new TGNumberEntry(parametersEntriesFrame,200,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryTau_ap1, layout1);
	labelTau_ap1 = new TGLabel(parametersLabelsFrame,"AP time-const. (slow) [ns]");
	parametersLabelsFrame->AddFrame(labelTau_ap1, layout2);

	entryPap2 = new TGNumberEntry(parametersEntriesFrame,10,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0,100);
	parametersEntriesFrame->AddFrame(entryPap2, layout1);
	labelPap2 = new TGLabel(parametersLabelsFrame,"AP prob. (fast) [%]");
	parametersLabelsFrame->AddFrame(labelPap2, layout2);

	entryTau_ap2 = new TGNumberEntry(parametersEntriesFrame,50,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryTau_ap2, layout1);
	labelTau_ap2 = new TGLabel(parametersLabelsFrame,"AP time-const. (fast) [ns]");
	parametersLabelsFrame->AddFrame(labelTau_ap2, layout2);

	entryPx = new TGNumberEntry(parametersEntriesFrame,10,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0,100);
	parametersEntriesFrame->AddFrame(entryPx, layout1);
	labelPx = new TGLabel(parametersLabelsFrame,"CT prob. [%]");
	parametersLabelsFrame->AddFrame(labelPx, layout2);

	entryEN = new TGNumberEntry(parametersEntriesFrame,2,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryEN, layout1);
	labelEN = new TGLabel(parametersLabelsFrame,"Pedestal noise");
	parametersLabelsFrame->AddFrame(labelEN, layout2);

	entryTau_rec = new TGNumberEntry(parametersEntriesFrame,10,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryTau_rec, layout1);
	labelTau_rec = new TGLabel(parametersLabelsFrame,"Recovery time [ns]");
	parametersLabelsFrame->AddFrame(labelTau_rec, layout2);

	///Single pixel waveform
	TGGroupFrame *waveformGroupFrame = new TGGroupFrame(sipmFrame, "Single Pixel Waveform", kHorizontalFrame);
	sipmFrame->AddFrame(waveformGroupFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));

	TGCompositeFrame *waveformEntriesFrame = new TGCompositeFrame(waveformGroupFrame, 400, 400);			//Entries
	waveformGroupFrame->AddFrame(waveformEntriesFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 8, 0));

	TGCompositeFrame *waveformLabelsFrame = new TGCompositeFrame(waveformGroupFrame, 400, 400);			//Labels
	waveformGroupFrame->AddFrame(waveformLabelsFrame, new TGLayoutHints(kLHintsRight | kLHintsTop, 5, 0, 8, 0));

	entryTau_le = new TGNumberEntry(waveformEntriesFrame,1,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	waveformEntriesFrame->AddFrame(entryTau_le, layout1);
	labelTau_le = new TGLabel(waveformLabelsFrame,"Rise time [ns]");
	waveformLabelsFrame->AddFrame(labelTau_le, layout2);

	entryTau_te = new TGNumberEntry(waveformEntriesFrame,40,6, -1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	waveformEntriesFrame->AddFrame(entryTau_te, layout1);
	labelTau_te = new TGLabel(waveformLabelsFrame,"Decay time [ns]");
	waveformLabelsFrame->AddFrame(labelTau_te, layout2);

	entryJitter = new TGNumberEntry(waveformEntriesFrame,200,6, -1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
	waveformEntriesFrame->AddFrame(entryJitter, layout1);
	labelJitter = new TGLabel(waveformLabelsFrame,"Jitter [ps]");
	waveformLabelsFrame->AddFrame(labelJitter, layout2);

	entrySignalAmp = new TGNumberEntry(waveformEntriesFrame,20,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	waveformEntriesFrame->AddFrame(entrySignalAmp, layout1);
	labelSignalAmp = new TGLabel(waveformLabelsFrame,"Amplitude [mV]");
	waveformLabelsFrame->AddFrame(labelSignalAmp, layout2);

	entryRMS = new TGNumberEntry(waveformEntriesFrame,1.5,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
	waveformEntriesFrame->AddFrame(entryRMS, layout1);
	labelRMS = new TGLabel(waveformLabelsFrame,"Electronic Noise [mV]");
	waveformLabelsFrame->AddFrame(labelRMS, layout2);

	entrySampling = new TGNumberEntry(waveformEntriesFrame,0.1,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	waveformEntriesFrame->AddFrame(entrySampling, layout1);
	labelSampling = new TGLabel(waveformLabelsFrame,"Sampling [ns]");
	waveformLabelsFrame->AddFrame(labelSampling, layout2);

	entryCutoff = new TGNumberEntry(waveformEntriesFrame,0.01,6,-1,TGNumberFormat::kNESRealThree,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
	waveformEntriesFrame->AddFrame(entryCutoff, layout1);
	labelCutoff = new TGLabel(waveformLabelsFrame,"Cutoff [%]");
	waveformLabelsFrame->AddFrame(labelCutoff, layout2);


	///Geometry
	TGGroupFrame *geometryGroupFrame = new TGGroupFrame(sipmFrame, "Geometry", kHorizontalFrame);
	sipmFrame->AddFrame(geometryGroupFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));

	TGCompositeFrame *geometryEntriesFrame = new TGCompositeFrame(geometryGroupFrame, 400, 400);			//Entries
	geometryGroupFrame->AddFrame(geometryEntriesFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 8, 0));

	TGCompositeFrame *geometryLabelsFrame = new TGCompositeFrame(geometryGroupFrame, 400, 400);			//Labels
	geometryGroupFrame->AddFrame(geometryLabelsFrame, new TGLayoutHints(kLHintsRight | kLHintsTop, 5, 0, 8, 0));

	entryNpx = new TGNumberEntry(geometryEntriesFrame,10,6,-1, TGNumberFormat::kNESInteger,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	geometryEntriesFrame->AddFrame(entryNpx, layout1);
	labelNpx = new TGLabel(geometryLabelsFrame,"nPixels x");
	geometryLabelsFrame->AddFrame(labelNpx, layout2);

	entryNpy = new TGNumberEntry(geometryEntriesFrame,10,6,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	geometryEntriesFrame->AddFrame(entryNpy, layout1);
	labelNpy = new TGLabel(geometryLabelsFrame,"nPixels y");
	geometryLabelsFrame->AddFrame(labelNpy, layout2);

	entrySizeX = new TGNumberEntry(geometryEntriesFrame,1,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	geometryEntriesFrame->AddFrame(entrySizeX, layout1);
	labelSizeX = new TGLabel(geometryLabelsFrame,"Size x [mm]");
	geometryLabelsFrame->AddFrame(labelSizeX, layout2);

	entrySizeY = new TGNumberEntry(geometryEntriesFrame,1,6,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	geometryEntriesFrame->AddFrame(entrySizeY, layout1);
	labelSizeY = new TGLabel(geometryLabelsFrame,"Size y [mm]");
	geometryLabelsFrame->AddFrame(labelSizeY, layout2);

}

void gossipGUI::BuildLightSourceFrame( TGFrame *parentFrame )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "gossipGUI::BuildLightSourceFrame( TGFrame *parentFrame )" << endl;

	lightSourceFrame = new TGCompositeFrame(parentFrame, 60, 20, kVerticalFrame);

	TGGroupFrame *parameterGroupFrame = new TGGroupFrame(lightSourceFrame, "Parameters", kHorizontalFrame);
	lightSourceFrame->AddFrame(parameterGroupFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));

	TGCompositeFrame *parametersEntriesFrame = new TGCompositeFrame(parameterGroupFrame, 400, 400);			//Entries
	parameterGroupFrame->AddFrame(parametersEntriesFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 8, 0));

	TGCompositeFrame *parametersLabelsFrame = new TGCompositeFrame(parameterGroupFrame, 400, 400);			//Labels
	parameterGroupFrame->AddFrame(parametersLabelsFrame, new TGLayoutHints(kLHintsRight | kLHintsTop, 5, 0, 8, 0));

	entryNgamma = new TGNumberEntry(parametersEntriesFrame,10,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryNgamma, layout1);
	labelNgamma = new TGLabel(parametersLabelsFrame,"nPhotons");
	parametersLabelsFrame->AddFrame(labelNgamma, layout2);

	checkButtonPhotonFluct = new TGCheckButton(parametersEntriesFrame,"");
	parametersEntriesFrame->AddFrame(checkButtonPhotonFluct, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, 6, 0));
	labelNgammaFluct = new TGLabel(parametersLabelsFrame,"Fluctuations");
	parametersLabelsFrame->AddFrame(labelNgammaFluct, layout2);
	checkButtonPhotonFluct->SetOn();

	comboBoxLEDpulse = new TGComboBox(parametersEntriesFrame,-1, kHorizontalFrame | kSunkenFrame | kDoubleBorder | kOwnBackground);
	parametersEntriesFrame->AddFrame(comboBoxLEDpulse, layout1);
	labelLEDpulse = new TGLabel(parametersLabelsFrame,"Pulse");
	parametersLabelsFrame->AddFrame(labelLEDpulse, layout2);

	comboBoxLEDpulse->AddEntry("flat",1);
	comboBoxLEDpulse->AddEntry("gaus",2);
	comboBoxLEDpulse->AddEntry("exp",3);
	comboBoxLEDpulse->AddEntry("exp2",4);
	comboBoxLEDpulse->Resize(80,22);
	comboBoxLEDpulse->Select(1);

	entryLEDtime = new TGNumberEntry(parametersEntriesFrame,5,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryLEDtime, layout1);
	labelLEDtime = new TGLabel(parametersLabelsFrame,"Initial time [ns]");
	parametersLabelsFrame->AddFrame(labelLEDtime, layout2);

	entryLEDduration = new TGNumberEntry(parametersEntriesFrame,5,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryLEDduration, layout1);
	labelLEDduration = new TGLabel(parametersLabelsFrame,"Duration [ns]");
	parametersLabelsFrame->AddFrame(labelLEDduration, layout2);

	entryLEDriseTime = new TGNumberEntry(parametersEntriesFrame,5,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryLEDriseTime, layout1);
	labelLEDriseTime = new TGLabel(parametersLabelsFrame,"Rise time [ns]");
	parametersLabelsFrame->AddFrame(labelLEDriseTime, layout2);

	comboBoxLEDgeometry = new TGComboBox(parametersEntriesFrame,-1, kHorizontalFrame | kSunkenFrame | kDoubleBorder | kOwnBackground);
	parametersEntriesFrame->AddFrame(comboBoxLEDgeometry, layout1);
	labelLEDgeometry = new TGLabel(parametersLabelsFrame,"Geometry");
	parametersLabelsFrame->AddFrame(labelLEDgeometry, layout2);

	comboBoxLEDgeometry->AddEntry("square",1);
	comboBoxLEDgeometry->AddEntry("elliptic",2);
	comboBoxLEDgeometry->Resize(80,22);
	comboBoxLEDgeometry->Select(1);

	entryLEDxPos = new TGNumberEntry(parametersEntriesFrame,0,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryLEDxPos, layout1);
	labelLEDxPos = new TGLabel(parametersLabelsFrame,"Position x [mm]");
	parametersLabelsFrame->AddFrame(labelLEDxPos, layout2);

	entryLEDyPos = new TGNumberEntry(parametersEntriesFrame,0,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryLEDyPos, layout1);
	labelLEDyPos = new TGLabel(parametersLabelsFrame,"Position y [mm]");
	parametersLabelsFrame->AddFrame(labelLEDyPos, layout2);

	entryLEDxWidth = new TGNumberEntry(parametersEntriesFrame,1,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryLEDxWidth, layout1);
	labelLEDxWidth = new TGLabel(parametersLabelsFrame,"Width x [mm]");
	parametersLabelsFrame->AddFrame(labelLEDxWidth, layout2);

	entryLEDyWidth = new TGNumberEntry(parametersEntriesFrame,1,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryLEDyWidth, layout1);
	labelLEDyWidth = new TGLabel(parametersLabelsFrame,"Width y [mm]");
	parametersLabelsFrame->AddFrame(labelLEDyWidth, layout2);

}

void gossipGUI::BuildDAQFrame( TGFrame *parentFrame )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "gossipGUI::BuildDAQFrame( TGFrame *parentFrame )" << endl;

	daqFrame = new TGCompositeFrame(parentFrame, 60, 20, kHorizontalFrame);

	TGButtonGroup *buttonGroupMeasurement = new TGButtonGroup(daqFrame,"Select Measurement");
	new TGRadioButton(buttonGroupMeasurement, "Waveform");
	new TGRadioButton(buttonGroupMeasurement, "Charge Spectrum");
	new TGRadioButton(buttonGroupMeasurement, "Time Spectrum");
	new TGRadioButton(buttonGroupMeasurement, "Threshold Scan");
	new TGRadioButton(buttonGroupMeasurement, "Statistic");
	new TGRadioButton(buttonGroupMeasurement, "Response Curve");
	buttonGroupMeasurement->SetButton(kTextLeft);
	buttonGroupMeasurement->Connect("Pressed(Int_t)", "gossipGUI", this, "SelectMeasurement(int)");
	daqFrame->AddFrame(buttonGroupMeasurement,new TGLayoutHints(kLHintsLeft, 0, 0, 5, 0));

	TGGroupFrame *parameterGroupFrame = new TGGroupFrame(daqFrame, "Parameters", kHorizontalFrame);
	daqFrame->AddFrame(parameterGroupFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));

	TGCompositeFrame *parametersEntriesFrame = new TGCompositeFrame(parameterGroupFrame, 400, 400);			//Entries
	parameterGroupFrame->AddFrame(parametersEntriesFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 8, 0));

	TGCompositeFrame *parametersLabelsFrame = new TGCompositeFrame(parameterGroupFrame, 400, 400);			//Labels
	parameterGroupFrame->AddFrame(parametersLabelsFrame, new TGLayoutHints(kLHintsRight | kLHintsTop, 5, 0, 8, 0));

	entryNentries = new TGNumberEntry(parametersEntriesFrame,1000,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryNentries, layout1);
	labelNentries = new TGLabel(parametersLabelsFrame,"nEntries");
	parametersLabelsFrame->AddFrame(labelNentries, layout2);

	entryGate = new TGNumberEntry(parametersEntriesFrame,300,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryGate, layout1);
	labelGate = new TGLabel(parametersLabelsFrame,"Gate [ns]   ");
	parametersLabelsFrame->AddFrame(labelGate, layout2);

	entryPreGate = new TGNumberEntry(parametersEntriesFrame,0,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryPreGate, layout1);
	labelPreGate = new TGLabel(parametersLabelsFrame,"Pregate [ns]   ");
	parametersLabelsFrame->AddFrame(labelPreGate, layout2);

	entryPedestal = new TGNumberEntry(parametersEntriesFrame,50,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryPedestal, layout1);
	labelPedestal = new TGLabel(parametersLabelsFrame,"Pedestal");
	parametersLabelsFrame->AddFrame(labelPedestal, layout2);

	entryThreshStart = new TGNumberEntry(parametersEntriesFrame,15,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryThreshStart, layout1);
	labelThreshStart = new TGLabel(parametersLabelsFrame,"Threshold start");
	parametersLabelsFrame->AddFrame(labelThreshStart, layout2);

	entryThreshStop = new TGNumberEntry(parametersEntriesFrame,60,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryThreshStop, layout1);
	labelThreshStop = new TGLabel(parametersLabelsFrame,"Threshold stop");
	parametersLabelsFrame->AddFrame(labelThreshStop, layout2);

	entryThreshStep = new TGNumberEntry(parametersEntriesFrame,1,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryThreshStep, layout1);
	labelThreshStep = new TGLabel(parametersLabelsFrame,"Threshold step");
	parametersLabelsFrame->AddFrame(labelThreshStep, layout2);

	entryDiscriMinTime = new TGNumberEntry(parametersEntriesFrame,3,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryDiscriMinTime, layout1);
	labelDiscriMinTime = new TGLabel(parametersLabelsFrame,"Discri. mintime");
	parametersLabelsFrame->AddFrame(labelDiscriMinTime, layout2);

	entryDiscriWidth = new TGNumberEntry(parametersEntriesFrame,10,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryDiscriWidth, layout1);
	labelDiscriWidth = new TGLabel(parametersLabelsFrame,"Discri. width");
	parametersLabelsFrame->AddFrame(labelDiscriWidth, layout2);

	entryNgammaMax = new TGNumberEntry(parametersEntriesFrame,1000,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryNgammaMax, layout1);
	labelNgammaMax = new TGLabel(parametersLabelsFrame,"nPhotons max.");
	parametersLabelsFrame->AddFrame(labelNgammaMax, layout2);

	entryNgammaStep = new TGNumberEntry(parametersEntriesFrame,50,8,-1,TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAPositive,TGNumberFormat::kNELNoLimits);
	parametersEntriesFrame->AddFrame(entryNgammaStep, layout1);
	labelNgammaStep = new TGLabel(parametersLabelsFrame,"nPhotons step");
	parametersLabelsFrame->AddFrame(labelNgammaStep, layout2);

}

void gossipGUI::BuildMainFrame()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "gossipGUI::BuildMainFrame()" << endl;

	BuildLoadFrame();

	mainFrame = new TGMainFrame(gClient->GetRoot(), 100, 100, kMainFrame | kVerticalFrame);

	//Tabs
	tab = new TGTab(mainFrame, 300, 300);

	tab->AddTab("SiPM");
	BuildSiPMFrame(tab->GetTabContainer("SiPM"));
	tab->GetTabContainer("SiPM")->AddFrame(sipmFrame);

	tab->AddTab("Light Source");
	BuildLightSourceFrame(tab->GetTabContainer("Light Source"));
	tab->GetTabContainer("Light Source")->AddFrame(lightSourceFrame);

	tab->AddTab("DAQ");
	BuildDAQFrame(tab->GetTabContainer("DAQ"));
	tab->GetTabContainer("DAQ")->AddFrame(daqFrame);
	SelectMeasurement(1);

	tab->Connect("Selected(int)", "gossipGUI", this, "onTabSwitch(int)");

	//Run Button
	TGTextButton *buttonRun = new TGTextButton(mainFrame,"     Run     ");
	mainFrame->AddFrame(buttonRun, new TGLayoutHints(kLHintsCenterX | kLHintsBottom, 2, 2, 5, 5));
	buttonRun->Connect("Clicked()", "gossipGUI", this, "onRunButtonClicked()");

	//Main Frame
	mainFrame->AddFrame(tab, new TGLayoutHints(kLHintsBottom | kLHintsExpandX | kLHintsExpandY, 2, 2, 5, 1));

	mainFrame->SetWindowName("GosSiP");
	mainFrame->SetIconName("GosSiP 1.20");

	mainFrame->SetMWMHints(kMWMDecorAll, kMWMFuncAll, kMWMInputModeless);
	mainFrame->MapSubwindows();
	mainFrame->Resize(mainFrame->GetDefaultSize());
	mainFrame->MapWindow();
}

void gossipGUI::BuildLoadFrame()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "gossipGUI::BuildLoadFrame()" << endl;

	loadFrame = new TGMainFrame(gClient->GetRoot(), 100, 100, kTempFrame | kVerticalFrame);

	//Cancel Button
	TGTextButton *buttonCancel = new TGTextButton(loadFrame,"&Cancel");
	loadFrame->AddFrame(buttonCancel, new TGLayoutHints(kLHintsTop | kLHintsCenterX, 2, 2, 10, 2));
	buttonCancel->Connect("Clicked()", "gossipGUI", this, "onCancelButtonClicked()");

	//Progress Bar
	progressBar = new TGHProgressBar(loadFrame,kLHintsExpandX);
	progressBar->SetMin(0);
	progressBar->SetMax(100);
	progressBar->SetBarColor("lightblue");
	progressBar->ShowPosition();
	progressBar->Resize(TGDimension(200,200));
	loadFrame->AddFrame(progressBar, new TGLayoutHints(kLHintsBottom | kLHintsCenterX, 5, 5, 10, 5));
	daq->Connect("Progress(int)", "gossipGUI", this, "SetProgress(int)");

	loadFrame->SetWindowName("Generating...");
	loadFrame->SetIconName("Generating...");
	loadFrame->MapSubwindows();
	loadFrame->Resize(loadFrame->GetDefaultSize());
}

void gossipGUI::onTabSwitch( int selected )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "gossipGUI::onTabSwitch( int selected )" << endl;

	mainFrame->Resize(tab->GetTabContainer(selected)->GetDefaultSize() + TGDimension(8,31+32));
}

void gossipGUI::onRunButtonClicked()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "gossipGUI::onRunButtonClicked()" << endl;

	//Rebuild main canvas if neccessary
	c_main = (TCanvas*)gROOT->FindObject("c_main");
	if(c_main==0) c_main = new TCanvas("c_main","c_main",0,0,700,500);
	c_main->cd();

	loadFrame->MapWindow();

	int x,y;
	unsigned int w,h;
	gVirtualX->GetGeometry(-1,x,y,w,h);
	loadFrame->Move(w/2-loadFrame->GetWidth()/2,h/2-loadFrame->GetHeight()/2);
	loadFrame->SetWMPosition(w/2-loadFrame->GetWidth()/2,h/2-loadFrame->GetHeight()/2);

	daq->cancel=false;
	SetParameters();

	c_main->cd();

	if(measurement==1)
	{
		c_main->Clear();
		double max = 0;
		double min = 0;
		for(int i=0;i<entryNentries->GetNumber();i++)
		{
			TGraph *g = (TGraph*)daq->Scope()->Clone();	//small memory leak, since clones are not beeing deleted...fix later =)

			if(g->GetYaxis()->GetXmax()>max) max = g->GetYaxis()->GetXmax();
			if(g->GetYaxis()->GetXmin()<min) min = g->GetYaxis()->GetXmin();

			if(i==0) g->Draw("ALP");
			else g->Draw("SAMELP");
		}

		TGraph *g_first = (TGraph*)c_main->GetListOfPrimitives()->At(0);
		g_first->GetYaxis()->SetRangeUser(min,max);

		c_main->SetLogy(false);
		c_main->SetLogx(false);
		sipm->GetHitMatrix()->DrawMatrix();
	}
	if(measurement==2)
	{
		TH1D* h_QDC = daq->QDCSpectrum(entryNentries->GetNumber());
		c_main->SetLogy(false);
		c_main->SetLogx(false);
		//     h_QDC->Draw("HIST E0");
		h_QDC->Draw("");
	}
	if(measurement==3)
	{
		daq->TDCSpectrum(entryNentries->GetNumber());
		c_main->SetLogy(true);
		c_main->SetLogx(true);
	}
	if(measurement==4)
	{
		daq->ThreshScan(entryGate->GetNumber()*1000,entryThreshStart->GetNumber(),entryThreshStop->GetNumber(),entryThreshStep->GetNumber());
		c_main->SetLogy(true);
		c_main->SetLogx(false);
	}
	if(measurement==5)
	{
		daq->Statistic(entryNentries->GetNumber());
		c_main->SetLogy(false);
		c_main->SetLogx(false);
	}
	if(measurement==6)
	{
		daq->DynamicRange(entryNentries->GetNumber(),entryNgammaMax->GetNumber(),entryNgammaStep->GetNumber());
		c_main->SetLogy(false);
		c_main->SetLogx(false);
	}

	c_main->Update();

	progressBar->Reset();

	loadFrame->UnmapWindow();
}

void gossipGUI::SetProgress( int progress )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "gossipGUI::SetProgress( int progress )" << endl;

	progressBar->SetPosition(progress);
	gSystem->ProcessEvents();
}

void gossipGUI::onCancelButtonClicked()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "gossipGUI::onCancelButtonClicked()" << endl;

	daq->cancel=true;
	progressBar->Reset();
}

void gossipGUI::SetParameters()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "gossipGUI::SetParameters()" << endl;

	//Sipm
	sipm->PDE = entryPDE->GetNumber()/100.0;
	sipm->tau_dr = entryTau_dr->GetNumber();
	sipm->Pap_s = entryPap1->GetNumber()/100.0;
	sipm->tau_ap_s = entryTau_ap1->GetNumber();
	sipm->Pap_f = entryPap2->GetNumber()/100.0;
	sipm->tau_ap_f = entryTau_ap2->GetNumber();
	sipm->jitter = entryJitter->GetNumber()/1000.;
	sipm->Pxt = entryPx->GetNumber()/100.0;
	sipm->NpixX = entryNpx->GetNumber();
	sipm->NpixY = entryNpy->GetNumber();
	sipm->xSipm = entrySizeX->GetNumber();
	sipm->ySipm = entrySizeY->GetNumber();
	sipm->SetPulseShape(entryTau_le->GetNumber(), entryTau_te->GetNumber());
	sipm->SetSampling(entrySampling->GetNumber());
	sipm->SetCutoff(entryCutoff->GetNumber()/100.);
	sipm->gain = entryGain->GetNumber();
	sipm->ENF = entryENF->GetNumber();
	sipm->EN = entryEN->GetNumber();
	sipm->tau_recovery = entryTau_rec->GetNumber();
	sipm->signalAmp = entrySignalAmp->GetNumber();
	sipm->noiseRMS = entryRMS->GetNumber();

	sipm->SetGeometry("square");

	//Photon Source
	led->SetNgamma(entryNgamma->GetNumber());
	led->SetTime(entryLEDtime->GetNumber());
	led->SetTimeWidth(entryLEDduration->GetNumber());
	led->SetXY(entryLEDxPos->GetNumber(), entryLEDyPos->GetNumber());
	led->SetXYWidth(entryLEDxWidth->GetNumber(), entryLEDyWidth->GetNumber());
	if(comboBoxLEDgeometry->GetSelected()==1) led->SetShape("square");
	if(comboBoxLEDgeometry->GetSelected()==2) led->SetShape("elliptic");
	if(comboBoxLEDpulse->GetSelected()==1) led->SetPulse("flat");
	if(comboBoxLEDpulse->GetSelected()==2) led->SetPulse("gaus");
	if(comboBoxLEDpulse->GetSelected()==3) led->SetPulse("exp");
	if(comboBoxLEDpulse->GetSelected()==4) led->SetPulse("exp2");
	led->SetFluctuation(checkButtonPhotonFluct->IsOn());

	//Daq
	sipm->SetGate(entryGate->GetNumber());
	sipm->SetPreGate(entryPreGate->GetNumber());
	daq->SetPedestal(entryPedestal->GetNumber());

	daq->SetDiscriMinTime(entryDiscriMinTime->GetNumber());
	daq->SetDiscriWidth(entryDiscriWidth->GetNumber());
}

void gossipGUI::SelectMeasurement( int meas )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "gossipGUI::SelectMeasurement( int meas )" << endl;

	measurement = meas;

	if(measurement==1){
		entryNentries->SetState(true);
		entryGate->SetState(true);
		entryPedestal->SetState(false);
		entryThreshStart->SetState(false);
		entryThreshStop->SetState(false);
		entryThreshStep->SetState(false);
		entryDiscriMinTime->SetState(false);
		entryDiscriWidth->SetState(false);
		entryNgammaMax->SetState(false);
		entryNgammaStep->SetState(false);

		entryNgamma->SetState(true);
		entryLEDtime->SetState(true);
		entryLEDduration->SetState(true);
		entryLEDxPos->SetState(true);
		entryLEDyPos->SetState(true);
		entryLEDxWidth->SetState(true);
		entryLEDyWidth->SetState(true);

		entryNentries->SetNumber(1);
	}

	if(measurement==2){
		entryNentries->SetState(true);
		entryGate->SetState(true);
		entryPedestal->SetState(true);
		entryThreshStart->SetState(false);
		entryThreshStop->SetState(false);
		entryThreshStep->SetState(false);
		entryDiscriMinTime->SetState(false);
		entryDiscriWidth->SetState(false);
		entryNgammaMax->SetState(false);
		entryNgammaStep->SetState(false);

		entryNgamma->SetState(true);
		entryLEDtime->SetState(true);
		entryLEDduration->SetState(true);
		entryLEDxPos->SetState(true);
		entryLEDyPos->SetState(true);
		entryLEDxWidth->SetState(true);
		entryLEDyWidth->SetState(true);

		entryNentries->SetNumber(10000);
	}

	if(measurement==3){
		entryNentries->SetState(true);
		entryGate->SetState(false);
		entryPedestal->SetState(false);
		entryThreshStart->SetState(false);
		entryThreshStop->SetState(false);
		entryThreshStep->SetState(false);
		entryDiscriMinTime->SetState(false);
		entryDiscriWidth->SetState(false);
		entryNgammaMax->SetState(false);
		entryNgammaStep->SetState(false);

		entryNgamma->SetState(false);
		entryLEDtime->SetState(false);
		entryLEDduration->SetState(false);
		entryLEDxPos->SetState(false);
		entryLEDyPos->SetState(false);
		entryLEDxWidth->SetState(false);
		entryLEDyWidth->SetState(false);

		entryNentries->SetNumber(100000);
	}

	if(measurement==4){
		entryNentries->SetState(false);
		entryGate->SetState(true);
		entryPedestal->SetState(false);
		entryThreshStart->SetState(true);
		entryThreshStop->SetState(true);
		entryThreshStep->SetState(true);
		entryDiscriMinTime->SetState(true);
		entryDiscriWidth->SetState(true);
		entryNgammaMax->SetState(false);
		entryNgammaStep->SetState(false);

		entryNgamma->SetState(false);
		entryLEDtime->SetState(false);
		entryLEDduration->SetState(false);
		entryLEDxPos->SetState(false);
		entryLEDyPos->SetState(false);
		entryLEDxWidth->SetState(false);
		entryLEDyWidth->SetState(false);

		labelGate->SetText("Integration gate [mu s]");
	}

	if(measurement!=4) labelGate->SetText("Integration gate [ns]   ");

	if(measurement==5){
		entryNentries->SetState(true);
		entryGate->SetState(true);
		entryPedestal->SetState(false);
		entryThreshStart->SetState(false);
		entryThreshStop->SetState(false);
		entryThreshStep->SetState(false);
		entryDiscriMinTime->SetState(false);
		entryDiscriWidth->SetState(false);
		entryNgammaMax->SetState(false);
		entryNgammaStep->SetState(false);

		entryNgamma->SetState(true);
		entryLEDtime->SetState(true);
		entryLEDduration->SetState(true);
		entryLEDxPos->SetState(true);
		entryLEDyPos->SetState(true);
		entryLEDxWidth->SetState(true);
		entryLEDyWidth->SetState(true);

		entryNentries->SetNumber(10000);
	}

	if(measurement==6){
		entryNentries->SetState(true);
		entryGate->SetState(true);
		entryPedestal->SetState(false);
		entryThreshStart->SetState(false);
		entryThreshStop->SetState(false);
		entryThreshStep->SetState(false);
		entryDiscriMinTime->SetState(false);
		entryDiscriWidth->SetState(false);
		entryNgammaMax->SetState(true);
		entryNgammaStep->SetState(true);

		entryNgamma->SetState(true);
		entryLEDtime->SetState(true);
		entryLEDduration->SetState(true);
		entryLEDxPos->SetState(true);
		entryLEDyPos->SetState(true);
		entryLEDxWidth->SetState(true);
		entryLEDyWidth->SetState(true);

		entryNentries->SetNumber(1000);
	}
}

void gossipGUI::ReadParaFile( const char* filename )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "gossipGUI::ReadParaFile( const char* filename )" << endl;

	string para, pm, dump;
	ifstream in(filename);

	double value;

	while(1)
	{
		in >> para;
		if(para == "PDE")
		{
			in >> value;
			entryPDE->SetNumber(value*100);
		}
		else if(para == "Gain")
		{
			in >> value;
			entryGain->SetNumber(value);
		}
		else if(para == "TauDR")
		{
			in >> value;
			entryTau_dr->SetNumber(value);
		}
		else if(para == "AP_s")
		{
			in >> value;
			entryPap1->SetNumber(value*100);
		}
		else if(para == "TauAP_s")
		{
			in >> value;
			entryTau_ap1->SetNumber(value);
		}
		else if(para == "AP_f")
		{
			in >> value;
			entryPap2->SetNumber(value*100);
		}
		else if(para == "TauAP_f")
		{
			in >> value;
			entryTau_ap2->SetNumber(value);
		}
		else if(para == "XT")
		{
			in >> value;
			entryPx->SetNumber(value*100);
		}
		else if(para == "ENF")
		{
			in >> value;
			entryENF->SetNumber(value);
		}
		else if(para == "EN")
		{
			in >> value;
			entryEN->SetNumber(value);
		}
		else if(para == "TauRec")
		{
			in >> value;
			entryTau_rec->SetNumber(value);
		}
		else if(para == "Jitter")
		{
			in >> value;
			entryJitter->SetNumber(value);
		}
		else if(para == "Npx")
		{
			in >> value;
			entryNpx->SetNumber(value);
		}
		else if(para == "Npy")
		{
			in >> value;
			entryNpy->SetNumber(value);
		}
		else if(para == "SizeX")
		{
			in >> value;
			entrySizeX->SetNumber(value);
		}
		else if(para == "SizeY")
		{
			in >> value;
			entrySizeY->SetNumber(value);
		}
		else if(para == "NoiseRMS")
		{
			in >> value;
			entryRMS->SetNumber(value);
		}
		else if(para == "SignalAmp")
		{
			in >> value;
			entrySignalAmp->SetNumber(value);
		}
		else if(para == "Tau1")
		{
			in >> value;
			entryTau_le->SetNumber(value);
		}
		else if(para == "Tau2")
		{
			in >> value;
			entryTau_te->SetNumber(value);
		}
		else getline(in, dump);

		if(!in.good()) break;
	}
}

