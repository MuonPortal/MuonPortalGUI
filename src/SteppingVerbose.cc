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
* @file SteppingVerbose.cc
* @class SteppingVerbose
* @brief Handle the verbosity of the stepping action
* @author Dr. Simone Riggi
* @date 05/04/2010
*/


#include <SteppingVerbose.h>

#include <G4SteppingManager.hh>
#include <G4UnitsTable.hh>

#include <iostream>

using namespace std;

namespace MuonPortalNS {

SteppingVerbose::SteppingVerbose()
{}


SteppingVerbose::~SteppingVerbose()
{}


void SteppingVerbose::StepInfo()
{
CopyState();
  
  int prec = cout.precision(3);

  if( verboseLevel >= 1 ){
    if( verboseLevel >= 4 ) VerboseTrack();
    if( verboseLevel >= 3 ){
      cout << endl;    
      cout << std::setw( 5) << "#Step#"     << " "
	     << std::setw( 6) << "X"          << "    "
	     << std::setw( 6) << "Y"          << "    "  
	     << std::setw( 6) << "Z"          << "    "
	     << std::setw( 9) << "KineE"      << " "
	     << std::setw( 9) << "dEStep"     << " "  
	     << std::setw(10) << "StepLeng"     
	     << std::setw(10) << "TrakLeng" 
	     << std::setw(10) << "Volume"    << "  "
	     << std::setw(10) << "Process"   << endl;	          
    }

    cout << std::setw(5) << fTrack->GetCurrentStepNumber() << " "
	<< std::setw(6) << G4BestUnit(fTrack->GetPosition().x(),"Length")
	<< std::setw(6) << G4BestUnit(fTrack->GetPosition().y(),"Length")
	<< std::setw(6) << G4BestUnit(fTrack->GetPosition().z(),"Length")
	<< std::setw(6) << G4BestUnit(fTrack->GetKineticEnergy(),"Energy")
	<< std::setw(6) << G4BestUnit(fStep->GetTotalEnergyDeposit(),"Energy")
	<< std::setw(6) << G4BestUnit(fStep->GetStepLength(),"Length")
	<< std::setw(6) << G4BestUnit(fTrack->GetTrackLength(),"Length")
	<< "  ";

    // if( fStepStatus != fWorldBoundary){ 
    if( fTrack->GetNextVolume() != 0 ) { 	
			int copyNo= fTrack->GetVolume()->GetCopyNo();
			G4String volumeName= fTrack->GetVolume()->GetName();
      cout << std::setw(10) << fTrack->GetVolume()->GetName()<< " "<< copyNo;
    } 
    else {
      cout << std::setw(10) << "OutOfWorld";
    }

    if(fStep->GetPostStepPoint()->GetProcessDefinedStep() != NULL){
      cout << "  " 
        << std::setw(10) << fStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    } 
    else {
      cout << "   UserLimit";
    }

    cout << endl;

    if( verboseLevel == 2 ){
      int tN2ndariesTot = fN2ndariesAtRestDoIt +
	                          fN2ndariesAlongStepDoIt +
	                          fN2ndariesPostStepDoIt;
      if(tN2ndariesTot>0){
	cout << "    :----- List of 2ndaries - "
	       << "#SpawnInStep=" << std::setw(3) << tN2ndariesTot 
	       << "(Rest="  << std::setw(2) << fN2ndariesAtRestDoIt
	       << ",Along=" << std::setw(2) << fN2ndariesAlongStepDoIt
	       << ",Post="  << std::setw(2) << fN2ndariesPostStepDoIt
	       << "), "
	       << "#SpawnTotal=" << std::setw(3) << (*fSecondary).size()
	       << " ---------------"
	       << endl;

	for(size_t lp1=(*fSecondary).size()-tN2ndariesTot; 
                        lp1<(*fSecondary).size(); lp1++){
	  cout << "    : "
		 << std::setw(6)
		 << G4BestUnit((*fSecondary)[lp1]->GetPosition().x(),"Length")
		 << std::setw(6)
		 << G4BestUnit((*fSecondary)[lp1]->GetPosition().y(),"Length")
		 << std::setw(6)
		 << G4BestUnit((*fSecondary)[lp1]->GetPosition().z(),"Length")
		 << std::setw(6)
		 << G4BestUnit((*fSecondary)[lp1]->GetKineticEnergy(),"Energy")
		 << std::setw(10)
		 << (*fSecondary)[lp1]->GetDefinition()->GetParticleName();
	  cout << endl;
	}
              
	cout << "    :-----------------------------"
	       << "----------------------------------"
	       << "-- EndOf2ndaries Info ---------------"
	       << endl;
      }
    }
    
  }
  cout.precision(prec);
}


void SteppingVerbose::TrackingStarted(){

  CopyState();
	int prec = cout.precision(3);
  if( verboseLevel > 0 ){

    cout << std::setw( 5) << "Step#"      << " "
         << std::setw( 6) << "X"          << "    "
	   << std::setw( 6) << "Y"          << "    "  
	   << std::setw( 6) << "Z"          << "    "
	   << std::setw( 9) << "KineE"      << " "
	   << std::setw( 9) << "dEStep"     << " "  
	   << std::setw(10) << "StepLeng"  
	   << std::setw(10) << "TrakLeng"
	   << std::setw(10) << "Volume"     << "  "
	   << std::setw(10) << "Process"    << endl;	     

    cout << std::setw(5) << fTrack->GetCurrentStepNumber() << " "
	<< std::setw(6) << G4BestUnit(fTrack->GetPosition().x(),"Length")
	<< std::setw(6) << G4BestUnit(fTrack->GetPosition().y(),"Length")
	<< std::setw(6) << G4BestUnit(fTrack->GetPosition().z(),"Length")
	<< std::setw(6) << G4BestUnit(fTrack->GetKineticEnergy(),"Energy")
	<< std::setw(6) << G4BestUnit(fStep->GetTotalEnergyDeposit(),"Energy")
	<< std::setw(6) << G4BestUnit(fStep->GetStepLength(),"Length")
	<< std::setw(6) << G4BestUnit(fTrack->GetTrackLength(),"Length")
	<< "  ";

    if(fTrack->GetNextVolume()){
			int copyNo= fTrack->GetVolume()->GetCopyNo();
			G4String volumeName= fTrack->GetVolume()->GetName();
      cout << std::setw(10) << fTrack->GetVolume()->GetName()<< " "<< copyNo;
    } 
		else {
      cout << std::setw(10) << "OutOfWorld";
    }
    cout  << "    initStep" << endl;
  }
  cout.precision(prec);
}

}//close namespace
