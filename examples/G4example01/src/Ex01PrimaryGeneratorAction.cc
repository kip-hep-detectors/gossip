// 
// --------------------------------------------------------------
//      Gossip GEANT 4 example - Ex01
// --------------------------------------------------------------

#include "Ex01PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"

using namespace CLHEP;


Ex01PrimaryGeneratorAction::Ex01PrimaryGeneratorAction()
{
	gps = new G4GeneralParticleSource();
}

Ex01PrimaryGeneratorAction::~Ex01PrimaryGeneratorAction()
{
	delete gps;
}

void Ex01PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	gps->GeneratePrimaryVertex(anEvent);
}

