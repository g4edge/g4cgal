#pragma once

#pragma once

#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "geomdefs.hh"

class G4VSurfaceMesh;

class G4VPrimitiveLocator {
public:
  G4VPrimitiveLocator();
  ~G4VPrimitiveLocator();

  virtual void Build(G4VSurfaceMesh *, G4double kCarTolerance) = 0;
  virtual G4double Distance(const G4ThreeVector &point) const = 0;
  virtual G4int ClosestPrimitive(const G4ThreeVector &point) const = 0;
  virtual std::vector<G4ThreeVector> Intersections(const G4ThreeVector &point,
                                                   const G4ThreeVector &dir,
                                                   std::vector<G4int> &interPrims,
                                                   G4bool outgoing = true) const  = 0 ;
  virtual EInside Inside(const G4ThreeVector &point) const = 0;

protected:
  G4double kCarTolerance;
};