
#ifndef SiPM_h
#define SiPM_h 1

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"

#include "SiPMSD.hh"
#include "SiPMMessenger.hh"

#include "sipmMC.h"

class SiPMMessenger;

class SiPM
{
	public:

		SiPM(G4RotationMatrix *pRot,
				const G4ThreeVector &tlate,
				G4VPhysicalVolume *pMother,
				G4bool pMany,
				G4int pCopyNo,
				G4bool pSurfChk);

		~SiPM();

		void			SetTranslation(G4ThreeVector translate);
		void			SetDimensions(G4double sipmX, G4double sipmY, G4double sipmZ);
		void			SetDimensions(G4ThreeVector dimensions);
		G4ThreeVector		GetDimensions();
		sipmMC*		GetGossip(){ return gossip; };
		SiPMSD*		GetSD(){ return sipmSD; };

	private:

		void                  BuildSurface();
		void			BuildSi();
		void			BuildSiO2();
		void			GossipSetup();

		G4double		coatingWidth;

		///sipm
		G4Box		*sipm_box;
		G4LogicalVolume	*sipm_log;
		G4PVPlacement	*sipm_phys;

		///coating
		G4Box		*coating_box;
		G4LogicalVolume	*coating_log;
		G4PVPlacement	*coating_phys;

		///materials
		G4Material		*Si;
		G4Material		*SiO2;

		///sensitive detector
		SiPMSD		*sipmSD;

		///gossip simluation
		sipmMC		*gossip;
		TGraph		*g_spect;	///spectral sensitivity

		///messenger
		SiPMMessenger		*sipmMessenger;
};

#endif

