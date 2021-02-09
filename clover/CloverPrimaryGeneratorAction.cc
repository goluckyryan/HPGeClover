
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

#include <vector>


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
  auto particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(6.13*MeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0));


  //Set energy for 16N iso decay  in MeV
  energyList = {{0.120, 100}, //energy, branch
                {1.775, 0.114},
                {1.955, 0.036},
                {2.742, 0.777},
                {8.872, 0.072},
                {7.117, 5},
                {6.917, 0.036},
                {6.130, 68.0+0.777}};

  //accumulate the branch
  G4int numEnergy = (G4int) energyList.size();
  for( int i = 1 ; i < numEnergy ; i++){
    energyList[i][1] += energyList[i-1][1]; 
  }
  //renormalized the branching ratio
  for( int i = 0 ; i < numEnergy ; i++){
    energyList[i][1] = energyList[i][1]/energyList[numEnergy-1][1];
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverPrimaryGeneratorAction::~CloverPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CloverPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  // Set Angle Need to make it as MACRO
  //G4double cosAngle1 = cos(atan(0./20.));
  //G4double cosAngle2 = cos(atan(20./20.));
  //G4double minCosTheta = cosAngle1 > cosAngle2 ? cosAngle2 : cosAngle1;
  //G4double maxCosTheta = cosAngle1 > cosAngle2 ? cosAngle1 : cosAngle2;
  //G4double cosTheta = minCosTheta + abs(maxCosTheta- minCosTheta) * G4UniformRand();
  
  G4double cosTheta = 1.- (1.-cos(atan(10./25.)))*G4UniformRand();

  //G4double cosTheta = G4UniformRand(); // downstream
  
  G4double phi = 360*degree*G4UniformRand();

  G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
  G4double ux = sinTheta*std::cos(phi),
           uy = sinTheta*std::sin(phi),
           uz = cosTheta;
  
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));

  fParticleGun->GeneratePrimaryVertex(anEvent);


  //generate a random number form 0 to 1
  G4double pop = G4UniformRand();
  
  G4double beamEnergy = 6.13;
  
  G4int numEnergy = (G4int) energyList.size();
  for( int i = 0; i < numEnergy  ; i++){
    if( i == 0 &&   pop <= energyList[i][1] ) beamEnergy = energyList[0][0] ;
    if( i > 0 &&  energyList[i-1][1] < pop && pop <= energyList[i][1] ) beamEnergy = energyList[i][0] ;
  }
  
  fParticleGun->SetParticleEnergy(beamEnergy);
  
  G4double energy = fParticleGun->GetParticleEnergy();
 
  fEventAction->SaveEventTheta(acos(cosTheta) / degree);
  fEventAction->SaveEventPhi(phi/degree);
  fEventAction->SaveEventEnergy(energy);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

