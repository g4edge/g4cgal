#include "G4HalfSpaceSolid.hh"
#include "G4HalfSpaceZone.hh"
#include "G4HalfSpacePlane.hh"
#include "G4ThreeVector.hh"
#include "G4ios.hh"

int main(int argc, char** argv)
{
    auto p1 = new G4HalfSpacePlane(G4ThreeVector(1,0,0),G4ThreeVector(1,0,0));
    auto p2 = new G4HalfSpacePlane(G4ThreeVector(-1,0,0),G4ThreeVector(-1,0,0));
    auto p3 = new G4HalfSpacePlane(G4ThreeVector(0,1,0),G4ThreeVector(0,1,0));
    auto p4 = new G4HalfSpacePlane(G4ThreeVector(0,-1,0),G4ThreeVector(0,-1,0));
    auto p5 = new G4HalfSpacePlane(G4ThreeVector(0,0,1),G4ThreeVector(0,0,1));
    auto p6 = new G4HalfSpacePlane(G4ThreeVector(0,0,-1),G4ThreeVector(0,0,-1));

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
    G4cout << "Distance to in> for out " << hss->DistanceToIn(G4ThreeVector(2,0,0)) << G4endl;

    G4cout << "Distance to out> for in " << hss->DistanceToOut(G4ThreeVector(0,0,0)) << G4endl;
    G4cout << "Distance to out> for out " << hss->DistanceToOut(G4ThreeVector(2,0,0)) << G4endl;

}