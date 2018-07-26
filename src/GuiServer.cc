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
* @file GuiServer.cc
* @class GuiServer
* @brief Main widget + server
* 
* @author S. Riggi
* @date 25/04/2010
*/

#include <GuiServer.h>
#include <Gui.h>
#include <Logger.h>
#include <QtGui>

namespace MuonPortalNS {

GuiServer::GuiServer(QObject *parent, long int portNo)
	: QObject(parent), port(portNo)   
{
    
	//## Create the TCP server
	DEBUG_LOG("Creating a tcp server listening on port "<<portNo<<" ...");
	server = new QTcpServer(this);

	//QHostAddress host("192.168.3.89");
	
	// whenever a user connects, it will emit signal
  connect(server, SIGNAL(newConnection()),this, SLOT(newConnection()));

  if(!server->listen(QHostAddress::Any, portNo)) {
		std::string errMsg= "Could not start TCP server!";
		ERROR_LOG(errMsg);
  	qDebug() << "Server could not start";
		QMessageBox::critical(0, tr("Gui Server"), tr("Unable to start the server: %1.").arg(server->errorString())  );
  	exit(1);
  }
  else {
		INFO_LOG("Started tcp server listening on port "<<portNo<<" ...");
  	qDebug() << "Server started!";
  }

	//## Create and show the GUI
	gui= new Gui;
	if(gui) {
		gui->show();
	}
	else{
		std::string errMsg= "Could not start GUI!";
		ERROR_LOG(errMsg);
		qDebug() << "Cannot start the gui";
		QMessageBox::critical(0, tr("Gui Server"), tr("Unable to start the gui"));
  	exit(1);
	}

}//close constructor


void GuiServer::newConnection(){

	//## Need to grab the socket
  socket = server->nextPendingConnection();
	connect(socket, SIGNAL(readyRead()),this, SLOT(processIncomingData()));

}//close GuiServer::newConnection()


void GuiServer::processIncomingData(){

	//## Read incoming data from client	
	char buffer[1024] = {0};
	socket->read(buffer, socket->bytesAvailable());
	
	std::string message= buffer;
	DEBUG_LOG("Server received a message from a client: "<<message);
	
	//## Reply to client	
	socket->write("Hi client, I got your message!\r\n");
  socket->flush();
  socket->waitForBytesWritten(3000);
	
	//## Close the socket
  socket->close();

}//close GuiServer::processIncomingData()

}//close namespace


