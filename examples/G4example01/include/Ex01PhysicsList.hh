// 
// --------------------------------------------------------------
//      Gossip GEANT 4 example - Ex01
// --------------------------------------------------------------

#ifndef Ex01PhysicsList_h
#define Ex01PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"


class Ex01PhysicsList: public G4VModularPhysicsList
{
	public:
		Ex01PhysicsList();
		virtual ~Ex01PhysicsList();

	public:
		virtual void SetCuts();

};

#endif

