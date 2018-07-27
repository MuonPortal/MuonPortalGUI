// ******************************************************************************
// * License and Disclaimer                                                     *
// *                                                                            *
// * Copyright 2018 Marilena Bandieramonte, S. Riggi									          *
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
* @file FOFThreadObj.cc
* @class FOFThreadObj
* @brief Friend-of-friends (FOF) algorithm processing thread
*
* @author M. Bandieramonte, S. Riggi
* @date 21/02/2012
*/

#include <FOFThreadObj.h>
#include <ConfigParser.h>
#include <Logger.h>
#include <TASImageNew.h>

#include <Gui.h>
#include <Utilities.h>
#include <QtGui>
#include <QString>

#include <QTcpSocket>
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

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1D.h>
#include <TF1.h>
#include <TH2.h>
#include <TH3.h>
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
#include <TMath.h>
#include <TDirectory.h>
#include <TStyle.h>
#include <TPolyLine3D.h>
#include <TPolyMarker3D.h>
#include <TPaveText.h>
#include <TVirtualFitter.h>
#include <TObjArray.h>

#include <TMatrixD.h>
#include <TColor.h>
#include <TApplication.h>
#include <TVector3.h>
#include <TView3D.h>
#include <TMarker.h>
#include <TPaletteAxis.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdexcept>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <assert.h>


using namespace std;

namespace MuonPortalNS {


FOFThreadObj::FOFThreadObj() {

	//## Connect status signal to socket sender
	connect(this,SIGNAL(logSig(QString)), this, SLOT(sender(QString)) );
	connect(this,SIGNAL(alarmSig(QString)), this, SLOT(sender(QString)) );

}//close constructor
  
   
FOFThreadObj::~FOFThreadObj() {


}//close destructor


void FOFThreadObj::SetConfig(){

	fVerbosity= ConfigParser::fClusteringVerbosity;
	fWaitPOCAOutput= ConfigParser::fClusteringWaitPOCAOutput;	
	fUseMaxNEvents= ConfigParser::fUseClusteringMaxNEvents;	
	fMaxNEvents= ConfigParser::fClusteringMaxNEvents;	
	fUseThetaCut= ConfigParser::fUseClusteringThetaCut;	
	fThetaMin= ConfigParser::fClusteringThetaMin;
	fThetaMax= ConfigParser::fClusteringThetaMax;
	fUseEnergyCut= ConfigParser::fUseClusteringEnergyCut;
	fEnergyMinCut= ConfigParser::fClusteringEnergyMin;
	fEnergyMaxCut= ConfigParser::fClusteringEnergyMax;
	fUseThetaWeight= ConfigParser::fUseClusteringThetaWeight;	
	fThetaWeight= ConfigParser::fClusteringThetaWeight;
	fSignalThreshold= ConfigParser::fClusteringSignalThreshold;
	fNEventThreshold= ConfigParser::fClusteringNEventThreshold;
	fUseAverageSignal= ConfigParser::fUseClusteringAverageSignal;
	fEps= ConfigParser::fClusteringEpsilon;
	fMinNPts= ConfigParser::fClusteringMinNPts;

	
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

	nBucket = 16;
	nMembers = 8;
  bPoca= 1;
	bVerbose = 1;
	for (int j=0;j<3;j++) fPeriod[j] = HUGE;

	fIsFailure= false;

}//close FOFThreadObj::SetConfig()

bool FOFThreadObj::Init(){

	
	//## Set config
	SetConfig();

	//## Open data files
	INFO_LOG("Opening input data file "<<fInputFileName.c_str()<<" ...");
	fInputFile= new TFile(fInputFileName.c_str(),"READ");	
	if(!fInputFile || fInputFile->IsZombie() ){
		QString errmsg = QString("--> FOF Image Reco: ERROR: Cannot open input file ");
		errmsg+= QString::fromStdString(fInputFileName);
		errmsg+= QString::fromStdString(std::string(" ... exit!"));
		emit(statusMessage(errmsg));
		emit error();

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= errmsg.toStdString();
		fLogMessage.progress= (int)(progressPercentage);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);

		fIsFailure= true;
		return false;
	}

	//## Set output data
	fOutputFile= new TFile(fOutputFileName.c_str(),"RECREATE");

	fDataTree= new TTree;	
	fDataTree->Branch("EventId",&fEventId,"EventId/I");		
	fDataTree->Branch("ScatteringAngle",&fEventWeight,"ScatteringAngle/D");
	fDataTree->Branch("POCAX",&fPOCAX,"POCAX/D");
	fDataTree->Branch("POCAY",&fPOCAY,"POCAY/D");
	fDataTree->Branch("POCAZ",&fPOCAZ,"POCAZ/D");
	
	fInfoTree= new TTree("InfoRun","InfoRun");
	fInfoTree->Branch("nClusters",&fNClusterGroups,"nClusters/I");
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

	fClusterInfoTree= new TTree("ClusterInfo","ClusterInfo");
	fClusterInfoTree->Branch("ClusterId",&fClusterId,"ClusterId/I");
	fClusterInfoTree->Branch("EventId",&fEventId,"EventId/I");	
	fClusterInfoTree->Branch("X",&fPOCAX,"X/D");
	fClusterInfoTree->Branch("Y",&fPOCAY,"Y/D");
	fClusterInfoTree->Branch("Z",&fPOCAZ,"Z/D");
	fClusterInfoTree->Branch("Theta",&fTheta,"Theta/D");
	
	RecMap3D= new TH3D("RecMap3D","RecMap3D",fNVoxelX,-fContainerSizeX/2,fContainerSizeX/2,fNVoxelY,-fContainerSizeY/2,fContainerSizeY/2,fNVoxelZ,-fContainerSizeZ/2,fContainerSizeZ/2);
	RecMap3D->Sumw2();

	NEventMap3D= new TH3D("NEventMap3D","NEventMap3D",fNVoxelX,-fContainerSizeX/2,fContainerSizeX/2,fNVoxelY,-fContainerSizeY/2,fContainerSizeY/2,fNVoxelZ,-fContainerSizeZ/2,fContainerSizeZ/2);
	NEventMap3D->Sumw2();

	fCurrentDirectory= std::string(qPrintable(QDir::currentPath())) + std::string("/");
	fInputFileName_noPath= fInputFileName.substr( fInputFileName.find_last_of("\\/")+1 );
	fInputFileName_noPath= fInputFileName_noPath.substr(0,fInputFileName_noPath.find_last_of(".") ) + std::string(".dat");	
	fInputFileNamePath = fInputFileName.substr( 0, fInputFileName.find_last_of("\\/")+1 );

	fVisIVOPointOutputFileName= fCurrentDirectory + std::string("FOFPoint-VisIVO_") + fInputFileName_noPath;
	fVisIVOPointOutputFileName_noPath= fVisIVOPointOutputFileName.substr( fVisIVOPointOutputFileName.find_last_of("\\/")+1 );
	fVisIVOPointImageFilePrefix= std::string("VisIVOPoint_") + fVisIVOPointOutputFileName_noPath;
	fVisIVOCommandName= fCurrentDirectory + std::string("/scripts/VisIVOPointImaging.sh ") + fVisIVOPointOutputFileName_noPath + TString(" ") + fCurrentDirectory + std::string("/figure/"); 	
	DEBUG_LOG("VisIVO cmd="<<fVisIVOCommandName);

	pointList.clear();
	pointList.resize(0);
	angleList.clear();
	angleList.resize(0);
	fAlarmClusterIdList.clear();
	fAlarmClusterIdList.resize(0);

	//## FOF init
	kdInit(&kd,nBucket,fPeriod);
	
	return true;

}//close FOFThreadObj::Init()


bool FOFThreadObj::ReadData(){

	DEBUG_LOG("Start reading data ...");

	double POCAX, POCAY, POCAZ, ScatteringAngle, KinEnergy;
	int EventId;

	TTree* Data= (TTree*)fInputFile->Get("data");
	if(!Data || Data->IsZombie()){
		std::string errMsg("Cannot open data tree...exit!");
		ERROR_LOG(errMsg);
		QString errmsg = QString(errMsg.c_str());
		emit(statusMessage(errmsg));
		emit error();

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= errmsg.toStdString();
		fLogMessage.progress= (int)(progressPercentage);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);

		fIsFailure= true;
		return false;
	}

	Data->SetBranchAddress("EventId",&EventId);
	Data->SetBranchAddress("POCAX",&POCAX);
	Data->SetBranchAddress("POCAY",&POCAY);
	Data->SetBranchAddress("POCAZ",&POCAZ);
	Data->SetBranchAddress("ScatteringAngle",&ScatteringAngle);
	Data->SetBranchAddress("KinEnergy",&KinEnergy);
		
	INFO_LOG("Reading data ...");
	
	fNSelEvents= 0;

	for(int i=0;i<Data->GetEntries();i++){
		Data->GetEntry(i);

		if(i%100000==0) DEBUG_LOG("--> "<<i<<"/"<<Data->GetEntries()<<" entries read...");
		
		if(fUseThetaCut && (ScatteringAngle<fThetaMin || ScatteringAngle>fThetaMax) ) continue;
		if(fUseEnergyCut && (KinEnergy<fEnergyMinCut || KinEnergy>fEnergyMaxCut) ) continue;
		if(POCAX<fContainerXStart || POCAX>fContainerXEnd) continue;
		if(POCAY<fContainerYStart || POCAY>fContainerYEnd) continue;
		if(POCAZ<fContainerZStart || POCAZ>fContainerZEnd) continue;
		
		pointList.push_back(TVector3(POCAX,POCAY,POCAZ));
		angleList.push_back(ScatteringAngle);
		eventIdList.push_back(EventId);
	
		fNSelEvents++;
	}//end loop entries

	DEBUG_LOG("--> NData="<<Data->GetEntries()<<"  fNSelEvents="<<fNSelEvents);


	struct dump h; //no. events
	struct poca_event pe_p; //id_ev, 3 coordinate, theta, thetaQ e energy
  
  kd->bPoca= bPoca; 
	kd->nPoca = fNSelEvents;
	kd->nActive = 0;
	if (bPoca) kd->nActive += kd->nPoca; //nActive ha come valore il numero di eventi nel file
	kd->p = (POCA*)malloc(kd->nActive*sizeof(POCA)); //sto allocando la memoria per tutti gli eventi
	assert(kd->p != NULL);
	
	  
	for(int i=0;i<pointList.size();i++){
		kd->p[i].iOrder = i; 
		kd->p[i].r[0] = pointList[i].X();
    kd->p[i].r[1] = pointList[i].Y();
    kd->p[i].r[2] = pointList[i].Z();
    kd->p[i].angle = angleList[i];
		kd->p[i].eventId = eventIdList[i];
	}//end loop events


	return true;

}//close FOFThreadObj::ReadData()


bool FOFThreadObj::Run(){

  INFO_LOG("Start FOF clustering run ...");
	
	//## Build KD tree
	kdBuildTree(kd);

	//## Perform the clustering
  fNClusterGroups = kdFoF(kd,fEps); //mb:fEps è un float non inizializzato se non dai l'opzione -e ---> linking lenght
  INFO_LOG("#"<<fNClusterGroups<<" clusters found ...");
    
	if (bVerbose) DEBUG_LOG("Number of initial groups: "<< nGroup);  //mb:qui no
	fNClusterGroups = kdTooSmall(kd,fMinNPts);
  INFO_LOG("#"<<fNClusterGroups<<" clusters found after kdTooSmall stage...");
  
	//## Order clusters
	kdOrder(kd);
	
	//## Fill cluster info
	fInfoTree->Fill(); 

	double NPtsPerCluster[fNClusterGroups];
	double SumWeight[fNClusterGroups];
	for(int i=0;i<fNClusterGroups;i++){
		NPtsPerCluster[i]= 0;
		SumWeight[i]= 0;
	}

	for(int i=0;i<kd->nPoca;i++) {		
		fClusterId= kd->p[i].iGroup;
		if(fClusterId<=0) continue;
		
		fPOCAX= pointList[i].X();
		fPOCAY= -pointList[i].Y();
		fPOCAZ= pointList[i].Z();
		fTheta= angleList[i];	
		fEventId= eventIdList[i];
		fClusterInfoTree->Fill();

		NPtsPerCluster[fClusterId-1]++;
		SumWeight[fClusterId-1]+= pow(fTheta,fThetaWeight);	
	}//end loop events

	bool isAlarm= false;

	for(int i=0;i<fNClusterGroups;i++){
		if(NPtsPerCluster[i]>0) SumWeight[i]/= NPtsPerCluster[i];
		else SumWeight[i]= 0;

		if(SumWeight[i]>fSignalThreshold && NPtsPerCluster[i]>fNEventThreshold) {
			isAlarm= true;
			fAlarmClusterIdList.push_back(i);
		}

	}//end loop clusters

	QString alarmMsg = QString("0");
	if(isAlarm) {
		emit alarm();

		alarmMsg= QString("1");
		emit(alarmStatusMessage(alarmMsg));

		fAlarmMessage.status= eRunning;
		fAlarmMessage.alarmClusterList= fAlarmClusterIdList;
		fAlarmMessage.alarmMessage= Form("--> ALARM (%d alarm clusters)!!!",(int)(fAlarmClusterIdList.size()));
		
		QString alarmMessage= makeAlarmMessage(fAlarmMessage);
		emit alarmSig(alarmMessage);
	}


	
	
	FILE* fout_point;
	fout_point= fopen(fVisIVOPointOutputFileName.c_str(),"w");
	fprintf(fout_point,"X Y Z W\n");
		
	double minw= 1.e+99;
	double maxw= -1.e+99;
	
	for(int i=0;i<kd->nPoca;i++) {	
		fClusterId= kd->p[i].iGroup;
		if(fClusterId<=0) continue;

		
		fPOCAX= pointList[i].X();
		fPOCAY= pointList[i].Y();
		fPOCAZ= pointList[i].Z();
		fTheta= angleList[i];	
		fEventId= eventIdList[i];
		
		//double w= SumWeight[fClusterId-1];
		double w= pow(fTheta,fThetaWeight);	

		
		//int gBin= RecMap3D->FindBin(fPOCAX,fPOCAY,fPOCAZ);
		//RecMap3D->SetBinContent(gBin,w);
		RecMap3D->Fill(fPOCAX,-fPOCAY,fPOCAZ,w);
		NEventMap3D->Fill(fPOCAX,-fPOCAY,fPOCAZ,1);

		fEventWeight= w;
		fDataTree->Fill();

		fprintf(fout_point,"%f  %f  %f  %f\n",fPOCAX,fPOCAY,fPOCAZ,w);
	
		if(w>maxw) maxw= w;
		if(w<minw) minw= w;
		
	}//end loop events

	//add vertex points to draw box in Visivo
	fprintf(fout_point,"%f  %f  %f  %f\n",fContainerXStart,fContainerYStart,fContainerZStart,minw);
	fprintf(fout_point,"%f  %f  %f  %f\n",fContainerXEnd,fContainerYEnd,fContainerZEnd,minw);

	fclose(fout_point);

	//## Set to zero bins below Nevent threshold and emit alarm
	

	for(int k=0;k<RecMap3D->GetNbinsZ();k++){
		for(int j=0;j<RecMap3D->GetNbinsY();j++){
			for(int i=0;i<RecMap3D->GetNbinsX();i++){		
				double binContent= RecMap3D->GetBinContent(i+1,j+1,k+1);
				double N= NEventMap3D->GetBinContent(i+1,j+1,k+1);
				if(N<fNEventThreshold) RecMap3D->SetBinContent(i+1,j+1,k+1,0.);	
				//if(binContent>fSignalThreshold && N>fNEventThreshold) isAlarm= true;
			}//end loop bins x
		}//end loop bins y
	}//end loop bins z
	
	//## Rescale map? Compute min & max POCA signal
	if(fUseAverageSignal){
		RecMap3D->Divide(NEventMap3D);
	}
	
	//## Finish kd
	kdFinish(kd);

	return true;
    
}//close Run()


void FOFThreadObj::process(){

	//## Set message 
	fLogMessage.taskName= "FOF";
	fLogMessage.command= "LOG";	
	fLogMessage.inputFileName= fInputFileName;		
	fLogMessage.outputFileName= fOutputFileName;	
	fLogMessage.jobDirName= fDestinationPath;
	fLogMessage.scanId= fScanId;
	fLogMessage.containerId= fContainerId;
	fLogMessage.timeStamp= fTimeStamp;			
	fLogMessage.status= eRunning;
	fLogMessage.logMessage= "FOF imaging started...";
	fLogMessage.progress= 0;
	fLogMessage.guiKey= fGuiKey;
	fLogMessage.guiHostName= fGuiHostName;
	fLogMessage.guiPort= fGuiPort;

	fAlarmMessage.taskName= "FOF";
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
	QString msg= "";

	//Init status progress
	progressPercentage= 0;
	nRecStages= 5;//Init + ReadData + Run + Draw + Save
	stagePercentage= 1./(double)(nRecStages)*100.;
	fIsFailure= false;

	
	msg = QString("--> FOF Image Reco: Input data file ");
	msg+= QString::fromStdString(fInputFileName);
	msg+= QString::fromStdString(std::string(" ..."));
	emit(statusMessage(msg));

	emit(statusProgress((int)(progressPercentage)));

	fLogMessage.status= eRunning;
	fLogMessage.logMessage= msg.toStdString();
	fLogMessage.progress= (int)(progressPercentage);

	logMessage= makeLogMessage(fLogMessage);
	emit logSig(logMessage);

	//========================================
	//==        INIT DATA STAGE
	//========================================
	//## Init
	bool init_status= Init();
	
	if(!init_status){
		std::string errMsg("FOF clustering initialization stage failed!");
		ERROR_LOG(errMsg);
		msg = QString(errMsg.c_str());
		emit(statusMessage(msg));

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= 0;

		logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
		
		return;
	}//close if
	else{
		std::string infoMsg("FOF clustering initialization stage completed!");
		INFO_LOG(infoMsg);
		msg = QString(infoMsg.c_str());
		emit(statusMessage(msg));

		progressPercentage+= stagePercentage;
		emit(statusProgress((int)(progressPercentage)));

		fLogMessage.status= eRunning;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= (int)(progressPercentage);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}

	//========================================
	//==        READ DATA STAGE
	//========================================
	//## Read data
	bool read_status= ReadData();
	
	if(!read_status){
		std::string errMsg("FOF clustering data reading stage failed!");
		ERROR_LOG(errMsg);
		msg = QString(errMsg.c_str());
		emit(statusMessage(msg));

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= 0;

		logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
		
		return;
	}//close if
	else{
		std::string infoMsg("FOF clustering data reading stage completed!");
		INFO_LOG(infoMsg);
		msg = QString(infoMsg.c_str());
		emit(statusMessage(msg));

		progressPercentage+= stagePercentage;
		emit(statusProgress((int)(progressPercentage)));

		fLogMessage.status= eRunning;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= (int)(progressPercentage);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}

	//========================================
	//==        RUN STAGE
	//========================================
	//## Clustering calculation
	bool run_status= Run();
	
	if(!run_status){
		std::string errMsg("FOF clustering run stage failed!");
		ERROR_LOG(errMsg);
		msg = QString(errMsg.c_str());
		emit(statusMessage(msg));

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= 0;

		logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
		
		return;
	}//close if
	else{
		std::string infoMsg("FOF clustering run stage completed!");
		INFO_LOG(infoMsg);
		msg = QString(infoMsg.c_str());
		emit(statusMessage(msg));

		progressPercentage+= stagePercentage;
		emit(statusProgress((int)(progressPercentage)));

		fLogMessage.status= eRunning;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= (int)(progressPercentage);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}


	//========================================
	//==        DRAW STAGE
	//========================================
	//## Draw
	bool draw_status= Draw();	
	
	if(!draw_status){
		std::string errMsg("FOF clustering draw stage failed!");
		ERROR_LOG(errMsg);
		msg = QString(errMsg.c_str());
		emit(statusMessage(msg));

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= 0;

		logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
		
		return;
	}//close if
	else{
		std::string infoMsg("FOF clustering draw stage completed!");
		INFO_LOG(infoMsg);
		msg = QString(infoMsg.c_str());
		emit(statusMessage(msg));
		
		progressPercentage+= stagePercentage;
		emit(statusProgress((int)(progressPercentage)));

		fLogMessage.status= eRunning;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= progressPercentage;

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}

	//========================================
	//==        SAVE TO FILE STAGE
	//========================================
	//## Save
	bool save_status= Save();
	
	if(!save_status){
		std::string errMsg("FOF clustering save to file stage failed!");
		ERROR_LOG(errMsg);
		msg = QString(errMsg.c_str());
		emit(statusMessage(msg));

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= 0;

		logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
		
		return;
	}//close if
	else{
		std::string infoMsg("FOF clustering save to file stage completed!");
		INFO_LOG(infoMsg);
		msg = QString(infoMsg.c_str());
		emit(statusMessage(msg));
		
		progressPercentage+= stagePercentage;
		emit(statusProgress(max((int)(progressPercentage),100)));

		fLogMessage.status= eRunning;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= max((int)(progressPercentage),100);

		logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}

	INFO_LOG("FOF clustering terminated with success!");
	emit finished();	
	msg = QString("--> FOF Image Reco: Terminated with success!");
	emit(statusMessage(msg));	

	fLogMessage.status= eFinished;
	fLogMessage.logMessage= msg.toStdString();
	fLogMessage.progress= progressPercentage;

	logMessage= makeLogMessage(fLogMessage);
	emit logSig(logMessage);

}//close FOFThreadObj::process()


bool FOFThreadObj::Draw(){

	//## Draw volume rendering figures
	//system(fVisIVOCommandName.c_str());

	//## Convert visivo image to ROOT image
	int NVisIVOImg= 37;
	for(int i=0;i<NVisIVOImg;i++){
		TString imgName;
		if(i<10) imgName= TString("figure/") + fVisIVOPointImageFilePrefix + Form("000000%d.png",i);	
		else if(i>=10 && i<100) imgName= TString("figure/") + fVisIVOPointImageFilePrefix + Form("00000%d.png",i);	
	  else imgName= TString("figure/") + fVisIVOPointImageFilePrefix + Form("0000%d.png",i);	
		TString imageOutName= Form("PointImg%d",i+1);
		//PointImg[i]= new MyTASImage(imgName);
		PointImg[i]= new TASImageNew(imgName);
		PointImg[i]->SetPaletteEnabled(false);
		PointImg[i]->SetNameTitle(imageOutName,imageOutName);
	}//end loop images

	return true;

}//close FOFThreadObj::Draw()


bool FOFThreadObj::Save(){

	fOutputFile->cd();
	fInfoTree->Write();
	fClusterInfoTree->Write();
	fDataTree->SetNameTitle("data","data");
	fDataTree->Write();
	RecMap3D->Write();
	NEventMap3D->Write();

	int NVisIVOImg= 37;
	for(int i=0;i<NVisIVOImg;i++){
		PointImg[i]->Write();
	}//end loop images

	fOutputFile->Close();

	return true;

}//close FOFThreadObj::Save()



QString FOFThreadObj::makeLogMessage(LOG_MESSAGE msg){

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

}//close FOFThreadObj::makeLogMessage()


QString FOFThreadObj::makeAlarmMessage(ALARM_MESSAGE msg){

	//## Create the json message to be sent 
	int nAlarmVoxels= (msg.alarmClusterList).size();

	QVariantList jsonContainer;
	QVariantList clusterIdListContainer;

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
		clusterIdListContainer << QVariant(msg.alarmClusterList[j]);
	}
	json.insert("clusterId",clusterIdListContainer);

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

}//close FOFThreadObj::makeAlarmMessage()




bool FOFThreadObj::sender(QString message){

	
	bool status= true;	
	QByteArray data;
	data.append(message);
	
	//## Create the tcp socket for communication
	INFO_LOG("Creating a tcp client communicating with host "<<fGuiHostName<<" on port "<<fGuiPort<<" ...");
	socket= new QTcpSocket(this);
	connect(socket, SIGNAL(connected()),this, SLOT(connected()));
	socket->connectToHost(QString::fromStdString(fGuiHostName),fGuiPort);	
	

	//## Send message	
	if( socket->waitForConnected() ) {
		DEBUG_LOG("Send this message via socket: "<<qPrintable(message));
  	socket->write(data);
		DEBUG_LOG("Message written to socket...");
		socket->flush();
		socket->waitForBytesWritten(3000);
  }
	
	socket->close();
	
	return status;

}//close sender()


void FOFThreadObj::kdTime(KD kd,int *puSecond,int *puMicro){
	
	struct rusage ru;

	getrusage(0,&ru);
	*puMicro = ru.ru_utime.tv_usec - kd->uMicro;
	*puSecond = ru.ru_utime.tv_sec - kd->uSecond;
	if (*puMicro < 0) {
		*puMicro += 1000000;
		*puSecond -= 1;
	}
	kd->uSecond = ru.ru_utime.tv_sec;
	kd->uMicro = ru.ru_utime.tv_usec;
	
}//close FOFThreadObj::kdTime()


int FOFThreadObj::kdInit(KD *pkd,int nBucket,float *fPeriod){

	KD kd;
	int j;

	kd = (KD)malloc(sizeof(struct kdContext));
	assert(kd != NULL);
	kd->nBucket = nBucket;
    
	for (j=0;j<3;++j) kd->fPeriod[j] = fPeriod[j];
	kd->p = NULL;
	kd->kdNodes = NULL;
	*pkd = kd;
	return(1);
	
}//close FOFThreadObj::kdInit()

int FOFThreadObj::contaRighe (FILE *fp){

	int i = 0;
  char buffer;
  while (1){
  	fread ((void *)&buffer,sizeof(char),1,fp);
    if (feof(fp))
    	break;
      if (buffer == '\n')
      	i++;
  }	
	return i;

}//close FOFThreadObj::contaRighe()



void FOFThreadObj::kdReadMuons(KD kd, FILE *fp, int bPoca){

	int i,j,nCnt,id_ev;
  float thetaQ, energy; //da sistemare
	struct dump h; //un solo campo: int npoca-> numero di eventi poca
	struct poca_event pe_p; //id_ev, 3 coordinate, theta, thetaQ e energy

  h.npoca= contaRighe(fp);     rewind(fp);
  //mb: aggiunto
  kd->bPoca= bPoca; //aggiunto!
	kd->nPoca = h.npoca;
  DEBUG_LOG("h.npoca: "<<h.npoca);
  
	kd->nActive = 0;
	if (bPoca) kd->nActive += kd->nPoca; //nActive ha come valore il numero di eventi nel file
	
	//Allocate events.
	kd->p = (POCA *)malloc(kd->nActive*sizeof(POCA)); //sto allocando la memoria per tutti gli eventi
	assert(kd->p != NULL);
	
	//Read Stuff!
	//DEBUG_LOG("Read from FLY done: "<<h.nsph<<", "<<h.ndark<<", "<<h.nstar);
	fflush(stdout);
	
	nCnt = 0;
    
	//for (i=0;i<h.npoca;++i) { //for sostituito con il while e fread sostituita con fscanf
    //while(fscanf(fp, "%i%f%f%f%f%f%f", &pe_p.id_event, &pe_p.r[0], &pe_p.r[1],&pe_p.r[2], &pe_p.theta,&pe_p.thetaQ, &pe_p.energy)>0){
    //while(fscanf(fp, "%d%f%f%f%f%f%f", &pe_p.id_event, &pe_p.r[0], &pe_p.r[1],&pe_p.r[2], &pe_p.theta,&pe_p.thetaQ, &pe_p.energy)>0){
    while(fscanf(fp, "%d%f%f%f%f%f", &pe_p.id_event, &pe_p.r[0], &pe_p.r[1],&pe_p.r[2], &pe_p.theta,&pe_p.thetaQ)>0){
      
			DEBUG_LOG("pe_p.id_event="<<pe_p.id_event<<", pe_p.r[0]="<<pe_p.r[0]<<", pe_p.r[1]="<<pe_p.r[1]<<", pe_p.r[2]="<<pe_p.r[2]<<", pe_p.theta="<<pe_p.theta);
 
		//fread(&pe_p,sizeof(struct poca_event),1,fp);
		if (bPoca) {
			kd->p[nCnt].iOrder = nCnt; //il campo iOrder dell'elemento nCnt viene posto a nCnt (ordine di arrivo)
			//mb: for (j=0;j<3;++j) kd->p[nCnt].r[j] = dp.pos[j]; //mb:commentato perchè dp era la dark particle mentre ora l'evento poca non possiede un campo pos[]
			//fprintf(stderr,"particle # %d\n", nCnt);
			for (j=0; j<3; j++)
			  kd->p[nCnt].r[j] = pe_p.r[j]; //memorizzo la posizione..
            //  fprintf(stdout, "pe_p.r %f\n",kd->p[nCnt].r[j]);
			  //			  ++nCnt;
			//if(nCnt%100==0)
			//printf("Leggo\n");
            kd->p[nCnt].angle = pe_p.theta; //memorizzo il theta--->memorizzare anche gli altri elementi?
		}
		++nCnt;
    }

	rewind(fp);
	DEBUG_LOG("nCnt final="<<nCnt);
  //fprintf(stderr, "nCnt final=%d\n", nCnt);

}//close FOFThreadObj::kdReadMuons()


void FOFThreadObj::kdSelect(KD kd,int d,int k,int l,int r){
	
	POCA *p,t;
	double v;
	int i,j;

	p = kd->p;
	while (r > l) {
		v = p[k].r[d];
		t = p[r];
		p[r] = p[k];
		p[k] = t;
		i = l - 1;
		j = r;
		while (1) {
			while (i < j) if (p[++i].r[d] >= v) break;
			while (i < j) if (p[--j].r[d] <= v) break;
			t = p[i];
			p[i] = p[j];
			p[j] = t;
			if (j <= i) break;
		}
		p[j] = p[i];
		p[i] = p[r];
		p[r] = t;
		if (i >= k) r = i - 1;
		if (i <= k) l = i + 1;
	}
	
}//close FOFThreadObj::kdSelect


void FOFThreadObj::kdCombine(KDN *p1,KDN *p2,KDN *pOut){

	int j;

	//Combine the bounds
	for (j=0;j<3;++j) {
		if (p2->bnd.fMin[j] < p1->bnd.fMin[j])
			pOut->bnd.fMin[j] = p2->bnd.fMin[j];
		else
			pOut->bnd.fMin[j] = p1->bnd.fMin[j];
		if (p2->bnd.fMax[j] > p1->bnd.fMax[j])
			pOut->bnd.fMax[j] = p2->bnd.fMax[j];
		else
			pOut->bnd.fMax[j] = p1->bnd.fMax[j];
	}//end loop

}//close kdCombine()


void FOFThreadObj::kdUpPass(KD kd,int iCell){

	KDN *c;
	int l,u,pj,j;

	c = kd->kdNodes;
	if (c[iCell].iDim != -1) {
		l = LOWER(iCell);
		u = UPPER(iCell);
		kdUpPass(kd,l);
		kdUpPass(kd,u);
		kdCombine(&c[l],&c[u],&c[iCell]);
	}
	else {
		l = c[iCell].pLower;
		u = c[iCell].pUpper;
		for (j=0;j<3;++j) {
			c[iCell].bnd.fMin[j] = kd->p[u].r[j];
			c[iCell].bnd.fMax[j] = kd->p[u].r[j];
		}
		for (pj=l;pj<u;++pj) {
			for (j=0;j<3;++j) {
				if (kd->p[pj].r[j] < c[iCell].bnd.fMin[j])
					c[iCell].bnd.fMin[j] = kd->p[pj].r[j];
				if (kd->p[pj].r[j] > c[iCell].bnd.fMax[j])
					c[iCell].bnd.fMax[j] = kd->p[pj].r[j];
			}
		}
	}
	
}//close kdUpPass()

void FOFThreadObj::kdBuildTree(KD kd)
{
	int l,n,i,d,m,j,diff;
	KDN *c;
	BND bnd;

	n = kd->nActive;
	kd->nLevels = 1;
	l = 1;
    
	while (n > kd->nBucket) { //finchè n=nEvPoca>16 divide n per due e moltiplica l per 2 e incrementa kd->nLevels di uno
  	n = n>>1;//mb: /2
		l = l<<1; //mb:*2
		++kd->nLevels;  
	}
    
	kd->nSplit = l; 
	kd->nNodes = l<<1;
	if (kd->kdNodes != NULL) free(kd->kdNodes);
	kd->kdNodes = (KDN *)malloc(kd->nNodes*sizeof(KDN)); //mb:alloco memoria per kdNodes (n. elementi pari a nNodes=l*2*sizeof(KDN)
	assert(kd->kdNodes != NULL);
	
	//## Calculate Bounds.
  INFO_LOG("Computing bounds ...");
	for (j=0;j<3;++j) {
		bnd.fMin[j] = kd->p[0].r[j];
		bnd.fMax[j] = kd->p[0].r[j];
	}
   
	DEBUG_LOG("nActive: "<<kd->nActive);
	for (i=1;i<kd->nActive;++i) { //mb:sto costruendo i bounds ciclando su tutti gli eventi poca e andando a guardare tutte le coordinate
        //printf("ciclo %d\n", i);
		for (j=0;j<3;++j) {
			if (bnd.fMin[j] > kd->p[i].r[j]) 
				bnd.fMin[j] = kd->p[i].r[j];
			else if (bnd.fMax[j] < kd->p[i].r[j])
				bnd.fMax[j] = kd->p[i].r[j];
			}
		}
	
	//## Set up ROOT node
	DEBUG_LOG("setUp root node");
	c = kd->kdNodes;
	c[fROOT].pLower = 0;
	c[fROOT].pUpper = kd->nActive-1;
	c[fROOT].bnd = bnd;
	i = fROOT;
    //printf("prima del while infinito\n");
	while (1) {
		//cout<<"c[i].pUpper="<<c[i].pUpper<<"  c[i].pLower + 1="<<c[i].pLower + 1<<endl; 
		assert(c[i].pUpper - c[i].pLower + 1 > 0);
		if (i < kd->nSplit && (c[i].pUpper - c[i].pLower) > 0) {
			d = 0;
			for (j=1;j<3;++j) {
				if (c[i].bnd.fMax[j]-c[i].bnd.fMin[j] > 
					c[i].bnd.fMax[d]-c[i].bnd.fMin[d]) d = j;
				}
			c[i].iDim = d;

			m = (c[i].pLower + c[i].pUpper)/2;
			kdSelect(kd,d,m,c[i].pLower,c[i].pUpper);

			c[i].fSplit = kd->p[m].r[d]; //mb:qui interviene la posizione
			c[LOWER(i)].bnd = c[i].bnd;
			c[LOWER(i)].bnd.fMax[d] = c[i].fSplit;
			c[LOWER(i)].pLower = c[i].pLower;
			c[LOWER(i)].pUpper = m;
			c[UPPER(i)].bnd = c[i].bnd;
			c[UPPER(i)].bnd.fMin[d] = c[i].fSplit;
			c[UPPER(i)].pLower = m+1;
			c[UPPER(i)].pUpper = c[i].pUpper;
			diff = (m-c[i].pLower+1)-(c[i].pUpper-m);
			assert(diff == 0 || diff == 1);
			i = LOWER(i);
			}
		else {
            //printf("dentro else\n");
			c[i].iDim = -1;
			SETNEXT(i);
			if (i == fROOT) break;
			}
		}
	kdUpPass(kd,fROOT);
  DEBUG_LOG("KdBuild ended");
	
}//close FOFThreadObj::kdBuildTree()


int FOFThreadObj::kdFoF(KD kd,float fEps) //fEps--> linking lenght
{
	POCA *p;
	KDN *c;
	int pi,pj,pn,cp;

	int iGroup;

	int *Fifo,iHead,iTail,nFifo; //puntatori coda-testa..
	float fEps2; //linking lenght al quadrato
	float dx,dy,dz,x,y,z,lx,ly,lz,sx,sy,sz,fDist2;

	p = kd->p;
	c = kd->kdNodes;
	lx = kd->fPeriod[0]; //parametri px, py e pz iniziali
	ly = kd->fPeriod[1];
	lz = kd->fPeriod[2];
	fEps2 = fEps*fEps;
	for (pn=0;pn<kd->nActive;++pn) p[pn].iGroup = 0; //inizializzazione del gruppo-cluster a zero per tutti gli eventi poca
	nFifo = kd->nActive;
	Fifo = (int *)malloc(nFifo*sizeof(int)); //alloco memoria
	assert(Fifo != NULL);
	iHead = 0;
	iTail = 0;
	iGroup = 0;
    //printf("fof primo for\n");
    
    
    //for (pn=0;pn<kd->nActive;++pn){ if (p[pn].iGroup) printf("\n\n***ERROREE: p[pn].iGroup: %d\n\n\n", p[pn].iGroup);}
    
	for (pn=0;pn<kd->nActive;++pn) {
        
        //printf("*******Ciclo pn: %d\n", pn);
        //printf("*******p[pn].iGroup: %d\n", p[pn].iGroup);
		
        
        
        
        /************
         capire perchè al ciclo 246 trovo p[pn].iGroup!=0 ma uguale a 245!!!
         
         ********/
        if (p[pn].iGroup) continue; //mb:se il gruppo è != 0 vado avanti all'elemento poca successivo
		++iGroup; //mb:sostanzialmente conta il numero di gruppi iniziali=n. di particelle/eventi
        
        //printf("**********iGroup: %d\n", iGroup);
        //printf("**********nActive: %d\n", kd->nActive);
       
        
        
		/*
		 ** Mark it and add to the do-fifo.
		 */
		p[pn].iGroup = iGroup; //assegna ogni evento ad un cluster diverso 
        
        //printf("******DOPO p[pn].iGroup: %d\n", p[pn].iGroup);
        //printf("\n\n\n\n");
        
        
		Fifo[iTail++] = pn;
		if (iTail == nFifo) iTail = 0;
		while (iHead != iTail) {
			pi = Fifo[iHead++];
			if (iHead == nFifo) iHead = 0;
			/*
			 ** Now do an fEps-Ball Gather!
			 */
			x = p[pi].r[0];
			y = p[pi].r[1];
			z = p[pi].r[2];
			cp = fROOT;
			while (1) {
                //printf("fof while true\n");
				INTERSECT(c,cp,fEps2,lx,ly,lz,x,y,z,sx,sy,sz);
				/*
				 ** We have an intersection to test.
				 */
				if (c[cp].iDim >= 0) {
                    //printf("fof if\n");
					cp = LOWER(cp);
					continue;
					}
				else {
                    //printf("fof else\n");
					for (pj=c[cp].pLower;pj<=c[cp].pUpper;++pj) {
						if (p[pj].iGroup) continue;
						dx = sx - p[pj].r[0];
						dy = sy - p[pj].r[1];
						dz = sz - p[pj].r[2];
						fDist2 = dx*dx + dy*dy + dz*dz;
						if (fDist2 < fEps2) {
							/*
							 ** Mark it and add to the do-fifo.
							 */
							p[pj].iGroup = iGroup; //*************************qui cambia il gruppo
							Fifo[iTail++] = pj;
							if (iTail == nFifo) iTail = 0;
							}
						}
					SETNEXT(cp);
					if (cp == fROOT) break;
					continue;
					}
			ContainedCell:
				for (pj=c[cp].pLower;pj<=c[cp].pUpper;++pj) {
					if (p[pj].iGroup) continue;
					/*
					 ** Mark it and add to the do-fifo.
					 */
					p[pj].iGroup = iGroup;   //*************************qui cambia il gruppo
					Fifo[iTail++] = pj;
					if (iTail == nFifo) iTail = 0;
					}
			GetNextCell:
				SETNEXT(cp);
				if (cp == fROOT) break;
				}
			}
    }
    //printf("iGroup: %d\n", iGroup);
    //printf("\n\n\n\n");
    
    //printf("\n\n\n*****FINE FOF\n\n\n");

    //for (pn=0;pn<kd->nActive;++pn) {
        //printf("p[%d].iGroup: %d\n",pn, p[pn].iGroup);
    //}
    
    
	free(Fifo);
	kd->nGroup = iGroup+1;
	return(kd->nGroup-1);
	
}//close FOFThreadObj::kdFoF()


int FOFThreadObj::kdTooSmall(KD kd,int nMembers)
{
	int *pnMembers,*pMap;
	int i,pi,nGroup;

	pnMembers = (int *)malloc(kd->nGroup*sizeof(int));
	assert(pnMembers != NULL);
	pMap = (int *)malloc(kd->nGroup*sizeof(int));
	assert(pMap != NULL);
	for (i=0;i<kd->nGroup;++i) pnMembers[i] = 0;
	for (pi=0;pi<kd->nActive;++pi) { //vado a contare il numero di membri di ciascun gruppo
		++pnMembers[kd->p[pi].iGroup];
		}
	for (i=1;i<kd->nGroup;++i) {
		if (pnMembers[i] < nMembers) { //se il gruppo non contiene un numero sufficiente di membri, azzero il numero di membri
			pnMembers[i] = 0;
			}
		}

	//## Create a remapping!
	pMap[0] = 0;
	nGroup = 1;
	for (i=1;i<kd->nGroup;++i) {
		pMap[i] = nGroup;
		if (pnMembers[i] == 0) {
			pMap[i] = 0;
			}
		else {
			++nGroup;
            //printf("\n\nHABEMUS element pnMembers[%d]= %d and pMap[%d]= %d\n\n", i, pnMembers[i], i, pMap[i]);
			}
		}
    
    DEBUG_LOG("kdTooSMall: nGroup after remapping: "<<nGroup); //4?
 //   for (pi=0;pi<kd->nGroup;++pi) {
   //     printf("pMap[%d]= %d\n", pi, pMap[pi]); //i gruppi sono 4, ma i valori di pMAp sono sempre=0!!! perchè???
    
  //  }
    
	//## Remap the groups.
	for (pi=0;pi<kd->nActive;++pi) {
		kd->p[pi].iGroup = pMap[kd->p[pi].iGroup];
       // printf("p[%d].iGroup in KDtooSMALL: %d\n",pi, kd->p[pi].iGroup);
		}
	free(pMap);
	free(pnMembers);
	kd->nGroup = nGroup;
	return(nGroup-1);
	
}//close FOFThreadObj::kdTooSmall


int FOFThreadObj::CmpParticles(const void *v1,const void *v2){

	POCA *p1 = (POCA *)v1;
	POCA *p2 = (POCA *)v2;
	return(p1->iOrder - p2->iOrder);
	
}//close FOFThreadObj::CmpParticles()


void FOFThreadObj::kdOrder(KD kd){

	qsort(kd->p,kd->nActive,sizeof(POCA),FOFThreadObj::CmpParticles);

}//close FOFThreadObj::kdOrder()


void FOFThreadObj::kdOutGroup(KD kd,char *pszFile,FILE *ip)
{
	FILE *fp;
    struct poca_event pe_p;
	int i,iCnt;

	fp = fopen(pszFile,"w");
	assert(fp != NULL);
        
	iCnt = 0;
	
	for (i=0;i<kd->nPoca;++i) {
		int counter= iCnt;
		if (kd->bPoca) fprintf(fp,"%d\t",kd->p[iCnt++].iGroup);
		else fprintf(fp,"0\t");
				
		if(fscanf(ip, "%i %f %f %f %f %f", &pe_p.id_event, &pe_p.r[0], &pe_p.r[1],&pe_p.r[2], &pe_p.theta,&pe_p.thetaQ)>0){
    	fprintf(fp, "%f\t%f\t%f\t%f\n", pe_p.r[0], pe_p.r[1],pe_p.r[2],pe_p.theta);
			//printf("--> %d\t%f\t%f\t%f\t%f\n", kd->p[counter].iGroup,pe_p.r[0], pe_p.r[1],pe_p.r[2],pe_p.theta);
		}

	}
	
	fclose(fp);

}//close FOFThreadObj::kdOutGroup()


void FOFThreadObj::kdFinish(KD kd){

	free(kd->p);
	free(kd->kdNodes);
	free(kd);
	
}//close FOFThreadObj::kdFinish()

}//close namespace

