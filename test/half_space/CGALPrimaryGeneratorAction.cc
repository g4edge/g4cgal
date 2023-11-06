#include "CGALPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CGALPrimaryGeneratorAction::CGALPrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(), fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  fParticleGun->SetParticleDefinition(particleTable->FindParticle(particleName = "e-"));
  fParticleGun->SetParticleEnergy(1 * GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CGALPrimaryGeneratorAction::~CGALPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CGALPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

    //G4ThreeVector v(1, 0, 0);
    //fParticleGun->SetParticlePosition(G4ThreeVector(-75 * mm, 0*mm, 0*mm));

    G4ThreeVector v(1, 0, 0);
    fParticleGun->SetParticlePosition(G4ThreeVector(-75 * mm, 75*(G4UniformRand()-0.5) *mm, 75*(G4UniformRand()-0.5) * mm));

    //G4ThreeVector v(0, 1, 0);
    //fParticleGun->SetParticlePosition(G4ThreeVector(75*(G4UniformRand()-0.5) *mm, -75*mm, 75*(G4UniformRand()-0.5) * mm));

    //G4ThreeVector v(0, 0, 1);
    //fParticleGun->SetParticlePosition(G4ThreeVector(75*(G4UniformRand()-0.5) *mm, 75*(G4UniformRand()-0.5) * mm, -75*mm));

    //auto r = G4UniformRand();
    //G4ThreeVector v(0, 2*(r-0.5), 1-2*(r-0.5));
    //fParticleGun->SetParticlePosition(G4ThreeVector(10*(G4UniformRand()-0.5) *mm, 10*(G4UniformRand()-0.5) * mm, -0*mm));

    v = v/v.mag();

    //fParticleGun->SetParticlePosition(G4ThreeVector(-50 * mm, 0*mm, 0* mm));
    fParticleGun->SetParticleMomentumDirection(v);
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
