// ******************************************************************************
// * License and Disclaimer                                                     *
// *                                                                            *
// * Copyright 2018 Simone Riggi																			          *
// *																																	          *
// * This file is part of MuonPortalGUI																          *
// * MuonPortalGUI is free software: you can redistribute it and/or modify it   *
// * under the terms of the GNU General Public License as published by          *
// * the Free Software Foundation, either * version 3 of the License,           *
// * or (at your option) any later version.                                     *
// * MuonPortalGUI is distributed in the hope that it will be useful, but 			*
// * WITHOUT ANY WARRANTY; without even the implied warranty of                 * 
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                       *
// * See the GNU General Public License for more details. You should            * 
// * have received a copy of the GNU General Public License along with          * 
// * MuonPortalGUI. If not, see http://www.gnu.org/licenses/.                   *
// ******************************************************************************

/**
* @file G4PrimaryGenerator.cc
* @class G4PrimaryGenerator
* @brief GEANT4 primary generator definition
*
* @author S. Riggi
* @date 21/02/2012
*/

#include <G4PrimaryGenerator.h>

#include <G4Event.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>
#include <globals.hh>

namespace MuonPortalNS {

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

}//close namespace
