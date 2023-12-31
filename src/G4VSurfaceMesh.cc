#include "G4VSurfaceMesh.hh"

#include "G4Polyhedron.hh"
#include "G4PolyhedronArbitrary.hh"

#include <map>

G4VSurfaceMesh::G4VSurfaceMesh() {}

void G4VSurfaceMesh::Fill(G4Polyhedron* polyIn)
{
  int nVert = polyIn->GetNoVertices();
  int nFacet = polyIn->GetNoFacets();

  std::map<G4int, G4int> vertMap;

  for (int iVert = 1; iVert <= nVert; iVert++) {
    G4Point3D v = polyIn->GetVertex(iVert);
    vertMap[iVert] = AddVertex(v.x(), v.y(), v.z());
  }

  G4int nNode = 0;
  G4int* iNode = new G4int[4];

  for (int iFacet = 1; iFacet <= nFacet; iFacet++) {
    polyIn->GetFacet(iFacet, nNode, iNode);
    if (nNode == 3) {
      AddFace(vertMap[iNode[0]],
              vertMap[iNode[1]],
              vertMap[iNode[2]]);
    }
    else if (nNode == 4) {
      AddFace(vertMap[iNode[0]],
              vertMap[iNode[1]],
              vertMap[iNode[2]],
              vertMap[iNode[3]]);
    }
    else {
      G4cout << "G4VSurfaceMesh> not 3 or 4 vertices in face" << G4endl;
    }
  }
  delete[] iNode;
}

G4VSurfaceMesh::~G4VSurfaceMesh() {}

G4Polyhedron* G4VSurfaceMesh::GetG4Polyhedron() {
  G4Polyhedron *poly = new G4Polyhedron(NumberOfVertices(),
                                        NumberOfFaces());

  for(G4int iVert = 0; iVert< NumberOfVertices();iVert++) {
    auto vert = GetVertex(iVert);
    poly->SetVertex(iVert+1,G4Point3D(vert[0],vert[1],vert[2]));
  }

  for(G4int iFace = 0; iFace< NumberOfFaces(); iFace++) {
    auto face = GetFace(iFace);
    if(face.size() == 3) {
      poly->SetFacet(iFace+1,
                     face[0]+1, face[1]+1,
                     face[2]+1);
    }
    else if(face.size() == 4) {
      poly->SetFacet(iFace+1,
                     face[0]+1,
                     face[1]+1,
                     face[2]+1,
                     face[3]+1);
    }
  }
  poly->SetReferences();

  return poly;
}


G4PolyhedronArbitrary* G4VSurfaceMesh::GetPolyhedronArbitrary()
{
  G4PolyhedronArbitrary* poly = new G4PolyhedronArbitrary(NumberOfVertices(), NumberOfFaces());

  for (auto i = 0; i < NumberOfVertices(); i++) {
    auto v = GetVertex(i);
    G4ThreeVector v3 = G4ThreeVector(v[0], v[1], v[2]);
    poly->AddVertex(v3);
  }

  for (auto i = 0; i < NumberOfFaces(); i++) {
    auto f = GetFace(i);
    if (f.size() == 3) {
      poly->AddFacet(f[0] + 1,
                     f[1] + 1,
                     f[2] + 1);
    }
    else if (f.size() == 4) {
      poly->AddFacet(f[0] + 1,
                     f[1] + 1,
                     f[2] + 1 ,
                     f[3] + 1);
    }
    else {
      G4cout << "PolyhedronArbitrary> face with >4 vertices " << G4endl;
    }
  }
  poly->SetReferences();
  return poly;
}