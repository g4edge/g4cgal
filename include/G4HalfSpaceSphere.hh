#pragma once

#include "G4ThreeVector.hh"
#include "G4VHalfSpace.hh"

class G4HalfSpaceSphere : public G4VHalfSpace {
public:
    G4HalfSpaceSphere();
    G4HalfSpaceSphere(G4double radius, G4ThreeVector centre);
    ~G4HalfSpaceSphere();

    virtual G4double Sdf(const G4ThreeVector&p) const override;
    std::vector<G4ThreeVector> Intersection(const G4ThreeVector& p, const G4ThreeVector& v) const override;

    virtual G4SurfaceMeshCGAL* GetSurfaceMesh() const override;


protected:
    G4double _r = 1.0;
    G4ThreeVector _centre;
};