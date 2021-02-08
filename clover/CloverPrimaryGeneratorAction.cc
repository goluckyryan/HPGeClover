
#include "CloverPrimaryGeneratorAction.hh"
#include "CloverEventAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverPrimaryGeneratorAction::CloverPrimaryGeneratorAction(CloverEventAction * eventAction)
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(nullptr),
   fEventAction(eventAction)
{
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic
  //
  auto particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("e-");
  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(50.*MeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverPrimaryGeneratorAction::~CloverPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CloverPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  // Need to make it as MACRO
  G4double cosAngle1 = cos(atan(3./20.));
  G4double cosAngle2 = cos(atan(5./20.));
  
  G4double minCosTheta = cosAngle1 > cosAngle2 ? cosAngle2 : cosAngle1;
  G4double maxCosTheta = cosAngle1 > cosAngle2 ? cosAngle1 : cosAngle2;
  
  G4double cosTheta = minCosTheta + abs(maxCosTheta- minCosTheta) * G4UniformRand();
  
  //G4cout << "Theta ====== " << acos(cosTheta) / degree << G4endl;

  //G4double cosTheta = 1.- (1.-cos(atan(5/17.)))*G4UniformRand();
  //G4double cosTheta = G4UniformRand(); // downstream

  
  G4double phi = 360*degree*G4UniformRand();
  //G4cout << "Phi ====== " << phi / degree << G4endl;

  
  G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
  G4double ux = sinTheta*std::cos(phi),
           uy = sinTheta*std::sin(phi),
           uz = cosTheta;
  
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));

  fParticleGun->GeneratePrimaryVertex(anEvent);

  
  G4double energy = fParticleGun->GetParticleEnergy();
  //G4cout << "energy ====== " << energy << G4endl;

  fEventAction->SaveEventTheta(acos(cosTheta) / degree);
  fEventAction->SaveEventPhi(phi/degree);
  fEventAction->SaveEventEnergy(energy);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

