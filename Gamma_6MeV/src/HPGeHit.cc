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
/// \file HPGeHit.cc
/// \brief Implementation of the HPGeHit class

#include "HPGeHit.hh"

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal G4Allocator<HPGeHit>* HPGeHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGeHit::HPGeHit(G4int id,G4double time)
: G4VHit(), 
  fId(id), fTime(time), fPos(0.), fPLogV(nullptr)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGeHit::~HPGeHit()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGeHit::HPGeHit(const HPGeHit &right)
: G4VHit(),
  fId(right.fId),
  fTime(right.fTime),
  fPos(right.fPos),
  fRot(right.fRot),
  fPLogV(right.fPLogV)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const HPGeHit& HPGeHit::operator=(const HPGeHit &right)
{
  fId = right.fId;
  fTime = right.fTime;
  fPos = right.fPos;
  fRot = right.fRot;
  fPLogV = right.fPLogV;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool HPGeHit::operator==(const HPGeHit &/*right*/) const
{
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HPGeHit::Draw()
{
  auto visManager = G4VVisManager::GetConcreteInstance();
  if (! visManager) return;

  G4Transform3D trans(fRot.inverse(),fPos);
  G4VisAttributes attribs;
  auto pVA = fPLogV->GetVisAttributes();
  if (pVA) attribs = *pVA;
  G4Colour colour(0.,1.,1.);
  attribs.SetColour(colour);
  attribs.SetForceSolid(true);
  visManager->Draw(*fPLogV,attribs,trans);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const std::map<G4String,G4AttDef>* HPGeHit::GetAttDefs() const
{
  G4bool isNew;
  auto store = G4AttDefStore::GetInstance("HPGeHit",isNew);

  if (isNew) {
    (*store)["HitType"] 
      = G4AttDef("HitType","Hit Type","Physics","","G4String");
    
    (*store)["ID"] 
      = G4AttDef("ID","ID","Physics","","G4int");
    
    (*store)["Time"] 
      = G4AttDef("Time","Time","Physics","G4BestUnit","G4double");
    
    (*store)["Pos"] 
      = G4AttDef("Pos","Position","Physics","G4BestUnit","G4ThreeVector");
    
    (*store)["LVol"] 
      = G4AttDef("LVol","Logical Volume","Physics","","G4String");
  }
  return store;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue>* HPGeHit::CreateAttValues() const
{
  auto values = new std::vector<G4AttValue>;
  
  values
    ->push_back(G4AttValue("HitType","HodoscopeHit",""));
  values
    ->push_back(G4AttValue("ID",G4UIcommand::ConvertToString(fId),""));
  values
    ->push_back(G4AttValue("Time",G4BestUnit(fTime,"Time"),""));
  values
    ->push_back(G4AttValue("Pos",G4BestUnit(fPos,"Length"),""));
  
  if (fPLogV)
    values->push_back(G4AttValue("LVol",fPLogV->GetName(),""));
  else
    values->push_back(G4AttValue("LVol"," ",""));
  
  return values;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HPGeHit::Print()
{
  G4cout << "  Hodoscope[" << fId << "] " << fTime/ns << " (nsec)" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
