#pragma once

#include "G4ThreeVector.hh"
#include "G4VHalfSpace.hh"

class G4HalfSpaceSphere : public G4VHalfSpace {
public:
    G4HalfSpaceSphere();
    G4HalfSpaceSphere(const G4ThreeVector& p0, G4double radius);
    ~G4HalfSpaceSphere();

    virtual G4double Sdf(const G4ThreeVector&p) const override;
    std::vector<G4ThreeVector> Intersection(const G4ThreeVector& p, const G4ThreeVector& v) const override;

    virtual Nef_polyhedron_3 GetNefPolyhedron() const override;


protected:
    G4ThreeVector _p0 = G4ThreeVector(0,0,1);
    G4double _r = 1.0;
};