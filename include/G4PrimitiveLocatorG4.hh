#pragma once

#include "G4Types.hh"
#include "G4VPrimitiveLocator.hh"
#include "G4Shader.hh"

class G4VSurfaceMesh;
class G4TriangularFacet;

class G4PrimitiveLocatorG4 : public G4VPrimitiveLocator {
public:
  G4PrimitiveLocatorG4();
  ~G4PrimitiveLocatorG4();

  virtual void Build(G4VSurfaceMesh *, G4double kCarToleranceIn) override;
  virtual G4double Distance(const G4ThreeVector &point) const override;
  virtual G4int ClosestPrimitive(const G4ThreeVector &point) const override;
  virtual std::vector<G4ThreeVector> Intersections(const G4ThreeVector &point,
                                                   const G4ThreeVector &dir,
                                                   std::vector<G4int> &interPrims,
                                                   G4bool outgoing) const override;
  virtual EInside Inside(const G4ThreeVector &point) const override;

private:
  std::vector<G4TriangularFacet*> _facets;
};