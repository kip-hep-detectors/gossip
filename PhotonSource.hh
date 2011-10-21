/********** Version 2.0 **********/

#ifndef PhotonSource_hh
#define PhotonSource_hh

#include <vector>
#include "TRandom3.h"
#include "PhotonList.hh"

#include <iostream>

using namespace std;

class PhotonSource
{

  public:
    
    PhotonSource();
    ~PhotonSource(){ };
    
    PhotonList  GeneratePhotons();
    void        SetShape( string Shape ) { shape = Shape; };
    void        SetPulse( string Pulse ) { pulse = Pulse; };
    void        SetXY( double X, double Y ) { x = X; y = Y; };
    void        SetXYWidth(double Xwidth, double Ywidth ) { xWidth = Xwidth; yWidth = Ywidth; };
    void        SetTime( double T ) { t = T; };
    void        SetTimeWidth( double Twidth ) { tWidth = Twidth; };
    void        SetNgamma( int nGamma ){ Ngamma = nGamma; };
    void        SetFluctuation( bool fluct ) { fluctuation = fluct; };
    
  private:
    
    PhotonList photonList;
    
    bool fluctuation;
    
    TRandom3 r;
    
    double x, y;
    double xWidth, yWidth;
    double t, tWidth;
    double Ngamma;
    
    string shape, pulse;
};

#endif
