#include "G4HalfSpaceZone.hh"

#include <algorithm>
#include <ranges>
#include <tuple>

G4HalfSpaceZone::G4HalfSpaceZone() {};
G4HalfSpaceZone::~G4HalfSpaceZone() {};

void G4HalfSpaceZone::AddIntersection(G4VHalfSpace *hs) {
    _half_spaces.push_back(std::make_pair(intersection, hs));
};

void G4HalfSpaceZone::AddSubtraction(G4VHalfSpace *hs) {
    _half_spaces.push_back(std::make_pair(subtraction, hs));
};

G4double G4HalfSpaceZone::Sdf(const G4ThreeVector &p) const {

    G4double sdf = -DBL_MAX;
    for(auto op : _half_spaces) {

        G4double d;

        if (op.first == intersection) {
            d = op.second->Sdf(p);
            sdf = std::max(d, sdf);
        }
        else {
            d = op.second->Sdf(p);
            sdf = std::max(-d, sdf);
        }
    }
    return sdf;
}

std::vector<G4ThreeVector> G4HalfSpaceZone::Intersection(const G4ThreeVector& p, const G4ThreeVector& v) const {

    std::vector<G4ThreeVector> trialInter;

    // create intersections
    for(auto op: _half_spaces) {
        G4ThreeVector i2;
        auto intersections = op.second->Intersection(p, v);
        for(auto i : intersections) {
            trialInter.push_back(i);
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

    return inter;

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