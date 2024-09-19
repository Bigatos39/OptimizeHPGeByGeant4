#include "MyRunAction.hh"
#include "MyDetectorConstruction.hh"
#include "MyPrimaryGenerator.hh"

#include "G4AccumulableManager.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

MyRunAction::MyRunAction() {
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  // Creating histograms
  man->CreateH1("Eabs", "Edep in absorber", 8188, 2.011E-3 * MeV,
                2.742235 * MeV);

  // Creating ntuple
  //
  man->CreateNtuple("HPGe", "Edep");
  man->CreateNtupleDColumn("fEdep");
  man->CreateNtupleDColumn("x");       // Position x
  man->CreateNtupleDColumn("y");       // Position y
  man->CreateNtupleDColumn("z");       // Position z
  man->CreateNtupleDColumn("px");      // Momentum px
  man->CreateNtupleDColumn("py");      // Momentum py
  man->CreateNtupleDColumn("pz");      // Momentum pz
  man->CreateNtupleIColumn("eventID"); // Event ID
  man->CreateNtupleIColumn("trackID"); // Track ID
  man->FinishNtuple();
}

MyRunAction::~MyRunAction() { delete G4AnalysisManager::Instance(); }

void MyRunAction::BeginOfRunAction(const G4Run *run) {
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->SetNtupleMerging(true);

  G4int runID = run->GetRunID();

  std::stringstream strRunID;
  strRunID << runID;

  man->OpenFile("output" + strRunID.str() + ".root");
  // man->OpenFile("output" + strRunID.str() + ".csv");
}

void MyRunAction::EndOfRunAction(const G4Run *run) {
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  // save histograms & ntuple
  //
  man->Write();
  man->CloseFile();
}
