#include "G4VSurfaceMesh.hh"
#include "G4Polyhedron.hh"
#include "G4PolyhedronArbitrary.hh"

G4VSurfaceMesh::G4VSurfaceMesh()
{}

void G4VSurfaceMesh::fill(G4Polyhedron *polyIn) {
    int nVert = polyIn->GetNoVertices();
    int nFacet = polyIn->GetNoFacets();

    for (int iVert = 1; iVert <= nVert; iVert++) {
        G4Point3D v = polyIn->GetVertex(iVert);
        AddVertex(v.x(), v.y(), v.z());
    }

    G4int nNode = 0;
    G4int *iNode = new G4int[4];

    for (int iFacet = 1; iFacet <= nFacet; iFacet++) {
        polyIn->GetFacet(iFacet, nNode, iNode);
        if (nNode == 3) {
            AddFace(iNode[0] - 1,iNode[1] - 1,iNode[2] - 1);
        }
        else if (nNode == 4) {
            AddFace(iNode[0] - 1,iNode[1] - 1,iNode[2] - 1,iNode[3]-1);
        }
        else {
            G4cout << "G4VSurfaceMesh > not 3 or 4 vertices in face" << G4endl;
        }
    }
    delete[] iNode;
}

G4VSurfaceMesh::~G4VSurfaceMesh()
{}

G4PolyhedronArbitrary* G4VSurfaceMesh::GetPolyhedronArbitrary() {
    G4PolyhedronArbitrary *poly = new G4PolyhedronArbitrary(NumberOfVertices(),NumberOfFaces());

    for(auto i = 0; i < NumberOfVertices(); i++) {
        auto v = GetVertex(i);
        G4ThreeVector v3 = G4ThreeVector(v[0],v[1],v[2]);
        poly->AddVertex(v3);
    }

    for(auto i = 0; i < NumberOfFaces(); i++) {
        auto f = GetFace(i);
        if (f.size() == 3) {
            poly->AddFacet(f[0]+1,f[1]+1,+f[2]+1);
        }
        else if(f.size() == 4) {
            poly->AddFacet(f[0]+1,f[1]+1,f[2]+1,f[3]+1);
        }
    }
    poly->SetReferences();
    return poly;
}