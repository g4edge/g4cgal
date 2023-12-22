#pragma once

#include <string>
#include <map>
#include "G4Polyhedron.hh"
#include "G4PolyhedronArbitrary.hh"
#include "G4VBooleanProcessor.hh"
#include "G4BooleanSolid.hh"
#include "G4SurfaceMeshCGAL.hh"
#include "G4VSolid.hh"

class G4BooleanProcessorCGAL : public G4VBooleanProcessor
{
private:
  G4int iOperation = 0;
  std::map<std::string,G4SurfaceMeshCGAL *>  meshCache;

public:
    G4BooleanProcessorCGAL() {}
    virtual ~G4BooleanProcessorCGAL() = default;

  virtual G4PolyhedronArbitrary* Intersection(G4Polyhedron* p1, G4Polyhedron* p2);
  virtual G4PolyhedronArbitrary* Union(G4Polyhedron* p1, G4Polyhedron* p2);
  virtual G4PolyhedronArbitrary* Subtraction(G4Polyhedron* p1, G4Polyhedron* p2);
  virtual G4SurfaceMeshCGAL* ProcessSurfaceMesh(const G4VSolid *s);
  virtual G4PolyhedronArbitrary* Process(const G4VSolid *s);
};