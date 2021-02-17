

#include "HeliosActionInitialization.hh"
#include "HeliosPrimaryGeneratorAction.hh"
#include "HeliosRunAction.hh"
#include "HeliosEventAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HeliosActionInitialization::HeliosActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HeliosActionInitialization::~HeliosActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HeliosActionInitialization::BuildForMaster() const
{
  auto eventAction = new HeliosEventAction;
  SetUserAction(new HeliosRunAction(eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HeliosActionInitialization::Build() const
{


  auto eventAction = new HeliosEventAction;
  SetUserAction(eventAction);

  SetUserAction(new HeliosPrimaryGeneratorAction());
  SetUserAction(new HeliosRunAction(eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
