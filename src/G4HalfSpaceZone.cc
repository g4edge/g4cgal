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
            // G4cout << "intersection " << op.second->Inside(p) << G4endl;
        }
        else if (op.first == subtraction) {
            bInside = bInside &! op.second->Inside(p);
            // G4cout << "subtraction " << !op.second->Inside(p) << G4endl;
        }
    }
    return bInside;
}
G4double G4HalfSpaceZone::Distance(const G4ThreeVector& p) const {
    G4double minDist = 1e99;
    for(auto op : _half_spaces) {
        auto d = op.second->Distance(p);
        if (fabs(d) < minDist) {
            minDist = d;
        }
        //G4cout << "G4HalfSpaceZone::Distance> d, minDist " << d << " " << minDist << G4endl;
    }
    return minDist;
}
G4double G4HalfSpaceZone::Distance(const G4ThreeVector& p, const G4ThreeVector& v) const {
    G4double minDist = 1e99;
    for(auto op : _half_spaces) {
        auto d = op.second->Distance(p,v);
        if (d > 0) {
            minDist = std::min(d, minDist);
        }
        //G4cout << "G4HalfSpaceZone::Distance> d, minDist " << d << " " << minDist << G4endl;
    }
    return minDist;
}
Nef_polyhedron_3 G4HalfSpaceZone::GetNefPolyhedron() const {
    return Nef_polyhedron_3(Plane_3(Point_3(0, 0, 0),
                                    Vector_3(0, 0, 0)));
}