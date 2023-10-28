#pragma once

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class CGALActionInitialization : public G4VUserActionInitialization
{
  public:
    CGALActionInitialization();
    virtual ~CGALActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

