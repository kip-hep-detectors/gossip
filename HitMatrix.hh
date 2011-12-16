
#ifndef HitMatrix_hh
#define HitMatrix_hh

#include <vector>
#include <TH2.h>
#include <TCanvas.h>

using namespace std;

enum { PE, DR, XT, AP };
enum { X, Y, TIME, TYPE, AMPLITUDE, PROCESSED };

class HitMatrix
{
  public:
    
    HitMatrix();
    ~HitMatrix(){ };
    
    //Functions:
    void                    Init();								//Initialize
    void                    Clear( int type );							//Deletes all hits of type
    int                     AddHit( int x, int y, double time, int type );			//Adds hit (amplitude = -1, processed = 0)
    void                    HitProcessed( unsigned int iHit );					//Set processed flag for iHit
    vector<double>          GetHit( unsigned int iHit );					//Returns hit i
    bool                    GetProcessed( unsigned int iHit );					//Returns processed status
    double                  GetPreviousTime( unsigned int iHit );				//Returns time of pixel hit before iHit
    int                     FindHit( vector<double> aHit );					//Returns position of aHit
    int                     nHits( int type );							//Returns number of hits of type
    int                     nHits();								//Returns total number of hits
    TH2D*                   DrawMatrix();							//Draws all hits of type (-1 = all)
    void                    PrintMatrix();							//Prints out matrix
    void                    SetAmplitude( unsigned int iHit, double amplitude );		//Set amplitude for iHit
    void                    SetGate( double g, bool gCut=true ){ gate = g; gateCut = gCut; };	//Set integtration gate
    void                    SetGeometry( TH2I* geometry );					//Set pixel layout
    
  private:
    
    double                  gate;								//Integration gate
    bool		    gateCut;								//Cut at end of gate - mainly for test purposes of TDCSpec	
    TH2I                   *h_geometry;								//Pixel layout
    int                     Nx,Ny;								//Number of pixels in x,y
    vector<vector<double> > matrix; 								//matrix of the hit-hits
    vector<double>          hit;								// x,y,type,time,amplitude,processed
    TH2D                   *h_hits;								
    TCanvas                *c_hitMatrix;							
};

#endif
