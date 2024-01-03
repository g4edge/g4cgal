#include "G4PrimitiveLocatorCGAL.hh"
#include "G4VSurfaceMesh.hh"

G4PrimitiveLocatorCGAL::G4PrimitiveLocatorCGAL()
                                               : G4VPrimitiveLocator()
{}

G4PrimitiveLocatorCGAL::~G4PrimitiveLocatorCGAL()
{}

void G4PrimitiveLocatorCGAL::Build(G4VSurfaceMesh *sm, G4double kCarToleranceIn) {
  // set tolerance
  kCarTolerance = kCarToleranceIn;

  G4cout << "G4PrimitiveLocatorCGAL::build> started" << G4endl;

  // loop over faces
  for(G4int i=0;i<sm->NumberOfFaces();i++) {
    auto f = sm->GetFace(i);
    auto v1 = sm->GetVertex(f[0]);
    auto v2 = sm->GetVertex(f[1]);
    auto v3 = sm->GetVertex(f[2]);

    auto a = Point(v1[0], v1[1], v1[2]);
    auto b = Point(v2[0], v2[1], v2[2]);
    auto c = Point(v3[0], v3[1], v3[2]);

    _facets.push_back(Triangle(a,b,c));
  }

  _tree = Tree(_facets.begin(),_facets.end());
  _tree.build();
  G4cout << "G4PrimitiveLocatorCGAL::build> finished" << G4endl;

  return;
}
double G4PrimitiveLocatorCGAL::Distance(const G4ThreeVector &pointG4) const {
  Point point(pointG4.x(), pointG4.y(), pointG4.z());
  FT sqd = sqrt(_tree.squared_distance(point));
  return sqd;
}

G4int G4PrimitiveLocatorCGAL::ClosestPrimitive(const G4ThreeVector &point) const {
  return 0;
}

std::vector<G4ThreeVector> G4PrimitiveLocatorCGAL::Intersections(const G4ThreeVector &pointG4,
                                                                 const G4ThreeVector &dirG4,
                                                                 std::vector<G4int> &interPrims,
                                                                 G4bool outgoing) const {
  Point p(pointG4.x(), pointG4.y(), pointG4.z());
  Direction d(dirG4.x(), dirG4.y(), dirG4.z());
  Ray ray(p,d);

  std::list<Ray_intersection> intersections;
  std::list<Primitive_id> primitives;
  _tree.all_intersections(ray,std::back_inserter(intersections));
  auto intPoint = std::vector<G4ThreeVector>();

  for(auto ip : intersections) {
    auto r = boost::get<Point>(&(ip->first));
    auto pid = ip->second;
    auto n = pid->supporting_plane().orthogonal_vector();

    auto ipG4 = G4ThreeVector(r->x(),
                              r->y(),
                              r->z());
    auto normG4 = G4ThreeVector(n.x(),
                                n.y(),
                                n.z());
    if (outgoing) {
      if ((ipG4 - pointG4).dot(normG4) > 0)
        intPoint.push_back(ipG4);
    }
    else {
      if ((ipG4 - pointG4).dot(normG4) < 0)
        intPoint.push_back(ipG4);
    }
  }

  return intPoint;
}

EInside G4PrimitiveLocatorCGAL::Inside(const G4ThreeVector &point) const {
  auto v0 = _facets[0].vertex(0);
  auto v1 = _facets[0].vertex(1);
  auto v2 = _facets[0].vertex(2);
  auto v0G4 = G4ThreeVector((v0.x()+v1.x()+v2.x())/3,
                            (v0.y()+v1.y()+v2.y())/3,
                            (v0.z()+v1.z()+v2.z())/3);
  G4ThreeVector dirG4 = (v0G4-point);
  dirG4 = dirG4/dirG4.mag();

  Point p(point.x(), point.y(), point.z());
  Direction d(dirG4.x(), dirG4.y(), dirG4.z());
  Ray ray(p,d);

  auto ip = _tree.first_intersection(ray);

  auto r = boost::get<Point>(&(ip->first));
  auto pid = ip->second;

  auto n = pid ->supporting_plane().orthogonal_vector();

  auto ipG4 = G4ThreeVector(r->x(),
                            r->y(),
                            r->z());
  auto normG4 = G4ThreeVector(n.x(),
                              n.y(),
                              n.z());

  auto dProj = (ipG4-point).dot(normG4);

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
