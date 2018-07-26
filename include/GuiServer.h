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
* @file GuiServer.h
* @class GuiServer
* @brief Main widget + server
* 
* @author S. Riggi
* @date 25/04/2010
*/
#ifndef GUI_SERVER_H
#define GUI_SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>

#include <Gui.h>

class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
class QCheckBox;
class QGroupBox;
class QPushButton;
class QTextEdit;
class QListWidget;
class QListWidgetItem;
class QStackedWidget;

namespace MuonPortalNS {

class GuiServer : public QObject {
    
	Q_OBJECT

	public:
    explicit GuiServer(QObject* parent = 0, long int portNo= 9999);
    
	public:
		void SetPort(long int portNumber) {port = portNumber;}
		void SetAllowedHosts(std::string host) {hostname = host;}

	signals:
    
	public slots:
  	void newConnection();
		void processIncomingData();

	private:
    QTcpServer* server;
		QTcpSocket* socket;
		long int port;
		std::string hostname;
		Gui* gui;

};//close class GuiServer

}//close namespace

#endif // GUI_SERVER_H


