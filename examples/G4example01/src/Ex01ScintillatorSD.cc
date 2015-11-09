// 
// --------------------------------------------------------------
//      Gossip GEANT 4 example - Ex01
// --------------------------------------------------------------

#include "Ex01ScintillatorSD.hh"

#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4VProcess.hh"

using namespace CLHEP;

Ex01ScintillatorSD::Ex01ScintillatorSD(G4String name):G4VSensitiveDetector(name)
{
	G4String HCname;
	collectionName.insert(HCname="scintCollection");
}

Ex01ScintillatorSD::~Ex01ScintillatorSD()
{
}

void Ex01ScintillatorSD::Initialize(G4HCofThisEvent* HCE)
{
	scintCollection = new Ex01ScintillatorHitsCollection(SensitiveDetectorName,collectionName[0]);
	static G4int HCID = -1;
	if(HCID<0)
	{
		HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

	}
	HCE->AddHitsCollection( HCID, scintCollection );
}

G4bool Ex01ScintillatorSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
	///don't process optical photons
	if(aStep->GetTrack()->GetDefinition()->GetParticleName() == (G4String)"opticalphoton") return false;

	///write energy deposition to hit collection
	Ex01ScintillatorHit* newHit = new Ex01ScintillatorHit();

	newHit->SetEdep(aStep->GetTotalEnergyDeposit());
	newHit->SetPos(aStep->GetPreStepPoint()->GetPosition());

	scintCollection->insert( newHit );

	return true;
}

void Ex01ScintillatorSD::EndOfEvent(G4HCofThisEvent*)
{
}

