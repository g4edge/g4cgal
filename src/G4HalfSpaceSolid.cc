#include "G4HalfSpaceSolid.hh"
#include "G4SurfaceMeshCGAL.hh"
#include "G4VSceneHandler.hh"

G4HalfSpaceSolid::G4HalfSpaceSolid() : G4VSolid("") {};
G4HalfSpaceSolid::G4HalfSpaceSolid(const char *nameIn) : G4VSolid(nameIn) {};
G4HalfSpaceSolid::G4HalfSpaceSolid(const G4String &nameIn) : G4VSolid(nameIn) {};

G4HalfSpaceSolid::~G4HalfSpaceSolid() {};

void G4HalfSpaceSolid::addZone(G4HalfSpaceZone *zone) {_zones.push_back(zone);}
void G4HalfSpaceSolid::removeZone(G4HalfSpaceZone *zone) {_zones.push_back(zone);}

G4int G4HalfSpaceSolid::numberOfZones() { return _zones.size();}

std::vector<G4HalfSpaceSolid*> G4HalfSpaceSolid::connectedSolids() {
    return std::vector<G4HalfSpaceSolid*>();
}

G4bool G4HalfSpaceSolid::CalculateExtent(const EAxis pAxis,
                                         const G4VoxelLimits& pVoxelLimit,
                                         const G4AffineTransform& pTransform,
                                         G4double& pMin, G4double& pMax) const {
    return false;
}

EInside G4HalfSpaceSolid::Inside(const G4ThreeVector& p) const {

    G4cout << "G4HalfSpaceSolid::Inside(" << p << ")>>>>>>>>>>>>>>>>>>>>>>>" << G4endl;

    G4double sdf = kInfinity;
    for(auto z : _zones) {
        auto d = z->Distance(p);
        sdf = std::min(d,sdf);
    }

    G4cout << "G4HalfSpaceSolid::Inside(" << p << ") minDist " << sdf << G4endl;


    if (sdf < -kCarTolerance/2.0) {
        G4cout << "G4HalfSpaceSolid::Inside(" << p << ") inside" << G4endl;
        G4cout << "G4HalfSpaceSolid::Inside(" << p << ")<<<<<<<<<<<<<<<<<<<<<<<" << G4endl;

        return kInside;
    }
    else if (fabs(sdf) < kCarTolerance/2.0) {
        G4cout << "G4HalfSpaceSolid::Inside(" << p << ") surface" << G4endl;
        G4cout << "G4HalfSpaceSolid::Inside(" << p << ")<<<<<<<<<<<<<<<<<<<<<<<" << G4endl;

        return kSurface;
    }
    else {
        G4cout << "G4HalfSpaceSolid::Inside(" << p << ") outside" << G4endl;
        G4cout << "G4HalfSpaceSolid::Inside(" << p << ")<<<<<<<<<<<<<<<<<<<<<<<" << G4endl;

        return kOutside;
    }


}

G4ThreeVector G4HalfSpaceSolid::SurfaceNormal(const G4ThreeVector& p) const {
    return G4ThreeVector(0,0,0);
}

G4double G4HalfSpaceSolid::DistanceToIn(const G4ThreeVector& p,
                                        const G4ThreeVector& v) const {
    G4cout << "G4HalfSpaceSolid::DistanceToIn(" << p << "," << v << ")>>>>>>>>>>>>>>>>>>>>>>>" << G4endl;


    G4double sdf = kInfinity;
    for(auto z : _zones) {
        auto d = z->Distance(p,v);
        G4cout << "G4HalfSpaceSolid::DistanceToIn> zone min dist " << d << G4endl;
        sdf = std::min(d,sdf);
    }
    G4cout << "G4HalfSpaceSolid::DistanceToIn(" << p << "," << v << ") " << fabs(sdf) << G4endl;
    G4cout << "G4HalfSpaceSolid::DistanceToIn(" << p << "," << v << ")<<<<<<<<<<<<<<<<<<<<<<<" << G4endl;

    if(sdf > 0) {
        return fabs(sdf);
    }
    else {
        return kInfinity;
    }
}

G4double G4HalfSpaceSolid::DistanceToIn(const G4ThreeVector& p) const {
    G4cout << "G4HalfSpaceSolid::DistanceToIn(" << p << ")>>>>>>>>>>>>>>>>>>>>>>>" << G4endl;


    G4double sdf = kInfinity;
    for(auto z : _zones) {
        auto d = z->Distance(p);
        G4cout << "G4HalfSpaceSolid::DistanceToIn> zone min dist " << d << G4endl;
        sdf = std::min(d, sdf);
    }
    G4cout << "G4HalfSpaceSolid::DistanceToIn(" << p << ") " << fabs(sdf) << G4endl;
    G4cout << "G4HalfSpaceSolid::DistanceToIn(" << p << ")<<<<<<<<<<<<<<<<<<<<<<<" << G4endl;

    if (sdf >= 0) {
        return fabs(sdf);
    }
    else {
        return kInfinity;
    }
}

G4double G4HalfSpaceSolid::DistanceToOut(const G4ThreeVector& p,
                                         const G4ThreeVector& v,
                                         const G4bool calcNorm,
                                         G4bool* validNorm,
                                         G4ThreeVector* n) const {
    G4cout << "G4HalfSpaceSolid::DistanceToOut(" << p << "," << v << ")>>>>>>>>>>>>>>>>>>>>>>>" << G4endl;

    G4double sdf = kInfinity;
    for(auto z : _zones) {
        auto d = z->DistanceToOut(p,v);
        G4cout << "G4HalfSpaceSolid::DistanceToOut> zone min dist " << d << G4endl;
        sdf = std::min(d, sdf);
    }
    G4cout << "G4HalfSpaceSolid::DistanceToOut(" << p << "," << v << ") " << fabs(sdf) << G4endl;
    G4cout << "G4HalfSpaceSolid::DistanceToOut(" << p << "," << v << ")<<<<<<<<<<<<<<<<<<<<<<<" << G4endl;

    if (sdf <=0) {
        return fabs(sdf);
    }
    else {
        return kInfinity;
    }
}

G4double G4HalfSpaceSolid::DistanceToOut(const G4ThreeVector& p) const {

    G4cout << "G4HalfSpaceSolid::DistanceToOut(" << p << ")>>>>>>>>>>>>>>>>>>>>>>>" << G4endl;

    G4double sdf = kInfinity;
    for(auto z : _zones) {
        auto d = z->Distance(p);
        G4cout << "G4HalfSpaceSolid::DistanceToOut> zone min dist " << d << G4endl;
        sdf = std::min(d,sdf);
    }
    G4cout << "G4HalfSpaceSolid::DistanceToOut(" << p << ") " << fabs(sdf) << G4endl;
    G4cout << "G4HalfSpaceSolid::DistanceToOut(" << p << ")<<<<<<<<<<<<<<<<<<<<<<<" << G4endl;

    return fabs(sdf);
}

G4GeometryType G4HalfSpaceSolid::GetEntityType() const {
    return G4String("halfSpaceSolid");
}

std::ostream& G4HalfSpaceSolid::StreamInfo(std::ostream& os) const {
    return os;
}

void G4HalfSpaceSolid::DescribeYourselfTo(G4VGraphicsScene& scene) const {
    Nef_polyhedron_3 nef(Nef_polyhedron_3::EMPTY);

    for(auto z : _zones) {
        nef += z->GetNefPolyhedron();
    }

    G4SurfaceMeshCGAL sm(nef);
    auto ph = sm.GetG4Polyhedron();

    scene.AddPrimitive(*ph);

    return;
}
