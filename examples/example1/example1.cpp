#include "PhotonSource.h"
#include "PhotonList.h"
#include "sipmMC.h"
#include "HitMatrix.h"
#include "QDC.h"
#include "Oscilloscope.h"

#include "stdlib.h"
#include "TH1D.h"
#include "TCanvas.h"

#include <TApplication.h>

using namespace std;


//------------------------------------

int main(int argc,char** argv){

	///Tapplication to display TCanvas
	TApplication app("sipmMC", 0, 0);
	TCanvas* c1 = new TCanvas("c1","c1",10,10,800,600);
	TCanvas* c_osci = new TCanvas("c_osci","c_osci",810,10,800,600);

	///Define lightSource and sipm
	PhotonSource *lightSource = new PhotonSource();
	sipmMC *sipm = new sipmMC();

	///Read sipm parameters from file
	sipm->GetParaFile("Parameters.txt");

	///Customize light source
	lightSource->SetNgamma(100);		///Number of photons
	lightSource->SetFluctuation(1);		///Poisson smearing for number of photons
	lightSource->SetShape("square");	///Light spot shape
	lightSource->SetXY(0,0);		///Light spot center 0,0 = center of sipm
	lightSource->SetXYWidth(1,1);		///Light spot size
	lightSource->SetPulse("exp");		///Light pulse time ditribution
	lightSource->SetTime(10);		///Incident time
	lightSource->SetTimeWidth(4);		///Characteristic time (tau for "exp", sigma for "gaus", width for "flat")

	PhotonList photons = lightSource->GeneratePhotons();	///Generate photon list

	double charge = sipm->Generate(photons);		///Generate sipm signal from photon list. Returns the signal charge
	/*alternatively you can call:*/
	//double charge = sipm->GetCharge();

	HitMatrix *matrix = sipm->GetHitMatrix();		///Get list of avalanches
	matrix->PrintMatrix();					///Diplay list of avalanches
	c1->cd();
	sipm->GetWaveform().GetGraph()->Draw();			///Diplay signal waveform

	c1->Update();

	///Access entries in avalanche list
	for(int i=0;i<matrix->size();i++)
	{
		cout << "Hit time: " << matrix->at(i).at(2) << endl;
	}

	cout << "Signal charge: " << charge << endl;


	///Using Digitizer modules
	QDC qdc;
	qdc.ConnectSiPM(sipm);
	qdc.SetPedestal(50);
	qdc.SetNbins(1024);
	qdc.SetConversion(1);

	Oscilloscope osci;
	osci.ConnectSiPM(sipm);

	sipm->Generate(photons);

	qdc.Run();
	osci.Run();

	double qdc_value = qdc.GetQDCValue();
	cout << "QDC value: " << qdc_value << endl;

	c_osci->cd();
	osci.GetWaveform().GetGraph()->Draw();
	c_osci->Update();

	cout << "Press any key to exit..." << endl;
	cin.get();

	return 0;
}

