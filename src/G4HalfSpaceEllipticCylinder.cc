#include "G4HalfSpaceEllipticCylinder.hh"

G4HalfEllipticCylinder::G4HalfEllipticCylinder() {}

G4HalfEllipticCylinder::G4HalfEllipticCylinder(const G4ThreeVector& p0,
                                               const G4ThreeVector& d,
                                               G4double r1,
                                               G4double r2) :
    _p0(p0), _d(d), _r1(r1), _r2(r2) {}

G4HalfEllipticCylinder::~G4HalfEllipticCylinder() {}

G4double G4HalfEllipticCylinder::Sdf(const G4ThreeVector&p) const {
    return 0;
}
std::vector<G4ThreeVector> G4HalfEllipticCylinder::Intersection(const G4ThreeVector& p, const G4ThreeVector &d) const {
    return std::vector<G4ThreeVector>();
}

void G4HalfEllipticCylinder::Translate(const G4ThreeVector& t) {}
void G4HalfEllipticCylinder::Rotate(const G4RotationMatrix& r) {}
void G4HalfEllipticCylinder::Transform(const G4AffineTransform& a) {}

G4SurfaceMeshCGAL* G4HalfEllipticCylinder::GetSurfaceMesh() const {
    return new G4SurfaceMeshCGAL();
}
