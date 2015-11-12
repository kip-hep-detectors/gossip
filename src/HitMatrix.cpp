#include "HitMatrix.h"

#include <TROOT.h>
#include <stdio.h>

using namespace std;


HitMatrix::HitMatrix()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "HitMatrix::HitMatrix()" << endl;

	Nx=10;
	Ny=10;
	gate=100;
	gateCut=true;
	h_geometry = new TH2I("h_geometry","h_geometry",10,0,10,10,0,10);
	h_hits = new TH2D();
	h_hits->SetNameTitle("HitMatrix","HitMatrix");
}


HitMatrix::~HitMatrix()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "HitMatrix::~HitMatrix()" << endl;

	delete h_geometry;
	delete h_hits;
	if(!c_hitMatrix) delete c_hitMatrix;
}


void HitMatrix::Init()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "HitMatrix::Init()" << endl;

	this->clear();
}


void HitMatrix::Clear( int type )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "HitMatrix::Clear( int type )" << endl;

	unsigned int i;
	bool setzero=false;

	if(this->size()>0)
	{
		for(i=0;i<this->size();i++)
		{
			if(setzero==true) i=0;
			if(this->at(i).at(TYPE)==type)
			{
				this->erase(this->begin()+i);
				if(i>0){ i--; setzero=false; }
				if(i==0) setzero=true;
			}
			else setzero=false;
		}
	}
}


void HitMatrix::SetGeometry( TH2I* geometry )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "HitMatrix::SetGeometry( TH2I* geometry )" << endl;

	h_geometry = geometry;
	Nx = h_geometry->GetNbinsX();
	Ny = h_geometry->GetNbinsY();
}


int HitMatrix::AddHit( int x, int y, double time, int type )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "HitMatrix::AddHit( int x, int y, double time, int type )" << endl;

	int amplitude=-1;
	int processed=0;
	int good=0;

	hit.clear();

	if(h_geometry->GetBinContent(x+1,y+1)==1)
	{
		if((time<=gate/* && time>=0*/) || gateCut==false)
		{
			hit.push_back(x);
			hit.push_back(y);
			hit.push_back(time);
			hit.push_back(type);
			hit.push_back(amplitude);
			hit.push_back(processed);
			this->push_back(hit);
		}
		else good=-1;
	}
	else good=-1;

	//Sort hit in time:
	int size = this->size();
	bool swaped=false;

	if(size!=1 && good!=-1)				//nothing to do for first hit
	{
		int i=1;
		while(size-i>0)					//stop if hit is first in list
		{
			if(hit[TIME]>=this->at(size-i-1).at(TIME)) break;	//stop if sorted
			i++;
			swaped=true;
		}
		if(swaped==true)
		{
			this->insert(this->end()-i,hit);
			this->erase(this->end());
		}
	}

	return good;
}


vector<double> HitMatrix::GetHit( unsigned int iHit )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "HitMatrix::GetHit( unsigned int iHit )" << endl;

	return this->at(iHit);
}


void HitMatrix::SetAmplitude( unsigned int iHit, double amplitude )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "HitMatrix::SetAmplitude( unsigned int iHit, double amplitude )" << endl;

	this->at(iHit).at(AMPLITUDE) = amplitude;
}


void HitMatrix::HitProcessed( unsigned int iHit )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "HitMatrix::HitProcessed( unsigned int iHit )" << endl;

	this->at(iHit).at(PROCESSED) = 1;
}


bool HitMatrix::GetProcessed( unsigned int iHit )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "HitMatrix::SetProcessed( unsigned int iHit )" << endl;

	return this->at(iHit).at(PROCESSED);
}


double HitMatrix::GetPreviousTime( unsigned int iHit )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "HitMatrix::GetPreviousTime( unsigned int iHit )" << endl;

	int i = iHit-1;
	double tlast=-1;

	while(i>=0)
	{
		if(this->at(i).at(X)==this->at(iHit).at(X) && this->at(i).at(Y)==this->at(iHit).at(Y))
		{
			tlast = this->at(i).at(TIME);
			break;
		}
		i--;
	}

	return tlast;
}


int HitMatrix::FindHit( vector<double> aHit )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "HitMatrix::FindHit( vector<double> aHit )" << endl;

	int n=-1;
	unsigned int i;

	for(i=0;i<this->size();i++)
	{
		if(aHit[X]==this->at(i).at(X) && aHit[Y]==this->at(i).at(Y) && aHit[TIME]==this->at(i).at(TIME) && aHit[TYPE]==this->at(i).at(TYPE)) n=i;
	}

	return n;
}


void HitMatrix::EraseHit( vector<double> aHit )
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "HitMatrix::EraseHit( vector<double> aHit )" << endl;

	int iHit = FindHit(aHit);
	this->erase(this->begin()+iHit);
}


int HitMatrix::nHits(int type)
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "HitMatrix::nHits( int type )" << endl;

	int n=0;
	unsigned int i;

	for(i=0;i<this->size();i++)
	{
		if(this->at(i).at(TYPE)==type) n++;
	}

	return n;
}


int HitMatrix::nHits()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "HitMatrix::nHits()" << endl;

	return this->size();
}


TH2D* HitMatrix::DrawMatrix()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "HitMatrix::DrawMatrix()" << endl;

	//Build canvas if neccessary
	c_hitMatrix = (TCanvas*)gROOT->FindObject("c_HitMatrix");
	if(c_hitMatrix==0) c_hitMatrix = new TCanvas("c_HitMatrix","c_HitMatrix",700,0,400,400);

	c_hitMatrix->cd();

	h_hits->Reset("M");
	h_hits->SetBins(Nx,0,Nx,Ny,0,Ny);
	unsigned int i;
	int x,y;

	for(i=0;i<this->size();i++)
	{
		x=this->at(i).at(X)+1;
		y=this->at(i).at(Y)+1;
		h_hits->SetBinContent(x,y,this->at(i).at(TYPE)+0.5);
	}

	h_hits->SetMaximum(4);
	h_hits->SetStats(kFALSE);
	h_hits->GetXaxis()->SetNdivisions(Nx);
	h_hits->GetYaxis()->SetNdivisions(Ny);
	h_hits->SetContour(4,0);
	c_hitMatrix->SetGridx();
	c_hitMatrix->SetGridy();
	h_hits->Draw("colz");

	c_hitMatrix->Modified();
	c_hitMatrix->Update();

	return h_hits;
}


void HitMatrix::PrintMatrix()
{
	if(getenv("GOSSIP_DEBUG")!=0 && strncmp(getenv("GOSSIP_DEBUG"),"1",1)==0) cout << "HitMatrix::PrintMatrix()" << endl;

	cout.precision(3);
	cout << "\nHitMatrix: " << endl;
	cout<<"X\tY\tTime\tType\tAmplitude\tProcessed\n"<<endl;
	unsigned int i;
	for(i=0;i<this->size();i++)
	{
		if(this->at(i).at(TYPE)==PE)
			cout<<this->at(i).at(0)<<"\t"<<this->at(i).at(1)<<"\t"<<this->at(i).at(TIME)<<"\tPE\t"<<this->at(i).at(AMPLITUDE)<<"\t\t"<<this->at(i).at(PROCESSED)<<endl;
		if(this->at(i).at(TYPE)==DR)
			cout<<this->at(i).at(0)<<"\t"<<this->at(i).at(1)<<"\t"<<this->at(i).at(TIME)<<"\tDR\t"<<this->at(i).at(AMPLITUDE)<<"\t\t"<<this->at(i).at(PROCESSED)<<endl;
		if(this->at(i).at(TYPE)==CT)
			cout<<this->at(i).at(0)<<"\t"<<this->at(i).at(1)<<"\t"<<this->at(i).at(TIME)<<"\tCT\t"<<this->at(i).at(AMPLITUDE)<<"\t\t"<<this->at(i).at(PROCESSED)<<endl;
		if(this->at(i).at(TYPE)==AP)
			cout<<this->at(i).at(0)<<"\t"<<this->at(i).at(1)<<"\t"<<this->at(i).at(TIME)<<"\tAP\t"<<this->at(i).at(AMPLITUDE)<<"\t\t"<<this->at(i).at(PROCESSED)<<endl;
	}
}
