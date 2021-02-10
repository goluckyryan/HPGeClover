
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

  std::vector<G4double>& GetdEList()  {return fdEList;};
  std::vector<G4double>& GetStepLengthList()  {return fdLList;};


private:
  // methods
  CloverCrystalHitsCollection* GetHitsCollection(G4int hcID, const G4Event* event) const;
  void PrintEventStatistics(G4double absoEdep, G4double absoTrackLength) const;
  
  // data members
  G4int  fCrystalHCID; // Hit collection ID

  G4double eventTheta;
  G4double eventPhi;
  G4double eventEnergy;

  G4int fNDet;

  std::vector<G4double> fdEList;   // dE of each crystal
  std::vector<G4double> fdLList;   // step length of each crystal

  //std::vector<G4double> fLastPost; //last tracking position
  
  
};
                     
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
