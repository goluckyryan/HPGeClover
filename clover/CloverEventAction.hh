
#ifndef CloverEventAction_h
#define CloverEventAction_h 1

#include "G4UserEventAction.hh"

#include "CloverCrystalHit.hh"

#include "globals.hh"

#include <vector>

/// Event action class
///
/// In EndOfEventAction(), it prints the accumulated quantities of the energy 
/// deposit and track lengths of charged particles in Crystal 
/// stored in the hits collections.

class CloverEventAction : public G4UserEventAction
{
public:
  CloverEventAction();
  virtual ~CloverEventAction();

  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void    EndOfEventAction(const G4Event* event);

  void SaveEventTheta(G4double theta) {eventTheta = theta;}
  void SaveEventPhi(G4double Phi) {eventPhi = Phi;}
  void SaveEventEnergy(G4double energy) {eventEnergy = energy;}

  std::vector<G4int>& GetDetList() {return fDetList;}
    
private:
  // methods
  CloverCrystalHitsCollection* GetHitsCollection(G4int hcID, const G4Event* event) const;
  void PrintEventStatistics(G4double absoEdep, G4double absoTrackLength) const;
  
  // data members                   
  G4int  fAbsHCID;

  G4double eventTheta;
  G4double eventPhi;
  G4double eventEnergy;

  std::vector<G4int>    fDetList;  // hit det list
  std::vector<G4double> fdEList;   // dE list
  std::vector<G4double> fdLList;   // length list
  
  
};
                     
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
