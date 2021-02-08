
#include "CloverCrystalSD.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverCrystalSD::CloverCrystalSD(const G4String& name, const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name),
   fHitsCollection(nullptr)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverCrystalSD::~CloverCrystalSD() 
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CloverCrystalSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection = new CloverCrystalHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);  
  hce->AddHitsCollection( hcID, fHitsCollection ); 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool CloverCrystalSD::ProcessHits(G4Step* step, G4TouchableHistory* /*history*/)
{

  // energy deposit
  G4double edep = step->GetTotalEnergyDeposit();
  
  // step length
  G4double stepLength = 0.;
  if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
    stepLength = step->GetStepLength();    
  }

  if ( edep==0. && stepLength == 0. ) return false;

  CloverCrystalHit* newHit = new CloverCrystalHit();

  newHit->SetTrackID  (step->GetTrack()->GetTrackID());
  newHit->SetCrystalID(step->GetPreStepPoint()->GetTouchableHandle() ->GetCopyNumber());
  newHit->SetEdep(edep);
  newHit->SetPos (step->GetPostStepPoint()->GetPosition());

  fHitsCollection->insert( newHit );

/*

  auto touchable = (step->GetPreStepPoint()->GetTouchable());
    
  // Get calorimeter cell id 
  auto layerNumber = touchable->GetReplicaNumber(1);

  detID = touchable->GetCopyNumber() ;

  G4cout << "******************* " << detID <<G4endl;
  
  // Get hit accounting data for this cell
  auto hit = (*fHitsCollection)[layerNumber];
  if ( ! hit ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hit " << layerNumber; 
    G4Exception("CloverCrystalSD::ProcessHits()",
      "MyCode0004", FatalException, msg);

  }else{

  }         

  // Get hit for total accounting
  //auto hitTotal = (*fHitsCollection)[fHitsCollection->entries()-1];
  
  // Add values
  hit->Add(edep, stepLength, detID);
  //hitTotal->Add(edep, stepLength, detID);

*/

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CloverCrystalSD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel > -1 ) { 
     G4int nofHits = fHitsCollection->entries();
     G4cout
       << G4endl 
       << "-------->Hits Collection: in this event they are " << nofHits 
       << " hits in the tracker chambers: " << G4endl;
     for ( G4int i=0; i<nofHits; ++i ) (*fHitsCollection)[i]->Print();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
