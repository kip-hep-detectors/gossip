//
// --------------------------------------------------------------
//      Gossip GEANT 4 example - Ex01
// --------------------------------------------------------------

#include "Ex01DetectorConstruction.hh"
#include "SiPMSD.hh"
#include "SiPM.hh"
#include "Ex01ScintillatorSD.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"

using namespace CLHEP;


Ex01DetectorConstruction::Ex01DetectorConstruction()
{
}

Ex01DetectorConstruction::~Ex01DetectorConstruction()
{
}

G4VPhysicalVolume* Ex01DetectorConstruction::Construct()
{
	G4cout << ">>>>> Construct Detector..." << G4endl;

	///------------------------------------------------------ materials
	G4cout << ">>> Materials..." << G4endl;

	G4NistManager* NistMan = G4NistManager::Instance();

	G4Material* Air = NistMan->FindOrBuildMaterial("G4_AIR");

	G4Material* LYSO = new G4Material("LYSO", 7.4*g/cm3, 4);

	G4Element* eLu = NistMan->FindOrBuildElement("Lu");	//Lu
	G4Element* eSi = NistMan->FindOrBuildElement("Si");	//Si
	G4Element* eO = NistMan->FindOrBuildElement("O");	//O
	G4Element* eY = NistMan->FindOrBuildElement("Y");	//Y

	LYSO->AddElement(eLu,71*perCent);
	LYSO->AddElement(eSi,7*perCent);
	LYSO->AddElement(eO,18*perCent);
	LYSO->AddElement(eY,4*perCent);


	///------------------------------------------------------ LYSO optical properties
	G4cout << ">>> LYSO Optical Parameters..." << G4endl;

	// Optical properties of LYSO
	const G4int nLYSO1 = 17;
	G4double FAST_Energy_LYSO[nLYSO1] = {1.0*eV,1.77*eV,2.07*eV,2.14*eV,2.21*eV,2.30*eV,2.38*eV,2.48*eV,2.58*eV,2.70*eV,2.82*eV,2.88*eV,2.95*eV,3.10*eV,3.26*eV,3.44*eV,4.08*eV};
	G4double FAST_COMPONENT_LYSO[nLYSO1] = {0.00,0.00,0.05,0.10,0.15,0.20,0.40,0.60,0.80,1.40,2.20,2.20,2.00,0.60,0.00,0.00,0.00};

	const G4int nLYSO2 = 2;
	G4double RIND_Energy_LYSO[nLYSO2] = {1.0*eV, 6.0*eV};
	G4double RIND_INDEX_LYSO[nLYSO2] = {1.82, 1.82};
	G4double ABS_Energy_LYSO[nLYSO2] = {1.0*eV, 6.0*eV};
	G4double ABS_LENGTH_LYSO[nLYSO2] = {5.*m, 5.*m};

	G4MaterialPropertiesTable* LYSO_mt = new G4MaterialPropertiesTable();
	LYSO_mt->AddProperty("FASTCOMPONENT", FAST_Energy_LYSO, FAST_COMPONENT_LYSO, nLYSO1);
	LYSO_mt->AddProperty("RINDEX", RIND_Energy_LYSO, RIND_INDEX_LYSO, nLYSO2);
	LYSO_mt->AddProperty("ABSLENGTH", ABS_Energy_LYSO, ABS_LENGTH_LYSO, nLYSO2);
	LYSO_mt->AddConstProperty("SCINTILLATIONYIELD", 20./keV);
	LYSO_mt->AddConstProperty("RESOLUTIONSCALE", 1.0);
	LYSO_mt->AddConstProperty("FASTTIMECONSTANT", 40.*ns);
	LYSO_mt->AddConstProperty("YIELDRATIO", 1.0);
	LYSO_mt->AddConstProperty("FASTSCINTILLATIONRISETIME", 0.5*ns);
	LYSO->SetMaterialPropertiesTable(LYSO_mt);


	///------------------------------------------------------ volumes
	G4cout << ">>> Volumes..." << G4endl;

	//------------------------------ world volume
	//------------------------------ beam along z axis
	G4cout << "> World..." << G4endl;

	G4double world_x = 10.*cm;
	G4double world_y = 10.*cm;
	G4double world_z = 10.*cm;
	G4Box* world_box = new G4Box("world_box", world_x, world_y, world_z);
	world_log = new G4LogicalVolume(world_box, Air, "world_log", 0, 0, 0);
	world_phys = new G4PVPlacement(0, G4ThreeVector(), world_log, "world", 0, false, 0);

	//------------------------------ scintillator
	G4cout << "> Scintillator..." << G4endl;

	G4double scintillator_x = 3.*mm;
	G4double scintillator_y = 3.*mm;
	G4double scintillator_z = 15.*mm;

	G4Box* scintillator_box = new G4Box("scintillator_box", scintillator_x/2., scintillator_y/2., scintillator_z/2.);
	scintillator_log = new G4LogicalVolume(scintillator_box, LYSO, "scintillator_log", 0, 0, 0);
	scintillator_phys = new G4PVPlacement(0,G4ThreeVector(), scintillator_log, "scintillator", world_log, false, 0);

	//------------------------------ sipm
	G4cout << "> SiPM..." << G4endl;

	G4double sipm_x = 3.*mm;	///3x3 mm2 sipm
	G4double sipm_y = 3.*mm;	///3x3 mm2 sipm
	G4double sipm_z = 500.*um;	///total sipm thickness (400um coating (fixed in sipm class) + 100um silicon)

	G4double sipm_pos_z = -scintillator_z/2. - sipm_z/2.;

	G4cout << 1 << G4endl;

	sipm = new SiPM(0, G4ThreeVector(0,0,0), world_phys, false, 0, false);	///build sipm
	G4cout << 1 << G4endl;
	sipm->SetDimensions(sipm_x, sipm_y, sipm_z);				///set dimensions to 3x3 mm2
	G4cout << 1 << G4endl;
	sipm->SetTranslation(G4ThreeVector(0,0,sipm_pos_z));			///move sipm to end of scintillator


	///------------------------------ Set sensitive detectors
	G4cout << ">>> Construct Sensitive Detectors..." << G4endl;

	G4SDManager* SDman = G4SDManager::GetSDMpointer();

	Ex01ScintillatorSD* scintSD = new Ex01ScintillatorSD("scintSD");
	SDman->AddNewDetector(scintSD);
	scintillator_log->SetSensitiveDetector(scintSD);


	///------------------------------ Build scintillator surface (Teflon wrapping)

	G4OpticalSurface* OpSurface = new G4OpticalSurface("scintillatorSurface");
	OpSurface->SetType(dielectric_dielectric);

	OpSurface->SetFinish(polishedfrontpainted);
	OpSurface->SetModel(unified);

	OpSurface->SetSigmaAlpha(1*deg);

	const G4int nEntries = 2;
	G4double PhotonEnergy[nEntries]    = {1.*eV, 6.*eV};

	G4double RefractiveIndex[nEntries] = {1, 1};
	G4double SpecularLobe[nEntries]    = {1, 1};
	G4double SpecularSpike[nEntries]   = {0, 0};
	G4double Backscatter[nEntries]     = {0, 0};
	G4double Lambertian[nEntries]      = {0, 0};
	G4double Reflectivity[nEntries]    = {0.98, 0.98};

	G4MaterialPropertiesTable* myMPT = new G4MaterialPropertiesTable();

	myMPT->AddProperty("RINDEX",                PhotonEnergy, RefractiveIndex, nEntries);
	myMPT->AddProperty("SPECULARLOBECONSTANT",  PhotonEnergy, SpecularLobe,    nEntries);
	myMPT->AddProperty("SPECULARSPIKECONSTANT", PhotonEnergy, SpecularSpike,   nEntries);
	myMPT->AddProperty("BACKSCAERCONSTANT",     PhotonEnergy, Backscatter,     nEntries);
	myMPT->AddProperty("LAMBERTIAN",            PhotonEnergy, Lambertian,      nEntries);
	myMPT->AddProperty("REFLECTIVITY",          PhotonEnergy, Reflectivity,    nEntries);

	new G4LogicalBorderSurface("scintillatorSurface", scintillator_phys, world_phys, OpSurface);

	G4cout << ">>>>> Construct Detector done!" << G4endl;

	return world_phys;
}

