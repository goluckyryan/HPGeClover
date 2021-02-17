
#ifndef HeliosRunAction_h
#define HeliosRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class HeliosEventAction;

class G4Run;


class HeliosRunAction : public G4UserRunAction
{
  public:
    HeliosRunAction(HeliosEventAction * eventAction);
    virtual ~HeliosRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

  private:
    HeliosEventAction * fEventAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

