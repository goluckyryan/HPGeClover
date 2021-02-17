
#ifndef HeliosPSDSD_h
#define HeliosPSDSD_h 1

#include "G4VSensitiveDetector.hh"

#include "HeliosPSDHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class HeliosPSDSD : public G4VSensitiveDetector
{
  public:
    HeliosPSDSD(const G4String& name, const G4String& hitsCollectionName, const G4int nPSD);
    virtual ~HeliosPSDSD();
  
    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
    HeliosPSDHitsCollection* fHitsCollection;

    G4int fNDet;
};

#endif

