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
    G4double sdf = Distance(p);

    if(sdf > 0) {
        return false;
    }
    else {
        return true;
    }
}

G4double G4HalfSpaceZone::Distance(const G4ThreeVector &p) const {

    G4cout << "G4HalfSpaceZone::Distance(" << p << ")======================" << G4endl;

    G4double sdf = -9e99;
    for(auto op : _half_spaces) {

        G4double d = 9e99;

        if (op.first == intersection) {
            d = op.second->Distance(p);
            sdf = std::max(d, sdf);
        }
        else {
            d = -op.second->Distance(p);
            sdf = std::max(d, sdf);
        }
        G4cout << "G4HalfSpaceZone::Distance(" << p << ") dist " << d << " " << sdf << G4endl;

    }
    return sdf;
}

G4double G4HalfSpaceZone::Distance(const G4ThreeVector& p, const G4ThreeVector& v) const {

    G4double sdf = -9e99;

    for(auto op : _half_spaces) {
        G4double d;
        if(op.first == intersection) {
            d = op.second->Distance(p, v);
            if(d != 9e99) {
                sdf = std::max(d, sdf);
            }
        }
        else {
            d = op.second->Distance(p, v);
            if(d != 9e99) {
                sdf = std::max(-d,sdf);
            }
        }

        G4cout << "G4HalfSpaceZone::Distance(" << p << "," << v << ") hs dist " << d << " " << sdf << G4endl;
    }

    auto pTest = p + sdf*v;
    auto pDist = Distance(pTest);
    G4cout << "G4HalfSpaceZone::Distance(" << p << "," << v << ") trial intersection dist " << pDist << G4endl;

    if(pDist > 0) {
        return 9e99;
    }
    else {
        return sdf;
    }
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