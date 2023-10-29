#include "G4ThreeVector.hh"

#include "G4VHalfSpace.hh"


class G4HalfSpacePlane : public G4VHalfSpace {
public:
    G4HalfSpacePlane();
    G4HalfSpacePlane(const G4ThreeVector& p0, const G4ThreeVector& n);
    G4HalfSpacePlane(G4double a, G4double b, G4double c, G4double d);
    ~G4HalfSpacePlane();

    virtual G4bool Inside(const G4ThreeVector& p) const override;
    virtual G4double Distance(const G4ThreeVector&p) const override;
    virtual G4double DistanceToIn(const G4ThreeVector& p) const override;
    virtual G4double DistanceToOut(const G4ThreeVector& p) const override;
    virtual G4double DistanceToIn(const G4ThreeVector& p,
                                  const G4ThreeVector& d) const override;
    virtual G4double DistanceToOut(const G4ThreeVector& p,
                                   const G4ThreeVector& d) const override;

    virtual Nef_polyhedron_3 GetNefPolyhedron() const override;


protected:
    G4ThreeVector _p0 = G4ThreeVector(0,0,1);
    G4ThreeVector _n = G4ThreeVector(0,0,0);


};