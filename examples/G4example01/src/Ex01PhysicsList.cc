// 
// --------------------------------------------------------------
//      Gossip GEANT 4 example - Ex01
// --------------------------------------------------------------

#include "Ex01PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"

using namespace CLHEP;


Ex01PhysicsList::Ex01PhysicsList():  G4VModularPhysicsList()
{
	// default cut value  (1.0mm)
	defaultCutValue = 1.0*mm;

	// General Physics
	RegisterPhysics( new G4DecayPhysics("decay") );

	// EM Physics
	RegisterPhysics( new G4EmStandardPhysics(0,"standard EM"));

	// Optical Physics
	G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics(0,"optical");
	opticalPhysics->SetScintillationYieldFactor(0.1);	///Scale down the scintillation yield by a factor of 10 to increase simulation speed!
	RegisterPhysics( opticalPhysics );

}

Ex01PhysicsList::~Ex01PhysicsList()
{
}

void Ex01PhysicsList::SetCuts(){
	//  " G4VUserPhysicsList::SetCutsWithDefault" method sets
	//   the default cut value for all particle types
	SetCutsWithDefault();
}
