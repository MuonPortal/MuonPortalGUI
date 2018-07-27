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
* @file ConfigParser.cc
* @class ConfigParser
* @brief Parse the configuration file containing program parameters
* 
* @author S. Riggi
* @date 25/04/2010
*/

#include <ConfigParser.h>
#include <Logger.h>

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdexcept>

using namespace std;

namespace MuonPortalNS {

//## GUI options
bool ConfigParser::fEnableSounds;

//## Detector info
std::string ConfigParser::fInputFileName;
std::vector<double> ConfigParser::fZPlane;
double ConfigParser::fContainerSizeX;
double ConfigParser::fContainerSizeY;
double ConfigParser::fContainerSizeZ;
double ConfigParser::fContainerPosX;
double ConfigParser::fContainerPosY;
double ConfigParser::fContainerPosZ;
double ConfigParser::fVoxelSizeX;
double ConfigParser::fVoxelSizeY;
double ConfigParser::fVoxelSizeZ;
bool ConfigParser::fSmearTrackPoints;
double ConfigParser::fSigmaPosX;
double ConfigParser::fSigmaPosY;
double ConfigParser::fSigmaPosZ;

//## POCA IMAGE RECO
bool ConfigParser::fUsePOCAAlgorithm;
int ConfigParser::fPOCAVerbosity;
bool ConfigParser::fUsePOCAQueueRun;
std::string ConfigParser::fPOCAInputFileName;	
std::string ConfigParser::fPOCAOutputFileName;
double ConfigParser::fPOCATolerance;
bool ConfigParser::fUsePOCAMaxNEvents;	
int ConfigParser::fPOCAMaxNEvents;	
bool ConfigParser::fUsePOCAThetaCut;	
double ConfigParser::fPOCAThetaMin;
double ConfigParser::fPOCAThetaMax;
bool ConfigParser::fUsePOCAThetaWeight;	
double ConfigParser::fPOCAThetaWeight;
bool ConfigParser::fUsePOCAAverageSignal;
double ConfigParser::fPOCASignalThreshold;
double ConfigParser::fPOCANEventThreshold;
std::string ConfigParser::fPOCADrawStyleName;
bool ConfigParser::fUsePOCAEnergyCut;
double ConfigParser::fPOCAEnergyMin;	
double ConfigParser::fPOCAEnergyMax;

//## ACF IMAGE RECO
bool ConfigParser::fUseACFAlgorithm;
int ConfigParser::fACFVerbosity;
bool ConfigParser::fUseACFQueueRun;
std::string ConfigParser::fACFInputFileName;	
std::string ConfigParser::fACFInputRandomFileName;	
std::string ConfigParser::fACFOutputFileName;
bool ConfigParser::fACFWaitPOCAOutput;
bool ConfigParser::fUseACFMaxNEvents;	
int ConfigParser::fACFMaxNEvents;	
bool ConfigParser::fUseACFThetaCut;	
double ConfigParser::fACFThetaMin;
double ConfigParser::fACFThetaMax;
bool ConfigParser::fUseACFThetaWeight;	
double ConfigParser::fACFThetaWeight;
double ConfigParser::fACFSignalThreshold;
double ConfigParser::fACFVoxelSizeX;
double ConfigParser::fACFVoxelSizeY;
double ConfigParser::fACFVoxelSizeZ;
int ConfigParser::fACFNCrownX;
int ConfigParser::fACFNCrownY;
int ConfigParser::fACFNCrownZ;
bool ConfigParser::fUseACFEnergyCut;
double ConfigParser::fACFEnergyMin;	
double ConfigParser::fACFEnergyMax;

//## CLUSTERING IMAGE RECO
bool ConfigParser::fUseClusteringAlgorithm;
int ConfigParser::fClusteringVerbosity;
bool ConfigParser::fUseClusteringQueueRun;
std::string ConfigParser::fClusteringInputFileName;	
std::string ConfigParser::fClusteringOutputFileName;
bool ConfigParser::fClusteringWaitPOCAOutput;
bool ConfigParser::fUseClusteringMaxNEvents;	
int ConfigParser::fClusteringMaxNEvents;	
bool ConfigParser::fUseClusteringThetaCut;	
double ConfigParser::fClusteringThetaMin;
double ConfigParser::fClusteringThetaMax;
bool ConfigParser::fUseClusteringThetaWeight;	
double ConfigParser::fClusteringThetaWeight;
double ConfigParser::fClusteringSignalThreshold;
double ConfigParser::fClusteringNEventThreshold;
bool ConfigParser::fUseClusteringAverageSignal;
double ConfigParser::fClusteringEpsilon;
double ConfigParser::fClusteringMinNPts;
bool ConfigParser::fUseClusteringEnergyCut;
double ConfigParser::fClusteringEnergyMin;	
double ConfigParser::fClusteringEnergyMax;

//## EM-LL IMAGE RECO
bool ConfigParser::fUseEMLLAlgorithm;
bool ConfigParser::fUseEMLLParallelAlgorithm;
int ConfigParser::fEMLLNParallelProc;
int ConfigParser::fEMLLVerbosity;
bool ConfigParser::fUseEMLLQueueRun;
std::string ConfigParser::fEMLLInputFileName;	
std::string ConfigParser::fEMLLOutputFileName;
int ConfigParser::fNIterations;
bool ConfigParser::fUseRegularization;
double ConfigParser::fBeta;
bool ConfigParser::fUseMedianUpdate;
double ConfigParser::fInitialScatteringDensity;
bool ConfigParser::fUseDetectorCovariance;
double ConfigParser::fEx;
double ConfigParser::fEy;
double ConfigParser::fExy;
bool ConfigParser::fUseResErrStopping;
double ConfigParser::fResErrTolerance; 
int ConfigParser::fNIterForResErrStopping;
bool ConfigParser::fUseLogLikeStopping;
double ConfigParser::fLogLikeTolerance; 
int ConfigParser::fNIterForLogLikeStopping;
bool ConfigParser::fUsePOCAInLL;
bool ConfigParser::fUseSideEvents;
bool ConfigParser::fUseTopBottomEvents;
bool ConfigParser::fUseEMLLThetaCut;
double ConfigParser::fEMLLThetaMin;	
double ConfigParser::fEMLLThetaMax;
bool ConfigParser::fUseEMLLDisplacementCut;
double ConfigParser::fEMLLDisplacementMin;
double ConfigParser::fEMLLDisplacementMax;
double ConfigParser::fAverageMomentum;
double ConfigParser::fReferenceMomentum;
bool ConfigParser::fUseEMLLMaxNEvents;	
int ConfigParser::fEMLLMaxNEvents;
double ConfigParser::fEMLLSignalThreshold;
double ConfigParser::fEMLLNEventThreshold;
std::string ConfigParser::fEMLLDrawStyleName;

bool ConfigParser::fUseEMLLTrueMomentum;
bool ConfigParser::fUseEMLLEnergyCut;
double ConfigParser::fEMLLEnergyMin;	
double ConfigParser::fEMLLEnergyMax;	

bool ConfigParser::fEMLLSmearTimeOfFlight;
double ConfigParser::fEMLLTimeOfFlightResolution;
bool ConfigParser::fUseEMLLTrackSpeedCut;
double ConfigParser::fEMLLTrackSpeedMinCut;
double ConfigParser::fEMLLTrackSpeedMaxCut;

bool ConfigParser::fUseEMLLQuantileUpdate;
double ConfigParser::fEMLLQuantileProb;

int ConfigParser::fNMinEventInVoxelForUpdate;
bool ConfigParser::fUseFilterVoxelEvents;

bool ConfigParser::fEMLLWaitClusteringOutput;
bool ConfigParser::fEMLLUseClusteringInfo;
std::string ConfigParser::fEMLLClusteringFileName;

ConfigParser::ConfigParser(std::string filename){

	fConfigFileName= filename;
	fInputFileName= "";
	
	//## GUI options
	fEnableSounds= false;

	//## Detector info
	fZPlane.clear();
	fZPlane.resize(0);
	
	fContainerSizeX= 600.;//in cm
	fContainerSizeY= 240.;//in cm
	fContainerSizeZ= 240.;//in cm
	fContainerPosX= 0.;//in cm
	fContainerPosY= 0.;//in cm
	fContainerPosZ= 0.;//in cm
	fVoxelSizeX= 10.;//in cm
	fVoxelSizeY= 10.;//in cm
	fVoxelSizeZ= 10.;//in cm

	fSmearTrackPoints= true;
	fSigmaPosX= 1;//in cm
	fSigmaPosY= 1;//in cm
	fSigmaPosZ= 1;//in cm
	
	//## POCA IMAGE RECO CONFIG
	fUsePOCAAlgorithm= true;
	fPOCAVerbosity= 1;
	fUsePOCAQueueRun= false;
	fPOCAInputFileName= "";	
	fPOCAOutputFileName= "POCAOutput.root";
	fPOCATolerance= 1.e-8;
	fUsePOCAMaxNEvents= false;	
	fPOCAMaxNEvents= 1.e+6;	
	fUsePOCAThetaCut= false;	
	fPOCAThetaMin= 0;
	fPOCAThetaMax= 100;
	fUsePOCAThetaWeight= true;	
	fPOCAThetaWeight= 2;
	fUsePOCAAverageSignal= false;
	fPOCASignalThreshold= 1000;
	fPOCANEventThreshold= 1;
	fPOCADrawStyleName= "COLZ";
	fUsePOCAEnergyCut= false;
  fPOCAEnergyMin= 0;//GeV
	fPOCAEnergyMax= 100;//GeV

	//## TOMOGRAPHY IMAGE RECO CONFIG
	fUseEMLLAlgorithm= true;
	fUseEMLLQueueRun= false;
	fUseEMLLParallelAlgorithm= false;
	fEMLLNParallelProc= 1;
	fEMLLVerbosity= 1;
	fEMLLInputFileName= "";	
	fEMLLOutputFileName= "EMLLOutput.root";
	fNIterations= 100;
	fUseRegularization= false;
	fBeta= 100;
	fUseMedianUpdate= false;
	fInitialScatteringDensity= 6.11172e-09;//init value for Air in rad^2/cm
	fUseDetectorCovariance= true;
	fEx= 0;
	fEy= 0;
	fExy= 0;
	fUseResErrStopping= false;
	fResErrTolerance= 1.e-06; 
	fNIterForResErrStopping= 5;
	fUseLogLikeStopping= false;
	fLogLikeTolerance= 1.e-06; 
	fNIterForLogLikeStopping= 5;
	fAverageMomentum= 1.;//in GeV/c
	fReferenceMomentum= 1;//GeV/c
	fUsePOCAInLL= true;	
	fUseSideEvents= false;
	fUseTopBottomEvents= true;
	fUseEMLLThetaCut= false;
	fEMLLThetaMin= 0;//in degree
	fEMLLThetaMax= 100;//in degree
	fUseEMLLDisplacementCut= false;
	fEMLLDisplacementMin= 1;//in cm
	fEMLLDisplacementMax= 100;//in cm
	fUseEMLLMaxNEvents= false;	
	fEMLLMaxNEvents= 1.e+6;	

	fUseEMLLTrueMomentum= false;
	fUseEMLLEnergyCut= false;
	fEMLLEnergyMin= 0;
	fEMLLEnergyMax= 100;
	fNMinEventInVoxelForUpdate= 10;
	fUseFilterVoxelEvents= false;

	fEMLLSmearTimeOfFlight= false;
	fEMLLTimeOfFlightResolution= 5;//in ns
	fUseEMLLTrackSpeedCut= false;
	fEMLLTrackSpeedMinCut= 0;
	fEMLLTrackSpeedMaxCut= 1;

	fUseEMLLQuantileUpdate= false;
	fEMLLQuantileProb= 0.5;

	fEMLLWaitClusteringOutput= false;
	fEMLLUseClusteringInfo= false;
	fEMLLClusteringFileName= "FOFOutput.root";

	//## ACF IMAGE RECO
	fUseACFAlgorithm= true;
	fUseACFQueueRun= false;	
	fACFVerbosity= 1;
	fACFInputFileName= "";
	fACFInputRandomFileName= "";	
	fACFOutputFileName= "";
	fACFWaitPOCAOutput= true;
	fUseACFMaxNEvents= true;	
	fACFMaxNEvents= 5.e+5;	
	fUseACFThetaCut= true;	
	fACFThetaMin= 2;
	fACFThetaMax= 100;
	fUseACFThetaWeight= true;	
	fACFThetaWeight= 2;
	fACFSignalThreshold= 0.5;
	fACFVoxelSizeX= 20;//cm
	fACFVoxelSizeY= 20;//cm
	fACFVoxelSizeZ= 20;//cm
	fACFNCrownX= 2;
	fACFNCrownY= 2;
	fACFNCrownZ= 2;
	fUseACFEnergyCut= false;
  fACFEnergyMin= 0;//GeV
	fACFEnergyMax= 100;//GeV

	//## Clustering reco
	fUseClusteringAlgorithm= true;
	fUseClusteringQueueRun= false;
	fClusteringVerbosity= 1;
	fClusteringInputFileName= "";	
	fClusteringOutputFileName= "";
	fClusteringWaitPOCAOutput= true;
	fUseClusteringMaxNEvents= true;	
	fClusteringMaxNEvents= 5.e+5;	
	fUseClusteringThetaCut= true;	
	fClusteringThetaMin= 2;
	fClusteringThetaMax= 100;
	fUseClusteringThetaWeight= true;	
	fClusteringThetaWeight= 2;
	fUseClusteringAverageSignal= false;
	fClusteringSignalThreshold= 0.5;
	fClusteringNEventThreshold= 0;
	fClusteringEpsilon= 5;//cm
	fClusteringMinNPts= 10;
	fUseClusteringEnergyCut= false;
  fClusteringEnergyMin= 0;//GeV
	fClusteringEnergyMax= 100;//GeV

}//close costructor


ConfigParser::ConfigParser(){

	fInputFileName= "";

	//## Detector info
	fZPlane.clear();
	fZPlane.resize(0);
	
	fContainerSizeX= 600.;//in cm
	fContainerSizeY= 240.;//in cm
	fContainerSizeZ= 240.;//in cm
	fContainerPosX= 0.;//in cm
	fContainerPosY= 0.;//in cm
	fContainerPosZ= 0.;//in cm
	fVoxelSizeX= 10.;//in cm
	fVoxelSizeY= 10.;//in cm
	fVoxelSizeZ= 10.;//in cm

	fSmearTrackPoints= true;
	fSigmaPosX= 1;//in cm
	fSigmaPosY= 1;//in cm
	fSigmaPosZ= 1;//in cm
	
	//## POCA IMAGE RECO CONFIG
	fPOCAVerbosity= 1;
	fUsePOCAQueueRun= false;
	fPOCAInputFileName= "";	
	fPOCAOutputFileName= "POCAOutput.root";
	fPOCATolerance= 1.e-8;
	fUsePOCAMaxNEvents= false;	
	fPOCAMaxNEvents= 1.e+6;	
	fUsePOCAThetaCut= false;	
	fPOCAThetaMin= 0;
	fPOCAThetaMax= 100;
	fUsePOCAThetaWeight= true;	
	fPOCAThetaWeight= 2;
	fUsePOCAAverageSignal= false;
	fPOCASignalThreshold= 1000;
	fPOCANEventThreshold= 1;
	fPOCADrawStyleName= "COLZ";

	//## TOMOGRAPHY IMAGE RECO CONFIG
	fUseEMLLAlgorithm= true;
	fUseEMLLQueueRun= false;
	fUseEMLLParallelAlgorithm= false;
	fEMLLNParallelProc= 1;
	fEMLLVerbosity= 1;
	fEMLLInputFileName= "";	
	fEMLLOutputFileName= "EMLLOutput.root";
	fNIterations= 100;
	fUseRegularization= false;
	fBeta= 100;
	fUseMedianUpdate= false;
	fInitialScatteringDensity= 6.11172e-09;//init value for Air in rad^2/cm
	fUseDetectorCovariance= true;
	fEx= 0;
	fEy= 0;
	fExy= 0;
	fUseResErrStopping= false;
	fResErrTolerance= 1.e-06; 
	fNIterForResErrStopping= 5;
	fUseLogLikeStopping= false;
	fLogLikeTolerance= 1.e-06; 
	fNIterForLogLikeStopping= 5;
	fAverageMomentum= 1.;//in GeV/c
	fReferenceMomentum= 1;//GeV/c
	fUsePOCAInLL= true;	
	fUseSideEvents= false;
	fUseTopBottomEvents= true;
	fUseEMLLThetaCut= false;
	fEMLLThetaMin= 0;//in degree
	fEMLLThetaMax= 100;//in degree
	fUseEMLLDisplacementCut= false;
	fEMLLDisplacementMin= 1;//in cm
	fEMLLDisplacementMax= 100;//in cm
	fUseEMLLMaxNEvents= false;	
	fEMLLMaxNEvents= 1.e+6;	
	
	fUseEMLLTrueMomentum= false;
	fUseEMLLEnergyCut= false;
	fEMLLEnergyMin= 0;
	fEMLLEnergyMax= 100;
	fNMinEventInVoxelForUpdate= 10;
	fUseFilterVoxelEvents= false;

	fEMLLSmearTimeOfFlight= false;
	fEMLLTimeOfFlightResolution= 5;//in ns
	fUseEMLLTrackSpeedCut= false;
	fEMLLTrackSpeedMinCut= 0;
	fEMLLTrackSpeedMaxCut= 1;

	fUseEMLLQuantileUpdate= false;
	fEMLLQuantileProb= 0.5;

	fEMLLWaitClusteringOutput= false;
	fEMLLUseClusteringInfo= false;
	fEMLLClusteringFileName= "FOFOutput.root";

	fUseACFAlgorithm= true;
	fUseACFQueueRun= false;
	fACFVerbosity= 1;
	fACFInputFileName= "";
	fACFInputRandomFileName= "";	
	fACFOutputFileName= "";
	fACFWaitPOCAOutput= true;
	fUseACFMaxNEvents= true;	
	fACFMaxNEvents= 5.e+5;	
	fUseACFThetaCut= true;	
	fACFThetaMin= 2;
	fACFThetaMax= 100;
	fUseACFThetaWeight= true;	
	fACFThetaWeight= 2;
	fACFSignalThreshold= 0.5;
	fACFVoxelSizeX= 20;//cm
	fACFVoxelSizeY= 20;//cm
	fACFVoxelSizeZ= 20;//cm
	fACFNCrownX= 2;
	fACFNCrownY= 2;
	fACFNCrownZ= 2;
	fUseACFEnergyCut= false;
  fACFEnergyMin= 0;//GeV
	fACFEnergyMax= 100;//GeV

	//## Clustering reco
	fUseClusteringAlgorithm= true;
	fUseClusteringQueueRun= false;
	fClusteringVerbosity= 1;
	fClusteringInputFileName= "";	
	fClusteringOutputFileName= "";
	fClusteringWaitPOCAOutput= true;
	fUseClusteringMaxNEvents= true;	
	fClusteringMaxNEvents= 5.e+5;	
	fUseClusteringThetaCut= true;	
	fClusteringThetaMin= 2;
	fClusteringThetaMax= 100;
	fUseClusteringThetaWeight= true;	
	fClusteringThetaWeight= 2;
	fUseClusteringAverageSignal= false;
	fClusteringSignalThreshold= 0.5;
	fClusteringNEventThreshold= 0;
	fClusteringEpsilon= 5;//cm
	fClusteringMinNPts= 10;
	fUseClusteringEnergyCut= false;
  fClusteringEnergyMin= 0;//GeV
	fClusteringEnergyMax= 100;//GeV

}//close costructor

ConfigParser::~ConfigParser(){

}//close destructor


void ConfigParser::ReadConfig() {

	// read configuration from file
	INFO_LOG("Reading and parsing file "<<fConfigFileName.c_str()<<" ...");

  ifstream in;  
  in.open(fConfigFileName.c_str());
  if(!in.good()) {
		std::stringstream ss;
		ss<<"Cannot read config file "<<fConfigFileName<<" (no configuration settings!!!)";
		ERROR_LOG(ss.str());
    throw std::runtime_error(ss.str());
		exit(1);
  }

	//Start parsing the config file
	char buffer[1000];//container for a full line
  std::string descriptor;//container for config descriptor 
  in.getline(buffer,1000);//get the full line
	std::string parsedline;

	
	while(std::getline(in,parsedline)) {
		
		char first_char= *(parsedline.c_str());
		
		if(first_char!='#' && first_char!='\n' && first_char!=' '){
			stringstream line(parsedline);
			stringstream line_copy(parsedline);
     
			line_copy >> descriptor;
      
      if(descriptor!="\n" && descriptor!=""){
				//get all config parameters

				//####################################
				//####  DETECTOR GEOMETRY
				//####################################
				if(descriptor.compare("planeZ")==0){
					double zplane;
					line >> descriptor >> zplane;			
					fZPlane.push_back(zplane);
		  	}
				
				else if(descriptor.compare("containerSize")==0){
					line >> descriptor >> fContainerSizeX >> fContainerSizeY >> fContainerSizeZ;
				}
				else if(descriptor.compare("containerPosition")==0){
					line >> descriptor >> fContainerPosX >> fContainerPosY >> fContainerPosZ;
				}
				else if(descriptor.compare("voxelSize")==0){
					line >> descriptor >> fVoxelSizeX >> fVoxelSizeY >> fVoxelSizeZ;
				}

				//####################################
				//####  GUI OPTIONS
				//####################################
				else if(descriptor.compare("enableSounds")==0){
					line >> descriptor >> fEnableSounds;
				}

				//########################
				//###   RECO CONFIG    ###
				//########################
			
				else if(descriptor.compare("smearTrackPoints")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue;			
					if(thisFlagValue.compare("T")==0) fSmearTrackPoints= true;
					else if(thisFlagValue.compare("F")==0) fSmearTrackPoints= false;
					else{
						string errMsg = "Invalid setting for smearTrackPoints, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}	
				else if(descriptor.compare("detectorResolution")==0){
					line >> descriptor >> fSigmaPosX >> fSigmaPosY >> fSigmaPosZ;
				}
				

				//## POCA IMAGE RECO
				else if(descriptor.compare("usePOCAAlgorithm")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue;			
					if(thisFlagValue.compare("T")==0) fUsePOCAAlgorithm= true;
					else if(thisFlagValue.compare("F")==0) fUsePOCAAlgorithm= false;
					else{
						string errMsg = "ConfigParser::ReadConfig(): ERROR: Invalid setting for usePOCAAlgorithm, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				else if(descriptor.compare("usePOCAQueueRun")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue;			
					if(thisFlagValue.compare("T")==0) fUsePOCAQueueRun= true;
					else if(thisFlagValue.compare("F")==0) fUsePOCAQueueRun= false;
					else{
						string errMsg = "Invalid setting for usePOCAQueueRun, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}


				else if(descriptor.compare("verbosityPOCA")==0){
					line >> descriptor >> fPOCAVerbosity;
				}
				else if(descriptor.compare("usePOCANEventCut")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fPOCAMaxNEvents;			
					if(thisFlagValue.compare("T")==0) fUsePOCAMaxNEvents= true;
					else if(thisFlagValue.compare("F")==0) fUsePOCAMaxNEvents= false;
					else{
						string errMsg = "Invalid setting for usePOCANEventCut, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}	
				else if(descriptor.compare("usePOCAScatteringAngleCut")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fPOCAThetaMin >> fPOCAThetaMax;			
					if(thisFlagValue.compare("T")==0) fUsePOCAThetaCut= true;
					else if(thisFlagValue.compare("F")==0) fUsePOCAThetaCut= false;
					else{
						string errMsg = "Invalid setting for usePOCAScatteringAngleCut, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				else if(descriptor.compare("usePOCAEnergyCut")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fPOCAEnergyMin >> fPOCAEnergyMax;			
					if(thisFlagValue.compare("T")==0) fUsePOCAEnergyCut= true;
					else if(thisFlagValue.compare("F")==0) fUsePOCAEnergyCut= false;
					else{
						string errMsg = "Invalid setting for usePOCAEnergyCut, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}	
				else if(descriptor.compare("usePOCAThetaWeight")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fPOCAThetaWeight;			
					if(thisFlagValue.compare("T")==0) fUsePOCAThetaWeight= true;
					else if(thisFlagValue.compare("F")==0) fUsePOCAThetaWeight= false;
					else{
						string errMsg = "Invalid setting for usePOCAThetaWeight, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}	
				else if(descriptor.compare("usePOCAAverageSignal")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue;			
					if(thisFlagValue.compare("T")==0) fUsePOCAAverageSignal= true;
					else if(thisFlagValue.compare("F")==0) fUsePOCAAverageSignal= false;
					else{
						string errMsg = "Invalid setting for usePOCAAverageSignal, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}	
				else if(descriptor.compare("POCAAlarmThreshold")==0){
					line >> descriptor >> fPOCANEventThreshold >> fPOCASignalThreshold;
				}
	
	

				//## FOF IMAGE RECO			
				else if(descriptor.compare("useClusteringAlgorithm")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue;			
					if(thisFlagValue.compare("T")==0) fUseClusteringAlgorithm= true;
					else if(thisFlagValue.compare("F")==0) fUseClusteringAlgorithm= false;
					else{
						string errMsg = "Invalid setting for useClusteringAlgorithm, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				else if(descriptor.compare("useClusteringQueueRun")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue;			
					if(thisFlagValue.compare("T")==0) fUseClusteringQueueRun= true;
					else if(thisFlagValue.compare("F")==0) fUseClusteringQueueRun= false;
					else{
						string errMsg = "Invalid setting for useClusteringQueueRun, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				else if(descriptor.compare("verbosityClustering")==0){
					line >> descriptor >> fClusteringVerbosity;
				}
				else if(descriptor.compare("useClusteringNEventCut")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fClusteringMaxNEvents;			
					if(thisFlagValue.compare("T")==0) fUseClusteringMaxNEvents= true;
					else if(thisFlagValue.compare("F")==0) fUseClusteringMaxNEvents= false;
					else{
						string errMsg = "Invalid setting for useClusteringNEventCut, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}	
				else if(descriptor.compare("useClusteringScatteringAngleCut")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fClusteringThetaMin >> fClusteringThetaMax;			
					if(thisFlagValue.compare("T")==0) fUseClusteringThetaCut= true;
					else if(thisFlagValue.compare("F")==0) fUseClusteringThetaCut= false;
					else{
						string errMsg = "Invalid setting for useClusteringScatteringAngleCut, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				else if(descriptor.compare("useClusteringEnergyCut")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fClusteringEnergyMin >> fClusteringEnergyMax;			
					if(thisFlagValue.compare("T")==0) fUseClusteringEnergyCut= true;
					else if(thisFlagValue.compare("F")==0) fUseClusteringEnergyCut= false;
					else{
						string errMsg = "Invalid setting for useClusteringEnergyCut, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}	
				else if(descriptor.compare("useClusteringThetaWeight")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fClusteringThetaWeight;			
					if(thisFlagValue.compare("T")==0) fUseClusteringThetaWeight= true;
					else if(thisFlagValue.compare("F")==0) fUseClusteringThetaWeight= false;
					else{
						string errMsg = "Invalid setting for useClusteringThetaWeight, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}	
				else if(descriptor.compare("useClusteringAverageSignal")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue;			
					if(thisFlagValue.compare("T")==0) fUseClusteringAverageSignal= true;
					else if(thisFlagValue.compare("F")==0) fUseClusteringAverageSignal= false;
					else{
						string errMsg = "Invalid setting for useClusteringAverageSignal, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}	
				else if(descriptor.compare("ClusteringEpsilon")==0){
					line >> descriptor >> fClusteringEpsilon;
				}
				else if(descriptor.compare("ClusteringMinNPts")==0){
					line >> descriptor >> fClusteringMinNPts;
				}
				else if(descriptor.compare("ClusteringAlarmThreshold")==0){
					line >> descriptor >> fClusteringNEventThreshold >> fClusteringSignalThreshold;
				}
	
				//## ACF IMAGE RECO
				else if(descriptor.compare("useACFAlgorithm")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue;			
					if(thisFlagValue.compare("T")==0) fUseACFAlgorithm= true;
					else if(thisFlagValue.compare("F")==0) fUseACFAlgorithm= false;
					else{
						string errMsg = "Invalid setting for useACFAlgorithm, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				else if(descriptor.compare("useACFQueueRun")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue;			
					if(thisFlagValue.compare("T")==0) fUseACFQueueRun= true;
					else if(thisFlagValue.compare("F")==0) fUseACFQueueRun= false;
					else{
						string errMsg = "Invalid setting for useACFQueueRun, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				else if(descriptor.compare("verbosityACF")==0){
					line >> descriptor >> fACFVerbosity;
				}
				else if(descriptor.compare("randomDataFileACF")==0){
					line >> descriptor >> fACFInputRandomFileName;
				}
				else if(descriptor.compare("useACFNEventCut")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fACFMaxNEvents;			
					if(thisFlagValue.compare("T")==0) fUseACFMaxNEvents= true;
					else if(thisFlagValue.compare("F")==0) fUseACFMaxNEvents= false;
					else{
						string errMsg = "Invalid setting for useACFNEventCut, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}	

				else if(descriptor.compare("useACFEnergyCut")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fACFEnergyMin >> fACFEnergyMax;			
					if(thisFlagValue.compare("T")==0) fUseACFEnergyCut= true;
					else if(thisFlagValue.compare("F")==0) fUseACFEnergyCut= false;
					else{
						string errMsg = "Invalid setting for useACFEnergyCut, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}		
				
				else if(descriptor.compare("useACFScatteringAngleCut")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fACFThetaMin >> fACFThetaMax;			
					if(thisFlagValue.compare("T")==0) fUseACFThetaCut= true;
					else if(thisFlagValue.compare("F")==0) fUseACFThetaCut= false;
					else{
						string errMsg = "Invalid setting for useACFScatteringAngleCut, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				else if(descriptor.compare("useACFThetaWeight")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fACFThetaWeight;			
					if(thisFlagValue.compare("T")==0) fUseACFThetaWeight= true;
					else if(thisFlagValue.compare("F")==0) fUseACFThetaWeight= false;
					else{
						string errMsg = "Invalid setting for useACFThetaWeight, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}	
				else if(descriptor.compare("ACFVoxelSize")==0){
					line >> descriptor >> fACFVoxelSizeX >> fACFVoxelSizeY >> fACFVoxelSizeZ;
				}
				else if(descriptor.compare("ACFNCrown")==0){
					line >> descriptor >> fACFNCrownX >> fACFNCrownY >> fACFNCrownZ;
				}
				else if(descriptor.compare("ACFAlarmThreshold")==0){
					line >> descriptor >> fACFSignalThreshold;
				}

				//## EM-LL IMAGE RECO
				else if(descriptor.compare("verbosityEMLL")==0){
					line >> descriptor >> fEMLLVerbosity;
				}
				else if(descriptor.compare("useEMLLQueueRun")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue;			
					if(thisFlagValue.compare("T")==0) fUseEMLLQueueRun= true;
					else if(thisFlagValue.compare("F")==0) fUseEMLLQueueRun= false;
					else{
						string errMsg = "Invalid setting for useEMLLQueueRun, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				else if(descriptor.compare("useEMLLAlgorithm")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue;			
					if(thisFlagValue.compare("T")==0) fUseEMLLAlgorithm= true;
					else if(thisFlagValue.compare("F")==0) fUseEMLLAlgorithm= false;
					else{
						string errMsg = "Invalid setting for useEMLLAlgorithm, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}

				else if(descriptor.compare("useEMLLParallelAlgorithm")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fEMLLNParallelProc;			
					if(thisFlagValue.compare("T")==0) fUseEMLLParallelAlgorithm= true;
					else if(thisFlagValue.compare("F")==0) fUseEMLLParallelAlgorithm= false;
					else{
						string errMsg = "Invalid setting for useEMLLParallelAlgorithm, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}

				else if(descriptor.compare("useEMLLNEventCut")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fEMLLMaxNEvents;			
					if(thisFlagValue.compare("T")==0) fUseEMLLMaxNEvents= true;
					else if(thisFlagValue.compare("F")==0) fUseEMLLMaxNEvents= false;
					else{
						string errMsg = "Invalid setting for useEMLLNEventCut, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}	

				else if(descriptor.compare("useEMLLEnergyCut")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fEMLLEnergyMin >> fEMLLEnergyMax;			
					if(thisFlagValue.compare("T")==0) fUseEMLLEnergyCut= true;
					else if(thisFlagValue.compare("F")==0) fUseEMLLEnergyCut= false;
					else{
						string errMsg = "Invalid setting for useEMLLEnergyCut, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}	

				else if(descriptor.compare("smearEMLLTimeOfFlight")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fEMLLTimeOfFlightResolution;			
					if(thisFlagValue.compare("T")==0) fEMLLSmearTimeOfFlight= true;
					else if(thisFlagValue.compare("F")==0) fEMLLSmearTimeOfFlight= false;
					else{
						string errMsg = "Invalid setting for smearEMLLTimeOfFlight, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}	

				else if(descriptor.compare("useEMLLTrackSpeedCut")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fEMLLTrackSpeedMinCut >> fEMLLTrackSpeedMaxCut;			
					if(thisFlagValue.compare("T")==0) fUseEMLLTrackSpeedCut= true;
					else if(thisFlagValue.compare("F")==0) fUseEMLLTrackSpeedCut= false;
					else{
						string errMsg = "Invalid setting for useEMLLTrackSpeedCut, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}	

				else if(descriptor.compare("nIteration")==0){
					line >> descriptor >> fNIterations;
				}
				else if(descriptor.compare("initScatteringDensity")==0){
					line >> descriptor >> fInitialScatteringDensity;
				}
				else if(descriptor.compare("useTrueMomentum")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue;			
					if(thisFlagValue.compare("T")==0) fUseEMLLTrueMomentum= true;
					else if(thisFlagValue.compare("F")==0) fUseEMLLTrueMomentum= false;
					else{
						string errMsg = "Invalid setting for useTrueMomentum, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				else if(descriptor.compare("referenceMomentum")==0){
					line >> descriptor >> fReferenceMomentum;
				}
				else if(descriptor.compare("averageMomentum")==0){
					line >> descriptor >> fAverageMomentum;
				}
				else if(descriptor.compare("useDetectorCovariance")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fEx >> fEy >> fExy;			
					if(thisFlagValue.compare("T")==0) fUseDetectorCovariance= true;
					else if(thisFlagValue.compare("F")==0) fUseDetectorCovariance= false;
					else{
						string errMsg = "Invalid setting for useDetectorCovariance, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				else if(descriptor.compare("useRegularization")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fBeta;			
					if(thisFlagValue.compare("T")==0) fUseRegularization= true;
					else if(thisFlagValue.compare("F")==0) fUseRegularization= false;
					else{
						string errMsg = "Invalid setting for useRegularization, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
	
				else if(descriptor.compare("useMedianUpdate")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue;			
					if(thisFlagValue.compare("T")==0) fUseMedianUpdate= true;
					else if(thisFlagValue.compare("F")==0) fUseMedianUpdate= false;
					else{
						string errMsg = "Invalid setting for useMedianUpdate, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}

				
				
				else if(descriptor.compare("useResErrStopping")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fResErrTolerance >> fNIterForResErrStopping;			
					if(thisFlagValue.compare("T")==0) fUseResErrStopping= true;
					else if(thisFlagValue.compare("F")==0) fUseResErrStopping= false;
					else{
						string errMsg = "Invalid setting for useResErrStopping, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}

				
				else if(descriptor.compare("useLogLikeStopping")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fLogLikeTolerance >> fNIterForLogLikeStopping;			
					if(thisFlagValue.compare("T")==0) fUseLogLikeStopping= true;
					else if(thisFlagValue.compare("F")==0) fUseLogLikeStopping= false;
					else{
						string errMsg = "Invalid setting for useLogLikeStopping, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				else if(descriptor.compare("usePOCAInLL")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue;			
					if(thisFlagValue.compare("T")==0) fUsePOCAInLL= true;
					else if(thisFlagValue.compare("F")==0) fUsePOCAInLL= false;
					else{
						string errMsg = "Invalid setting for usePOCAInLL, use T or F!";			
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				
				else if(descriptor.compare("useSideEvents")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue;			
					if(thisFlagValue.compare("T")==0) fUseSideEvents= true;
					else if(thisFlagValue.compare("F")==0) fUseSideEvents= false;
					else{
						string errMsg = "Invalid setting for useSideEvents, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				else if(descriptor.compare("useTopBottomEvents")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue;			
					if(thisFlagValue.compare("T")==0) fUseTopBottomEvents= true;
					else if(thisFlagValue.compare("F")==0) fUseTopBottomEvents= false;
					else{
						string errMsg = "Invalid setting for useTopBottomEvents, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}


				else if(descriptor.compare("useEMLLScatteringAngleCut")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fEMLLThetaMin >> fEMLLThetaMax;			
					if(thisFlagValue.compare("T")==0) fUseEMLLThetaCut= true;
					else if(thisFlagValue.compare("F")==0) fUseEMLLThetaCut= false;
					else{
						string errMsg = "Invalid setting for useScatteringAngleCut, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				else if(descriptor.compare("useEMLLDisplacementCut")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fEMLLDisplacementMin >> fEMLLDisplacementMax;			
					if(thisFlagValue.compare("T")==0) fUseEMLLDisplacementCut= true;
					else if(thisFlagValue.compare("F")==0) fUseEMLLDisplacementCut= false;
					else{
						string errMsg = "Invalid setting for useDisplacementCut, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				else if(descriptor.compare("NMinEventInVoxelForUpdate")==0){
					line >> descriptor >> fNMinEventInVoxelForUpdate;
				}
				else if(descriptor.compare("useFilterVoxelEvents")==0){
					std::string thisFlagValue;
					std::string thisFlagValue2;
		    	line >> descriptor >> thisFlagValue;			
					if(thisFlagValue.compare("T")==0) fUseFilterVoxelEvents= true;
					else if(thisFlagValue.compare("F")==0) fUseFilterVoxelEvents= false;
					else{
						string errMsg = "Invalid setting for useFilterVoxelEvents, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				else if(descriptor.compare("useQuantileUpdate")==0){
					std::string thisFlagValue;
		    	line >> descriptor >> thisFlagValue >> fEMLLQuantileProb;			
					if(thisFlagValue.compare("T")==0) fUseEMLLQuantileUpdate= true;
					else if(thisFlagValue.compare("F")==0) fUseEMLLQuantileUpdate= false;
					else{
						string errMsg = "Invalid setting for useQuantileUpdate, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				else if(descriptor.compare("useClusteringInfo")==0){
					std::string thisFlagValue;
					std::string thisFlagValue2;
		    	line >> descriptor >> thisFlagValue >> fEMLLClusteringFileName;			
					if(thisFlagValue.compare("T")==0) fEMLLUseClusteringInfo= true;
					else if(thisFlagValue.compare("F")==0) fEMLLUseClusteringInfo= false;
					else{
						string errMsg = "Invalid setting for useClusteringInfo, use T or F!";
						ERROR_LOG(errMsg);
    				throw std::runtime_error(errMsg);
						exit(1);
					}
		  	}
				else if(descriptor.compare("EMLLAlarmThreshold")==0){
					line >> descriptor >> fEMLLNEventThreshold >> fEMLLSignalThreshold;
				}


				else{
					//config setting not defined
					line >> descriptor;
					std::stringstream ss;
					ss<<"Descriptor "<<descriptor<<" not defined (bad settings)!";
					ERROR_LOG(ss.str());
    			throw std::runtime_error(ss.str());
					exit(1);
				}//close else

			}//close if descriptor
		}//close if buffer

		if (!in.good()) break;
	}//close while

	in.close();

	
	//## Print parsed info
	Print();

}//close ReadConfig()



void ConfigParser::Print(){

	//print parsed information
	cout<<"################################"<<endl;
	cout<<"###     PARSED  SETTINGS   #####"<<endl;
	cout<<"################################"<<endl;
		
}//close Print()


}//close namespace
