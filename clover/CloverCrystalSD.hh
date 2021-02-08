
#ifndef CloverCrystalSD_h
#define CloverCrystalSD_h 1

#include "G4VSensitiveDetector.hh"

#include "CloverCrystalHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class CloverCrystalSD : public G4VSensitiveDetector
{
  public:
    CloverCrystalSD(const G4String& name, const G4String& hitsCollectionName);
    virtual ~CloverCrystalSD();
  
    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
    CloverCrystalHitsCollection* fHitsCollection;
};

#endif

