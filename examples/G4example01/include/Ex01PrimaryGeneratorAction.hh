// 
// --------------------------------------------------------------
//      Gossip GEANT 4 example - Ex01
// --------------------------------------------------------------

#ifndef Ex01PrimaryGeneratorAction_h
#define Ex01PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4GeneralParticleSource;
class G4Event;

class Ex01PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
	public:
		Ex01PrimaryGeneratorAction();
		~Ex01PrimaryGeneratorAction();

		void GeneratePrimaries(G4Event* anEvent);

	private:
		G4GeneralParticleSource *gps;
};

#endif

