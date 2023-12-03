#pragma once

#include "G4ThreeVector.hh"
#include "G4VHalfSpace.hh"

class G4HalfEllipticCylinder: public G4VHalfSpace {
public:
    G4HalfEllipticCylinder();
    G4HalfEllipticCylinder(const G4ThreeVector& p0, const G4ThreeVector& d, G4double r1, G4double r2);
    ~G4HalfEllipticCylinder();

    virtual G4double Sdf(const G4ThreeVector&p) const override;
    virtual std::vector<G4ThreeVector> Intersection(const G4ThreeVector& p, const G4ThreeVector &d) const override;

    virtual void Translate(const G4ThreeVector& t) override;
    virtual void Rotate(const G4RotationMatrix& r) override;
    virtual void Transform(const G4AffineTransform& a) override;

    virtual G4SurfaceMeshCGAL* GetSurfaceMesh() const override;

protected:
    G4ThreeVector _p0 = G4ThreeVector(0,0,0);
    G4ThreeVector _d = G4ThreeVector(0,0,1);
    G4double _r1 = 1.0;
    G4double _r2 = 1.0;
};