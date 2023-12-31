#include "G4PrimitiveLocatorCGAL.hh"

G4PrimitiveLocatorCGAL::G4PrimitiveLocatorCGAL()
                                               : G4VPrimitiveLocator()
{}

G4PrimitiveLocatorCGAL::~G4PrimitiveLocatorCGAL()
{}

void G4PrimitiveLocatorCGAL::Build(G4VSurfaceMesh *, G4double kCarToleranceIn) {
  kCarTolerance = kCarToleranceIn;
  return;
}
double G4PrimitiveLocatorCGAL::Distance(const G4ThreeVector &point) const {
  return 0;
}

G4int G4PrimitiveLocatorCGAL::ClosestPrimitive(const G4ThreeVector &point) const {
  return 0;
}

std::vector<G4ThreeVector> G4PrimitiveLocatorCGAL::Intersections(const G4ThreeVector &point,
                                                                 const G4ThreeVector &dir,
                                                                 std::vector<G4int> &interPrims,
                                                                 G4bool outgoing) const {
  return std::vector<G4ThreeVector>();
}

EInside G4PrimitiveLocatorCGAL::Inside(const G4ThreeVector &point) const {
  return kSurface;
}
