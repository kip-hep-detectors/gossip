#include "sipmGUI.hh"
// #include <TSystem.h>


int main(int argc,char** argv){

  //     gROOT->Macro("$(HOME)/RootStyle/ilcStylePatrick.C");
  
  TApplication app("sipmMC", 0, 0);
  sipmGUI mainWin;
  if(argc == 2) mainWin.ReadParaFile(argv[1]);
  app.Run();



  return 0;
}
