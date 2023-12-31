#pragma once

#include "G4Types.hh"
#include "G4VPrimitiveLocator.hh"

class G4VSurfaceMesh;

class G4PrimitiveLocatorCGAL : public G4VPrimitiveLocator {
public:
  G4PrimitiveLocatorCGAL();
  ~G4PrimitiveLocatorCGAL();

  virtual void Build(G4VSurfaceMesh *, G4double kCarTolerance) override;
  virtual G4double Distance(const G4ThreeVector &point) const override;
  virtual G4int ClosestPrimitive(const G4ThreeVector &point) const override;
  virtual std::vector<G4ThreeVector> Intersections(const G4ThreeVector &point,
                                                   const G4ThreeVector &dir,
                                                   std::vector<G4int> &interPrims,
                                                   G4bool outgoing) const override;
  virtual EInside Inside(const G4ThreeVector &point) const override;
private:
};
