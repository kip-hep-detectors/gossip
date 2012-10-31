#ifndef PhotonList_h
#define PhotonList_h

#include <vector>

using namespace std;

/**
 * List of photons
 *
 * Class inherits from vector<vector<double> >
 *
 * Photon vector contains: 0:x, 1:y, 2:time
 */
class PhotonList : public vector<vector<double> >
{

  public:
    
    PhotonList();
    ~PhotonList(){ };
    
    void AddPhoton( double x, double y, double time );	/**<Adds a photon to the list*/
    void AddPhotonList( PhotonList photonList );	/**<Adds a complete photon list to the list*/
    
  private:
    
    vector<double> photon;				/**<Photon vector: 0:x, 1:y, 2:time. x,y coordinates in mm! (SiPM center is at 0,0)*/
};

#endif
