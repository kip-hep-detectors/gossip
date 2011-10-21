/********** Version 1.2 **********/

#include "SiPM_GUI.h"
#include <TMultiGraph.h>
//#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>
#include <string>


//ClassImp (SiPM_GUI);

SiPM_GUI::SiPM_GUI(){
  
   TGMainFrame *fMainFrame = new TGMainFrame(gClient->GetRoot(),100,100,kMainFrame | kVerticalFrame);
    
   sipm = new SiPM_MC();

//Build GUI

   fLayout1 = new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 0, 4, 4);
   fLayout2 = new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 0, 7, 7);

//Canvas
   c_main = new TCanvas("c_main","c_main", 0, 0, 700, 500);
  /* 
   TStyle *ilcStyle = new TStyle("ILC","Ilc style");
   ilcStyle->SetFillColor(0);
   ilcStyle->SetPalette(1);
   gROOT->SetStyle("ILC");
   gROOT->ForceStyle();
   */
//Frames
   fHframeSettings = new TGHorizontalFrame(fMainFrame, 400, 400);	//Parameters und Measurement
   fMainFrame->AddFrame(fHframeSettings, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 5, 5));
    
   fGframeParameters = new TGGroupFrame(fHframeSettings, "Parameters", kHorizontalFrame);	//Paramters
   fHframeSettings->AddFrame(fGframeParameters, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
     
   
//Parameter Entries

   fVframeParaEntries = new TGVerticalFrame(fGframeParameters, 400, 400);	//Parameter Entries
   fGframeParameters->AddFrame(fVframeParaEntries, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 8, 0));

   fNumberEntryPDE = new TGNumberEntry(fVframeParaEntries,12.0,6,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 2,0,100);
   fVframeParaEntries->AddFrame(fNumberEntryPDE, fLayout1);
   fNumberEntryGain = new TGNumberEntry(fVframeParaEntries,1.4,6,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 2,(TGNumberFormat::ELimit) 0);
   fVframeParaEntries->AddFrame(fNumberEntryGain, fLayout1);
   fNumberEntryTau_dr = new TGNumberEntry(fVframeParaEntries,2799,6,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 0);
   fVframeParaEntries->AddFrame(fNumberEntryTau_dr, fLayout1);
   fNumberEntryPap1 = new TGNumberEntry(fVframeParaEntries,13.9,6,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 2,0,100);
   fVframeParaEntries->AddFrame(fNumberEntryPap1, fLayout1);
   fNumberEntryTau_ap1 = new TGNumberEntry(fVframeParaEntries,225.6,6,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 2,(TGNumberFormat::ELimit) 0);
   fVframeParaEntries->AddFrame(fNumberEntryTau_ap1, fLayout1);
   fNumberEntryPap2 = new TGNumberEntry(fVframeParaEntries,22.8,6,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 2,0,100);
   fVframeParaEntries->AddFrame(fNumberEntryPap2, fLayout1);
   fNumberEntryTau_ap2 = new TGNumberEntry(fVframeParaEntries,83.2,6,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 2,(TGNumberFormat::ELimit) 0);
   fVframeParaEntries->AddFrame(fNumberEntryTau_ap2, fLayout1);   
   fNumberEntryPx = new TGNumberEntry(fVframeParaEntries,18.2,6,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 2,0,100);
   fVframeParaEntries->AddFrame(fNumberEntryPx, fLayout1);
   fNumberEntryENF = new TGNumberEntry(fVframeParaEntries,0.1,6,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 0);
   fVframeParaEntries->AddFrame(fNumberEntryENF, fLayout1);
   fNumberEntryEN = new TGNumberEntry(fVframeParaEntries,0.9,6,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 0);
   fVframeParaEntries->AddFrame(fNumberEntryEN, fLayout1);
   fNumberEntryNpx = new TGNumberEntry(fVframeParaEntries,10,6,-1,(TGNumberFormat::EStyle) 0,(TGNumberFormat::EAttribute) 2,(TGNumberFormat::ELimit) 2,0,100);
   fVframeParaEntries->AddFrame(fNumberEntryNpx, fLayout1);
   fNumberEntryNpy = new TGNumberEntry(fVframeParaEntries,10,6,-1,(TGNumberFormat::EStyle) 0,(TGNumberFormat::EAttribute) 2,(TGNumberFormat::ELimit) 2,0,100);
   fVframeParaEntries->AddFrame(fNumberEntryNpy, fLayout1);
   fNumberEntryTau_rec = new TGNumberEntry(fVframeParaEntries,41.9,6,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 2,(TGNumberFormat::ELimit) 0);
   fVframeParaEntries->AddFrame(fNumberEntryTau_rec, fLayout1);
   fNumberEntryTau_le = new TGNumberEntry(fVframeParaEntries,1,6,-1,(TGNumberFormat::EStyle) 2,(TGNumberFormat::EAttribute) 2,(TGNumberFormat::ELimit) 0);
   fVframeParaEntries->AddFrame(fNumberEntryTau_le, fLayout1);
   fNumberEntryTau_te = new TGNumberEntry(fVframeParaEntries,40,6,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 2,(TGNumberFormat::ELimit) 0);
   fVframeParaEntries->AddFrame(fNumberEntryTau_te, fLayout1);

//Labels

   fVframeParaLabels = new TGVerticalFrame(fGframeParameters, 400, 400);		//Parameter Labels
   fGframeParameters->AddFrame(fVframeParaLabels, new TGLayoutHints(kLHintsRight | kLHintsTop, 5, 0, 8, 0));

   fLabelPDE = new TGLabel(fVframeParaLabels,"PDE [%]");
   fVframeParaLabels->AddFrame(fLabelPDE, fLayout2);
   fLabelGain = new TGLabel(fVframeParaLabels,"Gain [QDC-Channels]");
   fVframeParaLabels->AddFrame(fLabelGain, fLayout2);
   fLabelTau_dr = new TGLabel(fVframeParaLabels,"Dark-rate time constant [ns]");
   fVframeParaLabels->AddFrame(fLabelTau_dr, fLayout2);
   fLabelPap1 = new TGLabel(fVframeParaLabels,"After-pulse prob. (slow) [%]");
   fVframeParaLabels->AddFrame(fLabelPap1, fLayout2);
   fLabelTau_ap1 = new TGLabel(fVframeParaLabels,"After-pulse time constant (slow) [ns]");
   fVframeParaLabels->AddFrame(fLabelTau_ap1, fLayout2);
   fLabelPap2 = new TGLabel(fVframeParaLabels,"After-pulse prob. (fast) [%]");
   fVframeParaLabels->AddFrame(fLabelPap2, fLayout2);
   fLabelTau_ap2 = new TGLabel(fVframeParaLabels,"After-pulse time constant (fast) [ns]");
   fVframeParaLabels->AddFrame(fLabelTau_ap2, fLayout2);
   fLabelPx = new TGLabel(fVframeParaLabels,"Cross-talk prob. [%]");
   fVframeParaLabels->AddFrame(fLabelPx, fLayout2);
   fLabelENF = new TGLabel(fVframeParaLabels,"Excess noise [QDC-Channles]");
   fVframeParaLabels->AddFrame(fLabelENF, fLayout2);
   fLabelEN = new TGLabel(fVframeParaLabels,"Electronic noise [QDC-Channles]");
   fVframeParaLabels->AddFrame(fLabelEN, fLayout2);
   fLabelNpx = new TGLabel(fVframeParaLabels,"Number of pixels in x");
   fVframeParaLabels->AddFrame(fLabelNpx, fLayout2);
   fLabelNpy = new TGLabel(fVframeParaLabels,"Number of pixels in y");
   fVframeParaLabels->AddFrame(fLabelNpy, fLayout2);
   fLabelTau_rec = new TGLabel(fVframeParaLabels,"Recovery time [ns]");
   fVframeParaLabels->AddFrame(fLabelTau_rec, fLayout2);
   fLabelTau_le = new TGLabel(fVframeParaLabels,"Leading edge time constant [ns]");
   fVframeParaLabels->AddFrame(fLabelTau_le, fLayout2);
   fLabelTau_te = new TGLabel(fVframeParaLabels,"Trailing edge time constant [ns]");
   fVframeParaLabels->AddFrame(fLabelTau_te, fLayout2);


//Measurement

   fGframeMeasurement = new TGGroupFrame(fHframeSettings, "Measurement", kVerticalFrame);	//Measurement
   fHframeSettings->AddFrame(fGframeMeasurement, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
      
/*  //Combo Box
   fComboBoxSelect = new TGComboBox(fVframeMeasSelect,-1,
					kHorizontalFrame | kSunkenFrame | kDoubleBorder | kOwnBackground);
   fVframeMeasSelect->AddFrame(fComboBoxSelect, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,5,2));
   
   fComboBoxSelect->AddEntry("QDC Spectrum",1);
   fComboBoxSelect->AddEntry("Time Spectrum",2);
   fComboBoxSelect->AddEntry("Threshold Scan",3);
   fComboBoxSelect->AddEntry("Oscilloscope",4);
   fComboBoxSelect->AddEntry("Detected Photons",5);
   fComboBoxSelect->AddEntry("Dark-rate Events",6);
   fComboBoxSelect->AddEntry("Cross-talk Events",7);
   fComboBoxSelect->AddEntry("After-pulse Events",8);
   fComboBoxSelect->AddEntry("C-t & A-p Events",9);
   //fComboBoxSelect->AddEntry("SimRMS",10);

   fComboBoxSelect->Resize(120,25);
   fComboBoxSelect->Select(1);
*/

   Mselect = new TGButtonGroup(fGframeMeasurement,"Select Measurement");
   new TGRadioButton(Mselect, "QDC Spectrum");
   new TGRadioButton(Mselect, "Time Spectrum");
   new TGRadioButton(Mselect, "Threshold Scan");
   new TGRadioButton(Mselect, "Oscilloscope");
   new TGRadioButton(Mselect, "Detected Photons");
   new TGRadioButton(Mselect, "Dark-rate Events");
   new TGRadioButton(Mselect, "Cross-talk Events");
   new TGRadioButton(Mselect, "After-pulse Events");
   new TGRadioButton(Mselect, "C-t & A-p Events");
   new TGRadioButton(Mselect, "Response");
   new TGRadioButton(Mselect, "Resolution");
   Mselect->SetButton(kTextLeft);
   Mselect->Connect("Pressed(Int_t)", "SiPM_GUI", this, "EnableEntries( int )");
   fGframeMeasurement->AddFrame(Mselect,new TGLayoutHints(kLHintsLeft, 0, 0, 5, 0));
   measurement=1;

//Measurement Parameters

   fLabelNentries = new TGLabel(fGframeMeasurement,"Number of entries:");
   fGframeMeasurement->AddFrame(fLabelNentries,  new TGLayoutHints(kLHintsLeft, 0, 0, 0, 0));
   fNumberEntryNentries = new TGNumberEntry(fGframeMeasurement,10000,6,-1,(TGNumberFormat::EStyle) 0,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 0);
   fGframeMeasurement->AddFrame(fNumberEntryNentries, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 5));
   fLabelGate = new TGLabel(fGframeMeasurement,"Integration gate [ns]:   ");
   fGframeMeasurement->AddFrame(fLabelGate, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 0));   
   fNumberEntryGate = new TGNumberEntry(fGframeMeasurement,3e2,6,-1,(TGNumberFormat::EStyle) 5,(TGNumberFormat::EAttribute) 2,(TGNumberFormat::ELimit) 2,0,1e9);
   fGframeMeasurement->AddFrame(fNumberEntryGate, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 5));
   fLabelPedestal = new TGLabel(fGframeMeasurement,"Pedestal position [QDC-Channles]:");
   fGframeMeasurement->AddFrame(fLabelPedestal, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 0));   
   fNumberEntryPedestal = new TGNumberEntry(fGframeMeasurement,20,6,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 2,0,1023);
   fGframeMeasurement->AddFrame(fNumberEntryPedestal, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 5));
   fLabelNgamma = new TGLabel(fGframeMeasurement,"Number of incident photons:");
   fGframeMeasurement->AddFrame(fLabelNgamma, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 0));  
   fNumberEntryNgamma = new TGNumberEntry(fGframeMeasurement,10,6,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 0);
   fGframeMeasurement->AddFrame(fNumberEntryNgamma, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 5));
   fLabelLEDwidth = new TGLabel(fGframeMeasurement,"Width of light pulse [ns]:");
   fGframeMeasurement->AddFrame(fLabelLEDwidth, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 0));
   fNumberEntryLEDwidth = new TGNumberEntry(fGframeMeasurement,4,6,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 0);
   fGframeMeasurement->AddFrame(fNumberEntryLEDwidth, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 5));
   fLabelLEDspot = new TGLabel(fGframeMeasurement,"Diameter of light spot [mm]:");
   fGframeMeasurement->AddFrame(fLabelLEDspot, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 0));
   fNumberEntryLEDspot = new TGNumberEntry(fGframeMeasurement,0,6,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 2,0,1.5);
   fGframeMeasurement->AddFrame(fNumberEntryLEDspot, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 5));

//Start Frame
    fGframeStart = new TGGroupFrame(fMainFrame, "Version 1.1", kVerticalFrame);
    fMainFrame->AddFrame(fGframeStart, new TGLayoutHints(kLHintsTop | kLHintsCenterX, 0, 0, 0, 0));
   
//Run Button
    fButtonStart = new TGTextButton(fGframeStart,"&Run");
    fGframeStart->AddFrame(fButtonStart, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 2, 2, 10, 2));
    fButtonStart->Connect("Clicked()", "SiPM_GUI", this, "RunMeasurement()");
    
//Cancel Button
    fButtonCancel = new TGTextButton(fGframeStart,"&Cancel");
    fGframeStart->AddFrame(fButtonCancel, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 2, 2, 10, 2));
    fButtonCancel->Connect("Clicked()", "SiPM_GUI", this, "CancelMeasurement()");
   
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
    sipm->Connect("Progress(int)", "SiPM_GUI", this, "SetProgress(int)");


//

    fMainFrame->SetWindowName("SiPM MC - Version 1.1");
    fMainFrame->SetIconName("SiPM MC - Version 1.1");

    fMainFrame->SetMWMHints(kMWMDecorAll, kMWMFuncAll, kMWMInputModeless);
    fMainFrame->MapSubwindows();
    fMainFrame->Resize(fMainFrame->GetDefaultSize());
    fMainFrame->MapWindow();

}


void SiPM_GUI::SetProgress(int p){
  	fHProgressBar->SetPosition(p);
	gSystem->ProcessEvents();
}


void SiPM_GUI::EnableEntries(int meas){

	  if(meas==1){
	    measurement=1;
	    fNumberEntryGate->SetState(true);
	    fNumberEntryNentries->SetState(true);
	    fNumberEntryPedestal->SetState(true);
	    fNumberEntryNgamma->SetState(true);
	    fNumberEntryLEDwidth->SetState(true);
	    fNumberEntryLEDspot->SetState(true);
	    fNumberEntryNentries->SetNumber(10000);
	  }
	  if(meas==2){
	    measurement=2;
	    fNumberEntryGate->SetState(false);
	    fNumberEntryNentries->SetState(true);
	    fNumberEntryPedestal->SetState(false);
	    fNumberEntryNgamma->SetState(false);
	    fNumberEntryLEDwidth->SetState(false);
	    fNumberEntryLEDspot->SetState(false);
	    fNumberEntryNentries->SetNumber(100000);
	  }
	  if(meas==3){
	    measurement=3;
	    fNumberEntryGate->SetState(true);
	    fNumberEntryNentries->SetState(false);
	    fNumberEntryPedestal->SetState(false);
	    fNumberEntryNgamma->SetState(false);
	    fNumberEntryLEDwidth->SetState(false);
	    fNumberEntryLEDspot->SetState(false);
	    fLabelGate->SetText("Integration gate [mu s]:");
	  }
	  if(meas!=3) fLabelGate->SetText("Integration gate [ns]:   ");
	  
	  if(meas==4){
	    measurement=4;
	    fNumberEntryGate->SetState(true);
	    fNumberEntryNentries->SetState(true);
	    fNumberEntryPedestal->SetState(false);
	    fNumberEntryNgamma->SetState(true);
	    fNumberEntryLEDwidth->SetState(true);
	    fNumberEntryLEDspot->SetState(true);
	    fNumberEntryNentries->SetNumber(1);
	  }
	  	  
	  if(meas==5){
	    measurement=5; 
	    fNumberEntryGate->SetState(true);
	    fNumberEntryNentries->SetState(true);
	    fNumberEntryPedestal->SetState(false);
	    fNumberEntryNgamma->SetState(true);
	    fNumberEntryLEDwidth->SetState(true);
	    fNumberEntryLEDspot->SetState(true);
	    fNumberEntryNentries->SetNumber(10000);
	  }
	  if(meas==6){
	    measurement=6;
	    fNumberEntryGate->SetState(true);
	    fNumberEntryNentries->SetState(true);
	    fNumberEntryPedestal->SetState(false);
	    fNumberEntryNgamma->SetState(false);
	    fNumberEntryLEDwidth->SetState(false);
	    fNumberEntryLEDspot->SetState(false);
	    fNumberEntryNentries->SetNumber(10000);
	  }
	  if(meas==7){
	    measurement=7;
	    fNumberEntryGate->SetState(true);
	    fNumberEntryNentries->SetState(true);
	    fNumberEntryPedestal->SetState(false);
	    fNumberEntryNgamma->SetState(false);
	    fNumberEntryLEDwidth->SetState(false);
	    fNumberEntryLEDspot->SetState(false);
	    fNumberEntryNentries->SetNumber(10000);
	  } 
	  if(meas==8){
	    measurement=8;
	    fNumberEntryGate->SetState(true);
	    fNumberEntryNentries->SetState(true);
	    fNumberEntryPedestal->SetState(false);
	    fNumberEntryNgamma->SetState(false);
	    fNumberEntryLEDwidth->SetState(false);
	    fNumberEntryLEDspot->SetState(false);
	    fNumberEntryNentries->SetNumber(10000);
	  }
	  if(meas==9){
	    measurement=9;
	    fNumberEntryGate->SetState(true);
	    fNumberEntryNentries->SetState(true);
	    fNumberEntryPedestal->SetState(false);
	    fNumberEntryNgamma->SetState(false);
	    fNumberEntryLEDwidth->SetState(false);
	    fNumberEntryLEDspot->SetState(false);
	    fNumberEntryNentries->SetNumber(10000);
	  }
	  if(meas==10){
	    measurement=10;
	    fNumberEntryGate->SetState(true);
	    fNumberEntryNentries->SetState(true);
	    fNumberEntryPedestal->SetState(false);
	    fNumberEntryNgamma->SetState(false);
	    fNumberEntryLEDwidth->SetState(true);
	    fNumberEntryLEDspot->SetState(true);
	    fNumberEntryNentries->SetNumber(100);
	  }  
	  if(meas==11){
	    measurement=11;
	    fNumberEntryGate->SetState(true);
	    fNumberEntryNentries->SetState(true);
	    fNumberEntryPedestal->SetState(false);
	    fNumberEntryNgamma->SetState(false);
	    fNumberEntryLEDwidth->SetState(true);
	    fNumberEntryLEDspot->SetState(true);
	    fNumberEntryNentries->SetNumber(100);
	  }  
	  
}


void SiPM_GUI::RunMeasurement(){

	cout << "Starting Measurement..." << endl;
	sipm->cancel=false;
	SetParameters();
	c_main->cd(); 
	
	if(measurement==1) { sipm->QDCSpectrum(fNumberEntryNentries->GetNumber())->Draw(); c_main->SetLogy(0); c_main->SetLogx(0);}
        //if(measurement==1) { sipm->ScanCT(fNumberEntryNentries->GetNumber()); c_main->SetLogy(0); c_main->SetLogx(0);}
	if(measurement==2) { sipm->TimeSpectrum(fNumberEntryNentries->GetNumber()); c_main->SetLogy(); c_main->SetLogx(); }
	if(measurement==3) { sipm->ThreshScan(fNumberEntryGate->GetNumber()*1000)->Draw("ALP"); c_main->SetLogy(); c_main->SetLogx(0); }
	if(measurement==4) {
	  c_main->Clear();
	  TMultiGraph *mg = new TMultiGraph();
	  mg->SetName("mg");
	  for(int i=0;i<fNumberEntryNentries->GetNumber();i++){
	    sipm->GeneratePhotons("rndm",true);
	    sipm->Generate(1);
	    mg->Add(sipm->Scope(0.1));
	  }
	  mg->Draw("ALP");
	  c_main->SetLogy(0); c_main->SetLogx(0);
	  sipm->matrix->DrawMatrix(-1);
	}
	if(measurement==5) { sipm->Histo(fNumberEntryNentries->GetNumber(),"PDE")->Draw(); c_main->SetLogy(0); c_main->SetLogx(0); }
	if(measurement==6) { sipm->Histo(fNumberEntryNentries->GetNumber(),"DR")->Draw(); c_main->SetLogy(0); c_main->SetLogx(0); }
	if(measurement==7) { sipm->Histo(fNumberEntryNentries->GetNumber(),"CT")->Draw(); c_main->SetLogy(0); c_main->SetLogx(0); }
	if(measurement==8) { sipm->Histo(fNumberEntryNentries->GetNumber(),"AP")->Draw(); c_main->SetLogy(0); c_main->SetLogx(0); }
	if(measurement==9) { sipm->Histo(fNumberEntryNentries->GetNumber(),"CTAP")->Draw(); c_main->SetLogy(0); c_main->SetLogx(0); }
	if(measurement==10) { sipm->Response(fNumberEntryNentries->GetNumber())->Draw("ALP"); c_main->SetLogy(0); c_main->SetLogx(0); }
	if(measurement==11) { sipm->Res_vs_Ngamma(fNumberEntryNentries->GetNumber())->Draw("ALP"); c_main->SetLogy(0); c_main->SetLogx(0); }

	c_main->Update();
	fHProgressBar->Reset();

}


void SiPM_GUI::CancelMeasurement(){
  sipm->cancel=true;
  fHProgressBar->Reset();
  cout << "Measurement Canceled..." << endl;
}


void SiPM_GUI::SetParameters(){
    sipm->PDE = fNumberEntryPDE->GetNumber()/100.0;
    sipm->tau_dr = fNumberEntryTau_dr->GetNumber();
    sipm->Pas = fNumberEntryPap1->GetNumber()/100.0;
    sipm->tau_aps = fNumberEntryTau_ap1->GetNumber();
    sipm->Paf = fNumberEntryPap2->GetNumber()/100.0;
    sipm->tau_apf = fNumberEntryTau_ap2->GetNumber();    
    sipm->Px = fNumberEntryPx->GetNumber()/100.0;
    sipm->Ngamma = fNumberEntryNgamma->GetNumber();
    sipm->Npx = fNumberEntryNpx->GetNumber();
    sipm->Npy = fNumberEntryNpy->GetNumber();
    sipm->Npix = fNumberEntryNpx->GetNumber() * fNumberEntryNpy->GetNumber();
    sipm->gate = fNumberEntryGate->GetNumber();
    sipm->LED_width = fNumberEntryLEDwidth->GetNumber();
    sipm->recovery = fNumberEntryTau_rec->GetNumber();
    sipm->tau1 = fNumberEntryTau_le->GetNumber();
    sipm->tau2 = fNumberEntryTau_te->GetNumber();
    sipm->G = fNumberEntryGain->GetNumber();
    sipm->ENF = fNumberEntryENF->GetNumber();
    sipm->EN = fNumberEntryEN->GetNumber();
    sipm->pedestal = fNumberEntryPedestal->GetNumber();
    sipm->d_spot = fNumberEntryLEDspot->GetNumber()*1000;
}


SiPM_GUI::~SiPM_GUI(){
  delete sipm, c_main,
	fButtonStart, fButtonExit, fButtonCancel,
	fNumberEntryPDE, fNumberEntryTau_dr, fNumberEntryPap1, fNumberEntryTau_ap1, fNumberEntryPap2, fNumberEntryTau_ap2,
	fNumberEntryPx, fNumberEntryNgamma, fNumberEntryNpx, fNumberEntryNpy,
	fNumberEntryGate, fNumberEntryTau_rec, fNumberEntryTau_le, fNumberEntryTau_te,
	fNumberEntryNentries, fNumberEntryGain, fNumberEntryLEDwidth, fNumberEntryLEDspot, fNumberEntryENF,
	fNumberEntryEN, fNumberEntryPedestal,
	fLabelPDE, fLabelTau_dr, fLabelPap1, fLabelTau_ap1, fLabelPap2, fLabelTau_ap2,
	fLabelPx, fLabelNgamma, fLabelNpx, fLabelNpy,
	fLabelGate, fLabelTau_rec, fLabelTau_le, fLabelTau_te, fLabelPedestal,
	fLabelNentries, fLabelGain, fLabelLEDwidth, fLabelLEDspot, fLabelENF, fLabelEN,
	fGframeMeasurement, fGframeParameters, fGframeStart,
	fHframeSettings, fVframeParaEntries, fVframeParaLabels, fVframeMeasSelect, fHframeMeasPara, fVframeMeasParaLabels,
	fVframeMeasParaEntries, fLayout1, fLayout2, fComboBoxSelect, fHProgressBar, Mselect;
  gApplication->Terminate();
}


//------------------------------------
int main(){

    gROOT->Macro("$(HOME)/RootStyle/ilcStylePatrick.C");
  
    TApplication app("SiPM_MC", 0, 0);
    SiPM_GUI mainWin;
    app.Run();

    return 0;
}
//------------------------------------

