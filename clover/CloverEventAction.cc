
#include "CloverEventAction.hh"
#include "CloverCrystalSD.hh"
#include "CloverCrystalHit.hh"
#include "CloverAnalysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverEventAction::CloverEventAction()
 : G4UserEventAction(),
   fCrystalHCID(-1),
   fNDet(4) // number of crystal, must be matched in CloverDetectorConstruction
{
  fdEList.clear();
  fdLList.clear();
  for(int i = 0 ; i < fNDet; i++){
    fdEList.push_back(0.);
    fdLList.push_back(0.);
  }

  eventTheta = 0;
  eventPhi   = 0;
  eventEnergy = 0;

  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverEventAction::~CloverEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverCrystalHitsCollection* CloverEventAction::GetHitsCollection(G4int hcID, const G4Event* event) const
{
  auto hitsCollection  = static_cast<CloverCrystalHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID; 
    G4Exception("CloverEventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }         

  return hitsCollection;
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CloverEventAction::PrintEventStatistics( G4double absoEdep, G4double absoTrackLength) const
{
  // print event statistics
  G4cout
     << "CloverEventAction::PrintEventStatistics" << G4endl
     << "   Absorber: total energy: " 
     << std::setw(7) << G4BestUnit(absoEdep, "Energy")
     << "       total track length: " 
     << std::setw(7) << G4BestUnit(absoTrackLength, "Length")
     << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CloverEventAction::BeginOfEventAction(const G4Event* /*event*/)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CloverEventAction::EndOfEventAction(const G4Event* event)
{

  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

  // periodic printing
//  G4cout << "######################################### CloverEventAction::EndOfEventAction" << G4endl;
//  G4int eventID = event->GetEventID();
//  if ( eventID < 100 || eventID % 100 == 0) {
//    G4cout << ">>>>>>>> Event: " << eventID  << G4endl;
//    if ( trajectoryContainer ) {
//      G4cout << "    " << n_trajectories
//             << " trajectories stored in this event." << G4endl;
//    }
//    G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
//    G4cout << "    "  
//           << hc->GetSize() << " hits stored in this event" << G4endl;
//  }

  //G4cout
  //  << "Enegry : " <<  G4BestUnit( eventEnergy,"Energy") << G4endl
  //  << "Theta  : " <<  eventTheta << G4endl
  //  << "Phi    : " <<  eventPhi << G4endl;
    
  
  // Get hits collections IDs (only once)
  if ( fCrystalHCID == -1 ) {
    fCrystalHCID = G4SDManager::GetSDMpointer()->GetCollectionID("CrystalHitsCollection");
  }

  // Get hits collections
  auto crystalHC = GetHitsCollection(fCrystalHCID, event); //this is G4VHitsCollection

  for( int i = 0 ; i < fNDet ; i++){
    CloverCrystalHit * crystalHit = (*crystalHC)[i];  //this is CloverCrystalHit :: G4VHit

    //add detector resolution of 1%

    G4double edep = crystalHit->GetEdep();
    G4double resol = G4RandGauss::shoot(1, 0.001);

    fdEList[i] = edep * resol;
    fdLList[i] = crystalHit->GetStepLength();

  }

  
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
 
  // fill ntuple
  analysisManager->FillNtupleIColumn(0, n_trajectories);

  analysisManager->FillNtupleDColumn(3, eventTheta);
  analysisManager->FillNtupleDColumn(4, eventPhi);
  analysisManager->FillNtupleDColumn(5, eventEnergy);
  
  analysisManager->AddNtupleRow();

}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
