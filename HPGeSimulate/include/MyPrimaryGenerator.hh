#ifndef MYPRIMARYGENERATOR_HH
#define MYPRIMARYGENERATOR_HH

#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
  MyPrimaryGenerator();
  ~MyPrimaryGenerator();

  virtual void GeneratePrimaries(G4Event *);

private:
  G4GeneralParticleSource *particleSource;
};

#endif
