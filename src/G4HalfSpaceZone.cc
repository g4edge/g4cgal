#include "G4HalfSpaceZone.hh"

#include <algorithm>

G4HalfSpaceZone::G4HalfSpaceZone() {};
G4HalfSpaceZone::~G4HalfSpaceZone() {};

void G4HalfSpaceZone::AddIntersection(G4VHalfSpace *hs) {
    _half_spaces.push_back(std::make_pair(intersection, hs));
};
void G4HalfSpaceZone::AddSubtraction(G4VHalfSpace *hs) {
    _half_spaces.push_back(std::make_pair(subtraction, hs));
};

G4bool G4HalfSpaceZone::Inside(const G4ThreeVector& p) const {
    G4bool bInside = true;
    for(auto op : _half_spaces) {
        if (op.first == intersection) {
            bInside = bInside & op.second->Inside(p);
        }
        else if (op.first == subtraction) {
            bInside = bInside &! op.second->Inside(p);
        }
    }
    return bInside;
}

G4double G4HalfSpaceZone::Distance(const G4ThreeVector &p) const {
    G4double minDist = 9e99;
    for(auto op : _half_spaces) {
        auto d = op.second->Distance(p);
        if(fabs(d) < minDist) {
            minDist = d;
        }
    }
    return minDist;
}

G4double G4HalfSpaceZone::DistanceToIn(const G4ThreeVector& p) const {
    G4double minDist = 9e99;
    for(auto op : _half_spaces) {
        auto d = op.second->DistanceToIn(p);
        G4cout << "G4HalfSpaceZone::DistanceToIn(" << p << ") hs dist " << d << G4endl;
        minDist = std::min(d, minDist);
    }
    return minDist;
}

G4double G4HalfSpaceZone::DistanceToOut(const G4ThreeVector& p) const {
    G4double minDist = 9e99;
    for(auto op : _half_spaces) {
        auto d = op.second->DistanceToOut(p);
        G4cout << "G4HalfSpaceZone::DistanceToOut(" << p << ") hs dist " << d << G4endl;
        minDist = std::min(d, minDist);
    }
    return minDist;
}

G4double G4HalfSpaceZone::DistanceToIn(const G4ThreeVector& p, const G4ThreeVector& v) const {
    G4double minDist = 9e99;
    for(auto op : _half_spaces) {
        auto d = op.second->DistanceToIn(p,v);
        G4cout << "G4HalfSpaceZone::DistanceToIn(" << p << "," << v << ") hs dist " << d << G4endl;
        minDist = std::min(d, minDist);
    }
    return minDist;
}

G4double G4HalfSpaceZone::DistanceToOut(const G4ThreeVector& p, const G4ThreeVector& v) const {
    G4double minDist = 9e99;
    for(auto op : _half_spaces) {
        auto d = op.second->DistanceToOut(p,v);
        minDist = std::min(d, minDist);
    }
    return minDist;
}

Nef_polyhedron_3 G4HalfSpaceZone::GetNefPolyhedron() const {

    Nef_polyhedron_3 nef(Nef_polyhedron_3::COMPLETE);

    for(auto op : _half_spaces) {
        if(op.first == intersection) {
            nef *= op.second->GetNefPolyhedron();
        }
        else if(op.first == subtraction) {
            nef -= op.second->GetNefPolyhedron();
        }
    }

    return nef;
}