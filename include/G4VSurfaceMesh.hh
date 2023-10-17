#pragma once

#include "G4Types.hh"

class G4Polyhedron;
class G4PolyhedronArbitrary;
class vtkPolyData;

class G4VSurfaceMesh
{
public:
    G4VSurfaceMesh();
    virtual ~G4VSurfaceMesh();
    virtual void fill(G4Polyhedron *polyIn);
    virtual G4PolyhedronArbitrary* GetPolyhedronArbitrary();
    virtual void AddVertex(double x, double y, double z) = 0 ;
    virtual void AddFace(G4int i1, G4int i2, G4int i3) = 0;
    virtual void AddFace(G4int i1, G4int i2, G4int i3, G4int i4) = 0;
    virtual std::vector<G4double> GetVertex(G4int iVertex) = 0;
    virtual std::vector<G4int> GetFace(G4int iFace) = 0;
    virtual int NumberOfVertices() = 0;
    virtual int NumberOfFaces() = 0;
};

