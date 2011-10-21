/********** Version 1.2 **********/

#ifndef HitMatrix_h
#define HitMatrix_h

#include <vector>

using namespace std;

class HitMatrix{

 public:

  HitMatrix();
  void Init( int );
  int AddHit( int, int, int, double );
  vector<double> GetEvent( int );
  double GetPreviousTime( vector<double> );
  int FindEvent( vector<double> );
  int nFind( int, int, int, double );
  int nHits( int );
  void DrawMatrix( int );
  void TSort( int );
  void TSortLastEvent( unsigned int );
  void PrintMatrix( );
  bool gcut;
  double gate;
  int Nx,Ny;
  vector<vector<double> > matrix; //matrix of the hit-events

 private:

  vector<double> event; // x,y,type (0:PDE, 1:DR, 2:XT, 3:AP),time,amplitude

};

#endif
