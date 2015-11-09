//
// --------------------------------------------------------------
//      Gossip GEANT 4 example - Ex01
// --------------------------------------------------------------

#include "Ex01RunAction.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "TFile.h"
#include "TTree.h"

using namespace std;

Ex01RunAction::Ex01RunAction()
{
}

Ex01RunAction::~Ex01RunAction()
{
}

void Ex01RunAction::BeginOfRunAction(const G4Run* aRun)
{
	G4cout << ">>>>> Starting Run <<<<<" << G4endl;

	file = new TFile("output.root", "UPDATE");		///ROOT file for output data

	tree_photons = new TTree("photons","photons");		///output tree for photon information
	tree_gossip = new TTree("gossip","gossip");		///output tree for simulation output ( charge and waveform )
	tree_edep = new TTree("edep","edep");			///output tree the energy deposition in the scintillator

	runNum = aRun->GetRunID();
}

void Ex01RunAction::EndOfRunAction(const G4Run*)
{
	G4cout << ">>>>> Run Finished <<<<<" << G4endl;

	file->Close();
	delete file;
}
