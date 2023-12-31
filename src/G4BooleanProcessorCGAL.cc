#include "G4BooleanProcessorCGAL.hh"

#include "G4Polyhedron.hh"
#include "G4PolyhedronArbitrary.hh"
#include "G4SurfaceMeshCGAL.hh"
#include "G4MultiUnion.hh"

// #define G4CGAL_DEBUG

G4PolyhedronArbitrary* G4BooleanProcessorCGAL::Intersection(G4Polyhedron* p1, G4Polyhedron* p2)
{
#ifdef G4CGAL_DEBUG
  G4cout << "G4BooleanProcessorCGAL::Intersection> " << iOperation << " " << p1 << " " << p2 << G4endl;
#endif

  G4SurfaceMeshCGAL* sm1 = new G4SurfaceMeshCGAL();
  G4SurfaceMeshCGAL* sm2 = new G4SurfaceMeshCGAL();

  sm1->Fill(p1);
  sm2->Fill(p2);

  G4bool valid = false;
  G4SurfaceMeshCGAL* sm3 = sm1->Intersection(sm2, valid);

  G4PolyhedronArbitrary* ap = sm3->GetPolyhedronArbitrary();

  iOperation ++;

  delete sm1;
  delete sm2;
  delete sm3;
  return ap;
}

G4PolyhedronArbitrary* G4BooleanProcessorCGAL::Union(G4Polyhedron* p1, G4Polyhedron* p2)
{
#ifdef G4CGAL_DEBUG
  G4cout << "G4BooleanProcessorCGAL::Union> " << iOperation << " " << p1 << " " << p2 << G4endl;
#endif

  G4SurfaceMeshCGAL* sm1 = new G4SurfaceMeshCGAL();
  G4SurfaceMeshCGAL* sm2 = new G4SurfaceMeshCGAL();

  sm1->Fill(p1);
  sm2->Fill(p2);

  G4bool valid = false;
  G4SurfaceMeshCGAL* sm3 = sm1->Union(sm2, valid);
  G4PolyhedronArbitrary* ap = sm3->GetPolyhedronArbitrary();
  iOperation++;

  delete sm1;
  delete sm2;
  delete sm3;
  return ap;
}

G4PolyhedronArbitrary* G4BooleanProcessorCGAL::Subtraction(G4Polyhedron* p1, G4Polyhedron* p2)
{
#ifdef G4CGAL_DEBUG
  G4cout << "G4BooleanProcessorCGAL::Subtraction> " << iOperation << " " << p1 << " " << p2 << G4endl;
#endif

  G4SurfaceMeshCGAL* sm1 = new G4SurfaceMeshCGAL();
  G4SurfaceMeshCGAL* sm2 = new G4SurfaceMeshCGAL();

  sm1->Fill(p1);
  sm2->Fill(p2);

  G4bool valid = false;
  G4SurfaceMeshCGAL* sm3 = sm1->Subtraction(sm2, valid);
  G4PolyhedronArbitrary* ap = sm3->GetPolyhedronArbitrary();
  iOperation++;

  delete sm1;
  delete sm2;
  delete sm3;

  return ap;
}

G4SurfaceMeshCGAL* G4BooleanProcessorCGAL::ProcessSurfaceMesh(const G4VSolid *bs) {

  // check if in cache
  //if(bs->GetName() != "placedB") {
  //  if (meshCache.find(std::string(bs->GetName())) != meshCache.end()) {
  //    G4cout << "G4BooleanProcessorCGAL::ProcessSurfaceMesh> returning cache " << bs->GetName() << G4endl;
  //    return meshCache[std::string(bs->GetName())];
  //  }
  //}

#ifdef G4CGAL_DEBUG
  G4cout << "G4BooleanProcessorCGAL::ProcessSurfaceMesh> " << bs->GetName() << " " << bs->GetEntityType() << G4endl;
#endif
  if(bs->GetEntityType() == "G4UnionSolid") {
    auto solid1 = bs->GetConstituentSolid(0);
    auto solid2 = bs->GetConstituentSolid(1);

#ifdef G4CGAL_DEBUG
    G4cout << "G4BooleanProcessorCGAL::ProcessSurfaceMesh> union " << bs->GetName() << " "
           << solid1->GetName() << " (" << solid1->GetEntityType() << ") "
           << ((G4DisplacedSolid*)solid2)->GetConstituentMovedSolid()->GetName() << " ("
           << ((G4DisplacedSolid*)solid2)->GetConstituentMovedSolid()->GetEntityType() << ") "
           << G4endl;
#endif
    auto sm1 = G4BooleanProcessorCGAL::ProcessSurfaceMesh(solid1);
    auto sm2 = G4BooleanProcessorCGAL::ProcessSurfaceMesh(solid2);

    G4bool valid = false;
    auto sm = sm1->Union(sm2, valid);

    if(!valid) {
      G4cout << "G4BooleanProcessorCGAL::ProcessSurfaceMesh> union failure " << solid1->GetName() << " " << solid2->GetName() << G4endl;
    }

    meshCache[bs->GetName()] = sm;
    return sm;
  }
  else if(bs->GetEntityType() == "G4IntersectionSolid") {
    auto solid1 = bs->GetConstituentSolid(0);
    auto solid2 = bs->GetConstituentSolid(1);

#ifdef G4CGAL_DEBUG
    G4cout << "G4BooleanProcessorCGAL::ProcessSurfaceMesh> intersection " << bs->GetName() << " "
           << solid1->GetName() << " (" << solid1->GetEntityType() << ") "
           << ((G4DisplacedSolid*)solid2)->GetConstituentMovedSolid()->GetName() << " ("
           << ((G4DisplacedSolid*)solid2)->GetConstituentMovedSolid()->GetEntityType() << ") "
           << G4endl;
#endif

    auto sm1 = G4BooleanProcessorCGAL::ProcessSurfaceMesh(solid1);
    auto sm2 = G4BooleanProcessorCGAL::ProcessSurfaceMesh(solid2);

    G4bool valid = false;
    auto sm = sm1->Intersection(sm2, valid);

    if(!valid) {
      G4cout << "G4BooleanProcessorCGAL::ProcessSurfaceMesh> intersection failure " << solid1->GetName() << " " << solid2->GetName() << G4endl;
    }

    meshCache[bs->GetName()] = sm;
    return sm;
  }
  else if(bs->GetEntityType() == "G4SubtractionSolid") {
    auto solid1 = bs->GetConstituentSolid(0);
    auto solid2 = bs->GetConstituentSolid(1);

#ifdef G4CGAL_DEBUG
    G4cout << "G4BooleanProcessorCGAL::ProcessSurfaceMesh> subtraction " << bs->GetName() << " "
           << solid1->GetName() << " (" << solid1->GetEntityType() << ") "
           << ((G4DisplacedSolid*)solid2)->GetConstituentMovedSolid()->GetName() << " ("
           << ((G4DisplacedSolid*)solid2)->GetConstituentMovedSolid()->GetEntityType() << ") "
           << G4endl;
#endif

    auto sm1 = G4BooleanProcessorCGAL::ProcessSurfaceMesh(solid1);
    auto sm2 = G4BooleanProcessorCGAL::ProcessSurfaceMesh(solid2);

    G4bool valid = false;
    auto sm = sm1->Subtraction(sm2, valid);

    if(!valid) {
      G4cout << "G4BooleanProcessorCGAL::ProcessSurfaceMesh> subtraction failure " << solid1->GetName() << " " << solid2->GetName() << G4endl;
    }

    meshCache[bs->GetName()] = sm;
    return sm;
  }
  else if(bs->GetEntityType() == "G4DisplacedSolid") {
#ifdef G4CGAL_DEBUG
    G4cout << "G4BooleanProcessorCGAL::ProcessSurfaceMesh> displaced " << bs->GetName() << G4endl;
#endif
    auto sm = ProcessSurfaceMesh( ((G4DisplacedSolid*)bs)->GetConstituentMovedSolid());
    auto at = ((G4DisplacedSolid*)bs)->GetTransform();
    auto r = at.NetRotation();
    auto t = at.NetTranslation();
    auto aa = r.axisAngle();
    auto a = aa.getAxis();
    auto ang = aa.getDelta();
    sm->Rotate(a,-ang);
    sm->Translate(-t);
    return sm;
  }
  else if(bs->GetEntityType() == "G4MultiUnion") {
    G4VSolid* solidA = ((G4MultiUnion*)bs)->GetSolid(0);

#ifdef G4CGAL_DEBUG
    G4cout << "G4BooleanProcessorCGAL::ProcessSurfaceMesh> multi union " << bs->GetName() << G4endl;
#endif

    auto solidA_SM = ProcessSurfaceMesh(solidA);

    const G4Transform3D transform0 = ((G4MultiUnion*)bs)->GetTransformation(0);
    G4DisplacedSolid dispSolidA("placedA", solidA, transform0);

    for (G4int i = 1; i < ((G4MultiUnion*)bs)->GetNumberOfSolids(); ++i)
    {
      G4VSolid* solidB = ((G4MultiUnion*)bs)->GetSolid(i);
      const G4Transform3D transform = ((G4MultiUnion*)bs)->GetTransformation(i);
      G4DisplacedSolid dispSolidB("placedB", solidB, transform);
      auto solidB_SM = ProcessSurfaceMesh(&dispSolidB);
      G4bool valid = false;
      solidA_SM = solidA_SM->Union(solidB_SM, valid);
    }
    meshCache[bs->GetName()] = solidA_SM;
    return solidA_SM;
  }
  else {

#ifdef G4CGAL_DEBUG
    G4cout << "G4BooleanProcessorCGAL::ProcessSurfaceMesh> other " << bs->GetEntityType() << G4endl;
#endif

    auto sm = new G4SurfaceMeshCGAL();
    sm->Fill(bs->GetPolyhedron());
    return sm;
  }
}

G4PolyhedronArbitrary* G4BooleanProcessorCGAL::Process(const G4VSolid *bs) {
  auto sm = ProcessSurfaceMesh(bs);
  return sm->GetPolyhedronArbitrary();
}

