// 
// --------------------------------------------------------------
//      Gossip GEANT 4 example - 
// --------------------------------------------------------------

#ifndef SiPMSD_h
#define SiPMSD_h 1

#include "G4VSensitiveDetector.hh"
#include "TF1.h"
#include "TH1D.h"
#include "vector"
#include "fstream"

#include "SiPMHit.hh"

#include "sipmMC.h"
#include "PhotonList.h"

class G4HCofThisEvent;
class G4Step;
class TTree;
class TFile;

using std::vector;
using std::ofstream;
using std::ios;

class SiPMSD : public G4VSensitiveDetector
{
	public:
		SiPMSD(G4String, sipmMC*);
		~SiPMSD();

		void Initialize(G4HCofThisEvent*);
		G4bool ProcessHits(G4Step*, G4TouchableHistory*);
		void EndOfEvent(G4HCofThisEvent*);

		void RunGossip( bool enable ){ write_gossip = enable; };
		void WritePhotons( bool enable ){ write_photons = enable; };
		void SetFilePhotons( string file ){ filename_photons = file; };
		void SetFileGossip( string file ){ filename_gossip = file; };

	private:

		SiPMHitsCollection* sipmCollection;

		G4double h;
		G4double c;

		sipmMC* sipm;
		PhotonList photons;

		ofstream file_photons;
		ofstream file_gossip;

		unsigned int n_event;		///event number

		bool write_photons;		///enable to save photon output
		bool write_gossip;		///enable to run gossip

		string filename_photons;	///filename for photon output
		string filename_gossip;		///filename for gossip output
};

#endif
