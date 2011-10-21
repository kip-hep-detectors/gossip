/********** Version 1.2 **********/

#include "HitMatrix.h"
#include <TCanvas.h>
#include <TH2D.h>
#include <vector>
#include <iostream>

using namespace std;


//QuickSort
/*
int partition(vector<vector<double> >& arr, int start, int end){
   
    double pivot = arr[end][3];                                       
    double bottom = start-1;                                                   
    double top = end;                                                           

    bool notdone = true;
    while (notdone){
        while (notdone){
            bottom += 1;                  

            if (bottom == top)   {
                notdone = false;                        
                break;
            }
            if (arr[bottom][3] > pivot){
                arr[top][3] = arr[bottom][3];       
                break;
            }
        }  
        while (notdone){
            top = top-1;                      
            
            if (top == bottom) {
                notdone = false;                      
                break;
            }
            if (arr[top][3] < pivot){   
                arr[bottom][3] = arr[top][3];
                break;
            }      
        }
    }
    arr[top][3] = pivot;                          
    return top;
}

int quickSort(vector<vector<double> >& arr, int start, int end){
  
        if (start < end){
             int split = partition(arr, start, end);
             quickSort(arr, start, split-1);         
             quickSort(arr, split+1, end); 
        }
        
        else return 0;
}
*/

//HitMatrix
HitMatrix::HitMatrix(){
  gcut=1;
}


void HitMatrix::Init(int type){

  unsigned int i;
  bool setzero=false;

  if(type==-1){
    matrix.clear();
  }

  if(type!=-1 && matrix.size()>0){
    for(i=0;i<matrix.size();i++){
      if(setzero==true) i=0;
      if(matrix[i][2]==type){
        matrix.erase(matrix.begin()+i);
        if(i>0){ i--; setzero=false; }
        if(i==0) setzero=true;
      }
      else setzero=false;
    }
  }

}


void HitMatrix::PrintMatrix(){
	
	cout << "HitMatrix: " << endl;
	for(int i=0;i<matrix.size();i++){
		cout << matrix[i][0] << "\t" << matrix[i][1] << "\t" << matrix[i][2] << "\t" << matrix[i][3] << "\t\t" << matrix[i][4] << endl;
	}
	cout << "\n" << endl;
	
}

int HitMatrix::AddHit( int x, int y, int type, double time ){

	int amplitude=1;
	int good=0;

	if(gcut==true){
		if(time<=gate && x>=0 && x<Nx && y>=0 && y<Ny){
			event.clear();
			event.push_back(x);
			event.push_back(y);
			event.push_back(type);
			event.push_back(time);
			event.push_back(amplitude);
			matrix.push_back(event);
		}
		if(x<0 || x>=Nx || y<0 || y>=Ny) good=-1;
	}

	if(gcut==false){
		if(x>=0 && x<Nx && y>=0 && y<Ny){
			event.clear();
			event.push_back(x);
			event.push_back(y);
			event.push_back(type);
			event.push_back(time);
			event.push_back(amplitude);
			matrix.push_back(event);
		}
		else good=-1;
	}
  
	return good;
}


void HitMatrix::TSortLastEvent( unsigned int iseed ){
  
    event.clear();
    event=matrix[matrix.size()-1];

    if(event[2]==2){
      matrix.insert(matrix.begin()+iseed+1,event);
      matrix.erase(matrix.end());
    }
    
    if(event[2]==3 || event[2]==1){
      unsigned int n=0;
      while(1){
	if(event[3]<=matrix[iseed+n][3]) break;
	n++;
      }
      matrix.insert(matrix.begin()+iseed+n,event);
      matrix.erase(matrix.end());      
    }
    
}


int HitMatrix::FindEvent( vector<double> evnt ){

  int n=-1;
  unsigned int i;

  for(i=0;i<matrix.size();i++){
    if(evnt[0]==matrix[i][0] && evnt[1]==matrix[i][1] && evnt[2]==matrix[i][2] && evnt[3]==matrix[i][3]) n=i;
  }

  return n;
}


int HitMatrix::nFind( int x, int y, int type, double time ){

  int n=0;
  unsigned int i;
  int c;

  if(type!=-1 && time!=-1) c=0;
  if(type!=-1 && time==-1) c=1;
  if(type==-1 && time!=-1) c=2;
  if(type==-1 && time==-1) c=3;

  for(i=0;i<matrix.size();i++){
    switch(c){
    case 0: if(matrix[i][0]==x && matrix[i][1]==y && matrix[i][2]==type && matrix[i][3]==time) n++;
            break;
    case 1: if(matrix[i][0]==x && matrix[i][1]==y && matrix[i][2]==type) n++;
            break;
    case 2: if(matrix[i][0]==x && matrix[i][1]==y && matrix[i][3]==time) n++;
            break;
    case 3: if(matrix[i][0]==x && matrix[i][1]==y) n++;
            break;
    }
  }

  return n;
}


vector<double> HitMatrix::GetEvent( int i ){

  return matrix[i];
}


double HitMatrix::GetPreviousTime( vector<double> event ){

  unsigned int i;
  double tlast=-1;

  for(i=0;i<matrix.size();i++){
    if(matrix[i][0]==event[0] && matrix[i][1]==event[1]){
      if(matrix[i][3]<event[3]){
        if(tlast==-1){ tlast=matrix[i][3]; }
        else { if(matrix[i][3]>tlast) tlast=matrix[i][3]; }
      }
    }
  }

  return tlast;
}


int HitMatrix::nHits(int type){
  
  int n=0;
  unsigned int i;

  if(type==-1){
    n=matrix.size();
  }


  if(type!=-1){
    for(i=0;i<matrix.size();i++){
      if(matrix[i][2]==type) n++;
    }
  }
  return n;
}


void HitMatrix::TSort(int istart){

  unsigned int i;
  unsigned int j;

  if(matrix.size()>1){
    //quickSort(matrix,istart,matrix.size()-1);
    
//bubble sort
    for(i=matrix.size()-1;i>0;i--){
      for(j=istart;j<i;j++){
        if(matrix[j][3]>matrix[j+1][3]){
          event=matrix[j];
          matrix[j] = matrix[j+1];
          matrix[j+1] = event;
        }
      }
    }
  }

}


void HitMatrix::DrawMatrix( int type){
  TCanvas *c = new TCanvas("HitMatrix","HitMatrix",0,560,400,400);
  TH2D *HitHisto = new TH2D("HitHisto","HitHisto",Nx,0,Nx,Ny,0,Ny);
  unsigned int i;
  int x,y;

  for(i=0;i<matrix.size();i++){
    x=matrix[i][0]+1;
    y=matrix[i][1]+1;
    if(matrix[i][2]==type) HitHisto->SetBinContent(x,y,type+0.5);
    if(type==-1) HitHisto->SetBinContent(x,y,matrix[i][2]+0.5);
  }
  HitHisto->SetMaximum(4);
  HitHisto->SetStats(kFALSE);
  HitHisto->GetXaxis()->SetNdivisions(Nx);
  HitHisto->GetYaxis()->SetNdivisions(Ny);
  HitHisto->SetContour(4,0);
  c->SetGridx();
  c->SetGridy();
  HitHisto->Draw("colz");
}
