#ifndef PhotonList_h
#define PhotonList_h

#include <vector>

using std::vector;

/**
 * Photon definition
 */
struct Photon
{
	double pos_x;
	double pos_y;
	double time;
	double wavelength;
};

/**
 * List of photons
 */
class PhotonList : public vector<Photon>
{
	public:
		PhotonList();
		~PhotonList(){};

		void AddPhoton( Photon phot );							/**<Adds a photon to the list*/
		void AddPhoton( double x, double y, double time, double wavelength = 0 );	/**<Adds a photon to the list*/
		void AddPhotonList( PhotonList photonList );					/**<Adds a complete photon list to the list*/
};

#endif
