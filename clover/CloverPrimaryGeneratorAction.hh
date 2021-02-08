
#ifndef CloverPrimaryGeneratorAction_h
#define CloverPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;

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
  G4ParticleGun*  fParticleGun; // G4 particle gun

  CloverEventAction * fEventAction;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
