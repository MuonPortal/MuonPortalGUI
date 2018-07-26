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
#include <TabMenu.h>
#include <Gui.h>
#include <Logger.h>
#include <Utilities.h>
#include <QtGui>

#include <iostream>

using namespace std;

namespace MuonPortalNS {

TabMenu::TabMenu(QWidget *parent)
{ 	
	//## Create tabs  
	fTabMenu = new QTabWidget(parent);
  fTabMenu->setObjectName(QString::fromUtf8("tab_Menu"));
  
	fContainerTab= new ContainerTab(fTabMenu);
	fContainerTab->setObjectName(QString::fromUtf8("tab_Container"));

	fPOCATab= new POCATab(fTabMenu);
	fPOCATab->setObjectName(QString::fromUtf8("tab_POCA"));

	fEMLLTab= new EMLLTab(fTabMenu);
	fEMLLTab->setObjectName(QString::fromUtf8("tab_EMLL"));

	fACFTab= new ACFTab(fTabMenu);
	fACFTab->setObjectName(QString::fromUtf8("tab_ACF"));

	fClusteringTab= new ClusteringTab(fTabMenu);
	fClusteringTab->setObjectName(QString::fromUtf8("tab_Clustering"));
	

	std::string initButton_imagepath= Gui::GUI_BASE_DIR + std::string("/share/initButton.png");
	pushButton_Init = new QPushButton(tr("Init"));
  pushButton_Init->setObjectName(QString::fromUtf8("pushButton_Init"));
	pushButton_Init->setIcon(QIcon(initButton_imagepath.c_str()));
	pushButton_Init->setFixedSize(QSize(60,30));

	std::string checkButton_imagepath= Gui::GUI_BASE_DIR + std::string("/share/checkButton.png");
	QImage image(checkButton_imagepath.c_str());
	image.setText(QString("Standard Config"),QString("Standard Config"));

	

	//## Add tabs to main menu
	
	fTabMenu->addTab(fContainerTab, tr("Geometry"));	
	fTabMenu->addTab(fPOCATab, tr("POCA"));
	fTabMenu->addTab(fACFTab, tr("ACF"));		
	fTabMenu->addTab(fClusteringTab, tr("Clustering"));		
	fTabMenu->addTab(fEMLLTab, tr("EMLL"));			
	

	//## Define object actions
	connect(pushButton_Init, SIGNAL(clicked()), this, SLOT(SetTabMenuOptions()) );	
	connect(pushButton_Init, SIGNAL(clicked()), Gui::fTomographyRecTabMenu, SLOT(EnableRunButton()) );
		
	

	//## Set object layout
	QHBoxLayout *buttonsLayout = new QHBoxLayout;
  buttonsLayout->addWidget(pushButton_Init);
	buttonsLayout->addSpacing(10);
	buttonsLayout->addStretch(1);
	
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(fTabMenu);
	mainLayout->addSpacerItem(new QSpacerItem(0, 500, QSizePolicy::Maximum, QSizePolicy::Maximum));
	mainLayout->addLayout(buttonsLayout);
  mainLayout->addStretch(1);
  setLayout(mainLayout);

}//close constructor


void TabMenu::SetTabMenuOptions(){

	//## Set all tab options
	INFO_LOG("Setting GUI options...");
	fContainerTab->SetContainerOptions();
	fPOCATab->SetPOCAOptions();
	fEMLLTab->SetEMLLOptions();
	fACFTab->SetACFOptions();
	fClusteringTab->SetClusteringOptions();

	
}//close TabMenu::SetTabMenuOptions()

}//close namespace

