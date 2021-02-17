
#include "HeliosPSDHit.hh"

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<HeliosPSDHit>* HeliosPSDHitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HeliosPSDHit::HeliosPSDHit()
 : G4VHit(),
   fTrackID(-1),
   fCrysalID(-1),
   fEdep(0.),
   fPos(G4ThreeVector()),
   fStepLength(0.),
   fBeamEnergy(0.)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HeliosPSDHit::~HeliosPSDHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HeliosPSDHit::HeliosPSDHit(const HeliosPSDHit& right)
  : G4VHit()
{
  fTrackID   = right.fTrackID;
  fCrysalID  = right.fCrysalID;
  fEdep      = right.fEdep;
  fPos       = right.fPos;

  fStepLength = right.fStepLength;
  fBeamEnergy = right.fBeamEnergy;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const HeliosPSDHit& HeliosPSDHit::operator=(const HeliosPSDHit& right)
{
  fTrackID   = right.fTrackID;
  fCrysalID  = right.fCrysalID;
  fEdep      = right.fEdep;
  fPos       = right.fPos;

  fStepLength = right.fStepLength;
  fBeamEnergy = right.fBeamEnergy;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool HeliosPSDHit::operator==(const HeliosPSDHit& right) const
{
  return ( this == &right ) ? true : false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HeliosPSDHit::Print()
{
  G4cout
     << "Beam Energy : " << fBeamEnergy
     << "  trackID: " << fTrackID << " PSD: " << fCrysalID 
     << "  Edep: "
     << std::setw(7) << G4BestUnit(fEdep,"Energy")
     << "  StepLen: "
     << std::setw(7) << G4BestUnit(fStepLength,"Length")
     << "  Position: "
     << std::setw(7) << G4BestUnit( fPos,"Length")
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
