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
* @file GuiOptionsTab.h
* @class GuiOptionsTab
* @brief GUI option tab menu
* 
* @author S. Riggi
* @date 25/04/2010
*/
#ifndef GUI_OPTIONS_TAB_H
#define GUI_OPTIONS_TAB_H

#include <QDialog>
#include <QObject>


class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
class QCheckBox;
class QGroupBox;
class QPushButton;
class QTextEdit;
class QComboBox;
class QSpinBox;
class QDoubleSpinBox;
class QRadioButton;
class QLabel;

namespace MuonPortalNS {

class GuiOptionsTab : public QWidget {
     
	Q_OBJECT

 	public:
  	GuiOptionsTab(QWidget *parent = 0);
 
	//public slots:
		
	public:
		void SetOptions();

	private:
		
		QCheckBox* checkBox_enableSounds;

};//close class GuiOptionsTab

}//close namespace

#endif
