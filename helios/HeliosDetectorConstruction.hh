#ifndef HeliosDetectorConstruction_h
#define HeliosDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4GlobalMagFieldMessenger;


class HeliosDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    HeliosDetectorConstruction();
    virtual ~HeliosDetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
     
  private:
    // methods
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();

    // data members
    G4int   fNumOfPSD;     // number of PSD
    G4int   fNumOfSide;    // number of side

    G4LogicalVolume**  fLogicPSD;    // pointer to the logical PSD

    G4Material*        fPSDMaterial; // pointer to the PSD material
    
    // other things
    static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger;  // magnetic field messenger

    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

