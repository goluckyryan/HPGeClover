
#include "HeliosRunAction.hh"
#include "HeliosAnalysis.hh"
#include "HeliosEventAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HeliosRunAction::HeliosRunAction(HeliosEventAction * eventAction)
//HeliosRunAction::HeliosRunAction()
 : G4UserRunAction()
 ,fEventAction(eventAction)
{ 
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in HeliosAnalysis.hh
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Create directories 
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
  
  // Creating ntuple
  //
  analysisManager->CreateNtuple("Helios", "Edep and TrackL");
  analysisManager->CreateNtupleIColumn("nTraj");
  analysisManager->CreateNtupleDColumn("energy", fEventAction->GetdEList() );
  analysisManager->CreateNtupleDColumn("stepLength", fEventAction->GetStepLengthList() );

  // event inital angle, energy
  analysisManager->CreateNtupleDColumn("bEnergy");
  analysisManager->CreateNtupleDColumn("theta");
  analysisManager->CreateNtupleDColumn("phi");
  
  analysisManager->FinishNtuple();
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HeliosRunAction::~HeliosRunAction()
{
  delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HeliosRunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //G4String fileName = "Helios.root";
  //analysisManager->OpenFile(fileName);
  analysisManager->OpenFile();
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HeliosRunAction::EndOfRunAction(const G4Run* /*run*/)
{
  
  auto analysisManager = G4AnalysisManager::Instance();

  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
