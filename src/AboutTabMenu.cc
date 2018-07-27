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
* @file AboutTabMenu.cc
* @class AboutTabMenu
* @brief About info tab menu
* 
* @author S. Riggi
* @date 25/04/2010
*/
#include <AboutTabMenu.h>
#include <Logger.h>
#include <Utilities.h>

#include <Gui.h>
#include <QtGui>

#include <iostream>

using namespace std;

namespace MuonPortalNS {

AboutTabMenu::AboutTabMenu(QWidget *parent){
 	
	//## Set bkg image
	std::string imagepath= Gui::GUI_BASE_DIR + std::string("/share/Logo_MuonPortal-scaled.png");
	std::string imagepath_pic= Gui::GUI_BASE_DIR + std::string("/share/MuonPortalPic.png");
	
	QImage image(imagepath.c_str());
	
	QLabel* imageLabel = new QLabel();
	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  imageLabel->setWindowFlags(Qt::FramelessWindowHint);
  imageLabel->setTextFormat(Qt::RichText);
	
	QImage image_pic(imagepath_pic.c_str());
	
	QLabel* imageLabel_pic = new QLabel();
	imageLabel_pic->setPixmap(QPixmap::fromImage(image_pic));
	imageLabel_pic->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  imageLabel_pic->setWindowFlags(Qt::FramelessWindowHint);
  imageLabel_pic->setTextFormat(Qt::RichText);
		

	QGroupBox* groupBox_textLabel= new QGroupBox("");
	groupBox_textLabel->setStyleSheet("border:0;");

	QLabel* textLabel= new QLabel;
	textLabel->setText(
		"<center>"
		"<font size=\"6\"> <b> MuonPortalGUI </b> </font> <BR>"
	  "<font size=\"5\"> Copyright &copy;2013. All rights reserved. </font> <BR>"
		
		"</center>"
		"<font size=\"4\"> <i>Author</i>: S. Riggi </font> <BR>"
		"<font size=\"4\"> <i>Address</i>: INAF Catania - Italy </font> <BR>"
		"<font size=\"4\"> <i> Bug report to</i>: <a href=\"simone.riggi@gmail.com\">simone.riggi@gmail.com</a> </font> <BR>"
		"<font size=\"4\"> <i> License & Credits</i>: MuonPortalGUI is licensed under the GNU General Public License (GPL). </font> <BR>" 
		"<font size=\"4\"> It CANNOT be distributed to non-MuonPortal members. </font> <BR>"
		"<font size=\"4\"> Credits to the authors are required for publications using the tool. </font> <BR>"

		"<BR>"
		"<font size=\"4\"> MuonPortalGUI is a C++ tool for tomography imaging developed for the Muon Portal project. </font> <BR> "
		"<font size=\"4\"> The tool uses GEANT4 & ROOT tools for detector modelling, particle tracking and data analysis. </font> <BR>" 
		"<font size=\"4\"> The GUI is developed in QT4 and the 3D viewer based on VTK lib.</font> <BR>"
		"<font size=\"4\"> </font> <BR>"
	);	
	
	textLabel->setObjectName("textLabel");


	QFont* textLabelFont = new QFont("Courier New");
  textLabelFont->setItalic(true);
  textLabelFont->setPixelSize(20);	
  
	QVBoxLayout* TextLabelLayout= new QVBoxLayout;
	TextLabelLayout->addWidget(textLabel);
	groupBox_textLabel->setLayout(TextLabelLayout);

	//## Set object layout
	QHBoxLayout* imageLayout= new QHBoxLayout;
	imageLayout->addStretch(1);
	//imageLayout->addWidget(imageLabel_pic);	
	imageLayout->addWidget(imageLabel);	
	
	QVBoxLayout* mainLayout = new QVBoxLayout;	
	mainLayout->addLayout(imageLayout);	
	mainLayout->addWidget(imageLabel_pic);	
	//mainLayout->addWidget(imageLabel);		
	mainLayout->addWidget(groupBox_textLabel);
	mainLayout->addStretch(1);
	setLayout(mainLayout);
	
	
}//close constructor

}//close namespace

