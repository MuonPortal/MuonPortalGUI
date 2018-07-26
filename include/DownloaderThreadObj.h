#ifndef DOWNLOADER_THREAD_OBJ_H
#define DOWNLOADER_THREAD_OBJ_H

#include <QDialog>
#include <QObject>


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
class QHostAddress;
class QtNetwork;
class QThread;



class DownloaderThreadObj : public QObject {
    
	Q_OBJECT
     
  public:
  	DownloaderThreadObj();
		~DownloaderThreadObj();
     
		enum DOWNLOAD_STATUS {eRunning= 1, eFinished= 2, eFinishedWithWarnings= 3, eWarning= 4, eFailure= 5, eCriticalFailure= 6};	


		void SetFileName(std::string filename){fFileName= filename;}
		
		
		void SetRemoteUserName(std::string name){fRemoteUserName= name;}
		void SetRemoteHostIp(std::string host){fRemoteHostIp= host;}
		void SetRemotePortNumber(std::string port){fRemotePortNumber= port;}
		void SetDestinationPath(std::string path){fDestinationPath= path;}
		void SetMaxNAttempts(int n){fMaxNAttempts= n;}

		void SetInputFileName(std::string filename){fInputFileName= filename;}
		void SetOutputFileName(std::string filename){fOutputFileName= filename;}
		void SetRemoteHostName(std::string host){fRemoteHostName= host;}
		void SetRemotePort(int port){fRemotePort= port;}
		void SetGuiHostName(std::string host){fGuiHostName= host;}
		void SetGuiPort(int port){fGuiPort= port;}
		void SetGuiKey(std::string key){fGuiKey= key;}
		void SetMaxRetryAttempts(int n){fMaxRetryAttempts= n;}
		void SetLocalDownload(bool choice){fIsLocalDownload= choice;}
		void SetScanId(int id){fScanId= id;}
		void SetContainerId(std::string id){fContainerId= id;}
		void SetTimeStamp(long int time){fTimeStamp= time;}
		

	signals:
    	void finished();
	    void error();
			void statusMessage(QString msg);
			void finishedMessage(QString msg);
			void downloadProgress(int progress);
			void logSig(QString msg);
			void commEstablished();

  public slots:
    void process();
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


		bool init();
		QString makeLogMessage(LOG_MESSAGE msg);
		

	private:


		QTcpSocket* socket;
		LOG_MESSAGE fLogMessage;

		bool fIsLocalDownload;
		std::string fFileName;
		std::string fRemoteUserName;
		std::string fRemotePortNumber;
		std::string fRemoteHostIp;
		std::string fDestinationPath;
		int fMaxNAttempts;
		

		std::string fInputFileName;
		std::string fOutputFileName;
		std::string fRemoteHostName;
		int fRemotePort;
		
		std::string fGuiHostName;
		int fGuiPort;	
		std::string fGuiKey;
		int fMaxRetryAttempts;	
		int fScanId;
		std::string fContainerId;
		long int fTimeStamp;

		std::string fCommand;


};


#endif
