#include <POCAThreadObj.h>
#include <ConfigParser.h>
#include <AnalysisConsts.h>
#include <Gui.h>

#include <QtGui>
#include <QString>

#include <QUdpSocket>
#include <QDataStream>
#include <QHostAddress>
#include <QThread>
#include <QTcpSocket>

#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <qjson/qjson_export.h>
#include <qjson/qobjecthelper.h>
#include <qjson/serializerrunnable.h>
#include <qjson/parserrunnable.h>


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
#include <TROOT.h>
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
#include <TColor.h>


#include <iomanip>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <math.h>
#include <assert.h>

using namespace std;


POCAThreadObj::POCAThreadObj(){

	//## Connect status signal to socket sender
	connect(this,SIGNAL(logSig(QString)), this, SLOT(sender(QString)) );
	connect(this,SIGNAL(alarmSig(QString)), this, SLOT(sender(QString)) );

}//close constructor

POCAThreadObj::~POCAThreadObj(){

	//## Delete objects
	if(fTemperaturePalette) fTemperaturePalette->Delete();

}//close destructor

void POCAThreadObj::SetConfig(){

	//## Get config from parser
	//fInputFileName= ConfigParser::fPOCAInputFileName;
	//fOutputFileName= ConfigParser::fPOCAOutputFileName;
	fVerbosity= ConfigParser::fPOCAVerbosity;
	fUseNMaxRecEvents= ConfigParser::fUsePOCAMaxNEvents;
	fNMaxRecEvents= ConfigParser::fPOCAMaxNEvents;
	
	fPOCATolerance= ConfigParser::fPOCATolerance;
	fUseScatteringAngleCut= ConfigParser::fUsePOCAThetaCut;
	fScatteringAngleMinCut= ConfigParser::fPOCAThetaMin;
	fScatteringAngleMaxCut= ConfigParser::fPOCAThetaMax;
	fUseEnergyCut= ConfigParser::fUsePOCAEnergyCut;
	fEnergyMinCut= ConfigParser::fPOCAEnergyMin;
	fEnergyMaxCut= ConfigParser::fPOCAEnergyMax;

	fZPlane= ConfigParser::fZPlane;	
	fNPlane= fZPlane.size();

	fContainerSizeX= ConfigParser::fContainerSizeX;
	fContainerSizeY= ConfigParser::fContainerSizeY;
	fContainerSizeZ= ConfigParser::fContainerSizeZ;
	fContainerXStart= -fContainerSizeX/2.;
	fContainerXEnd= +fContainerSizeX/2.;
	fContainerYStart= -fContainerSizeY/2.;
	fContainerYEnd= +fContainerSizeY/2.;
	fContainerZStart= -fContainerSizeZ/2.;
	fContainerZEnd= +fContainerSizeZ/2.;
	
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

	fUsePOCAThetaWeight= ConfigParser::fUsePOCAThetaWeight;
	fPOCAThetaWeight= ConfigParser::fPOCAThetaWeight;
	fUseAveragePOCASignal= ConfigParser::fUsePOCAAverageSignal;
	fPOCANEventThreshold= ConfigParser::fPOCANEventThreshold;
	fPOCASignalThreshold= ConfigParser::fPOCASignalThreshold;
	fDrawMode= ConfigParser::fPOCADrawStyleName;
	
	fIsFailure= false;

	SetGraphicsStyle();

}//close POCAThreadObj::SetConfig()


bool POCAThreadObj::Init(){

	QString msg = QString("POCAThreadObj::Init(): Init data...");
	emit(statusMessage(msg));
	
	fLogMessage.status= eRunning;
	fLogMessage.logMessage= msg.toStdString();
	fLogMessage.progress= (int)(progressPercentage);

	QString logMessage= makeLogMessage(fLogMessage);
	emit logSig(logMessage);


	//## Set config parameters
	SetConfig();

	//## Create input file and count number of events
	fInputFileStream.open(fInputFileName.c_str());
	if(!fInputFileStream.good()) {
		QString errmsg = QString("POCAThreadObj::Init(): ERROR: Cannot open input file ");
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
	fNEvents = 0;
  
	std::string s;
  while(getline(fInputFileStream,s)) {
  	fNEvents++;
  }
   
	msg = QString("POCAThreadObj::ReadData(): INFO: ");
	msg+= QString("%1").arg(fNEvents);
	msg+= QString::fromStdString(std::string(" events in input file ..."));
	emit(statusMessage(msg));

	fLogMessage.status= eRunning;
	fLogMessage.logMessage= msg.toStdString();
	fLogMessage.progress= (int)(progressPercentage);

	logMessage= makeLogMessage(fLogMessage);
	emit logSig(logMessage);

	fInputFileStream.clear();
	fInputFileStream.seekg(ios::beg);//rewind for reading

	//## Initialize raw data list
	fRawDataList= new POCARawData[fNEvents];

	//## Init output data tree	
	fOutputFile= new TFile(fOutputFileName.c_str(),"RECREATE");
	fOutputFile->cd();

	
	fScatteringDataInfo= new TTree("data","data");
	fScatteringDataInfo->Branch("EventId",&fEventId,"EventId/I");
	fScatteringDataInfo->Branch("ScatteringAngle",&fScatteringAngle,"ScatteringAngle/D");
	fScatteringDataInfo->Branch("POCAX",&fPOCAX,"POCAX/D");
	fScatteringDataInfo->Branch("POCAY",&fPOCAY,"POCAY/D");
	fScatteringDataInfo->Branch("POCAZ",&fPOCAZ,"POCAZ/D");
	fScatteringDataInfo->Branch("KinEnergy",&fKinEnergy,"KinEnergy/D");
	fScatteringDataInfo->Branch("Dmin", &fDMin , "Dmin/D");

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

	fInfoTree= new TTree("InfoRun","InfoRun");
	fInfoTree->Branch("NThreshold",&fPOCANEventThreshold,"NThreshold/I");
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


	fAlarmVoxelIdList.clear();
	fAlarmVoxelIdList.resize(0);

	

	//## Set output filenames
	fCurrentDirectory= std::string(qPrintable(QDir::currentPath())) + std::string("/");
	fInputFileName_noPath= fInputFileName.substr( fInputFileName.find_last_of("\\/")+1 );
	fInputFileNamePath = fInputFileName.substr( 0, fInputFileName.find_last_of("\\/")+1 );

	fVisIVOOutputFileName= fCurrentDirectory + std::string("POCA-VisIVO_") + fInputFileName_noPath;
	fVisIVOOutputFileName_noPath= fVisIVOOutputFileName.substr( fVisIVOOutputFileName.find_last_of("\\/")+1 );

	fVisIVOVolRendImageFilePrefix= std::string("VisIVOVolRend_") + fVisIVOOutputFileName_noPath;

	fVisIVOPointOutputFileName= fCurrentDirectory + std::string("POCAPoint-VisIVO_") + fInputFileName_noPath;
	fVisIVOPointOutputFileName_noPath= fVisIVOPointOutputFileName.substr( fVisIVOPointOutputFileName.find_last_of("\\/")+1 );
	fVisIVOPointImageFilePrefix= std::string("VisIVOPoint_") + fVisIVOPointOutputFileName_noPath;


	fVisIVOCommandName= fCurrentDirectory + std::string("/scripts/VisIVOVolumeImaging.sh ") + fVisIVOOutputFileName_noPath + Form(" %d %d %d ",fNVoxelX,fNVoxelY,fNVoxelZ) + fCurrentDirectory + std::string("/figure/"); 
		

	return true;

}//close POCAThreadObj::Init()


bool POCAThreadObj::ReadData(){
	
	QString msg = QString("POCAThreadObj::ReadData(): INFO: Start reading data...");
	emit(statusMessage(msg));

	fLogMessage.status= eRunning;
	fLogMessage.logMessage= msg.toStdString();
	fLogMessage.progress= (int)(progressPercentage);

	QString logMessage= makeLogMessage(fLogMessage);
	emit logSig(logMessage);


	fNReadEvents= 0;
	double thisProgressPercentage= 0;

	for(int j=0;j<fNEvents;j++){

		//## Break event loop?
		if(fUseNMaxRecEvents && j>=fNMaxRecEvents) {
			break;
		}
		
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
			QString errmsg = QString("POCAThreadObj::ReadData(): ERROR: Troubles in input file reading (strange characters?)...");
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
		
		fRawDataList[j].EventId= EventId;
		fRawDataList[j].p= p;
		for(int i=0;i<nplanes;i++){
			fRawDataList[j].HitX[i]= xHit[i];
			fRawDataList[j].HitY[i]= yHit[i];
		}
	
		
		if(j%100000==0){
			thisProgressPercentage= (double)(fNReadEvents)/(double)(fNEvents)*stagePercentage;
			double thisReadProgressStatus= progressPercentage+thisProgressPercentage;

			emit(statusProgress( (int)(thisReadProgressStatus) ));
			
			cout<<"POCA Image Reco: INFO: event="<<j<<" fNEvents="<<fNEvents<<"  fNReadEvents="<<fNReadEvents<<"  thisProgressPercentage="<<thisProgressPercentage<<"  (int)(progressPercentage+thisProgressPercentage)="<<(int)(progressPercentage+thisProgressPercentage)<<endl;

			msg = QString("--> POCA Image Reco: Read data [Event No. %1] ...").arg(j);
			emit(statusMessage(msg));

			fLogMessage.status= eRunning;
			fLogMessage.logMessage= msg.toStdString();
			fLogMessage.progress= (int)(thisReadProgressStatus);

			QString logMessage= makeLogMessage(fLogMessage);
			emit logSig(logMessage);
		}

		fNReadEvents++;

	}//end loop events in file
	
	
	cout<<"POCAThreadObj::ReadData(): INFO: "<<fNReadEvents<<" events read..."<<endl;
	
	//## Close input file
	fInputFileStream.close();

	return true;

}//close POCAThreadObj::ReadData()


void POCAThreadObj::process(){

	//## Set message 
	fLogMessage.taskName= "POCA";
	fLogMessage.command= "LOG";	
	fLogMessage.inputFileName= fInputFileName;		
	fLogMessage.outputFileName= fOutputFileName;	
	fLogMessage.jobDirName= fDestinationPath;
	fLogMessage.scanId= fScanId;
	fLogMessage.containerId= fContainerId;
	fLogMessage.timeStamp= fTimeStamp;			
	fLogMessage.status= eRunning;
	fLogMessage.logMessage= "POCA imaging started...";
	fLogMessage.progress= 0;
	fLogMessage.guiKey= fGuiKey;
	fLogMessage.guiHostName= fGuiHostName;
	fLogMessage.guiPort= fGuiPort;

	fAlarmMessage.taskName= "POCA";
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


	progressPercentage= 0;
	nRecStages= 5;//Init + ReadData + Run + Draw + Save
	stagePercentage= 1./(double)(nRecStages)*100.;
	fIsFailure= false;


	msg = QString("--> POCA Image Reco: Input data file ");
	msg+= QString::fromStdString(fInputFileName);
	msg+= QString::fromStdString(std::string(" ..."));
	emit(statusMessage(msg));

	emit(statusProgress((int)(progressPercentage)));

	fLogMessage.status= eRunning;
	fLogMessage.logMessage= msg.toStdString();
	fLogMessage.progress= (int)(progressPercentage);

	logMessage= makeLogMessage(fLogMessage);
	emit logSig(logMessage);
	
	//## Init data
	bool init_status= Init();
	//if(fIsFailure) return;
	//progressPercentage+= stagePercentage;
	//emit(statusProgress((int)(progressPercentage)));

	if(!init_status){
	
		msg = QString("--> POCA Image Reco: Init data failed!");
		emit(statusMessage(msg));

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= 0;

		logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
		
		return;

	}
	else{

		msg = QString("--> POCA Image Reco: Init data ... done!");
		emit(statusMessage(msg));

		progressPercentage+= stagePercentage;
		emit(statusProgress((int)(progressPercentage)));

		cout<<"--> POCA Image Reco: Init progressPercentage="<<progressPercentage<< "  stagePercentage="<<stagePercentage<<endl;

		fLogMessage.status= eRunning;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= (int)(progressPercentage);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}



	//## Read data
	bool read_status= ReadData();
	//if(fIsFailure) return;
	//progressPercentage+= stagePercentage;
	//emit(statusProgress((int)(progressPercentage)));

	if(!read_status){
	
		msg = QString("--> POCA Image Reco: Read data failed!");
		emit(statusMessage(msg));

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= 0;

		logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
		
		return;

	}
	else{
	
		msg = QString("--> POCA Image Reco: Read data ... done!");
		emit(statusMessage(msg));

		progressPercentage+= stagePercentage;
		emit(statusProgress((int)(progressPercentage)));

		cout<<"--> POCA Image Reco: Read progressPercentage="<<progressPercentage<< "  stagePercentage="<<stagePercentage<<endl;


		fLogMessage.status= eRunning;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= (int)(progressPercentage);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}




	//## Calculate POCA and fill data
	bool run_status= Run();
	//if(fIsFailure) return;
	//progressPercentage+= stagePercentage;
	//emit(statusProgress((int)(progressPercentage)));
	
	if(!run_status){
	
		msg = QString("--> POCA Image Reco: Run algo stage failed!");
		emit(statusMessage(msg));

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= 0;

		logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
		
		return;

	}
	else{

		msg = QString("--> POCA Image Reco: Run algo stage ... done!");
		emit(statusMessage(msg));

		progressPercentage+= stagePercentage;
		emit(statusProgress((int)(progressPercentage)));

		cout<<"--> POCA Image Reco: Run progressPercentage="<<progressPercentage<< "  stagePercentage="<<stagePercentage<<endl;

		fLogMessage.status= eRunning;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= (int)(progressPercentage);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}



	//## Draw tomographic maps
	bool draw_status= Draw();
	//if(fIsFailure) return;

	if(!draw_status){
	
		msg = QString("--> POCA Image Reco: Draw stage failed!");
		emit(statusMessage(msg));

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= 0;

		logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
		
		return;

	}
	else{

		msg = QString("--> POCA Image Reco: Draw stage ... done!");
		emit(statusMessage(msg));
		
		progressPercentage+= stagePercentage;
		emit(statusProgress((int)(progressPercentage)));

		cout<<"--> POCA Image Reco: Draw progressPercentage="<<progressPercentage<< "  stagePercentage="<<stagePercentage<<endl;

		fLogMessage.status= eRunning;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= progressPercentage;

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}

	//## Save to file
	bool save_status= Save();
	//if(fIsFailure) return;
	//progressPercentage+= stagePercentage;
	//emit(statusProgress(max((int)(progressPercentage),100)));

	if(!save_status){
	
		msg = QString("--> POCA Image Reco: Save stage failed!");
		emit(statusMessage(msg));

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= 0;

		logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
		
		return;

	}
	else{

		msg = QString("--> POCA Image Reco: Save stage ... done!");
		emit(statusMessage(msg));
		
		progressPercentage+= stagePercentage;
		emit(statusProgress(max((int)(progressPercentage),100)));

		cout<<"--> POCA Image Reco: Save progressPercentage="<<progressPercentage<< "  stagePercentage="<<stagePercentage<<endl;
		
		fLogMessage.status= eRunning;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= max((int)(progressPercentage),100);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}


	if(fIsFailure) {
		emit error();	
	
		QString msg = QString("--> POCA Image Reco: Terminated with failure!");
		emit(statusMessage(msg));	

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= progressPercentage;

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}
	else {
		emit finished();
		QString msg = QString("--> POCA Image Reco: Terminated with success!");
		emit(statusMessage(msg));	
		emit(statusProgress(max((int)(progressPercentage),100)));

		fLogMessage.status= eFinished;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= progressPercentage;

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}

}//close POCAThreadObj::process()


bool POCAThreadObj::Run(){

	cout<<"POCAThreadObj::Run(): INFO: Start POCA reconstruction..."<<endl;
	const int nplanes= 4;
	double xHit[nplanes];
	double yHit[nplanes];
	double zHit[nplanes];	
	
	fNSelEvents= 0;
	double thisProgressPercentage= 0;

	FILE* fout_point;
	fout_point= fopen(fVisIVOPointOutputFileName.c_str(),"w");
	fprintf(fout_point,"X Y Z Theta2\n");


	//## Calculate POCA for all events
	for(int j=0;j<fNReadEvents;j++){
		//cout<<"--> Processing event "<<j<<"/"<<fNReadEvents<<endl;

		fEventId= fRawDataList[j].EventId;
		fKinEnergy= fRawDataList[j].p;
		
		//## Smear track points?
		if(fSmearTrackPoints){
			for(int i=0;i<nplanes;i++){
				xHit[i]= fRawDataList[j].HitX[i];
				yHit[i]= fRawDataList[j].HitY[i];
				zHit[i]= fZPlane[i];

				bool isGoodSmearing= false;
				double randx= 0;
				double randy= 0;
				double randz= 0;

				while(!isGoodSmearing){
					randx= gRandom->Gaus(0,fSigmaPosX);
					randy= gRandom->Gaus(0,fSigmaPosY);
					randz= gRandom->Gaus(0,fSigmaPosZ);
	
					if(fabs(randx)<ConfigParser::fSigmaPosX && fabs(randy)<ConfigParser::fSigmaPosY && fabs(randz)<ConfigParser::fSigmaPosZ ) isGoodSmearing= true;
					else {
						//cout<<"LikelihoodImageReco::SetScatteringData(): INFO: Bad smearing (rand xyz="<<randx<<","<<randy<<","<<randz<<") ...re-generate random numbers!"<<endl;
					}
				}//end while
	
				xHit[i]+= randx; 
				yHit[i]+= randy;
				zHit[i]+= randz;
			}//end loop planes
		}//close if
		else{
			for(int i=0;i<nplanes;i++){
				xHit[i]= fRawDataList[j].HitX[i];
				yHit[i]= fRawDataList[j].HitY[i];
				zHit[i]= fZPlane[i];
			}//end loop planes
		}//close else
		

		//## Entry point and direction
		TVector3 P0_in( xHit[0], yHit[0], zHit[0]); 
  	TVector3 P1_in( xHit[1], yHit[1], zHit[1]);
	
		TVector3 u_in = P1_in-P0_in;
		double Tx_in= u_in.X()/u_in.Z();
 		double Ty_in= u_in.Y()/u_in.Z();
  	TVector3 entryDir = u_in.Unit();	
		TVector3 entryPoint= TVector3(P0_in.X()+Tx_in*(fContainerZStart-P0_in.Z()), P0_in.Y()+Ty_in*(fContainerZStart-P0_in.Z()), fContainerZStart);

	
		//## Exit point and direction
		TVector3 P0_out( xHit[2], yHit[2], zHit[2]); 
  	TVector3 P1_out( xHit[3], yHit[3], zHit[3]);
		TVector3 u_out = P1_out-P0_out; 
		double Tx_out= u_out.X()/u_out.Z();
 		double Ty_out= u_out.Y()/u_out.Z();
  	TVector3 exitDir = u_out.Unit();
		TVector3 exitPoint= TVector3(P0_out.X()+Tx_out*(fContainerZEnd-P0_out.Z()), P0_out.Y()+Ty_out*(fContainerZEnd-P0_out.Z()), fContainerZEnd);
	
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
	
		double delta= a*c-b*b;
		double t_in;
		double t_out;
		if(delta<=fPOCATolerance){//parallel lines
			cerr<<"POCAThreadObj::Run(): WARNING: delta smaller than tolerance (1.e-08), setting safe values..."<<endl;
			t_in= 0.;	
			t_out = (b>c ? d/b : e/c);	
		}
		else{//skew lines
			t_in= (b*e-c*d)/delta;
			t_out= (a*e-b*d)/delta;
		}
		TVector3 POCA_in= P0_in + t_in*u_in;
		TVector3 POCA_out= P0_out + t_out*u_out;
		TVector3 POCA= 0.5*(POCA_in+POCA_out);
		fPOCAX= POCA.X();
    fPOCAY= POCA.Y();
    fPOCAZ= POCA.Z();

		// Check POCA values
		if(TMath::IsNaN(fPOCAX) || TMath::IsNaN(fPOCAY) || TMath::IsNaN(fPOCAZ)){
			cerr<<"POCAThreadObj::Run(): WARNING: NAN values for POCA coordinates...skip event!"<<endl;
			continue;
		}
		if(fabs(fPOCAX)==TMath::Infinity() || fabs(fPOCAY)==TMath::Infinity() || fabs(fPOCAZ)==TMath::Infinity()){
			cerr<<"POCAThreadObj::Run(): WARNING: Inf values for POCA coordinates...skip event!"<<endl;
			continue;
		}
		// Check if POCA is inside the container 
		if( fPOCAX<fContainerXStart || fPOCAX>fContainerXEnd ||
     		fPOCAY<fContainerYStart || fPOCAY>fContainerYEnd ||
     		fPOCAZ<fContainerZStart || fPOCAZ>fContainerZEnd
		){
			//cerr<<"POCAThreadObj::Run(): INFO: POCA ("<<fPOCAX<<","<<fPOCAY<<","<<fPOCAZ<<") outside container ("<<fContainerXStart<<"-"<<fContainerYStart<< ","<<fContainerYStart<<"-"<<fContainerYEnd<<","<<fContainerZStart<<"-"<<fContainerZEnd<<")...skip event!"<<endl;
			continue;
		}

		fScatteringAngle= entryDir.Angle(exitDir)*TMath::RadToDeg();//in deg
		if(fUseScatteringAngleCut && (fScatteringAngle<fScatteringAngleMinCut || fScatteringAngle>fScatteringAngleMaxCut) ) continue;
		if(fUseEnergyCut && (fKinEnergy<fEnergyMinCut || fKinEnergy>fEnergyMaxCut) ) continue;
		
		fScatteringDataInfo->Fill();
		fNSelEvents++;


		//## Fill histos
		int	VoxelIdX = (int)( (fPOCAX+fVoxelSizeX/2.*fNVoxelX)/fVoxelSizeX);
		int VoxelIdY = (int)( (fPOCAY+fVoxelSizeY/2.*fNVoxelY)/fVoxelSizeY);
		int VoxelIdZ = (int)( (fPOCAZ+fVoxelSizeZ/2.*fNVoxelZ)/fVoxelSizeZ);
		
		
		int binX= dummyXLevelHisto->FindBin(fPOCAX);
		int binY= dummyYLevelHisto->FindBin(fPOCAY);
		int binZ= dummyZLevelHisto->FindBin(fPOCAZ);
		
		int voxelId= GetVoxelId(VoxelIdX,VoxelIdY,VoxelIdZ);
		
		if(fUsePOCAThetaWeight){
			
			RecMap3D->Fill(fPOCAX,-fPOCAY,fPOCAZ,pow(fScatteringAngle,fPOCAThetaWeight));
			NEventMap3D->Fill(fPOCAX,-fPOCAY,fPOCAZ,1);

			
			if(!dummyXLevelHisto->IsBinOverflow(binX) && !dummyXLevelHisto->IsBinUnderflow(binX)){ 
				RecMap_XFix[binX-1]->Fill(-fPOCAY,fPOCAZ,pow(fScatteringAngle,fPOCAThetaWeight));
				NEventMap_XFix[binX-1]->Fill(-fPOCAY,fPOCAZ,1);
			}
			if(!dummyYLevelHisto->IsBinOverflow(binY) && !dummyYLevelHisto->IsBinUnderflow(binY)){
				RecMap_YFix[binY-1]->Fill(fPOCAX,fPOCAZ,pow(fScatteringAngle,fPOCAThetaWeight));
				NEventMap_YFix[binY-1]->Fill(fPOCAX,fPOCAZ,1);
			}
			if(!dummyZLevelHisto->IsBinOverflow(binZ) && !dummyZLevelHisto->IsBinUnderflow(binZ)){
				RecMap_ZFix[binZ-1]->Fill(fPOCAX,-fPOCAY,pow(fScatteringAngle,fPOCAThetaWeight));
				NEventMap_ZFix[binZ-1]->Fill(fPOCAX,-fPOCAY,1);	
			}	

			
		}
		else{
			RecMap3D->Fill(fPOCAX,-fPOCAY,fPOCAZ);
			NEventMap3D->Fill(fPOCAX,-fPOCAY,fPOCAZ,1);
			if(!dummyXLevelHisto->IsBinOverflow(binX) && !dummyXLevelHisto->IsBinUnderflow(binX)) {
				RecMap_XFix[binX-1]->Fill(-fPOCAY,fPOCAZ);
				NEventMap_XFix[binX-1]->Fill(-fPOCAY,fPOCAZ,1);
			}
			if(!dummyYLevelHisto->IsBinOverflow(binY) && !dummyYLevelHisto->IsBinUnderflow(binY)){
				RecMap_YFix[binY-1]->Fill(fPOCAX,fPOCAZ);
				NEventMap_YFix[binY-1]->Fill(fPOCAX,fPOCAZ,1);
			}
			if(!dummyZLevelHisto->IsBinOverflow(binZ) && !dummyZLevelHisto->IsBinUnderflow(binZ)){
				RecMap_ZFix[binZ-1]->Fill(fPOCAX,-fPOCAY);
				NEventMap_ZFix[binZ-1]->Fill(fPOCAX,-fPOCAY,1);
			}
		}
		
		//fprintf(fout_point,"%f  %f  %f  %f\n",fPOCAX,-fPOCAY,fPOCAZ,log10(pow(fScatteringAngle,fPOCAThetaWeight)));
		fprintf(fout_point,"%f  %f  %f  %f\n",fPOCAX,-fPOCAY,fPOCAZ,pow(fScatteringAngle,fPOCAThetaWeight));


		if(j%100000==0){
			thisProgressPercentage= (double)(fNSelEvents)/(double)(fNReadEvents)*stagePercentage;
			double thisRunProgressStatus= progressPercentage + thisProgressPercentage;
			emit(statusProgress( (int)(thisRunProgressStatus) ));

			QString msg = QString("--> POCA Image Reco: Compute POCA [Event No. %1] ...").arg(j+1);
			emit(statusMessage(msg));

			fLogMessage.status= eRunning;
			fLogMessage.logMessage= msg.toStdString();
			fLogMessage.progress= (int)(thisRunProgressStatus);

			QString logMessage= makeLogMessage(fLogMessage);
			emit logSig(logMessage);
		}


	}//end loop events

	cout<<"POCAThreadObj::Run(): INFO: "<<fNSelEvents<<" events selected..."<<endl;


	//## Set to zero bins below Nevent threshold
	for(int k=0;k<RecMap3D->GetNbinsZ();k++){
		for(int j=0;j<RecMap3D->GetNbinsY();j++){
			for(int i=0;i<RecMap3D->GetNbinsX();i++){		
				double N= NEventMap3D->GetBinContent(i+1,j+1,k+1);
				if(N<fPOCANEventThreshold) RecMap3D->SetBinContent(i+1,j+1,k+1,0.);	
			}//end loop bins x
		}//end loop bins y
	}//end loop bins z

	for(int k=0;k<fNVoxelZ;k++){
		for(int i=0;i<RecMap_ZFix[k]->GetNbinsX();i++){
			for(int j=0;j<RecMap_ZFix[k]->GetNbinsY();j++){
				double N= NEventMap_ZFix[k]->GetBinContent(i+1,j+1);
				if(N<fPOCANEventThreshold) RecMap_ZFix[k]->SetBinContent(i+1,j+1,0.);	
			}//end loop bins Y
		}//end loop bins X
	}//end loop voxel Z

	for(int k=0;k<fNVoxelX;k++){
		for(int i=0;i<RecMap_XFix[k]->GetNbinsX();i++){
			for(int j=0;j<RecMap_XFix[k]->GetNbinsY();j++){
				double N= NEventMap_XFix[k]->GetBinContent(i+1,j+1);
				if(N<fPOCANEventThreshold) RecMap_XFix[k]->SetBinContent(i+1,j+1,0.);	
			}//end loop bins Y
		}//end loop bins X
	}//end loop voxel X

	for(int k=0;k<fNVoxelY;k++){
		for(int i=0;i<RecMap_YFix[k]->GetNbinsX();i++){
			for(int j=0;j<RecMap_YFix[k]->GetNbinsY();j++){
				double N= NEventMap_YFix[k]->GetBinContent(i+1,j+1);
				if(N<fPOCANEventThreshold) RecMap_YFix[k]->SetBinContent(i+1,j+1,0.);	
			}//end loop bins Y
		}//end loop bins X
	}//end loop voxel Y

	//## Rescale map? Compute min & max POCA signal
	fMinPOCASignal= 1.e+99;
	fMaxPOCASignal= -1.e+99;
	if(fUseAveragePOCASignal){
		RecMap3D->Divide(NEventMap3D);
		
		for(int k=0;k<fNVoxelX;k++){
			RecMap_XFix[k]->Divide(NEventMap_XFix[k]);
		}
		for(int k=0;k<fNVoxelY;k++){
			RecMap_YFix[k]->Divide(NEventMap_YFix[k]);
		}
		for(int k=0;k<fNVoxelZ;k++){
			RecMap_ZFix[k]->Divide(NEventMap_ZFix[k]);
		}
	}
		

	//## Write to ascii file and emit alarm
	FILE* fout;
	fout= fopen(fVisIVOOutputFileName.c_str(),"w");
	fprintf(fout,"S\n");


	
	bool isAlarm= false;

	for(int k=0;k<RecMap3D->GetNbinsZ();k++){
		for(int j=0;j<RecMap3D->GetNbinsY();j++){
			for(int i=0;i<RecMap3D->GetNbinsX();i++){		
				double x= RecMap3D->GetXaxis()->GetBinCenter(i+1);
				double y= RecMap3D->GetYaxis()->GetBinCenter(j+1);
				double z= RecMap3D->GetZaxis()->GetBinCenter(k+1);
				double binContent= RecMap3D->GetBinContent(i+1,j+1,k+1);
				int gBinId= RecMap3D->GetBin(i+1,j+1,k+1); 

				if(binContent>fPOCASignalThreshold) {
					isAlarm= true;
					fAlarmVoxelIdList.push_back(gBinId);
				}

				int	VoxelIdX = (int)( (x+fVoxelSizeX/2.*fNVoxelX)/fVoxelSizeX);
				int VoxelIdY = (int)( (y+fVoxelSizeY/2.*fNVoxelY)/fVoxelSizeY);
				int VoxelIdZ = (int)( (z+fVoxelSizeZ/2.*fNVoxelZ)/fVoxelSizeZ);
		
				int voxelId= GetVoxelId(VoxelIdX,VoxelIdY,VoxelIdZ);
				
				fprintf(fout,"%f\n",binContent);
				
			}//end loop y bins
		}//end loop x bins
	}//end loop z bins
	
	fclose(fout);
	fclose(fout_point);


	FILE* fout_vtk;
	fout_vtk= fopen("VTKData.vtk","w");
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
	//cout<<"VECTORS vector_name float"<<endl;
	for(int k=0;k<RecMap3D->GetNbinsZ();k++){
		for(int j=0;j<RecMap3D->GetNbinsY();j++){
			for(int i=0;i<RecMap3D->GetNbinsX();i++){	
				double x= RecMap3D->GetXaxis()->GetBinCenter(i+1);	
				double y= RecMap3D->GetYaxis()->GetBinCenter(j+1);
				double z= RecMap3D->GetZaxis()->GetBinCenter(k+1);
				double binContent= RecMap3D->GetBinContent(i+1,j+1,k+1);
				fprintf(fout_vtk,"%f\n",binContent);

				//cout<<x<<"  "<<y<<"  "<<z<<endl;		
			}//end loop y bins
		}//end loop x bins
	}//end loop z bins

	fclose(fout_vtk);

	QString alarmMsg = QString("0");
	if(isAlarm) {
		emit alarm();

		alarmMsg= QString("1");
		emit(alarmStatusMessage(alarmMsg));

		fAlarmMessage.status= eRunning;
		fAlarmMessage.alarmVoxelList= fAlarmVoxelIdList;
		fAlarmMessage.alarmMessage= Form("--> ALARM (%d alarm clusters)!!!",(int)(fAlarmVoxelIdList.size()));
		
		QString alarmMessage= makeAlarmMessage(fAlarmMessage);
		//emit alarmSig(alarmMessage);
	}

	return true;

}//close POCAThreadObj::Run()



bool POCAThreadObj::Draw(){


	fMinPOCASignal= RecMap3D->GetMinimum(0);
	fMaxPOCASignal= RecMap3D->GetMaximum();

	cout<<"POCAThreadObj::Run(): INFO: min/max signal: "<<fMinPOCASignal<<"/"<<fMaxPOCASignal<<"  logScale="<<log10(fMinPOCASignal)<<"/"<<log10(fMaxPOCASignal)<<endl;
	
	
	//## Draw YZ tomography
	for(int k=0;k<fNVoxelX;k++){
		//RecMapPlot_XFix[k]->cd();

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

		RecMap_XFix[k]->SetMinimum(fMinPOCASignal);
		RecMap_XFix[k]->SetMaximum(fMaxPOCASignal);
		RecMap_XFix[k]->SetContour(fNColourContours);
		//RecMap_XFix[k]->Draw(fDrawMode.c_str());
		
	}//end loop voxel X

	//## Draw XZ tomography
	for(int k=0;k<fNVoxelY;k++){
		//RecMapPlot_YFix[k]->cd();

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

		RecMap_YFix[k]->SetMinimum(fMinPOCASignal);
		RecMap_YFix[k]->SetMaximum(fMaxPOCASignal);
		RecMap_YFix[k]->SetContour(fNColourContours);
		//RecMap_YFix[k]->Draw(fDrawMode.c_str());
		
	}//end loop voxel Y
	
	//## Draw XY tomography
	for(int k=0;k<fNVoxelZ;k++){
		//RecMapPlot_ZFix[k]->cd();

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

		RecMap_ZFix[k]->SetMinimum(fMinPOCASignal);
		RecMap_ZFix[k]->SetMaximum(fMaxPOCASignal);
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
		TString imageOutName= Form("VolRendImg%d",i+1);
		//VolRendImg[i]= new MyTASImage(imgName);
		VolRendImg[i]= new TASImageNew(imgName);
		VolRendImg[i]->SetPaletteEnabled(false);
		VolRendImg[i]->SetNameTitle(imageOutName,imageOutName);
	}//end loop images

	return true;
	
}//close POCAThreadObj::Draw()


bool POCAThreadObj::Save(){

	cout<<"POCAThreadObj::Save(): INFO: Saving results to file ..."<<endl;
	fOutputFile->cd();
	fScatteringDataInfo->Write();
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
	int NVisIVOImg= 37;
	for(int i=0;i<NVisIVOImg;i++){
		VolRendImg[i]->Write();
	}//end loop images

	fOutputFile->Close();

	return true;

}//close POCAThreadObj::Save()



QString POCAThreadObj::makeLogMessage(LOG_MESSAGE msg){

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

}//close POCAThreadObj::makeLogMessage()


QString POCAThreadObj::makeAlarmMessage(ALARM_MESSAGE msg){

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

}//close POCAThreadObj::makeAlarmMessage()




bool POCAThreadObj::sender(QString message){

	
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

}//close POCAThreadObj::sender()


void POCAThreadObj::SetGraphicsStyle(){

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
