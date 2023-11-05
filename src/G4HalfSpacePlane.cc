#include "G4HalfSpacePlane.hh"
#include "G4VSolid.hh"


G4HalfSpacePlane::G4HalfSpacePlane() {
};

G4HalfSpacePlane::G4HalfSpacePlane(const G4ThreeVector &p0, const G4ThreeVector &n) : _p0(p0), _n(n)
{}

G4HalfSpacePlane::G4HalfSpacePlane(G4double a, G4double b, G4double c, G4double d) {
    _n = G4ThreeVector(a,b,c);
    _n = _n/_n.mag();
    _p0 = _n*d;
};

G4HalfSpacePlane::~G4HalfSpacePlane() {};

G4double G4HalfSpacePlane::Sdf(const G4ThreeVector &p) const {
    G4double dist = (p - _p0).dot(_n);
    return dist;
}

std::vector<G4ThreeVector> G4HalfSpacePlane::Intersection(const G4ThreeVector& p, const G4ThreeVector &v) const {
    std::vector<G4ThreeVector> intersections;

    auto vNorm = v/v.mag();
    auto dDenom = vNorm.dot(_n);
    auto lambda = (_p0-p).dot(_n)/ dDenom;

    if(dDenom != 0 && lambda >= 0)
        intersections.push_back(lambda*v+p);

    return intersections;
}

Nef_polyhedron_3 G4HalfSpacePlane::GetNefPolyhedron() const {
    return Nef_polyhedron_3(Plane_3(Point_3(_p0.x(), _p0.y(), _p0.z()),
                            Direction_3(_n.x(), _n.y(), _n.z())));
}

