#include "CGALPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CGALPrimaryGeneratorAction::CGALPrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(), fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  fParticleGun->SetParticleDefinition(particleTable->FindParticle(particleName = "e-"));
  fParticleGun->SetParticleEnergy(10 * GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CGALPrimaryGeneratorAction::~CGALPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CGALPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4ThreeVector v(1, 0, 0);

  fParticleGun->SetParticlePosition(G4ThreeVector(-10 * m, 0 * m, 0 * m));
  fParticleGun->SetParticleMomentumDirection(v);
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
