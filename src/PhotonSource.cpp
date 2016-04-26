#include "PhotonSource.h"

#include "gglobals.h"

#include <iostream>
#include <TF1.h>
#include <math.h>

using namespace std;

PhotonSource::PhotonSource()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "PhotonSource::PhotonSource()" << endl;

	r.SetSeed(0);
	x=0;
	y=0;
	t=0;
	xWidth=1;
	yWidth=1;
	tWidth=1;
	tRise=0.1;
	Ngamma=0;
	shape="square";
	pulse="flat";
	fluctuation=true;

	fExp2 = new TF1("exp2","[0]*[1]/([0]-[1])*(exp(-x/[0])-exp(-x/[1]))",0,1000);
}


PhotonList PhotonSource::GeneratePhotons()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "PhotonSource::GeneratePhotons()" << endl;

	photonList.clear();

	//Number of photons N
	double N=0;					//wenn man direkt Ngamma=r.Poisson(Ngamma) übergibt, macht ROOT Probleme
	if(fluctuation==true) N=r.Poisson(Ngamma);
	else                  N=Ngamma;

	//Photon position & time
	double xHit=0;
	double yHit=0;
	double tHit=0;

	fExp2->SetParameters(tRise,tWidth);

	for(int i=0;i<N;i++)
	{
		//Shape
		if(shape=="square")
		{
			xHit=(r.Rndm()-0.5)*xWidth + x;
			yHit=(r.Rndm()-0.5)*yWidth + y;
		}
		else if(shape=="elliptic")
		{
			xHit=(r.Rndm()-0.5)*xWidth;
			yHit=2*(r.Rndm()-0.5)*sqrt(pow(yWidth/2.,2)-pow(yWidth/xWidth*xHit,2));

			xHit += x;
			yHit += y;
		}
		else if(shape=="custom")
		{
			xHit=hShape->ProjectionX()->GetRandom() + x;
			yHit=hShape->ProjectionY()->GetRandom() + y;
		}
		else
		{
			cout << C_RED << "ERROR: Unknown shape!" << C_RESET << endl;
		}

		//Time structure
		if(pulse=="flat") tHit=r.Rndm()*tWidth + t;
		else if(pulse=="gaus") tHit=r.Gaus (t,tWidth);
		else if(pulse=="exp" || (pulse=="exp2" && tRise<=0))  tHit=r.Exp  (tWidth) + t;
		else if(pulse=="exp2") tHit=fExp2->GetRandom() + t;
		else if(pulse=="custom") tHit=hTime->GetRandom() + t;
		else cout << C_RED << "ERROR: Unknown pulse!" << C_RESET << endl;

		//Add photon to list
		photonList.AddPhoton(xHit,yHit,tHit);
	}

	return photonList;
}

