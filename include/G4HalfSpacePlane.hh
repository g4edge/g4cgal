#pragma once

#include "G4ThreeVector.hh"
#include "G4VHalfSpace.hh"

class G4HalfSpacePlane : public G4VHalfSpace {
public:
    G4HalfSpacePlane();
    G4HalfSpacePlane(const G4ThreeVector& p0, const G4ThreeVector& n);
    G4HalfSpacePlane(G4double a, G4double b, G4double c, G4double d);
    ~G4HalfSpacePlane();

    virtual G4double Sdf(const G4ThreeVector&p) const override;
    virtual std::vector<G4ThreeVector> Intersection(const G4ThreeVector& p, const G4ThreeVector &d) const override;

    virtual void Translate(const G4ThreeVector& t) override;
    virtual void Rotate(const G4RotationMatrix& r) override;
    virtual void Transform(const G4AffineTransform& a) override;

    virtual G4SurfaceMeshCGAL* GetSurfaceMesh() const override;


protected:
    G4ThreeVector _p0 = G4ThreeVector(0,0,1);
    G4ThreeVector _n = G4ThreeVector(0,0,0);


};