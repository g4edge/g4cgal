#include "G4HalfSpaceXACircularCylinder.hh"
#include "CLHEP/Matrix/Matrix.h"
#include "CLHEP/Matrix/Vector.h"

G4HalfSpaceXACircularCylinder::G4HalfSpaceXACircularCylinder() {}

G4HalfSpaceXACircularCylinder::G4HalfSpaceXACircularCylinder(G4double y,
                                                             G4double z,
                                                             G4double r) :
                                                             _y0(y), _z0(z), _r(r) {}

G4HalfSpaceXACircularCylinder::~G4HalfSpaceXACircularCylinder() {}

G4double G4HalfSpaceXACircularCylinder::Sdf(const G4ThreeVector&p) const {
    return sqrt(pow(p.y() - _y0,2) + pow(p.z() - _z0,2) ) - _r;
}

std::vector<G4ThreeVector> G4HalfSpaceXACircularCylinder::Intersection(const G4ThreeVector& p, const G4ThreeVector &d) const {

    auto q = CLHEP::HepMatrix(3,3,0);
    auto qp = CLHEP::HepVector(3);
    auto n = CLHEP::HepVector(3);
    auto p0 = CLHEP::HepVector(3);

    n(1) = d.x();
    n(2) = d.y();
    n(3) = d.z();

    p0(1) = p.x();
    p0(2) = p.y();
    p0(3) = p.z();

    q(2,2) = 1./pow(_r,2);
    q(3,3) = 1./pow(_r,2);

    auto a = (n.T()*q*n)(1,1);
    auto b = (n.T()*q*p0 + qp.T()*n + p0.T()*q*n)(1,1);
    auto c = (qp.T()*p0 + p0.T()*q*p0)(1,1) - 1 ;

    G4int nSoln;
    G4double l1, l2;
    QuadraticSolve(a,b,c,nSoln,l1,l2);

    auto inter = std::vector<G4ThreeVector>();
    if (l1 >= 0 ) {
        inter.push_back(l1*d+p);
    }
    if (l2 >= 0 ) {
        inter.push_back(l2*d+p);
    }

    //G4cout << "return intersections" << G4endl;
    return inter;
}

void G4HalfSpaceXACircularCylinder::Translate(const G4ThreeVector& t) {
    _y0 += t.y();
    _z0 += t.z();
}

void G4HalfSpaceXACircularCylinder::Rotate(const G4RotationMatrix& r) {
    G4cout << "G4HalfSpaceXACircularCylinder::Rotate> Not implemented" << G4endl;
}
void G4HalfSpaceXACircularCylinder::Transform(const G4AffineTransform& a) {
    G4cout << "G4HalfSpaceXACircularCylinder::Transform> Not implemented" << G4endl;
}

G4SurfaceMeshCGAL* G4HalfSpaceXACircularCylinder::GetSurfaceMesh() const {
    return new G4SurfaceMeshCGAL();
}
