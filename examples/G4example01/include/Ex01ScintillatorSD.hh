// 
// --------------------------------------------------------------
//      Gossip GEANT 4 example - Ex01
// --------------------------------------------------------------

#ifndef Ex01ScintillatorSD_h
#define Ex01ScintillatorSD_h 1

#include "G4VSensitiveDetector.hh"

#include "Ex01ScintillatorHit.hh"

class G4HCofThisEvent;
class G4Step;

class Ex01ScintillatorSD : public G4VSensitiveDetector
{
	public:
		Ex01ScintillatorSD(G4String);
		~Ex01ScintillatorSD();

		void Initialize(G4HCofThisEvent*);
		G4bool ProcessHits(G4Step*, G4TouchableHistory*);
		void EndOfEvent(G4HCofThisEvent*);

	private:

		Ex01ScintillatorHitsCollection* scintCollection;
};

#endif

