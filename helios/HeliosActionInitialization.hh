
#ifndef HeliosActionInitialization_h
#define HeliosActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.
///

class HeliosActionInitialization : public G4VUserActionInitialization
{
  public:
    HeliosActionInitialization();
    virtual ~HeliosActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif

    
