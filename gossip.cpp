#include "GUI.h"
#include "gossipConfig.h"
#include <getopt.h>
#include "TRint.h"

int main(int argc,char** argv)
{
	//Parse program options
	const struct option longopts[] =
	{
		{ "version", no_argument, 0, 'v'},
		{ "help", no_argument, 0, 'h'},
		{ "parfile", required_argument, 0, 's'},
		{ 0,0,0,0},
	};

	int index;
	int iarg=0;

	//turn off getopt error message
	//opterr=1;

	const char* parafile = NULL;

	while(iarg != -1)
	{
		iarg = getopt_long(argc, argv, "vhp:", longopts, &index);
		switch (iarg)
		{
			case 'h':
				cout << endl;
				cout << "---GosSiP is a framework for the simulation of the response of Silicon Photomultipliers---" << endl;
				cout << endl;
				cout << "Usage: gossip [options]" << endl;
				cout << endl;
				cout << "Possible options are:" << endl;
				cout << "\t --help (or -h)\n\t\tOutput this text; GosSiP is not executed." << endl;
				cout << "\t --version (or -v)\n\t\tOutput the version number of GosSiP; GosSiP is not executed." << endl;
				cout << "\t --parfile FILE (or -p FILE)\n\t\tStarts GosSiP loading the SiPM parameters from FILE." << endl;
				cout << endl;
				return 1;

			case 'v':
				cout << "GosSiP version: " << gossip_VERSION_MAJOR << "." << gossip_VERSION_MINOR << endl;
				return 1;

			case 's':
				parafile = optarg;
				break;
		}
	}

	//Run GosSiP
	TRint app("sipmMC", 0, 0);

	GUI gui;
	gui.BuildMainFrame();
	if(parafile) gui.ReadParaFile(parafile);

	app.Run();

	return 0;
}
