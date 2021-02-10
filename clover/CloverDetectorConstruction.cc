
#include "CloverDetectorConstruction.hh"
#include "CloverCrystalSD.hh"


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
G4GlobalMagFieldMessenger* CloverDetectorConstruction::fMagFieldMessenger = 0; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverDetectorConstruction::CloverDetectorConstruction()
 : G4VUserDetectorConstruction(),
   fNumOfCrystal(0),
   fLogicCrystal(NULL), fCrystalMaterial(NULL),
   fCheckOverlaps(true)
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

  // Iron
  nistManager->FindOrBuildMaterial("G4_Fe");

  // Aluminium
  nistManager->FindOrBuildMaterial("G4_Al");

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
  G4double crystalRadius = 25.*mm;
  G4double crystalZPos = 11.5*cm + crystalLength/2.;
  G4double cutXY = 46.0 * mm;

  G4double worldSizeZ  = 2*(crystalLength + crystalZPos);
  G4double worldSizeXY = worldSizeZ;

  //pipe
  G4double pipeOuterRadius     = 100* mm;
  G4double pipeWallThickness   =  2 * mm;
  G4double pipeLength          = 120* mm;
  G4double pipeZPos            = 0.*cm;

  //Clover casing
  G4double caseXYInner = 2*(cutXY + 1.0) * mm;
  G4double caseZInner  = crystalLength/2. + 4 * mm;
  G4double caseXYWallThickness = 3.0 * mm;
  G4double caseZWallThickness = 3.0 * mm;
  
  // Get materials
  auto backgroundMaterial = G4Material::GetMaterial("G4_AIR");
  //auto backgroundMaterial = G4Material::GetMaterial("Galactic");
   
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

  // vaccum Pipe
  
  G4Tubs * pipe = new G4Tubs("pipe", pipeOuterRadius - pipeWallThickness , pipeOuterRadius, pipeLength, 0, 360*degree);
  G4Tubs * pipe0 = new G4Tubs("pipe0", 0 , pipeOuterRadius, pipeLength, 0, 360*degree);
  
  G4RotationMatrix * rotX = new G4RotationMatrix(); rotX->rotateX(90*degree);
  G4RotationMatrix * rotY = new G4RotationMatrix(); rotY->rotateY(90*degree);
  
  G4SubtractionSolid * pipe1 = new G4SubtractionSolid("Pipe1", pipe, pipe0, rotY, G4ThreeVector());

  G4LogicalVolume * pipe1LV = new G4LogicalVolume( pipe1, G4Material::GetMaterial("G4_Fe"), "Pipe");
  G4LogicalVolume * pipe2LV = new G4LogicalVolume( pipe, G4Material::GetMaterial("G4_Fe"), "Pipe2");
  
  new G4PVPlacement( rotX,                // no rotation
                     G4ThreeVector(0, 0, pipeZPos),  // at (0,0,0)
                     pipe1LV,          // its logical volume                         
                     "Pipe1",          // its name
                     worldLV,          // its mother  volume
                     false,            // no boolean operation
                     0,                // copy number
                     fCheckOverlaps);  // checking overlaps
                     
  new G4PVPlacement( rotY,                // no rotation
                     G4ThreeVector(0, 0, pipeZPos),  // at (0,0,0)
                     pipe2LV,          // its logical volume                         
                     "Pipe2",          // its name
                     worldLV,          // its mother  volume
                     false,            // no boolean operation
                     0,                // copy number
                     fCheckOverlaps);  // checking overlaps
  
  pipe1LV->SetVisAttributes(new G4VisAttributes(G4Colour(1.0,1.0,1.0)));
  pipe2LV->SetVisAttributes(new G4VisAttributes(G4Colour(1.0,1.0,1.0)));
                    
  // Crystals
  G4VisAttributes * crystalVisAtt= new G4VisAttributes(G4Colour(0.5,0.5,1.0));
  crystalVisAtt->SetVisibility(true);

  for( G4int i = 0 ; i < fNumOfCrystal ; i++){ 
    G4String name = "HPGe"+ std::to_string(i);
    G4cout << " crystal name : " << name << G4endl;
    G4double phi = 360/fNumOfCrystal * degree;

    G4double rho = (cutXY/2  + 0.3* mm) / sin(phi/2.);

    G4Tubs * base = new G4Tubs("base",  0, crystalRadius, crystalLength/2., 0, 360*degree);
    G4Box * cut = new G4Box("cut", cutXY/2. , cutXY/2. , crystalLength/2. * 1.2);

    G4IntersectionSolid * crystalS = new G4IntersectionSolid("HPGe", base, cut);

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

  // Al casing
  
  G4Box* case1 = new G4Box("case1", caseXYInner/2., caseXYInner/2., caseZInner  );
  G4Box* case2 = new G4Box("case2", (caseXYInner + caseXYWallThickness)/2. , (caseXYInner + caseXYWallThickness)/2., caseZInner + caseZWallThickness*2.);
  G4SubtractionSolid * casing = new G4SubtractionSolid("casing", case2, case1);
  
  auto caseLV = new G4LogicalVolume(casing, G4Material::GetMaterial("G4_Al"), "Case");
  
  new G4PVPlacement( 0,                // no rotation
                     G4ThreeVector(0, 0, crystalZPos),  // at (0,0,0)
                     caseLV,           // its logical volume                         
                     "Casing",         // its name
                     worldLV,          // its mother  volume
                     false,            // no boolean operation
                     0,                // copy number
                     fCheckOverlaps);  // checking overlaps
  
  caseLV->SetVisAttributes(new G4VisAttributes(G4Colour(1.0,1.0,0.0)));

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
