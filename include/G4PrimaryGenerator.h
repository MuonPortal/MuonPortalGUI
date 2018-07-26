
#ifndef G4PrimaryGeneratorAction_h
#define G4PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;

class G4PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
  public:
    G4PrimaryGenerator();
    ~G4PrimaryGenerator();

  public:
    void GeneratePrimaries(G4Event* anEvent);

  private:
    G4ParticleGun* particleGun;
};

#endif
