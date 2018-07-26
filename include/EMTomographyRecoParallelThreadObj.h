
/**
* @file EMTomographyRecoParallelThreadObj.h
* @class EMTomographyRecoParallelThreadObj
* @brief Reconstruct the tomographic image with likelihood method
*
* @author S. Riggi
* @date 21/02/2012
*/



#ifndef EMTOMOGRAPHYRECO_PARALLELTHREAD_OBJ_H
#define EMTOMOGRAPHYRECO_PARALLELTHREAD_OBJ_H

#include <TASImageNew.h>
//#include <MyTASImage.h>

#include <mpi.h>
#include <QObject>



#include <VoxelData.h>
#include <MuonEventData.h>

//## G4 classes
#include <G4Navigator.hh>
#include <G4RunManager.hh>
#include <G4UImanager.hh>

//#ifdef G4VIS_USE
#include <G4VisExecutive.hh>
//#endif

//## ROOT classes
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TFile.h>
#include <TTree.h>
#include <TMatrixD.h>
#include <TF1.h>
#include <TGraph.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TApplication.h>
#include <TGeoManager.h>
#include <TImage.h>


#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>



#include <QDialog>
#include <QUrl>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>

class QObject;
class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
class QCheckBox;
class QGroupBox;
class QPushButton;
class QTextEdit;
class QSpinBox;
class QDoubleSpinBox;
class QComboBox;
class QLineEdit;
class QRadioButton;
class QProgressBar;
class QMovie;
class QLabel;
class QUdpSocket;
class QTcpSocket;
class QTcpServer;
class QHostAddress;
class QtNetwork;
class QThread;
class QStatusBar;
class QSpacerItem;

using namespace std;


class EMTomographyRecoParallelThreadObj : public QObject {
    
	Q_OBJECT

  public:
		
		/** 
		\brief Class constructor: initialize structures.
 		*/
    EMTomographyRecoParallelThreadObj();
		/**
		* \brief Class destructor: free allocated memory
		*/
   ~EMTomographyRecoParallelThreadObj();


		#define MAX_VOXELS_EVENT 500

		//## Raw data
		struct MPIRawData{
			int EventId;
			double HitX[4];
			double HitY[4];
			double p;
		};	

		//## Scattering data
		struct MPIScatData{
			bool HasNavigationFailure;	
			bool HasTraversedContainer;
			bool HasTraversedContainerFromTop;
			bool HasTraversedContainerFromSide;
			bool IsPOCAInsideContainer;
			bool IsPOCAReliable;	
			double ScatteringAngle;
			double POCA[3];
			double EntryPoint[3];
			double EntryDirection[3];
			double ExitPoint[3];
			double ExitDirection[3]; 
			double MiddlePoint[3];
			double MiddleDirection[3];
			double DeltaThetaX;
			double DeltaThetaY;
			double DeltaX;
			double DeltaY;
			double pr;
			double Ex;
			double Ey;
			double Exy;
			double KinEnergy;
			int EventId;
			int POCAVoxelId;
			int NHitVoxels;
			int VoxelIdList[MAX_VOXELS_EVENT];//safe value
			//double TOF;
			//double TrackLength;
			//double TrackSpeed;	
		};

		struct MPIVoxelPathLengthData{
			double fL;
			double fT;
		};

		struct MPIFitInfo{
			double fLogLikelihood;
			double fEMLogLikelihood;
			double fChi2;
			double fDeltaLogLikelihood;
			double fRelDeltaLogLikelihood;
			double fDeltaEMLogLikelihood;
			double fRMS;
			double fBias;
			double fRMSThreatVoxel;
			double fBiasThreatVoxel;
			double fIterRMS;
			double fResErr;
			double fSResErr;
			double fRelResErr;
			double fRelSResErr;
			double fPixelMinRelResErr;
			double fPixelMaxRelResErr;
			double fPixelMean;
			double fPixelVariance;
		};

		enum EMML_STATUS {eRunning= 1, eFinished= 2, eFinishedWithWarnings= 3, eWarning= 4, eFailure= 5, eCriticalFailure= 6};	

		void SetConfigFileName(std::string filename){fConfigFileName= filename;}
		void SetInputFileName(std::string filename){fInputFileName= filename;}
		void SetOutputFileName(std::string filename){fOutputFileName= filename;}
		void SetLogFileName(std::string filename){fLogFileName= filename;}
		
		void SetClusteringFileName(std::string filename){fClusteringFileName= filename;}

		void SetRemoteUserName(std::string name){fRemoteUserName= name;}
		void SetRemoteHostIp(std::string host){fRemoteHostIp= host;}
		void SetRemotePortNumber(int port){fRemotePortNumber= port;}
		void SetDestinationPath(std::string path){fDestinationPath= path;}
		void SetBatchRun(bool choice){fIsBatchRun= choice;}
		void SetGuiHostName(std::string host){fGuiHostName= host;}
		void SetGuiPort(int port){fGuiPort= port;}
		void SetGuiKey(std::string key){fGuiKey= key;}
		void SetScanId(int id){fScanId= id;}
		void SetContainerId(std::string id){fContainerId= id;}
		void SetTimeStamp(long int time){fTimeStamp= time;}

	signals:
			void finished();
	    void error();
			void statusMessage(QString msg);
			void statusProgress(int progress);
			void alarm();
			void alarmStatusMessage(QString msg);
			void logSig(QString msg);
			void alarmSig(QString msg);
			void commEstablished();


  public slots:
		void process();
		void stop(){;}	
  	void connected() {
			emit commEstablished();	
		}

  	bool sender(QString message);
		

	private:

		struct LOG_MESSAGE {
			std::string taskName;
			std::string command;
			std::string inputFileName;
			std::string outputFileName;
			std::string jobDirName;
			int scanId;
			std::string containerId;
			long int timeStamp;			
			int status;
			std::string logMessage;
			int progress;
			std::string guiKey;
			std::string guiHostName;
			int guiPort;
		};

		
		struct ALARM_MESSAGE {
			std::string taskName;
			std::string command;
			std::string inputFileName;
			std::string outputFileName;
			std::string jobDirName;
			int scanId;
			std::string containerId;
			long int timeStamp;			
			int status;
			std::string alarmMessage;
			std::vector<int> alarmVoxelList;
			std::string guiKey;
			std::string guiHostName;
			int guiPort;
		};

		

		QString makeLogMessage(LOG_MESSAGE msg);
		QString makeAlarmMessage(ALARM_MESSAGE msg);

	
		/**
		* \brief Set config options from parser
		*/
		void SetConfig();
		/**
		* \brief Init data
		*/
		bool Init();
		/**
		* \brief Build G4 detector
		*/
		bool BuildG4Detector();
		/**
		* \brief Voxel finder
		*/
		void VoxelFinder();
		/**
		* \brief Set scattering data
		*/
		void SetScatteringData();
		
		/**
		* \brief Read data event-by-event from input file
		*/
		bool ReadData();
		/**
		* \brief Draw
		*/
		void Draw();
		/**
		* \brief Image reconstructor
		*/
		void ImageReco();
		/**
		* \brief Save to file
		*/
		void Save();
		/**
		* \brief Dump total CPU load
		*/
		void DumpTotalLoad();

		/**
		* \brief Is good event?
		*/
		bool IsGoodEvent();
	
		/**
		* \brief EM algorithm step
		*/
		std::vector<double> EMUpdate(std::vector<double> par, MPIFitInfo& fitInfo,bool isAccelerated);
		/**
		* \brief ShrinkageFunction for regularization
		*/
		double ShrinkageFunction(double t, double tau);
		/**
		* \brief Get material scattering density 
		*/
		double GetMaterialScatteringDensity(std::string name);

		void SetGraphicsStyle();	
		int GetVoxelId(int voxelIdX, int voxelIdY, int voxelIdZ){
			return voxelIdX + fNVoxelX*voxelIdY + fNVoxelX*fNVoxelY*voxelIdZ;
		}//close GetVoxelId()

	private:

		QTcpSocket* socket;
		LOG_MESSAGE fLogMessage;
		ALARM_MESSAGE fAlarmMessage;
	
		volatile bool fIsThreadStopped;
		bool fIsFinished;
		bool fIsFailure;

		std::string fFileName;
		std::string fLogFileName;
		std::string fRemoteUserName;
		int fRemotePortNumber;
		std::string fRemoteHostIp;

		std::string fDestinationPath;
		std::string fConfigFileName;
		bool fIsBatchRun;

		std::string fGuiHostName;
		int fGuiPort;	
		std::string fGuiKey;
		int fMaxRetryAttempts;	
		int fScanId;
		std::string fContainerId;
		long int fTimeStamp;
	

		//## General  
		//std::string fAsciiInputFileName;
		//std::ifstream fAsciiFileStream;	
		//int fVerbosity;
		//int fNEvents;
		//bool fUseTestData;
		//std::string fTestInputFileName;
		//TFile* fTestInputFile;
		//TTree* fTestData;

		std::string fInputFileName;
		std::ifstream fInputFileStream;	
		int fVerbosity;
		int fNEvents;

		std::string fClusteringFileName;
		TFile* fClusteringFile;
		TTree* fClusteringInfo;
		bool fUseClusteringInfo;
		bool fWaitClusteringOutput;
		
		std::string fCurrentDirectory;
		std::string fInputFileName_noPath;
		std::string fInputFileNamePath;
		std::string fVisIVOOutputFileName;
		std::string fVisIVOOutputFileName_noPath;
		std::string fVisIVOVolRendImageFilePrefix;
	
		std::string fVisIVOPointOutputFileName;
		std::string fVisIVOPointOutputFileName_noPath;
		std::string fVisIVOPointImageFilePrefix;
		std::string fVisIVOCommandName;

		//## Detector	
		int fNPlane;
		std::vector<double> fZPlane;
		double fContainerSizeX;
		double fContainerSizeY;
		double fContainerSizeZ;
		double fContainerThickness;
		double fContainerPosX;
		double fContainerPosY;
		double fContainerPosZ;	
		double fContainerXStart;
		double fContainerXEnd;
		double fContainerYStart;
		double fContainerYEnd;
		double fContainerZStart;
		double fContainerZEnd;
		double fVoxelSizeX;
		double fVoxelSizeY;
		double fVoxelSizeZ;
		int fNVoxelX;
		int fNVoxelY;
		int fNVoxelZ;
		int fNVoxel;
		double fSigmaPos;


		//## Algorithm parameters
		double fScatteringDensityInit;
		bool fUseMaxRecEvents;
		double fNMaxRecEvents;
		int fStartEventId;
	 	int fNIterations;
		bool fUseRegularization;
		double fBeta;
	  bool fSmearTrackPoints;
		double fSigmaPosX;
		double fSigmaPosY;
		double fSigmaPosZ;

		
		bool fUseMedianUpdate;
		bool fUseDetectorCovariance;
		double fEx;
		double fEy;
		double fExy;
		bool fUseAcceleration;
		bool fIsGoodAccelerationStep;
		int fNSecants;
		bool fUseResErrStopping;
		double fResErrTolerance;
		int fNIterForResErrStopping;	
		bool fUseLogLikeStopping;
		double fLogLikeTolerance;
		int fNIterForLogLikeStopping;
		bool fUseSquaredResErrStopping;
		double fSquaredResErrTolerance;
		int fNIterForSquaredResErrStopping;	
		
		bool fUseTrueMomentum;
		double fReferenceMomentum;
		double fAverageMomentum;
		double fTrueMomentum;
		bool fUsePOCAInLL;
		bool fUseSideEvents;
		bool fUseTopBottomEvents;

		bool fUseScatteringAngleCut;
		double fScatteringAngleMinCut;	
		double fScatteringAngleMaxCut;
		bool fUseDisplacementCut;
		double fDisplacementMinCut;
		double fDisplacementMaxCut;

		bool fStartWithPOCAImage;
		int fNPOCAPerVoxelCut;
		double fPOCASignalCut;		

		bool fUseEnergyCut;
  	double fEnergyMinCut;	
		double fEnergyMaxCut;

		bool fUseTrackSpeedCut;
  	double fTrackSpeedMinCut;	
		double fTrackSpeedMaxCut;
		bool fSmearTimeOfFlight;
		double fTimeOfFlightResolution;
 
		int fNMinEventInVoxelForUpdate;
		bool fUseFilterVoxelEvents;


		//## Algorithm data
		std::vector<double> fLambda; 
		std::vector<double> fTrueLambda;
		
		int* fNEventPerVoxel;
		std::vector<double> fPOCAVoxelSignal;  	
		std::vector<int> fNPOCAPerVoxel;
		MuonEventData* fMuonEventData;	
		std::vector<MuonEventData*> fMuonEventDataCollection;
		
		std::vector<int> fAlarmVoxelIdList;
		
		std::vector< std::vector<double> > fU;
		std::vector< std::vector<double> > fV;
			
		std::vector<TVector3> fThreatListSize;
    std::vector<TVector3> fThreatListPos;
    std::vector<int> fThreatVoxelId;
    std::vector<std::string> fThreatListMaterial;	
		
		MPIScatData fScatteringData;
		MPIScatData* fScatteringDataList;
		MPIRawData fRawData;
		MPIRawData* fRawDataList;
		MPI_Datatype MPIDataList_type;

		int fEventId;

		
		//## GEANT4 classes
		G4Navigator* fG4Navigator;
		G4UImanager* fUImanager;
		G4RunManager* fRunManager;
		//RecorderBase* fRecorder;
		G4VisManager* fVisManager;


		//## Output data
		std::string fOutputFileName;
		TFile* fOutputFile;
		
		std::ofstream fScatteringDataOutputAsciiFile;
		std::string fScatteringDataOutputFileName;
					
		TFile* fScatteringDataOutputFile;
		TTree* fScatteringDataInfo;		
		double fDeltaThetaX;
		double fDeltaThetaY;	
		double fScatteringAngle;
		double fDeltaX;
		double fDeltaY;
		double fPr;
		int fPOCAVoxelId;
		int fNHitVoxels;
		double fX0;
		double fY0;
		double fZ0;
		double fX;
		double fY;
		double fZ;
		double fDirX0;
		double fDirY0;
		double fDirZ0;
		double fDirX;
		double fDirY;
		double fDirZ;
		double fPOCAX;	
		double fPOCAY;	
		double fPOCAZ;	
		double fKinEnergy;
		int fNTraversedVoxels;
		int fVoxelIdList[MAX_VOXELS_EVENT];
		double fVoxelWx[MAX_VOXELS_EVENT];
		double fVoxelWy[MAX_VOXELS_EVENT];
		double fVoxelWxy[MAX_VOXELS_EVENT];
		
		int fEventN;




		TTree* fInfoTree;

		TTree* fTrueTomographyImageInfo;
		TTree* fRecTomographyImageInfo;
		int fNx;
		int fNy;
		int fNz;	
		int fVoxelId;
		double fVoxelX;
		double fVoxelY;
		double fVoxelZ;
		double fScatteringDensity;
		int fNEventInVoxel;

		
		TTree* fIterationInfo;
		int fIterNo;
		double fLL;
		double fEMLL;
		double fDeltaLL;
		double fDeltaEMLL;
		double fImageRMS;
		double fImageBias;
		double fImageRMSThreatVoxel;
		double fImageBiasThreatVoxel;
		double fImageIterRMS;
		double fImageResErr;
		double fImageSResErr;
		double fImageRelResErr;
		double fImageRelSResErr;
		double fPixelMinRelResErr;
		double fPixelMaxRelResErr;
		double fPixelMean;
		double fPixelVariance;	
		int fNPar;
		double fLambdaIter[1000000];
		double fDeltaLambdaIter[1000000];


		//## Time counters
		/*	
		timespec globalCPUTime;
		timespec initCPUTime;
		timespec datareadCPUTime;
		timespec datatransmissionCPUTime;
		timespec scatdataCPUTime;
		timespec voxelfinderCPUTime;
		timespec datareduceCPUTime;
		timespec collectDataCPUTime;
		timespec imageRecoCPUTime;
		*/
		double globalCPUTime;
		double initCPUTime;
		double datareadCPUTime;
		double datatransmissionCPUTime;
		double scatdataCPUTime;
		double voxelfinderCPUTime;
		double datareduceCPUTime;
		double collectDataCPUTime;
		double imageRecoCPUTime;
		double saveCPUTime;

		TTree* fCPUTimeInfoTree;
		int fNTotEvents;
		int fNReadEvents;
		int fNSelEvents;
		int fNSelEvents_proc;
		int fNEvents_topBottomContainer;
		int fNEvents_sideContainer;
		int fNEvents_outsideContainer;
		int fNEvents_navigationFailure;
		int fNEvents_pocaUnreliable;
		int fNEvents_pocaInsideContainer;	
		int fNIter;
		double fCPUTime_Tot;
		double fCPUTime_Init;
		double fCPUTime_DataRead;
		double fCPUTime_DataTrasmission;
		double fCPUTime_ScatData;
		double fCPUTime_VoxelFinder;
		double fCPUTime_DataReduce;
		double fCPUTime_DataCollect;
		double fCPUTime_ImageReco;
		double fCPUTime_Save;
		
		int nRecStages;
		double progressPercentage;
		double stagePercentage;


		//## Draw	options
		static const int fNColourContours= 2000;
		std::string fDrawMode;
		int fEMLLNEventThreshold;
		double fEMLLSignalThreshold;	
		double fMinEMLLSignal;
		double fMaxEMLLSignal;
		static const int NMAXTOMSECTIONS= 1000;
		
		TH3D* RecMap3D;	
		TH3D* NEventMap3D;
		TH2D* RecMap_XFix[NMAXTOMSECTIONS];
		TH2D* RecMap_YFix[NMAXTOMSECTIONS];
		TH2D* RecMap_ZFix[NMAXTOMSECTIONS];
		TH2D* NEventMap_XFix[NMAXTOMSECTIONS];
		TH2D* NEventMap_YFix[NMAXTOMSECTIONS];
		TH2D* NEventMap_ZFix[NMAXTOMSECTIONS];

		TH1D* fSExpHisto;
		std::vector<TH1D*> fSExpHistoList;
		TGraph* fVoxelThetaGraph;
		std::vector<TGraph*> fVoxelThetaGraphList;
		TGraph* fVoxelDisplacementGraph;
		std::vector<TGraph*> fVoxelDisplacementGraphList;

		TH2D* fDataXHisto;
		std::vector<TH2D*> fDataXHistoList;

		TH1D* dummyXLevelHisto;
		TH1D* dummyYLevelHisto;
		TH1D* dummyZLevelHisto;
		TStyle* myStyle;
		
		static const int MAXRENDIMG= 1000;
		//MyTASImage* VolRendImg[MAXRENDIMG];
		TASImageNew* VolRendImg[MAXRENDIMG];
		TImagePalette* fTemperaturePalette;


		//## MPI options
		int nproc;
		int myid;
		int mapEventsBefore;
		int mapEventsAfter;
		int* mapEvents;

};


#endif 


