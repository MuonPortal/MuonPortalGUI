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
* @file Gui.h
* @class Gui
* @brief Main widget
* 
* @author S. Riggi
* @date 25/04/2010
*/
#ifndef GUI_H
#define GUI_H

#include <ConfigParser.h>

#include <AboutTabMenu.h>
#include <TabMenu.h>
#include <TomographyRecTabMenu.h>

#include <QDialog>
#include <QObject>


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

class Gui : public QWidget {
     
	Q_OBJECT

 	public:
  	Gui(QWidget *parent = 0);
		
	public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
		
	signals:
			void failure();

	public:

		static ConfigParser* fConfigParser; 
		static std::string GUI_BASE_DIR;		
		static std::string GUI_CONFIG_DIR;		
		static std::string GUI_DATA_DIR;
		static std::string GUI_SCRIPT_DIR;	
		static std::string SHELL_DIR;
		static std::string GUI_HOSTNAME;
		static int GUI_PORT;	
		static std::string GUI_KEY;
		static std::string SEI_HOSTNAME;
		static std::string SEI_USERNAME;
		static std::string SOCK_END_PATTERN;
		static long int SOCK_LISTEN_PORT;
		

 	private:

		bool SetGuiEnvVariables();
		void createIcons();

	private:

		QListWidget *contentsWidget;
    QStackedWidget *pagesWidget;

	public:
	
		AboutTabMenu* fAboutTabMenu;
		static TabMenu* fTabMenu;
		static TomographyRecTabMenu* fTomographyRecTabMenu;
	
};//close Gui class

}//close namespace

#endif


