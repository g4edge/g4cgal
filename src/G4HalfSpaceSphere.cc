#include "G4HalfSpaceSphere.hh"
#include "G4Orb.hh"
#include "G4SurfaceMeshCGAL.hh"

G4HalfSpaceSphere::G4HalfSpaceSphere() {}
G4HalfSpaceSphere::G4HalfSpaceSphere(G4double radius) : _r(radius) {}

G4HalfSpaceSphere::~G4HalfSpaceSphere() {}

G4double G4HalfSpaceSphere::Sdf(const G4ThreeVector&p) const {
    return p.mag() - _r;
}

std::vector<G4ThreeVector> G4HalfSpaceSphere::Intersection(const G4ThreeVector& p, const G4ThreeVector& v) const {
    // v = lambda d + p
    // |v| = r
    // | lambda d + p | = r
    // (lambda d + p) . (lambda d p) = r.r
    // lambda^2 d.d + 2 lambda d.(p) + (p).(p) = r.r
    // a = d.d
    // b = 2 d.(p)
    // c = (p).(p) - r*r

    std::vector<G4ThreeVector> intersections;

    auto a = v.dot(v);
    auto b = 2*v.dot(p);
    auto c = p.dot(p) - _r*_r;

    G4int nSoln = 0;
    G4double lambda1 = 0;
    G4double lambda2 = 0;
    G4VHalfSpace::QuadraticSolve(a,b,c,nSoln, lambda1, lambda2);
    intersections.push_back(lambda1*v + p);
    intersections.push_back(lambda2*v + p);

    return intersections;
}

Nef_polyhedron_3 G4HalfSpaceSphere::GetNefPolyhedron() const {

    G4Orb *orb = new G4Orb("test",_r);
    auto *poly = orb->CreatePolyhedron();

    G4SurfaceMeshCGAL sm;
    sm.fill(poly);

    Nef_polyhedron_3 nef(Nef_polyhedron_3::EMPTY);

    return nef;
}