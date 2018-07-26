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
* @file ContainerTab.h
* @class ContainerTab
* @brief GUI Container geometry tab menu
* 
* @author S. Riggi
* @date 25/04/2010
*/
#ifndef CONTAINER_TAB_H
#define CONTAINER_TAB_H

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

class ContainerTab : public QWidget {
     
	Q_OBJECT

 	public:
  	ContainerTab(QWidget *parent = 0);
 
	public slots:
		
		void BrowseInputFile();	

	public:

		void SetContainerOptions();


	private:
		
		QComboBox* comboBox_inputFile;
		QPushButton* pushButton_inputFile;	

		static const int MAXNPLANES= 4;
		QLabel* labelList_DetectorPlaneZ[MAXNPLANES];
		QDoubleSpinBox* spinboxList_DetectorPlaneZ[MAXNPLANES];

		QDoubleSpinBox* spinbox_containerSizeX;
		QDoubleSpinBox* spinbox_containerSizeY;
		QDoubleSpinBox* spinbox_containerSizeZ;

		QDoubleSpinBox* spinbox_containerPosX;
		QDoubleSpinBox* spinbox_containerPosY;
		QDoubleSpinBox* spinbox_containerPosZ;
		
		QDoubleSpinBox* spinbox_containerVoxelSizeX;
		QDoubleSpinBox* spinbox_containerVoxelSizeY;
		QDoubleSpinBox* spinbox_containerVoxelSizeZ;
		
	
		//Selection options
		QSpinBox* spinbox_Verbosity;
		QCheckBox* checkBox_maxNEvents;
		QSpinBox* spinbox_maxNEvents;

		QCheckBox* checkBox_smearTrackPoints;
		QDoubleSpinBox* spinbox_detResolutionX;
		QDoubleSpinBox* spinbox_detResolutionY;
		QDoubleSpinBox* spinbox_detResolutionZ;

		QCheckBox* checkBox_useScatteringAngleCut;
		QDoubleSpinBox* spinbox_minScatteringAngle;
		QDoubleSpinBox* spinbox_maxScatteringAngle;

		QCheckBox* checkBox_useDisplacementCut;
		QDoubleSpinBox* spinbox_minDisplacement;
		QDoubleSpinBox* spinbox_maxDisplacement;

		QCheckBox* checkBox_useEnergyCut;
		QDoubleSpinBox* spinbox_minEnergy;
		QDoubleSpinBox* spinbox_maxEnergy;

		QCheckBox* checkBox_smearTimeOfFlight;
		QDoubleSpinBox* spinbox_timeOfFlightResolution;

		QCheckBox* checkBox_useTrackSpeedCut;
		QDoubleSpinBox* spinbox_minTrackSpeed;
		QDoubleSpinBox* spinbox_maxTrackSpeed;

};//close class ContainerTab

}//close namespace

#endif
