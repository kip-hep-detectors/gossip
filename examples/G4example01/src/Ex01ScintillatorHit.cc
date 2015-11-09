// 
// --------------------------------------------------------------
//      Gossip GEANT 4 example - Ex01
// --------------------------------------------------------------

#include "Ex01ScintillatorHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<Ex01ScintillatorHit> Ex01ScintillatorHitAllocator;


Ex01ScintillatorHit::Ex01ScintillatorHit() {}

Ex01ScintillatorHit::~Ex01ScintillatorHit() {}

	Ex01ScintillatorHit::Ex01ScintillatorHit(const Ex01ScintillatorHit& right)
: G4VHit()
{
	edep = right.edep;
	pos = right.pos;
}

const Ex01ScintillatorHit& Ex01ScintillatorHit::operator=(const Ex01ScintillatorHit& right)
{
	edep = right.edep;
	pos = right.pos;
	return *this;
}

G4int Ex01ScintillatorHit::operator==(const Ex01ScintillatorHit& right) const
{
	return (this==&right) ? 1 : 0;
}

void Ex01ScintillatorHit::Draw()
{
	//	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
	//	if(pVVisManager)
	//	{
	//		G4Circle circle(pos);
	//		circle.SetScreenSize(4.);
	//		circle.SetFillStyle(G4Circle::filled);
	//		G4Colour colour(1.,0.,0.);
	//		G4VisAttributes attribs(colour);
	//		circle.SetVisAttributes(attribs);
	//		pVVisManager->Draw(circle);
	//	}
}

void Ex01ScintillatorHit::Print()
{
}
