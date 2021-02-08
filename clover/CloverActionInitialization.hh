
#ifndef CloverActionInitialization_h
#define CloverActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.
///

class CloverActionInitialization : public G4VUserActionInitialization
{
  public:
    CloverActionInitialization();
    virtual ~CloverActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif

    
