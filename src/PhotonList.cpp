#include "PhotonList.h"

#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

PhotonList::PhotonList()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "PhotonList::PhotonList()" << endl;

	photon.clear();
}

void PhotonList::AddPhoton( double x, double y, double time, double wavelength )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "PhotonList::AddPhoton( double x, double y, double time, double wavelength )" << endl;

	photon.clear();
	photon.push_back(x);
	photon.push_back(y);
	photon.push_back(time);
	photon.push_back(wavelength);
	this->push_back(photon);
}

void PhotonList::AddPhotonList( PhotonList photonList )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "PhotonList::AddPhotonList( PhotonList photonList )" << endl;

	for(unsigned int i=0;i<photonList.size();i++)
	{
		photon.clear();
		photon = photonList.at(i);
		this->push_back(photon);
	}
}

