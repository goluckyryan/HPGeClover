
#ifndef CloverCrystalHit_h
#define CloverCrystalHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

#include <vector>

/// Calorimeter hit class
///
/// It defines data members to store the the energy deposit and track lengths
/// of charged particles in a selected volume:
/// - fEdep, fTrackLength

class CloverCrystalHit : public G4VHit
{
  public:
    CloverCrystalHit();
    CloverCrystalHit(const CloverCrystalHit&);
    virtual ~CloverCrystalHit();

    // operators
    const CloverCrystalHit& operator=(const CloverCrystalHit&);
    G4bool operator==(const CloverCrystalHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print();

    // Set methods
    void SetTrackID    (G4int track)      { fTrackID = track; };
    void SetCrystalID  (G4int id)         { fCrysalID = id; };
    void SetEdep       (G4double de)      { fEdep += de; };
    void SetPos        (G4ThreeVector xyz){ fPos = xyz; };
    
    void SetStepLength (G4double sl)      { fStepLength += sl; };
    void SetBeamEnergy (G4double be)      { fBeamEnergy = be;};

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4int GetChamberNb() const   { return fCrysalID; };
    G4double GetEdep() const     { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };

    G4double GetStepLength () const  { return fStepLength ; };
    G4double GetBeamEnergy () const  { return fBeamEnergy ; };

  private:

    G4int         fTrackID;
    G4int         fCrysalID;
    G4double      fEdep;
    G4ThreeVector fPos;
    G4double      fStepLength;
    G4double      fBeamEnergy;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using CloverCrystalHitsCollection = G4THitsCollection<CloverCrystalHit>;

extern G4ThreadLocal G4Allocator<CloverCrystalHit>* CloverCrystalHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* CloverCrystalHit::operator new(size_t)
{
  if (!CloverCrystalHitAllocator) CloverCrystalHitAllocator = new G4Allocator<CloverCrystalHit>;
  
  return (void *) CloverCrystalHitAllocator->MallocSingle();
}

inline void CloverCrystalHit::operator delete(void *hit)
{
  if (!CloverCrystalHitAllocator) CloverCrystalHitAllocator = new G4Allocator<CloverCrystalHit>;
  
  CloverCrystalHitAllocator->FreeSingle((CloverCrystalHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
