

#include <G4PrimaryGenerator.h>

#include <G4Event.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>
#include <globals.hh>

G4PrimaryGenerator::G4PrimaryGenerator(){

 	int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  particleGun->SetParticleDefinition(particleTable->FindParticle(particleName="geantino"));
  particleGun->SetParticleEnergy(1.0*CLHEP::GeV);
  particleGun->SetParticlePosition(G4ThreeVector(0.0*CLHEP::m, 0.0*CLHEP::m, -4.0*CLHEP::m));
	particleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
}

G4PrimaryGenerator::~G4PrimaryGenerator()
{
  delete particleGun;
}

void G4PrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
 
  particleGun->GeneratePrimaryVertex(anEvent);
}


