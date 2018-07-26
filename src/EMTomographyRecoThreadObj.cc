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
* @file EMTomographyRecoThreadObj.cc
* @class EMTomographyRecoThreadObj
* @brief EM-ML algorithm processing thread
* 
* @author S. Riggi
* @date 25/04/2010
*/
#include <EMTomographyRecoThreadObj.h>
#include <VoxelData.h>
#include <MuonEventData.h>
#include <Gui.h>
#include <QtGui>
#include <QString>

#include <QUdpSocket>
#include <QDataStream>
#include <QHostAddress>
#include <QThread>

#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <qjson/qjson_export.h>
#include <qjson/qobjecthelper.h>
#include <qjson/serializerrunnable.h>
#include <qjson/parserrunnable.h>

#include <ConfigParser.h>
#include <VoxelData.h>
#include <MuonEventData.h>
#include <AnalysisConsts.h>
#include <Utilities.h>

//## G4 classes
#include <G4DetectorGeometry.h>
#include <G4PhysicsList.h>
#include <G4PrimaryGenerator.h>
#include <SteppingVerbose.h>

#include <G4Navigator.hh>
#include <G4GeometryManager.hh>
#include <G4TransportationManager.hh>
#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <G4Material.hh>
#include <G4VPhysicalVolume.hh>
#include <G4UnitsTable.hh>

#ifdef G4VIS_USE
#include <G4VisExecutive.hh>
#endif

//## ROOT classes
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TF1.h>
#include <TH2.h>
#include <TLegend.h>
#include <TCut.h>
#include <TEventList.h>
#include <TMath.h>
#include <TPad.h>
#include <TVirtualPad.h>
#include <TCanvas.h>
#include <TStyle.h>

#include <TSystem.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TMath.h>
#include <TRandom3.h>
#include <TVector3.h>
#include <TVirtualFitter.h>
#include <TPolyLine3D.h>
#include <TGeoTrack.h>
#include <TGraph.h>
#include <TGraph2D.h>
#include <TApplication.h>
#include <TGeoManager.h>
#include <TColor.h>

//## standard classes
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <assert.h>
#include <utility>
#include <numeric>


#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <fcntl.h>


using namespace std;

namespace MuonPortalNS {

EMTomographyRecoThreadObj::EMTomographyRecoThreadObj() {

	fIsThreadStopped= false;

	fFileName= "prova.root";
	fDestinationPath= ".";
	fRemotePortNumber= 5901;
	fRemoteHostIp= "127.0.0.1";
	fRemoteUserName= "pippo";
	fConfigFileName= "EMTomographyImagingConfig.cfg";
	fLogFileName= "out.log";
	fIsBatchRun= false;

	//## Connect status signal to socket sender
	connect(this,SIGNAL(logSig(QString)), this, SLOT(sender(QString)) );
	connect(this,SIGNAL(alarmSig(QString)), this, SLOT(sender(QString)) );

	//## General
	fInputFileName= "";
	fOutputFileName= "EMLLOutput.root"; 
	fVerbosity= 2;
	
	//## Algorithm parameters
	fScatteringDensityInit= 6.11172e-09;//init value for Air in rad^2/cm
	fNMaxRecEvents= -1;//-1= all events
	fNIterations= 10;
	fUseRegularization= false;
	fBeta= 1.e+08;
	fReferenceMomentum= 3.;//in GeV/c
	fUseMedianUpdate= false;
	fUseDetectorCovariance= false;
	fUseAcceleration= false;
	fNSecants= 3;
	fUseResErrStopping= false;
	fResErrTolerance= 1.e-05;
	fNIterForResErrStopping= 5;	
	fUseLogLikeStopping= false;
	fLogLikeTolerance= 0.1;
	fNIterForLogLikeStopping= 5;
	fUseSideEvents= false;

	//## G4 classes
	fG4Navigator= 0;
  fUImanager= 0;
	fRunManager= 0;
	fVisManager= 0;

	//## Algorithm data
	fMuonEventData= 0;
	fMuonEventDataCollection.clear();
	fMuonEventDataCollection.resize(0);

}//close constructor
     
EMTomographyRecoThreadObj::~EMTomographyRecoThreadObj() {

	//## Delete objects
	if(fRunManager) delete fRunManager;
	
	if(fTemperaturePalette) fTemperaturePalette->Delete();

	/*
	for(int k=0;k<fNVoxelX;k++) {
		if(RecMapPlot_XFix[k]) RecMapPlot_XFix[k]->Destructor();
	}
	for(int k=0;k<fNVoxelY;k++) {
		if(RecMapPlot_YFix[k]) RecMapPlot_YFix[k]->Destructor();
	}
	for(int k=0;k<fNVoxelZ;k++) {
		if(RecMapPlot_ZFix[k]) RecMapPlot_ZFix[k]->Destructor();
	}
	for(int k=0;k<37;k++){
		if(VolRendImg[k]) VolRendImg[k]->Destructor();
	}
	*/

}//close destructor


void EMTomographyRecoThreadObj::SetConfig(){

	//## Get config from parser
	cout<<"EMTomographyRecoThreadObj::SetConfig(): INFO: Get steerings from config file ..."<<endl;

	
	fVerbosity= ConfigParser::fEMLLVerbosity;
	//fInputFileName= ConfigParser::fEMLLInputFileName;	
	//fOutputFileName= ConfigParser::fEMLLOutputFileName;
	fNIterations= ConfigParser::fNIterations;
	fScatteringDensityInit= ConfigParser::fInitialScatteringDensity;
	fAverageMomentum= ConfigParser::fAverageMomentum;
	fReferenceMomentum= ConfigParser::fReferenceMomentum;
	fUseDetectorCovariance= ConfigParser::fUseDetectorCovariance;
	fEx= ConfigParser::fEx;
	fEy= ConfigParser::fEy;
	fExy= ConfigParser::fExy;		
	fUseMedianUpdate= ConfigParser::fUseMedianUpdate;
	fUseRegularization= ConfigParser::fUseRegularization;
	fBeta= ConfigParser::fBeta;

	fUseResErrStopping= ConfigParser::fUseResErrStopping;
	fResErrTolerance= ConfigParser::fResErrTolerance; 
	fNIterForResErrStopping= ConfigParser::fNIterForResErrStopping;
	fUseLogLikeStopping= ConfigParser::fUseLogLikeStopping;
	fLogLikeTolerance= ConfigParser::fLogLikeTolerance; 
	fNIterForLogLikeStopping= ConfigParser::fNIterForLogLikeStopping;
	fUseMaxRecEvents= ConfigParser::fUseEMLLMaxNEvents;	
	fNMaxRecEvents= ConfigParser::fEMLLMaxNEvents;	
	fStartEventId= 0;
	fUseScatteringAngleCut= ConfigParser::fUseEMLLThetaCut;	
	fScatteringAngleMinCut= ConfigParser::fEMLLThetaMin;
	fScatteringAngleMaxCut= ConfigParser::fEMLLThetaMax;
	fUseDisplacementCut= ConfigParser::fUseEMLLDisplacementCut;	
	fDisplacementMinCut= ConfigParser::fEMLLDisplacementMin;
	fDisplacementMaxCut= ConfigParser::fEMLLDisplacementMax;
	fUsePOCAInLL= ConfigParser::fUsePOCAInLL;
	fUseTopBottomEvents= ConfigParser::fUseTopBottomEvents;
	fUseSideEvents= ConfigParser::fUseSideEvents;
	fEMLLSignalThreshold= ConfigParser::fEMLLSignalThreshold;
	fEMLLNEventThreshold= ConfigParser::fEMLLNEventThreshold;
	fDrawMode= ConfigParser::fEMLLDrawStyleName;

	
	fUseTrueMomentum= ConfigParser::fUseEMLLTrueMomentum;
	fUseEnergyCut= ConfigParser::fUseEMLLEnergyCut;
  fEnergyMinCut= ConfigParser::fEMLLEnergyMin;
	fEnergyMaxCut= ConfigParser::fEMLLEnergyMax;

	fUseTrackSpeedCut= ConfigParser::fUseEMLLTrackSpeedCut;
  fTrackSpeedMinCut= ConfigParser::fEMLLTrackSpeedMinCut;	
	fTrackSpeedMaxCut= ConfigParser::fEMLLTrackSpeedMaxCut;
	fSmearTimeOfFlight= ConfigParser::fEMLLSmearTimeOfFlight;
	fTimeOfFlightResolution= ConfigParser::fEMLLTimeOfFlightResolution;

	fNMinEventInVoxelForUpdate= ConfigParser::fNMinEventInVoxelForUpdate;
	fUseFilterVoxelEvents= ConfigParser::fUseFilterVoxelEvents;

	fZPlane= ConfigParser::fZPlane;
	fNPlane= (int)fZPlane.size();
	fContainerSizeX= ConfigParser::fContainerSizeX;
	fContainerSizeY= ConfigParser::fContainerSizeY;
	fContainerSizeZ= ConfigParser::fContainerSizeZ;
	fContainerXStart= -fContainerSizeX/2;
	fContainerXEnd= fContainerXStart + fContainerSizeX;
	fContainerYStart= -fContainerSizeY/2;
	fContainerYEnd= fContainerYStart+ fContainerSizeY;
	fContainerZStart= -fContainerSizeZ/2;
	fContainerZEnd= fContainerZStart+ fContainerSizeZ;
	fVoxelSizeX= ConfigParser::fVoxelSizeX;
	fVoxelSizeY= ConfigParser::fVoxelSizeY;
	fVoxelSizeZ= ConfigParser::fVoxelSizeZ;
	fNVoxelX = (int)(fContainerSizeX/fVoxelSizeX); 
	fNVoxelY = (int)(fContainerSizeY/fVoxelSizeY);
	fNVoxelZ = (int)(fContainerSizeZ/fVoxelSizeZ);
	fNVoxel= fNVoxelX*fNVoxelY*fNVoxelZ;

	fSmearTrackPoints= ConfigParser::fSmearTrackPoints;
	fSigmaPosX= ConfigParser::fSigmaPosX/sqrt(12.);
	fSigmaPosY= ConfigParser::fSigmaPosY/sqrt(12.);
  fSigmaPosZ= ConfigParser::fSigmaPosZ/sqrt(12.);

	fWaitClusteringOutput= ConfigParser::fEMLLWaitClusteringOutput;
	fUseClusteringInfo= ConfigParser::fEMLLUseClusteringInfo;
	if(!fWaitClusteringOutput) fClusteringFileName= ConfigParser::fEMLLClusteringFileName;
	

	if(fVerbosity>1){
		cout<< "##################################"<<endl;
		cout<< "##    LIKELIHOOD RECO CONFIG    ##"<<endl;
		cout<< "##################################"<<endl;
		cout<<"fInputFileName= "<<fInputFileName<<endl;	
		cout<<"fNMaxRecEvents="<<fNMaxRecEvents<<endl;
		cout<<"fNIterations: "<< fNIterations<<endl;
		cout<<"fUseRegularization? "<< fUseRegularization<<endl;
		cout<<"fBeta: "<< fBeta<<endl;
		cout<<"fUseMedianUpdate? "<< fUseMedianUpdate<<endl;
		cout<<"fInitialScatteringDensity="<<fScatteringDensityInit<<endl;
		cout<<"fUseDetectorCovariance? "<< fUseDetectorCovariance<<endl;
		cout<<"fEx="<<fEx<<"  fEy="<<fEy<<"  fExy="<<fExy<<endl;
		cout<<"fUseResErrStopping? "<< fUseResErrStopping<<endl;
		cout<<"fResErrTolerance= "<< fResErrTolerance<<endl;
		cout<<"fNIterForResErrStopping= "<<fNIterForResErrStopping<<endl;
		cout<<"fUseLogLikeStopping= "<<fUseLogLikeStopping<<endl;
		cout<<"fLogLikeTolerance= "<<fLogLikeTolerance<<endl;
		cout<<"fNIterForLogLikeStopping= "<<fNIterForLogLikeStopping<<endl;
		cout<<"fReferenceMomentum[GeV]: "<< fReferenceMomentum<<endl;
		cout<<"fAverageMomentum[GeV]: "<< fAverageMomentum<<endl;
		cout<<"fNPlane="<<fNPlane<<endl;
		for(unsigned int k=0;k<fZPlane.size();k++) cout<<"PlaneZ "<<k+1<<" [cm]="<<fZPlane[k]<<endl;
		cout<<"fSigmaPos[cm]= "<<fSigmaPos<<endl;
		cout<<"fNVoxel="<<fNVoxel<<" ("<<fNVoxelX<<","<<fNVoxelY<<","<<fNVoxelZ<<")"<<endl;
		cout<<"fVoxelSize ("<<fVoxelSizeX<<","<<fVoxelSizeY<<","<<fVoxelSizeZ<<")"<<endl;
		cout<<"fContainerXStart[cm]= "<<fContainerXStart<<"  fContainerXEnd[cm]="<<fContainerXEnd<<endl;
		cout<<"fContainerYStart[cm]= "<<fContainerYStart<<"  fContainerYEnd[cm]="<<fContainerYEnd<<endl;
		cout<<"fContainerZStart[cm]= "<<fContainerZStart<<"  fContainerZEnd[cm]="<<fContainerZEnd<<endl;
		cout<<"SmearTrackPoints? "<<fSmearTrackPoints<<" Sigma("<<fSigmaPosX<<","<<fSigmaPosY<<","<<fSigmaPosZ<<")"<<endl;
		cout<<"UsePOCAInLL? "<<fUsePOCAInLL<<endl;
		cout<<"fEMLLSignalThreshold= "<<fEMLLSignalThreshold<<"  fEMLLNEventThreshold="<<fEMLLNEventThreshold<<endl;
	}

	SetGraphicsStyle();

}//close EMTomographyRecoThreadObj::SetConfig()



bool EMTomographyRecoThreadObj::Init(){

	cout<<"EMTomographyRecoThreadObj::Init(): INFO: Initialize likelihood reconstruction ..."<<endl;

	//## Init random generator
	delete gRandom;
	gRandom= new TRandom3;

	//## Set config parameters
	SetConfig();

	//## Create input file and count number of events
	fInputFileStream.open(fInputFileName.c_str());
	if(!fInputFileStream.good()) {
		QString errmsg = QString("--> ERROR: Cannot open input file ");
		errmsg+= QString::fromStdString(fInputFileName);
		errmsg+= QString::fromStdString(std::string(" ... exit!"));
		emit(statusMessage(errmsg));
		emit error();

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= errmsg.toStdString();
		fLogMessage.progress= progressPercentage;

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);

		fIsFailure= true;
		return false;
  }
	fNEvents = 0;
  
	std::string s;
  while(getline(fInputFileStream,s)) {
  	fNEvents++;
  }
   
	fInputFileStream.clear();
	fInputFileStream.seekg(ios::beg);//rewind for reading
	

	//## Build G4 detector geometry
	bool G4Status= BuildG4Detector();
	if(!G4Status){
		QString errmsg = QString("EMTomographyRecoThreadObj::Init(): ERROR: G4 geometry creation failed ... exit!");
		emit(statusMessage(errmsg));
		emit error();

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= errmsg.toStdString();
		fLogMessage.progress= progressPercentage;

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
		fIsFailure= true;
		return false;
	}

	//## Init data
	fLambda.clear();
	fLambda.resize(0);
	fNEventPerVoxel.clear();
	fNEventPerVoxel.resize(0);	
	fPOCAVoxelSignal.clear();
	fPOCAVoxelSignal.resize(0);
	fNPOCAPerVoxel.clear();
	fNPOCAPerVoxel.resize(0);
	fAlarmVoxelIdList.clear();
	fAlarmVoxelIdList.resize(0);
	for(int i=0;i<fNVoxel;i++){
		fLambda.push_back(fScatteringDensityInit);
		fPOCAVoxelSignal.push_back(0);
		fNPOCAPerVoxel.push_back(0);
		fNEventPerVoxel.push_back(0);
	}

	

	double airLambda= GetMaterialScatteringDensity("Air");
	double aluminiumLambda= GetMaterialScatteringDensity("Aluminium");
	double siliconLambda= GetMaterialScatteringDensity("Silicon");
	double copperLambda= GetMaterialScatteringDensity("Copper");
	double nickelLambda= GetMaterialScatteringDensity("Nickel");
	double zincLambda= GetMaterialScatteringDensity("Zinc");
	double ironLambda= GetMaterialScatteringDensity("Iron");
	double leadLambda= GetMaterialScatteringDensity("Lead");
	double tungstenLambda= GetMaterialScatteringDensity("Tungsten");
	double silverLambda= GetMaterialScatteringDensity("Silver");
	double platinumLambda= GetMaterialScatteringDensity("Platinum");
	double goldLambda= GetMaterialScatteringDensity("Gold");
  double concreteLambda= GetMaterialScatteringDensity("Concrete");
  double tinLambda= GetMaterialScatteringDensity("Tin");
  double uraniumLambda= GetMaterialScatteringDensity("Uranium");
  double plutoniumLambda= GetMaterialScatteringDensity("Plutonium");
	double waterLambda= GetMaterialScatteringDensity("Water");
	double pvcLambda= GetMaterialScatteringDensity("PVC");
	


  cout<<"EMTomographyRecoThreadObj::Init(): INFO: Lambda[mrad^2 cm]: Air="<<airLambda<<"  Al="<<aluminiumLambda<<"  Iron="<<ironLambda<<"  Concrete="<<concreteLambda<<"  Tin="<<tinLambda<<"  Uranium="<<uraniumLambda<<"  Plutonium="<<plutoniumLambda<<"  Water="<<waterLambda<<"  PVC="<<pvcLambda<<endl;

      

	//## Init output file and trees
	fOutputFile= new TFile(fOutputFileName.c_str(),"RECREATE");
	fOutputFile->cd();

	fRecTomographyImageInfo= new TTree("RecInfo","RecInfo");
	fRecTomographyImageInfo->Branch("VoxelId",&fVoxelId,"VoxelId/I");
	fRecTomographyImageInfo->Branch("VoxelX",&fVoxelX,"VoxelX/D");
	fRecTomographyImageInfo->Branch("VoxelY",&fVoxelY,"VoxelY/D");
	fRecTomographyImageInfo->Branch("VoxelZ",&fVoxelZ,"VoxelZ/D");
	fRecTomographyImageInfo->Branch("Nx",&fNx,"Nx/I");
	fRecTomographyImageInfo->Branch("Ny",&fNy,"Ny/I");
	fRecTomographyImageInfo->Branch("Nz",&fNz,"Nz/I");
	fRecTomographyImageInfo->Branch("Lambda",&fScatteringDensity,"Lambda/D");
	fRecTomographyImageInfo->Branch("NEvents",&fNEventInVoxel,"NEvents/I");

	
	fScatteringDataInfo= new TTree("data","data");
	fScatteringDataInfo->Branch("EventId",&fEventId,"EventId/I");
	fScatteringDataInfo->Branch("DeltaThetaX",&fDeltaThetaX,"DeltaThetaX/D");
	fScatteringDataInfo->Branch("DeltaThetaY",&fDeltaThetaY,"DeltaThetaY/D");
	fScatteringDataInfo->Branch("DeltaX",&fDeltaX,"DeltaX/D");
	fScatteringDataInfo->Branch("DeltaY",&fDeltaY,"DeltaY/D");
	fScatteringDataInfo->Branch("ScatteringAngle",&fScatteringAngle,"ScatteringAngle/D");
	fScatteringDataInfo->Branch("Pr",&fPr,"Pr/D");
	fScatteringDataInfo->Branch("POCAVoxelId",&fPOCAVoxelId,"POCAVoxelId/I");
	fScatteringDataInfo->Branch("NHitVoxels",&fNHitVoxels,"NHitVoxels/I");
	fScatteringDataInfo->Branch("VoxelId",fVoxelIdList,"VoxelId[NHitVoxels]/I");
	//fScatteringDataInfo->Branch("VoxelWx",fVoxelWx,"VoxelWx[NHitVoxels]/D");	
	//fScatteringDataInfo->Branch("VoxelWy",fVoxelWy,"VoxelWy[NHitVoxels]/D");	
	//fScatteringDataInfo->Branch("VoxelWxy",fVoxelWxy,"VoxelWxy[NHitVoxels]/D");	
	fScatteringDataInfo->Branch("POCAX",&fPOCAX,"POCAX/D");
	fScatteringDataInfo->Branch("POCAY",&fPOCAY,"POCAY/D");
	fScatteringDataInfo->Branch("POCAZ",&fPOCAZ,"POCAZ/D");
	fScatteringDataInfo->Branch("KinEnergy",&fKinEnergy,"KinEnergy/D");
		
	fInfoTree= new TTree("InfoRun","InfoRun");
	fInfoTree->Branch("NThreshold",&fEMLLNEventThreshold,"NThreshold/I");
	fInfoTree->Branch("NVoxelX",&fNVoxelX,"NVoxelX/I");
	fInfoTree->Branch("NVoxelY",&fNVoxelY,"NVoxelY/I");
	fInfoTree->Branch("NVoxelZ",&fNVoxelZ,"NVoxelZ/I");
	fInfoTree->Branch("VoxelSizeX",&fVoxelSizeX,"VoxelSizeX/D");
	fInfoTree->Branch("VoxelSizeY",&fVoxelSizeY,"VoxelSizeY/D");
	fInfoTree->Branch("VoxelSizeZ",&fVoxelSizeZ,"VoxelSizeZ/D");
	fInfoTree->Branch("XMin",&fContainerXStart,"XMin/D");
	fInfoTree->Branch("XMax",&fContainerXEnd,"XMax/D");
	fInfoTree->Branch("YMin",&fContainerYStart,"YMin/D");
	fInfoTree->Branch("YMax",&fContainerYEnd,"YMax/D");
	fInfoTree->Branch("ZMin",&fContainerZStart,"ZMin/D");
	fInfoTree->Branch("ZMax",&fContainerZEnd,"ZMax/D");
	fInfoTree->Fill();

	dummyXLevelHisto= new TH1D("dummyXLevelHisto","dummyXLevelHisto",fNVoxelX,-fContainerSizeX/2,fContainerSizeX/2);
	dummyYLevelHisto= new TH1D("dummyYLevelHisto","dummyYLevelHisto",fNVoxelY,-fContainerSizeY/2,fContainerSizeY/2);
	dummyZLevelHisto= new TH1D("dummyZLevelHisto","dummyZLevelHisto",fNVoxelZ,-fContainerSizeZ/2,fContainerSizeZ/2);

	RecMap3D= new TH3D("RecMap3D","RecMap3D",fNVoxelX,-fContainerSizeX/2,fContainerSizeX/2,fNVoxelY,-fContainerSizeY/2,fContainerSizeY/2,fNVoxelZ,-fContainerSizeZ/2,fContainerSizeZ/2);
	RecMap3D->Sumw2();

	NEventMap3D= new TH3D("NEventMap3D","NEventMap3D",fNVoxelX,-fContainerSizeX/2,fContainerSizeX/2,fNVoxelY,-fContainerSizeY/2,fContainerSizeY/2,fNVoxelZ,-fContainerSizeZ/2,fContainerSizeZ/2);
	NEventMap3D->Sumw2();

	
	for(int i=0;i<fNVoxelX;i++){
		double thisX= -fContainerSizeX/2 + i*fVoxelSizeX;
		thisX+= fVoxelSizeX/2.;

		TString histoName= Form("RecMap_XFix%d",i+1);
		TString histoTitle= Form("x[cm]=%1.1f",thisX);
		RecMap_XFix[i]= new TH2D(histoName,histoTitle,fNVoxelY,-fContainerSizeY/2,fContainerSizeY/2,fNVoxelZ,-fContainerSizeZ/2,fContainerSizeZ/2);
		RecMap_XFix[i]->Sumw2();
	
		histoName= Form("NEventMap_XFix%d",i+1);
		NEventMap_XFix[i]= new TH2D(histoName,histoTitle,fNVoxelY,-fContainerSizeY/2,fContainerSizeY/2,fNVoxelZ,-fContainerSizeZ/2,fContainerSizeZ/2);
		NEventMap_XFix[i]->Sumw2();

	}//end loop voxel X

	for(int i=0;i<fNVoxelY;i++){
		double thisY= -fContainerSizeY/2 + i*fVoxelSizeY;
		thisY+= fVoxelSizeY/2.;
		
		TString histoName= Form("RecMap_YFix%d",i+1);
		TString histoTitle= Form("y[cm]=%1.1f",thisY);
		RecMap_YFix[i]= new TH2D(histoName,histoTitle,fNVoxelX,-fContainerSizeX/2,fContainerSizeX/2,fNVoxelZ,-fContainerSizeZ/2,fContainerSizeZ/2);
		RecMap_YFix[i]->Sumw2();

		histoName= Form("NEventMap_YFix%d",i+1);
		NEventMap_YFix[i]= new TH2D(histoName,histoTitle,fNVoxelX,-fContainerSizeX/2,fContainerSizeX/2,fNVoxelZ,-fContainerSizeZ/2,fContainerSizeZ/2);
		NEventMap_YFix[i]->Sumw2();
	}//end loop voxel Y

	for(int i=0;i<fNVoxelZ;i++){
		double thisZ= -fContainerSizeZ/2 + i*fVoxelSizeZ;
		thisZ+= fVoxelSizeZ/2.;
		
		TString histoName= Form("RecMap_ZFix%d",i+1);
		TString histoTitle= Form("z[cm]=%1.1f",thisZ);

		RecMap_ZFix[i]= new TH2D(histoName,histoTitle,fNVoxelX,-fContainerSizeX/2,fContainerSizeX/2,fNVoxelY,-fContainerSizeY/2,fContainerSizeY/2);
		RecMap_ZFix[i]->Sumw2();

		histoName= Form("NEventMap_ZFix%d",i+1);
		NEventMap_ZFix[i]= new TH2D(histoName,histoTitle,fNVoxelX,-fContainerSizeX/2,fContainerSizeX/2,fNVoxelY,-fContainerSizeY/2,fContainerSizeY/2);
		NEventMap_ZFix[i]->Sumw2();
	
	}//end loop voxel Z

	fIterationInfo= new TTree("IterationInfo","IterationInfo");
	fIterationInfo->Branch("Iter",&fIterNo,"Iter/I");
	fIterationInfo->Branch("LL",&fLL,"LL/D");
	fIterationInfo->Branch("EMLL",&fEMLL,"EMLL/D");
	fIterationInfo->Branch("DeltaLL",&fDeltaLL,"DeltaLL/D");
	fIterationInfo->Branch("DeltaEMLL",&fDeltaEMLL,"DeltaEMLL/D");
	fIterationInfo->Branch("RMS",&fImageRMS,"RMS/D");
	fIterationInfo->Branch("Bias",&fImageBias,"Bias/D");
	fIterationInfo->Branch("RMSThreat",&fImageRMSThreatVoxel,"RMSThreat/D");
	fIterationInfo->Branch("BiasThreat",&fImageBiasThreatVoxel,"BiasThreat/D");
	fIterationInfo->Branch("IterRMS",&fImageIterRMS,"IterRMS/D");
	fIterationInfo->Branch("ResErr",&fImageResErr,"ResErr/D");
	fIterationInfo->Branch("SResErr",&fImageSResErr,"SResErr/D");
	fIterationInfo->Branch("RelResErr",&fImageRelResErr,"RelResErr/D");
	fIterationInfo->Branch("RelSResErr",&fImageRelSResErr,"RelSResErr/D");
	fIterationInfo->Branch("PixelMinRelResErr",&fPixelMinRelResErr,"PixelMinRelResErr/D");
	fIterationInfo->Branch("PixelMaxRelResErr",&fPixelMaxRelResErr,"PixelMaxRelResErr/D");
	fIterationInfo->Branch("PixelMean",&fPixelMean,"PixelMean/D");
	fIterationInfo->Branch("PixelVariance",&fPixelVariance,"PixelVariance/D");
	fIterationInfo->Branch("nPar",&fNPar,"nPar/I");
	fIterationInfo->Branch("Lambda",fLambdaIter,"Lambda[nPar]/D");
	
	
	fCPUTimeInfoTree= new TTree("CPUTimeInfo","CPUTimeInfo");
	fCPUTimeInfoTree->Branch("NEvents",&fNTotEvents,"NEvents/I");
	fCPUTimeInfoTree->Branch("NReadEvents",&fNReadEvents,"NReadEvents/I");
	fCPUTimeInfoTree->Branch("NSelEvents",&fNSelEvents,"NSelEvents/I");
	fCPUTimeInfoTree->Branch("NEvents_topbottom",&fNEvents_topBottomContainer,"NEvents_topbottom/I");
	fCPUTimeInfoTree->Branch("NEvents_side",&fNEvents_sideContainer,"NEvents_side/I");
	fCPUTimeInfoTree->Branch("NEvents_outside",&fNEvents_outsideContainer,"NEvents_outside/I");
	fCPUTimeInfoTree->Branch("NEvents_navigationFailure",&fNEvents_navigationFailure,"NEvents_navigationFailure/I");
	fCPUTimeInfoTree->Branch("NEvents_pocaInside",&fNEvents_pocaInsideContainer,"NEvents_pocaInside/I");
	fCPUTimeInfoTree->Branch("NEvents_pocaUnreliable",&fNEvents_pocaUnreliable,"NEvents_pocaUnreliable/I");
	fCPUTimeInfoTree->Branch("NIter",&fNIter,"NIter/I");
	fCPUTimeInfoTree->Branch("t_tot",&fCPUTime_Tot,"t_tot/D");
	fCPUTimeInfoTree->Branch("t_init",&fCPUTime_Init,"t_init/D");
	fCPUTimeInfoTree->Branch("t_collectData",&fCPUTime_DataCollect,"t_collectData/D");
	fCPUTimeInfoTree->Branch("t_imageReco",&fCPUTime_ImageReco,"t_imageReco/D");

	//## Set output filenames
	fCurrentDirectory= std::string(qPrintable(QDir::currentPath())) + std::string("/");
	fInputFileName_noPath= fInputFileName.substr( fInputFileName.find_last_of("\\/")+1 );
	fInputFileNamePath = fInputFileName.substr( 0, fInputFileName.find_last_of("\\/")+1 );

	fVisIVOOutputFileName= fCurrentDirectory + std::string("LL-VisIVO_") + fInputFileName_noPath;
	fVisIVOOutputFileName_noPath= fVisIVOOutputFileName.substr( fVisIVOOutputFileName.find_last_of("\\/")+1 );

	fVisIVOVolRendImageFilePrefix= std::string("VisIVOVolRend_") + fVisIVOOutputFileName_noPath;

	fVisIVOCommandName= fCurrentDirectory + std::string("/scripts/VisIVOVolumeImaging.sh ") + fVisIVOOutputFileName_noPath + Form(" %d %d %d ",fNVoxelX,fNVoxelY,fNVoxelZ) + fCurrentDirectory + std::string("/figure/"); 

	
	//## Clustering file 
	if(fUseClusteringInfo){
		fClusteringFile= new TFile(fClusteringFileName.c_str(),"READ");
		if(!fClusteringFile || fClusteringFile->IsZombie()){
			QString errmsg = QString("--> ERROR: Cannot open clustering input file ");
			errmsg+= QString::fromStdString(fClusteringFileName);
			errmsg+= QString::fromStdString(std::string(" ... exit!"));
			emit(statusMessage(errmsg));
			emit error();

			fLogMessage.status= eFailure;
			fLogMessage.logMessage= errmsg.toStdString();
			fLogMessage.progress= progressPercentage;

			QString logMessage= makeLogMessage(fLogMessage);
			emit logSig(logMessage);

			fIsFailure= true;
			return false;
		}
		fClusteringInfo= (TTree*)fClusteringFile->Get("ClusterInfo");
		if(!fClusteringInfo){
			QString errmsg = QString("--> ERROR: Cannot get cluster info from file ... exit!");
			emit(statusMessage(errmsg));
			emit error();
	
			fLogMessage.status= eFailure;
			fLogMessage.logMessage= errmsg.toStdString();
			fLogMessage.progress= progressPercentage;

			QString logMessage= makeLogMessage(fLogMessage);
			emit logSig(logMessage);
			fIsFailure= true;
			return false;
		}		
		fClusteringInfo->BuildIndex("EventId");		
				
	}//close if use clustering info
	
	return true;
	
}//close EMTomographyRecoThreadObj::Init()


bool EMTomographyRecoThreadObj::BuildG4Detector(){

	cout<<"EMTomographyRecoThreadObj::BuildG4Detector(): INFO: Building G4 detector ..."<<endl;
	
	G4VSteppingVerbose::SetInstance(new SteppingVerbose);
	
	//## Construct the default run manager
  if (!fRunManager) fRunManager = new G4RunManager;

  //## Set mandatory initialization classes
  G4DetectorGeometry* detector = new G4DetectorGeometry();
	fRunManager->SetUserInitialization(detector);

  G4PhysicsList* physicsList= new G4PhysicsList();
  fRunManager->SetUserInitialization(physicsList);

  G4PrimaryGenerator* generator = new G4PrimaryGenerator();
  fRunManager->SetUserAction(generator);

  //## Initialize G4 kernel
  fRunManager->Initialize();

  //## Get the pointer to the UI manager and set verbosities
  G4UImanager* UI = G4UImanager::GetUIpointer();
  UI->ApplyCommand("/run/verbose 1");
  UI->ApplyCommand("/event/verbose 1");
  UI->ApplyCommand("/tracking/verbose 1");


	//## Close geometry 
	G4GeometryManager::GetInstance()->CloseGeometry();

	//## Access to G4Navigator
	fG4Navigator= G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();

	
	if(!fG4Navigator){	
		QString errmsg = QString("--> ERROR: Cannot get G4 geometry navigator...exit!");
		emit(statusMessage(errmsg));
		emit error();

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= errmsg.toStdString();
		fLogMessage.progress= progressPercentage;

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);

		fIsFailure= true;
		return false;
	}

	cout<<"EMTomographyRecoThreadObj::BuildG4Detector(): INFO: end "<<endl;

	return true;

}//close EMTomographyRecoThreadObj::BuildG4Detector()



void EMTomographyRecoThreadObj::process(){

	//## Set the log message parameters
	//## Update only status, logmessage and progress status while running
	//## Emit start process signal
	fLogMessage.taskName= "EMML";
	fLogMessage.command= "LOG";	
	fLogMessage.inputFileName= fInputFileName;		
	fLogMessage.outputFileName= fOutputFileName;	
	fLogMessage.jobDirName= fDestinationPath;
	fLogMessage.scanId= fScanId;
	fLogMessage.containerId= fContainerId;
	fLogMessage.timeStamp= fTimeStamp;			
	fLogMessage.status= eRunning;
	fLogMessage.logMessage= "EMML imaging started...";
	fLogMessage.progress= 0;
	fLogMessage.guiKey= fGuiKey;
	fLogMessage.guiHostName= fGuiHostName;
	fLogMessage.guiPort= fGuiPort;

	fAlarmMessage.taskName= "EMML";
	fAlarmMessage.command= "ALARM";	
	fAlarmMessage.inputFileName= fInputFileName;		
	fAlarmMessage.outputFileName= fOutputFileName;	
	fAlarmMessage.jobDirName= fDestinationPath;
	fAlarmMessage.scanId= fScanId;
	fAlarmMessage.containerId= fContainerId;
	fAlarmMessage.timeStamp= fTimeStamp;			
	fAlarmMessage.status= eRunning;
	fAlarmMessage.alarmMessage= "OK";
	fAlarmMessage.guiKey= fGuiKey;
	fAlarmMessage.guiHostName= fGuiHostName;
	fAlarmMessage.guiPort= fGuiPort;


	QString logMessage= "";

	fIsFinished= false;
	fIsFailure= false;

	//while(!fIsThreadStopped || !fIsFinished || !fIsFailure){

	//## Start EM tomography reconstruction
	nRecStages= 4;//Init + InitReco + Reco + Save
	progressPercentage= 0.;
	stagePercentage= 1./(double)(nRecStages)*100.;


	cout<<"EMTomographyRecoThreadObj::process(): INFO: Running image reconstruction of data file "<<fInputFileName.c_str()<<endl;
	QString msg = QString("--> EM Image Reco: Input data file ");
	msg+= QString::fromStdString(fInputFileName);
	msg+= QString::fromStdString(std::string(" ..."));
	emit(statusMessage(msg));

		
	cout<<"EMTomographyRecoThreadObj::process(): progressPercentage= "<<progressPercentage<<endl;
	emit(statusProgress((int)(progressPercentage)));


	fLogMessage.status= eRunning;
	fLogMessage.logMessage= msg.toStdString();
	fLogMessage.progress= (int)(progressPercentage);

	logMessage= makeLogMessage(fLogMessage);
	emit logSig(logMessage);
	

	//## Start global clock	
	timespec globalTimerStart;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &globalTimerStart);
	
	//## Init time counters
	globalCPUTime.tv_sec= 0.;
	globalCPUTime.tv_nsec= 0.;	
	initCPUTime.tv_sec= 0.;
	initCPUTime.tv_nsec= 0.;
	collectDataCPUTime.tv_sec= 0.;
	collectDataCPUTime.tv_nsec= 0.;
	imageRecoCPUTime.tv_sec= 0.;
	imageRecoCPUTime.tv_nsec= 0.;

	fNReadEvents= 0;
	fNSelEvents= 0;
	fNEvents_topBottomContainer= 0;
	fNEvents_sideContainer= 0;
	fNEvents_outsideContainer= 0;
	fNEvents_navigationFailure= 0;
	fNEvents_pocaUnreliable= 0;
	fNEvents_pocaInsideContainer= 0;


	//if(fIsThreadStopped || fIsFailure || fIsFinished) break;
	

	//## Init step
	msg = QString("--> EM Image Reco: Init data ...");
	emit(statusMessage(msg));
	
	fLogMessage.status= eRunning;
	fLogMessage.logMessage= msg.toStdString();
	fLogMessage.progress= 0;
	logMessage= makeLogMessage(fLogMessage);
	emit logSig(logMessage);

	timespec initTimerStart;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &initTimerStart);	
	bool init_status= Init();
	timespec initTimerEnd;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &initTimerEnd);
	initCPUTime= Utilities::TimeDiff(initTimerStart,initTimerEnd);
	
	if(!init_status){
		msg = QString("--> EM Image Reco: Init data failed!");
		emit(statusMessage(msg));

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= 0;

		logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
		return;
	}
	else{
		msg = QString("--> EM Image Reco: Init data ... done!");
		emit(statusMessage(msg));

		progressPercentage+= stagePercentage;
		cout<<"EMTomographyRecoThreadObj::process(): stagePercentage="<<stagePercentage<<"  progressPercentage="<<progressPercentage<<endl;
		emit(statusProgress((int)(progressPercentage)));

		fLogMessage.status= eRunning;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= (int)(progressPercentage);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}


	//if(fIsThreadStopped || fIsFailure || fIsFinished) break;
	
	//## Start event loop
	cout<<"EMTomographyRecoThreadObj::process(): INFO: Reading "<<fNEvents<<" muon events ..."<<endl;
	timespec collectDataTimerStart;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &collectDataTimerStart);	

	int EventCounter= 0;
	double thisProgressPercentage= 0;

	for(int i=0;i<fNEvents;i++){

		//if(fIsThreadStopped || fIsFailure || fIsFinished) break;
	
		//if(fVerbosity>1) cout<<"EMTomographyRecoThreadObj::Run(): INFO: Event No. "<<i<<endl;
		fEventId= i;

		//## Break event loop?
		if(fUseMaxRecEvents && EventCounter>=fNMaxRecEvents) break;

		//## Skip current event id
		if(fEventId<fStartEventId) continue;
		EventCounter++;

		//## Read event data
		bool read_status= ReadData();
		if(!read_status){
			msg = QString("--> EM Image Reco: Read data failed!");
			emit(statusMessage(msg));

			fLogMessage.status= eFailure;
			fLogMessage.logMessage= msg.toStdString();
			fLogMessage.progress= 0;

			logMessage= makeLogMessage(fLogMessage);
			emit logSig(logMessage);
			return;
		}

		//## Set scattering data
		SetScatteringData();		
		
		fNReadEvents++;
		
		//## Is good event?
		if(!IsGoodEvent()){
			//if(fVerbosity>1) cout<<"EMTomographyRecoThreadObj::Run(): INFO: Bad event ...skip!"<<endl;
			continue;
		}

		fNSelEvents++;

		//## Find path in voxels
		VoxelFinder();

		if(i%100000==0){
			thisProgressPercentage= (double)(fNReadEvents)/(double)(fNEvents)*stagePercentage;
			double thisProgressStatus= progressPercentage+thisProgressPercentage;

			emit(statusProgress((int)(thisProgressStatus)));
			
			msg = QString("--> EM Image Reco: Init EM [Event No. %1/%2] ...").arg(i).arg(fNEvents);
			emit(statusMessage(msg));

			fLogMessage.status= eRunning;
			fLogMessage.logMessage= msg.toStdString();
			fLogMessage.progress= (int)(thisProgressStatus);

			logMessage= makeLogMessage(fLogMessage);
			emit logSig(logMessage);
		}

	}//end loop events
	
	fInputFileStream.close();

	//if(fIsThreadStopped || fIsFailure || fIsFinished) break;
	

	progressPercentage+= thisProgressPercentage;

	cout<<"EMTomographyRecoThreadObj::process(): INFO: "<<fNReadEvents<<" events processed..."<<endl;
	cout<<"EMTomographyRecoThreadObj::process(): INFO: "<<fNSelEvents<<" events selected for the likelihood iteration..."<<endl;
	
	msg = QString("--> EM Image Reco: Init EM ... done!");
	emit(statusMessage(msg));

	fLogMessage.status= eRunning;
	fLogMessage.logMessage= msg.toStdString();
	fLogMessage.progress= progressPercentage;

	logMessage= makeLogMessage(fLogMessage);
	emit logSig(logMessage);

	timespec collectDataTimerEnd;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &collectDataTimerEnd);
	collectDataCPUTime= Utilities::TimeDiff(collectDataTimerStart,collectDataTimerEnd);

	//## Run the image reconstruction
	msg = QString("--> EM Image Reco: Run EM ...");
	emit(statusMessage(msg));

	fLogMessage.status= eRunning;
	fLogMessage.logMessage= msg.toStdString();
	fLogMessage.progress= progressPercentage;

	logMessage= makeLogMessage(fLogMessage);
	emit logSig(logMessage);


	timespec imageRecoTimerStart;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &imageRecoTimerStart);	
	ImageReco();	
	timespec imageRecoTimerEnd;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &imageRecoTimerEnd);
	imageRecoCPUTime= Utilities::TimeDiff(imageRecoTimerStart,imageRecoTimerEnd);

	msg = QString("--> EM Image Reco: Run EM ... done!");
	emit(statusMessage(msg));

	fLogMessage.status= eRunning;
	fLogMessage.logMessage= msg.toStdString();
	fLogMessage.progress= progressPercentage;

	logMessage= makeLogMessage(fLogMessage);
	emit logSig(logMessage);

	//if(fIsThreadStopped || fIsFailure || fIsFinished) break;
	

	//## Draw tomographic maps
	msg = QString("--> EM Image Reco: Drawing maps ...");
	emit(statusMessage(msg));
	fLogMessage.status= eRunning;
	fLogMessage.logMessage= msg.toStdString();
	fLogMessage.progress= progressPercentage;

	logMessage= makeLogMessage(fLogMessage);
	emit logSig(logMessage);

	Draw();
	msg = QString("--> EM Image Reco: Drawing maps ... done!");
	emit(statusMessage(msg));

	fLogMessage.status= eRunning;
	fLogMessage.logMessage= msg.toStdString();
	fLogMessage.progress= progressPercentage;

	logMessage= makeLogMessage(fLogMessage);
	emit logSig(logMessage);

	//## Stop global clock
	timespec globalTimerEnd;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &globalTimerEnd);
	globalCPUTime = Utilities::TimeDiff(globalTimerStart,globalTimerEnd);

	//if(fIsThreadStopped || fIsFailure || fIsFinished) break;
	
	
	//## Dump CPU time
	DumpTotalLoad();

	//if(fIsThreadStopped || fIsFailure || fIsFinished) break;
	

	//## Save results to file
	msg = QString("--> EM Image Reco: Save to file ...");
	emit(statusMessage(msg));

	fLogMessage.status= eRunning;
	fLogMessage.logMessage= msg.toStdString();
	fLogMessage.progress= progressPercentage;

	logMessage= makeLogMessage(fLogMessage);
	emit logSig(logMessage);

	Save();
	
	progressPercentage+= stagePercentage;
	//emit(statusProgress(max(100,(int)(progressPercentage)) ));
	emit(statusProgress(max(100.,progressPercentage) ));

	msg = QString("--> EM Image Reco: Save to file ... done!");
	emit(statusMessage(msg));

	fLogMessage.status= eRunning;
	fLogMessage.logMessage= msg.toStdString();
	fLogMessage.progress= progressPercentage;

	logMessage= makeLogMessage(fLogMessage);
	emit logSig(logMessage);

	//if(fIsThreadStopped || fIsFailure || fIsFinished) break;
	
	//emit finished(); 
	fIsFinished= true;

	//}//close while fIsThreadStopped

	if(fIsThreadStopped){
		QString msg = QString("--> EM Image Reco: Stopped by the user!");
		emit(statusMessage(msg));	
		emit error();	

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= progressPercentage;

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}
	if(fIsFinished){
		emit finished(); 

		QString msg = QString("--> EM Image Reco: Terminated with success!");
		emit(statusMessage(msg));	

		fLogMessage.status= eFinished;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= progressPercentage;

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}
	
}//close EMTomographyRecoThreadObj::process()





void EMTomographyRecoThreadObj::SetScatteringData(){

	//## Smear track points with resolution?
	const int nplanes= 4;
	double xHit[nplanes];
	double yHit[nplanes];
	double zHit[nplanes];	
	TVector3 HitPointList[nplanes];

	double muonMass= 0.10565836668;
	double trueKinEnergy= fRawData.p;
	double trueMomentum= sqrt(trueKinEnergy*(2*muonMass+trueKinEnergy));
	int eventId= fRawData.EventId;
	
	//double tof= fRawData.tof;
	//if(fSmearTimeOfFlight){
	//	tof+= gRandom->Gaus(0,fTimeOfFlightResolution);
	//}

	for(int i=0;i<nplanes;i++){
		xHit[i]= fRawData.HitX[i];
		yHit[i]= fRawData.HitY[i];
		zHit[i]= fZPlane[i];
		HitPointList[i]= TVector3(xHit[i],yHit[i],zHit[i]);
	}
	

	if(fSmearTrackPoints){
		for(int i=0;i<nplanes;i++){

			bool isGoodSmearing= false;
			double randx= 0;
			double randy= 0;
			double randz= 0;

			while(!isGoodSmearing){
				randx= gRandom->Gaus(0,fSigmaPosX);
				randy= gRandom->Gaus(0,fSigmaPosY);
				randz= gRandom->Gaus(0,fSigmaPosZ);
	
				if(fabs(randx)<ConfigParser::fSigmaPosX && fabs(randy)<ConfigParser::fSigmaPosY && fabs(randz)<ConfigParser::fSigmaPosZ ) isGoodSmearing= true;
				//else {
				//	cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: Bad smearing (rand xyz="<<randx<<","<<randy<<","<<randz<<") ...re-generate random numbers!"<<endl;
				//}
			}
	
			xHit[i]+= randx; 
			yHit[i]+= randy;
			zHit[i]+= randz;
			HitPointList[i]= TVector3(xHit[i],yHit[i],zHit[i]);
			
		}//end loop planes		
	}//close if smear track points


	//## Set entry point and direction
	TVector3 P0_in( xHit[0], yHit[0], zHit[0]); 
  TVector3 P1_in( xHit[1], yHit[1], zHit[1]);
	
	TVector3 u_in = P1_in-P0_in;
	double Tx_in= u_in.X()/u_in.Z();
 	double Ty_in= u_in.Y()/u_in.Z();
  TVector3 entryDir = u_in.Unit();	
	TVector3 entryPoint= TVector3(P0_in.X()+Tx_in*(fContainerZStart-P0_in.Z()), P0_in.Y()+Ty_in*(fContainerZStart-P0_in.Z()), fContainerZStart);

	TVector3 entrydebugPoint= TVector3(P0_in.X()+Tx_in*(600.-P0_in.Z()), P0_in.Y()+Ty_in*(600.-P0_in.Z()), 600.);
	
	//## Set exit point and direction
	TVector3 P0_out( xHit[2], yHit[2], zHit[2]); 
  TVector3 P1_out( xHit[3], yHit[3], zHit[3]);
	//TVector3 P1_out( rawData.HitX[3], rawData.HitY[3], 600.);		
	TVector3 u_out = P1_out-P0_out; 
	double Tx_out= u_out.X()/u_out.Z();
 	double Ty_out= u_out.Y()/u_out.Z();
  TVector3 exitDir = u_out.Unit();
	TVector3 exitPoint= TVector3(P0_out.X()+Tx_out*(fContainerZEnd-P0_out.Z()), P0_out.Y()+Ty_out*(fContainerZEnd-P0_out.Z()), fContainerZEnd);
	
	TVector3 exitdebugPoint= TVector3(P0_out.X()+Tx_out*(600.-P0_out.Z()), P0_out.Y()+Ty_out*(600.-P0_out.Z()), 600.);
	


	//## Check if the event has traversed the container (at least passing from the side)
	bool hasEnteredTopContainer= (entryPoint.X()>=fContainerXStart && entryPoint.X()<=fContainerXEnd && entryPoint.Y()>=fContainerYStart && entryPoint.Y()<=fContainerYEnd);
	bool hasEnteredBottomContainer= (exitPoint.X()>=fContainerXStart && exitPoint.X()<=fContainerXEnd && exitPoint.Y()>=fContainerYStart && exitPoint.Y()<=fContainerYEnd);
	
	bool hasTraversedTopBottomContainer= (hasEnteredTopContainer && hasEnteredBottomContainer);
	bool hasTraversedSideContainer= ( (hasEnteredTopContainer || hasEnteredBottomContainer) && !hasTraversedTopBottomContainer );
	bool hasTraversedContainer= (hasTraversedTopBottomContainer || hasTraversedSideContainer);
 
	//bool hasTraversedContainer= hasTraversedFullContainer;
	bool hasNavigationFailure= false;

	if(fVerbosity>2){
		cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: entryPoint("<<entryPoint.X()<<","<<entryPoint.Y()<<","<<entryPoint.Z()<<")  hasEnteredTopContainer? "<<hasEnteredTopContainer<<endl;
		cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: exitPoint("<<exitPoint.X()<<","<<exitPoint.Y()<<","<<exitPoint.Z()<<") hasEnteredBottomContainer? "<<hasEnteredBottomContainer<<endl;
		cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: hasTraversedSideContainer? "<<hasTraversedSideContainer<<"  hasTraversedTopBottomContainer? "<<hasTraversedTopBottomContainer<<"  hasTraversedContainer? "<<hasTraversedContainer<<endl;
	}

	
	//## Detect entry and exit point in the container if the event has passed from the side
	//## Need to use G4Navigator for cases in which the track enters the container side
	G4ThreeVector currentPoint;
	G4ThreeVector currentDirection;
	G4ThreeVector currentStopPoint;
	double startPosZ;
	double stopPosZ;
	double currentPosZ;	
	G4ThreeVector n(0,0,1);
	double safetyStep = 0.0;	
	double tolerance= 1.e-9;
	double epsStep= 0;
	G4VPhysicalVolume* currentVolume= 0;
	G4VPhysicalVolume* newVolume= 0;

	if(hasTraversedSideContainer){

		//## Find entry side point
		if(!hasEnteredTopContainer){
			currentPoint= G4ThreeVector(P1_in.X()*CLHEP::cm,P1_in.Y()*CLHEP::cm,P1_in.Z()*CLHEP::cm);
			currentDirection= G4ThreeVector(entryDir.X(),entryDir.Y(),entryDir.Z());
			currentStopPoint= G4ThreeVector(exitPoint.X()*CLHEP::cm,exitPoint.Y()*CLHEP::cm,exitPoint.Z()*CLHEP::cm);
			startPosZ= P1_in.Z();
			stopPosZ= exitPoint.Z();
			currentPosZ= startPosZ;
	
			currentVolume= 0;
			newVolume= 0;
	
			//## Do a loop until the container boundary is reached
			bool isEntryFound= false;

			//cout<<"currentPoint("<<currentPoint.x()/cm<<","<<currentPoint.y()/cm<<","<<currentPoint.z()/cm<<endl;
			//cout<<"currentDirection("<<currentDirection.x()<<","<<currentDirection.y()<<","<<currentDirection.z()<<endl;
			//cout<<"currentStopPoint("<<currentStopPoint.x()/cm<<","<<currentStopPoint.y()/cm<<","<<currentStopPoint.z()/cm<<endl;
	
		
			while( fabs(currentPosZ-stopPosZ)>tolerance ){
	
				currentVolume= fG4Navigator->LocateGlobalPointAndSetup(currentPoint,&currentDirection);
			
				//## Calculate max step point given the current position and the position of a plane z= StopPosZ    
				double stepNum= (currentStopPoint-currentPoint).dot(n);
				double stepDenom= currentDirection.dot(n);
				double currentMaxStep= (fabs(stepNum/stepDenom) + epsStep);
		
				//## Move to the next boundary
				double stepSize= fG4Navigator->ComputeStep( currentPoint, currentDirection, currentMaxStep, safetyStep );
				if(stepSize>currentMaxStep) stepSize= currentMaxStep;
			
				//## Find the location of the next boundary
    		G4ThreeVector newPoint= currentPoint + stepSize * currentDirection;

				//## Check if navigator actually managed to move
    		if( (newPoint - currentPoint).mag2() > 0) {

					//## Get new volume info
					newVolume= fG4Navigator->LocateGlobalPointAndSetup(newPoint,&currentDirection);
					if(!newVolume){
						cerr<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: WARNING: Navigator outside the world...exit loop!"<<endl;
						break;
					}		
			
					//## Update current point and material properties
					currentPoint= newPoint;
					currentPosZ= newPoint.z()/CLHEP::cm;
			
					//cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: currentVolume: "<<currentVolume->GetName()<<"  newVolume: "<<newVolume->GetName()<<"  currentPosZ= "<<currentPosZ<<endl;
			
					if(currentVolume->GetName()=="ExpHall_phys" && newVolume->GetName()=="VoxelEnsemble_phys"){
						isEntryFound= true;
						//cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: Found entry point in the container...exit loop!"<<endl;
						//cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: entry point ("<<newPoint.x()/cm<<","<<newPoint.y()/cm<<","<<newPoint.z()/cm<<")"<<endl;
			
						//Overwrite previous entering point
						entryPoint= TVector3(newPoint.x()/CLHEP::cm,newPoint.y()/CLHEP::cm,newPoint.z()/CLHEP::cm);

						break;
					}
	
				}//close if 

			}//end loop reached z depth 

  		//cout<<"end loop entry point"<<endl;

			if(!isEntryFound){
				//cerr<<"EMTomographyRecoThreadObj::SetScatteringData(): ERROR: Cannot find entry point in the container side...exit!"<<endl;
				//hasTraversedContainer= false;
				hasNavigationFailure= true;
				//exit(1);
			}

		}//close if !hasEnteredTopContainer

		
		//## Find exit side point
		if(!hasEnteredBottomContainer){
			currentPoint= G4ThreeVector(P0_out.X()*CLHEP::cm,P0_out.Y()*CLHEP::cm,P0_out.Z()*CLHEP::cm);
			currentDirection= G4ThreeVector(-exitDir.X(),-exitDir.Y(),-exitDir.Z());
			currentStopPoint= G4ThreeVector(entryPoint.X()*CLHEP::cm,entryPoint.Y()*CLHEP::cm,entryPoint.Z()*CLHEP::cm);
			startPosZ= P0_out.Z();
			stopPosZ= entryPoint.Z();
			currentPosZ= startPosZ;

			currentVolume= 0;
			newVolume= 0;
	
			//## Do a loop until the container boundary is reached
			bool isExitFound= false;
		
			while( fabs(currentPosZ-stopPosZ)>tolerance ){
	
				currentVolume= fG4Navigator->LocateGlobalPointAndSetup(currentPoint,&currentDirection);
			
				//## Calculate max step point given the current position and the position of a plane z= StopPosZ    
				double stepNum= (currentStopPoint-currentPoint).dot(n);
				double stepDenom= currentDirection.dot(n);
				double currentMaxStep= (fabs(stepNum/stepDenom) + epsStep);
		
				//## Move to the next boundary
				double stepSize= fG4Navigator->ComputeStep( currentPoint, currentDirection, currentMaxStep, safetyStep );
				if(stepSize>currentMaxStep) stepSize= currentMaxStep;
			
				//## Find the location of the next boundary
    		G4ThreeVector newPoint= currentPoint + stepSize * currentDirection;

				//## Check if navigator actually managed to move
    		if( (newPoint - currentPoint).mag2() > 0) {

					//## Get new volume info
					newVolume= fG4Navigator->LocateGlobalPointAndSetup(newPoint,&currentDirection);
					if(!newVolume){
						cerr<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: WARNING: Navigator outside the world...exit loop!"<<endl;
						break;
					}		
			
					//## Update current point and material properties
					currentPoint= newPoint;
					currentPosZ= newPoint.z()/CLHEP::cm;
			
					//cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: currentVolume: "<<currentVolume->GetName()<<"  newVolume: "<<newVolume->GetName()<<"  currentPosZ= "<<currentPosZ<<endl;
			
					if(currentVolume->GetName()=="ExpHall_phys" && newVolume->GetName()=="VoxelEnsemble_phys"){
						isExitFound= true;
						//cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: Found exit point in the container...exit loop!"<<endl;
						//cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: exit point ("<<newPoint.x()/cm<<","<<newPoint.y()/cm<<","<<newPoint.z()/cm<<")"<<endl;

						//Overwrite previous exit point
						exitPoint= TVector3(newPoint.x()/CLHEP::cm,newPoint.y()/CLHEP::cm,newPoint.z()/CLHEP::cm);

						break;
					}
	
				}//close if 

			}//end loop reached z depth 

  		//cout<<"end loop exit point"<<endl;

			if(!isExitFound){
				//cerr<<"EMTomographyRecoThreadObj::SetScatteringData(): ERROR: Cannot find exit point in the container side...exit!"<<endl;
				//hasTraversedContainer= false;
				hasNavigationFailure= true;
				//exit(1);
			}

		}//close if !hasEnteredBottomContainer
	
	}//close if hasTraversedSideContainer

	
	double OuterPlaneZSeparation= fZPlane[1]-fZPlane[0];	
	double InnerPlaneZSeparation= fZPlane[2]-fZPlane[1];
	//double H= fContainerZEnd-fContainerZStart;
	double H= exitPoint.Z()-entryPoint.Z();	
	double Lxy= sqrt(1+Tx_in*Tx_in+Ty_in*Ty_in);
	double L= H*Lxy; 	
	TVector3 P= entryPoint + L*entryDir;
	
	double ScatteringAngle= entryDir.Angle(exitDir);//in rad
	double ThetaX0= TMath::ATan2(entryDir.X(),entryDir.Z());//in rad
	double ThetaY0= TMath::ATan2(entryDir.Y(),entryDir.Z());//in rad
	double ThetaX= TMath::ATan2(exitDir.X(),exitDir.Z());//in rad
	double ThetaY= TMath::ATan2(exitDir.Y(),exitDir.Z());//in rad
	double DeltaThetaX= ThetaX-ThetaX0;
	double DeltaThetaY= ThetaY-ThetaY0;

	double DeltaX= (exitPoint.X()-P.X())*cos(ThetaX0)*Lxy*cos(ThetaX0+DeltaThetaX)/cos(DeltaThetaX);
	double DeltaY= (exitPoint.Y()-P.Y())*cos(ThetaY0)*Lxy*cos(ThetaY0+DeltaThetaY)/cos(DeltaThetaY);

	double pr= fReferenceMomentum/fAverageMomentum; 
	if(fUseTrueMomentum){
		pr= fReferenceMomentum/trueMomentum; 
	}
	
	//if(fVerbosity>2){
		//cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: Etrue[GeV]="<<trueKinEnergy<<"  Ptrue[GeV]="<<trueMomentum<<"  P0[GeV]="<<fReferenceMomentum<<"  <P>[GeV]="<<fAverageMomentum<<"  pr="<<pr<<endl;
	//}
	
	double Ex= 4.*pow(fSigmaPosX*sqrt(12)/OuterPlaneZSeparation,2);//rad^2
	double Ey= 2.*(1+InnerPlaneZSeparation/OuterPlaneZSeparation+pow(InnerPlaneZSeparation/OuterPlaneZSeparation,2))*pow(fSigmaPosY*sqrt(12),2);//cm^2
	double Exy= 2.*pow(fSigmaPosZ*sqrt(12)/OuterPlaneZSeparation,2)*InnerPlaneZSeparation;//rad*cm
	if(fVerbosity>1){
		cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO:: Ex="<<Ex<<"  Ey="<<Ey<<"  Exy="<<Exy<<endl;
	}

	//## Calculate POCA : point of closest approach of the entry-exit lines 
	//## t_in= (be-cd)/(ac-b^2)
	//## t_out= (ae-bd)/(ac-b^2) 
	//## w= P0_in-P0_out 
	//## a= u_in (dot) u_in
	//## b= u_in (dot) u_out	
	//## c= u_out (dot) u_out
	//## d= u_in (dot) w
	//## e= u_out (dot) w
	TVector3 w= P0_in-P0_out;
	double a= u_in.Dot(u_in);
	double b= u_in.Dot(u_out);
	double c= u_out.Dot(u_out);
	double d= u_in.Dot(w);
	double e= u_out.Dot(w);
	/*
	double a= entryDir.Dot(entryDir);
	double b= entryDir.Dot(exitDir);
	double c= exitDir.Dot(exitDir);
	double d= entryDir.Dot(w);
	double e= exitDir.Dot(w);
	*/
	double delta= a*c-b*b;
	double t_in;
	double t_out;

	
	if(delta<=1.e-8){//parallel lines
		t_in= 0.;
		t_out = (b>c ? d/b : e/c);
		if(fVerbosity>1) {
			cerr<<"EMTomographyRecoThreadObj::SetScatteringData(): WARNING: delta smaller than tolerance (1.e-08), POCA calculation may be affected!"<<endl;
		}
	}
	else{//skew lines
		t_in= (b*e-c*d)/delta;
		t_out= (a*e-b*d)/delta;
	}


	TVector3 POCA_in= P0_in + t_in*u_in;
	TVector3 POCA_out= P0_out + t_out*u_out;
	//TVector3 POCA_in= P0_in + t_in*entryDir;
	//TVector3 POCA_out= P0_out + t_out*exitDir;
	TVector3 POCA= 0.5*(POCA_in+POCA_out);
	
	bool isPOCAInsideContainer= false;
	if(POCA.X()>fContainerXStart && POCA.X()<fContainerXEnd &&
     POCA.Y()>fContainerYStart && POCA.Y()<fContainerYEnd &&
     POCA.Z()>fContainerZStart && POCA.Z()<fContainerZEnd
  ){
		isPOCAInsideContainer= true;
	}

	bool isReliablePOCA= false;
	if(POCA.Z()>entryPoint.Z() && POCA.Z()<exitPoint.Z() ) {
		isReliablePOCA= true;
	}

	if(fUseClusteringInfo){
		int eventFoundFlag= fClusteringInfo->GetEntryWithIndex(eventId);
		if(eventFoundFlag==-1) isReliablePOCA= false;
	}

	//## Calculate track length and speed
	//double trackLength= 0;
	//trackLength+= (HitPointList[1]-HitPointList[0]).Mag();//distance hit 1-2
	//trackLength+= (entryPoint-HitPointList[1]).Mag();//distance hit 2-entry
	//if(isPOCAInsideContainer && isReliablePOCA) {
	//	trackLength+= (POCA-entryPoint).Mag();//distance poca-entry
	//	trackLength+= (exitPoint-POCA).Mag();//distance exit-poca
	//}
	//else trackLength+= (exitPoint-entryPoint).Mag();//distance exit-entry

	//trackLength+= (HitPointList[2]-exitPoint).Mag();//distance hit 3-exit
	//trackLength+= (HitPointList[3]-HitPointList[2]).Mag();//distance hit 4-3
	
	//double trackSpeed= trackLength/tof;//in cm/ns
	//double trackSpeed_scaled= trackSpeed/(TMath::C()*1.e-7);//scale by the speed of light
	//#########################

	
	if(fVerbosity>2){
		cout<<"*** IN TRACK ***"<<endl;
		cout<<"P0_in ("<<P0_in.X()<<","<<P0_in.Y()<<","<<P0_in.Z()<<")"<<endl;
		cout<<"P1_in ("<<P1_in.X()<<","<<P1_in.Y()<<","<<P1_in.Z()<<")"<<endl;
		cout<<"P1_in debug ("<<entrydebugPoint.X()<<","<<entrydebugPoint.Y()<<","<<entrydebugPoint.Z()<<")"<<endl;
		cout<<"dir ("<<u_in.X()<<","<<u_in.Y()<<","<<u_in.Z()<<")"<<endl;
		cout<<"dir ("<<entryDir.X()<<","<<entryDir.Y()<<","<<entryDir.Z()<<")"<<endl;
		cout<<"*** OUT TRACK ***"<<endl;
		cout<<"P0_out ("<<P0_out.X()<<","<<P0_out.Y()<<","<<P0_out.Z()<<")"<<endl;
		cout<<"P1_out ("<<P1_out.X()<<","<<P1_out.Y()<<","<<P1_out.Z()<<")"<<endl;
		cout<<"P1_out debug ("<<exitdebugPoint.X()<<","<<exitdebugPoint.Y()<<","<<exitdebugPoint.Z()<<")"<<endl;
		cout<<"dir ("<<u_out.X()<<","<<u_out.Y()<<","<<u_out.Z()<<")"<<endl;
		cout<<"dir ("<<exitDir.X()<<","<<exitDir.Y()<<","<<exitDir.Z()<<")"<<endl;
		cout<<"delta="<<delta<<"  t_in="<<t_in<<"  t_out="<<t_out<<endl;
		cout<<"w ("<<w.X()<<","<<w.Y()<<","<<w.Z()<<")  a="<<a<<"  b="<<b<<"  c="<<c<<"  d="<<d<<"  e="<<e<<endl;
		cout<<"POCA_in ("<<POCA_in.X()<<","<<POCA_in.Y()<<","<<POCA_in.Z()<<")"<<endl;
		cout<<"POCA_out ("<<POCA_out.X()<<","<<POCA_out.Y()<<","<<POCA_out.Z()<<")"<<endl;
		cout<<"--> POCA ("<<POCA.X()<<","<<POCA.Y()<<","<<POCA.Z()<<")"<<endl;
	}

	double poca[3]= {POCA.X(),POCA.Y(),POCA.Z()}; 

	double EntryPointInContainer[3]= {entryPoint.X(),entryPoint.Y(),entryPoint.Z()};
	double ExitPointInContainer[3]= {exitPoint.X(),exitPoint.Y(),exitPoint.Z()};
	double MiddlePointInContainer[3]= {exitPoint.X(),exitPoint.Y(),exitPoint.Z()};

	TVector3 entryexitDir= (exitPoint-entryPoint).Unit();
	TVector3 entrypocaDir= (POCA-entryPoint).Unit();
	TVector3 pocaexitDir= (exitPoint-POCA).Unit();
	
	double EntryDirectionInContainer[3]= {entryexitDir.X(),entryexitDir.Y(),entryexitDir.Z()};
	double ExitDirectionInContainer[3]= {exitDir.X(),exitDir.Y(),exitDir.Z()};
	double MiddleDirectionInContainer[3]= {entryexitDir.X(),entryexitDir.Y(),entryexitDir.Z()};	

	if(isPOCAInsideContainer && isReliablePOCA && fUsePOCAInLL){
		MiddlePointInContainer[0]= POCA.X();
		MiddlePointInContainer[1]= POCA.Y();
		MiddlePointInContainer[2]= POCA.Z();

		EntryDirectionInContainer[0]= entrypocaDir.X();
		EntryDirectionInContainer[1]= entrypocaDir.Y();
		EntryDirectionInContainer[2]= entrypocaDir.Z();
		
		MiddleDirectionInContainer[0]= pocaexitDir.X();
		MiddleDirectionInContainer[1]= pocaexitDir.Y();
		MiddleDirectionInContainer[2]= pocaexitDir.Z();
	}
	


	//## Set scattering data	
	fScatteringData.HasNavigationFailure= hasNavigationFailure;	
	fScatteringData.HasTraversedContainer= hasTraversedContainer;	
	fScatteringData.HasTraversedContainerFromTop= hasTraversedTopBottomContainer;	
	fScatteringData.HasTraversedContainerFromSide= hasTraversedSideContainer;
	fScatteringData.IsPOCAInsideContainer= isPOCAInsideContainer;		
	fScatteringData.IsPOCAReliable= isReliablePOCA;
	fScatteringData.DeltaThetaX= DeltaThetaX;
	fScatteringData.DeltaThetaY= DeltaThetaY;
	fScatteringData.DeltaX= DeltaX;
	fScatteringData.DeltaY= DeltaY;
	fScatteringData.pr= pr; 
	fScatteringData.Ex= Ex;
	fScatteringData.Ey= Ey;
	fScatteringData.Exy= Exy;
	fScatteringData.KinEnergy= trueKinEnergy;
	fScatteringData.ScatteringAngle= ScatteringAngle;
	fScatteringData.EventId= fEventId;
	//fScatteringData.TOF= tof;
	//fScatteringData.TrackLength= trackLength;	
	//fScatteringData.TrackSpeed= trackSpeed_scaled;

	for(int i=0;i<3;i++){
		fScatteringData.POCA[i]= poca[i];
		fScatteringData.EntryPoint[i]= EntryPointInContainer[i];
		fScatteringData.EntryDirection[i]= EntryDirectionInContainer[i];
		fScatteringData.ExitPoint[i]= ExitPointInContainer[i];
		fScatteringData.ExitDirection[i]= ExitDirectionInContainer[i];
		fScatteringData.MiddlePoint[i]= MiddlePointInContainer[i];
		fScatteringData.MiddleDirection[i]= MiddleDirectionInContainer[i];
	}
	
	
	if(hasTraversedTopBottomContainer) fNEvents_topBottomContainer++;
	if(hasTraversedSideContainer) fNEvents_sideContainer++;
	if(!hasTraversedContainer) fNEvents_outsideContainer++;
	if(hasNavigationFailure) fNEvents_navigationFailure++;
	if(!isReliablePOCA) fNEvents_pocaUnreliable++;
	if(isPOCAInsideContainer) fNEvents_pocaInsideContainer++;	

	if(fVerbosity>2){
		cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: IsPocaInsideContainer? "<<isPOCAInsideContainer<<"  POCA=("<<poca[0]<<","<<poca[1]<<","<<poca[2]<<")"<<endl;	
		cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: EntryPoint ("<<EntryPointInContainer[0]<<","<<EntryPointInContainer[1]<<","<<EntryPointInContainer[2]<<")"<<endl;	
		cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: MiddlePoint ("<<MiddlePointInContainer[0]<<","<<MiddlePointInContainer[1]<<","<<MiddlePointInContainer[2]<<")"<<endl;	
		cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: ExitPoint ("<<ExitPointInContainer[0]<<","<<ExitPointInContainer[1]<<","<<ExitPointInContainer[2]<<")"<<endl;	
		cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: EntryDir ("<<EntryDirectionInContainer[0]<<","<<EntryDirectionInContainer[1]<<","<<EntryDirectionInContainer[2]<<")"<<endl;
		cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: MiddleDir ("<<MiddleDirectionInContainer[0]<<","<<MiddleDirectionInContainer[1]<<","<<MiddleDirectionInContainer[2]<<")"<<endl;
		cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: ScatteringAngle[deg]= "<<ScatteringAngle*TMath::RadToDeg()<<"  DeltaThetaX[deg]="<<DeltaThetaX*TMath::RadToDeg()<<"  DeltaThetaY[deg]="<<DeltaThetaY*TMath::RadToDeg()<<endl;
		cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO: DeltaX[cm]= "<<DeltaX<<"  DeltaY[cm]="<<DeltaY<<endl;
	}
	
	
}//close EMTomographyRecoThreadObj::SetScatteringData()






bool EMTomographyRecoThreadObj::IsGoodEvent(){

	//## Apply quality cuts
	bool isGoodEvent= true;

	double xEntry= fScatteringData.EntryPoint[0];
	double yEntry= fScatteringData.EntryPoint[1];
	double zEntry= fScatteringData.EntryPoint[2];
	double xExit= fScatteringData.ExitPoint[0];
	double yExit= fScatteringData.ExitPoint[1];
	double zExit= fScatteringData.ExitPoint[2];
	double ScatteringAngle= fScatteringData.ScatteringAngle* TMath::RadToDeg();
	double DeltaX= fScatteringData.DeltaX;
	double DeltaY= fScatteringData.DeltaY;
	double KinEnergy= fScatteringData.KinEnergy;
	bool HasTraversedContainer= fScatteringData.HasTraversedContainer;
  bool HasTraversedContainerFromSide= fScatteringData.HasTraversedContainerFromSide;	
	bool HasTraversedContainerFromTop= fScatteringData.HasTraversedContainerFromTop;
	bool HasNavigationFailure= fScatteringData.HasNavigationFailure;
	
	//double TOF= fScatteringData.TOF;	
	//double TrackSpeed= fScatteringData.TrackSpeed;	
	//double TrackLength= fScatteringData.TrackLength;	

	if(HasNavigationFailure){
		if(fVerbosity>1) cout<<"EMTomographyRecoThreadObj::IsGoodEvent(): INFO: Skip event due to navigation failures ..."<<endl;
		isGoodEvent= false;	
	}

	if(!HasTraversedContainer){
		if(fVerbosity>1) cout<<"EMTomographyRecoThreadObj::IsGoodEvent(): INFO: Skip event not traversing the entire container volume ..."<<endl;
		isGoodEvent= false;	
	} 

	if(HasTraversedContainerFromTop && !fUseTopBottomEvents){
		if(fVerbosity>1) cout<<"EMTomographyRecoThreadObj::IsGoodEvent(): INFO: Skip event traversing the container volume from top-bottom sides..."<<endl;
		isGoodEvent= false;	
	}

	if(HasTraversedContainerFromSide && !fUseSideEvents){
		if(fVerbosity>1) cout<<"EMTomographyRecoThreadObj::IsGoodEvent(): INFO: Skip event traversing the container volume from side..."<<endl;
		isGoodEvent= false;	
	}

	if(fUseEnergyCut && (KinEnergy<fEnergyMinCut || KinEnergy>fEnergyMaxCut) ){
		if(fVerbosity>1) cout<<"EMTomographyRecoThreadObj::IsGoodEvent(): INFO: Skip event below energy threshold (E="<<KinEnergy<<" ["<<fEnergyMinCut<<","<<fEnergyMaxCut<<"])"<<endl;
		isGoodEvent= false;	
	}

	if(fUseScatteringAngleCut && (ScatteringAngle<fScatteringAngleMinCut || ScatteringAngle>fScatteringAngleMaxCut) ){
		if(fVerbosity>1) cout<<"EMTomographyRecoThreadObj::IsGoodEvent(): INFO: Skip event not passing ScatteringAngle cut (Theta="<<ScatteringAngle<<" ["<<fScatteringAngleMinCut<<","<<fScatteringAngleMaxCut<<"])..."<<endl;
		isGoodEvent= false;
	}

	if(fUseDisplacementCut && ( (fabs(DeltaX)<fDisplacementMinCut || fabs(DeltaX)>fDisplacementMaxCut) && (fabs(DeltaY)<fDisplacementMinCut || fabs(DeltaY)>fDisplacementMaxCut) ) ){
		if(fVerbosity>1) cout<<"EMTomographyRecoThreadObj::IsGoodEvent(): INFO: Skip event not passing Displacement cut (DeltaX="<<DeltaX<<" DeltaY="<<DeltaY<<" ["<<fDisplacementMinCut<<","<<fDisplacementMaxCut<<"])..."<<endl;
		isGoodEvent= false;
	}

	//if(fUseTrackSpeedCut && (TrackSpeed<fTrackSpeedMinCut || TrackSpeed>fTrackSpeedMaxCut) ){
	//	isGoodEvent= false;
	//}

	return isGoodEvent;
	
}//close EMTomographyRecoThreadObj::IsGoodEvent()




void EMTomographyRecoThreadObj::VoxelFinder(){

	if(fVerbosity>2) cout<<"EMTomographyRecoThreadObj::VoxelFinder(): INFO: Calculating path length in voxels ..."<<endl;
	
	std::map<int,VoxelPathLengthData> voxelPathDataMap;
	
	if(fVerbosity>2){
		cout<<"EMTomographyRecoThreadObj::VoxelFinder(): INFO: IsPocaInsideContainer? "<<fScatteringData.IsPOCAInsideContainer<<"  POCA=("<<fScatteringData.POCA[0]<<","<<fScatteringData.POCA[1]<<","<<fScatteringData.POCA[2]<<")"<<endl;
		cout<<"EMTomographyRecoThreadObj::VoxelFinder(): INFO: ScatteringAngle[deg]="<<fScatteringData.ScatteringAngle* TMath::RadToDeg()<<"  DeltaX[cm]="<<fScatteringData.DeltaX<<"  DeltaY[cm]="<<fScatteringData.DeltaY<<endl;  	
		cout<<"EMTomographyRecoThreadObj::VoxelFinder(): INFO: EntryPoint ("<<fScatteringData.EntryPoint[0]<<","<<fScatteringData.EntryPoint[1]<<","<<fScatteringData.EntryPoint[2]<<")"<<endl;	
		cout<<"EMTomographyRecoThreadObj::VoxelFinder(): INFO: MiddlePoint ("<<fScatteringData.MiddlePoint[0]<<","<<fScatteringData.MiddlePoint[1]<<","<<fScatteringData.MiddlePoint[2]<<")"<<endl;	
		cout<<"EMTomographyRecoThreadObj::VoxelFinder(): INFO: ExitPoint ("<<fScatteringData.ExitPoint[0]<<","<<fScatteringData.ExitPoint[1]<<","<<fScatteringData.ExitPoint[2]<<")"<<endl;	

		cout<<"EMTomographyRecoThreadObj::VoxelFinder(): INFO: EntryDir ("<<fScatteringData.EntryDirection[0]<<","<<fScatteringData.EntryDirection[1]<<","<<fScatteringData.EntryDirection[2]<<")"<<endl;
		cout<<"EMTomographyRecoThreadObj::VoxelFinder(): INFO: MiddleDir ("<<fScatteringData.MiddleDirection[0]<<","<<fScatteringData.MiddleDirection[1]<<","<<fScatteringData.MiddleDirection[2]<<")"<<endl;
		cout<<"EMTomographyRecoThreadObj::VoxelFinder(): INFO: ExitDir ("<<fScatteringData.ExitDirection[0]<<","<<fScatteringData.ExitDirection[1]<<","<<fScatteringData.ExitDirection[2]<<")"<<endl;				
	}	
	
	
	
	//## Set start navigation info
	double xPOCA= fScatteringData.POCA[0];
	double yPOCA= fScatteringData.POCA[1];
	double zPOCA= fScatteringData.POCA[2];
	double xEntry= fScatteringData.EntryPoint[0];
	double yEntry= fScatteringData.EntryPoint[1];
	double zEntry= fScatteringData.EntryPoint[2];
	double xEntryDir= fScatteringData.EntryDirection[0];
	double yEntryDir= fScatteringData.EntryDirection[1];
	double zEntryDir= fScatteringData.EntryDirection[2];
	double xMiddle= fScatteringData.MiddlePoint[0];
	double yMiddle= fScatteringData.MiddlePoint[1];
	double zMiddle= fScatteringData.MiddlePoint[2];
	double xMiddleDir= fScatteringData.MiddleDirection[0];
	double yMiddleDir= fScatteringData.MiddleDirection[1];
	double zMiddleDir= fScatteringData.MiddleDirection[2];
	double xExit= fScatteringData.ExitPoint[0];
	double yExit= fScatteringData.ExitPoint[1];
	double zExit= fScatteringData.ExitPoint[2];


	G4ThreeVector currentPoint= G4ThreeVector(xEntry*CLHEP::cm,yEntry*CLHEP::cm,zEntry*CLHEP::cm);
	G4ThreeVector currentDirection= G4ThreeVector(xEntryDir,yEntryDir,zEntryDir);
	G4ThreeVector currentStopPoint= G4ThreeVector(xMiddle*CLHEP::cm,yMiddle*CLHEP::cm,zMiddle*CLHEP::cm);
	double depthZ= zEntry;
	double StopPosZ= zMiddle;
	
	//## Set normal to plane surface
	G4ThreeVector n(0,0,1);
	double safetyStep = 0.0;	
	double tolerance= 1.e-9;
	double epsStep= 0;
	double totalPathLength= 0.;//total traversed path length in cm
	double totalPathRadLength= 0.;//total traversed path length in rad length units
	
	G4VPhysicalVolume* currentVolume= 0;
	G4VPhysicalVolume* newVolume= 0;
	
	

	//## Do a loop until the stop position depth is reached
	//cout<<"depthZ"<<depthZ<<"  StopPosZ="<<StopPosZ<<endl;

	while( fabs(depthZ-StopPosZ)>tolerance ){

		currentVolume= fG4Navigator->LocateGlobalPointAndSetup(currentPoint,&currentDirection);
		int currentVoxelId= currentVolume->GetCopyNo();
		
		//## Calculate max step point given the current position and the position of a plane z= StopPosZ    
		double stepNum= (currentStopPoint-currentPoint).dot(n);
		double stepDenom= currentDirection.dot(n);
		double currentMaxStep= (fabs(stepNum/stepDenom) + epsStep);
		
		//## Move to the next boundary
		double stepSize= fG4Navigator->ComputeStep( currentPoint, currentDirection, currentMaxStep, safetyStep );
		if(stepSize>currentMaxStep){
			stepSize= currentMaxStep;
		}
		//cout<<"stepSize="<<stepSize<<"  currentMaxStep="<<currentMaxStep<<"  stepNum="<<stepNum<<"  stepDenom="<<stepDenom<<endl;

		//## Find the location of the next boundary
    G4ThreeVector newPoint= currentPoint + stepSize * currentDirection;

		//## Check if navigator actually managed to move
    if( (newPoint - currentPoint).mag2() > 0) {

			//## Add traversed path length
			double traversedLength= stepSize/CLHEP::cm;//in cm
			totalPathLength+= traversedLength;
			
			std::map<int,VoxelPathLengthData>::iterator it= voxelPathDataMap.find(currentVoxelId);	
			VoxelPathLengthData thisVoxelPathData;
			thisVoxelPathData.fL= traversedLength;
			thisVoxelPathData.fT= totalPathLength;
			if( voxelPathDataMap.empty() || it==voxelPathDataMap.end() ){
				voxelPathDataMap.insert( make_pair(currentVoxelId,thisVoxelPathData) );
			}
			else{
				((*it).second).fL+= traversedLength;
				((*it).second).fT+= traversedLength;
			}

			//cout<<"depthZ= "<<depthZ<<"  currentVoxelId="<<currentVoxelId<<"  L[cm]="<<traversedLength<<"  T[cm]="<<totalPathLength<<endl;

			//## Get new volume info
			newVolume= fG4Navigator->LocateGlobalPointAndSetup(newPoint,&currentDirection);
			if(!newVolume){
				cerr<<"EMTomographyRecoThreadObj::SetVoxelData(): INFO: WARNING: Navigator outside the world...exit loop!"<<endl;
				break;
			}		
			
			//## Update current point and material properties
			currentPoint= newPoint;
			depthZ= newPoint.z()/CLHEP::cm;
			
		}//close if 

	}//end loop reached z depth 

	
	//## Middle point -> Exit point
	if(fScatteringData.IsPOCAInsideContainer){	
		currentPoint= G4ThreeVector(xMiddle*CLHEP::cm,yMiddle*CLHEP::cm,zMiddle*CLHEP::cm);
		currentDirection= G4ThreeVector(xMiddleDir,yMiddleDir,zMiddleDir);
		currentStopPoint= G4ThreeVector(xExit*CLHEP::cm,yExit*CLHEP::cm,zExit*CLHEP::cm);
		depthZ= zMiddle;
		StopPosZ= zExit;
		
		//## Do a loop until the stop position depth is reached
		while( fabs(depthZ-StopPosZ)>tolerance ){
			
			currentVolume= fG4Navigator->LocateGlobalPointAndSetup(currentPoint,&currentDirection); 
			int currentVoxelId= currentVolume->GetCopyNo();
			
			//## Calculate max step point given the current position and the position of a plane
			//## z= StopPosZ
			double stepNum= (currentStopPoint-currentPoint).dot(n);
			double stepDenom= currentDirection.dot(n);
			double currentMaxStep= (fabs(stepNum/stepDenom) + epsStep);
			
			//## Move to the next boundary
			double stepSize= fG4Navigator->ComputeStep( currentPoint, currentDirection, currentMaxStep, safetyStep );
			if(stepSize>currentMaxStep){
				stepSize= currentMaxStep;
			}

			//## Find the location of the next boundary
    	G4ThreeVector newPoint= currentPoint + stepSize * currentDirection;

			//## Check if navigator actually managed to move
   		if( (newPoint - currentPoint).mag2() > 0) {

				//## Add traversed path length
				double traversedLength= stepSize/CLHEP::cm;//in cm
				totalPathLength+= traversedLength;
				
				
				std::map<int,VoxelPathLengthData>::iterator it= voxelPathDataMap.find(currentVoxelId);	
				VoxelPathLengthData thisVoxelPathData;
				thisVoxelPathData.fL= traversedLength;
				thisVoxelPathData.fT= totalPathLength;
				if( voxelPathDataMap.empty() || it==voxelPathDataMap.end() ){
					voxelPathDataMap.insert( make_pair(currentVoxelId,thisVoxelPathData) );
				}
				else{
					((*it).second).fL+= traversedLength;
					((*it).second).fT+= traversedLength;
				}

				//cout<<"depthZ= "<<depthZ<<"  currentVoxelId="<<currentVoxelId<<"  L[cm]="<<traversedLength<<"  T[cm]="<<totalPathLength<<endl;

				//## Get new volume info
				newVolume= fG4Navigator->LocateGlobalPointAndSetup(newPoint,&currentDirection);
				if(!newVolume){
					cerr<<"EMTomographyRecoThreadObj::SetVoxelData(): WARNING: Navigator outside the world...exit loop!"<<endl;
					break;
				}		

				//## Update current point and material properties
				currentPoint= newPoint;
				depthZ= newPoint.z()/CLHEP::cm;
				
			}//close if 

			
		}//end loop reached z depth 
				
	}//close if fIsPOCAInsideContainer
	
	if(fVerbosity>2) {
		cout<<"EMTomographyRecoThreadObj::SetVoxelData(): INFO: Total path length l[cm]="<<totalPathLength<<endl;
		cout<<"EMTomographyRecoThreadObj::SetVoxelData(): INFO: Creating muon event and append data ..."<<endl;	
	}
	
	
	//## Set muon event 
	fMuonEventData= new MuonEventData;
	fMuonEventData->Reset();

	
	double ScatteringAngle= fScatteringData.ScatteringAngle;//*1.e+03;//mrad
	double DeltaThetaX= fScatteringData.DeltaThetaX;//*1.e+03;//mrad
	double DeltaThetaY= fScatteringData.DeltaThetaY;//*1.e+03;//mrad
	double DeltaX= fScatteringData.DeltaX;//cm
	double DeltaY= fScatteringData.DeltaY;//cm
	double Pr= fScatteringData.pr;//no units
	double Ex= fScatteringData.Ex;//*1.e+6;//mrad^2
	double Ey= fScatteringData.Ey;//cm^2
	double Exy= fScatteringData.Exy;//*1.e+03;//mrad*cm
	double POCAX= fScatteringData.POCA[0];//in cm
	double POCAY= fScatteringData.POCA[1];//in cm
	double POCAZ= fScatteringData.POCA[2];//in cm

	fMuonEventData->fEventId= fEventId;
	fMuonEventData->fDeltaThetaX= DeltaThetaX;//*1.e+03;//mrad
	fMuonEventData->fDeltaThetaY= DeltaThetaY;//*1.e+03;//mrad
	fMuonEventData->fDeltaX= DeltaX;//cm
	fMuonEventData->fDeltaY= DeltaY;//cm
	fMuonEventData->fPr= Pr;//no units
	if(fUseDetectorCovariance){

		fMuonEventData->fEx= fEx;//*1.e+6;//mrad^2
		fMuonEventData->fEy= fEy;//cm^2
		fMuonEventData->fExy= fExy;//*1.e+03;//mrad*cm
	}
	else{
		fMuonEventData->fEx= 0.;
		fMuonEventData->fEy= 0.;
		fMuonEventData->fExy= 0.;
	}

	if(fVerbosity>2) cout<<"EMTomographyRecoThreadObj::VoxelFinder(): INFO: Voxel info..."<<endl;
	bool isSanityCheckPassed= true;

	int voxelCounter= 0;

	

	for(std::map<int,VoxelPathLengthData>::const_iterator it=voxelPathDataMap.begin();it!=voxelPathDataMap.end();++it){
		VoxelPathLengthData thisVoxelPathData= (*it).second;
		int voxelId= (*it).first;
		double L= thisVoxelPathData.fL;
		double T= totalPathLength-thisVoxelPathData.fT;
		double Wx= L;
		double Wy= L*L*L/3. + L*L*T + L*T*T;
		double Wxy= L*L/2. + L*T; 
		if(fVerbosity>2) cout<<"Voxel no. "<<voxelId<<"  L[cm]="<<L<<"  T[cm]="<<T<<endl;

		//## sanity check!
		if( L>sqrt(fVoxelSizeX*fVoxelSizeX+fVoxelSizeY*fVoxelSizeY+fVoxelSizeZ*fVoxelSizeZ) || L<0){
			cerr<<"EMTomographyRecoThreadObj::VoxelFinder(): WARNING: Computed path length in voxel exceeds maximum length, potential problems...check!"<<endl;
			isSanityCheckPassed= false;
		}

		VoxelData aVoxelData;
		aVoxelData.fId= voxelId;
		aVoxelData.fWx= Wx;
		aVoxelData.fWy= Wy;
		aVoxelData.fWxy= Wxy;
		
		fMuonEventData->AddVoxelData(aVoxelData);

		fVoxelIdList[voxelCounter]= voxelId;
		fVoxelWx[voxelCounter]= Wx;
		fVoxelWy[voxelCounter]= Wy;
		fVoxelWxy[voxelCounter]= Wxy;
		voxelCounter++;

		//integrity check
		if(voxelId<0 || voxelId>=fNVoxel){
			cerr<<"EMTomographyRecoThreadObj::VoxelFinder(): ERROR: Invalid voxel id (id="<<voxelId<<") ...exit!"<<endl;
			exit(1);
		}		
		fNEventPerVoxel[voxelId]++;
	}
	
	fNTraversedVoxels= voxelCounter;

	

	if(fVerbosity>2) cout<<"EMTomographyRecoThreadObj::VoxelFinder(): INFO: "<<voxelPathDataMap.size()<<" voxel hit in this event..."<<endl;

	//## Add event to global collection
	if(isSanityCheckPassed) {
		fMuonEventDataCollection.push_back(fMuonEventData);


		//## Find POCA voxel
		fPOCAVoxelId= -1;
		
		
		if(fScatteringData.IsPOCAInsideContainer){	
			int pocaVoxelIdX = (int)( (xPOCA + fVoxelSizeX/2.*fNVoxelX)/fVoxelSizeX);
			int pocaVoxelIdY = (int)( (yPOCA + fVoxelSizeY/2.*fNVoxelY)/fVoxelSizeY);
			int pocaVoxelIdZ = (int)( (zPOCA + fVoxelSizeZ/2.*fNVoxelZ)/fVoxelSizeZ);
			fPOCAVoxelId = pocaVoxelIdX + fNVoxelX*pocaVoxelIdY + fNVoxelX*fNVoxelY*pocaVoxelIdZ;
			if(fPOCAVoxelId<0 || fPOCAVoxelId>=fNVoxel){ 
				cerr<<"EMTomographyRecoThreadObj::VoxelFinder(): ERROR: Invalid pocaVoxelId="<<fPOCAVoxelId<<"("<<pocaVoxelIdX<<","<<pocaVoxelIdY<<","<<pocaVoxelIdZ<<") ...exit!"<<endl;
				cerr<<"POCA ("<<xPOCA<<","<<yPOCA<<","<<zPOCA<<")"<<endl;
				exit(1);
			}	
			if(fScatteringData.IsPOCAReliable){
				fPOCAVoxelSignal[fPOCAVoxelId]+= pow(fScatteringData.ScatteringAngle*TMath::RadToDeg(),2);
				fNPOCAPerVoxel[fPOCAVoxelId]++;
			}

		}
		else {
			fPOCAVoxelId= -1;
		}

		//cout<<"LikelihoodImageReco::VoxelFinder(): INFO: POCAVoxelId: "<<fPOCAVoxelId<<"  DeltaTheta=("<<DeltaThetaX<<","<<DeltaThetaY<<") DeltaX("<<DeltaX<<","<<DeltaY<<") Pr="<<Pr<<endl;

		//## Fill scattering data tree
		fNHitVoxels= (int)(voxelPathDataMap.size());
		fDeltaThetaX= DeltaThetaX;
		fDeltaThetaY= DeltaThetaY;
		fDeltaX= DeltaX;
		fDeltaY= DeltaY;
		fScatteringAngle= ScatteringAngle* TMath::RadToDeg();
		fPr= fScatteringData.pr;
		fPOCAX= POCAX;
		fPOCAY= POCAY;
		fPOCAZ= POCAZ;
		fKinEnergy= fScatteringData.KinEnergy;
		//fTOF= fScatteringData.TOF;
		//fTrackLength= fScatteringData.TrackLength;
		//fTrackSpeed= fScatteringData.TrackSpeed;	
		fScatteringDataInfo->Fill();

	}//close if sanity check passed

	
	if(fVerbosity>2) cout<<"EMTomographyRecoThreadObj::VoxelFinder(): INFO: end voxel finder..."<<endl;

	

}//close EMTomographyRecoThreadObj::VoxelFinder()






void EMTomographyRecoThreadObj::ImageReco(){

	cout<<"EMTomographyRecoThreadObj::ImageReco(): Start image reconstruction ..."<<endl;

	//## Start EM minimization
	FitInfo fitInfo;
	fitInfo.fLogLikelihood= 0.;
	fitInfo.fEMLogLikelihood= 0.;
	fitInfo.fChi2= 0.;
	fitInfo.fRelDeltaLogLikelihood= 0.;
	fitInfo.fDeltaLogLikelihood= 0.;
	fitInfo.fDeltaEMLogLikelihood= 0.;
	fitInfo.fRMS= 0.;
	fitInfo.fBias= 0.;
	fitInfo.fRMSThreatVoxel= 0.;
	fitInfo.fBiasThreatVoxel= 0.;
	fitInfo.fIterRMS= 0.;
	fitInfo.fResErr= 0.;
	fitInfo.fSResErr= 0.;
	fitInfo.fRelResErr= 0.;
  fitInfo.fRelSResErr= 0.;

	FitInfo tmpfitInfo;
	tmpfitInfo= fitInfo;
	FitInfo tmpfitInfo2;
	tmpfitInfo2= fitInfo;

	int nStopIter_resErr= 0;
	int nStopIter_logLike= 0;
	int nStopIter_sresErr= 0;
	int lastIter_resErr= -1;
	int lastIter_logLike= -1;
	int lastIter_sresErr= -1;

	

	std::vector<double> tmpLambda;
	tmpLambda.clear();
	tmpLambda.assign(fLambda.size(),0.);
	
	fNIter= fNIterations;
	fIsGoodAccelerationStep= true;

	
	double thisProgressPercentage= 0;

	for(int k=0;k<fNIterations;k++){
		
		std::vector<double> lambdaUpdate;
		
	
		//## Dump info
		cout<<"*** ITER NO. "<<k<<" ***"<<endl;
		cout<<"--> LogL(complete)="<<fitInfo.fEMLogLikelihood<<"  LogL(obs)="<<fitInfo.fLogLikelihood<<"  DeltaLogL abs/rel="<<fitInfo.fDeltaLogLikelihood<<"/"<<fitInfo.fRelDeltaLogLikelihood<<endl;
  	cout<<"--> RMS tot/threat="<<fitInfo.fRMS<<"/"<<fitInfo.fRMSThreatVoxel<<"  BIAS tot/threat="<<fitInfo.fBias<<"/"<<fitInfo.fBiasThreatVoxel<<"  rmsIter="<<fitInfo.fIterRMS<<endl;
		cout<<"--> Residuals abs/squared: "<<fitInfo.fResErr<<"/"<<fitInfo.fSResErr<<"  Relative res abs/squared: "<<fitInfo.fRelResErr<<"/"<<fitInfo.fRelSResErr<<endl;
		cout<<"--> Pixel RelRes min/max: "<<fitInfo.fPixelMinRelResErr<<"/"<<fPixelMaxRelResErr<<"  Mean/Variance: "<<fitInfo.fPixelMean<<"/"<<fPixelVariance<<endl;
		cout<<"************************"<<endl;

		//## EM iteration step
		cout<<"EMTomographyRecoThreadObj::ImageReco(): INFO: Using standard EM update ..."<<endl;
		lambdaUpdate= EMUpdate(fLambda,fitInfo,false);		
		
		//## Copy back to lambda
		cout<<"EMTomographyRecoThreadObj::ImageReco(): INFO: Updating Lambda values ..."<<endl;
		if(fVerbosity>1){ 
			cout<<"*********************"<<endl;
			for(unsigned int j=0;j<lambdaUpdate.size();j++) {
				fLambda[j]= lambdaUpdate[j];			
				cout<<"--> Voxel no. "<<j<<":  Lambda[mrad^2 cm]="<<fLambda[j]<<endl;
			}
			cout<<"*********************"<<endl;
		}
		else{
			for(unsigned int j=0;j<lambdaUpdate.size();j++) {
				fLambdaIter[j]= fLambda[j];
				fLambda[j]= lambdaUpdate[j];
			}
		}

		//## Add fit info 
		fIterNo= k;
		fLL= fitInfo.fLogLikelihood;
		fEMLL= fitInfo.fEMLogLikelihood;
		fDeltaLL= fitInfo.fDeltaLogLikelihood;
		fDeltaEMLL= fitInfo.fDeltaEMLogLikelihood;
		fImageRMS= fitInfo.fRMS;
		fImageBias= fitInfo.fBias;
		fImageRMSThreatVoxel= fitInfo.fRMSThreatVoxel;
		fImageBiasThreatVoxel= fitInfo.fBiasThreatVoxel;
		fImageIterRMS= fitInfo.fIterRMS;
		fImageResErr= fitInfo.fResErr;
		fImageSResErr= fitInfo.fSResErr;
		fImageRelResErr= fitInfo.fRelResErr;
		fImageRelSResErr= fitInfo.fRelSResErr;
		fPixelMinRelResErr= fitInfo.fPixelMinRelResErr;
		fPixelMaxRelResErr= fitInfo.fPixelMaxRelResErr;
		fPixelMean= fitInfo.fPixelMean;
		fPixelVariance= fitInfo.fPixelVariance;
		fNPar= (int)lambdaUpdate.size();
		fIterationInfo->Fill();

		//## Check Res-Error stopping condition
		//if(fUseResErrStopping && fitInfo.fResErr<=fResErrTolerance && k>0){
		if(fUseResErrStopping && fitInfo.fRelResErr<=fResErrTolerance && k>0){
			if( (lastIter_resErr==-1) || (k-lastIter_resErr)==1 ) {
				nStopIter_resErr++;
				lastIter_resErr= k;
			}
			//cout<<"LikelihoodsImageReco::ImageReco(): INFO: Residual Error stopping condition matched (obs/target="<<fitInfo.fResErr<<"/"<<fResErrTolerance<<") during "<<nStopIter_resErr<<" iterations ..."<<endl;
			cout<<"EMTomographyRecoThreadObj::ImageReco(): INFO: Residual Error stopping condition matched (obs/target="<<fitInfo.fRelResErr<<"/"<<fResErrTolerance<<") during "<<nStopIter_resErr<<" iterations ..."<<endl;

			if(nStopIter_resErr>=fNIterForResErrStopping) {
				//cout<<"LikelihoodImageReco::ImageReco(): INFO: Residual Error stopping condition matched (obs/target="<<fitInfo.fResErr<<"/"<<fResErrTolerance<<" during "<<nStopIter_resErr<<" iterations) ...end iteration!"<<endl;
				cout<<"EMTomographyRecoThreadObj::ImageReco(): INFO: Residual Error stopping condition matched (obs/target="<<fitInfo.fRelResErr<<"/"<<fResErrTolerance<<" during "<<nStopIter_resErr<<" iterations) ...end iteration!"<<endl;
				fNIter= k;

				progressPercentage= 3.*stagePercentage;
				emit(statusProgress((int)(progressPercentage)));

				fLogMessage.status= eRunning;
				fLogMessage.logMessage= "--> EM Image Reco: Convergence reached!";
				fLogMessage.progress= (int)(progressPercentage);

				QString logMessage= makeLogMessage(fLogMessage);
				emit logSig(logMessage);

				break;
			}			
		}//close if

		

		//## Check Log-Like stopping condition
		if(fUseLogLikeStopping && fitInfo.fDeltaLogLikelihood<=fLogLikeTolerance && k>0){
			
			if(lastIter_logLike==-1 || (k-lastIter_logLike)==1 ) {
				nStopIter_logLike++;
				lastIter_logLike= k;
			}
			cout<<"EMTomographyRecoThreadObj::ImageReco(): INFO: LogLikelihood stopping condition matched (obs/target="<<fitInfo.fDeltaLogLikelihood<<"/"<<fLogLikeTolerance<<") during "<<nStopIter_logLike<<" iterations ..."<<endl;

			if(nStopIter_logLike>=fNIterForLogLikeStopping) {
				cout<<"EMTomographyRecoThreadObj::ImageReco(): INFO: LogLikelihood stopping condition matched (obs/target="<<fitInfo.fDeltaLogLikelihood<<"/"<<fLogLikeTolerance<<" during "<<nStopIter_logLike<<" iterations)...end iteration!"<<endl;
				fNIter= k;

				progressPercentage= 3.*stagePercentage;
				emit(statusProgress((int)(progressPercentage)));
				fLogMessage.status= eRunning;
				fLogMessage.logMessage= "--> EM Image Reco: Convergence reached!";
				fLogMessage.progress= (int)(progressPercentage);

				QString logMessage= makeLogMessage(fLogMessage);
				emit logSig(logMessage);

				break;
			}			
		}//close if

		thisProgressPercentage= (double)(k+1)/(double)(fNIterations)*stagePercentage;
		
		emit(statusProgress((int)(progressPercentage+thisProgressPercentage)));

		fLogMessage.status= eRunning;
		fLogMessage.logMessage= Form("--> EM Image Reco: Iteration no. %d",k);
		fLogMessage.progress= (int)(progressPercentage+thisProgressPercentage);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);

	}//end iter loop
	
	progressPercentage+= thisProgressPercentage;

	//## Fill tree with final rec image
	RecMap3D->Reset();	
	NEventMap3D->Reset();

	for(unsigned int j=0;j<fLambda.size();j++){
		int copyNo= j;
		fNx = size_t( copyNo%fNVoxelX );
		fNy = size_t( (copyNo/fNVoxelX)%fNVoxelY );
		fNz = size_t( copyNo/(fNVoxelX*fNVoxelY) );
		fScatteringDensity= fLambda[j];
		fNEventInVoxel= fNEventPerVoxel[j];
		fVoxelX= fContainerXStart + fVoxelSizeX/2 + fNx*fVoxelSizeX;
		fVoxelY= -(fContainerYStart + fVoxelSizeY/2 + fNy*fVoxelSizeY);
		fVoxelZ= fContainerZStart + fVoxelSizeZ/2 + fNz*fVoxelSizeZ;
		fVoxelId= j;
		fRecTomographyImageInfo->Fill();

	
		
		int binX= dummyXLevelHisto->FindBin(fVoxelX);
		int binY= dummyYLevelHisto->FindBin(fVoxelY);
		int binZ= dummyZLevelHisto->FindBin(fVoxelZ);
		
		double LogLambda= log10(fScatteringDensity);
	
		if( !dummyXLevelHisto->IsBinOverflow(binX) && !dummyXLevelHisto->IsBinUnderflow(binX) && 
				!dummyYLevelHisto->IsBinOverflow(binY) && !dummyXLevelHisto->IsBinUnderflow(binY) &&
				!dummyZLevelHisto->IsBinOverflow(binZ) && !dummyZLevelHisto->IsBinUnderflow(binZ) &&
        fNEventInVoxel>fEMLLNEventThreshold
		){
			//cout<<"Voxel No. "<<fVoxelId<<"  ("<<fVoxelX<<","<<fVoxelY<<","<<fVoxelZ<<") fNEventInVoxel="<<fNEventInVoxel<<"  Lambda="<< fScatteringDensity<<endl;
			RecMap3D->Fill(fVoxelX,fVoxelY,fVoxelZ,fScatteringDensity);
			NEventMap3D->Fill(fVoxelX,fVoxelY,fVoxelZ,fNEventInVoxel);

			RecMap_XFix[binX-1]->Fill(fVoxelY,fVoxelZ,fScatteringDensity);
			NEventMap_XFix[binX-1]->Fill(fVoxelY,fVoxelZ,fNEventInVoxel);
		
			RecMap_YFix[binY-1]->Fill(fVoxelX,fVoxelZ,fScatteringDensity);	
			NEventMap_YFix[binY-1]->Fill(fVoxelX,fVoxelZ,fNEventInVoxel);	
		
			RecMap_ZFix[binZ-1]->Fill(fVoxelX,fVoxelY,fScatteringDensity);
			NEventMap_ZFix[binZ-1]->Fill(fVoxelX,fVoxelY,fNEventInVoxel);
		}
			
	}//end loop voxels

	/*
	//## Set to zero bins below Nevent threshold
	for(int k=0;k<RecMap3D->GetNbinsZ();k++){
		for(int j=0;j<RecMap3D->GetNbinsY();j++){
			for(int i=0;i<RecMap3D->GetNbinsX();i++){		
				double N= NEventMap3D->GetBinContent(i+1,j+1,k+1);
				cout<<"("<<i<<","<<j<<","<<k<<") N="<<N<<"  fEMLLNEventThreshold="<<fEMLLNEventThreshold<<endl;
				//if(N<fEMLLNEventThreshold) RecMap3D->SetBinContent(i+1,j+1,k+1,0);	
			}//end loop bins x
		}//end loop bins y
	}//end loop bins z

	for(int k=0;k<fNVoxelZ;k++){
		for(int i=0;i<RecMap_ZFix[k]->GetNbinsX();i++){
			for(int j=0;j<RecMap_ZFix[k]->GetNbinsY();j++){
				double N= NEventMap_ZFix[k]->GetBinContent(i+1,j+1);
				if(N<fEMLLNEventThreshold) RecMap_ZFix[k]->SetBinContent(i+1,j+1,0);	
			}//end loop bins Y
		}//end loop bins X
	}//end loop voxel Z

	for(int k=0;k<fNVoxelX;k++){
		for(int i=0;i<RecMap_XFix[k]->GetNbinsX();i++){
			for(int j=0;j<RecMap_XFix[k]->GetNbinsY();j++){
				double N= NEventMap_XFix[k]->GetBinContent(i+1,j+1);
				if(N<fEMLLNEventThreshold) RecMap_XFix[k]->SetBinContent(i+1,j+1,0);	
			}//end loop bins Y
		}//end loop bins X
	}//end loop voxel X

	for(int k=0;k<fNVoxelY;k++){
		for(int i=0;i<RecMap_YFix[k]->GetNbinsX();i++){
			for(int j=0;j<RecMap_YFix[k]->GetNbinsY();j++){
				double N= NEventMap_YFix[k]->GetBinContent(i+1,j+1);
				if(N<fEMLLNEventThreshold) RecMap_YFix[k]->SetBinContent(i+1,j+1,0);	
			}//end loop bins Y
		}//end loop bins X
	}//end loop voxel Y
*/
	//## Write to ascii file and emit alarm
	//FILE* fout;
	//fout= fopen(fVisIVOOutputFileName.c_str(),"w");
	//fprintf(fout,"S\n");


	cout<<"EMTomographyRecoThreadObj::ImageReco(): INFO: Dump output data to VTK format..."<<endl;

	FILE* fout_vtk;
	fout_vtk= fopen("EMLLVTKData.vtk","w");
	fprintf(fout_vtk,"# vtk DataFile Version 2.0\n");
	fprintf(fout_vtk,"Sample rectilinear grid\n");	
	fprintf(fout_vtk,"ASCII\n");	
	fprintf(fout_vtk,"DATASET RECTILINEAR_GRID\n");	
	fprintf(fout_vtk,"DIMENSIONS %d %d %d\n",fNVoxelX,fNVoxelY,fNVoxelZ);	
	fprintf(fout_vtk,"X_COORDINATES %d float\n",fNVoxelX);	
	for(int i=0;i<RecMap3D->GetNbinsX();i++){	
		double x= RecMap3D->GetXaxis()->GetBinCenter(i+1);
		fprintf(fout_vtk,"%f\n",x);		
	}
	fprintf(fout_vtk,"Y_COORDINATES %d float\n",fNVoxelY);	
	for(int j=0;j<RecMap3D->GetNbinsY();j++){
		double y= RecMap3D->GetYaxis()->GetBinCenter(j+1);
		fprintf(fout_vtk,"%f\n",y);			
	}
	fprintf(fout_vtk,"Z_COORDINATES %d float\n",fNVoxelZ);
	for(int k=0;k<RecMap3D->GetNbinsZ();k++){
		double z= RecMap3D->GetZaxis()->GetBinCenter(k+1);
		fprintf(fout_vtk,"%f\n",z);
	}
	fprintf(fout_vtk,"POINT_DATA %d\n",fNVoxel);
	fprintf(fout_vtk,"SCALARS scalars float\n");
	fprintf(fout_vtk,"LOOKUP_TABLE default\n");
	for(int k=0;k<RecMap3D->GetNbinsZ();k++){
		for(int j=0;j<RecMap3D->GetNbinsY();j++){
			for(int i=0;i<RecMap3D->GetNbinsX();i++){		
				double binContent= RecMap3D->GetBinContent(i+1,j+1,k+1);
				
				fprintf(fout_vtk,"%f\n",binContent);		
			}//end loop y bins
		}//end loop x bins
	}//end loop z bins

	fclose(fout_vtk);


	ofstream fout;
  fout.open(fVisIVOOutputFileName.c_str());
	fout << "S"<<endl;

	bool isAlarm= false;
	cout<<"EMTomographyRecoThreadObj::ImageReco(): INFO: Check alarm condition.."<<endl;


	for(int k=0;k<RecMap3D->GetNbinsZ();k++){
		for(int j=0;j<RecMap3D->GetNbinsY();j++){
			for(int i=0;i<RecMap3D->GetNbinsX();i++){		
				double x= RecMap3D->GetXaxis()->GetBinCenter(i+1);
				double y= RecMap3D->GetYaxis()->GetBinCenter(j+1);
				double z= RecMap3D->GetZaxis()->GetBinCenter(k+1);
				double binContent= RecMap3D->GetBinContent(i+1,j+1,k+1);
				double N= NEventMap3D->GetBinContent(i+1,j+1,k+1);

				
				int	VoxelIdX = (int)( (x+fVoxelSizeX/2.*fNVoxelX)/fVoxelSizeX);
				int VoxelIdY = (int)( (y+fVoxelSizeY/2.*fNVoxelY)/fVoxelSizeY);
				int VoxelIdZ = (int)( (z+fVoxelSizeZ/2.*fNVoxelZ)/fVoxelSizeZ);
		
				int voxelId= GetVoxelId(VoxelIdX,VoxelIdY,VoxelIdZ);
				
				if(log10(binContent)>fEMLLSignalThreshold && N>fEMLLNEventThreshold) {
					isAlarm= true;
					fAlarmVoxelIdList.push_back(voxelId);
				}

				//fprintf(fout,"%f\n",binContent);
				fout<<binContent<<endl;

			}//end loop y bins
		}//end loop x bins
	}//end loop z bins
	
	//fclose(fout);
	fout.close();



	QString alarmMsg = QString("0");
	if(isAlarm) {
		emit alarm();

		alarmMsg= QString("1");
		emit(alarmStatusMessage(alarmMsg));

		fAlarmMessage.status= eRunning;
		fAlarmMessage.alarmVoxelList= fAlarmVoxelIdList;
		fAlarmMessage.alarmMessage= Form("--> ALARM (%d alarm voxels)!!!",fAlarmVoxelIdList.size());
		
		QString alarmMessage= makeAlarmMessage(fAlarmMessage);
		emit alarmSig(alarmMessage);
	}

}//close EMTomographyRecoThreadObj::ImageReco()


void EMTomographyRecoThreadObj::Draw(){

	fMinEMLLSignal= RecMap3D->GetMinimum(0);
	fMaxEMLLSignal= RecMap3D->GetMaximum();
	cout<<"EMTomographyRecoThreadObj::Draw(): INFO: min/max signal: "<<fMinEMLLSignal<<"/"<<fMaxEMLLSignal<<"  logScale="<<log10(fMinEMLLSignal)<<"/"<<log10(fMaxEMLLSignal)<<endl;
	
	
	//## Draw YZ tomography
	for(int k=0;k<fNVoxelX;k++){
		gStyle->SetTitleFontSize(0.05);
		gStyle->SetTitleFont(52,"t");
		RecMap_XFix[k]->SetLabelSize(10);
		RecMap_XFix[k]->SetTitleFont(52);
		RecMap_XFix[k]->GetXaxis()->SetTitle("Y [cm]");
		RecMap_XFix[k]->GetXaxis()->SetTitleSize(0.08);
		RecMap_XFix[k]->GetXaxis()->SetTitleOffset(0.75);
		RecMap_XFix[k]->GetXaxis()->SetTitleFont(52);
		RecMap_XFix[k]->GetXaxis()->SetLabelSize(0.05);
		RecMap_XFix[k]->GetXaxis()->SetLabelOffset(0.02);
		RecMap_XFix[k]->GetXaxis()->SetLabelFont(42);
		RecMap_XFix[k]->GetYaxis()->SetTitle("Z [cm]");
		RecMap_XFix[k]->GetYaxis()->SetTitleSize(0.08);
		RecMap_XFix[k]->GetYaxis()->SetTitleOffset(0.6);
		RecMap_XFix[k]->GetYaxis()->SetTitleFont(52);
		RecMap_XFix[k]->GetYaxis()->SetLabelSize(0.05);
		RecMap_XFix[k]->GetYaxis()->SetLabelOffset(0.012);
		RecMap_XFix[k]->GetYaxis()->SetLabelFont(42);
		RecMap_XFix[k]->GetZaxis()->SetTitle("Z [cm]");
		RecMap_XFix[k]->GetZaxis()->SetTitleSize(0.05);
		RecMap_XFix[k]->GetZaxis()->SetTitleOffset(1.1);
		RecMap_XFix[k]->GetZaxis()->SetTitleFont(52);
		RecMap_XFix[k]->GetZaxis()->SetLabelSize(0.04);
		RecMap_XFix[k]->GetZaxis()->SetLabelOffset(0.012);
		RecMap_XFix[k]->GetZaxis()->SetLabelFont(42);

		RecMap_XFix[k]->SetMinimum(fMinEMLLSignal);
		RecMap_XFix[k]->SetMaximum(fMaxEMLLSignal);
		RecMap_XFix[k]->SetContour(fNColourContours);
		//RecMap_XFix[k]->Draw(fDrawMode.c_str());
	
	}//end loop voxel X

	//## Draw XZ tomography
	for(int k=0;k<fNVoxelY;k++){
		gStyle->SetTitleFontSize(0.05);
		gStyle->SetTitleFont(52,"t");
		RecMap_YFix[k]->SetLabelSize(10);
		RecMap_YFix[k]->SetTitleFont(52);
		RecMap_YFix[k]->GetXaxis()->SetTitle("Y [cm]");
		RecMap_YFix[k]->GetXaxis()->SetTitleSize(0.08);
		RecMap_YFix[k]->GetXaxis()->SetTitleOffset(0.75);
		RecMap_YFix[k]->GetXaxis()->SetTitleFont(52);
		RecMap_YFix[k]->GetXaxis()->SetLabelSize(0.05);
		RecMap_YFix[k]->GetXaxis()->SetLabelOffset(0.02);
		RecMap_YFix[k]->GetXaxis()->SetLabelFont(42);
		RecMap_YFix[k]->GetYaxis()->SetTitle("Z [cm]");
		RecMap_YFix[k]->GetYaxis()->SetTitleSize(0.08);
		RecMap_YFix[k]->GetYaxis()->SetTitleOffset(0.6);
		RecMap_YFix[k]->GetYaxis()->SetTitleFont(52);
		RecMap_YFix[k]->GetYaxis()->SetLabelSize(0.05);
		RecMap_YFix[k]->GetYaxis()->SetLabelOffset(0.012);
		RecMap_YFix[k]->GetYaxis()->SetLabelFont(42);
		RecMap_YFix[k]->GetZaxis()->SetTitle("Z [cm]");
		RecMap_YFix[k]->GetZaxis()->SetTitleSize(0.05);
		RecMap_YFix[k]->GetZaxis()->SetTitleOffset(1.1);
		RecMap_YFix[k]->GetZaxis()->SetTitleFont(52);
		RecMap_YFix[k]->GetZaxis()->SetLabelSize(0.04);
		RecMap_YFix[k]->GetZaxis()->SetLabelOffset(0.012);
		RecMap_YFix[k]->GetZaxis()->SetLabelFont(42);

		RecMap_YFix[k]->SetMinimum(fMinEMLLSignal);
		RecMap_YFix[k]->SetMaximum(fMaxEMLLSignal);
		RecMap_YFix[k]->SetContour(fNColourContours);
		//RecMap_YFix[k]->Draw(fDrawMode.c_str());
		
	}//end loop voxel Y
	
	//## Draw XY tomography
	for(int k=0;k<fNVoxelZ;k++){
		
		gStyle->SetTitleFontSize(0.05);
		gStyle->SetTitleFont(52,"t");
		RecMap_ZFix[k]->SetLabelSize(10);
		RecMap_ZFix[k]->SetTitleFont(52);
		RecMap_ZFix[k]->GetXaxis()->SetTitle("X [cm]");
		RecMap_ZFix[k]->GetXaxis()->SetTitleSize(0.08);
		RecMap_ZFix[k]->GetXaxis()->SetTitleOffset(0.75);
		RecMap_ZFix[k]->GetXaxis()->SetTitleFont(52);
		RecMap_ZFix[k]->GetXaxis()->SetLabelSize(0.05);
		RecMap_ZFix[k]->GetXaxis()->SetLabelOffset(0.02);
		RecMap_ZFix[k]->GetXaxis()->SetLabelFont(42);
		RecMap_ZFix[k]->GetYaxis()->SetTitle("Y [cm]");
		RecMap_ZFix[k]->GetYaxis()->SetTitleSize(0.08);
		RecMap_ZFix[k]->GetYaxis()->SetTitleOffset(0.6);
		RecMap_ZFix[k]->GetYaxis()->SetTitleFont(52);
		RecMap_ZFix[k]->GetYaxis()->SetLabelSize(0.05);
		RecMap_ZFix[k]->GetYaxis()->SetLabelOffset(0.012);
		RecMap_ZFix[k]->GetYaxis()->SetLabelFont(42);
		RecMap_ZFix[k]->GetZaxis()->SetTitle("Z [cm]");
		RecMap_ZFix[k]->GetZaxis()->SetTitleSize(0.05);
		RecMap_ZFix[k]->GetZaxis()->SetTitleOffset(1.1);
		RecMap_ZFix[k]->GetZaxis()->SetTitleFont(52);
		RecMap_ZFix[k]->GetZaxis()->SetLabelSize(0.04);
		RecMap_ZFix[k]->GetZaxis()->SetLabelOffset(0.012);
		RecMap_ZFix[k]->GetZaxis()->SetLabelFont(42);

		RecMap_ZFix[k]->SetMinimum(fMinEMLLSignal);
		RecMap_ZFix[k]->SetMaximum(fMaxEMLLSignal);
		RecMap_ZFix[k]->SetContour(fNColourContours);
		//RecMap_ZFix[k]->Draw(fDrawMode.c_str());

	}//end loop voxel Z


	

	//## Draw volume rendering figures
	//system(fVisIVOCommandName.c_str());

	//## Convert visivo image to ROOT image
	int NVisIVOImg= 37;
	for(int i=0;i<NVisIVOImg;i++){
		TString imgName;
		if(i<10) imgName= TString("figure/") + fVisIVOVolRendImageFilePrefix + Form("000000%d.png",i);	
		else if(i>=10 && i<100) imgName= TString("figure/") + fVisIVOVolRendImageFilePrefix + Form("00000%d.png",i);	
	  else imgName= TString("figure/") + fVisIVOVolRendImageFilePrefix + Form("0000%d.png",i);	
		cout<<"imgName="<<imgName<<endl;
		TString imageOutName= Form("VolRendImg%d",i+1);
		//VolRendImg[i]= new MyTASImage(imgName);
		VolRendImg[i]= new TASImageNew(imgName);	
		VolRendImg[i]->SetPaletteEnabled(false);
		VolRendImg[i]->SetNameTitle(imageOutName,imageOutName);
	}//end loop images
}//close EMTomographyRecoThreadObj::Draw()



std::vector<double> EMTomographyRecoThreadObj::EMUpdate(std::vector<double> par, FitInfo& fitInfo,bool isAccelerated){

	//## Reset vectors
	int nPar= (int)(par.size());
	
	std::vector<double> sumS;
	sumS.clear();
	sumS.resize(0);
	sumS.assign(nPar,0.);

	std::vector< std::vector<double> > SList;
	SList.clear();
	SList.resize(0);

	/*
	std::vector< std::vector<double> > ThetaXList;
	ThetaXList.clear();
	ThetaXList.resize(0);

	std::vector< std::vector<double> > ThetaYList;
	ThetaYList.clear();
	ThetaYList.resize(0);

	std::vector< std::vector<double> > DeltaXList;
	DeltaXList.clear();
	DeltaXList.resize(0);

	std::vector< std::vector<double> > DeltaYList;
	DeltaYList.clear();
	DeltaYList.resize(0);

	std::vector< std::vector<int> > EventIdList;
	EventIdList.clear();
	EventIdList.resize(0);
	*/
	std::vector<double> parUpdate;
	parUpdate.clear();
	parUpdate.resize(0);

	
	for(int j=0;j<nPar;j++) {
		SList.push_back( std::vector<double>() );
		parUpdate.push_back(par[j]);

		//ThetaXList.push_back( std::vector<double>() );
		//ThetaYList.push_back( std::vector<double>() );
		//DeltaXList.push_back( std::vector<double>() );
		//DeltaYList.push_back( std::vector<double>() );		
		//EventIdList.push_back( std::vector<int>() );
	}

	double LogLikelihood= 0.;
	double completeLogLikelihood= 0.;
	

	//## Loop over events
	for(unsigned int i=0;i<fMuonEventDataCollection.size();i++){
		fMuonEventDataCollection[i]->SetCovarianceMatrix(par);//update covariance matrix
		fMuonEventDataCollection[i]->SetConditionalExpectation(par);//update conditional expectations
		std::vector<VoxelData> thisVoxelDataCollection= fMuonEventDataCollection[i]->fVoxelDataCollection;

		double thisThetaX= fMuonEventDataCollection[i]->fDeltaThetaX*TMath::RadToDeg();
		double thisThetaY= fMuonEventDataCollection[i]->fDeltaThetaY*TMath::RadToDeg();
		double thisDeltaX= fMuonEventDataCollection[i]->fDeltaX;
		double thisDeltaY= fMuonEventDataCollection[i]->fDeltaY;
		int thisEventId= fMuonEventDataCollection[i]->fEventId;

		for(unsigned int j=0;j<thisVoxelDataCollection.size();j++){
			int voxelId= thisVoxelDataCollection[j].fId;
			double Sij= thisVoxelDataCollection[j].fS;
			sumS[voxelId]+= Sij;	

			SList[voxelId].push_back(Sij);

			//ThetaXList[voxelId].push_back(thisThetaX);
			//ThetaYList[voxelId].push_back(thisThetaY);
			//DeltaXList[voxelId].push_back(thisDeltaX);	
			//DeltaYList[voxelId].push_back(thisDeltaY);
			//EventIdList[voxelId].push_back(thisEventId);					
		}//end loop voxels
		
		LogLikelihood+= -fMuonEventDataCollection[i]->fLikelihood;

	}//end loop events
	
	
	//## Update voxel parameters
	double rmsSum= 0.;
	double biasSum= 0.;
	double rmsThreatSum= 0.;
	double biasThreatSum= 0.;
	double rmsIterSum= 0.;
	double resErr= 0.;
	double sresErr= 0.;
	double relresErr= 0.;
	double relsresErr= 0.;
	double pixelMinRelResErr= 1.e+99;	
	double pixelMaxRelResErr= -1.e+99;	
	double pixelMean= 0.;
	double pixelVariance= 0.;

	
	
	
	for(int j=0;j<nPar;j++){
		
		int Mj= fNEventPerVoxel[j];
		double sumSij= sumS[j];
		
		//if(Mj>0) {
		if(Mj>fNMinEventInVoxelForUpdate) {
			double LambdaML= fScatteringDensityInit;
			double LambdaFinal= fScatteringDensityInit;

			std::vector<double> thisSList(SList[j].begin(),SList[j].end());
			std::sort(thisSList.begin(),thisSList.end());
			
			if(fUseFilterVoxelEvents){

				/*
				std::vector<double> thisSList= SList[j];
				std::sort(thisSList.begin(),thisSList.end());	

				//remove largest entry
				thisSList.erase(thisSList.end()-1,thisSList.end());
				
				//sum the entries
				sumSij= std::accumulate(thisSList.begin(),thisSList.end(),0);
				*/

				//OLD VERSION
				/*
				//sumSij= 0;
				//cout<<"Voxel no. "<<j<<" S("<<endl;
				//for(unsigned int l=0;l<thisSList.size()-1;l++) {
					//sumSij+= thisSList[l];
				//}
				//cout<<thisSList[thisSList.size()-1]<<")"<<endl;
				//Mj--;
				*/
			}

			if(fUseMedianUpdate) {
				std::vector<double> thisSList= SList[j];
				LambdaML= 0.5*Utilities::GetMedian(thisSList);
			}
			/*
			if(fUseQuantileUpdate){
				//fill array with ordered S values and compute quantiles
				int nentries= (int)(thisSList.size());
				double orderedSList[nentries];	
				double quantilesProb[1]= {fQuantileProb};
				double quantiles[1];
				for(int l=0;l<nentries;l++) orderedSList[l]= thisSList[l];
								
				TMath::Quantiles(nentries,1, orderedSList, quantiles, quantilesProb, kTRUE, 0, 7);
				LambdaML= 0.5*quantiles[0];
				
			}
			*/
			else{
				LambdaML= sumSij/(2.*Mj);
			}

			LambdaFinal= LambdaML;

			if(fUseRegularization){
				double Bj= fBeta/Mj;
				LambdaFinal= ShrinkageFunction(LambdaML,Bj);			
			}

			parUpdate[j]= LambdaFinal;

			/*
			fSExpHistoList[j]->Reset();
			for(int l=0;l<SList[j].size();l++) {
				fSExpHistoList[j]->Fill( log10(SList[j][l]) );
				fVoxelThetaGraphList[j]->SetPoint(l,ThetaXList[j][l],ThetaYList[j][l]);
				fVoxelDisplacementGraphList[j]->SetPoint(l,DeltaXList[j][l],DeltaYList[j][l]);

				fDataXHistoList[j]->Fill(ThetaXList[j][l],DeltaXList[j][l]);
				fDataYHistoList[j]->Fill(ThetaYList[j][l],DeltaYList[j][l]);
			}//end loop event in voxel
			*/
			

		}//end if Mj>0

		completeLogLikelihood+= -Mj*log(parUpdate[j]) - sumSij/(2.*parUpdate[j]);
		
		int copyNo= j;
		int voxelIdX = size_t( copyNo%fNVoxelX );
		int voxelIdY = size_t( (copyNo/fNVoxelX)%fNVoxelY );
		int voxelIdZ = size_t( copyNo/(fNVoxelX*fNVoxelY) );
		
		//## Calculate RMS and bias
		rmsIterSum+= pow(parUpdate[j]-par[j],2);
		resErr+= fabs(parUpdate[j]-par[j])/par[j];
		sresErr+= pow( (parUpdate[j]-par[j])/par[j] ,2);
		relresErr+= fabs(par[j]);
		relsresErr+= pow(par[j],2);

		double pixelRelUpdate= fabs(parUpdate[j]/par[j]-1);
		if(pixelRelUpdate<pixelMinRelResErr) pixelMinRelResErr= pixelRelUpdate;
		if(pixelRelUpdate>pixelMaxRelResErr) pixelMaxRelResErr= pixelRelUpdate;
		pixelMean+= parUpdate[j];		

	}//end loop voxels

	//## Add regularization likelihood term
	if(fUseRegularization){
		LogLikelihood+= -fBeta*relresErr;
	}

	
	double rmsIter= sqrt(rmsIterSum/(nPar-1));
	double residualErr= resErr;	
	double residualSquaredErr= sresErr;
	double residualRelErr= resErr/nPar;		
	double residualSquaredRelErr= sqrt(sresErr/nPar);
	pixelMean/= nPar;

	
	
	//## Calculate pixel variances
	for(int j=0;j<nPar;j++){
		pixelVariance+= pow(parUpdate[j]-pixelMean,2);
	}	
	pixelVariance/= (nPar-1);

	
	//## Save fit information
	fitInfo.fDeltaLogLikelihood= LogLikelihood-fitInfo.fLogLikelihood;
	fitInfo.fRelDeltaLogLikelihood= fabs((LogLikelihood-fitInfo.fLogLikelihood)/fitInfo.fLogLikelihood);
	fitInfo.fDeltaEMLogLikelihood= completeLogLikelihood-fitInfo.fEMLogLikelihood;	
	fitInfo.fLogLikelihood= LogLikelihood;
	fitInfo.fEMLogLikelihood= completeLogLikelihood;
	fitInfo.fChi2= -2*LogLikelihood;	
	fitInfo.fIterRMS= rmsIter;

	fitInfo.fResErr= residualErr;
	fitInfo.fSResErr= residualSquaredErr;
	fitInfo.fRelResErr= residualRelErr;
	fitInfo.fRelSResErr= residualSquaredRelErr;
	fitInfo.fPixelMinRelResErr= pixelMinRelResErr;
	fitInfo.fPixelMaxRelResErr= pixelMaxRelResErr;
	fitInfo.fPixelMean= pixelMean;
	fitInfo.fPixelVariance= pixelVariance;

	return parUpdate;

}//close EMTomographyRecoThreadObj::EMUpdate()



double EMTomographyRecoThreadObj::ShrinkageFunction(double t, double tau){

	double fval;
	if(t>=0){
		fval= (sqrt(1+4*tau*t)-1)/(2*tau);
	}
	else{
		fval= -ShrinkageFunction(-t,tau);
	}

	return fval;

}//close EMTomographyRecoThreadObj::ShrinkageFunction()

bool EMTomographyRecoThreadObj::ReadData(){

	//## Read input file line-by-line
	std::string parsedline;
	std::string descriptor;
	std::getline(fInputFileStream,parsedline);
	std::stringstream line(parsedline);
	std::stringstream line_copy(parsedline);
     
	line_copy >> descriptor;
	
	int EventId;
	const int nplanes= 4;
	double xHit[nplanes];
	double yHit[nplanes];
	double p;
  
  if(descriptor!="\n" && descriptor!=""){
		line >> EventId >> xHit[0] >> yHit[0] >> xHit[1] >> yHit[1] >> xHit[2] >> yHit[2] >> xHit[3] >> yHit[3] >> p;
	}
	else{
		cerr<<"EMTomographyRecoThreadObj::ReadData(): ERROR: Troubles in input file reading (strange characters?)..."<<endl;
		return false;
	}


	fRawData.EventId= EventId;
	fRawData.p= p;
	for(int i=0;i<nplanes;i++){
		fRawData.HitX[i]= xHit[i];
		fRawData.HitY[i]= yHit[i];
	}
	
	if(fVerbosity>2){
		cout<<"EMTomographyRecoThreadObj::ReadData(): DEBUG: EventId="<<EventId<<": "<< xHit[0] << " "<<yHit[0] <<" "<<xHit[1] <<" "<< yHit[1] <<" "<< xHit[2] <<" "<< yHit[2] <<" "<< xHit[3] <<" "<< yHit[3] <<endl;
	}
	
	return true;

}//close EMTomographyRecoThreadObj::ReadData()




QString EMTomographyRecoThreadObj::makeLogMessage(LOG_MESSAGE msg){

	//## Create the json message to be sent 
	QVariantList jsonContainer;
 	QVariantMap json;
 	json.insert("task", QVariant(msg.taskName.c_str()) );
	json.insert("command", QVariant(msg.command.c_str()) );
	json.insert("inputFileName", QVariant(msg.inputFileName.c_str()) );
	json.insert("outputFileName", QVariant(msg.outputFileName.c_str()) );
	json.insert("jobDirName", QVariant(msg.jobDirName.c_str()) );
	json.insert("scanId", QVariant(msg.scanId) );
 	json.insert("containerId", QVariant(msg.containerId.c_str()) );
	json.insert("timeStamp", QVariant( (long long)(msg.timeStamp) ) );
	json.insert("status", QVariant(msg.status) );
	json.insert("log", QVariant(msg.logMessage.c_str()) );
	json.insert("progress", QVariant(msg.progress) );
	json.insert("guiKey", QVariant(msg.guiKey.c_str()) );	
	json.insert("guihostName", QVariant(msg.guiHostName.c_str()) );	
	json.insert("guiportNo", QVariant(msg.guiPort) );
	jsonContainer << json;

 	QJson::Serializer serializer;
	serializer.setIndentMode(QJson::IndentCompact);
 	bool ok;
 	QByteArray data = serializer.serialize(jsonContainer, &ok);
	data.append("#END#");

	return QString(data);

}//close EMTomographyRecoThreadObj::makeLogMessage()


QString EMTomographyRecoThreadObj::makeAlarmMessage(ALARM_MESSAGE msg){

	//## Create the json message to be sent 
	int nAlarmVoxels= (msg.alarmVoxelList).size();

	QVariantList jsonContainer;
	QVariantList voxelIdListContainer;

 	QVariantMap json;
 	json.insert("task", QVariant(msg.taskName.c_str()) );
	json.insert("command", QVariant(msg.command.c_str()) );
	json.insert("inputFileName", QVariant(msg.inputFileName.c_str()) );
	json.insert("outputFileName", QVariant(msg.outputFileName.c_str()) );
	json.insert("jobDirName", QVariant(msg.jobDirName.c_str()) );
	json.insert("scanId", QVariant(msg.scanId) );
 	json.insert("containerId", QVariant(msg.containerId.c_str()) );
	json.insert("timeStamp", QVariant( (long long)(msg.timeStamp) ) );
	json.insert("status", QVariant(msg.status) );
	json.insert("log", QVariant(msg.alarmMessage.c_str()) );
		
	for(unsigned int j=0;j<nAlarmVoxels;j++){
		voxelIdListContainer << QVariant(msg.alarmVoxelList[j]);
	}
	json.insert("voxelId",voxelIdListContainer);

	json.insert("guiKey", QVariant(msg.guiKey.c_str()) );	
	json.insert("guihostName", QVariant(msg.guiHostName.c_str()) );	
	json.insert("guiportNo", QVariant(msg.guiPort) );
	jsonContainer << json;

 	QJson::Serializer serializer;
	serializer.setIndentMode(QJson::IndentCompact);
 	bool ok;
 	QByteArray data = serializer.serialize(jsonContainer, &ok);
	data.append(Gui::SOCK_END_PATTERN.c_str());

	return QString(data);

}//close EMTomographyRecoThreadObj::makeAlarmMessage()




bool EMTomographyRecoThreadObj::sender(QString message){

	
	bool status= true;	
	QByteArray data;
	data.append(message);
	
	//## Create the tcp socket for communication
	socket= new QTcpSocket(this);
	connect(socket, SIGNAL(connected()),this, SLOT(connected()));
	socket->connectToHost(QString::fromStdString(fGuiHostName),fGuiPort);	
	

	//## Send message	
	if( socket->waitForConnected() ) {
		socket->write(data);
		socket->flush();
		socket->waitForBytesWritten(3000);
  }
	
	socket->close();
	
	return status;

}//close EMTomographyRecoThreadObj::sender()



double EMTomographyRecoThreadObj::GetMaterialScatteringDensity(std::string name){

	
	G4Material* material= 0;
	//standard
	if(name=="Air") {
		material= G4Material::GetMaterial("Air");
	}	
	else if(name=="Water") {
		material= G4Material::GetMaterial("Water");
	}
	else if(name=="PVC") {
		material= G4Material::GetMaterial("PVC");
	}	
	else if(name=="Aluminium") {
		material= G4Material::GetMaterial("Aluminium");
	}
	else if(name=="Silicon") {
		material= G4Material::GetMaterial("Silicon");
	}
	else if(name=="Copper") {
		material= G4Material::GetMaterial("Copper");
	}
	else if(name=="Nickel") {
		material= G4Material::GetMaterial("Nickel");
	}	
	else if(name=="Zinc") {
		material= G4Material::GetMaterial("Zinc");
	}
	else if(name=="Iron") {
		material= G4Material::GetMaterial("Iron");
	}
	else if(name=="Lead") {
		material= G4Material::GetMaterial("Lead");
	}	
	else if(name=="Tungsten") {
		material= G4Material::GetMaterial("Tungsten");
	}		
	else if(name=="Silver") {
		material= G4Material::GetMaterial("Silver");
	}	
	else if(name=="Platinum") {
		material= G4Material::GetMaterial("Platinum");
	}	
	else if(name=="Gold") {
		material= G4Material::GetMaterial("Gold");
	}	
	else if(name=="Concrete") {
		material= G4Material::GetMaterial("Concrete");
	}	
	else if(name=="Tin") {
   	material= G4Material::GetMaterial("Tin");
  }


	//radioactive
	else if(name=="Technetium") {
		material= G4Material::GetMaterial("Technetium");
	}
	else if(name=="Promethium") {
		material= G4Material::GetMaterial("Promethium");
	}
	else if(name=="Polonium") {
		material= G4Material::GetMaterial("Polonium");
	}	
	else if(name=="Astatine") {
		material= G4Material::GetMaterial("Astatine");
	}
	else if(name=="Radon") {
		material= G4Material::GetMaterial("Radon");
	}
	else if(name=="Francium") {
		material= G4Material::GetMaterial("Francium");
	}
	else if(name=="Radium") {
	 material= G4Material::GetMaterial("Radium");
	}
	else if(name=="Actinium") {
		material= G4Material::GetMaterial("Actinium");
	}
	else if(name=="Thorium") {
		material= G4Material::GetMaterial("Thorium");
	}
	else if(name=="Protactinium") {
		material= G4Material::GetMaterial("Protactinium");
	}
	else if(name=="Uranium") {
		material= G4Material::GetMaterial("Uranium");
	}
	else if(name=="Neptunium") {
		material= G4Material::GetMaterial("Neptunium");
	}
	else if(name=="Plutonium") {
		material= G4Material::GetMaterial("Plutonium");
	}
	else if(name=="Americium") {
		material= G4Material::GetMaterial("Americium");
	}
	else if(name=="Curium") {
		material= G4Material::GetMaterial("Curium");
	}
	else if(name=="Berkelium") {
		material= G4Material::GetMaterial("Berkelium");
	}
	else if(name=="Californium") {
		material= G4Material::GetMaterial("Californium");
	}
	else{
		cerr<<"EMTomographyRecoThreadObj::GetMaterialScatteringDensity(): ERROR: Cannot get material "<<name<<" ...exit!"<<endl;
		exit(1);
	}
	
	double radLength= material->GetRadlen()/CLHEP::cm;//in cm
	double lambda= pow(15.e-3/fReferenceMomentum,2)/radLength;//in rad^2/cm
	//lambda*= 1.e+6;//in mrad^2/cm
	//cout<<"TomographyImageReco::GetMaterialScatteringDensity(): INFO: radLength[cm]="<<radLength<<endl;
	//cout<<"TomographyImageReco::GetMaterialScatteringDensity(): INFO: Lambda[rad^2/cm]="<<lambda<<endl;

	return lambda;

}//close EMTomographyRecoThreadObj::GetMaterialScatteringDensity()

void EMTomographyRecoThreadObj::DumpTotalLoad(){

	cout<<"EMTomographyRecoThreadObj::DumpTotalLoad(): INFO: CPU total load..."<<endl;
	
	double initLoad= Utilities::TimeToSec(initCPUTime)/Utilities::TimeToSec(globalCPUTime)*100.; 
	double collectDataLoad= Utilities::TimeToSec(collectDataCPUTime)/Utilities::TimeToSec(globalCPUTime)*100.;  
	double imageRecoLoad= Utilities::TimeToSec(imageRecoCPUTime)/Utilities::TimeToSec(globalCPUTime)*100.; 
	
	fNTotEvents= max((double)fNMaxRecEvents,(double)(fNEvents));
	fCPUTime_Tot= Utilities::TimeToSec(globalCPUTime);
	fCPUTime_Init= Utilities::TimeToSec(initCPUTime);
	fCPUTime_DataCollect= Utilities::TimeToSec(collectDataCPUTime);
	fCPUTime_ImageReco= Utilities::TimeToSec(imageRecoCPUTime);	
	fCPUTimeInfoTree->Fill();

	cout<<"==============   TOT   =================="<<endl;
	cout<<"TOT: "<<globalCPUTime.tv_sec<<" s "<<globalCPUTime.tv_nsec<<" ns"<<endl;
	cout<<"INIT ["<<initLoad<<"%]: "<<initCPUTime.tv_sec<<" s "<<initCPUTime.tv_nsec<<" ns"<<endl;
	cout<<"DATA COLLECT ["<<collectDataLoad<<"%]: "<<collectDataCPUTime.tv_sec<<" s "<<collectDataCPUTime.tv_nsec<<" ns"<<endl;
	cout<<"IMAGE RECO ["<<imageRecoLoad<<"%]: "<<imageRecoCPUTime.tv_sec<<" s "<<imageRecoCPUTime.tv_nsec<<" ns"<<endl;
	cout<<"========================================="<<endl;
	cout<<endl;	


}//close EMTomographyRecoThreadObj::DumpTotalLoad()


void EMTomographyRecoThreadObj::Save(){

	cout<<"EMTomographyRecoThreadObj::Save(): INFO: Saving results to file ..."<<endl;
	fOutputFile->cd();

	fScatteringDataInfo->Write();
	fRecTomographyImageInfo->Write();
	fIterationInfo->Write();
	fCPUTimeInfoTree->Write();
	fInfoTree->Write();

	RecMap3D->Write();
	NEventMap3D->Write();
	for(int k=0;k<fNVoxelX;k++) {
		RecMap_XFix[k]->Write();
		NEventMap_XFix[k]->Write();
	}
	for(int k=0;k<fNVoxelY;k++) {
		RecMap_YFix[k]->Write();
		NEventMap_YFix[k]->Write();
	}
	for(int k=0;k<fNVoxelZ;k++) {	
		RecMap_ZFix[k]->Write();
		NEventMap_ZFix[k]->Write();
	}
	/*
	for(int k=0;k<fNVoxel;k++) {
		fSExpHistoList[k]->Write();
		fDataXHistoList[k]->Write();
		fDataYHistoList[k]->Write();
		//fVoxelThetaGraphList[k]->Write();
		//fVoxelDisplacementGraphList[k]->Write();
	}
	*/
	int NVisIVOImg= 37;
	for(int i=0;i<NVisIVOImg;i++){
		VolRendImg[i]->Write();
	}//end loop images

	fOutputFile->Close();

}//close EMTomographyRecoThreadObj::Save()


void EMTomographyRecoThreadObj::SetGraphicsStyle(){

	myStyle= new TStyle("myStyle","left centered");
	myStyle->SetCanvasDefH(700); 
  myStyle->SetCanvasDefW(700); 
	//myStyle->SetCanvasPreferGL(1);

	myStyle->SetFrameBorderMode(0);
	myStyle->SetCanvasBorderMode(0);
  myStyle->SetPadBorderMode(0);
  myStyle->SetPadColor(0);
  myStyle->SetCanvasColor(kWhite);
  myStyle->SetTitleFillColor(0);
  myStyle->SetTitleBorderSize(1);
  myStyle->SetStatColor(0);
  myStyle->SetStatBorderSize(1);
  //myStyle->SetOptTitle(0);
  myStyle->SetOptStat(0);
  myStyle->SetOptFit(0);
  //myStyle->SetPalette(90,0);
  myStyle->SetTitleBorderSize(0);//border size of Title PavelLabel
  myStyle->SetTitleX(0.1f);
	myStyle->SetTitleW(0.8f);
  myStyle->SetStatY(0.975);                
  myStyle->SetStatX(0.95);                
  myStyle->SetStatW(0.2);                
  myStyle->SetStatH(0.15);                
  myStyle->SetTitleXOffset(1);
  myStyle->SetTitleYOffset(1);
  //myStyle->SetMarkerStyle(20);
  //myStyle->SetMarkerSize(0.5);
  myStyle->SetFuncWidth(1.);
  myStyle->SetPadTopMargin(0.08);
  myStyle->SetPadBottomMargin(0.17);
  //myStyle->SetPadLeftMargin(0.15);
  //myStyle->SetPadRightMargin(0.15);
	myStyle->SetPadLeftMargin(0.15);
  myStyle->SetPadRightMargin(0.17);	
  myStyle->SetTitleSize(0.06,"X");
  myStyle->SetTitleSize(0.06,"Y");
	myStyle->SetTitleFont(52,"X");
	myStyle->SetTitleFont(52,"Y");
	myStyle->SetTitleFont(52,"Z");
	myStyle->SetTitleFont(42,"X");
	myStyle->SetTitleFont(42,"Y");
	myStyle->SetTitleFont(42,"Z");
  myStyle->SetErrorX(0.);
	myStyle->cd();

	//gStyle->SetPalette(1);
	gStyle->SetOptLogz(1);
	gStyle->SetTitleFont(52,"X");
	gStyle->SetTitleFont(52,"Y");
	gStyle->SetTitleFont(52,"Z");
	gStyle->SetTitleFont(42,"X");
	gStyle->SetTitleFont(42,"Y");
	gStyle->SetTitleFont(42,"Z");
	//gStyle->SetHistTopMargin(0);

	static const UInt_t Number = 12;
  static Double_t Red[Number]    = { 0.164, 0.150, 0.250, 0.450, 0.670, 0.880, 1.000, 1.000,1.000,0.970,0.850,0.650};
  static Double_t Green[Number]  = { 0.043, 0.306, 0.630, 0.853, 0.973, 1.000,1.000,0.880,0.679,0.430,0.150,0.000};
  static Double_t Blue[Number]   = { 0.850,1.000,1.000,1.000,1.000,1.000,0.750,0.600,0.450,0.370,0.196,0.130};
	double Length[Number] = { 0.,0.2,0.3,0.4,0.5,0.7,0.75,0.8,0.85,0.9,0.95,1};// viola, blu,celeste,verde,giallo,rosso
	int FI = TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,fNColourContours);
	int colorList[fNColourContours];
	for (int i=0;i<fNColourContours;i++) {
		colorList[i] = FI+i;
	}

	fTemperaturePalette= new TImagePalette(fNColourContours,colorList);

}//close function

}//close namespace
