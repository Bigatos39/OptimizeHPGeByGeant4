#include "MySteppingAction.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction) {
  fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction() {}

void MySteppingAction::UserSteppingAction(const G4Step *step) {
  G4LogicalVolume *volume = step->GetPreStepPoint()
                                ->GetTouchableHandle()
                                ->GetVolume()
                                ->GetLogicalVolume();

  const MyDetectorConstruction *detectorConstruction =
      static_cast<const MyDetectorConstruction *>(
          G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();

  if (volume != fScoringVolume)
    return;

  G4double edep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edep);

  // Get Position
  G4ThreeVector position = step->GetPreStepPoint()->GetPosition();
  G4double x = position.x();
  G4double y = position.y();
  G4double z = position.z();

  G4ThreeVector momentum = step->GetPreStepPoint()->GetMomentum();
  G4double px = momentum.x();
  G4double py = momentum.y();
  G4double pz = momentum.z();

  // Get event ID
  G4int eventID =
      G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

  // Get Track ID
  G4int trackID = step->GetTrack()->GetTrackID();

  // Pass the values to MyEventAction for storage in ntuple
  fEventAction->AddStepInfo(edep, x, y, z, px, py, pz, eventID, trackID);
}

void MyEventAction::AddStepInfo(G4double edep, G4double x, G4double y,
                                G4double z, G4double px, G4double py,
                                G4double pz, G4int eventID, G4int trackID) {
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  // Fill ntuple with step information
  man->FillNtupleDColumn(0, edep);    // Energy deposition
  man->FillNtupleDColumn(1, x);       // Position x
  man->FillNtupleDColumn(2, y);       // Position y
  man->FillNtupleDColumn(3, z);       // Position z
  man->FillNtupleDColumn(4, px);      // Momentum px
  man->FillNtupleDColumn(5, py);      // Momentum py
  man->FillNtupleDColumn(6, pz);      // Momentum pz
  man->FillNtupleIColumn(7, eventID); // Event ID
  man->FillNtupleIColumn(8, trackID); // Track ID

  // Add row to ntuple
  man->AddNtupleRow();
}
