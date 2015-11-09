// 
// --------------------------------------------------------------
//      Gossip GEANT 4 example - Ex01
// --------------------------------------------------------------

#ifndef Ex01RunAction_h
#define Ex01RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "fstream"

class G4Run;
class TFile;
class TTree;

class Ex01RunAction : public G4UserRunAction
{
	public:
		Ex01RunAction();
		virtual ~Ex01RunAction();

		void BeginOfRunAction(const G4Run*);
		void EndOfRunAction(const G4Run*);

		int GetRunID(){ return runNum; };

		TTree* GetTreePhotons(){ return tree_photons; };
		TTree* GetTreeGossip(){ return tree_gossip; };
		TTree* GetTreeEdep(){ return tree_edep; };

	private:

		int runNum;

		TFile* file;
		TTree* tree_photons;
		TTree* tree_gossip;
		TTree* tree_edep;
};

#endif

