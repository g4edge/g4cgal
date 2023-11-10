#include "CGALDetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"

#include "G4HalfSpacePlane.hh"
#include "G4HalfSpaceSphere.hh"
#include "G4HalfSpaceAARBox.hh"
#include "G4HalfSpaceZone.hh"
#include "G4HalfSpaceSolid.hh"


#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4BoxInstrumented.hh"

G4VPhysicalVolume* CGALDetectorConstruction::Construct() {
    //////////////////////////////
    // Get nist material manager
    G4NistManager* nist = G4NistManager::Instance();

    //////////////////////////////
    // Envelope parameters

    G4double env_sizeXY = 20*cm, env_sizeZ = 30*cm;
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

#if 0
    auto p1 = new G4HalfSpacePlane(G4ThreeVector(25*mm,0,0),G4ThreeVector(1,0,0));
    auto p2 = new G4HalfSpacePlane(G4ThreeVector(-25*mm,0,0),G4ThreeVector(-1,0,0));
    auto p3 = new G4HalfSpacePlane(G4ThreeVector(0,25*mm,0),G4ThreeVector(0,1,0));
    auto p4 = new G4HalfSpacePlane(G4ThreeVector(0,-25*mm,0),G4ThreeVector(0,-1,0));
    auto p5 = new G4HalfSpacePlane(G4ThreeVector(0,0,25*mm),G4ThreeVector(0,0,1));
    auto p6 = new G4HalfSpacePlane(G4ThreeVector(0,0,-25*mm),G4ThreeVector(0,0,-1));
    auto p7 = new G4HalfSpacePlane(G4ThreeVector(-12.5*mm,-12.5*mm,-12.5*mm), G4ThreeVector(1,1,0));
    auto p8 = new G4HalfSpacePlane(G4ThreeVector(0,-12.5*mm,-12.5*mm), G4ThreeVector(1,0,1));
    auto z = new G4HalfSpaceZone();
    z->AddIntersection(p1);
    z->AddIntersection(p2);
    z->AddIntersection(p3);
    z->AddIntersection(p4);
    z->AddIntersection(p5);
    z->AddIntersection(p6);
    auto hss = new G4HalfSpaceSolid("hsSolid");
    hss->addZone(z);
#endif

    auto b1 = new G4HalfSpaceAARBox(-125*mm, 0*mm,-25*mm, 25*mm, -25*mm, 25*mm);
    auto z = new G4HalfSpaceZone();
    z->AddIntersection(b1);
    auto hss = new G4HalfSpaceSolid("hsSolid");
    hss->addZone(z);

    auto logicHSS = new G4LogicalVolume(hss,
                                        env_mat,
                                        "hss");

    auto physHSS = new G4PVPlacement(nullptr,
                                     G4ThreeVector(),
                                     logicHSS,
                                     "physHSS",
                                      logicWorld,
                                      //nullptr,
                                     false,
                                     0,
                                     false);

    return physWorld;
}