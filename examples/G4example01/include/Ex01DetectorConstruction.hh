// 
// --------------------------------------------------------------
//      Gossip GEANT 4 example - Ex01
// --------------------------------------------------------------

#ifndef Ex01DetectorConstruction_H
#define Ex01DetectorConstruction_H 1

#include "G4VUserDetectorConstruction.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class SiPM;

class Ex01DetectorConstruction : public G4VUserDetectorConstruction
{
	public:

		Ex01DetectorConstruction();
		~Ex01DetectorConstruction();

		G4VPhysicalVolume* Construct();

	private:

		// Logical volumes
		//
		G4LogicalVolume* world_log;
		G4LogicalVolume* scintillator_log;

		// Physical volumes
		//
		G4VPhysicalVolume* world_phys;
		G4VPhysicalVolume* scintillator_phys;

		// SiPM
		//
		SiPM* sipm;

};

#endif

