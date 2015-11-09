// 
// --------------------------------------------------------------
//      Gossip GEANT 4 example - 
// --------------------------------------------------------------

#ifndef SiPMHit_h
#define SiPMHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class SiPMHit : public G4VHit
{
	public:

		SiPMHit();
		~SiPMHit();
		SiPMHit(const SiPMHit&);
		const SiPMHit& operator=(const SiPMHit&);
		G4int operator==(const SiPMHit&) const;

		inline void* operator new(size_t);
		inline void  operator delete(void*);

		void Draw();
		void Print();

	public:

		void SetPos(G4ThreeVector xyz){ pos = xyz; };
		void SetTime(G4double t){ time = t; };
		void SetWavelength(G4double wl){ wavelength = wl; };

		G4ThreeVector GetPos(){ return pos; };
		G4double GetTime(){ return time; };
		G4double GetWavelength(){ return wavelength; };

	private:

		G4ThreeVector pos;	///photon position on sipm
		G4double time;		///photon arrival time on sipm
		G4double wavelength;	///photon wavelength
};

typedef G4THitsCollection<SiPMHit> SiPMHitsCollection;

extern G4Allocator<SiPMHit> SiPMHitAllocator;

inline void* SiPMHit::operator new(size_t)
{
	void *aHit;
	aHit = (void *) SiPMHitAllocator.MallocSingle();
	return aHit;
}

inline void SiPMHit::operator delete(void *aHit)
{
	SiPMHitAllocator.FreeSingle((SiPMHit*) aHit);
}

#endif

