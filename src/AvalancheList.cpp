#include "AvalancheList.h"

#include <TROOT.h>
#include <stdio.h>

using namespace std;


AvalancheList::AvalancheList()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "AvalancheList::AvalancheList()" << endl;

	//Nx=10;
	//Ny=10;
	gate=100;
	gateCut=true;
	h_geometry = new TH2I("h_geometry", "h_geometry", 10, 0, 10, 10, 0, 10);
	//h_hits = new TH2D();
	//h_hits->SetNameTitle("AvalancheList", "AvalancheList");
}


AvalancheList::~AvalancheList()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "AvalancheList::~AvalancheList()" << endl;

	delete h_geometry;
	//delete h_hits;
	//if(!c_hitMatrix) delete c_hitMatrix;
}


void AvalancheList::Init()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "AvalancheList::Init()" << endl;

	avlist.clear();
}


void AvalancheList::Clear( int type )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "AvalancheList::Clear( int type )" << endl;

	unsigned int i;
	bool setzero=false;

	if(avlist.size()>0)
	{
		for(i=0;i<avlist.size();i++)
		{
			if(setzero==true) i=0;
			if(avlist[i].type==type)
			{
				avlist.erase(avlist.begin()+i);
				if(i>0){ i--; setzero=false; }
				if(i==0) setzero=true;
			}
			else setzero=false;
		}
	}
}


void AvalancheList::SetGeometry( TH2I* geometry )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "AvalancheList::SetGeometry( TH2I* geometry )" << endl;

	h_geometry = geometry;
	//Nx = h_geometry->GetNbinsX();
	//Ny = h_geometry->GetNbinsY();
}


void AvalancheList::SetGate( double g, bool gCut )
{
	gate = g;
	gateCut = gCut;
}


int AvalancheList::AddHit( int x, int y, double time, int type )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "AvalancheList::AddHit( int x, int y, double time, int type )" << endl;

	int amplitude=0;
	int good=0;

	if(h_geometry->GetBinContent(x+1, y+1)==1)
	{
		if(time<=gate || gateCut==false)
		{
			Avalanche av(x, y, time, type, amplitude);
			auto it = std::upper_bound(avlist.begin(), avlist.end(), av);	//TODO user iterator of last av to make search faster!
			avlist.insert(it, av);
		}
		else good=-1;
	}
	else good=-1;

	return good;
}


double AvalancheList::GetPreviousTime( unsigned int iHit )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "AvalancheList::GetPreviousTime( unsigned int iHit )" << endl;

	int i = iHit-1;
	double tlast=-1;

	while(i>=0)
	{
		if(avlist[i].x==avlist[iHit].x && avlist[i].y==avlist[iHit].y)
		{
			tlast = avlist[i].time;
			break;
		}
		i--;
	}

	return tlast;
}


void AvalancheList::EraseHit( unsigned int iHit )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "AvalancheList::EraseHit( vector<double> aHit )" << endl;

	avlist.erase(avlist.begin() + iHit);
}


int AvalancheList::nHits( int type )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "AvalancheList::nHits( int type )" << endl;

	int n=0;

	for(auto av : avlist)
	{
		if(av.type == type) n++;
	}

	return n;
}


int AvalancheList::nHits()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "AvalancheList::nHits()" << endl;

	return avlist.size();
}


//TH2D* AvalancheList::DrawMatrix()
//{
	//if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "AvalancheList::DrawMatrix()" << endl;

	////Build canvas if neccessary
	//c_hitMatrix = (TCanvas*)gROOT->FindObject("c_AvalancheList");
	//if(c_hitMatrix==0) c_hitMatrix = new TCanvas("c_AvalancheList", "c_AvalancheList", 700, 0, 400, 400);

	//c_hitMatrix->cd();

	//h_hits->Reset("M");
	//h_hits->SetBins(Nx, 0, Nx, Ny, 0, Ny);

	//int x, y;
	//for(auto av : avlist)
	//{
		//x=av.x+1;
		//y=av.y+1;
		//h_hits->SetBinContent(x, y, av.type+0.5);
	//}

	//h_hits->SetMaximum(4);
	//h_hits->SetStats(kFALSE);
	//h_hits->GetXaxis()->SetNdivisions(Nx);
	//h_hits->GetYaxis()->SetNdivisions(Ny);
	//h_hits->SetContour(4, 0);
	//c_hitMatrix->SetGridx();
	//c_hitMatrix->SetGridy();
	//h_hits->Draw("colz");

	//c_hitMatrix->Modified();
	//c_hitMatrix->Update();

	//return h_hits;
//}


void AvalancheList::PrintMatrix()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"), "1", 1)==0) cout << "AvalancheList::PrintMatrix()" << endl;

	cout.precision(3);
	cout << "\nAvalancheList: " << endl;
	cout<<"X\tY\tTime\tType\tAmplitude\tProcessed\n"<<endl;

	string type;
	for(auto av : avlist)
	{
		if(av.type==PE) type = "PE";
		else if(av.type==DR) type = "DR";
		else if(av.type==CT) type = "CT";
		else if(av.type==AP) type = "AP";
		else type = "NK";

		cout << av.x << "\t" << av.y << "\t" << av.time << "\t" << type << "\t" << av.amplitude << endl;
	}
}
