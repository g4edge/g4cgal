#pragma once

#include "G4VHalfSpace.hh"

class G4HalfSpaceZone : G4VHalfSpace {
public:
    enum operation { intersection, subtraction };

    G4HalfSpaceZone();
    ~G4HalfSpaceZone();

    void AddIntersection(G4VHalfSpace *hs);
    void AddSubtraction(G4VHalfSpace *hs);

    virtual G4bool   Inside(const G4ThreeVector& p) const override;
    virtual G4double Distance(const G4ThreeVector &p) const override;
    virtual G4double Distance(const G4ThreeVector& p, const G4ThreeVector& v) const override;
    virtual G4double DistanceToOut(const G4ThreeVector& p, const G4ThreeVector& v) const;
    virtual Nef_polyhedron_3 GetNefPolyhedron() const override;
    virtual G4ThreeVector Normal(const G4ThreeVector&p) const override;

protected:
    std::vector<std::pair<operation, G4VHalfSpace*>> _half_spaces;

};