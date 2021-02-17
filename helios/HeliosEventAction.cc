
#include "HeliosEventAction.hh"
#include "HeliosPSDSD.hh"
#include "HeliosPSDHit.hh"
#include "HeliosAnalysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include <iomanip>
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HeliosEventAction::HeliosEventAction()
 : G4UserEventAction(),
   fPSDHCID(-1),
   fNDet(24) // number of PSD, must be matched in HeliosDetectorConstruction
{
  fdEList.clear();
  fdLList.clear();
  for(int i = 0 ; i < fNDet; i++){
    fdEList.push_back(0.);
    fdLList.push_back(0.);
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HeliosEventAction::~HeliosEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HeliosPSDHitsCollection* HeliosEventAction::GetHitsCollection(G4int hcID, const G4Event* event) const
{
  auto hitsCollection  = static_cast<HeliosPSDHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID; 
    G4Exception("HeliosEventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }         

  return hitsCollection;
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HeliosEventAction::PrintEventStatistics( G4double absoEdep, G4double absoTrackLength) const
{
  // print event statistics
  G4cout
     << "HeliosEventAction::PrintEventStatistics" << G4endl
     << "   Absorber: total energy: " 
     << std::setw(7) << G4BestUnit(absoEdep, "Energy")
     << "       total track length: " 
     << std::setw(7) << G4BestUnit(absoTrackLength, "Length")
     << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HeliosEventAction::BeginOfEventAction(const G4Event* /*event*/)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HeliosEventAction::EndOfEventAction(const G4Event* event)
{

  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

  // periodic printing
  //G4cout << "######################################### HeliosEventAction::EndOfEventAction" << G4endl;
  //G4int eventID = event->GetEventID();
  //if ( eventID < 100 || eventID % 1000 == 0) {
  //  G4cout << ">>>>>>>> Event: " << eventID  << G4endl;
  //  if ( trajectoryContainer ) {
  //    G4cout << "    " << n_trajectories
  //           << " trajectories stored in this event." << G4endl;
  //  }
  //  G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
  //  G4cout << "    "  
  //         << hc->GetSize() << " hits stored in this event" << G4endl;
  //}

  G4PrimaryVertex * pv = event->GetPrimaryVertex();
  G4PrimaryParticle * pp = pv->GetPrimary();

  G4double beamEnergy = pp->GetKineticEnergy() * 1000; // to keV

  G4ThreeVector pmomt = pp->GetMomentumDirection();
  G4double beamTheta = pmomt.theta() / degree;
  G4double beamPhi = pmomt.phi() / degree;
  
  //G4cout << "Beam Energy : " << beamEnergy << G4endl;
  //       << "Beam theta : " << beamTheta << G4endl
  //       << "Beam phi   : " << beamPhi << G4endl;
    
  // Get hits collections IDs (only once)
  if ( fPSDHCID == -1 ) {
    fPSDHCID = G4SDManager::GetSDMpointer()->GetCollectionID("PSDHitsCollection");
  }

  // Get hits collections
  auto PSDHC = GetHitsCollection(fPSDHCID, event); //this is G4VHitsCollection

  for( int i = 0 ; i < fNDet ; i++){
    HeliosPSDHit * PSDHit = (*PSDHC)[i];  //this is HeliosPSDHit :: G4VHit

    //add detector resolution of 1%

    G4double edep = PSDHit->GetEdep();
    G4double resol = G4RandGauss::shoot(1, 0.001);

    fdEList[i] = edep * resol * 1000;  // to keV
    fdLList[i] = PSDHit->GetStepLength();

  }

  
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
 
  // fill ntuple
  analysisManager->FillNtupleIColumn(0, n_trajectories);

  analysisManager->FillNtupleDColumn(3, beamEnergy);
  analysisManager->FillNtupleDColumn(4, beamTheta);
  analysisManager->FillNtupleDColumn(5, beamPhi);
  
  analysisManager->AddNtupleRow();

}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
