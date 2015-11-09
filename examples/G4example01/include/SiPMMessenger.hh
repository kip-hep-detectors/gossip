
#ifndef SiPMMessenger_h
#define SiPMMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include "SiPM.hh"

class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class SiPM;

class SiPMMessenger: public G4UImessenger
{
	public:
		SiPMMessenger(SiPM*);
		~SiPMMessenger();

		void SetNewValue(G4UIcommand*, G4String);

	private:
		SiPM* mySiPM;

		G4UIdirectory* Dir;
		G4UIdirectory* detDir;

		G4UIcmdWith3VectorAndUnit* sizeCmd;

		G4UIcmdWithADouble* pdeCmd;
		G4UIcmdWithADouble* gainCmd;
		G4UIcmdWithADouble* tdrCmd;
		G4UIcmdWithADouble* apsCmd;
		G4UIcmdWithADouble* tapsCmd;
		G4UIcmdWithADouble* apfCmd;
		G4UIcmdWithADouble* tapfCmd;
		G4UIcmdWithADouble* xtCmd;
		G4UIcmdWithADouble* enfCmd;
		G4UIcmdWithADouble* enCmd;
		G4UIcmdWithADouble* npixCmd;
		G4UIcmdWithADouble* recoveryCmd;
		G4UIcmdWith3Vector* shapeCmd;
		G4UIcmdWithADouble* signalAmplitudeCmd;
		G4UIcmdWithADouble* signalNoiseCmd;
		G4UIcmdWithADouble* gateCmd;
		G4UIcmdWithADouble* pregateCmd;
		G4UIcmdWithABool* writePhotonsCmd;
		G4UIcmdWithABool* writeGossipCmd;
		G4UIcmdWithAString* filePhotonsCmd;
		G4UIcmdWithAString* fileGossipCmd;
		G4UIcmdWithAString* parafileCmd;
};

#endif

