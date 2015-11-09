#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "TGraph.h"
#include "TFile.h"
#include "sipmMC.h"
#include "PhotonList.h"

using namespace std;

int main(int argc, char** argv)
{
	if(argc<2)
	{
		printf("Please enter a file name: './processPhotonOutput exmaple.bin'\n");
		exit(0);
	}

	const char* filename = argv[1];

	///open gossip output file
	FILE *file = fopen(filename,"rb");

	///Photon list to be processed by sipm simulation
	PhotonList photons;

	///sipm simulation
	sipmMC *sipm = new sipmMC();
	sipm->GetParaFile("input/MPPC_3x3.txt");
	sipm->SetSampling(0.2);
	sipm->SetGate(300);
	sipm->SetPreGate(100);

	TFile f("input/SpectralSensitivity.root");
	sipm->SetSpectralSensitivity((TGraph*)f.Get("MPPC_noRef"));
	f.Close();

	///TGraph for the waveform
	TGraph *g_wf = new TGraph();

	unsigned int eventNb;	///event number
	unsigned int nPhotons;	///number of photons in the event

	char *buffer[1024];	///buffer for reading in values

	///loop through events
	while(1)
	{
		///get event number
		int ret = fread(&buffer, 1, sizeof(unsigned int), file);
		if(ret != sizeof(unsigned int)) break;		///break at end of file
		eventNb = *((unsigned int*)buffer);

		///get number of photons
		fread(&buffer, 1, sizeof(float), file);
		nPhotons = *((unsigned int*)buffer);

		///build photon list
		for(unsigned int i=0;i<nPhotons;i++)
		{
			///get x position of photon
			fread(&buffer, 1, sizeof(float), file);
			float x = *((float*)buffer);

			///get y position of photon
			fread(&buffer, 1, sizeof(float), file);
			float y = *((float*)buffer);

			///get arrival time of photon
			fread(&buffer, 1, sizeof(float), file);
			float t = *((float*)buffer);

			///get wavelength of photon
			fread(&buffer, 1, sizeof(float), file);
			float wl = *((float*)buffer);

			photons.AddPhoton(x,y,t,wl);
		}

		sipm->Generate(photons);
		
		g_wf = sipm->GetWaveform();
		
		double charge = sipm->GetCharge();
		
		cout << "Signal charge: " << charge << endl;

		///now you can do stuff with the waveform
		///e.g. get the timestamp when signal crosses threshold of 2 mV
		double ts = -1;
		for(int i=0;i<g_wf->GetN();i++)
		{
			double amplitude = g_wf->GetY()[i];
			double time = g_wf->GetX()[i];
			if(amplitude>2)
			{
				ts = time;
				break;
			}
		}
		cout << "Timestamp at " << ts << " ns" << endl;
	}
}

