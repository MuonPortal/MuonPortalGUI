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
* @file FOFThreadObj.h
* @class FOFThreadObj
* @brief Friend-of-friends (FOF) algorithm processing thread
*
* @author S. Riggi
* @date 21/02/2012
*/
#ifndef FOF_THREAD_OBJ_H
#define FOF_THREAD_OBJ_H

#include <TASImageNew.h>

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

class FOFThreadObj : public QObject {
    
	Q_OBJECT
     
  public:

  	FOFThreadObj();
		~FOFThreadObj();
   
		enum FOF_STATUS {eRunning= 1, eFinished= 2, eFinishedWithWarnings= 3, eWarning= 4, eFailure= 5, eCriticalFailure= 6};	


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
			std::vector<int> alarmClusterList;
			std::string guiKey;
			std::string guiHostName;
			int guiPort;
		};

		QString makeLogMessage(LOG_MESSAGE msg);
		QString makeAlarmMessage(ALARM_MESSAGE msg);


		void SetGraphicsStyle();
		bool Init();
		bool ReadData();
		bool Draw();
		bool Save();
		bool Run();
		void SetConfig();

		#define MAXDIM 3
		//#define forever for(;;)
		typedef float Real;
    	int ndim ;
    	int nsph ;
    	int ndark ;
    	int nstar ;

		struct poca_event {
    	float r[3];
    	float theta;
    	int id_event;
    	float thetaQ;
    	float energy;
		};

		struct gas_particle {
    	Real mass;
    	Real pos[MAXDIM];
    	Real vel[MAXDIM];
    	Real rho;
    	Real temp;
    	Real hsmooth;
    	Real metals ;
    	Real phi ;
		};

		struct gas_particle *gas_particles;

		struct dark_particle {
    	Real mass;
    	Real pos[MAXDIM];
    	Real vel[MAXDIM];
    	Real eps;
    	Real phi ;
		} ;

		struct dark_particle *dark_particles;

		struct star_particle {
    	Real mass;
    	Real pos[MAXDIM];
    	Real vel[MAXDIM];
    	Real metals ;
    	Real tform ;
    	Real eps;
    	Real phi ;
		} ;

		struct star_particle *star_particles;

		struct dump {
    	int npoca ;
		} ;

		struct dump header ;

		#define fROOT		1
#define LOWER(i)	(i<<1)
#define UPPER(i)	((i<<1)+1)
#define PARENT(i)	(i>>1)
#define SIBLING(i) 	((i&1)?i-1:i+1)
#define SETNEXT(i)\
{\
	while (i&1) i=i>>1;\
	++i;\
	}

#define DARK	1
#define GAS		2
#define STAR	4

#define KD_ORDERTEMP	256

typedef struct poca {
	
    float r[3];
		float angle;
    int eventId;
    int iGroup;
    int iOrder;
	} POCA;


typedef struct bndBound {
	float fMin[3];
	float fMax[3];
	} BND;

typedef struct kdNode {
	float fSplit;
	BND bnd;
	int iDim;
	int pLower;
	int pUpper;
	} KDN;

typedef struct kdContext { //significato di questi elementi?
	
    int nBucket;//rimesse --->rimettere!!! =16
	int nParticles; //rimesse
    int nPoca;//equivalente di nDark----> nPoca
	int bPoca; //equivalente di bDark mentre bGas e bStar si possono eliminare
	int nActive; //equivalente a quello di prima
	POCA *p; //eequivalente di PARTICLE*p
	KDN *kdNodes;
    //mb: aggiunti da me
    float fTime;
	float fPeriod[3]; //sarebbero px, py e pz
    int nLevels;
	int nNodes;
	int nSplit;
    int nGroup;
	int uSecond;
	int uMicro;
	} * KD;


#define INTERSECT(c,cp,fBall2,lx,ly,lz,x,y,z,sx,sy,sz)\
{\
	float dx,dy,dz,dx1,dy1,dz1,fDist2,fMax2;\
	dx = c[cp].bnd.fMin[0]-x;\
	dx1 = x-c[cp].bnd.fMax[0];\
	dy = c[cp].bnd.fMin[1]-y;\
	dy1 = y-c[cp].bnd.fMax[1];\
	dz = c[cp].bnd.fMin[2]-z;\
	dz1 = z-c[cp].bnd.fMax[2];\
	if (dx > 0.0) {\
		if (dx1+lx < dx) {\
			dx1 += lx;\
			dx -= lx;\
			sx = x+lx;\
			fDist2 = dx1*dx1;\
			fMax2 = dx*dx;\
			}\
		else {\
			sx = x;\
			fDist2 = dx*dx;\
			fMax2 = dx1*dx1;\
			}\
		if (fDist2 > fBall2) goto GetNextCell;\
		}\
	else if (dx1 > 0.0) {\
		if (dx+lx < dx1) {\
		    dx += lx;\
			dx1 -= lx;\
			sx = x-lx;\
			fDist2 = dx*dx;\
			fMax2 = dx1*dx1;\
			}\
		else {\
			sx = x;\
			fDist2 = dx1*dx1;\
			fMax2 = dx*dx;\
			}\
		if (fDist2 > fBall2) goto GetNextCell;\
		}\
	else {\
		sx = x;\
		fDist2 = 0.0;\
		if (dx < dx1) fMax2 = dx*dx;\
		else fMax2 = dx1*dx1;\
		}\
	if (dy > 0.0) {\
		if (dy1+ly < dy) {\
		    dy1 += ly;\
			dy -= ly;\
			sy = y+ly;\
			fDist2 += dy1*dy1;\
			fMax2 += dy*dy;\
			}\
		else {\
			sy = y;\
			fDist2 += dy*dy;\
			fMax2 += dy1*dy1;\
			}\
		if (fDist2 > fBall2) goto GetNextCell;\
		}\
	else if (dy1 > 0.0) {\
		if (dy+ly < dy1) {\
		    dy += ly;\
			dy1 -= ly;\
			sy = y-ly;\
			fDist2 += dy*dy;\
			fMax2 += dy1*dy1;\
			}\
		else {\
			sy = y;\
			fDist2 += dy1*dy1;\
			fMax2 += dy*dy;\
			}\
		if (fDist2 > fBall2) goto GetNextCell;\
		}\
	else {\
		sy = y;\
		if (dy < dy1) fMax2 += dy*dy;\
		else fMax2 += dy1*dy1;\
		}\
	if (dz > 0.0) {\
		if (dz1+lz < dz) {\
		    dz1 += lz;\
            dz -= lz;\
			sz = z+lz;\
			fDist2 += dz1*dz1;\
			fMax2 += dz*dz;\
			}\
		else {\
			sz = z;\
			fDist2 += dz*dz;\
			fMax2 += dz1*dz1;\
			}\
		if (fDist2 > fBall2) goto GetNextCell;\
		}\
	else if (dz1 > 0.0) {\
		if (dz+lz < dz1) {\
			dz += lz;\
		    dz1 -= lz;\
			sz = z-lz;\
			fDist2 += dz*dz;\
			fMax2 += dz1*dz1;\
			}\
		else {\
			sz = z;\
			fDist2 += dz1*dz1;\
			fMax2 += dz*dz;\
			}\
		if (fDist2 > fBall2) goto GetNextCell;\
		}\
	else {\
		sz = z;\
		if (dz < dz1) fMax2 += dz*dz;\
		else fMax2 += dz1*dz1;\
		}\
	if (fMax2 < fBall2) goto ContainedCell;\
	}

		int contaRighe (FILE *fp);
		void kdTime(KD,int *,int *);
		int kdInit(KD *,int,float *);
		//void kdReadTipsy(KD,FILE *,int,int,int);
		void kdReadMuons(KD kd, FILE *fp, int bPoca);
		void kdBuildTree(KD);
		int kdFoF(KD,float);
		int kdTooSmall(KD,int);
		void kdOrder(KD);
		void kdOutGroup(KD kd,char *pszFile,FILE *ip);
		void kdFinish(KD);
		void kdSelect(KD kd,int d,int k,int l,int r);
		void kdCombine(KDN *p1,KDN *p2,KDN *pOut);
		void kdUpPass(KD kd,int iCell);
		static int CmpParticles(const void *v1,const void *v2);

	private:

		QTcpSocket* socket;
		LOG_MESSAGE fLogMessage;
		ALARM_MESSAGE fAlarmMessage;

		std::string fInputFileName;
		TFile* fInputFile;
		TFile* fRandomDataFile;

		TFile* fOutputFile;
		std::string fOutputFileName;
		
		std::string fDestinationPath;
		std::string fGuiHostName;
		int fGuiPort;	
		std::string fGuiKey;	
		int fScanId;
		std::string fContainerId;
		long int fTimeStamp;

		TH3D* RecMap3D;
		TH3D* NEventMap3D;

		std::string fCurrentDirectory;
		std::string fInputFileName_noPath;
		std::string fInputFileNamePath;
		std::string fVisIVOPointOutputFileName;	
		std::string fVisIVOPointOutputFileName_noPath;
		std::string fVisIVOPointImageFilePrefix;
		std::string fVisIVOCommandName;

		double progressPercentage;
		double stagePercentage;
		int nRecStages;
		bool fIsFailure;

		bool fWaitPOCAOutput;
		int fVerbosity;
		double fContainerSizeX;
		double fContainerSizeY;
		double fContainerSizeZ;
		double fContainerXStart;
		double fContainerXEnd;
		double fContainerYStart;
		double fContainerYEnd;
		double fContainerZStart;
		double fContainerZEnd;	
		
		bool fUseMaxNEvents;	
		int fMaxNEvents;	
		bool fUseThetaCut;	
		double fThetaMin;
		double fThetaMax;
		bool fUseEnergyCut;
		double fEnergyMinCut;
		double fEnergyMaxCut;

		bool fUseThetaWeight;
		double fThetaWeight;
		double fSignalThreshold;
		bool fUseAverageSignal;
		int fNEventThreshold;
		int fNVoxelX;
		int fNVoxelY;
		int fNVoxelZ;
		int fNVoxel;
		double fVoxelSizeX;
		double fVoxelSizeY;
		double fVoxelSizeZ;
		int fNSelEvents;
		
		std::vector<TVector3> pointList;
		std::vector<double> angleList;
		std::vector<int> eventIdList;
		std::vector<int> fAlarmClusterIdList;

		double fEps;
		int fMinNPts;
		int nBucket;
		float fPeriod[3];
		int nGroup;
		int bVerbose;
		int bPoca;
		int nMembers;
		int sec;
		int usec;
		KD kd;
		FILE *ip;

		TTree* fDataTree;
		double fEventWeight;
		int fEventId;
		TTree* fInfoTree;
		int fNClusterGroups;

		TTree* fClusterInfoTree;
		/*
		static const int MAXNPOINTSPERCLUSTER= 1000000;
		double fPOCAX[MAXNPOINTSPERCLUSTER];
		double fPOCAY[MAXNPOINTSPERCLUSTER];
		double fPOCAZ[MAXNPOINTSPERCLUSTER];
		double fTheta[MAXNPOINTSPERCLUSTER];
		int fClusterId;
		int fNClusterPoints;
		*/
		int fClusterId;
		double fPOCAX;
		double fPOCAY;
		double fPOCAZ;
		double fTheta;
		
		TStyle* myStyle;
		static const int fNColourContours= 2000;

		static const int MAXIMG=1000;
		TASImageNew* PointImg[MAXIMG];
		
};//close class

}//close namespace

#endif


