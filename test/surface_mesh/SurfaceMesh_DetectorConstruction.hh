#pragma once

#include "G4VUserDetectorConstruction.hh"

class SurfaceMesh_DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    SurfaceMesh_DetectorConstruction() = default;

    virtual G4VPhysicalVolume* Construct();

  private:
    G4VPhysicalVolume* fWorld;
};