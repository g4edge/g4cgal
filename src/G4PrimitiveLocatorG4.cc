#include "G4TriangularFacet.hh"
#include "G4PrimitiveLocatorG4.hh"
#include "G4VSurfaceMesh.hh"
#include "G4Shader.hh"

#define G4PRIMITIVELOCATORG4_DEBUG

G4PrimitiveLocatorG4::G4PrimitiveLocatorG4()
    : G4VPrimitiveLocator()
{}

G4PrimitiveLocatorG4::~G4PrimitiveLocatorG4()
{}

void G4PrimitiveLocatorG4::Build(G4VSurfaceMesh *sm, G4double kCarToleranceIn) {

  // set tolerance
  kCarTolerance = kCarToleranceIn;

  G4cout << "G4PrimitiveLocatorG4::build> started" << G4endl;

  // loop over faces
  for(G4int i=0;i<sm->NumberOfFaces();i++) {
    auto f = sm->GetFace(i);
    auto v1 = sm->GetVertex(f[0]);
    auto v2 = sm->GetVertex(f[1]);
    auto v3 = sm->GetVertex(f[2]);

    auto a = G4ThreeVector(v1[0], v1[1], v1[2]);
    auto b = G4ThreeVector(v2[0], v2[1], v2[2]);
    auto c = G4ThreeVector(v3[0], v3[1], v3[2]);

    _facets.push_back(new G4TriangularFacet(a,b,c, G4FacetVertexType::ABSOLUTE));
  }

  G4cout << "G4PrimitiveLocatorG4::build> finished" << G4endl;

  return;

}

G4double G4PrimitiveLocatorG4::Distance(const G4ThreeVector &point) const {
  G4double minDist = 1e99;
  G4int iFacet = 0;
  for(auto f : _facets) {
    auto d = f->Distance(point).mag();
    // G4cout << "Distance> " << iFacet << " " << f->Distance(point) << " " << d << G4endl;
    minDist = d < minDist ? d : minDist;
    iFacet++;
  }

  return minDist;
}

G4int G4PrimitiveLocatorG4::ClosestPrimitive(const G4ThreeVector &point) const{

  // loop over all primitives
  G4double minDist = 1e99;
  G4int minPrim = -1;
  G4int iFacet = 0;
  for(auto f : _facets) {
    auto d = f->Distance(point).mag();
    if (d <= minDist) {
      minPrim = iFacet;
      minDist = d;
    }
    // G4cout << "ClosestPrimitive> " << iFacet << " " << f->Distance(point) << " " << d << G4endl;
    iFacet++;
  }
  return minPrim;
}

std::vector<G4ThreeVector> G4PrimitiveLocatorG4::Intersections(const G4ThreeVector &point,
                                                               const G4ThreeVector &dir,
                                                               std::vector<G4int> &intPrim,
                                                               G4bool outgoing) const {
  G4double dist;
  G4double distSurface;
  G4ThreeVector normal;
  std::vector<G4ThreeVector> intPoint;
  int iFacet = 0;

  for(auto f : _facets) {

    if(outgoing) {
      auto bInter = f->Intersect(point, dir, true, dist, distSurface, normal);
      if (bInter) {
        intPrim.push_back(iFacet);
        intPoint.push_back(point + dir * dist);
      }
    }
    else {
      auto bInter = f->Intersect(point, dir, false, dist, distSurface, normal);
      if (bInter) {
        intPrim.push_back(iFacet);
        intPoint.push_back(point + dir * dist);
      }
    }
    iFacet++;
  }

  sort( intPoint.begin(), intPoint.end(), [=](const G4ThreeVector &v1, const G4ThreeVector &v2)
  {return (v1-point).mag() > (v2-point).mag();});
  return intPoint;
}

EInside G4PrimitiveLocatorG4::Inside(const G4ThreeVector &point) const {
  // G4cout << "G4PrimitiveLocatorG4::inside> point=" << point << G4endl;

  auto facet = _facets[0];
  G4ThreeVector dir = (facet->GetPointOnFace()-point);
  dir = dir/dir.mag();

  std::vector<G4int> intPrim;
  std::vector<G4int> intPrimOutward;
  std::vector<G4int> intPrimInward;
  std::vector<G4ThreeVector> intPoint;
  auto intPointOutward = Intersections(point, dir, intPrimOutward, true);
  auto intPointInward = Intersections(point, dir, intPrimInward, false);
  intPoint.insert(intPoint.end(), intPointOutward.begin(), intPointOutward.end());
  intPoint.insert(intPoint.end(), intPointInward.begin(), intPointInward.end());
  intPrim.insert(intPrim.end(), intPrimOutward.begin(), intPrimOutward.end());
  intPrim.insert(intPrim.end(), intPrimInward.begin(), intPrimInward.end());

  G4int iMin = -1;
  G4int pMin = -1;
  G4double dMin = 1e99;
  for(G4int iInter =0; iInter< intPoint.size(); iInter++) {
    auto d = (intPoint[iInter] - point).mag();
    dMin = d <= dMin ? d : dMin;
    pMin = d <= dMin ? intPrim[iInter] : pMin;
    iMin = d <= dMin ? iInter : iMin;
    //G4cout << "G4PrimitiveLocatorG4::inside> " << dir << " " << iInter << " " << intPoint[iInter] << " " << d << " " << dMin << G4endl;
  }

  auto dProj = _facets[pMin]->GetSurfaceNormal().dot(intPoint[iMin] - point);

  //G4cout << "G4PrimitiveLocatorG4::inside> " << iMin << " " << _facets[iMin]->GetSurfaceNormal() << " " << intPoint[iMin]-point << " " << dProj << G4endl;
  auto retVal = kOutside;
  if(fabs(dProj) <= kCarTolerance/2.0)
    retVal = kSurface;
  else if(dProj > kCarTolerance/2.0)
    retVal =  kInside;
  else
    retVal =  kOutside;

  // G4cout << "G4PrimitiveLocatorG4::inside> ret=" << retVal << G4endl;
  return retVal;
}