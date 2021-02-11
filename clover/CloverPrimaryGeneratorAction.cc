
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
#include "G4GenericMessenger.hh"
#include "Randomize.hh"

#include <vector>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverPrimaryGeneratorAction::CloverPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fMessenger(nullptr),
   fParticleGun(nullptr),
   fAngle(180 * degree)
   //,fEventAction(eventAction)
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
  fEnergyList = {{0.120, 100}, //energy, branch
                {1.775, 0.114},
                {1.955, 0.036},
                {2.742, 0.777},
                {8.872, 0.072},
                {7.117, 5},
                {6.917, 0.036},
                {6.130, 68.0+0.777}};

  //accumulate the branch
  G4int numEnergy = (G4int) fEnergyList.size();
  for( int i = 1 ; i < numEnergy ; i++){
    fEnergyList[i][1] += fEnergyList[i-1][1]; 
  }
  //renormalized the branching ratio
  for( int i = 0 ; i < numEnergy ; i++){
    fEnergyList[i][1] = fEnergyList[i][1]/fEnergyList[numEnergy-1][1];
  }

  //Define command for this class
  DefineCommands();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverPrimaryGeneratorAction::~CloverPrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CloverPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  G4double cosTheta = 1.- (1.-cos(fAngle ))*G4UniformRand();

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
  
  G4int numEnergy = (G4int) fEnergyList.size();
  for( int i = 0; i < numEnergy  ; i++){
    if( i == 0 &&   pop <= fEnergyList[i][1] ) beamEnergy = fEnergyList[0][0] ;
    if( i > 0 &&  fEnergyList[i-1][1] < pop && pop <= fEnergyList[i][1] ) beamEnergy = fEnergyList[i][0] ;
  }
  
  fParticleGun->SetParticleEnergy(beamEnergy);

  //G4double energy = fParticleGun->GetParticleEnergy();
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void CloverPrimaryGeneratorAction::DefineCommands()
{
  // Define /Clover/generator command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this, "/Clover/generator/", "Primary generator control");

  // OpeningAngle command
  auto& openingAngleCmd = fMessenger->DeclarePropertyWithUnit("OpeningAngle", "deg", fAngle, "Opening Angle of beam in deg");
  openingAngleCmd.SetParameterName("t", true);
  openingAngleCmd.SetRange("t>=0.");
  openingAngleCmd.SetDefaultValue("180.");

  

}

