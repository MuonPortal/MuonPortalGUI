#ifndef ACFANALYSIS_THREAD_OBJ_H
#define ACFANALYSIS_THREAD_OBJ_H

#include <QDialog>
#include <QObject>


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

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;



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


class ACFAnalysisThreadObj : public QObject {
    
	Q_OBJECT
     
  public:

  	ACFAnalysisThreadObj();
		~ACFAnalysisThreadObj();
   
		enum ACF_STATUS {eRunning= 1, eFinished= 2, eFinishedWithWarnings= 3, eWarning= 4, eFailure= 5, eCriticalFailure= 6};	


		void SetRandomDataFileName(std::string filename){fRandomDataFileName= filename;}
			
		void SetInputFileName(std::string filename){fInputFileName= filename;}
		void SetOutputFileName(std::string filename){fOutputFileName= filename;}
		void SetDestinationPath(std::string path){fDestinationPath= path;}
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


  public slots:
		void process();
  	bool sender(QString message);
  	

	private:

		void SetGraphicsStyle();
		bool Init();
		bool ReadData();
		bool Draw();
		bool Save();
		bool ACFRun();
		
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
			std::vector<double> alarmRScaleList;
			std::string guiKey;
			std::string guiHostName;
			int guiPort;
		};

		QString makeLogMessage(LOG_MESSAGE msg);
		QString makeAlarmMessage(ALARM_MESSAGE msg);

	
		QTcpSocket* socket;
		LOG_MESSAGE fLogMessage;
		ALARM_MESSAGE fAlarmMessage;

		std::string fInputFileName;
		std::string fRandomDataFileName;
		TFile* fInputFile;
		TFile* fRandomDataFile;

		TFile* fOutputFile;
		std::string fOutputFileName;
		
		int fVerbosity;
		std::string fDestinationPath;
		std::string fGuiHostName;
		int fGuiPort;	
		std::string fGuiKey;	
		int fScanId;
		std::string fContainerId;
		long int fTimeStamp;

		std::vector<double> fAlarmRScaleList;

		double progressPercentage;
		double stagePercentage;
		int nRecStages;
		bool fIsFailure;

		double fContainerSizeX;
		double fContainerSizeY;
		double fContainerSizeZ;
		double fContainerXStart;
		double fContainerXEnd;
		double fContainerYStart;
		double fContainerYEnd;
		double fContainerZStart;
		double fContainerZEnd;
		bool fUseThetaCut;
		double fThetaMin;
		double fThetaMax;
		bool fUseThetaWeight;
		double fThetaWeight;
	
		bool fUseEnergyCut;
		double fEnergyMinCut;
		double fEnergyMaxCut;
	
		bool fUseMaxNEvents;
		int fMaxNEvents;
		double fSignalThreshold;

		int fNVoxelX;
		int fNVoxelY;
		int fNVoxelZ;
		int fNVoxel;
		double fVoxelSizeX;
		double fVoxelSizeY;
		double fVoxelSizeZ;

		int fNCrownX;
		int fNCrownY;
		int fNCrownZ;
	
		int fNTotBins;
	
		double fRScaleStep;
		double fRScaleMin;
		double fRScaleMax;


		double fDD;
		double fRR;
		double fDR;	
		double fN;
		double fNr;
		double fNWeighted;
		double fNrWeighted;
		double fDDWeighted;
		double fRRWeighted;
		double fDRWeighted;
		TH1D* fDDPairHisto;
		TH1D* fRRPairHisto;
		TH1D* fDRPairHisto;
		TH1D* fDDPairWeightedHisto;
		TH1D* fRRPairWeightedHisto;
		TH1D* fDRPairWeightedHisto;
		TGraph* fACFGraph_LS;
		TGraph* fACFGraph_H;
		TGraph* fACFGraph_DP;
		TGraph* fACFGraph_PH;
		TGraph* fACFWeightedGraph_LS;
		TGraph* fACFWeightedGraph_H;
		TGraph* fACFWeightedGraph_DP;
		TGraph* fACFWeightedGraph_PH;
	
		struct VoxelData{
			std::vector<TVector3> fPointList;
			std::vector<double> fThetaList;
		};
		std::vector<VoxelData> fVoxelDataList;
		std::vector<VoxelData> fVoxelRandomDataList;
		TH3D* fDataMap3D;
		TH3D* fRandomDataMap3D;


		TStyle* myStyle;
		static const int fNColourContours= 2000;
		
};

#endif
