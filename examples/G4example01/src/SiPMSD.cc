// 
// --------------------------------------------------------------
//      Gossip GEANT 4 example - 
// --------------------------------------------------------------

#include "SiPMSD.hh"

#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4VProcess.hh"

#include "TTree.h"
#include "TFile.h"

using namespace CLHEP;

SiPMSD::SiPMSD(G4String name, sipmMC* gossip):G4VSensitiveDetector(name), sipm(gossip)
{
	G4String HCname;
	collectionName.insert(HCname="sipmCollection");

	n_event = 0;

	sipm->SetSampling(0.2);
	sipm->SetGate(500);
	sipm->SetPreGate(500);

	h = 4.13566733e-15*eV*s;
	c = 299792458*m/s;

	write_photons = false;				///default - don't write photon information
	write_gossip = false;				///default - don't run gossip

	filename_photons = "output_photons.bin";	///default path
	filename_gossip = "output_gossip.bin";		///default path
}

SiPMSD::~SiPMSD()
{
}

void SiPMSD::Initialize(G4HCofThisEvent* HCE)
{
	sipmCollection = new SiPMHitsCollection(SensitiveDetectorName,collectionName[0]);
	static G4int HCID = -1;
	if(HCID<0)
	{
		HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	}
	HCE->AddHitsCollection( HCID, sipmCollection );

	///write photon information
	if(write_photons)
	{
		file_photons.open( filename_photons.c_str(), ios::out | ios::app | ios::binary);
		file_photons.write((char*)&n_event,sizeof(int));	///write event number
	}

	///write gossip information
	if(write_gossip)
	{
		file_gossip.open( filename_gossip.c_str(), ios::out | ios::app | ios::binary);
		file_gossip.write((char*)&n_event,sizeof(int));	///write event number
	}

	photons.clear();
}

G4bool SiPMSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
	///only process optical photons
	if(aStep->GetTrack()->GetDefinition()->GetParticleName() != (G4String)"opticalphoton") return false;

	///only process photons which are absorpt
	if(aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()!="OpAbsorption") return false;

	///write photon position and time to hit collection
	SiPMHit* newHit = new SiPMHit();

	G4ThreeVector pos = aStep->GetPostStepPoint()->GetPosition();
	G4double time = aStep->GetPostStepPoint()->GetGlobalTime();
	G4double wavelength = h*c/aStep->GetTrack()->GetTotalEnergy();

	newHit->SetPos(pos);
	newHit->SetTime(time);
	newHit->SetWavelength(wavelength);

	sipmCollection->insert( newHit );

	///write photon to list
	photons.AddPhoton(pos.x()/mm, pos.y()/mm, time/ns, wavelength/nm);

	return true;
}

void SiPMSD::EndOfEvent(G4HCofThisEvent*)
{
	///write photon information
	if(write_photons)
	{
		unsigned int n_photons = photons.size();
		file_photons.write((char*)&n_photons,sizeof(unsigned int));	///write number of photons

		for(unsigned int i=0;i<n_photons;i++)
		{
			float x = photons.at(i).at(0);
			float y = photons.at(i).at(1);
			float t = photons.at(i).at(2);
			float wl = photons.at(i).at(3);

			file_photons.write((char*)&x,sizeof(float));	///write x position of photon
			file_photons.write((char*)&y,sizeof(float));	///write y position of photon
			file_photons.write((char*)&t,sizeof(float));	///write arrival time of photon
			file_photons.write((char*)&wl,sizeof(float));	///write wavelength of photon
		}

		file_photons.close();
	}

	///run gossip and write output data
	if(write_gossip)
	{
		sipm->Generate(photons);			///generate sipm response from photon list
		float charge = sipm->GetCharge();		///get output signal charge
		TGraph *waveform = sipm->GetWaveform();	///get output waveform

		float sampling = sipm->GetSampling();
		unsigned int n_samples = waveform->GetN();

		file_gossip.write((char*)&charge,sizeof(float));		///write output charge
		file_gossip.write((char*)&sampling,sizeof(float));		///write sampling
		file_gossip.write((char*)&n_samples,sizeof(unsigned int));	///write number of samples

		for(int i=0;i<waveform->GetN();i++)				///write amplitudes
		{
			float amplitude = waveform->GetY()[i];
			file_gossip.write((char*)&amplitude,sizeof(float));
		}

		file_gossip.close();
	}

	n_event++;
}

