#include "G4HalfSpaceAARBox.hh"
#include "G4HalfSpaceZone.hh"
#include "G4HalfSpacePlane.hh"
#include "G4SurfaceMeshCGAL.hh"
#include "G4Box.hh"

G4HalfSpaceAARBox::G4HalfSpaceAARBox() {}
G4HalfSpaceAARBox::G4HalfSpaceAARBox(G4double xmin, G4double xmax,
                                     G4double ymin, G4double ymax,
                                     G4double zmin, G4double zmax) :
                                     _xmin(xmin), _xmax(xmax),
                                     _ymin(ymin), _ymax(ymax),
                                     _zmin(zmin), _zmax(zmax) {
    _hsZone = new G4HalfSpaceZone();
    auto p1 = new G4HalfSpacePlane(G4ThreeVector(xmax,0,0),G4ThreeVector(1,0,0));
    auto p2 = new G4HalfSpacePlane(G4ThreeVector(xmin,0,0),G4ThreeVector(-1,0,0));
    auto p3 = new G4HalfSpacePlane(G4ThreeVector(0,ymax,0),G4ThreeVector(0,1,0));
    auto p4 = new G4HalfSpacePlane(G4ThreeVector(0,ymin,0),G4ThreeVector(0,-1,0));
    auto p5 = new G4HalfSpacePlane(G4ThreeVector(0,0,zmax),G4ThreeVector(0,0,1));
    auto p6 = new G4HalfSpacePlane(G4ThreeVector(0,0,zmin),G4ThreeVector(0,0,-1));

    _hsZone->AddIntersection(p1);
    _hsZone->AddIntersection(p2);
    _hsZone->AddIntersection(p3);
    _hsZone->AddIntersection(p4);
    _hsZone->AddIntersection(p5);
    _hsZone->AddIntersection(p6);
}

G4HalfSpaceAARBox::~G4HalfSpaceAARBox() {
    delete _hsZone;
}

G4double G4HalfSpaceAARBox::Sdf(const G4ThreeVector&p) const {
    return _hsZone->Sdf(p);
}

std::vector<G4ThreeVector> G4HalfSpaceAARBox::Intersection(const G4ThreeVector& p, const G4ThreeVector& v) const {
    auto intersections = _hsZone->Intersection(p,v);
    return intersections;
}

G4SurfaceMeshCGAL* G4HalfSpaceAARBox::GetSurfaceMesh() const {
    G4double xc = (_xmax + _xmin)/2.0;
    G4double yc = (_ymax + _ymin)/2.0;
    G4double zc = (_zmax + _zmin)/2.0;
    G4Box b = G4Box("temp",(_xmax - _xmin)/2.0 , (_ymax - _ymin)/2.0, (_zmax - _zmin)/2.0);
    G4Polyhedron *g4poly = b.GetPolyhedron();
    G4SurfaceMeshCGAL *sm = new G4SurfaceMeshCGAL();
    sm->fill(g4poly);
    sm->Translate(xc,yc,zc);
    return sm;
}