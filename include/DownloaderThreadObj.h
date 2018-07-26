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
* @file DownloaderThreadObj.h
* @class DownloaderThreadObj
* @brief Download data processing thread
* 
* @author S. Riggi
* @date 25/04/2010
*/
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

namespace MuonPortalNS {

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

};//close class

}//close namespace

#endif
