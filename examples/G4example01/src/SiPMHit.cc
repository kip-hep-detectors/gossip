// 
// --------------------------------------------------------------
//      Gossip GEANT 4 example - 
// --------------------------------------------------------------

#include "SiPMHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<SiPMHit> SiPMHitAllocator;


SiPMHit::SiPMHit() {}

SiPMHit::~SiPMHit() {}

SiPMHit::SiPMHit(const SiPMHit& right)
: G4VHit()
{
	pos = right.pos;
	time = right.time;
	wavelength = wavelength;
}

const SiPMHit& SiPMHit::operator=(const SiPMHit& right)
{
	pos = right.pos;
	time = right.time;
	wavelength = wavelength;
	return *this;
}

G4int SiPMHit::operator==(const SiPMHit& right) const
{
	return (this==&right) ? 1 : 0;
}

void SiPMHit::Draw()
{
	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
	if(pVVisManager)
	{
		G4Circle circle(pos);
		circle.SetScreenSize(4.);
		circle.SetFillStyle(G4Circle::filled);
		G4Colour colour(1.,0.,0.);
		G4VisAttributes attribs(colour);
		circle.SetVisAttributes(attribs);
		pVVisManager->Draw(circle);
	}
}

void SiPMHit::Print()
{
}

