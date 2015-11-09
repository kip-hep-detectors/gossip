// 
// --------------------------------------------------------------
//      Gossip GEANT 4 example - Ex01
// --------------------------------------------------------------

#ifndef Ex01ScintillatorHit_h
#define Ex01ScintillatorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class Ex01ScintillatorHit : public G4VHit
{
	public:

		Ex01ScintillatorHit();
		~Ex01ScintillatorHit();
		Ex01ScintillatorHit(const Ex01ScintillatorHit&);
		const Ex01ScintillatorHit& operator=(const Ex01ScintillatorHit&);
		G4int operator==(const Ex01ScintillatorHit&) const;

		inline void* operator new(size_t);
		inline void  operator delete(void*);

		void Draw();
		void Print();

	public:

		void SetEdep(G4double de){ edep = de; };
		void SetPos(G4ThreeVector xyz){ pos = xyz; };

		G4double GetEdep(){ return edep; };
		G4ThreeVector GetPos(){ return pos; };

	private:

		G4double edep;		///energy deposition of hit
		G4ThreeVector pos;	///position of hit
};

typedef G4THitsCollection<Ex01ScintillatorHit> Ex01ScintillatorHitsCollection;

extern G4Allocator<Ex01ScintillatorHit> Ex01ScintillatorHitAllocator;

inline void* Ex01ScintillatorHit::operator new(size_t)
{
	void *aHit;
	aHit = (void *) Ex01ScintillatorHitAllocator.MallocSingle();
	return aHit;
}

inline void Ex01ScintillatorHit::operator delete(void *aHit)
{
	Ex01ScintillatorHitAllocator.FreeSingle((Ex01ScintillatorHit*) aHit);
}

#endif

