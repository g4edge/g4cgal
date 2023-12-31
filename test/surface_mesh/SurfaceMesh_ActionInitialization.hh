#pragma once

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class SurfaceMesh_ActionInitialization : public G4VUserActionInitialization
{
public:
  SurfaceMesh_ActionInitialization();
  virtual ~SurfaceMesh_ActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;
};