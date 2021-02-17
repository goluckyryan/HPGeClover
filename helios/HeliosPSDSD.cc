
#include "HeliosPSDSD.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

///....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HeliosPSDSD::HeliosPSDSD(const G4String& name, const G4String& hitsCollectionName, const G4int nPSD)
 : G4VSensitiveDetector(name),
   fHitsCollection(nullptr),
   fNDet(nPSD)
{
  collectionName.insert(hitsCollectionName);
}

///....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HeliosPSDSD::~HeliosPSDSD() 
{ 
}

///....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HeliosPSDSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection = new HeliosPSDHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);  
  hce->AddHitsCollection( hcID, fHitsCollection );

  for (G4int i=0; i<fNDet; i++ ) {
    fHitsCollection->insert(new HeliosPSDHit());
  }

  //G4cout << "######### size of fHitCollection : " <<  fHitsCollection->GetSize() << G4endl;

}

///....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool HeliosPSDSD::ProcessHits(G4Step* step, G4TouchableHistory* /*history*/)
{

  // energy deposit
  G4double edep = step->GetTotalEnergyDeposit();
  
  // step length
  G4double stepLength = 0.;
  if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
    stepLength = step->GetStepLength();    
  }

  if ( edep==0. && stepLength == 0. ) return false;

  G4int PSDID = step->GetPreStepPoint()->GetTouchableHandle() ->GetCopyNumber();
  //----------- save hit in each PSD
  HeliosPSDHit * hit = (*fHitsCollection)[PSDID];

  hit->SetTrackID  (step->GetTrack()->GetTrackID());
  hit->SetEdep(edep);
  hit->SetPos (step->GetPostStepPoint()->GetPosition());
  hit->SetStepLength( stepLength);
  hit->SetPSDID(PSDID);


  //---------- Save indivual hit
  HeliosPSDHit* newHit = new HeliosPSDHit();
  
  newHit->SetTrackID  (step->GetTrack()->GetTrackID());
  newHit->SetPSDID(PSDID);
  newHit->SetEdep(edep);
  newHit->SetPos (step->GetPostStepPoint()->GetPosition());
  newHit->SetStepLength( stepLength);
  
  fHitsCollection->insert( newHit );


  return true;
}

///....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HeliosPSDSD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel > 1 ) { 
     G4int nofHits = fHitsCollection->GetSize();
     G4cout
       << G4endl 
       << ">>>>>>>>>>Hits Collection: in this event they are " << nofHits 
       << " hits in the tracker chambers: " << G4endl;
     for ( G4int i=0; i<nofHits; ++i ) {
       if( i == fNDet ) G4cout << "---------------------------" << G4endl;
       G4cout << i << " |";
       (*fHitsCollection)[i]->Print();
     }
  }
}

///....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
