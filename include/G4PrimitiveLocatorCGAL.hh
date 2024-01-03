#pragma once

#include "G4Types.hh"
#include "G4VPrimitiveLocator.hh"

#include <CGAL/Simple_cartesian.h>
typedef CGAL::Simple_cartesian<double> K;
typedef K::FT FT;
typedef K::Ray_3 Ray;
typedef K::Line_3 Line;
typedef K::Point_3 Point;
typedef K::Triangle_3 Triangle;
typedef K::Segment_3 Segment;
typedef K::Direction_3 Direction;
typedef std::vector<Triangle>::iterator Iterator;

#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_triangle_primitive.h>
typedef CGAL::AABB_triangle_primitive<K, Iterator> Primitive;
typedef CGAL::AABB_traits<K, Primitive> AABB_triangle_traits;
typedef CGAL::AABB_tree<AABB_triangle_traits> Tree;
typedef Tree::Primitive_id Primitive_id;

typedef boost::optional< Tree::Intersection_and_primitive_id<Ray>::Type > Ray_intersection;

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
  std::vector<Triangle> _facets;
  Tree _tree;
};
