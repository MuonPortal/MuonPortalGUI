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
* @file TabMenu.h
* @class TabMenu
* @brief GUI tab menu
* 
* @author S. Riggi
* @date 25/04/2010
*/
#ifndef TAB_MENU_H
#define TAB_MENU_H

#include <QDialog>
#include <QObject>

#include <ContainerTab.h>
#include <POCATab.h>
#include <EMLLTab.h>
#include <ACFTab.h>
#include <ClusteringTab.h>

class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
class QCheckBox;
class QGroupBox;
class QPushButton;
class QTextEdit;
class QComboBox;

namespace MuonPortalNS {

class TabMenu : public QWidget {
     
	Q_OBJECT

 	public:

  	TabMenu(QWidget *parent = 0);

	public slots:
		void SetTabMenuOptions();

	public:

		
	private:

		QTabWidget* fTabMenu;
		
		ContainerTab* fContainerTab;
		POCATab* fPOCATab;
		EMLLTab* fEMLLTab;
		ACFTab* fACFTab;
		ClusteringTab* fClusteringTab;

		QPushButton* pushButton_Init;	
		
};//close class TabMenu

}//close namespace

#endif
