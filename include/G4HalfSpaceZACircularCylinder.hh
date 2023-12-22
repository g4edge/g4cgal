#pragma once

#include "G4ThreeVector.hh"
#include "G4VHalfSpace.hh"

class G4HalfSpaceZACircularCylinder: public G4VHalfSpace {
public:
    G4HalfSpaceZACircularCylinder();
    G4HalfSpaceZACircularCylinder(G4double x, G4double y, G4double r);
    ~G4HalfSpaceZACircularCylinder();

    virtual G4double Sdf(const G4ThreeVector&p) const override;
    virtual std::vector<G4ThreeVector> Intersection(const G4ThreeVector& p, const G4ThreeVector &d) const override;

    virtual void Translate(const G4ThreeVector& t) override;
    virtual void Rotate(const G4RotationMatrix& r) override;
    virtual void Transform(const G4AffineTransform& a) override;

    virtual G4SurfaceMeshCGAL* GetSurfaceMesh() const override;

protected:
    G4double _x0 = 0;
    G4double _y0 = 0;
    G4double _r = 1.0;
};
