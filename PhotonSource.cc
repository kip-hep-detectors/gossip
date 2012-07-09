#include "PhotonSource.hh"
#include <iostream>
#include "TF1.h"
#include "math.h"
#include "ctime"

using namespace std;

PhotonSource::PhotonSource()
{
  r.SetSeed(time (NULL));
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
}


PhotonList PhotonSource::GeneratePhotons()
{
  photonList.clear();

  //Number of photons N
  double N=0;					//wenn man direkt Ngamma=r.Poisson(Ngamma) übergibt, macht ROOT Probleme
  if(fluctuation==true) N=r.Poisson(Ngamma);
  else                  N=Ngamma;

  //Photon position & time
  double xHit=0;
  double yHit=0;
  double tHit=0;

  //Double exponential for exp2 option
  TF1 exp2("exp2","[0]*[1]/([0]-[1])*(exp(-x/[0])-exp(-x/[1]))",0,1000);
  exp2.SetParameters(tRise,tWidth);

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
      xHit=(r.Rndm()-0.5)*xWidth + x;
      yHit=2*(r.Rndm()-0.5)*sqrt(pow(yWidth/2.,2)-pow(yWidth/xWidth*xHit,2)) + y;
    }
    else if(shape=="custom")
    {
      xHit=hShape->ProjectionX()->GetRandom() + x;
      yHit=hShape->ProjectionY()->GetRandom() + y;
    }
    else
    {
      cout << ">>> Error: Unknown shape!" << endl;
    }

    //Time structure
    if(pulse=="flat") tHit=r.Rndm()*tWidth + t;
    else if(pulse=="gaus") tHit=r.Gaus (t,tWidth);
    else if(pulse=="exp" || (pulse=="exp2" && tRise<=0))  tHit=r.Exp  (tWidth) + t;
    else if(pulse=="exp2") tHit=exp2.GetRandom() + t;
    else if(pulse=="custom") tHit=hTime->GetRandom() + t;
    else cout << ">>> Error: Unknown pulse!" << endl;
    
    //Add photon to list
    photonList.AddPhoton(xHit,yHit,tHit);
  }

  return photonList;
}
