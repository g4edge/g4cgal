#include "G4HalfSpacePlane.hh"
#include "G4VSolid.hh"

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_rational.h>
#include <CGAL/Extended_cartesian.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Surface_mesh.h>

typedef CGAL::Exact_rational ER;
typedef CGAL::Extended_cartesian<ER> Kernel_ECER;
typedef CGAL::Nef_polyhedron_3<Kernel_ECER> Nef_polyhedron_3_ECER;
typedef CGAL::Surface_mesh<Kernel_ECER> Surface_mesh_ECER;
typedef Kernel_ECER::Point_3 Point_3_ECER;
typedef Kernel_ECER::Vector_3 Vector_3_ECER;
typedef Kernel_ECER::Plane_3 Plane_3_ECER;
typedef Kernel_ECER::Direction_3 Direction_3_ECER;

G4HalfSpacePlane::G4HalfSpacePlane() {
};

G4HalfSpacePlane::G4HalfSpacePlane(const G4ThreeVector &p0, const G4ThreeVector &n) : _p0(p0), _n(n)
{}

G4HalfSpacePlane::G4HalfSpacePlane(G4double a, G4double b, G4double c, G4double d) {
    _n = G4ThreeVector(a,b,c);
    _n = _n/_n.mag();
    _p0 = _n*d;
};

G4HalfSpacePlane::~G4HalfSpacePlane() {};

G4double G4HalfSpacePlane::Sdf(const G4ThreeVector &p) const {
    G4double dist = (p - _p0).dot(_n);
    return dist;
}

std::vector<G4ThreeVector> G4HalfSpacePlane::Intersection(const G4ThreeVector& p, const G4ThreeVector &v) const {
    std::vector<G4ThreeVector> intersections;

    auto vNorm = v/v.mag();
    auto dDenom = vNorm.dot(_n);
    auto lambda = (_p0-p).dot(_n)/ dDenom;

    if(dDenom != 0 && lambda >= 0)
        intersections.push_back(lambda*v+p);

    return intersections;
}

void G4HalfSpacePlane::Translate(const G4ThreeVector& t) {
    _p0 = _p0 + t;
}

void G4HalfSpacePlane::Rotate(const G4RotationMatrix& r) {
    _p0 = r*_p0;
    _n = r*_n;
}

void G4HalfSpacePlane::Transform(const G4AffineTransform& a) {
    _p0 = a.TransformPoint(_p0);
    _n = a.NetRotation()*_n;
}


G4SurfaceMeshCGAL* G4HalfSpacePlane::GetSurfaceMesh() const {

    Nef_polyhedron_3_ECER nef = Nef_polyhedron_3_ECER(Nef_polyhedron_3_ECER::COMPLETE);
    nef *= Nef_polyhedron_3_ECER(Plane_3_ECER(Point_3_ECER(1000000000,0,0),
                                         Direction_3_ECER(1, 0, 0)));
    nef *= Nef_polyhedron_3_ECER(Plane_3_ECER(Point_3_ECER(-1000000000,0,0),
                                         Direction_3_ECER(-1, 0, 0)));
    nef *= Nef_polyhedron_3_ECER(Plane_3_ECER(Point_3_ECER(0,1000000000,0),
                                         Direction_3_ECER(0, 1, 0)));
    nef *= Nef_polyhedron_3_ECER(Plane_3_ECER(Point_3_ECER(0,-1000000000,0),
                                         Direction_3_ECER(0, -1, 0)));
    nef *= Nef_polyhedron_3_ECER(Plane_3_ECER(Point_3_ECER(0,0,1000000000),
                                         Direction_3_ECER(0, 0, 1)));
    nef *= Nef_polyhedron_3_ECER(Plane_3_ECER(Point_3_ECER(0,0,-1000000000),
                                         Direction_3_ECER(0, 0, -1)));

    nef *= Nef_polyhedron_3_ECER(Plane_3_ECER(Point_3_ECER(_p0.x(), _p0.y(), _p0.z()),
                                         Direction_3_ECER(_n.x(), _n.y(), _n.z())));

    return new G4SurfaceMeshCGAL(nef);
}

