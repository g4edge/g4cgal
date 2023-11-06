#include "G4HalfSpaceSphere.hh"
#include "G4Orb.hh"
#include "G4SurfaceMeshCGAL.hh"

G4HalfSpaceSphere::G4HalfSpaceSphere() {}
G4HalfSpaceSphere::G4HalfSpaceSphere(G4double radius) : _r(radius) {}

G4HalfSpaceSphere::~G4HalfSpaceSphere() {}

G4double G4HalfSpaceSphere::Sdf(const G4ThreeVector&p) const {
    return p.mag() - _r;
}

std::vector<G4ThreeVector> G4HalfSpaceSphere::Intersection(const G4ThreeVector& p, const G4ThreeVector& v) const {
    // v = lambda d + p
    // |v| = r
    // | lambda d + p | = r
    // (lambda d + p) . (lambda d p) = r.r
    // lambda^2 d.d + 2 lambda d.(p) + (p).(p) = r.r
    // a = d.d
    // b = 2 d.(p)
    // c = (p).(p) - r*r

    std::vector<G4ThreeVector> intersections;

    auto a = v.dot(v);
    auto b = 2*v.dot(p);
    auto c = p.dot(p) - _r*_r;

    G4int nSoln = 0;
    G4double lambda1 = 0;
    G4double lambda2 = 0;
    G4VHalfSpace::QuadraticSolve(a,b,c,nSoln, lambda1, lambda2);
    intersections.push_back(lambda1*v + p);
    intersections.push_back(lambda2*v + p);

    return intersections;
}

Nef_polyhedron_3 G4HalfSpaceSphere::GetNefPolyhedron() const {

#if 0
    G4Orb o = G4Orb("test",25);
    G4Polyhedron *g4poly = o.GetPolyhedron();
    G4SurfaceMeshCGAL sm;
    sm.fill(g4poly);

    auto cgpoly = sm.GetCGALPolyhedron_3_ECER();
    G4cout << cgpoly.is_valid(true) << " " << cgpoly.is_closed() << G4endl;
#endif

    Nef_polyhedron_3_ECER nef(Nef_polyhedron_3::COMPLETE);
    nef *= Nef_polyhedron_3(Plane_3(Point_3(_r,0,0),
                                    Direction_3(1,0,0)));
    nef *= Nef_polyhedron_3(Plane_3(Point_3(-_r,0,0),
                                    Direction_3(-1,0,0)));

    nef *= Nef_polyhedron_3(Plane_3(Point_3(0,_r,0),
                                    Direction_3(0,1,0)));
    nef *= Nef_polyhedron_3(Plane_3(Point_3(0,-_r,0),
                                    Direction_3(0,-1,0)));

    nef *= Nef_polyhedron_3(Plane_3(Point_3(0,0,_r),
                            Direction_3(0,0,1)));
    nef *= Nef_polyhedron_3(Plane_3(Point_3(0,0,-_r),
                            Direction_3(0,0,-1)));


    return nef;
}