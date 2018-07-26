


#ifndef TOMOGRAPHYREC_TAB_MENU_H
#define TOMOGRAPHYREC_TAB_MENU_H

#include <QDialog>
#include <QScienceSpinBox.h>
#include <QObject>


#include <DownloaderThreadObj.h>
#include <POCAThreadObj.h>
#include <EMTomographyRecoThreadObj.h>
#include <ACFAnalysisThreadObj.h>
#include <FOFThreadObj.h>

#include <POCAViewerThreadObj.h>
#include <EMLLViewerThreadObj.h>
#include <ACFViewerThreadObj.h>
#include <ClusteringViewerThreadObj.h>

#include <VolumeRenderingViewer.h>

#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>
#include <QSound>

#include <MyThread.h>

#include <QUrl>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
class QCheckBox;
class QGroupBox;
class QPushButton;
class QToolButton;
class QTextEdit;
class QSpinBox;
class QDoubleSpinBox;
class QComboBox;
class QLineEdit;
class QRadioButton;
class QProgressBar;
class QMovie;
class QLabel;
class QGridLayout;
class QVBoxLayout;

class QUdpSocket;
class QTcpSocket;
class QTcpServer;
class QHostAddress;
class QtNetwork;
class QThread;
class QStatusBar;
class QSpacerItem;

class TomographyRecTabMenu : public QWidget {
     
	Q_OBJECT

 	public:

  	TomographyRecTabMenu(QWidget *parent = 0);
		~TomographyRecTabMenu();

		enum STATUS_CODE {eOK=0, eWARNING=1, eERROR=2, eCRITICAL_ERROR=3};
		enum RUN_CODE {eManual=0, eManualBatch=1, eAuto=2, eAutoBatch=3};
		enum RUN_EXEC_CODE {eLocalInteractive=1, eLocalBatch=2, eRemoteBatch=3};
		
		struct LOG_MESSAGE {
			STATUS_CODE status;	
			std::string msg;
		};
		
	signals:

		void downloadStartSig(QString msg);
		void downloadEndSig(QString msg);	
		void EMMLStartSig(QString msg);
		void POCAStartSig(QString msg);
		void FOFStartSig(QString msg);
		void ACFStartSig(QString msg);
		void viewerStartSig(QString msg);
		

	public slots:

		void downloadStart(QString msg);
		void EMMLStart(QString msg);
		void POCAStart(QString msg);
		void FOFStart(QString msg);
		void ACFStart(QString msg);
		void viewerStart(QString msg);
		void logHandler(QString msg);
		void alarmHandler(QString msg);
		
		void SetTomographyRecOptions();
		
		void ManualRunTomographyReconstruction();
		void ManualBatchRunTomographyReconstruction();
		void Run();
		//void ProcessSocketData();
		void ProcessSocketData(QByteArray);
		void SocketReadyReadAction();
		void BufferizeSocketData();

		
		void SetBusyDownloadStatus();
		void SetBusyPOCARecoStatus();
		void SetBusyEMTomographyRecoStatus();
		void SetBusyACFRecoStatus();
		void SetBusyClusteringRecoStatus();
		void SetInitImageStatus();	
		void SetCompleteDownloadStatus();
		void SetCompletePOCARecoStatus();
		void SetCompleteEMTomographyRecoStatus();
		void SetCompleteACFRecoStatus();
		void SetCompleteClusteringRecoStatus();
		void SetFailureDownloadStatus();
		void SetFailurePOCARecoStatus();
		void SetFailureEMTomographyRecoStatus();
		void SetFailureACFRecoStatus();
		void SetFailureClusteringRecoStatus();
		
		
		void StartPOCAViewer();
		void StartPOCAVolRenderingViewer();
		void StartLLRecoViewer();
		void StartLLVolRenderingViewer();
		void StartClusteringRecoViewer();
		void StartClusteringVolRenderingViewer();
		void StartACFRecoViewer();
		

		void acceptConnection();
		void sendAlarmMessage(QString);
		
		void displayAlarm_POCAReco();
		void displayAlarm_LLReco();
		void displayAlarm_ACFReco();
		void displayAlarm_ClusteringReco();

		void sendAlarm_POCAReco(QString);
		void sendAlarm_LLReco(QString);
		void sendAlarm_ClusteringReco(QString);
		void sendAlarm_ACFReco(QString);

		void playAlarmSound_POCAReco();
		void playAlarmSound_LLReco();
		void playAlarmSound_ACFReco();
		void playAlarmSound_ClusteringReco();
		
		void stopLLReco();
		void stopPOCAReco();
		void stopClusteringReco();
		void stopACFReco();
	
		void EnableRunButton();
		void EnableLLStopButton();
		void EnablePOCAStopButton();
		void EnableACFStopButton();
		void EnableClusteringStopButton();
	
		void EnablePOCAViewerButton();
		void EnableLLRecoViewerButton();
		void EnableClusteringRecoViewerButton();
		void EnableACFRecoViewerButton();
		
		void serviceReplyFinished(QNetworkReply*);
		
	signals:
		void alarm(QString);
		
	public:
		
		
		
			
	private:

		QTabWidget* fTomographyMainTabMenu;
		
		QPushButton* pushButton_Run;
		QPushButton* pushButton_ManualRun;	
		QCheckBox* checkBox_runMode;
		QCheckBox* checkBox_batchMode;	
		QComboBox* comboBox_runExecMode;

		int fRunMode;
		int fRunExecMode;

		bool fIsServerRunning;

		static const int MAXNSCANS= 3;
		//static const int MAXNSCANS= 1;
		QGroupBox* groupBoxList_display[MAXNSCANS];
		QProgressBar* progressBarList_fileStatus[MAXNSCANS];
		QProgressBar* progressBarList_LLRecoStatus[MAXNSCANS];
		QProgressBar* progressBarList_POCARecoStatus[MAXNSCANS];
		QProgressBar* progressBarList_ACFRecoStatus[MAXNSCANS];
		QProgressBar* progressBarList_ClusteringRecoStatus[MAXNSCANS];

		QPushButton* pushButtonList_LLRecoStatus[MAXNSCANS];
		QPushButton* pushButtonList_POCARecoStatus[MAXNSCANS];
		QPushButton* pushButtonList_ACFRecoStatus[MAXNSCANS];
		QPushButton* pushButtonList_ClusteringRecoStatus[MAXNSCANS];

		QPushButton* pushButtonList_POCAVolRendering[MAXNSCANS];
		QPushButton* pushButtonList_LLVolRendering[MAXNSCANS];
		QPushButton* pushButtonList_ClusteringVolRendering[MAXNSCANS];
		

		QPushButton* stopThreadButtonList_LLRecoStatus[MAXNSCANS];
		QPushButton* stopThreadButtonList_POCARecoStatus[MAXNSCANS];
		QPushButton* stopThreadButtonList_ACFRecoStatus[MAXNSCANS];
		QPushButton* stopThreadButtonList_ClusteringRecoStatus[MAXNSCANS];


		QLabel* imageLabelList_fileStatus[MAXNSCANS];
		QLabel* imageLabelList_LLRecoStatus[MAXNSCANS];
		QLabel* imageLabelList_POCARecoStatus[MAXNSCANS];
		QLabel* imageLabelList_ACFRecoStatus[MAXNSCANS];	
		QLabel* imageLabelList_ClusteringRecoStatus[MAXNSCANS];
		QLabel* imageLabelList_Alarm_POCAReco[MAXNSCANS];
		QLabel* imageLabelList_AlarmText_POCAReco[MAXNSCANS];	
		QLabel* imageLabelList_Alarm_EMTomographyReco[MAXNSCANS];
		QLabel* imageLabelList_AlarmText_EMTomographyReco[MAXNSCANS];
		QLabel* imageLabelList_Alarm_ACFReco[MAXNSCANS];
		QLabel* imageLabelList_AlarmText_ACFReco[MAXNSCANS];
		QLabel* imageLabelList_Alarm_ClusteringReco[MAXNSCANS];
		QLabel* imageLabelList_AlarmText_ClusteringReco[MAXNSCANS];
		QMovie* movieList_fileStatus_Red[MAXNSCANS];
		QMovie* movieList_fileStatus_Green[MAXNSCANS];
		QMovie* movieList_LLRecoStatus_Red[MAXNSCANS];
		QMovie* movieList_LLRecoStatus_Green[MAXNSCANS];
		QMovie* movieList_POCARecoStatus_Red[MAXNSCANS];
		QMovie* movieList_POCARecoStatus_Green[MAXNSCANS];
		QMovie* movieList_ACFRecoStatus_Red[MAXNSCANS];
		QMovie* movieList_ACFRecoStatus_Green[MAXNSCANS];
		QMovie* movieList_ClusteringRecoStatus_Red[MAXNSCANS];
		QMovie* movieList_ClusteringRecoStatus_Green[MAXNSCANS];

		QMovie* movieList_fileStatus_StaticRed[MAXNSCANS];
		QMovie* movieList_LLRecoStatus_StaticRed[MAXNSCANS];
		QMovie* movieList_POCARecoStatus_StaticRed[MAXNSCANS];
		QMovie* movieList_ACFRecoStatus_StaticRed[MAXNSCANS];
		QMovie* movieList_ClusteringRecoStatus_StaticRed[MAXNSCANS];

		QMovie* movieList_Alarm_POCAReco[MAXNSCANS];
		QMovie* movieList_AlarmText_POCAReco[MAXNSCANS];
		QMovie* movieList_Alarm_EMTomographyReco[MAXNSCANS];
		QMovie* movieList_AlarmText_EMTomographyReco[MAXNSCANS];
		QMovie* movieList_Alarm_ACFReco[MAXNSCANS];
		QMovie* movieList_AlarmText_ACFReco[MAXNSCANS];
		QMovie* movieList_Alarm_ClusteringReco[MAXNSCANS];
		QMovie* movieList_AlarmText_ClusteringReco[MAXNSCANS];

		QLabel* labelList_statusBar_fileStatus[MAXNSCANS];
		QStatusBar* statusBarList_fileStatus[MAXNSCANS];
		QLabel* labelList_statusBar_LLRecoStatus[MAXNSCANS];
		QStatusBar* statusBarList_LLRecoStatus[MAXNSCANS];
		QLabel* labelList_statusBar_POCARecoStatus[MAXNSCANS];
		QStatusBar* statusBarList_POCARecoStatus[MAXNSCANS];
		QLabel* labelList_statusBar_ACFRecoStatus[MAXNSCANS];
		QStatusBar* statusBarList_ACFRecoStatus[MAXNSCANS];
		QLabel* labelList_statusBar_ClusteringRecoStatus[MAXNSCANS];
		QStatusBar* statusBarList_ClusteringRecoStatus[MAXNSCANS];

		QLabel* labelList_fileStatus[MAXNSCANS];
		QLabel* labelList_LLRecoStatus[MAXNSCANS];
		QLabel* labelList_POCARecoStatus[MAXNSCANS];
		QLabel* labelList_ACFRecoStatus[MAXNSCANS];
		QLabel* labelList_ClusteringRecoStatus[MAXNSCANS];
	
		QGridLayout* DisplayBoxGroupLayout[MAXNSCANS];
		QSpacerItem* emptyspace[MAXNSCANS];

		int fCurrentScanIndex;

		QTcpSocket* tcpSocket;
		QTcpServer* server;
		QTcpSocket* client;
		
		QString ACQIpAddress;
		int ACQPortNumber;
		QString CCMIpAddress;
		int CCMPortNumber;

		QString fSocketIP;
		qint16 fSocketPortNumber;
		std::string fRemoteUser;
		std::string fRemoteHostIp;
		std::string fRemotePortNumber;
		std::string fRemotePathName; 
		std::string fDestinationPath;
		int fNMaxAttempts;

		QThread* downloaderThread;
		DownloaderThreadObj* downloaderThreadObj;
		QThread* pocaRecoThread;
		POCAThreadObj* pocaRecoThreadObj;
		//MyThread* emTomographyRecoThread;
		QThread* emTomographyRecoThread;
		EMTomographyRecoThreadObj* emTomographyRecoThreadObj;
		QThread* acfAnalysisThread;
		ACFAnalysisThreadObj* acfAnalysisThreadObj;
		QThread* fofThread;
		FOFThreadObj* fofThreadObj;
		
		Phonon::MediaObject* mMediaObject;
    Phonon::AudioOutput* mAudioOutput;
		QSound* alarmSound;
		Phonon::MediaObject* mMediaObject_POCAReco[MAXNSCANS];
		Phonon::MediaObject* mMediaObject_LLReco[MAXNSCANS];
		Phonon::MediaObject* mMediaObject_ClusteringReco[MAXNSCANS];
		Phonon::MediaObject* mMediaObject_ACFReco[MAXNSCANS];
		Phonon::AudioOutput* mAudioOutput_POCAReco[MAXNSCANS];
		Phonon::AudioOutput* mAudioOutput_LLReco[MAXNSCANS];
		Phonon::AudioOutput* mAudioOutput_ClusteringReco[MAXNSCANS];
		Phonon::AudioOutput* mAudioOutput_ACFReco[MAXNSCANS];
		
		std::string fPOCAOutputFileNameList[MAXNSCANS];
		std::string fACFOutputFileNameList[MAXNSCANS];
		std::string fClusteringOutputFileNameList[MAXNSCANS];
		std::string fEMLLOutputFileNameList[MAXNSCANS];

		QUrl* webServiceUrl;
		QNetworkAccessManager* networkManager;
		QNetworkRequest* networkRequest;


		QVector<QPushButton*> pushButtonList_POCAViewer;
		QVector<QPushButton*> pushButtonList_ACFViewer;
		QVector<QPushButton*> pushButtonList_LLViewer;
		QVector<QPushButton*> pushButtonList_ClusteringViewer;

		QByteArray fBuffer; // global or member variable

};






#endif
