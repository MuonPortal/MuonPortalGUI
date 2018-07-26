#ifndef EMTOMOGRAPHYRECO_THREAD_OBJ_H
#define EMTOMOGRAPHYRECO_THREAD_OBJ_H



#include <QObject>

#include <VoxelData.h>
#include <MuonEventData.h>

#include <TASImageNew.h>
//#include <MyTASImage.h>

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

using namespace std;


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


class EMTomographyRecoThreadObj : public QObject {
    
	Q_OBJECT
     
  public:

  	EMTomographyRecoThreadObj();
		~EMTomographyRecoThreadObj();

		enum EMML_STATUS {eRunning= 1, eFinished= 2, eFinishedWithWarnings= 3, eWarning= 4, eFailure= 5, eCriticalFailure= 6};	


		//## Raw data
		struct RawData{
			int EventId;
			double HitX[4];
			double HitY[4];
			double p;
		};	

		//## Scattering data
		struct ScatData{
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
			//double TOF;
			//double TrackLength;
			//double TrackSpeed;	
		};

		struct VoxelPathLengthData{
			double fL;
			double fT;
		};

		struct FitInfo{
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
		void stop(){
			cout<<"====  SToP ===="<<endl;
			fIsThreadStopped= true;
		}
  	void connected() {
			emit commEstablished();	
		}

  	/**
		* \brief Communicate message through socket
		*/
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
		* \brief Image reconstructor
		*/
		void ImageReco();
		/**
		* \brief Draw
		*/
		void Draw();
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
		std::vector<double> EMUpdate(std::vector<double> par, FitInfo& fitInfo,bool isAccelerated);
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
		std::vector<int> fNEventPerVoxel;
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
		
		ScatData fScatteringData;
		RawData fRawData;

		int fEventId;

		//## GEANT4 classes
		G4Navigator* fG4Navigator;
		G4UImanager* fUImanager;
		G4RunManager* fRunManager;
		G4VisManager* fVisManager;

		//## Output data
		std::string fOutputFileName;
		TFile* fOutputFile;
		
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
		int fVoxelIdList[1000];
		double fVoxelWx[1000];
		double fVoxelWy[1000];
		double fVoxelWxy[1000];
		
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
		timespec globalCPUTime;
		timespec initCPUTime;
		timespec collectDataCPUTime;
		timespec imageRecoCPUTime;

		TTree* fCPUTimeInfoTree;
		int fNTotEvents;
		int fNReadEvents;
		int fNSelEvents;
		int fNEvents_topBottomContainer;
		int fNEvents_sideContainer;
		int fNEvents_outsideContainer;
		int fNEvents_navigationFailure;
		int fNEvents_pocaUnreliable;
		int fNEvents_pocaInsideContainer;	
		int fNIter;
		double fCPUTime_Tot;
		double fCPUTime_Init;
		double fCPUTime_DataCollect;
		double fCPUTime_ImageReco;

		int nRecStages;
		double progressPercentage;
		double stagePercentage;

		//## Draw	
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
		

};


#endif
