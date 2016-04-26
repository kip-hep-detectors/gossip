#ifndef AvalancheList_h
#define AvalancheList_h

#include <iostream>
#include <vector>
#include <TH2.h>
//#include <TCanvas.h>

using std::vector;
using std::cout;
using std::endl;

struct Avalanche
{
	Avalanche()
	{
		x = -1;
		y = -1;
		time = 0;
		type = -1;
		amplitude = 0;
	}

	Avalanche( int kx, int ky, double ktime, int ktype, double kamplitude=0 )
	{
		x = kx;
		y = ky;
		time = ktime;
		type = ktype;
		amplitude = kamplitude;
	}

	bool operator<( const Avalanche& rhs ) const
	{
		return time < rhs.time;
	}

	bool operator==( const Avalanche& rhs ) const
	{
		return x == rhs.x && y == rhs.y && time == rhs.time && type == rhs.type && amplitude == rhs.amplitude;
	}

	int x;
	int y;
	double time;
	int type;
	double amplitude;
	bool processed;
};


enum { PE, DR, CT, AP };

/**
 * List of avalanche breakdowns (hits)
 */
class AvalancheList
{
	public:

		AvalancheList();
		~AvalancheList();

		Avalanche& At( int i ){ return avlist[i]; };
		Avalanche& operator []( int i ){ return avlist[i]; }

		void		Init();						/**<Initializes the hitmatrix and delets all hits*/
		void		Clear( int type );				/**<Deletes all hits of type "type"*/
		int		AddHit( int x, int y, double time, int type );	/**<Adds hit (amplitude = 0). x, y coordinates in units of pixels! (x, y >= 0, 0)*/
		double		GetPreviousTime( unsigned int iHit );		/**<Returns time of pixel hit before iHit*/
		void		EraseHit( Avalanche av );			/**<Deletes aHit from the list*/
		int		nHits( int type );				/**<Returns number of hits of type "type"*/
		int		nHits();					/**<Returns total number of hits*/
		TH2D*		DrawMatrix();					/**<Draws all hits of type "type" (-1 = all)*/
		void		PrintMatrix();					/**<Prints out hitmatrix*/
		void		SetGate( double g, bool gCut=true );		/**<Sets integtration gate in ns. gCut=false one more avalanche after end of gate appears in the hitmatrix. Mainly for test purposes of TDCSpectrum. Leave at "true" for most applications!*/
		void		SetGeometry( TH2I* geometry );			/**<Sets pixel layout*/

	private:

		vector<Avalanche> avlist;

		double		gate;
		bool		gateCut;
		TH2I		*h_geometry;
		//int		Nx;
		//int		Ny;
		//TH2D		*h_hits;
		//TCanvas		*c_hitMatrix;
};

#endif
