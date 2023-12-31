#include "SurfaceMesh_ActionInitialization.hh"
#include "SurfaceMesh_PrimaryGeneratorAction.hh"

SurfaceMesh_ActionInitialization::SurfaceMesh_ActionInitialization() : G4VUserActionInitialization() {}

SurfaceMesh_ActionInitialization::~SurfaceMesh_ActionInitialization() {}

void SurfaceMesh_ActionInitialization::BuildForMaster() const {}

void SurfaceMesh_ActionInitialization::Build() const
{
  SetUserAction(new SurfaceMesh_PrimaryGeneratorAction);
}

