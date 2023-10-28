#include "CGALDetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4HalfSpaceSolid.hh"
#include "G4HalfSpacePlane.hh"
#include "G4HalfSpaceZone.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

G4VPhysicalVolume* CGALDetectorConstruction::Construct() {
    //////////////////////////////
    // Get nist material manager
    G4NistManager* nist = G4NistManager::Instance();

    //////////////////////////////
    // Envelope parameters

    G4double env_sizeXY = 20*cm, env_sizeZ = 30*cm;
    G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");

    //////////////////////////////
    // World
    G4double world_sizeXY = 1.2*env_sizeXY;
    G4double world_sizeZ  = 1.2*env_sizeZ;
    G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

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


    auto p1 = new G4HalfSpacePlane(G4ThreeVector(25*mm,0,0),G4ThreeVector(1,0,0));
    auto p2 = new G4HalfSpacePlane(G4ThreeVector(-25*mm,0,0),G4ThreeVector(-1,0,0));
    auto p3 = new G4HalfSpacePlane(G4ThreeVector(0,25*mm,0),G4ThreeVector(0,1,0));
    auto p4 = new G4HalfSpacePlane(G4ThreeVector(0,-25*mm,0),G4ThreeVector(0,-1,0));
    auto p5 = new G4HalfSpacePlane(G4ThreeVector(0,0,25*mm),G4ThreeVector(0,0,1));
    auto p6 = new G4HalfSpacePlane(G4ThreeVector(0,0,-25*mm),G4ThreeVector(0,0,-1));

    auto z = new G4HalfSpaceZone();
    z->AddIntersection(p1);
    z->AddIntersection(p2);
    z->AddIntersection(p3);
    z->AddIntersection(p4);
    z->AddIntersection(p5);
    z->AddIntersection(p6);

    auto hss = new G4HalfSpaceSolid("hsSolid");
    hss->addZone(z);

    auto inside = z->Inside(G4ThreeVector(0,0,0));
    auto distance1 =  z->Distance(G4ThreeVector(0,0,0));
    auto distance2 = z->Distance(G4ThreeVector(0,0,0),G4ThreeVector(1, 1,0));
    G4cout << inside << " " << distance1 << " " << distance2 << G4endl;

    G4cout << "Inside> for in " << hss->Inside(G4ThreeVector(0.9,0,0)) << G4endl;
    G4cout << "Inside> for surface " << hss->Inside(G4ThreeVector(1.0,0,0)) << G4endl;
    G4cout << "Inside> for out " << hss->Inside(G4ThreeVector(1.1,0,0)) << G4endl;

    G4cout << "Distance to in> for in " << hss->DistanceToIn(G4ThreeVector(0,0,0)) << G4endl;
    G4cout << "Distance to in> for out " << hss->DistanceToIn(G4ThreeVector(10,10,10)) << G4endl;

    G4cout << "Distance to out> for in " << hss->DistanceToOut(G4ThreeVector(0,0,0)) << G4endl;
    G4cout << "Distance to out> for out " << hss->DistanceToOut(G4ThreeVector(2,0,0)) << G4endl;

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