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
#include <DownloaderThreadObj.h>
#include <Logger.h>
#include <Gui.h>
#include <Utilities.h>
#include <QtGui>
#include <DownloaderThreadObj.h>

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

#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <fcntl.h>

using namespace std;

namespace MuonPortalNS {


DownloaderThreadObj::DownloaderThreadObj() {

	fIsLocalDownload= false;
	fMaxNAttempts= 3;
	fFileName= "dummy.root";
	fDestinationPath= ".";
	fRemotePortNumber= "22";
	fRemoteHostIp= "127.0.0.1";
	fRemoteUserName= "pippo";

	fInputFileName= "dummy.root";
	fOutputFileName= "dummy.root";
	fCommand= "";

	//## Connect status signal to socket sender
	connect(this,SIGNAL(logSig(QString)), this, SLOT(sender(QString)) );

}//close constructor
     
DownloaderThreadObj::~DownloaderThreadObj() {

}//close destructor


bool DownloaderThreadObj::init(){

	//## Set the log message parameters
	//## Update only status, logmessage and progress status while running
	//## Emit start process signal
	fLogMessage.taskName= "DOWNLOADER";
	fLogMessage.command= "LOG";	
	fLogMessage.inputFileName= fInputFileName;		
	fLogMessage.outputFileName= fOutputFileName;	
	fLogMessage.jobDirName= fDestinationPath;
	fLogMessage.scanId= fScanId;
	fLogMessage.containerId= fContainerId;
	fLogMessage.timeStamp= fTimeStamp;			
	fLogMessage.status= eRunning;
	fLogMessage.logMessage= "Data download started...";
	fLogMessage.progress= 0;
	fLogMessage.guiKey= fGuiKey;		
	fLogMessage.guiHostName= fGuiHostName;
	fLogMessage.guiPort= fGuiPort;
	

	//## Check parameters and emit failure message in case of errors
	//...
	//...
	

	//## Create command name to be executed
	fCommand= "";

	std::string outputFileName= fDestinationPath + std::string("/") + fOutputFileName;

	if(fIsLocalDownload){
		fCommand= std::string("/usr/bin/rsync -av --ignore-times --progress ") + fInputFileName + std::string(" ") + outputFileName + std::string(" | unbuffer -p grep -o \"[0-9]*%\"");
	}
	else{
		fCommand= std::string("/usr/bin/rsync -av --ignore-times --progress ") + fRemoteUserName + std::string("@") + fRemoteHostName + std::string(":") + fInputFileName + std::string(" ") + outputFileName + std::string(" | unbuffer -p grep -o \"[0-9]*%\"");
	}

	cout<<"DownloaderThreadObj::init(): INFO: command= "<<fCommand<<endl;	

	return true;
	
}//close DownloaderThreadObj:init()



void DownloaderThreadObj::process(){


	//## Init process
	bool init_status= init();
	if(!init_status){
		fLogMessage.status= eFailure;
		fLogMessage.logMessage= "--> Download init failed!";
		fLogMessage.progress= 0;

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);

		return;
	}


	//## Transfer data file from remote host
	cout<<"DownloaderThreadObj::process(): INFO: Starting to transfer data file "<<fInputFileName.c_str()<<endl;
	
	
	int attemptCounter= 0;
	int status= 0;
	int progressPercentage= 0;

	//## Start loop on max attempts allowed
	while(attemptCounter<=fMaxRetryAttempts){
		
		//## Open a bidirectional pipe for reading
		FILE *fp;
  	char path[1035];

  	//fp = popen("/usr/bin/rsync -av --progress sriggi@astrct.oact.inaf.it:simdata_Muon*.out . | unbuffer -p grep -o \"[0-9]*%\"", "r");
		fp = popen(fCommand.c_str(),"r");
  	if (fp == NULL) {
			status= 1;
    	cerr<<"DownloaderThreadObj::process(): ERROR: Download failed ["<<attemptCounter<<" attempt]...retry!"<<endl;
			QString msg = QString("--> Download failed [%1 attempt] ...").arg(attemptCounter);
			emit(statusMessage(msg));

			fLogMessage.status= eWarning;
			fLogMessage.logMessage= msg.toStdString();
			fLogMessage.progress= progressPercentage;

			QString logMessage= makeLogMessage(fLogMessage);
			emit logSig(logMessage);

			pclose(fp);
			attemptCounter++;
    	continue;			
  	}

  	//## Here we simply loop throught file descriptor (console && error) until finish
		progressPercentage= 0;
		bool isFileExisting= true;

 	 	while (fgets(path, sizeof(path)-1, fp) != NULL) {
			progressPercentage= atoi(path);
			isFileExisting= false;

    	cout<<"DownloaderThreadObj::process(): INFO: progressPercentage= "<<progressPercentage<<endl;
			if(progressPercentage>=0 && progressPercentage<=100) {
				emit(downloadProgress(progressPercentage));
				QString msg = QString("--> Download in progress [%1] ...").arg(progressPercentage);
				emit(statusMessage(msg));
		
				fLogMessage.status= eRunning;
				fLogMessage.logMessage= msg.toStdString();
				fLogMessage.progress= progressPercentage;

				QString logMessage= makeLogMessage(fLogMessage);
				emit logSig(logMessage);
			}
			else break;
 		}

  	//## Be sure to close before returning, otherwise something can crash...
  	pclose(fp);

		attemptCounter++;

		//## Check status
		if(progressPercentage==100){
			cout<<"DownloaderThreadObj::process(): INFO: Download completed!"<<endl;	
			status= 0;	
			break;
		}
		else{
			cerr<<"DownloaderThreadObj::process(): INFO: Cannot get full percentage download...retry!"<<endl;
			progressPercentage= 0;
			QString msg = QString("Download troubles (cannot get full percentage)...retry!");
			emit(statusMessage(msg));
			status= 1;

			fLogMessage.status= eWarning;
			fLogMessage.logMessage= msg.toStdString();
			fLogMessage.progress= progressPercentage;

			QString logMessage= makeLogMessage(fLogMessage);
			emit logSig(logMessage);
		}
		
	}//end loop while



	if(status==0){
		cout<<"DownloaderThreadObj::process(): INFO: Data transfer succesful!"<<endl;
		emit finished();  
		QString msg = QString("--> Download terminated with success!!");
		emit(statusMessage(msg));

		fLogMessage.status= eFinished;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= progressPercentage;

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
		emit finishedMessage(logMessage);
	}
	else{
		cerr<<"DownloaderThreadObj::process(): ERROR: Cannot download data file from remote host (check connection/file paths?)...exit!"<<endl;
		QString msg = QString("Download failed ... check network connection/file paths!");
		emit(statusMessage(msg));
		emit(error());

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= progressPercentage;

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}

}//close DownloaderThreadObj::process()


QString DownloaderThreadObj::makeLogMessage(LOG_MESSAGE msg){

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
	data.append(Gui::SOCK_END_PATTERN.c_str());	

	return QString(data);

}//close DownloaderThreadObj::makeLogMessage()


bool DownloaderThreadObj::sender(QString message)
{	
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
		DEBUG_LOG("Send this message via socket: "<<qPrintable(message)<<" ...");
  	socket->write(data);
		DEBUG_LOG("Written to socket...");
		socket->flush();
		socket->waitForBytesWritten(3000);
  }
	
	socket->close();
	
	return status;

}//close DownloaderThreadObj::sender()

}//close namespace
