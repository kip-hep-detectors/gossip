/********** Version 2.0 **********/

#ifndef PhotonList_hh
#define PhotonList_hh

#include <vector>

using namespace std;

class PhotonList : public vector<vector<double> >
{

  public:
    
    PhotonList();
    ~PhotonList(){ };
    
    void AddPhoton( double x, double y, double time );
    
  private:
    
    vector<double> photon;
    
};

#endif
