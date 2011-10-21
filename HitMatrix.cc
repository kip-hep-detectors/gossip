/********** Version 2.0 **********/

#include "HitMatrix.hh"
#include <iostream>
#include <stdio.h>

using namespace std;


HitMatrix::HitMatrix()
{
  Nx=10;
  Ny=10;
  gate=100;
  h_geometry = new TH2I("h_geometry","h_geometry",10,0,10,10,0,10);
  h_hits = new TH2D();
  h_hits->SetNameTitle("HitMatrix","HitMatrix");
}


void HitMatrix::Init()
{
  matrix.clear();
}


void HitMatrix::Clear( int type )
{
  unsigned int i;
  bool setzero=false;

  if(matrix.size()>0)
  {
    for(i=0;i<matrix.size();i++)
    {
      if(setzero==true) i=0;
      if(matrix[i][TYPE]==type)
      {
	matrix.erase(matrix.begin()+i);
	if(i>0){ i--; setzero=false; }
	if(i==0) setzero=true;
      }
      else setzero=false;
    }
  }
}


void HitMatrix::SetGeometry( TH2I* geometry )
{
  h_geometry = geometry;
  Nx = h_geometry->GetNbinsX();
  Ny = h_geometry->GetNbinsY();
}


int HitMatrix::AddHit( int x, int y, double time, int type )
{
  int amplitude=-1;
  int processed=0;
  int good=0;

  hit.clear();

  if(time<=gate && time>=0 && h_geometry->GetBinContent(x+1,y+1)==1)
  {
    hit.push_back(x);
    hit.push_back(y);
    hit.push_back(time);
    hit.push_back(type);
    hit.push_back(amplitude);
    hit.push_back(processed);
    matrix.push_back(hit);
  }
  else good=-1;

  //Sort hit in time:
  int size = matrix.size();
  bool swaped=false;

  if(size!=1 && good!=-1)				//nothing to do for first hit
  {
    int i=1;
    while(size-i>0)					//stop if hit is first in list
    {
      if(hit[TIME]>=matrix[size-i-1][TIME]) break;	//stop if sorted
      i++;
      swaped=true;
    }
    if(swaped==true)
    {
      matrix.insert(matrix.end()-i,hit);
      matrix.erase(matrix.end());
    }
  }

  return good;
}


vector<double> HitMatrix::GetHit( unsigned int iHit )
{
  return matrix[iHit];
}


void HitMatrix::SetAmplitude( unsigned int iHit, double amplitude )
{
  matrix[iHit][AMPLITUDE] = amplitude;
}


void HitMatrix::HitProcessed( unsigned int iHit )
{
  matrix[iHit][PROCESSED] = 1;
}


bool HitMatrix::GetProcessed( unsigned int iHit )
{
  return matrix[iHit][PROCESSED];
}


double HitMatrix::GetPreviousTime( unsigned int iHit )
{
  int i = iHit-1;
  double tlast=-1;

  while(i>=0)
  {
    if(matrix[i][X]==matrix[iHit][X] && matrix[i][Y]==matrix[iHit][Y])
    {
      tlast = matrix[i][TIME];
      break;
    }
    i--;
  }

  return tlast;
}


int HitMatrix::FindHit( vector<double> aHit )
{
  int n=-1;
  unsigned int i;

  for(i=0;i<matrix.size();i++)
  {
    if(aHit[X]==matrix[i][X] && aHit[Y]==matrix[i][Y] && aHit[TIME]==matrix[i][TIME] && aHit[TYPE]==matrix[i][TYPE]) n=i;
  }

  return n;
}


int HitMatrix::nHits(int type)
{
  int n=0;
  unsigned int i;

  for(i=0;i<matrix.size();i++)
  {
    if(matrix[i][TYPE]==type) n++;
  }

  return n;
}


int HitMatrix::nHits()
{
  return matrix.size();
}


TH2D* HitMatrix::DrawMatrix()
{
  c_hitMatrix = new TCanvas("HitMatrix","HitMatrix",700,0,400,400);
  h_hits->Reset("M");
  h_hits->SetBins(Nx,0,Nx,Ny,0,Ny);
  unsigned int i;
  int x,y;

  for(i=0;i<matrix.size();i++)
  {
    x=matrix[i][X]+1;
    y=matrix[i][Y]+1;
    h_hits->SetBinContent(x,y,matrix[i][TYPE]+0.5);
  }

  h_hits->SetMaximum(4);
  h_hits->SetStats(kFALSE);
  h_hits->GetXaxis()->SetNdivisions(Nx);
  h_hits->GetYaxis()->SetNdivisions(Ny);
  h_hits->SetContour(4,0);
  c_hitMatrix->SetGridx();
  c_hitMatrix->SetGridy();
  h_hits->Draw("colz");

  return h_hits;
}


void HitMatrix::PrintMatrix()
{
  cout.precision(3);
  cout << "\nHitMatrix: " << endl;
  cout<<"X\tY\tTime\tType\tAmplitude\tProcessed\n"<<endl;
  unsigned int i;
  for(i=0;i<matrix.size();i++)
  {
    if(matrix[i][TYPE]==PE)
cout<<matrix[i][0]<<"\t"<<matrix[i][1]<<"\t"<<matrix[i][TIME]<<"\tPE\t"<<matrix[i][AMPLITUDE]<<"\t\t"<<matrix[i][PROCESSED]<<endl;
    if(matrix[i][TYPE]==DR)
cout<<matrix[i][0]<<"\t"<<matrix[i][1]<<"\t"<<matrix[i][TIME]<<"\tDR\t"<<matrix[i][AMPLITUDE]<<"\t\t"<<matrix[i][PROCESSED]<<endl;
    if(matrix[i][TYPE]==XT)
cout<<matrix[i][0]<<"\t"<<matrix[i][1]<<"\t"<<matrix[i][TIME]<<"\tXT\t"<<matrix[i][AMPLITUDE]<<"\t\t"<<matrix[i][PROCESSED]<<endl;
    if(matrix[i][TYPE]==AP)
cout<<matrix[i][0]<<"\t"<<matrix[i][1]<<"\t"<<matrix[i][TIME]<<"\tAP\t"<<matrix[i][AMPLITUDE]<<"\t\t"<<matrix[i][PROCESSED]<<endl;
  }
}
