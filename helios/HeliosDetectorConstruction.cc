
#include "HeliosDetectorConstruction.hh"
#include "HeliosPSDSD.hh"


#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4RotationMatrix.hh"

#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal 
G4GlobalMagFieldMessenger* HeliosDetectorConstruction::fMagFieldMessenger = 0; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HeliosDetectorConstruction::HeliosDetectorConstruction()
 : G4VUserDetectorConstruction(),
   fNumOfPSD(0),
   fLogicPSD(NULL), fPSDMaterial(NULL),
   fCheckOverlaps(true)
{
  fNumOfPSD = 24;  //also need to change the fNDet in HeliosEventAction.cc
  fNumOfSide = 4;
  fLogicPSD = new G4LogicalVolume* [fNumOfPSD];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HeliosDetectorConstruction::~HeliosDetectorConstruction()
{
  delete [] fLogicPSD;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* HeliosDetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HeliosDetectorConstruction::DefineMaterials()
{ 
  // Lead material defined using NIST Manager
  auto nistManager = G4NistManager::Instance();
  
  // Vacuum
  new G4Material("Galactic", 1., 1.01*g/mole, universe_mean_density, kStateGas, 2.73*kelvin, 3.e-18*pascal);

  // Air
  nistManager->FindOrBuildMaterial("G4_AIR");

  // Si
  nistManager->FindOrBuildMaterial("G4_Si");

  // Aluminium
  nistManager->FindOrBuildMaterial("G4_Al");

  // Si
  fPSDMaterial = nistManager->FindOrBuildMaterial("G4_Si");

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* HeliosDetectorConstruction::DefineVolumes()
{
  // Geometry parameters
  G4int    nDet = fNumOfPSD/fNumOfSide; // number det in each side
  G4double PSDLength = 50.*mm;
  G4double PSDWidth = 10.*mm;
  G4double PSDThick = 1.0*mm;
  G4double PSDgap = 5*mm;

  //Array Frame
  G4double ArrayInnerRadius   = 7.0* mm;
  G4double ArrayLength        = 400* mm;  
  G4double ArrayRadius        = 11.*mm;
  G4double ArrayZPos          = 100.*mm;
  
  
  G4double worldSizeZ  = 2* nDet * (PSDLength + PSDgap);
  G4double worldSizeXY = worldSizeZ;

  // Get materials
  //auto backgroundMaterial = G4Material::GetMaterial("G4_AIR");
  auto backgroundMaterial = G4Material::GetMaterial("Galactic");
   
  // World
  auto worldS = new G4Box("World",  worldSizeXY/2, worldSizeXY/2, worldSizeZ); 
                         
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

  // Array Frame 
  G4Box * tube = new G4Box("tube", ArrayRadius - PSDThick, ArrayRadius - PSDThick, ArrayLength/2.);
  G4Tubs * hole = new G4Tubs("hole", 0 , ArrayInnerRadius, ArrayLength * 2, 0, 360*degree);
  
  G4SubtractionSolid * frame = new G4SubtractionSolid("frame", tube, hole, 0, G4ThreeVector());

  G4LogicalVolume * frameLV = new G4LogicalVolume( frame, G4Material::GetMaterial("G4_Al"), "Array");
  
  frameLV->SetVisAttributes(new G4VisAttributes(G4Colour(0.5,0.5,0.5)));
  
  new G4PVPlacement( 0,                // no rotation
                     G4ThreeVector(0, 0, ArrayLength/2. + ArrayZPos - 40 * mm),  // at (0,0,0)
                     frameLV,          // its logical volume                         
                     "Array",          // its name
                     worldLV,          // its mother  volume
                     false,            // no boolean operation
                     0,                // copy number
                     fCheckOverlaps);  // checking overlaps
                     
  
  
  // PSDs
  G4VisAttributes * PSDVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  PSDVisAtt->SetVisibility(true);

  for( G4int i = 0 ; i < fNumOfSide ; i++){

    G4double phi = 360*degree/fNumOfSide;

    G4RotationMatrix * rotZ = new G4RotationMatrix(); rotZ->rotateZ(-i*(phi));
    
    G4ThreeVector posR = G4ThreeVector( ArrayRadius * cos(i* phi ), ArrayRadius * sin(i * phi), 0);
    
    for ( G4int j = 0; j < nDet ; j++){
     
      G4String name = "PSD"+ std::to_string(i);
      G4cout << " PSD name : " << name << G4endl;

      G4Box * psd = new G4Box("PSD",   PSDThick/2.,PSDWidth/2., PSDLength/2.);

      fLogicPSD[i] = new G4LogicalVolume(psd, fPSDMaterial, "PSDLV");
      fLogicPSD[i]->SetVisAttributes(PSDVisAtt);
      
      G4ThreeVector pos = posR + G4ThreeVector(0, 0, j* (PSDLength + PSDgap) + ArrayZPos + PSDLength/2.);
      
      new G4PVPlacement( rotZ,                // no rotation
                         pos,              // its position
                         fLogicPSD[i],        // its logical volume                         
                         name,             // its name
                         worldLV,          // its mother  volume
                         false,            // no boolean operation
                         i,                // copy number
                         fCheckOverlaps);               // checking overlaps

    }
  }

  // Always return the physical World
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HeliosDetectorConstruction::ConstructSDandField()
{

  G4cout << "********************* HeliosDetectorConstruction::ConstructSDandField() " << G4endl;
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  // Sensitive detectors
  HeliosPSDSD * PSDSD = new HeliosPSDSD("PSD", "PSDHitsCollection", fNumOfPSD);
  G4SDManager::GetSDMpointer()->AddNewDetector(PSDSD);

  //Set PSDSD to all logical volumn under the same name of "PSDLV"
  SetSensitiveDetector("PSDLV",PSDSD, true);

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
