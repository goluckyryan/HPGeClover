
#ifndef HeliosPhysicsList_h
#define HeliosPhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HeliosPhysicsList: public G4VUserPhysicsList
{
  public:
    HeliosPhysicsList();
   ~HeliosPhysicsList();

  protected:
    // Construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();
 
    virtual void SetCuts();
    
  protected:
    // these methods Construct particles 
    void ConstructBosons();
    void ConstructLeptons();

  protected:
  // these methods Construct physics processes and register them
    void ConstructGeneral();
    void ConstructEM();
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

