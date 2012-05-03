#ifndef PhotonSource_hh
#define PhotonSource_hh

#include <vector>
#include "TRandom3.h"
#include "PhotonList.hh"
#include <iostream>

using namespace std;

/**
 * Simple simulation of a pulsed light source
 */
class PhotonSource
{

  public:
    
    PhotonSource();
    ~PhotonSource(){ };
    /**Generates a photon list*/
    PhotonList  GeneratePhotons();
    /**Sets light spot shape. Options are "square","elliptic"*/
    void        SetShape( string Shape ){ shape = Shape; };
    /**Sets time distribution. Options are "gaus","flat","exp","exp2"*/
    void        SetPulse( string Pulse ){ pulse = Pulse; };
    /**Sets x,y position in mm of light spot center*/
    void        SetXY( double X, double Y ){ x = X; y = Y; };
    /**Sets width of light spot in x,y in mm*/
    void        SetXYWidth( double Xwidth, double Ywidth ){ xWidth = Xwidth; yWidth = Ywidth; };
    /**Sets initial time of light pulse*/
    void        SetTime( double T ){ t = T; };
    /**Sets duration of light pulse. For "gaus": sigma, "flat": width, "exp": time constant*/
    void        SetTimeWidth( double Twidth ){ tWidth = Twidth; };
    /**Sets rise of light pulse. Only for "exp2" option*/
    void        SetRiseTime( double Trise ){ tRise = Trise; };
    /**Sets mean number of photons in the pulse*/
    void        SetNgamma( int nGamma ){ Ngamma = nGamma; };
    /**Enables poisson smearing of number of photons in the pulse*/
    void        SetFluctuation( bool fluct ){ fluctuation = fluct; };
                                              
    
  private:
    
    PhotonList	photonList;
    TRandom3 	r;
    bool 	fluctuation;
    double 	x, y;
    double 	xWidth, yWidth;
    double 	t, tWidth, tRise;
    double 	Ngamma;
    string 	shape, pulse;
};

#endif
