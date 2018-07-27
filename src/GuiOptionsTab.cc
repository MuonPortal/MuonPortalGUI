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
* @file GuiOptionsTab.cc
* @class GuiOptionsTab
* @brief GUI option tab menu
* 
* @author S. Riggi
* @date 25/04/2010
*/
#include <GuiOptionsTab.h>
#include <Gui.h>
#include <Logger.h>

#include <QtGui>
#include <TString.h>

#include <iostream>

using namespace std;

namespace MuonPortalNS {

GuiOptionsTab::GuiOptionsTab(QWidget *parent)
     : QWidget(parent)
{

	QLabel* label_InfoSeparation = new QLabel("Main options");
	label_InfoSeparation->setFrameStyle(QFrame::HLine | QFrame::Raised);
	label_InfoSeparation->setStyleSheet("QLabel { color : gray; }");

	checkBox_enableSounds= new QCheckBox("Enable sounds?");
	checkBox_enableSounds->setObjectName(QString::fromUtf8("checkBox_enableSounds"));
	checkBox_enableSounds->setChecked(false);

	//## Define object actions
	//...

	//## Set object layout
	QHBoxLayout* OptionFrameLayout= new QHBoxLayout;
	OptionFrameLayout->addWidget(checkBox_enableSounds);
	OptionFrameLayout->addStretch(1);
	
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(OptionFrameLayout);
	mainLayout->addStretch(1);
	setLayout(mainLayout);

}//close constructor


void GuiOptionsTab::SetOptions(){

	//## Pass gui options to parser
	Gui::fConfigParser->fEnableSounds= checkBox_enableSounds->isChecked();
	
}//close SetOptions()


}//close namespace
