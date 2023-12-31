#include <random>
#include "globals.hh"
#include "G4SurfaceMeshSolid.hh"
#include "G4VSurfaceMesh.hh"
#include "G4SurfaceMeshCGAL.hh"
#include "G4VGraphicsScene.hh"
#include "G4VPrimitiveLocator.hh"

#include "G4VisAttributes.hh"
#include "G4Polyline.hh"
#include "G4Text.hh"
#include "G4Circle.hh"
#include "G4Square.hh"
#include "G4Polymarker.hh"
#include "G4Polyhedron.hh"

#include "G4Orb.hh"
#include "G4DisplacedSolid.hh"

G4SurfaceMeshSolid::G4SurfaceMeshSolid()
                                       : G4VSolid("")
{
  _mesh = nullptr;
  _pl = nullptr;
}

G4SurfaceMeshSolid::G4SurfaceMeshSolid(G4String name,
                                       G4VSurfaceMesh *mesh,
                                       G4VPrimitiveLocator *pl )
                                       : G4VSolid(name)
{
  _mesh = mesh;
  _pl = pl;
  _pl->Build(_mesh, kCarTolerance);
}

G4SurfaceMeshSolid::~G4SurfaceMeshSolid() {}

/* G4VSolid virtual methods */
G4bool G4SurfaceMeshSolid::CalculateExtent(const EAxis pAxis,
                                           const G4VoxelLimits& pVoxelLimit,
                                           const G4AffineTransform& pTransform,
                                           G4double& pMin, G4double& pMax) const {
  return false;
}

EInside G4SurfaceMeshSolid::Inside(const G4ThreeVector& p) const {
  return _pl->Inside(p);
}

G4ThreeVector G4SurfaceMeshSolid::SurfaceNormal(const G4ThreeVector& p) const  {
  return G4ThreeVector();
}

G4double G4SurfaceMeshSolid::DistanceToIn(const G4ThreeVector& p,
                                          const G4ThreeVector& v) const {
  std::vector<G4int> interPrims;
  auto intPoint = _pl->Intersections(p, v, interPrims, false);
  if(intPoint.size() == 0)
    return kInfinity;
  else
    return (intPoint[0] - p).mag();
};

G4double G4SurfaceMeshSolid::DistanceToIn(const G4ThreeVector& p) const {
  return _pl->Distance(p);
};

G4double G4SurfaceMeshSolid::DistanceToOut(const G4ThreeVector& p,
                                           const G4ThreeVector& v,
                                           const G4bool calcNorm,
                                           G4bool* validNorm,
                                           G4ThreeVector* n) const  {
  std::vector<G4int> intPrim;
  auto intPoint = _pl->Intersections(p,v,intPrim, true);
  if(intPoint.size() == 0 ) {
    return kInfinity;
  }
  else {
    return (intPoint[0] - p).mag();
  }
};

G4double G4SurfaceMeshSolid::DistanceToOut(const G4ThreeVector& p) const {
  return _pl->Distance(p);
};

G4GeometryType  G4SurfaceMeshSolid::GetEntityType() const {
  return G4String("G4SurfaceMeshSolid");
};

std::ostream& G4SurfaceMeshSolid::StreamInfo(std::ostream& os) const {
  return os;
};

void G4SurfaceMeshSolid::DescribeYourselfTo(G4VGraphicsScene& scene) const {

  auto p = _mesh->GetG4Polyhedron();
  scene.AddPrimitive(*p);

  for(auto dp : _debugPolyhedra) {
    auto va = new G4VisAttributes(dp.first);

    try {
      G4Polyhedron *p = std::get<G4Polyhedron*>(dp.second);
      scene.AddPrimitive(*p);
    }
    catch (std::bad_variant_access const& ex)
    {}

    try {
      G4Polyline *p = std::get<G4Polyline*>(dp.second);
      scene.AddPrimitive(*p);
    }
    catch (std::bad_variant_access const& ex)
    {}
  }
  return;
};

void G4SurfaceMeshSolid::Test(G4ThreeVector testPointIn,
                              G4ThreeVector testDirIn) {
  auto d = _pl->Distance(testPointIn);
  auto c = _pl->ClosestPrimitive(testPointIn);
  std::vector<G4int> interPrimOutward;
  std::vector<G4int> interPrimInward;
  auto intPointOutward = _pl->Intersections(testPointIn, testDirIn, interPrimOutward, true);
  auto intPointInward = _pl->Intersections(testPointIn, testDirIn, interPrimInward, false);
  auto in = _pl->Inside(testPointIn);

  std::vector<G4ThreeVector> intPoint;
  intPoint.insert(intPoint.end(), intPointOutward.begin(), intPointOutward.end());
  intPoint.insert(intPoint.end(), intPointInward.begin(), intPointInward.end());
  std::vector<G4int> interPrim;
  interPrim.insert(interPrim.end(), interPrimOutward.begin(), interPrimOutward.end());
  interPrim.insert(interPrim.end(), interPrimInward.begin(), interPrimInward.end());

  //G4cout << "distance> " << d << G4endl;
  //G4cout << "closest prim> " << c << G4endl;
  //G4cout << "inside> " << in << G4endl;
  //for(G4int j=0;j<interPrim.size();j++) {
  //  G4cout << "inter> " << intPoint[j] << " " << interPrim[j] << G4endl;
  //}

  auto face = _mesh->GetFace(c);
  auto v1 = _mesh->GetVertex(face[0]);
  auto v2 = _mesh->GetVertex(face[1]);
  auto v3 = _mesh->GetVertex(face[2]);

  G4Orb o = G4Orb("temp",1*CLHEP::mm);
  auto sm = new G4SurfaceMeshCGAL();
  sm->Fill(o.GetPolyhedron());
  sm->Translate(testPointIn.x(), testPointIn.y(), testPointIn.z());
  auto p = sm->GetG4Polyhedron();
  auto va = new G4VisAttributes(p->GetVisAttributes());
  va->SetColor(1,0,0);
  p->SetVisAttributes(va);
  _debugPolyhedra[va] = p;

  // closest primitive polyline
  auto closestPrimitive = new G4Polyline();
  closestPrimitive->push_back(G4Point3D(v1[0],v1[1],v1[2]));
  closestPrimitive->push_back(G4Point3D(v2[0],v2[1],v2[2]));
  closestPrimitive->push_back(G4Point3D(v3[0],v3[1],v3[2]));
  closestPrimitive->push_back(G4Point3D(v1[0],v1[1],v1[2]));

  auto closestPrimitiveVA = new G4VisAttributes(closestPrimitive->GetVisAttributes());
  closestPrimitiveVA->SetColor(0,0,1);
  closestPrimitiveVA->SetLineWidth(10.0);
  closestPrimitive->SetVisAttributes(closestPrimitiveVA);
  _debugPolyhedra[closestPrimitiveVA] = closestPrimitive;

  // closest distance sphere
  o = G4Orb("temp", d * CLHEP::mm);
  sm = new G4SurfaceMeshCGAL();
  sm->Fill(o.GetPolyhedron());
  sm->Translate(testPointIn.x(), testPointIn.y(), testPointIn.z());
  p = sm->GetG4Polyhedron();
  va = new G4VisAttributes(p->GetVisAttributes());
  va->SetColor(0, 1, 0, 0.5);
  p->SetVisAttributes(va);
  _debugPolyhedra[va] = p;

  // intersection ray
  auto rayPrimitive = new G4Polyline();
  rayPrimitive->push_back(testPointIn);
  rayPrimitive->push_back(testPointIn+testDirIn*1000);

  auto rayPrimitiveVA = new G4VisAttributes(rayPrimitive->GetVisAttributes());
  rayPrimitiveVA->SetColor(0,0,1);
  rayPrimitiveVA->SetLineWidth(10.0);
  rayPrimitive->SetVisAttributes(rayPrimitiveVA);
  _debugPolyhedra[rayPrimitiveVA] = rayPrimitive;

  // intersection points
  for(auto inter : intPoint) {
    o = G4Orb("temp", 1 * CLHEP::mm);
    sm = new G4SurfaceMeshCGAL();
    sm->Fill(o.GetPolyhedron());
    sm->Translate(inter.x(), inter.y(), inter.z());
    p = sm->GetG4Polyhedron();
    va = new G4VisAttributes(p->GetVisAttributes());
    va->SetColor(0, 1, 0);
    p->SetVisAttributes(va);
    _debugPolyhedra[va] = p;
  }
}

void G4SurfaceMeshSolid::TestRandomInside() {

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(-30, 30);

  for(auto iEvent = 0; iEvent< 1000; iEvent++) {
    auto testPointIn = G4ThreeVector(dis(gen),dis(gen),dis(gen));
    auto in = _pl->Inside(testPointIn);
    G4cout << iEvent << " " << in << " " << testPointIn << " " << testPointIn.mag() << G4endl;

    auto o = G4Orb("temp", 1 * CLHEP::mm);
    auto sm = new G4SurfaceMeshCGAL();
    sm->Fill(o.GetPolyhedron());
    sm->Translate(testPointIn.x(), testPointIn.y(), testPointIn.z());
    auto p = sm->GetG4Polyhedron();
    auto va = new G4VisAttributes(p->GetVisAttributes());
    if (in == kInside) {
      va->SetColor(1, 0, 0);
    }
    else if (in == kOutside) {
      va->SetColor(0, 1, 0);
    }
    p->SetVisAttributes(va);
    _debugPolyhedra[va] = p;

  }
}