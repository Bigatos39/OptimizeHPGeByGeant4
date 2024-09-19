#include "MyDetectorConstruction.hh"

MyDetectorConstruction::MyDetectorConstruction() {
  // Messenger to change parameters of the geometry
  detectorMessenger = new MyDetectorMessenger(this);
}

MyDetectorConstruction::~MyDetectorConstruction() { delete detectorMessenger; }

G4VPhysicalVolume *MyDetectorConstruction::Construct() {

  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

void MyDetectorConstruction::DefineMaterials() {
  // NIST
  G4NistManager *nist = G4NistManager::Instance();

  // Air
  Air = nist->FindOrBuildMaterial("G4_AIR");

  // Al
  Al = nist->FindOrBuildMaterial("G4_Al");

  // Li
  Li = nist->FindOrBuildMaterial("G4_Li");

  // Ge
  Ge = nist->FindOrBuildMaterial("G4_Ge");

  // Mylar
  Mylar = nist->FindOrBuildMaterial("G4_MYLAR");

  // Vacuum
  Vacuum = nist->FindOrBuildMaterial("G4_Galactic");
  Vacuum_Color = new G4VisAttributes(G4Colour(1., 0., 0., 0.1));
  Vacuum_Color->SetVisibility(true);
  Vacuum_Color->SetForceSolid(true);

  // GeB
  G4Element *elGe = nist->FindOrBuildElement("Ge");
  G4Element *elB = nist->FindOrBuildElement("B");
  GeB = new G4Material("GeB", 5.32 * g / cm3, 2);
  GeB->AddElement(elGe, 50 * perCent);
  GeB->AddElement(elB, 50 * perCent);

  // GeB
  G4Element *elLi = nist->FindOrBuildElement("Li");
  GeLi = new G4Material("GeB", 5.32 * g / cm3, 2);
  GeLi->AddElement(elGe, 50 * perCent);
  GeLi->AddElement(elLi, 50 * perCent);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

G4VPhysicalVolume *MyDetectorConstruction::DefineVolumes() {
  // ------------------------------------------------------------------------------------------
  // Mother world volume
  // ------------------------------------------------------------------------------------------
  solidWorld = new G4Box("solidWorld", 500 * cm, 500 * cm, 500 * cm);
  logicWorld = new G4LogicalVolume(solidWorld, Air, "logicWorld");
  physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0,
                                false, 0, check_overlap);
  logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

  // ------------------------------------------------------------------------------------------
  // Shield
  // ------------------------------------------------------------------------------------------

  // ------------------------------------------------------------------------------------------
  // Endcap
  // ------------------------------------------------------------------------------------------
  G4RotationMatrix *Rotation1 = new G4RotationMatrix();
  Rotation1->rotateY(0. * deg);
  Rotation1->rotateX(90. * deg);
  Rotation1->rotateZ(0. * deg);

  solid_Endcap_Outer = new G4Tubs("solid_Endcap_Outer", 0., 100 * mm, 130 * mm,
                                  0 * deg, 360 * deg);
  solid_Endcap_Inner = new G4Tubs("solid_Endcap_Inner", 0., 99 * mm, 130 * mm,
                                  0 * deg, 360 * deg);

  G4RotationMatrix rotm = G4RotationMatrix();
  G4ThreeVector position1 = G4ThreeVector(0., 0., 0.);
  G4Transform3D tr1 = G4Transform3D(rotm, position1);
  G4ThreeVector position2 = G4ThreeVector(0., 0., -0.075 * cm);
  G4Transform3D tr2 = G4Transform3D(rotm, position2);
  G4SubtractionSolid *solid_Endcap = new G4SubtractionSolid(
      "solid_Endcap", solid_Endcap_Outer, solid_Endcap_Inner, tr2);

  logic_Endcap = new G4LogicalVolume(solid_Endcap, Al, "logic_Endcap_Outer");
  new G4PVPlacement(Rotation1, G4ThreeVector(0., -6.452 * cm, 0.), logic_Endcap,
                    "logic_Endcap", logicWorld, false, 0, check_overlap);

  // ------------------------------------------------------------------------------------------
  // Vacuum volume
  // ------------------------------------------------------------------------------------------
  logic_Vacuum_Volume =
      new G4LogicalVolume(solid_Endcap_Inner, Vacuum, "logic_Vacuum_Volume");
  new G4PVPlacement(Rotation1, G4ThreeVector(0., -6.527 * cm, 0.),
                    logic_Vacuum_Volume, "phys_Vacuum_Volume", logicWorld,
                    false, 0, check_overlap);

  // ------------------------------------------------------------------------------------------
  // Germanium Crystal
  // ------------------------------------------------------------------------------------------
  G4double radius_Ge = 65.9 * mm / 2;
  G4double radiusHole_Ge = 11.5 * mm / 2;
  G4double height_Ge = 77 * mm;
  G4double holeDepth_Ge = 64.9 * mm;
  G4double depth_Spherical_Cap = 5 * mm;

  solid_Ge = new G4Tubs("solid_Ge", 0., radius_Ge, (height_Ge - 8) / 2 * mm,
                        0. * deg, 360 * deg);
  solid_Crystal_Hole =
      new G4Tubs("solid_Crystal_Hole", 0., radiusHole_Ge,
                 (holeDepth_Ge - radiusHole_Ge) / 2, 0. * deg, 360 * deg);
  G4Sphere *solid_Spherical_Cap =
      new G4Sphere("solid_Spherical_Cap", 0., radiusHole_Ge, 0. * deg,
                   360 * deg, 0. * deg, 90. * deg);
  G4ThreeVector positionSphereCap(0., 0., (holeDepth_Ge - radiusHole_Ge) / 2);
  G4UnionSolid *solid_Hole_With_Spherical_Cap =
      new G4UnionSolid("solid_Hole_With_Spherical_Cap", solid_Crystal_Hole,
                       solid_Spherical_Cap, 0, positionSphereCap);
  G4ThreeVector position3 = G4ThreeVector(0., 0., -4.74 * mm);
  G4SubtractionSolid *solid_Ge_Crystal =
      new G4SubtractionSolid("solid_Ge_Crystal", solid_Ge,
                             solid_Hole_With_Spherical_Cap, 0, position3);
  // make half circle
  G4Sphere *solid_Crystal_halfCircle =
      new G4Sphere("solid_Crystal_halfCircle", 0., radius_Ge, 0.0 * deg,
                   360 * deg, 0.0 * deg, 90.0 * deg);
  G4ThreeVector position_halfCircle(0., 0., height_Ge / 2 - 4 * mm);
  G4Box *solid_subtractHalfCircle = new G4Box(
      "solid_subtractHalfCircle", radius_Ge, radius_Ge, radius_Ge - 6.4 * mm);
  G4SubtractionSolid *solid_subtractBySquareBox = new G4SubtractionSolid(
      "solid_subtractBySquareBox", solid_Crystal_halfCircle,
      solid_subtractHalfCircle, 0, position_halfCircle);
  // total Crystal Ge
  G4UnionSolid *solid_Ge_Crystal_With_Hemisphere =
      new G4UnionSolid("solid_Ge_Crystal_With_Hemisphere", solid_Ge_Crystal,
                       solid_subtractBySquareBox, 0, position_halfCircle);
  logic_Ge_Crystal = new G4LogicalVolume(solid_Ge_Crystal_With_Hemisphere, Ge,
                                         "logic_Ge_Crystal");
  new G4PVPlacement(0, G4ThreeVector(0., 0., 3.183 * cm), logic_Ge_Crystal,
                    "phys_Ge_Crystal", logic_Vacuum_Volume, false, 0,
                    check_overlap);

  // ------------------------------------------------------------------------------------------
  // Ge / B Dead layer: 0.3 micron
  // ------------------------------------------------------------------------------------------
  G4double thickness_B_deadLayer = 0.0003 * mm;
  G4double radiusHole_inner_B = radiusHole_Ge - thickness_B_deadLayer;
  G4double radiusHole_outer_B = radiusHole_Ge;

  G4Tubs *solid_B_deadLayer =
      new G4Tubs("solid_B_deadLayer", radiusHole_inner_B, radiusHole_outer_B,
                 (holeDepth_Ge - radiusHole_Ge) / 2, 0. * deg, 360 * deg);
  G4Sphere *solid_B_deadLayer_Cap =
      new G4Sphere("solid_B_deadLayer_Cap", 0., radiusHole_inner_B, 0.0 * deg,
                   360 * deg, 0.0 * deg, 90.0 * deg);
  G4UnionSolid *solid_B_deadLayer_withCap = new G4UnionSolid(
      "solid_B_deadLayer_withCap", solid_B_deadLayer, solid_B_deadLayer_Cap, 0,
      G4ThreeVector(0.0, 0.0, (holeDepth_Ge - radiusHole_Ge) / 2));
  logic_B_deadLayer =
      new G4LogicalVolume(solid_B_deadLayer_withCap, GeB, "logic_B_deadLayer");
  new G4PVPlacement(0, G4ThreeVector(0., 0., 27 * mm), logic_B_deadLayer,
                    "phys_B_deadLayer", logic_Vacuum_Volume, false, 0,
                    check_overlap);

  // ------------------------------------------------------------------------------------------
  // Ge / Li Dead layer: 0.7 mm
  // ------------------------------------------------------------------------------------------
  G4double thickness_Li_deadLayer = 0.7 * mm;
  G4double radius_Li_deadLayer_inner = radius_Ge;
  G4double radius_Li_deadLayer_outer = radius_Ge + thickness_Li_deadLayer;

  G4Tubs *solid_Li_deadLayer = new G4Tubs(
      "solid_Li_deadLayer", radius_Li_deadLayer_inner,
      radius_Li_deadLayer_outer, (height_Ge - 8) / 2, 0. * deg, 360 * deg);
  G4Sphere *solid_Li_deadLayer_Cap = new G4Sphere(
      "solid_Li_deadLayer_Cap", radius_Li_deadLayer_inner,
      radius_Li_deadLayer_outer, 0.0 * deg, 360 * deg, 0.0 * deg, 90.0 * deg);
  G4Box *solid_Li_deadLayer_Box = new G4Box(
      "solid_Li_deadLayer_Box", radius_Li_deadLayer_outer,
      radius_Li_deadLayer_outer, radius_Li_deadLayer_outer - 6.4 * mm);
  G4SubtractionSolid *solid_Li_deadLayer_Top = new G4SubtractionSolid(
      "solid_Li_deadLayer_Top", solid_Li_deadLayer_Cap, solid_Li_deadLayer_Box,
      0, G4ThreeVector(0.0, 0.0, (height_Ge - 8) / 2));
  G4UnionSolid *solid_Li_deadLayer_withCap = new G4UnionSolid(
      "solid_Li_deadLayer_withCap", solid_Li_deadLayer, solid_Li_deadLayer_Top,
      0, G4ThreeVector(0.0, 0.0, (height_Ge - 8) / 2));
  logic_Li_deadLayer = new G4LogicalVolume(solid_Li_deadLayer_withCap, GeLi,
                                           "logic_Li_deadLayer");
  new G4PVPlacement(0, G4ThreeVector(0., 0., 31.83 * mm), logic_Li_deadLayer,
                    "phys_Li_deadLayer", logic_Vacuum_Volume, false, 0,
                    check_overlap);

  // Set Scoring Volume
  fScoringVolume = logic_Ge_Crystal;

  // ------------------------------------------------------------------------------------------
  // IR 0.03mm Al / 0.03mm MyLar
  // ------------------------------------------------------------------------------------------
  // MyLar
  G4double thickness_Mylar = 0.03 * mm;
  G4double IR_radius = radius_Ge + 4 * mm + 0.8 * mm;
  G4Tubs *solid_Mylar = new G4Tubs("solid_Mylar", 0., IR_radius,
                                   thickness_Mylar / 2.0, 0 * deg, 360 * deg);
  G4LogicalVolume *logic_Mylar =
      new G4LogicalVolume(solid_Mylar, Mylar, "logic_Mylar");
  G4double zPosition_MyLar = height_Ge / 2 + thickness_Mylar / 2 + 4 * mm;
  G4ThreeVector position_Mylar = G4ThreeVector(0., 0., zPosition_MyLar);
  new G4PVPlacement(0, position_Mylar, logic_Mylar, "phys_Mylar",
                    logic_Ge_Crystal, false, 0, check_overlap);
  // Al
  G4double thickness_Al = 0.03 * mm;
  G4Tubs *solid_Al = new G4Tubs("solid_Al", 0., IR_radius, thickness_Al / 2.0,
                                0 * deg, 360 * deg);
  G4LogicalVolume *logic_Al = new G4LogicalVolume(solid_Al, Al, "logic_Al");
  G4double zPositionAl =
      height_Ge / 2 + thickness_Mylar / 2 + 4 * mm + thickness_Al / 2;
  G4ThreeVector position_Al = G4ThreeVector(0., 0., zPositionAl);
  new G4PVPlacement(0, position_Al, logic_Al, "phys_Al", logic_Ge_Crystal,
                    false, 0, check_overlap);

  // ------------------------------------------------------------------------------------------
  // Al Cover 0.8 mm
  // ------------------------------------------------------------------------------------------
  G4double cupLength = 105 * mm;
  G4double thickness_Al_cover = 0.8 * mm;
  G4double outer_radius_Al =
      radius_Ge + 4 * mm + thickness_Al_cover + thickness_Li_deadLayer;
  G4double inner_radius_Al = radius_Ge + 4 * mm + thickness_Li_deadLayer;
  G4double height_Al_cover = cupLength - 3 * mm;

  G4Tubs *solid_Al_cover =
      new G4Tubs("solid_Al", inner_radius_Al, outer_radius_Al,
                 height_Al_cover / 2, 0. * deg, 360. * deg);
  G4LogicalVolume *logic_Al_cover =
      new G4LogicalVolume(solid_Al_cover, Al, "logic_Al_cover");
  G4double zPosition_Al_cover =
      height_Ge / 2 -
      (height_Al_cover - height_Ge - thickness_Al - thickness_Mylar) / 2 -
      2.7 * mm;
  G4ThreeVector position_Al_cover(0., 0., zPosition_Al_cover);
  new G4PVPlacement(0, position_Al_cover, logic_Al_cover, "phys_Al_cover",
                    logic_Vacuum_Volume, false, 0, check_overlap);

  // ------------------------------------------------------------------------------------------
  // Double half circle: 3 mm Al bottom
  // ------------------------------------------------------------------------------------------
  G4double thicknessBottom_Al = 3 * mm;
  G4double radius_Al_bottom =
      radius_Ge + 4 * mm + thickness_Al_cover; // radius_Ge + space(4mm)
  G4Tubs *solid_circleBottom_Al =
      new G4Tubs("circleBottom_Al", 0., radius_Al_bottom, thicknessBottom_Al,
                 0 * deg, 360 * deg);
  // make hole in bottom Al
  G4double height_Rectangle_Hole = radius_Al_bottom * 2;
  G4Box *solid_Rectangle_Hole =
      new G4Box("solid_Rectangle_Hole", radiusHole_Ge / 2,
                height_Rectangle_Hole / 2, thicknessBottom_Al + 1);
  G4ThreeVector positionRectangle_Hole(0, 0, 0);
  G4SubtractionSolid *solid_CircleBottom_With_Hole = new G4SubtractionSolid(
      "solid_CircleBottom_With_Hole", solid_circleBottom_Al,
      solid_Rectangle_Hole, 0, positionRectangle_Hole);
  G4double zPosition_Al_bottom = -(height_Ge / 2 - 7.8);
  G4ThreeVector positionBottom_Al(0., 0., zPosition_Al_bottom);
  G4LogicalVolume *logic_CircleBottom_With_Hole = new G4LogicalVolume(
      solid_CircleBottom_With_Hole, Al, "logic_CircleBottom_With_Hole");
  new G4PVPlacement(0, positionBottom_Al, logic_CircleBottom_With_Hole,
                    "phys_Al_bottom_With_Hole", logic_Vacuum_Volume, false, 0,
                    check_overlap);

  // Set Colors -- (const alpha = 0.1)
  // Endcap
  G4VisAttributes *vis_Endcap =
      new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.1)); // gray
  vis_Endcap->SetForceSolid(true);
  vis_Endcap->SetForceAuxEdgeVisible(true);
  logic_Endcap->SetVisAttributes(vis_Endcap);

  // Germanium Crystal
  G4VisAttributes *vis_Ge_Crystal =
      new G4VisAttributes(G4Colour(0.5, 0.0, 0.5, 0.1)); // dark puple
  vis_Ge_Crystal->SetForceSolid(true);
  logic_Ge_Crystal->SetVisAttributes(vis_Ge_Crystal);

  // B in Hole Crystal
  G4VisAttributes *vis_Hole_B =
      new G4VisAttributes(G4Colour(1, 0.0, 0.0, 0.1)); // red
  vis_Hole_B->SetForceSolid(true);
  logic_B_deadLayer->SetVisAttributes(vis_Hole_B);

  // Li in dead Layer
  G4VisAttributes *vis_deadLayer_Li =
      new G4VisAttributes(G4Colour(0.0, 0.0, 1, 0.1)); // blue
  vis_deadLayer_Li->SetForceSolid(true);
  logic_Li_deadLayer->SetVisAttributes(vis_deadLayer_Li);

  // MyLar top
  G4VisAttributes *vis_MyLar =
      new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.1)); // green
  vis_MyLar->SetForceSolid(true);
  logic_Mylar->SetVisAttributes(vis_MyLar);

  // Al top
  G4VisAttributes *vis_Al =
      new G4VisAttributes(G4Colour(0.75, 0.75, 0.75, 0.1));
  vis_Al->SetForceSolid(true);
  logic_Al->SetVisAttributes(vis_Al);

  // Al cover
  G4VisAttributes *vis_Al_cover =
      new G4VisAttributes(G4Colour(0.75, 0.75, 0.75, 0.1));
  vis_Al_cover->SetForceSolid(true);
  logic_Al_cover->SetVisAttributes(vis_Al_cover);

  G4VisAttributes *vis_Al_bottom =
      new G4VisAttributes(G4Colour(0.75, 0.75, 0.75, 0.1));
  vis_Al_bottom->SetForceSolid(true);
  logic_CircleBottom_With_Hole->SetVisAttributes(vis_Al_bottom);

  return physWorld;
}
