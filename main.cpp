#include "gossipGUI.h"

#include "TRint.h"

int main(int argc,char** argv)
{
  
  TRint app("sipmMC", 0, 0);
  
  gossipGUI gui;
  gui.BuildMainFrame();
  if(argc == 2) gui.ReadParaFile(argv[1]);

  app.Run();

  return 0;
}
