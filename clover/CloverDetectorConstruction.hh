#ifndef CloverDetectorConstruction_h
#define CloverDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4GlobalMagFieldMessenger;


class CloverDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    CloverDetectorConstruction();
    virtual ~CloverDetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
     
  private:
    // methods
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();

    // data members
    G4int   fNumOfCrystal;     // number of Crystal

    G4LogicalVolume**  fLogicCrystal;    // pointer to the logical Crystal

    G4Material*        fCrystalMaterial; // pointer to the crystal material
    
    // other things
    static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger;  // magnetic field messenger

    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

