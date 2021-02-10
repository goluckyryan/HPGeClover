
#ifndef CloverPrimaryGeneratorAction_h
#define CloverPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

#include <vector>

class G4ParticleGun;
class G4Event;
class G4GenericMessenger;

class CloverEventAction;

/// The primary generator action class with particle gum.

class CloverPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  CloverPrimaryGeneratorAction(CloverEventAction * eventAction);    
  virtual ~CloverPrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event* event);
  
  // set methods
  void SetRandomFlag(G4bool value);

private:
  void DefineCommands();
  G4GenericMessenger* fMessenger;

  G4ParticleGun*  fParticleGun; // G4 particle gun

  G4double fAngle ; 

  CloverEventAction * fEventAction;

  std::vector<std::vector<G4double>> fEnergyList;

  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
