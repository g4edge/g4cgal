#include "SurfaceMesh_DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4TessellatedSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4SurfaceMeshCGAL.hh"
#include "G4SurfaceMeshSolid.hh"
#include "G4PrimitiveLocatorG4.hh"
#include "G4PrimitiveLocatorCGAL.hh"
#include "G4SolidTrackingTimer.hh"

G4VPhysicalVolume* SurfaceMesh_DetectorConstruction::Construct() {

  //////////////////////////////
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  //////////////////////////////
  // Envelope parameters
  G4double env_sizeXY = 30*cm, env_sizeZ = 30*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_Al");

  //////////////////////////////
  // World
  G4double world_sizeXY = env_sizeXY;
  G4double world_sizeZ  = env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

  auto solidWorld = new G4Box("World",
                              0.5 * world_sizeXY,
                              0.5 * world_sizeXY,
                              0.5 * world_sizeZ);

  auto logicWorld = new G4LogicalVolume(solidWorld,
                                        world_mat,
                                        "World");

  auto physWorld = new G4PVPlacement(nullptr,
                                     G4ThreeVector(),
                                     logicWorld,
                                     "World",
                                     nullptr,
                                     false,
                                     0,
                                     false);


  //auto g4SolidToTest = new G4Box("g4TestSolid",2*cm, 2*cm, 2*cm);
  auto g4SolidToTest = new G4Orb("g4TestSolid", 2*cm);

  auto smToTest      = new G4SurfaceMeshCGAL();
  auto smLocatorG4   = new G4PrimitiveLocatorG4();
  auto smLocatorCGAL = new G4PrimitiveLocatorCGAL();
  smToTest->Fill(g4SolidToTest->CreatePolyhedron());
  auto smSolidToTest = new G4SurfaceMeshSolid("smTestSolid", smToTest, smLocatorCGAL);

  auto tsSolidToTest = smSolidToTest->GetTessellatedSolid();

  auto logicalToTest = new G4LogicalVolume(smSolidToTest,
                                           env_mat,
                                           "logicalToTest");

  auto physToTest = new G4PVPlacement(nullptr,
                                      G4ThreeVector(),
                                      logicalToTest,
                                      "physicalToTest",
                                      logicWorld,
                                      //nullptr,
                                      false,
                                      0,
                                      false);

  //smSolidToTest->Test(G4ThreeVector(0.0*cm,0.0*cm,-5*cm),
  //                    G4ThreeVector(0,0,1));
  //smSolidToTest->TestRandomInside();

  auto g4TrackingTimer = G4SolidTrackingTimer(g4SolidToTest,
                                             -50,50,
                                             -50,50,
                                             -50,50);
  g4TrackingTimer.Run(100000);
  g4TrackingTimer.Print(1);

  auto smTrackingTimer = G4SolidTrackingTimer(smSolidToTest,
                                              -50,50,
                                              -50,50,
                                              -50,50);
  smTrackingTimer.Run(100000);
  smTrackingTimer.Print(1);

  auto tsTrackingTimer = G4SolidTrackingTimer(tsSolidToTest,
                                              -50,50,
                                              -50,50,
                                              -50,50);
  tsTrackingTimer.Run(100000);
  tsTrackingTimer.Print(1);

  return physWorld;
}