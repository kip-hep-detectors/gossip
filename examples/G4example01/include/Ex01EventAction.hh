// 
// --------------------------------------------------------------
//      Gossip GEANT 4 example - Ex01
// --------------------------------------------------------------

#ifndef Ex01EventAction_h
#define Ex01EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>

/**Inlude gossip headers*/
#include "sipmMC.h"
#include "PhotonList.h"

class Ex01RunAction;
class TTree;
class TGraph;

class Ex01EventAction : public G4UserEventAction
{
	public:
		Ex01EventAction(Ex01RunAction*);
		~Ex01EventAction();

		void BeginOfEventAction(const G4Event*);
		void EndOfEventAction(const G4Event*);

	private:

		void SetBranches();
		Ex01RunAction* runAct;

		G4int sipmCollID;
		G4int scintCollID;

		int runNb;			///run number
		int eventNb;			///event number

		/** Option 1:
		 * You can write out the photon information and simulate the sipm output outside of the Geant4 simulation
		 * This is more flexible since you can change the sipm simulation parameters without having to rerun the G4 simulation
		 * (see 'processOutput.cpp')
		 * This is probably what you want to do
		 */

		TTree *tree_photons;		///ROOT tree for photon information

		std::vector<double> posX;	///photon x position on sipm
		std::vector<double> posY;	///photon y position on sipm
		std::vector<double> time;	///photon arrival time on sipm
		std::vector<double> wavelength;	///photon wavelength

		int nHits;			///number of photons on the sipm

		/**----------------------------------------------------------------------------------**/



		/** Option 2:
		 * You can directly simulate the sipm output during the Geant4 run
		 * This is only recommended if you want to process huge amounts of data and cannot store all the photon information on disk as for option 1
		 */

		TTree *tree_gossip;		///ROOT tree for simulation output

		sipmMC *sipm;			///sipm simulation
		PhotonList photons;		///Photon list to be processed by sipm simulation

		double charge;			///sipm output charge
		TGraph *waveform;		///sipm output waveform

		/**----------------------------------------------------------------------------------**/



		/** Option 3:
		 * You can only write out the total energy deposition in the scintillator and use the GosSiP PhotonSource class to simulate the photon distribution
		 * This is a very minimalistic alternative - you lose all the information about the individual photons and have to parameterise the spatial and time distribution using the PhotonSource class
		 * This option is useful if you already know the spatial and time distribution of the photons since you just need the total energy deposition from Geant4 to scale the number of photons and
		 * don't need to simulate the photons in Geant4 which save a lot of time
		 */

		TTree *tree_edep;		///ROOT tree for the energy deposition

		double edep;			///total energy deposition in the event

		/**----------------------------------------------------------------------------------**/
};

#endif


