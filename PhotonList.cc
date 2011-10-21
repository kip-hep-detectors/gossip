/********** Version 2.0 **********/

#include "PhotonList.hh"

using namespace std;

PhotonList::PhotonList()
{
  photon.clear();
}

void PhotonList::AddPhoton( double x, double y, double time )
{
  photon.clear();
  photon.push_back(x);
  photon.push_back(y);
  photon.push_back(time);
  this->push_back(photon);
}
