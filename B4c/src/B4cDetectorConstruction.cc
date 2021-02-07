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
/// \file B4cDetectorConstruction.cc
/// \brief Implementation of the B4cDetectorConstruction class

#include "B4cDetectorConstruction.hh"
#include "B4cCalorimeterSD.hh"


#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal 
G4GlobalMagFieldMessenger* B4cDetectorConstruction::fMagFieldMessenger = 0; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4cDetectorConstruction::B4cDetectorConstruction()
 : G4VUserDetectorConstruction(),
   fCheckOverlaps(true),
   fNofClover(-1)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4cDetectorConstruction::~B4cDetectorConstruction()
{ 
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B4cDetectorConstruction::Construct()
{
  // Define materials 
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4cDetectorConstruction::DefineMaterials()
{ 
  // Lead material defined using NIST Manager
  auto nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_Pb");
  
  // Liquid argon material
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;  
  G4double density; 
  new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);
         // The argon by NIST Manager is a gas with a different density

  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);

  // Air
  nistManager->FindOrBuildMaterial("G4_AIR");

  // Ge
  nistManager->FindOrBuildMaterial("G4_Ge");

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B4cDetectorConstruction::DefineVolumes()
{
  // Geometry parameters
  fNofClover = 4;
  G4double crystalLength = 8.*cm;
  G4double crystalRadius = 2.5*cm;

  G4double crystalZPos = 20.*cm + crystalLength/2.;

  auto layerThickness  = crystalLength;
  auto cloverThickness = layerThickness;
  auto cloverSize      = fNofClover * 2* crystalRadius;
  
  auto worldSizeXY = 4 * cloverSize;
  auto worldSizeZ  = 4 * (cloverThickness + crystalZPos); 
  
  // Get materials
  auto backgroundMaterial = G4Material::GetMaterial("G4_AIR");
  auto absorberMaterial = G4Material::GetMaterial("G4_Ge");
  //auto gapMaterial = G4Material::GetMaterial("liquidArgon");
  
  //if ( ! backgroundMaterial || ! absorberMaterial || ! gapMaterial ) {
  //  G4ExceptionDescription msg;
  //  msg << "Cannot retrieve materials already defined."; 
  //  G4Exception("B4DetectorConstruction::DefineVolumes()",
  //    "MyCode0001", FatalException, msg);
  //}  
   
  //     
  // World
  //
  auto worldS 
    = new G4Box("World",           // its name
                 worldSizeXY/2, worldSizeXY/2, worldSizeZ/2); // its size
                         
  auto worldLV
    = new G4LogicalVolume(
                 worldS,           // its solid
                 backgroundMaterial,  // its material
                 "World");         // its name
                                   
  auto worldPV
    = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),  // at (0,0,0)
                 worldLV,          // its logical volume                         
                 "World",          // its name
                 0,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 
                    
  // Crystals
  //
  auto crystalS 
    = new G4Tubs("HPGeS",            // its name
                 0, crystalRadius, crystalLength, 0, 360*degree); // its size
                         
  auto crystalLV
    = new G4LogicalVolume(
                 crystalS,        // its solid
                 absorberMaterial, // its material
                 "CrystalLV");        // its name
                                   

  for( G4int i = 0 ; i < fNofClover ; i++){ 
    G4String name = "HPGe"+ std::to_string(i);
    G4cout << " crystal name : " << name << G4endl;
    G4double phi = 360/fNofClover * degree;
    G4double rho = crystalRadius / sin(phi/2.);
    new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(rho * cos(i* phi + phi/2.), rho * sin(i * phi + phi/2.), crystalZPos), // its position
                 crystalLV,       // its logical volume                         
                 name,           // its name
                 worldLV,          // its mother  volume
                 false,            // no boolean operation
                 i,                // copy number
                 0);  // checking overlaps

  }
  
  //                                        
  // Visualization attributes
  //
  worldLV->SetVisAttributes (G4VisAttributes::GetInvisible());


  auto crystalVisAtt= new G4VisAttributes(G4Colour(0.5,0.5,1.0));
  crystalVisAtt->SetVisibility(true);
  crystalLV->SetVisAttributes(crystalVisAtt);

  //
  // Always return the physical World
  //
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4cDetectorConstruction::ConstructSDandField()
{
  // G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  // 
  // Sensitive detectors
  //
  auto crystalSD = new B4cCalorimeterSD("Crystal", "CrystalHitsCollection", fNofClover);
  G4SDManager::GetSDMpointer()->AddNewDetector(crystalSD);
  SetSensitiveDetector("CrystalLV",crystalSD);

  // 
  // Magnetic field
  //
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  
  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
