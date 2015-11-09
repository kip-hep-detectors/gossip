// 
// --------------------------------------------------------------
//      Gossip GEANT 4 example - Ex01
// --------------------------------------------------------------

#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

class Ex01DetectorConstruction;
class Ex01EventAction;

class Ex01SteppingAction : public G4UserSteppingAction
{
	public:
		Ex01SteppingAction();

		void UserSteppingAction(const G4Step*);

	private:

};

#endif

