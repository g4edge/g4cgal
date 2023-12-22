#include "G4HalfSpaceSolid.hh"
#include "G4SurfaceMeshCGAL.hh"
#include "G4VSceneHandler.hh"

G4HalfSpaceSolid::G4HalfSpaceSolid() : G4VSolid("") {};
G4HalfSpaceSolid::G4HalfSpaceSolid(const char *nameIn) : G4VSolid(nameIn) {};
G4HalfSpaceSolid::G4HalfSpaceSolid(const G4String &nameIn) : G4VSolid(nameIn) {};

G4HalfSpaceSolid::~G4HalfSpaceSolid() {};

G4double G4HalfSpaceSolid::Sdf(const G4ThreeVector &p) const {
    G4double sdf = kInfinity;
    for(auto z : _zones) {
        auto d = z->Sdf(p);
        sdf = std::min(d,sdf);
    }

    return sdf;
}

std::vector<G4ThreeVector> G4HalfSpaceSolid::Intersection(const G4ThreeVector& p, const G4ThreeVector &v) const {

    std::vector<G4ThreeVector> trialInter;
    for (auto z : _zones) {
        auto zoneIntersections = z->Intersection(p,v);
        for(auto zi : zoneIntersections) {
            trialInter.push_back(zi);
        }
    }

    // test intersections
    std::vector<G4ThreeVector> inter;
    for(auto i : trialInter) {
        auto sdf = Sdf(i);
        if(fabs(sdf)< 1e-9) {
            inter.push_back(i);
        }
    }

    return inter;
}

void G4HalfSpaceSolid::Translate(const G4ThreeVector& t) {
    for(auto z : _zones) {
        z->Translate(t);
    }
}

void G4HalfSpaceSolid::Rotate(const G4RotationMatrix& r) {
    for(auto z : _zones) {
        z->Rotate(r);
    }
}

void G4HalfSpaceSolid::Transform(const G4AffineTransform& a) {
    for(auto z : _zones) {
        z->Transform(a);
    }
}

G4SurfaceMeshCGAL* G4HalfSpaceSolid::GetSurfaceMesh() const {

    G4SurfaceMeshCGAL *sm1 = new G4SurfaceMeshCGAL();

    for(auto z : _zones) {
        auto sm2 = z->GetSurfaceMesh();
        G4bool valid;
        sm1 = sm1->Union(sm2, valid);
    }

    return sm1;

}


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

    G4double sdf = Sdf(p);

    if (sdf < -kCarTolerance/2.0) {
        return kInside;
    }
    else if (fabs(sdf) < kCarTolerance/2.0) {
        return kSurface;
    }
    else {
        return kOutside;
    }
}

G4ThreeVector G4HalfSpaceSolid::SurfaceNormal(const G4ThreeVector& p) const {
    return Normal(p,1e-5);
}

G4double G4HalfSpaceSolid::DistanceToIn(const G4ThreeVector& p,
                                        const G4ThreeVector& v) const {
    G4ThreeVector i2;

    if (Inside(p) == EInside::kInside) {
        return 0;
    }

    std::vector<G4ThreeVector> trialInter;
    for (auto z: _zones) {
        auto zoneIntersections = z->Intersection(p, v);
        for (auto zi: zoneIntersections) {
            trialInter.push_back(zi);
        }
    }

    // test intersections
    std::vector<G4ThreeVector> inter;
    for (auto i: trialInter) {
        auto sdf = Sdf(i);
        if (fabs(sdf) < 1e-9) {
            inter.push_back(i);
        }
    }

    // order intersections in distance
    auto g4tvSort = ([p](G4ThreeVector v1, G4ThreeVector v2) {
        return ((v1 - p).mag() <= (v2 - p).mag());
    });
    std::sort(inter.begin(), inter.end(), g4tvSort);

    //if(inter.size() > 0) {
    for(auto i : inter) {
        if (SurfaceNormal(i).dot(v) <= 0) {
            return (i - p).mag();
        }
    }

    return kInfinity;
}

G4double G4HalfSpaceSolid::DistanceToIn(const G4ThreeVector& p) const {

    G4double distToIn = Sdf(p);

    if(distToIn >= 0 ) {
        return distToIn;
    }
    else {
        return 0;
    }
}

G4double G4HalfSpaceSolid::DistanceToOut(const G4ThreeVector& p,
                                         const G4ThreeVector& v,
                                         const G4bool calcNorm,
                                         G4bool* validNorm,
                                         G4ThreeVector* n) const {


    if(Inside(p) == EInside::kOutside) {
        return 0;
    }

    G4ThreeVector i2;

    std::vector<G4ThreeVector> trialInter;
    for (auto z : _zones) {
        auto zoneIntersections = z->Intersection(p,v);
        for(auto zi : zoneIntersections) {
            trialInter.push_back(zi);
        }
    }

    // test intersections
    std::vector<G4ThreeVector> inter;
    for(auto i : trialInter) {
        auto sdf = Sdf(i);
        if(fabs(sdf)< 1e-9) {
            inter.push_back(i);
        }
    }

    // order intersections in distance
    auto g4tvSort = ([p](G4ThreeVector v1, G4ThreeVector v2) {
        return ((v1-p).mag()<=(v2-p).mag());
    });
    std::sort(inter.begin(), inter.end(), g4tvSort);

    for(auto i : inter) {
        if (SurfaceNormal(i).dot(v) >= 0) {
            return (i-p).mag();
        }
    }

    return 0;
}

G4double G4HalfSpaceSolid::DistanceToOut(const G4ThreeVector& p) const {
    G4double distToOut= Sdf(p);

    if(distToOut < 0 ) {
        return fabs(distToOut);
    }
    else {
        return 0;
    }

}

G4GeometryType G4HalfSpaceSolid::GetEntityType() const {
    return G4String("halfSpaceSolid");
}

std::ostream& G4HalfSpaceSolid::StreamInfo(std::ostream& os) const {
    return os;
}

void G4HalfSpaceSolid::DescribeYourselfTo(G4VGraphicsScene& scene) const {
    auto sm = GetSurfaceMesh();
    auto ph = sm->GetG4Polyhedron();
    scene.AddPrimitive(*((G4Polyhedron*)ph));

    return;
}
