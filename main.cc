#include "sipmGUI.hh"

int main(int argc,char** argv)
{
  
  TApplication app("sipmMC", 0, 0);
  sipmGUI mainWin;
  if(argc == 2) mainWin.ReadParaFile(argv[1]);
  app.Run();

  return 0;
}
