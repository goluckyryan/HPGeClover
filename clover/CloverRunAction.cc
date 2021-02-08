
#include "CloverRunAction.hh"
#include "CloverAnalysis.hh"
#include "CloverEventAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverRunAction::CloverRunAction(CloverEventAction * eventAction)
//CloverRunAction::CloverRunAction()
 : G4UserRunAction()
 ,fEventAction(eventAction)
{ 
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

 /*
  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in CloverAnalysis.hh
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Create directories 
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
  
  // Creating ntuple
  //
  analysisManager->CreateNtuple("Clover", "Edep and TrackL");
  analysisManager->CreateNtupleDColumn("Eabs");
  analysisManager->CreateNtupleDColumn("Labs");
  analysisManager->CreateNtupleIColumn("nHit");
  analysisManager->CreateNtupleIColumn("DetID", fEventAction->GetDetList() );

  // event inital angle, energy
  analysisManager->CreateNtupleDColumn("theta");
  analysisManager->CreateNtupleDColumn("phi");
  analysisManager->CreateNtupleDColumn("e");
  
  analysisManager->FinishNtuple();
  *
  */
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverRunAction::~CloverRunAction()
{
  //delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CloverRunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  /*
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = "Clover.root";
  analysisManager->OpenFile(fileName);
  * */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CloverRunAction::EndOfRunAction(const G4Run* /*run*/)
{
  /*
  auto analysisManager = G4AnalysisManager::Instance();

  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
  */ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
