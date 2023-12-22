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

void G4HalfSpaceZone::Translate(const G4ThreeVector& t) {
    for(auto op : _half_spaces) {
        op.second->Translate(t);
    }
}

void G4HalfSpaceZone::Rotate(const G4RotationMatrix& r) {
    for(auto op : _half_spaces) {
        op.second->Rotate(r);
    }
}

void G4HalfSpaceZone::Transform(const G4AffineTransform& a) {
    for(auto op : _half_spaces) {
        op.second->Transform(a);
    }
}

G4SurfaceMeshCGAL* G4HalfSpaceZone::GetSurfaceMesh() const {

    G4SurfaceMeshCGAL *sm1 = new G4SurfaceMeshCGAL();

    G4bool first = true;
    for(auto op : _half_spaces) {
        if(op.first == intersection) {
            auto sm2 =  op.second->GetSurfaceMesh();
            G4cout << "hs intersection " << sm2->NumberOfVertices() << " " << sm2->NumberOfFaces() << G4endl;
            if (first) {
                G4bool valid = false;
                sm1 = sm1->Union(sm2,valid);
                first = false;
            }
            else {
                G4bool valid = false;
                sm1 = sm1->Intersection(sm2, valid);
            }
        }
        else if(op.first == subtraction) {
            auto sm2 = op.second->GetSurfaceMesh();
            G4cout << "hs subtraction " << sm2->NumberOfVertices() << " " << sm2->NumberOfFaces() << G4endl;
            if (first) {
                G4bool valid = false;
                sm1 = sm1->Union(sm2, valid);
                first = false;
            }
            else {
                G4bool valid = false;
                sm1 = sm1->Subtraction(sm2, valid);
            }
        }
    }

    G4cout << "zone final subtraction " << sm1->NumberOfVertices() << " " << sm1->NumberOfFaces() << G4endl;

    return sm1;
}