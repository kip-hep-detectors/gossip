// 
// --------------------------------------------------------------
//      Gossip GEANT 4 example - Ex01
// --------------------------------------------------------------

#include "Ex01EventAction.hh"
#include "Ex01RunAction.hh"
#include "SiPMHit.hh"
#include "Ex01ScintillatorHit.hh"

#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

#include "TTree.h"
#include "TFile.h"

using namespace CLHEP;


Ex01EventAction::Ex01EventAction(Ex01RunAction* runAction) : runAct(runAction)
{
	sipmCollID = -1;
	scintCollID = -1;

	///Option 2:
	sipm = new sipmMC();					///create instance of sipmMC
	sipm->GetParaFile("input/MPPC_3x3.txt");		///read in sipm parameters for file MPPC_3x3.txt
	sipm->SetSampling(0.2);					///set sampling rate to 0.2ns ( = 5 GSPS )
	sipm->SetGate(500);					///set measurement window to 500 ns
	sipm->SetPreGate(500);					///set simulation time before measurement window to 500 ns

	TFile f("input/SpectralSensitivity.root");
	sipm->SetSpectralSensitivity((TGraph*)f.Get("MPPC_noRef"));
	f.Close();

	waveform = sipm->GetWaveform();
}

Ex01EventAction::~Ex01EventAction()
{
}

void Ex01EventAction::SetBranches()
{
	///Option 1:
	tree_photons = runAct->GetTreePhotons();

	tree_photons->Branch("run", &runNb, "run/I");		///branch for run number
	tree_photons->Branch("event", &eventNb, "event/I");	///branch for event number
	tree_photons->Branch("nHits", &nHits, "nHits/I");	///branch for number of photon hits
	tree_photons->Branch("posX", &posX);			///branch for photon x position on sipm
	tree_photons->Branch("posY", &posY);			///branch for photon y position on sipm
	tree_photons->Branch("time", &time);			///branch for photon arrival time on sipm
	tree_photons->Branch("wavelength", &wavelength);	///branch for photon arrival time on sipm

	///Option 2:
	tree_gossip = runAct->GetTreeGossip();

	tree_gossip->Branch("run", &runNb, "run/I");		///branch for run number
	tree_gossip->Branch("event", &eventNb, "event/I");	///branch for event number
	tree_gossip->Branch("charge", &charge, "charge/D");	///branch for sipm signal charge
	tree_gossip->Branch("waveform", &waveform);		///branch for sipm output waveform (TGraph)

	///Option 3:
	tree_edep = runAct->GetTreeEdep();

	tree_edep->Branch("run", &runNb, "run/I");		///branch for run number
	tree_edep->Branch("event", &eventNb, "event/I");	///branch for event number
	tree_edep->Branch("edep", &edep, "edep/D");		///branch for total energy deposition in scintillator
}

void Ex01EventAction::BeginOfEventAction(const G4Event*)
{
	///set branches
	SetBranches();

	///reset all variables

	///Option 1:
	posX.clear();
	posY.clear();
	time.clear();
	nHits = 0.;

	///Option 2:
	photons.clear();

	///Option 3:
	edep = 0.;
}

void Ex01EventAction::EndOfEventAction(const G4Event* evt)
{
	G4cout << "\n >>> Event " << evt->GetEventID() << G4endl;

	eventNb = evt->GetEventID();	///get the event number
	runNb = runAct->GetRunID();	///get run number from Ex01RunAction

	///get information from hit collections
	G4SDManager * SDman = G4SDManager::GetSDMpointer();
	sipmCollID = SDman->GetCollectionID("sipmCollection");
	scintCollID = SDman->GetCollectionID("scintCollection");

	if(sipmCollID<0) return;
	if(scintCollID<0) return;

	G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
	SiPMHitsCollection* sipmHC = 0;
	Ex01ScintillatorHitsCollection* scintHC = 0;

	if(HCE)
	{
		sipmHC = (SiPMHitsCollection*)(HCE->GetHC(sipmCollID));
		scintHC = (Ex01ScintillatorHitsCollection*)(HCE->GetHC(scintCollID));
	}

	///loop through sipm photon hits
	if(sipmHC)
	{
		int n = sipmHC->entries();

		for(int i=0;i<n;i++)
		{
			double x = (*sipmHC)[i]->GetPos().x()/mm;	///photon x position on sipm
			double y = (*sipmHC)[i]->GetPos().y()/mm;	///photon y position on sipm
			double t = (*sipmHC)[i]->GetTime()/ns;		///photon arrival time on sipm
			double wl = (*sipmHC)[i]->GetWavelength()/nm;	///photon wavelength

			///Option 1:
			posX.push_back(x);
			posY.push_back(y);
			time.push_back(t);
			wavelength.push_back(wl);
			nHits++;

			///Option 2:
			photons.AddPhoton(x,y,t,wl);			///add photon to the photon list
		}
	}

	///loop through scintillator hits
	if(scintHC)
	{
		int n = scintHC->entries();

		for(int i=0;i<n;i++)
		{
			///Option 3:
			edep += (*scintHC)[i]->GetEdep()/MeV;	///sum up all energy deposits
		}
	}

	///Option 2:
	sipm->Generate(photons);			///generate sipm response from photon list
	charge = sipm->GetCharge();			///get output signal charge
	waveform = sipm->GetWaveform();			///get output waveform


	///fill trees
	tree_photons->Fill();	///Option 1
	tree_gossip->Fill();	///Option 2
	tree_edep->Fill();	///Option 3

	///write trees	
	tree_photons->Write("",TObject::kOverwrite);	///Option 1
	tree_gossip->Write("",TObject::kOverwrite);	///Option 2
	tree_edep->Write("",TObject::kOverwrite);	///Option 3
}
