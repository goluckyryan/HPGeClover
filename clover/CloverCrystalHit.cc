
#include "CloverCrystalHit.hh"

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<CloverCrystalHit>* CloverCrystalHitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverCrystalHit::CloverCrystalHit()
 : G4VHit(),
   fTrackID(-1),
   fCrysalID(-1),
   fEdep(0.),
   fPos(G4ThreeVector())
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverCrystalHit::~CloverCrystalHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverCrystalHit::CloverCrystalHit(const CloverCrystalHit& right)
  : G4VHit()
{
  fTrackID   = right.fTrackID;
  fCrysalID = right.fCrysalID;
  fEdep      = right.fEdep;
  fPos       = right.fPos;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const CloverCrystalHit& CloverCrystalHit::operator=(const CloverCrystalHit& right)
{
  fTrackID   = right.fTrackID;
  fCrysalID = right.fCrysalID;
  fEdep      = right.fEdep;
  fPos       = right.fPos;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool CloverCrystalHit::operator==(const CloverCrystalHit& right) const
{
  return ( this == &right ) ? true : false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CloverCrystalHit::Print()
{
  G4cout
     << "  trackID: " << fTrackID << " Crystal: " << fCrysalID << " |"
     << "Edep: "
     << std::setw(7) << G4BestUnit(fEdep,"Energy")
     << " Position: "
     << std::setw(7) << G4BestUnit( fPos,"Length")
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
