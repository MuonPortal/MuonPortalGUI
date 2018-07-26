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
* @file ACFTab.h
* @class ACFTab
* @brief GUI Autocorrelation algorithm tab menu
* 
* @author S. Riggi
* @date 25/04/2010
*/
#ifndef ACF_TAB_H
#define ACF_TAB_H

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

namespace MuonPortalNS {

class ACFTab : public QWidget {
     
	Q_OBJECT

 	public:
  	ACFTab(QWidget *parent = 0);
 
	public slots:
		
		void BrowseInputFile();	
		void BrowseOutputFile();	
		void BrowseInputRandomFile();

	public:

		void SetACFOptions();
		bool DumpACFConfigFile();
		
	private:

		std::string fACFConfigFileName;
		
		QGroupBox* groupBox_ACF;

		QComboBox* comboBox_inputFile;
		QPushButton* pushButton_inputFile;
		QComboBox* comboBox_inputRandomFile;
		QPushButton* pushButton_inputRandomFile;		
		QComboBox* comboBox_outputFile;
		QPushButton* pushButton_outputFile;	
	
		QCheckBox* checkBox_waitPOCA;
		QCheckBox* checkBox_useQueueRun;
	
		
		QSpinBox* spinbox_Verbosity;
		QDoubleSpinBox* spinbox_minRScale;
		QDoubleSpinBox* spinbox_maxRScale;
		QDoubleSpinBox* spinbox_VoxelSizeX;
		QDoubleSpinBox* spinbox_VoxelSizeY;
		QDoubleSpinBox* spinbox_VoxelSizeZ;
		QSpinBox* spinbox_NCrownX;
		QSpinBox* spinbox_NCrownY;
		QSpinBox* spinbox_NCrownZ;

		QCheckBox* checkBox_maxNEvents;
		QSpinBox* spinbox_maxNEvents;

		QCheckBox* checkBox_useScatteringAngleCut;
		QDoubleSpinBox* spinbox_minScatteringAngle;
		QDoubleSpinBox* spinbox_maxScatteringAngle;

		QDoubleSpinBox* spinbox_alarmThreshold;
		QCheckBox* checkBox_useThetaWeight;
		QDoubleSpinBox* spinbox_thetaWeight;

		QCheckBox* checkBox_useEnergyCut;
		QDoubleSpinBox* spinbox_minEnergy;
		QDoubleSpinBox* spinbox_maxEnergy;

};//close class ACFTab

}//close namespace

#endif
