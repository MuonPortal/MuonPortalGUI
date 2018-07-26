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


