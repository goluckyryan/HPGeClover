

#include "CloverActionInitialization.hh"
#include "CloverPrimaryGeneratorAction.hh"
#include "CloverRunAction.hh"
#include "CloverEventAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverActionInitialization::CloverActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverActionInitialization::~CloverActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CloverActionInitialization::BuildForMaster() const
{
  auto eventAction = new CloverEventAction;
  SetUserAction(new CloverRunAction(eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CloverActionInitialization::Build() const
{


  auto eventAction = new CloverEventAction;
  SetUserAction(eventAction);

  SetUserAction(new CloverPrimaryGeneratorAction(eventAction));
  SetUserAction(new CloverRunAction(eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
