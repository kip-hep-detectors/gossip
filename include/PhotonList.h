#ifndef PhotonList_h
#define PhotonList_h

#include <vector>

using std::vector;

enum { PL_X, PL_Y, PL_TIME, PL_WL };

/**
 * List of photons
 *
 * Class inherits from vector<vector<double> >
 *
 * Photon vector contains: 0:x, 1:y, 2:time, 3:wavelength
 */
class PhotonList : public vector<vector<double> >
{

	public:

		PhotonList();
		~PhotonList(){};

		void AddPhoton( double x, double y, double time, double wavelength = 0 );	/**<Adds a photon to the list*/
		void AddPhotonList( PhotonList photonList );					/**<Adds a complete photon list to the list*/

	private:

		vector<double> photon;								/**<Photon vector: 0:x, 1:y, 2:time, 3:wavelength. x,y coordinates are in mm (SiPM center is at 0,0), time in ns, wavelength in nm*/
};

#endif
