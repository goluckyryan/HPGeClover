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
/// \file electromagnetic/TestEm4/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
//
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  //
  // define a material from its elements.   case 1: chemical molecule
  // 
  G4double a, z;
  G4double density;  
  G4int ncomponents, natoms;
 
  G4Element* C = new G4Element("Carbon"  ,"C" , z= 6., a= 12.01*g/mole);
  G4Element* F = new G4Element("Fluorine","N" , z= 9., a= 18.99*g/mole);
 
  G4Material* C6F6 = 
  new G4Material("FluorCarbonate", density= 1.61*g/cm3, ncomponents=2);
  C6F6->AddElement(C, natoms=6);
  C6F6->AddElement(F, natoms=6);
  
  G4cout << C6F6 << G4endl;

  G4Material* Ge = new G4Material("Germaniam", z= 32., a = 72.64*g/mole, density = 5.32*g/cm3); 
  G4cout << Ge << G4endl;

  G4NistManager* nist = G4NistManager::Instance();
  G4Material* air = nist->FindOrBuildMaterial("G4_AIR");
  G4cout<< air << G4endl;

  // world Box
  G4Box * worldBox = new G4Box("World", 0.5*m, 0.5*m, 0.5*m);
  G4LogicalVolume* worldLog = new G4LogicalVolume(worldBox, air, "World");
  
  // Container
  G4double Rmin=0., Rmax=2.5*cm, deltaZ= 8*cm, Phimin=0., deltaPhi=360*degree;
  G4Tubs*  HPGe = new G4Tubs("HPGe", Rmin,Rmax,deltaZ,Phimin,deltaPhi);   
  G4LogicalVolume*  HPGeLog = new G4LogicalVolume(HPGe,  Ge,  "HPGe"); 


  //====== placement
  G4VPhysicalVolume* physiWorld = new G4PVPlacement(0,                        //no rotation
                                                    G4ThreeVector(),          //at (0,0,0)
                                                    worldLog,               //its logical volume
                                                    "World",                   //its name
                                                    0,                        //its mother  volume
                                                    false,                    //no boolean operation
                                                    0);                       //copy number

  G4VPhysicalVolume* detector = new G4PVPlacement(0,                        //no rotation
                                                    G4ThreeVector(0.0*cm, 0.0*cm, 20*cm),          //at (0,0,0)
                                                    HPGeLog,               //its logical volume
                                                    "HPGe",                   //its name
                                                    worldLog,                        //its mother  volume
                                                    false,                    //no boolean operation
                                                    0);                       //copy number

  //
  //always return the physical World
  //  
  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
