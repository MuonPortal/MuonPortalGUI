#ifndef G4PhysicsList_h
#define G4PhysicsList_h 1

#include <globals.hh>
#include <G4VUserPhysicsList.hh>


class G4PhysicsList : public G4VUserPhysicsList
{
  public:
    G4PhysicsList();
   ~G4PhysicsList();

  public:
    void ConstructParticle();
    void ConstructProcess();

    void SetCuts();

    //these methods Construct particles
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();

    //these methods Construct physics processes and register them
    void ConstructGeneral();
    void ConstructEM();
    
    //for the Messenger 
    void SetVerbose(int);
    
    
  private:
    
};


#endif 
