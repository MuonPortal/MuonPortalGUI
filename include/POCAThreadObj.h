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
* @file POCAThreadObj.h
* @class POCAThreadObj
* @brief POCA algorithm processing thread
* 
* @author S. Riggi
* @date 25/04/2010
*/
#ifndef POCAThreadObj_h
#define POCAThreadObj_h 1

#include <ConfigParser.h>
#include <TASImageNew.h>

#include <QDialog>
#include <QObject>

#include <vector>
#include <map>
#include <string>
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
#include <TImage.h>


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

namespace MuonPortalNS {

class POCAThreadObj: public QObject {
    
	Q_OBJECT

  public:
		
		/** 
		\brief Class constructor: initialize structures.
 		*/
    POCAThreadObj();
		/**
		* \brief Class destructor: free allocated memory
		*/
   ~POCAThreadObj();

		enum POCA_STATUS {eRunning= 1, eFinished= 2, eFinishedWithWarnings= 3, eWarning= 4, eFailure= 5, eCriticalFailure= 6};	


		//## Raw data
		struct POCARawData{
			int EventId;
			double HitX[4];
			double HitY[4];
			double p;
		};	

		
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
  	bool sender(QString message);
  	void connected() {
			emit commEstablished();	
		}

	public:

		void SetConfig();
		bool Init();
		bool ReadData();
		bool Run();
		bool Save();
		bool Draw();
		void SetGraphicsStyle();	

		void SetInputFileName(std::string filename){fInputFileName= filename;}
		void SetOutputFileName(std::string filename){fOutputFileName= filename;}
		void SetDestinationPath(std::string path){fDestinationPath= path;}
		void SetGuiHostName(std::string host){fGuiHostName= host;}
		void SetGuiPort(int port){fGuiPort= port;}
		void SetGuiKey(std::string key){fGuiKey= key;}
		void SetScanId(int id){fScanId= id;}
		void SetContainerId(std::string id){fContainerId= id;}
		void SetTimeStamp(long int time){fTimeStamp= time;}


		int GetVoxelId(int voxelIdX, int voxelIdY, int voxelIdZ){
			return voxelIdX + fNVoxelX*voxelIdY + fNVoxelX*fNVoxelY*voxelIdZ;
		}//close GetVoxelId()
	
		
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


		
		QTcpSocket* socket;
		LOG_MESSAGE fLogMessage;
		ALARM_MESSAGE fAlarmMessage;

	
		std::ifstream fInputFileStream;	
		std::string fInputFileName;
		TFile* fOutputFile;
		TTree* fScatteringDataInfo;
		TTree* fInfoTree;
		std::string fOutputFileName;
		std::string fCurrentDirectory;
		std::string fInputFileName_noPath;
		std::string fInputFileNamePath;

		std::string fDestinationPath;
		std::string fGuiHostName;
		int fGuiPort;	
		std::string fGuiKey;
		int fScanId;
		std::string fContainerId;
		long int fTimeStamp;


		std::string fVisIVOOutputFileName;
		std::string fVisIVOOutputFileName_noPath;
		std::string fVisIVOVolRendImageFilePrefix;
	
		std::string fVisIVOPointOutputFileName;
		std::string fVisIVOPointOutputFileName_noPath;
		std::string fVisIVOPointImageFilePrefix;
		std::string fVisIVOCommandName;
		int fVerbosity;
		bool fIsFailure;

		double progressPercentage;
		double stagePercentage;
		int nRecStages;

		int fEventId;
		double fKinEnergy;
		double fDMin;
		double fScatteringAngle;
		double fPOCAX;
		double fPOCAY;
		double fPOCAZ;
		
		POCARawData* fRawDataList;	

		//## Detector	
		int fNPlane;
		std::vector<double> fZPlane;
		double fContainerSizeX;
		double fContainerSizeY;
		double fContainerSizeZ;
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
		int fNEvents;
		int fNReadEvents;
		int fNSelEvents;
		bool fUseNMaxRecEvents;
		double fNMaxRecEvents;
		bool fUseScatteringAngleCut;
		double fScatteringAngleMinCut;	
		double fScatteringAngleMaxCut;
		bool fSmearTrackPoints;
		double fSigmaPosX;
		double fSigmaPosY;
		double fSigmaPosZ;
		double fPOCATolerance;

		bool fUseEnergyCut;
		double fEnergyMinCut;
		double fEnergyMaxCut;
	

		TVector3 fPOCA;
		TVector3 fPOCAParallel;

		std::vector<int> fAlarmVoxelIdList;


		//## Draw	
		static const int fNColourContours= 2000;
		std::string fDrawMode;
		bool fUseAveragePOCASignal;
		int fPOCANEventThreshold;
		double fPOCASignalThreshold;	
		bool fUsePOCAThetaWeight;	
		double fPOCAThetaWeight;
		double fMinPOCASignal;
		double fMaxPOCASignal;
		static const int NMAXTOMSECTIONS= 1000;
		TCanvas* RecMap3DPlot;
		TCanvas* RecMapPlot_XFix[NMAXTOMSECTIONS];
		TCanvas* RecMapPlot_YFix[NMAXTOMSECTIONS];		
		TCanvas* RecMapPlot_ZFix[NMAXTOMSECTIONS];
		TH3D* RecMap3D;	
		TH3D* NEventMap3D;
		TH2D* RecMap_XFix[NMAXTOMSECTIONS];
		TH2D* RecMap_YFix[NMAXTOMSECTIONS];
		TH2D* RecMap_ZFix[NMAXTOMSECTIONS];
		TH2D* NEventMap_XFix[NMAXTOMSECTIONS];
		TH2D* NEventMap_YFix[NMAXTOMSECTIONS];
		TH2D* NEventMap_ZFix[NMAXTOMSECTIONS];
		TH1D* dummyXLevelHisto;
		TH1D* dummyYLevelHisto;
		TH1D* dummyZLevelHisto;
		TStyle* myStyle;

		TASImageNew* RecImg_XFix[NMAXTOMSECTIONS];
		TASImageNew* RecImg_YFix[NMAXTOMSECTIONS];
		TASImageNew* RecImg_ZFix[NMAXTOMSECTIONS];

		static const int MAXRENDIMG=1000;
		TASImageNew* VolRendImg[MAXRENDIMG];
		TImagePalette* fTemperaturePalette;
		
};//close class

}//close namespace

#endif 

