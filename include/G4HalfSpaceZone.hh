#pragma once

#include "G4VHalfSpace.hh"

class G4HalfSpaceZone : G4VHalfSpace {
public:
    enum operation { intersection, subtraction };

    G4HalfSpaceZone();
    ~G4HalfSpaceZone();

    using G4VHalfSpace::Inside;
    void AddIntersection(G4VHalfSpace *hs);
    void AddSubtraction(G4VHalfSpace *hs);

    virtual G4double Sdf(const G4ThreeVector &p) const override;
    virtual std::vector<G4ThreeVector> Intersection(const G4ThreeVector& p, const G4ThreeVector &d) const override;

    virtual Nef_polyhedron_3 GetNefPolyhedron() const override;

protected:
    std::vector<std::pair<operation, G4VHalfSpace*>> _half_spaces;

};