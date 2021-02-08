
#ifndef CloverRunAction_h
#define CloverRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class CloverEventAction;

class G4Run;


class CloverRunAction : public G4UserRunAction
{
  public:
    CloverRunAction(CloverEventAction * eventAction);
    virtual ~CloverRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

  private:
    CloverEventAction * fEventAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

