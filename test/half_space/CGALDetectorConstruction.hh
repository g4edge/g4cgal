#pragma once

#include "G4VUserDetectorConstruction.hh"

class CGALDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    CGALDetectorConstruction() = default;

    virtual G4VPhysicalVolume* Construct();

  private:
    G4VPhysicalVolume* fWorld;
};