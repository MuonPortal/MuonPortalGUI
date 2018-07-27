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
* @file ConfigParser.h
* @class ConfigParser
* @brief Parse the configuration file containing program parameters
* 
* @author S. Riggi
* @date 25/04/2010
*/
#ifndef _CONFIGPARSER_H_
#define _CONFIGPARSER_H_

#include <vector>
#include <string>

namespace MuonPortalNS {

class ConfigParser {
  
public:
  
  
	/** 
	\brief Class constructor
 	*/
  ConfigParser(std::string filename);
	ConfigParser();
	/** 
	\brief Class destructor
 	*/
  ~ConfigParser();
	/** 
	\brief Read the config file, parse and set info to be used by other classes
 	*/
	void ReadConfig();
	


private:

	/** 
	\brief Print parsed information
 	*/
	void Print();



private:

	std::string fConfigFileName;
	static std::string fInputFileName;

	//## GUI options
	static bool fEnableSounds;
	
	//## detector info	
	static std::vector<double> fZPlane;
	static bool fSmearTrackPoints;
	static double fSigmaPosX;
	static double fSigmaPosY;
	static double fSigmaPosZ;
	
	static double fContainerSizeX;
	static double fContainerSizeY;
	static double fContainerSizeZ;
	static double fContainerPosX;
	static double fContainerPosY;
	static double fContainerPosZ;
	static double fVoxelSizeX;
	static double fVoxelSizeY;
	static double fVoxelSizeZ;
	

	//## POCA reco
	static bool fUsePOCAAlgorithm;
	static int fPOCAVerbosity;
	static bool fUsePOCAQueueRun;
	static std::string fPOCAInputFileName;	
	static std::string fPOCAOutputFileName;
	static double fPOCATolerance;
	static bool fUsePOCAMaxNEvents;	
	static int fPOCAMaxNEvents;	
	static bool fUsePOCAThetaCut;	
	static double fPOCAThetaMin;
	static double fPOCAThetaMax;
	static bool fUsePOCAThetaWeight;	
	static double fPOCAThetaWeight;
	static bool fUsePOCAAverageSignal;
	static double fPOCASignalThreshold;
	static double fPOCANEventThreshold;
	static std::string fPOCADrawStyleName;
	static bool fUsePOCAEnergyCut;
  static double fPOCAEnergyMin;	
	static double fPOCAEnergyMax;

	//## ACF reco
	static bool fUseACFAlgorithm;
	static int fACFVerbosity;
	static bool fUseACFQueueRun;
	static std::string fACFInputFileName;
	static std::string fACFInputRandomFileName;	
	static std::string fACFOutputFileName;
	static bool fACFWaitPOCAOutput;
	static bool fUseACFMaxNEvents;	
	static int fACFMaxNEvents;	
	static bool fUseACFThetaCut;	
	static double fACFThetaMin;
	static double fACFThetaMax;
	static bool fUseACFThetaWeight;	
	static double fACFThetaWeight;
	static double fACFSignalThreshold;
	static double fACFVoxelSizeX;
	static double fACFVoxelSizeY;
	static double fACFVoxelSizeZ;
	static int fACFNCrownX;
	static int fACFNCrownY;
	static int fACFNCrownZ;
	static bool fUseACFEnergyCut;
  static double fACFEnergyMin;	
	static double fACFEnergyMax;

	//## Clustering reco
	static bool fUseClusteringAlgorithm;
	static int fClusteringVerbosity;
	static bool fUseClusteringQueueRun;
	static std::string fClusteringInputFileName;	
	static std::string fClusteringOutputFileName;
	static bool fClusteringWaitPOCAOutput;
	static bool fUseClusteringMaxNEvents;	
	static int fClusteringMaxNEvents;	
	static bool fUseClusteringThetaCut;	
	static double fClusteringThetaMin;
	static double fClusteringThetaMax;
	static bool fUseClusteringThetaWeight;	
	static double fClusteringThetaWeight;
	static bool fUseClusteringAverageSignal;
	static double fClusteringSignalThreshold;
	static double fClusteringNEventThreshold;
	static double fClusteringEpsilon;
	static double fClusteringMinNPts;
	static bool fUseClusteringEnergyCut;
  static double fClusteringEnergyMin;	
	static double fClusteringEnergyMax;

	//## EM-LL reco
	static bool fUseEMLLAlgorithm;
	static bool fUseEMLLParallelAlgorithm;
	static int fEMLLNParallelProc;
		
	static int fEMLLVerbosity;
	static bool fUseEMLLQueueRun;
	static std::string fEMLLInputFileName;	
	static std::string fEMLLOutputFileName;
	static int fNIterations;
	static double fInitialScatteringDensity;
	static bool fUseTrueMomentum;
	static double fAverageMomentum;
	static double fReferenceMomentum;
	static bool fUseDetectorCovariance;	
	static double fEx;
	static double fEy;
	static double fExy;		
	static bool fUseMedianUpdate;
	static bool fUseRegularization;
	static double fBeta;
	static bool fUseResErrStopping;
	static double fResErrTolerance; 
	static int fNIterForResErrStopping;
	static bool fUseLogLikeStopping;
	static double fLogLikeTolerance; 
	static int fNIterForLogLikeStopping;
	static bool fUseEMLLMaxNEvents;	
	static int fEMLLMaxNEvents;	
	static bool fUseEMLLThetaCut;	
	static double fEMLLThetaMin;
	static double fEMLLThetaMax;
	static bool fUseEMLLDisplacementCut;	
	static double fEMLLDisplacementMin;
	static double fEMLLDisplacementMax;
	static bool fUsePOCAInLL;
	static bool fUseTopBottomEvents;
	static bool fUseSideEvents;
	static double fEMLLSignalThreshold;
	static double fEMLLNEventThreshold;
	static std::string fEMLLDrawStyleName;

	static bool fUseEMLLTrueMomentum;
	static bool fUseEMLLEnergyCut;
  static double fEMLLEnergyMin;	
	static double fEMLLEnergyMax;
	
	static bool fEMLLSmearTimeOfFlight;
	static double fEMLLTimeOfFlightResolution;
	static bool fUseEMLLTrackSpeedCut;
	static double fEMLLTrackSpeedMinCut;
	static double fEMLLTrackSpeedMaxCut;


	static int fNMinEventInVoxelForUpdate;
	static bool fUseFilterVoxelEvents;

	static bool fUseEMLLQuantileUpdate;
	static double fEMLLQuantileProb;

	static bool fEMLLWaitClusteringOutput;
	static bool fEMLLUseClusteringInfo;
	static std::string fEMLLClusteringFileName;
	
	friend class G4DetectorGeometry;
	friend class ContainerTab;
	friend class EMLLTab;			
	friend class POCATab;		
	friend class ClusteringTab;	
	friend class GuiOptionsTab;		
	friend class ACFTab;	
	friend class TomographyRecTabMenu;
	friend class POCAThreadObj;	
	friend class EMTomographyRecoThreadObj;
	friend class EMTomographyRecoParallelThreadObj;		
	friend class FOFThreadObj;	
	friend class ACFAnalysisThreadObj;	
	
};//close ConfigParser class

}//close namespace 

#endif
 
