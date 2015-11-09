
#include "SiPMMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

SiPMMessenger::SiPMMessenger(SiPM* sipm)
:mySiPM(sipm)
{
	Dir = new G4UIdirectory("/sipm/");
	Dir->SetGuidance("UI commands for SiPM.");

	detDir = new G4UIdirectory("/sipm/");
	detDir->SetGuidance("Crystal Geometry.");

	pdeCmd = new G4UIcmdWithADouble("/sipm/pde",this);
	pdeCmd->SetGuidance("Set PDE of SiPM");
	pdeCmd->SetParameterName("PDE",false);
	pdeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	gainCmd = new G4UIcmdWithADouble("/sipm/gain",this);
	gainCmd->SetGuidance("Set gain of SiPM");
	gainCmd->SetParameterName("Gain [QDC-Channels]",false);
	gainCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	tdrCmd = new G4UIcmdWithADouble("/sipm/tDR",this);
	tdrCmd->SetGuidance("Set dark-rate time constant of SiPM");
	tdrCmd->SetParameterName("tDR [ns]",false);
	tdrCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	apsCmd = new G4UIcmdWithADouble("/sipm/pAP_slow",this);
	apsCmd->SetGuidance("Set slow after-pulse probability of SiPM");
	apsCmd->SetParameterName("pAP_slow",false);
	apsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	tapsCmd = new G4UIcmdWithADouble("/sipm/tAP_slow",this);
	tapsCmd->SetGuidance("Set slow after-pulse time constant of SiPM");
	tapsCmd->SetParameterName("tAP_slow [ns]",false);
	tapsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	apfCmd = new G4UIcmdWithADouble("/sipm/pAP_fast",this);
	apfCmd->SetGuidance("Set fast after-pulse probability of SiPM");
	apfCmd->SetParameterName("pAP_fast",false);
	apfCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	tapfCmd = new G4UIcmdWithADouble("/sipm/tAP_fast",this);
	tapfCmd->SetGuidance("Set slow after-pulse time constant of SiPM");
	tapfCmd->SetParameterName("tAP_fast [ns]",false);
	tapfCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	xtCmd = new G4UIcmdWithADouble("/sipm/pCT",this);
	xtCmd->SetGuidance("Set cross-talk probability of SiPM");
	xtCmd->SetParameterName("Pxt",false);
	xtCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	enfCmd = new G4UIcmdWithADouble("/sipm/enf",this);
	enfCmd->SetGuidance("Set excess noise of SiPM");
	enfCmd->SetParameterName("ENF [QDC-Channels]",false);
	enfCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	enCmd = new G4UIcmdWithADouble("/sipm/en",this);
	enCmd->SetGuidance("Set electronic noise of SiPM");
	enCmd->SetParameterName("EN [QDC-Channels]",false);
	enCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	npixCmd = new G4UIcmdWithADouble("/sipm/nPix",this);
	npixCmd->SetGuidance("Set number of pixels of SiPM");
	npixCmd->SetParameterName("Npix",false);
	npixCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	recoveryCmd = new G4UIcmdWithADouble("/sipm/tRecovery",this);
	recoveryCmd->SetGuidance("Set recovery time of SiPM");
	recoveryCmd->SetParameterName("tRecovery [ns]",false);
	recoveryCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	shapeCmd = new G4UIcmdWith3Vector("/sipm/pulseShape",this);
	shapeCmd->SetGuidance("Set rise time, fall time and sampling of single pixel signal");
	shapeCmd->SetParameterName("tau1 [ns]", "tau2 [ns]", "sampling [ns]",false);
	shapeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	signalAmplitudeCmd = new G4UIcmdWithADouble("/sipm/signalAmplitude",this);
	signalAmplitudeCmd->SetGuidance("Set amplitude of single pixel signal");
	signalAmplitudeCmd->SetParameterName("Amplitude [mV]",false);
	signalAmplitudeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	signalNoiseCmd = new G4UIcmdWithADouble("/sipm/noise",this);
	signalNoiseCmd->SetGuidance("Set noise RMS");
	signalNoiseCmd->SetParameterName("noise [mV]",false);
	signalNoiseCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	gateCmd = new G4UIcmdWithADouble("/sipm/gate",this);
	gateCmd->SetGuidance("Set integration gate length");
	gateCmd->SetParameterName("Gate [ns]",false);
	gateCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	pregateCmd = new G4UIcmdWithADouble("/sipm/pregate",this);
	pregateCmd->SetGuidance("Set simulation time before measurement");
	pregateCmd->SetParameterName("Pregate [ns]",false);
	pregateCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	sizeCmd = new G4UIcmdWith3VectorAndUnit("/sipm/size",this);
	sizeCmd->SetGuidance("Set size of the sipm in x,y,z");
	sizeCmd->SetUnitCategory("Length");
	sizeCmd->SetParameterName("sizeX", "sizeY", "sizeZ",false);
	sizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	writePhotonsCmd = new G4UIcmdWithABool("/sipm/writePhotons",this);
	writePhotonsCmd->SetGuidance("Enable to out photon information");
	writePhotonsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	writeGossipCmd = new G4UIcmdWithABool("/sipm/runGossip",this);
	writeGossipCmd->SetGuidance("Enable to run GosSiP and write output");
	writeGossipCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	filePhotonsCmd = new G4UIcmdWithAString("/sipm/filenamePhotons",this);
	filePhotonsCmd->SetGuidance("Set filename for photon information");
	filePhotonsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	fileGossipCmd = new G4UIcmdWithAString("/sipm/filenameGossip",this);
	fileGossipCmd->SetGuidance("Set filename for GosSiP outpu information");
	fileGossipCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	parafileCmd = new G4UIcmdWithAString("/sipm/readParaFile",this);
	parafileCmd->SetGuidance("Reads in a SiPM parameter file");
	parafileCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

SiPMMessenger::~SiPMMessenger()
{
	delete pdeCmd;
	delete gainCmd;
	delete tdrCmd;
	delete apsCmd;
	delete tapsCmd;
	delete apfCmd;
	delete tapfCmd;
	delete xtCmd;
	delete enfCmd;
	delete enCmd;
	delete npixCmd;
	delete recoveryCmd;
	delete shapeCmd;
	delete signalAmplitudeCmd;
	delete signalNoiseCmd;
	delete gateCmd;
	delete pregateCmd;
	delete sizeCmd;
	delete detDir;
	delete Dir;
	delete writePhotonsCmd;
	delete writeGossipCmd;
	delete filePhotonsCmd;
	delete fileGossipCmd;
	delete parafileCmd;
}


void SiPMMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
	if( command == sizeCmd )
	{
		G4ThreeVector dimensions = sizeCmd->GetNew3VectorValue(newValue);
		mySiPM->SetDimensions(dimensions);

		mySiPM->GetGossip()->xSipm = dimensions.x();
		mySiPM->GetGossip()->ySipm = dimensions.y();

		G4cout << ">>>>> SiPM size is now " << G4BestUnit(dimensions.x(),"Length") << " x "
			<< G4BestUnit(dimensions.y(),"Length") << " x "
			<< G4BestUnit(dimensions.z(),"Length") << " x "
			<< "(X,Y,Z) <<<<<" << G4endl;
	}

	if( command == pdeCmd )
	{
		mySiPM->GetGossip()->PDE = pdeCmd->GetNewDoubleValue(newValue);
		G4cout << ">>>>> SiPM PDE is now " << pdeCmd->GetNewDoubleValue(newValue) << " <<<<<" << G4endl;
	}

	if( command == gainCmd )
	{
		mySiPM->GetGossip()->gain = gainCmd->GetNewDoubleValue(newValue);
		G4cout << ">>>>> SiPM gain is now " << gainCmd->GetNewDoubleValue(newValue) << " <<<<<" << G4endl;
	}

	if( command == tdrCmd )
	{
		mySiPM->GetGossip()->tau_dr = tdrCmd->GetNewDoubleValue(newValue);
		G4cout << ">>>>> SiPM tau_dr is now " << tdrCmd->GetNewDoubleValue(newValue) << " <<<<<" << G4endl;
	}

	if( command == apsCmd )
	{
		mySiPM->GetGossip()->Pap_s = apsCmd->GetNewDoubleValue(newValue);
		G4cout << ">>>>> SiPM Pap_s is now " << apsCmd->GetNewDoubleValue(newValue) << " <<<<<" << G4endl;
	}

	if( command == tapsCmd )
	{
		mySiPM->GetGossip()->tau_ap_s = tapsCmd->GetNewDoubleValue(newValue);
		G4cout << ">>>>> SiPM tau_ap_s is now " << tapsCmd->GetNewDoubleValue(newValue) << " <<<<<" << G4endl;
	}

	if( command == apfCmd )
	{
		mySiPM->GetGossip()->Pap_f = apfCmd->GetNewDoubleValue(newValue);
		G4cout << ">>>>> SiPM Pap_f is now " << apfCmd->GetNewDoubleValue(newValue) << " <<<<<" << G4endl;
	}

	if( command == tapfCmd )
	{
		mySiPM->GetGossip()->tau_ap_f = tapfCmd->GetNewDoubleValue(newValue);
		G4cout << ">>>>> SiPM tau_ap_f is now " << tapfCmd->GetNewDoubleValue(newValue) << " <<<<<" << G4endl;
	}

	if( command == xtCmd )
	{
		mySiPM->GetGossip()->Pxt = xtCmd->GetNewDoubleValue(newValue);
		G4cout << ">>>>> SiPM Pxt is now " << xtCmd->GetNewDoubleValue(newValue) << " <<<<<" << G4endl;
	}

	if( command == enfCmd )
	{
		mySiPM->GetGossip()->ENF = enfCmd->GetNewDoubleValue(newValue);
		G4cout << ">>>>> SiPM ENF is now " << enfCmd->GetNewDoubleValue(newValue) << " <<<<<" << G4endl;
	}

	if( command == enCmd )
	{
		mySiPM->GetGossip()->EN = enCmd->GetNewDoubleValue(newValue);
		G4cout << ">>>>> SiPM EN is now " << enCmd->GetNewDoubleValue(newValue) << " <<<<<" << G4endl;
	}

	if( command == npixCmd )
	{
		mySiPM->GetGossip()->NpixX = sqrt(npixCmd->GetNewDoubleValue(newValue));
		mySiPM->GetGossip()->NpixY = sqrt(npixCmd->GetNewDoubleValue(newValue));
		G4cout << ">>>>> SiPM Npx / Npy is now " << sqrt(npixCmd->GetNewDoubleValue(newValue)) << " / " << sqrt(npixCmd->GetNewDoubleValue(newValue)) << " <<<<<" << G4endl;
	}

	if( command == recoveryCmd )
	{
		mySiPM->GetGossip()->tau_recovery = recoveryCmd->GetNewDoubleValue(newValue);
		G4cout << ">>>>> SiPM tau_recovery is now " << recoveryCmd->GetNewDoubleValue(newValue) << " <<<<<" << G4endl;
	}

	if( command == shapeCmd )
	{
		G4ThreeVector value = shapeCmd->GetNew3VectorValue(newValue);
		double tau1 = value.x();
		double tau2 = value.y();
		double sampling = value.z();

		mySiPM->GetGossip()->SetPulseShape(tau1,tau2);
		mySiPM->GetGossip()->SetSampling(sampling);
		G4cout << ">>>>> SiPM pulse shape is now: tau1 = " << tau1 << ", tau2 = " << tau2 << ", sampling = " << sampling << " <<<<<" << G4endl;
	}

	if( command == signalAmplitudeCmd )
	{
		mySiPM->GetGossip()->signalAmp = signalAmplitudeCmd->GetNewDoubleValue(newValue);
		G4cout << ">>>>> SiPM signalAmp is now " << signalAmplitudeCmd->GetNewDoubleValue(newValue) << " <<<<<" << G4endl;
	}

	if( command == signalNoiseCmd )
	{
		mySiPM->GetGossip()->noiseRMS = signalNoiseCmd->GetNewDoubleValue(newValue);
		G4cout << ">>>>> SiPM noiseRMS is now " << signalNoiseCmd->GetNewDoubleValue(newValue) << " <<<<<" << G4endl;
	}

	if( command == gateCmd )
	{
		mySiPM->GetGossip()->SetGate(gateCmd->GetNewDoubleValue(newValue));
		G4cout << ">>>>> SiPM gate is now " << gateCmd->GetNewDoubleValue(newValue) << " <<<<<" << G4endl;
	}

	if( command == pregateCmd )
	{
		mySiPM->GetGossip()->SetPreGate(pregateCmd->GetNewDoubleValue(newValue));
		G4cout << ">>>>> SiPM pregate is now " << pregateCmd->GetNewDoubleValue(newValue) << " <<<<<" << G4endl;
	}

	if( command == writePhotonsCmd )
	{
		mySiPM->GetSD()->WritePhotons(writePhotonsCmd->GetNewBoolValue(newValue));
		G4cout << ">>>>> Write out photon information " << writePhotonsCmd->GetNewBoolValue(newValue) << " <<<<<" << G4endl;
	}

	if( command == writeGossipCmd )
	{
		mySiPM->GetSD()->RunGossip(writeGossipCmd->GetNewBoolValue(newValue));
		G4cout << ">>>>> Run GosSiP " << writeGossipCmd->GetNewBoolValue(newValue) << " <<<<<" << G4endl;
	}

	if( command == filePhotonsCmd )
	{
		mySiPM->GetSD()->SetFilePhotons(newValue);
		G4cout << ">>>>> Photons output file is now: " << newValue << " <<<<<" << G4endl;
	}

	if( command == fileGossipCmd )
	{
		mySiPM->GetSD()->SetFileGossip(newValue);
		G4cout << ">>>>> GosSiP output file is now: " << newValue << " <<<<<" << G4endl;
	}

	if( command == parafileCmd )
	{
		mySiPM->GetGossip()->GetParaFile(newValue.c_str());
	}
}
