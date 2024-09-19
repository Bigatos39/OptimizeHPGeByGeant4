// This is the program to simulate Gamma ART-6000TM
// Version: 2.0
// Date: 12/14/2021
#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"

#include "./include/MyActionInitialization.hh"
#include "./include/MyDetectorConstruction.hh"
#include "./include/MyPhysicList.hh"

int main(int argc, char **argv) {
  G4RunManager *runManager = new G4RunManager();

  runManager->SetUserInitialization(new MyDetectorConstruction());
  runManager->SetUserInitialization(new MyPhysicList());
  runManager->SetUserInitialization(new MyActionInitialization());
  runManager->Initialize();

  G4UIExecutive *ui = 0;

  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }

  G4VisManager *visManager = new G4VisExecutive();
  visManager->Initialize();

  G4UImanager *UImanager = G4UImanager::GetUIpointer();

  if (ui) {
    UImanager->ApplyCommand("/control/execute defaultMacro.mac");
    ui->SessionStart();
  } else {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  }

  return 0;
}
