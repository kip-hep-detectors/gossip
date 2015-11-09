// 
// --------------------------------------------------------------
//      Gossip GEANT 4 example - Ex01
// --------------------------------------------------------------

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Ex01DetectorConstruction.hh"
#include "Ex01PhysicsList.hh"
#include "Ex01PrimaryGeneratorAction.hh"
#include "Ex01RunAction.hh"
#include "Ex01EventAction.hh"
#include "Ex01SteppingAction.hh"

int main(int argc,char** argv)
{
  
  // Construct the default run manager
  //
  G4RunManager* runManager = new G4RunManager;

  // set mandatory initialization classes
  //
  Ex01DetectorConstruction* detector = new Ex01DetectorConstruction();
  runManager->SetUserInitialization(detector);

  G4VUserPhysicsList* physics = new Ex01PhysicsList;
  runManager->SetUserInitialization(physics);

  // set mandatory user action class
  //
  G4VUserPrimaryGeneratorAction* gen_action = new Ex01PrimaryGeneratorAction;
  runManager->SetUserAction(gen_action);
  
  Ex01RunAction* run_action = new Ex01RunAction();
  runManager->SetUserAction(run_action);

  Ex01EventAction* event_action = new Ex01EventAction(run_action);
  runManager->SetUserAction(event_action);

  Ex01SteppingAction* stepping_action = new Ex01SteppingAction();
  runManager->SetUserAction(stepping_action);

  // Initialize G4 kernel
  //
  runManager->Initialize();
  
  #ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
  #endif    
  
  // Get the pointer to the User Interface manager
  //
  G4UImanager * UImanager = G4UImanager::GetUIpointer();  
  
  if(argc>1) // run macro :
  {
    G4String command1 = "/control/execute ";
    UImanager->ApplyCommand(command1 + argv[1]);
  }  
  if(argc==1) // interactive mode : define UI session
  {
    #ifdef G4UI_USE
    G4UIExecutive * ui = new G4UIExecutive(argc,argv);
    #ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute vis.mac");
    #endif
    ui->SessionStart();
    delete ui;
    #endif
    
    #ifdef G4VIS_USE
    delete visManager;
    #endif
  }

  delete runManager;
  
  return 0;
}
