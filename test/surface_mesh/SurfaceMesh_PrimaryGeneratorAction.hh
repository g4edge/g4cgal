#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4Event;
class G4ParticleGun;

/// Minimal primary generator action to demonstrate the use of GDML geometries

class SurfaceMesh_PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  SurfaceMesh_PrimaryGeneratorAction();
  ~SurfaceMesh_PrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event* anEvent);

private:
  G4ParticleGun* fParticleGun;
};