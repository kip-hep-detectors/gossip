#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "TGraph.h"

using namespace std;

int main(int argc, char** argv)
{
	if(argc<2)
	{
		printf("Please enter a file name: './processGossipOutput exmaple.bin'\n");
		exit(0);
	}

	const char* filename = argv[1];

	///open gossip output file
	FILE *file = fopen(filename,"rb");

	///TGraph for the waveform
	TGraph *g_wf = new TGraph();

	unsigned int eventNb;	///event number
	float charge;		///signal charge
	float sampling; 	///sampling
	unsigned int sampleNb;	///sampleNb

	char *buffer[1024];	///buffer for reading in values

	///loop through events
	while(1)
	{
		///get event number
		int ret = fread(&buffer, 1, sizeof(unsigned int), file);
		if(ret != sizeof(unsigned int)) break;		///break at end of file
		eventNb = *((unsigned int*)buffer);

		///get charge
		fread(&buffer, 1, sizeof(float), file);
		charge = *((float*)buffer);

		///get sampling
		fread(&buffer, 1, sizeof(float), file);
		sampling = *((float*)buffer);

		///get number of samples
		fread(&buffer, 1, sizeof(float), file);
		sampleNb = *((unsigned int*)buffer);

		///get waveform
		double amplitude;
		double time;
		for(unsigned int i=0;i<sampleNb;i++)
		{
			time = sampling * i;

			fread(&buffer, 1, sizeof(float), file);
			amplitude = *((float*)buffer);

			g_wf->SetPoint(i,time,amplitude);
		}

		///now you can do stuff with the waveform
		///e.g. get the timestamp when signal crosses threshold of 2 mV
		double ts = -1;
		for(unsigned int i=0;i<sampleNb;i++)
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

