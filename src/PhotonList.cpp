#include "PhotonList.h"

#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

PhotonList::PhotonList()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "PhotonList::PhotonList()" << endl;
}

void PhotonList::AddPhoton( Photon photon )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "PhotonList::AddPhoton( Photon photon )" << endl;

	this->push_back(photon);
}

void PhotonList::AddPhoton( double x, double y, double time, double wavelength )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "PhotonList::AddPhoton( double x, double y, double time, double wavelength )" << endl;

	Photon photon;
	photon.pos_x = x;
	photon.pos_y = y;
	photon.time = time;
	photon.wavelength = wavelength;
	this->push_back(photon);
}

void PhotonList::AddPhotonList( PhotonList phot_list )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "PhotonList::AddPhotonList( PhotonList photonList )" << endl;

	for(auto it : phot_list)
	{
		Photon photon = it;
		this->push_back(photon);
	}
}

