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
* @file POCATab.h
* @class POCATab
* @brief GUI POCA algorithm tab menu
* 
* @author S. Riggi
* @date 25/04/2010
*/
#ifndef POCA_TAB_H
#define POCA_TAB_H

#include <QDialog>
#include <QScienceSpinBox.h>
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

namespace MuonPortalNS {

class POCATab : public QWidget {
     
	Q_OBJECT

 	public:
  	POCATab(QWidget *parent = 0);
 
	public slots:
		
		void BrowseInputFile();	
		void BrowseOutputFile();	
		
	public:
		void SetPOCAOptions();
		
	private:

		bool DumpPOCAConfigFile();
		std::string fPOCAConfigFileName;		
		
		QGroupBox* groupBox_POCA;

		QComboBox* comboBox_inputFile;
		QPushButton* pushButton_inputFile;	
		QComboBox* comboBox_outputFile;
		QPushButton* pushButton_outputFile;	

		QCheckBox* checkBox_useQueueRun;
	
		QScienceSpinBox* spinbox_POCATolerance;

		QCheckBox* checkBox_maxNEvents;
		QSpinBox* spinbox_maxNEvents;

		QCheckBox* checkBox_useScatteringAngleCut;
		QDoubleSpinBox* spinbox_minScatteringAngle;
		QDoubleSpinBox* spinbox_maxScatteringAngle;

		QDoubleSpinBox* spinbox_alarmThreshold;
		QSpinBox* spinbox_NEventThreshold;
		QComboBox* comboBox_drawStyle;

		QCheckBox* checkBox_useThetaWeight;
		QDoubleSpinBox* spinbox_thetaWeight;
		QCheckBox* checkBox_useAverageSignal;
		
		QCheckBox* checkBox_useEnergyCut;
		QDoubleSpinBox* spinbox_minEnergy;
		QDoubleSpinBox* spinbox_maxEnergy;
		
};//close class

}//close namespace

#endif
