#include "G4HalfSpaceSphere.hh"

G4HalfSpaceSphere::G4HalfSpaceSphere() {}
G4HalfSpaceSphere::G4HalfSpaceSphere(const G4ThreeVector& p0, G4double radius) : _p0(p0), _r(radius) {}

G4HalfSpaceSphere::~G4HalfSpaceSphere() {}

G4double G4HalfSpaceSphere::Sdf(const G4ThreeVector&p) const {
    return (p - _p0).mag() - _r;
}

std::vector<G4ThreeVector> G4HalfSpaceSphere::Intersection(const G4ThreeVector& p, const G4ThreeVector& v) const {
    // v = lambda d + p
    // |(v - p0)| = r
    // | lambda d + p - p0| = r
    // (lambda d + p-p0) . (lambda d p-p0) = r.r
    // lambda^2 d.d + 2 lambda d.(p-p0) + (p-p0).(p-p0) = r.r
    // a = d.d
    // b = 2 d.(p-p0)
    // c = (p-p0).(p-p0) - r*r

    std::vector<G4ThreeVector> intersections;

    auto a = v.dot(v);
    auto b = 2*v.dot(p-_p0);
    auto c = (p-_p0).dot(p-_p0) - _r*_r;

    G4int nSoln = 0;
    G4double lambda1 = 0;
    G4double lambda2 = 0;
    G4VHalfSpace::QuadraticSolve(a,b,c,nSoln, lambda1, lambda2);
    intersections.push_back(lambda1*v + p);
    intersections.push_back(lambda2*v + p);

    return intersections;
}

Nef_polyhedron_3 G4HalfSpaceSphere::GetNefPolyhedron() const {
    return Nef_polyhedron_3();
}