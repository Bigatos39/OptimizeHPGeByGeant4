#ifndef MYDETECTORCONSTRUCTION_HH
#define MYDETECTORCONSTRUCTION_HH

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VUserDetectorConstruction.hh"

#include "G4AutoDelete.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Ellipsoid.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4MultiUnion.hh"
#include "G4NistManager.hh"
#include "G4Orb.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"

#include "MyDetectorMessenger.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction {
public:
  MyDetectorConstruction();
  ~MyDetectorConstruction();

  virtual G4VPhysicalVolume *Construct();

private:
  // virtual void ConstructSDandField();

private:
  // Methods
  void DefineMaterials();
  G4VPhysicalVolume *DefineVolumes();

  G4bool check_overlap = true;
  static G4ThreadLocal G4GlobalMagFieldMessenger *fMagFieldMessenger;

public:
  // Solid volume
  G4Box *solidWorld;
  G4Tubs *solid_Endcap_Outer, *solid_Endcap_Inner, *solid_Ge,
      *solid_Crystal_Hole;

  // Logical volume
  G4LogicalVolume *logicWorld, *logic_Endcap, *logic_Vacuum_Volume,
      *logic_Ge_Crystal, *logic_B_deadLayer, *logic_Li_deadLayer;

  // Physical volume
  G4VPhysicalVolume *physWorld;

  // Material
  G4Material *Air, *Al, *Ge, *Li, *Mylar, *Vacuum, *GeB, *GeLi;

  G4VisAttributes *Steel_Color, *Vacuum_Color, *Tungsten_Color;

  MyDetectorMessenger *detectorMessenger;

public:
  // Get methods
  G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }

private:
  G4LogicalVolume *fScoringVolume;
};

#endif
