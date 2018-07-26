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
* @file G4PhysicsList.h
* @class G4PhysicsList
* @brief GEANT4 physics process definition
*
* @author S. Riggi
* @date 21/02/2012
*/
#ifndef G4PhysicsList_h
#define G4PhysicsList_h 1

#include <globals.hh>
#include <G4VUserPhysicsList.hh>

namespace MuonPortalNS {


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
    
};//close class

}//close namespace

#endif 
