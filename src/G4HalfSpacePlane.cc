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

G4bool G4HalfSpacePlane::Inside(const G4ThreeVector& p) const {
    if (Distance(p) > 0) {
        return false;
    }
    else {
        return true;
    }
}

G4double G4HalfSpacePlane::Distance(const G4ThreeVector &p) const {
    return (p-_p0).dot(_n);
}

G4double G4HalfSpacePlane::Distance(const G4ThreeVector &p,
                                    const G4ThreeVector &d) const {

    auto dNorm = d/d.mag();
    auto dDenom = dNorm.dot(_n);
    auto lambda = _p0.dot(_n)/ dNorm.dot(_n);

    auto dist = lambda/fabs(lambda)*(lambda * dNorm + p).mag();
    return dist;
}

Nef_polyhedron_3 G4HalfSpacePlane::GetNefPolyhedron() const {
    return Nef_polyhedron_3(Plane_3(Point_3(_p0.x(), _p0.y(), _p0.z()),
                                    Vector_3(_n.x(), _n.y(), _n.z())));
}

