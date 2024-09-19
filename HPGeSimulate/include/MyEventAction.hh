#ifndef MYENVENTACTION_HH
#define MYENVENTACTION_HH

#include "G4UserEventAction.hh"

#include "MyAnalysis.hh"
#include "MyRunAction.hh"

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

#include "G4AnalysisManager.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"
#include "globals.hh"
#include <iomanip>

class MyEventAction : public G4UserEventAction {
public:
  MyEventAction();
  virtual ~MyEventAction();

  virtual void BeginOfEventAction(const G4Event *event);
  virtual void EndOfEventAction(const G4Event *event);

  void AddEdep(G4double edep) { fEdep += edep; }
  void AddStepInfo(G4double edep, G4double x, G4double y, G4double z,
                   G4double px, G4double py, G4double pz, G4int eventID,
                   G4int trackID);

private:
  G4double fEdep;
};

#endif
