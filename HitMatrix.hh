#ifndef HitMatrix_hh
#define HitMatrix_hh

#include <vector>
#include <TH2.h>
#include <TCanvas.h>

using namespace std;

enum { PE, DR, XT, AP };
enum { X, Y, TIME, TYPE, AMPLITUDE, PROCESSED };

/**
 * List of avalanche breakdowns (hits)
 * 
 * Class inherits from vector<vector<double> >
 * 
 * Hit vector contains: 0:x, 1:y, 2:type, 3:time, 4:amplitude, 5:processed flag
 *
 * Types are: 0:PE (detected photon), 1:DR (thermal pulse), 2:XT (cross-talk), 3:AP (after-pulse)
 */
class HitMatrix : public vector<vector<double> >
{
  public:
    
    HitMatrix();
    ~HitMatrix(){ };
    
    void                Init();									/**<Initializes the hitmatrix and delets all hits*/
    void                Clear( int type );							/**<Deletes all hits of type "type"*/
    int                 AddHit( int x, int y, double time, int type );				/**<Adds hit (amplitude = -1, processed = 0). x,y coordinates in units of pixels! (x,y >= 0,0)*/
    void                HitProcessed( unsigned int iHit );					/**<Sets processed flag for iHit*/
    vector<double>	GetHit( unsigned int iHit );						/**<Returns hit i in the list. Hit vector contains: 0:x, 1:y, 2:type, 3:time, 4:amplitude, 5:processed flag.*/
    bool                GetProcessed( unsigned int iHit );					/**<Returns processed status*/
    double              GetPreviousTime( unsigned int iHit );					/**<Returns time of pixel hit before iHit*/
    int                 FindHit( vector<double> aHit );						/**<Returns position i of aHit in the list*/
    int                 nHits( int type );							/**<Returns number of hits of type "type"*/
    int                 nHits();								/**<Returns total number of hits*/
    TH2D*               DrawMatrix();								/**<Draws all hits of type "type" (-1 = all)*/
    void                PrintMatrix();								/**<Prints out hitmatrix*/
    void                SetAmplitude( unsigned int iHit, double amplitude );			/**<Sets amplitude for iHit*/
												/**Sets integtration gate in ns. gCut=false one more avalanche after end of gate appears in the hitmatrix. Mainly for test purposes of TDCSpectrum. Leave at "true" for most applications!*/
    void                SetGate( double g, bool gCut=true ){ gate = g; gateCut = gCut; };
    void                SetGeometry( TH2I* geometry );						/**<Sets pixel layout*/
    
  private:
    
    double              gate;
    bool		gateCut;
    TH2I                *h_geometry;
    int                 Nx;
    int                 Ny;
    vector<double>	hit;
    TH2D                *h_hits;
    TCanvas             *c_hitMatrix;
};

#endif
