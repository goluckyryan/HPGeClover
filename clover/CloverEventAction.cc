
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
   fAbsHCID(-1)
{
  fDetList.clear();
  fdEList.clear();
  fdLList.clear();

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

  G4int eventID = event->GetEventID();
  if ( eventID < 100 || eventID % 100 == 0) {
    G4cout << ">>> Event: " << eventID  << G4endl;
    if ( trajectoryContainer ) {
      G4cout << "    " << n_trajectories
             << " trajectories stored in this event." << G4endl;
    }
    G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
    G4cout << "    "  
           << hc->GetSize() << " hits stored in this event" << G4endl;
  }

  /*
  G4cout << "######################################### CloverEventAction::EndOfEventAction" << G4endl;

  // Get hits collections IDs (only once)
  if ( fAbsHCID == -1 ) {
    fAbsHCID = G4SDManager::GetSDMpointer()->GetCollectionID("CrystalHitsCollection");
  }

  // Get hits collections
  auto absoHC = GetHitsCollection(fAbsHCID, event); //this is G4VHitsCollection

  G4cout
    << "Enegry : " <<  eventEnergy << G4endl
    << "Theta  : " <<  eventTheta << G4endl
    << "Phi    : " <<  eventPhi << G4endl;
    
  

  // Get hit with total values
  G4cout << "=========== number of Hit Collection : " << absoHC->entries() << G4endl;

  auto absoHit = (*absoHC)[absoHC->entries()-1];  //this is CloverCalorHit :: G4VHit

  int nHit = absoHit->GetNumHit();
  fDetList = absoHit->GetDetID();

  // Print per event (modulo n)
  auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "================> End of event Summary, eventID: " << eventID << G4endl;

    G4cout << "Number of hit : " << nHit << G4endl;    

    PrintEventStatistics( absoHit->GetEdep(), absoHit->GetTrackLength());

    for(int i = 0; i < nHit ; i ++ ){
      G4cout << fDetList[i] << " " ;
    }
    G4cout << G4endl;

    G4cout << "######################################" <<  G4endl;     
  }  

  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
 
  // fill histograms
  //analysisManager->FillH1(0, absoHit->GetEdep());
  //analysisManager->FillH1(1, absoHit->GetTrackLength());
  
  // fill ntuple

  //add detector resolution of 1%
  
  analysisManager->FillNtupleDColumn(0, absoHit->GetEdep());
  analysisManager->FillNtupleDColumn(1, absoHit->GetTrackLength());
  analysisManager->FillNtupleDColumn(2, absoHit->GetNumHit()+1);

  analysisManager->FillNtupleDColumn(4, eventTheta);
  analysisManager->FillNtupleDColumn(5, eventPhi);
  analysisManager->FillNtupleDColumn(6, eventEnergy);
  
  analysisManager->AddNtupleRow();
  *
  */ 
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
