//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// 
/// \file B4cEventAction.cc
/// \brief Implementation of the B4cEventAction class

#include "B4cEventAction.hh"
#include "B4cCalorimeterSD.hh"
#include "B4cCalorHit.hh"
#include "B4Analysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4cEventAction::B4cEventAction()
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

B4cEventAction::~B4cEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4cCalorHitsCollection* B4cEventAction::GetHitsCollection(G4int hcID, const G4Event* event) const
{
  auto hitsCollection  = static_cast<B4cCalorHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID; 
    G4Exception("B4cEventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }         

  return hitsCollection;
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4cEventAction::PrintEventStatistics( G4double absoEdep, G4double absoTrackLength) const
{
  // print event statistics
  G4cout
     << "B4cEventAction::PrintEventStatistics" << G4endl
     << "   Absorber: total energy: " 
     << std::setw(7) << G4BestUnit(absoEdep, "Energy")
     << "       total track length: " 
     << std::setw(7) << G4BestUnit(absoTrackLength, "Length")
     << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4cEventAction::BeginOfEventAction(const G4Event* /*event*/)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4cEventAction::EndOfEventAction(const G4Event* event)
{

  G4cout << "######################################### B4cEventAction::EndOfEventAction" << G4endl;

  // Get hits collections IDs (only once)
  if ( fAbsHCID == -1 ) {
    fAbsHCID = G4SDManager::GetSDMpointer()->GetCollectionID("CrystalHitsCollection");
  }

  // Get hits collections
  auto absoHC = GetHitsCollection(fAbsHCID, event); //this is B4cCalorHit :: G4VHitsCollection

  G4cout
    << "Enegry : " <<  eventEnergy << G4endl
    << "Theta  : " <<  eventTheta << G4endl
    << "Phi    : " <<  eventPhi << G4endl;
    


  // Get hit with total values
  auto absoHit = (*absoHC)[absoHC->entries()-1];

  int nHit = absoHit->GetNumHit();

  fDetList = absoHit->GetDetID();

  // Print per event (modulo n)
  auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "================> End of event Summary: " << eventID << G4endl;

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
  analysisManager->FillNtupleDColumn(0, absoHit->GetEdep());
  analysisManager->FillNtupleDColumn(1, absoHit->GetTrackLength());
  analysisManager->FillNtupleDColumn(2, absoHit->GetNumHit()+1);

  analysisManager->FillNtupleDColumn(4, eventTheta);
  analysisManager->FillNtupleDColumn(5, eventPhi);
  analysisManager->FillNtupleDColumn(6, eventEnergy);
  
  analysisManager->AddNtupleRow();  
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
