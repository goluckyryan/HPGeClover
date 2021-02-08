
#include "CloverDetectorConstruction.hh"
#include "CloverCrystalSD.hh"


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
G4GlobalMagFieldMessenger* CloverDetectorConstruction::fMagFieldMessenger = 0; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverDetectorConstruction::CloverDetectorConstruction()
 : G4VUserDetectorConstruction(),
   fCheckOverlaps(true),
   fNumOfCrystal(0),
   fLogicCrystal(NULL), fCrystalMaterial(NULL)
{
  fNumOfCrystal = 4;  //also need to change the fNDet in CloverEventAction.cc
  fLogicCrystal = new G4LogicalVolume* [fNumOfCrystal];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverDetectorConstruction::~CloverDetectorConstruction()
{
  delete [] fLogicCrystal;
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* CloverDetectorConstruction::Construct()
{
  // Define materials 
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CloverDetectorConstruction::DefineMaterials()
{ 
  // Lead material defined using NIST Manager
  auto nistManager = G4NistManager::Instance();
  
  // Vacuum
  new G4Material("Galactic", 1., 1.01*g/mole, universe_mean_density, kStateGas, 2.73*kelvin, 3.e-18*pascal);

  // Air
  nistManager->FindOrBuildMaterial("G4_AIR");

  // Ge
  fCrystalMaterial = nistManager->FindOrBuildMaterial("G4_Ge");

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* CloverDetectorConstruction::DefineVolumes()
{
  // Geometry parameters
  fNumOfCrystal = 4;
  G4double crystalLength = 8.*cm;
  G4double crystalRadius = 2.5*cm;

  G4double crystalZPos = 20.*cm + crystalLength/2.;

  auto layerThickness  = crystalLength;
  auto cloverThickness = layerThickness;
  auto cloverSize      = fNumOfCrystal * 2* crystalRadius;
  
  auto worldSizeXY = 4 * cloverSize;
  auto worldSizeZ  = 4 * (cloverThickness + crystalZPos); 
  
  // Get materials
  auto backgroundMaterial = G4Material::GetMaterial("G4_AIR");
   
  // World

  auto worldS = new G4Box("World",  worldSizeXY/2, worldSizeXY/2, worldSizeZ/2); 
                         
  auto worldLV = new G4LogicalVolume( worldS,  backgroundMaterial, "World"); 
                                   
  auto worldPV = new G4PVPlacement( 0,                // no rotation
                                    G4ThreeVector(),  // at (0,0,0)
                                    worldLV,          // its logical volume                         
                                    "World",          // its name
                                    0,                // its mother  volume
                                    false,            // no boolean operation
                                    0,                // copy number
                                    fCheckOverlaps);  // checking overlaps

  worldLV->SetVisAttributes (G4VisAttributes::GetInvisible());

                    
  // Crystals
  G4VisAttributes * crystalVisAtt= new G4VisAttributes(G4Colour(0.5,0.5,1.0));
  crystalVisAtt->SetVisibility(true);

  for( G4int i = 0 ; i < fNumOfCrystal ; i++){ 
    G4String name = "HPGe"+ std::to_string(i);
    G4cout << " crystal name : " << name << G4endl;
    G4double phi = 360/fNumOfCrystal * degree;
    G4double rho = crystalRadius / sin(phi/2.);

    G4Tubs * crystalS = new G4Tubs("HPGeS",  0, crystalRadius, crystalLength, 0, 360*degree);
            
    fLogicCrystal[i] = new G4LogicalVolume(crystalS, fCrystalMaterial, "CrystalLV");

    fLogicCrystal[i]->SetVisAttributes(crystalVisAtt);

    G4ThreeVector pos = G4ThreeVector(rho * cos(i* phi + phi/2.), rho * sin(i * phi + phi/2.), crystalZPos);
    
    new G4PVPlacement( 0,                // no rotation
                       pos,              // its position
                       fLogicCrystal[i],        // its logical volume                         
                       name,             // its name
                       worldLV,          // its mother  volume
                       false,            // no boolean operation
                       i,                // copy number
                       fCheckOverlaps);               // checking overlaps


  }

  // Always return the physical World
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CloverDetectorConstruction::ConstructSDandField()
{

  G4cout << "********************* CloverDetectorConstruction::ConstructSDandField() " << G4endl;
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  // Sensitive detectors
  CloverCrystalSD * crystalSD = new CloverCrystalSD("Crystal", "CrystalHitsCollection", fNumOfCrystal);
  G4SDManager::GetSDMpointer()->AddNewDetector(crystalSD);

  //Set crystalSD to all logical volumn under the same name of "CrystalLV"
  SetSensitiveDetector("CrystalLV",crystalSD, true);

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
