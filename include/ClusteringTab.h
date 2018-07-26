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
* @file ClusteringTab.h
* @class ClusteringTab
* @brief Clustering algorithm GUI tab
* 
* @author S. Riggi
* @date 25/04/2010
*/
#ifndef CLUSTERING_TAB_H
#define CLUSTERING_TAB_H

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
class QObject;

namespace MuonPortalNS {

class ClusteringTab : public QWidget {
     
	Q_OBJECT

 	public:
  	ClusteringTab(QWidget* parent = 0);
 
	public slots:
		void BrowseInputFile();
		void BrowseOutputFile();
		
	public:

		void SetClusteringOptions();
		
	private:

		bool DumpClusteringConfigFile();
		std::string fClusteringConfigFileName;		
		
		QGroupBox* groupBox_FOFClustering;

		QComboBox* comboBox_inputFile;
		QPushButton* pushButton_inputFile;	
		QComboBox* comboBox_outputFile;
		QPushButton* pushButton_outputFile;	

		QCheckBox* checkBox_waitPOCA;
		QSpinBox* spinbox_Verbosity;
		QCheckBox* checkBox_useQueueRun;
	

		QDoubleSpinBox* spinbox_epsilon;
		QDoubleSpinBox* spinbox_minNpts;

		QCheckBox* checkBox_useScatteringAngleCut;
		QDoubleSpinBox* spinbox_minScatteringAngle;
		QDoubleSpinBox* spinbox_maxScatteringAngle;
	
		QCheckBox* checkBox_maxNEvents;
		QSpinBox* spinbox_maxNEvents;

		QSpinBox* spinbox_NEventThreshold;
		QDoubleSpinBox* spinbox_alarmThreshold;
		QCheckBox* checkBox_useThetaWeight;
		QDoubleSpinBox* spinbox_thetaWeight;
		QCheckBox* checkBox_useAverageSignal;

		QCheckBox* checkBox_useEnergyCut;
		QDoubleSpinBox* spinbox_minEnergy;
		QDoubleSpinBox* spinbox_maxEnergy;

};//close class ClusteringTab

}//close namespace

#endif
