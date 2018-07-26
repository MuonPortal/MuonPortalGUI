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
* @file ContainerTab.cc
* @class ContainerTab
* @brief GUI Container geometry tab menu
* 
* @author S. Riggi
* @date 25/04/2010
*/
#include <ContainerTab.h>
#include <Gui.h>
#include <Logger.h>

#include <QtGui>
#include <TString.h>

#include <iostream>

using namespace std;

namespace MuonPortalNS {

ContainerTab::ContainerTab(QWidget *parent)
     : QWidget(parent)
{

	QLabel* label_InfoSeparation = new QLabel("Main options");
	label_InfoSeparation->setFrameStyle(QFrame::HLine | QFrame::Raised);
	label_InfoSeparation->setStyleSheet("QLabel { color : gray; }");

	QLabel* label_inputFile= new QLabel();
	label_inputFile->setObjectName(QString::fromUtf8("label_inputFile"));
	label_inputFile->setText("Input File");

	comboBox_inputFile= new QComboBox;
	comboBox_inputFile->setObjectName(QString::fromUtf8("comboBox_inputFile"));
	comboBox_inputFile->setEditable(true);
	comboBox_inputFile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	comboBox_inputFile->addItem("data/simdata_MuonWritingAtCenterScenario.out");
	comboBox_inputFile->addItem("data/simdata_UnshieldThreatAtCenterDenseScenario.out");
	comboBox_inputFile->addItem("data/simdata_EmptyContainerScenario_last.out");
	comboBox_inputFile->setFixedWidth(450);
	
	pushButton_inputFile = new QPushButton(tr("Browse"));
  pushButton_inputFile->setObjectName(QString::fromUtf8("pushButton_inputFile"));
	pushButton_inputFile->setFixedSize(QSize(80,20));

	//## Define geometry options
	QLabel* label_InfoOptionSeparation = new QLabel("Detector Options");
	label_InfoOptionSeparation->setFrameStyle(QFrame::HLine | QFrame::Raised);
	label_InfoOptionSeparation->setStyleSheet("QLabel { color : red; }");

	//double defaultDetPlaneZ[]= {10,110,390,490};
	//double defaultDetPlaneZ[]= {-240,-140,140,240};//SIMULATION
	double defaultDetPlaneZ[]= {-270,-135,135,270};//REAL

	for(int i=0;i<MAXNPLANES;i++){	
		QString objName = QString("labelList_DetectorPlaneZ%1").arg(i+1);
		QString labelText = QString("Detector Plane %1").arg(i+1);
		labelList_DetectorPlaneZ[i]= new QLabel();
		labelList_DetectorPlaneZ[i]->setObjectName(objName);
		labelList_DetectorPlaneZ[i]->setText(labelText);

		objName = QString("spinboxList_DetectorPlaneZ%1").arg(i+1);
		spinboxList_DetectorPlaneZ[i]= new QDoubleSpinBox;
		spinboxList_DetectorPlaneZ[i]->setObjectName(objName);
		spinboxList_DetectorPlaneZ[i]->setRange(-1000,1000);
  	spinboxList_DetectorPlaneZ[i]->setValue(defaultDetPlaneZ[i]);
		spinboxList_DetectorPlaneZ[i]->setSingleStep(1);
		spinboxList_DetectorPlaneZ[i]->setFixedWidth(100);

	}//end loop planes


	QLabel* label_containerSize= new QLabel();
	label_containerSize->setObjectName(QString::fromUtf8("label_containerSize"));
	label_containerSize->setText("Container XYZ Size [cm]");

	spinbox_containerSizeX= new QDoubleSpinBox;
	spinbox_containerSizeX->setObjectName(QString::fromUtf8("spinbox_containerSizeX"));
	spinbox_containerSizeX->setRange(0., 10000.);
  spinbox_containerSizeX->setValue(590);
	spinbox_containerSizeX->setSingleStep(1);
	spinbox_containerSizeX->setDecimals(1);
	spinbox_containerSizeX->setFixedWidth(80);

	spinbox_containerSizeY= new QDoubleSpinBox;
	spinbox_containerSizeY->setObjectName(QString::fromUtf8("spinbox_containerSizeY"));
	spinbox_containerSizeY->setRange(0., 10000.);
  spinbox_containerSizeY->setValue(240);
	spinbox_containerSizeY->setSingleStep(1);
	spinbox_containerSizeY->setDecimals(1);
	spinbox_containerSizeY->setFixedWidth(80);	

	spinbox_containerSizeZ= new QDoubleSpinBox;
	spinbox_containerSizeZ->setObjectName(QString::fromUtf8("spinbox_containerSizeZ"));
	spinbox_containerSizeZ->setRange(0., 10000.);
  spinbox_containerSizeZ->setValue(240);
	spinbox_containerSizeZ->setSingleStep(1);
	spinbox_containerSizeZ->setDecimals(1);
	spinbox_containerSizeZ->setFixedWidth(80);


	QLabel* label_containerPos= new QLabel();
	label_containerPos->setObjectName(QString::fromUtf8("label_containerPos"));
	label_containerPos->setText("Container XYZ Pos [cm]");

	spinbox_containerPosX= new QDoubleSpinBox;
	spinbox_containerPosX->setObjectName(QString::fromUtf8("spinbox_containerPosX"));
	spinbox_containerPosX->setRange(-10000., 10000.);
  spinbox_containerPosX->setValue(0);
	spinbox_containerPosX->setSingleStep(1);
	spinbox_containerPosX->setDecimals(1);
	spinbox_containerPosX->setFixedWidth(80);

	spinbox_containerPosY= new QDoubleSpinBox;
	spinbox_containerPosY->setObjectName(QString::fromUtf8("spinbox_containerPosY"));
	spinbox_containerPosY->setRange(-10000., 10000.);
  spinbox_containerPosY->setValue(0);
	spinbox_containerPosY->setSingleStep(1);
	spinbox_containerPosY->setDecimals(1);
	spinbox_containerPosY->setFixedWidth(80);	

	spinbox_containerPosZ= new QDoubleSpinBox;
	spinbox_containerPosZ->setObjectName(QString::fromUtf8("spinbox_containerPosZ"));
	spinbox_containerPosZ->setRange(-10000., 10000.);
  spinbox_containerPosZ->setValue(0);
	spinbox_containerPosZ->setSingleStep(1);
	spinbox_containerPosZ->setDecimals(1);
	spinbox_containerPosZ->setFixedWidth(80);

	

	QLabel* label_containerVoxelSize= new QLabel();
	label_containerVoxelSize->setObjectName(QString::fromUtf8("label_containerVoxelSize"));
	label_containerVoxelSize->setText("Container Voxel XYZ Size [cm]");

	spinbox_containerVoxelSizeX= new QDoubleSpinBox;
	spinbox_containerVoxelSizeX->setObjectName(QString::fromUtf8("spinbox_containerVoxelSizeX"));
	spinbox_containerVoxelSizeX->setRange(0., 100.);
  spinbox_containerVoxelSizeX->setValue(10.0);
	spinbox_containerVoxelSizeX->setSingleStep(1);	
	spinbox_containerVoxelSizeX->setDecimals(0);
	spinbox_containerVoxelSizeX->setFixedWidth(80);

	spinbox_containerVoxelSizeY= new QDoubleSpinBox;
	spinbox_containerVoxelSizeY->setObjectName(QString::fromUtf8("spinbox_containerVoxelSizeY"));
	spinbox_containerVoxelSizeY->setRange(0., 100.);
  spinbox_containerVoxelSizeY->setValue(10.);
	spinbox_containerVoxelSizeY->setSingleStep(1);
	spinbox_containerVoxelSizeY->setDecimals(0);
	spinbox_containerVoxelSizeY->setFixedWidth(80);	

	spinbox_containerVoxelSizeZ= new QDoubleSpinBox;
	spinbox_containerVoxelSizeZ->setObjectName(QString::fromUtf8("spinbox_containerVoxelSizeZ"));
	spinbox_containerVoxelSizeZ->setRange(0., 100.);
  spinbox_containerVoxelSizeZ->setValue(10.);
	spinbox_containerVoxelSizeZ->setSingleStep(1);
	spinbox_containerVoxelSizeZ->setDecimals(0);
	spinbox_containerVoxelSizeZ->setFixedWidth(80);

	//## Selection options
	QLabel* label_OptionSelectionSeparation = new QLabel("Selection");
	label_OptionSelectionSeparation->setFrameStyle(QFrame::HLine | QFrame::Raised);
	label_OptionSelectionSeparation->setStyleSheet("QLabel { color : green; }");

	QLabel* label_Verbosity= new QLabel();
	label_Verbosity->setObjectName(QString::fromUtf8("label_Verbosity"));
	label_Verbosity->setText("Verbosity");

	spinbox_Verbosity= new QSpinBox;
	spinbox_Verbosity->setObjectName(QString::fromUtf8("spinbox_Verbosity"));
	spinbox_Verbosity->setRange(0, 3);
  spinbox_Verbosity->setValue(1);
	spinbox_Verbosity->setSingleStep(1);
	spinbox_Verbosity->setFixedWidth(80);

	checkBox_smearTrackPoints= new QCheckBox("Smear track points (reso X-Y-Z [cm])?");
	checkBox_smearTrackPoints->setObjectName(QString::fromUtf8("checkBox_smearTrackPoints"));
	checkBox_smearTrackPoints->setChecked(true);

	spinbox_detResolutionX= new QDoubleSpinBox;
	spinbox_detResolutionX->setObjectName(QString::fromUtf8("spinbox_detResolutionX"));
	spinbox_detResolutionX->setRange(0, 100);
  spinbox_detResolutionX->setValue(1);
	spinbox_detResolutionX->setSingleStep(0.1);
	spinbox_detResolutionX->setFixedWidth(80);

	spinbox_detResolutionY= new QDoubleSpinBox;
	spinbox_detResolutionY->setObjectName(QString::fromUtf8("spinbox_detResolutionY"));
	spinbox_detResolutionY->setRange(0, 100);
  spinbox_detResolutionY->setValue(1);
	spinbox_detResolutionY->setSingleStep(0.1);
	spinbox_detResolutionY->setFixedWidth(80);

	spinbox_detResolutionZ= new QDoubleSpinBox;
	spinbox_detResolutionZ->setObjectName(QString::fromUtf8("spinbox_detResolutionZ"));
	spinbox_detResolutionZ->setRange(0, 100);
  spinbox_detResolutionZ->setValue(1);
	spinbox_detResolutionZ->setSingleStep(0.1);
	spinbox_detResolutionZ->setFixedWidth(80);


	checkBox_smearTimeOfFlight= new QCheckBox("Smear TOF?");
	checkBox_smearTimeOfFlight->setObjectName(QString::fromUtf8("checkBox_smearTimeOfFlight"));
	checkBox_smearTimeOfFlight->setChecked(false);
		
	spinbox_timeOfFlightResolution= new QDoubleSpinBox;
	spinbox_timeOfFlightResolution->setObjectName(QString::fromUtf8("spinbox_timeOfFlightResolution"));
	spinbox_timeOfFlightResolution->setRange(0, 100);
  spinbox_timeOfFlightResolution->setValue(5);
	spinbox_timeOfFlightResolution->setSingleStep(0.1);
	spinbox_timeOfFlightResolution->setFixedWidth(80);

	checkBox_maxNEvents= new QCheckBox("Use max NEvents?");
	checkBox_maxNEvents->setObjectName(QString::fromUtf8("checkBox_maxNEvents"));
	checkBox_maxNEvents->setChecked(true);

	spinbox_maxNEvents= new QSpinBox;
	spinbox_maxNEvents->setObjectName(QString::fromUtf8("spinbox_maxNEvents"));
	spinbox_maxNEvents->setRange(0, 1.e+8);
  spinbox_maxNEvents->setValue(500000);
	spinbox_maxNEvents->setSingleStep(1);
	spinbox_maxNEvents->setFixedWidth(120);

	checkBox_useScatteringAngleCut= new QCheckBox("Theta Cut [deg]");
	checkBox_useScatteringAngleCut->setObjectName(QString::fromUtf8("checkBox_useScatteringAngleCut"));
	checkBox_useScatteringAngleCut->setChecked(true);
		
	spinbox_minScatteringAngle= new QDoubleSpinBox;
	spinbox_minScatteringAngle->setObjectName(QString::fromUtf8("spinbox_minScatteringAngle"));
	spinbox_minScatteringAngle->setRange(0, 10000);
  spinbox_minScatteringAngle->setValue(0);
	spinbox_minScatteringAngle->setSingleStep(0.5);
	spinbox_minScatteringAngle->setFixedWidth(80);

	spinbox_maxScatteringAngle= new QDoubleSpinBox;
	spinbox_maxScatteringAngle->setObjectName(QString::fromUtf8("spinbox_maxScatteringAngle"));
	spinbox_maxScatteringAngle->setRange(0, 10000);
  spinbox_maxScatteringAngle->setValue(30);
	spinbox_maxScatteringAngle->setSingleStep(0.5);
	spinbox_maxScatteringAngle->setFixedWidth(80);

		

	checkBox_useDisplacementCut= new QCheckBox("DeltaX Cut [cm]");
	checkBox_useDisplacementCut->setObjectName(QString::fromUtf8("checkBox_useDisplacementCut"));
	checkBox_useDisplacementCut->setChecked(false);
		
	spinbox_minDisplacement= new QDoubleSpinBox;
	spinbox_minDisplacement->setObjectName(QString::fromUtf8("spinbox_minDisplacement"));
	spinbox_minDisplacement->setRange(-100, 100);
  spinbox_minDisplacement->setValue(-100);
	spinbox_minDisplacement->setSingleStep(0.5);
	spinbox_minDisplacement->setFixedWidth(80);

	spinbox_maxDisplacement= new QDoubleSpinBox;
	spinbox_maxDisplacement->setObjectName(QString::fromUtf8("spinbox_maxDisplacement"));
	spinbox_maxDisplacement->setRange(0, 10000);
  spinbox_maxDisplacement->setValue(100);
	spinbox_maxDisplacement->setSingleStep(0.5);
	spinbox_maxDisplacement->setFixedWidth(80);


	checkBox_useEnergyCut= new QCheckBox("Energy Cut [GeV]");
	checkBox_useEnergyCut->setObjectName(QString::fromUtf8("checkBox_useEnergyCut"));
	checkBox_useEnergyCut->setChecked(false);
		
	spinbox_minEnergy= new QDoubleSpinBox;
	spinbox_minEnergy->setObjectName(QString::fromUtf8("spinbox_minEnergy"));
	spinbox_minEnergy->setRange(0, 100);
  spinbox_minEnergy->setValue(1);
	spinbox_minEnergy->setSingleStep(0.1);
	spinbox_minEnergy->setFixedWidth(80);

	spinbox_maxEnergy= new QDoubleSpinBox;
	spinbox_maxEnergy->setObjectName(QString::fromUtf8("spinbox_maxEnergy"));
	spinbox_maxEnergy->setRange(0, 100);
  spinbox_maxEnergy->setValue(100);
	spinbox_maxEnergy->setSingleStep(0.1);
	spinbox_maxEnergy->setFixedWidth(80);


	

	checkBox_useTrackSpeedCut= new QCheckBox("TrackSpeed Cut");
	checkBox_useTrackSpeedCut->setObjectName(QString::fromUtf8("checkBox_useTrackSpeedCut"));
	checkBox_useTrackSpeedCut->setChecked(false);
		
	spinbox_minTrackSpeed= new QDoubleSpinBox;
	spinbox_minTrackSpeed->setObjectName(QString::fromUtf8("spinbox_minTrackSpeed"));
	spinbox_minTrackSpeed->setRange(0, 1);
  spinbox_minTrackSpeed->setValue(0);
	spinbox_minTrackSpeed->setSingleStep(0.1);
	spinbox_minTrackSpeed->setFixedWidth(80);

	spinbox_maxTrackSpeed= new QDoubleSpinBox;
	spinbox_maxTrackSpeed->setObjectName(QString::fromUtf8("spinbox_maxTrackSpeed"));
	spinbox_maxTrackSpeed->setRange(0, 1);
  spinbox_maxTrackSpeed->setValue(1);
	spinbox_maxTrackSpeed->setSingleStep(0.1);
	spinbox_maxTrackSpeed->setFixedWidth(80);


	//## Define object actions
	connect(pushButton_inputFile, SIGNAL(clicked()), this, SLOT(BrowseInputFile()) );

	//## Set object layout
	QHBoxLayout* InputFileLayout= new QHBoxLayout;
	InputFileLayout->addWidget(label_inputFile);
	InputFileLayout->addWidget(comboBox_inputFile);
	InputFileLayout->addWidget(pushButton_inputFile);
	InputFileLayout->addStretch(1);

	//container
	QHBoxLayout* containerSizeLayout= new QHBoxLayout;
	containerSizeLayout->addWidget(label_containerSize);
	containerSizeLayout->addWidget(spinbox_containerSizeX);
	containerSizeLayout->addWidget(spinbox_containerSizeY);
	containerSizeLayout->addWidget(spinbox_containerSizeZ);
	containerSizeLayout->addStretch(1);

	QHBoxLayout* containerPosLayout= new QHBoxLayout;
	containerPosLayout->addWidget(label_containerPos);
	containerPosLayout->addWidget(spinbox_containerPosX);
	containerPosLayout->addWidget(spinbox_containerPosY);
	containerPosLayout->addWidget(spinbox_containerPosZ);
	containerPosLayout->addStretch(1);

	QHBoxLayout* containerVoxelSizeLayout= new QHBoxLayout;
	containerVoxelSizeLayout->addWidget(label_containerVoxelSize);
	containerVoxelSizeLayout->addWidget(spinbox_containerVoxelSizeX);
	containerVoxelSizeLayout->addWidget(spinbox_containerVoxelSizeY);
	containerVoxelSizeLayout->addWidget(spinbox_containerVoxelSizeZ);
	containerVoxelSizeLayout->addStretch(1);

	QHBoxLayout* VerbosityLayout= new QHBoxLayout;
	VerbosityLayout->addWidget(label_Verbosity);
	VerbosityLayout->addWidget(spinbox_Verbosity);
	VerbosityLayout->addStretch(1);

	
	QHBoxLayout* SmearTrackPointLayout= new QHBoxLayout;
	SmearTrackPointLayout->addWidget(checkBox_smearTrackPoints);
	SmearTrackPointLayout->addWidget(spinbox_detResolutionX);
	SmearTrackPointLayout->addWidget(spinbox_detResolutionY);
	SmearTrackPointLayout->addWidget(spinbox_detResolutionZ);
	SmearTrackPointLayout->addStretch(1);

	QHBoxLayout* SmearTOFLayout= new QHBoxLayout;	
	SmearTOFLayout->addWidget(checkBox_smearTimeOfFlight);
	SmearTOFLayout->addWidget(spinbox_timeOfFlightResolution);
	SmearTOFLayout->addStretch(1);

	QHBoxLayout* NMaxEventCutLayout= new QHBoxLayout;
	NMaxEventCutLayout->addWidget(checkBox_maxNEvents);
	NMaxEventCutLayout->addWidget(spinbox_maxNEvents);
	NMaxEventCutLayout->addStretch(1);

	QHBoxLayout* ThetaCutLayout= new QHBoxLayout;
	ThetaCutLayout->addWidget(checkBox_useScatteringAngleCut);
	ThetaCutLayout->addWidget(spinbox_minScatteringAngle);
	ThetaCutLayout->addWidget(spinbox_maxScatteringAngle);
	ThetaCutLayout->addStretch(1);

	QHBoxLayout* DisplacementCutLayout= new QHBoxLayout;
	DisplacementCutLayout->addWidget(checkBox_useDisplacementCut);
	DisplacementCutLayout->addWidget(spinbox_minDisplacement);
	DisplacementCutLayout->addWidget(spinbox_maxDisplacement);
	DisplacementCutLayout->addStretch(1);

	QHBoxLayout* EnergyCutLayout= new QHBoxLayout;	
	//EnergyCutLayout->addWidget(checkBox_useTrueMomentum);
	EnergyCutLayout->addWidget(checkBox_useEnergyCut);
	EnergyCutLayout->addWidget(spinbox_minEnergy);
	EnergyCutLayout->addWidget(spinbox_maxEnergy);
	EnergyCutLayout->addStretch(1);

	

	QHBoxLayout* TrackSpeedCutLayout= new QHBoxLayout;	
	TrackSpeedCutLayout->addWidget(checkBox_useTrackSpeedCut);
	TrackSpeedCutLayout->addWidget(spinbox_minTrackSpeed);
	TrackSpeedCutLayout->addWidget(spinbox_maxTrackSpeed);
	TrackSpeedCutLayout->addStretch(1);


	QHBoxLayout* DetPlaneZLayout[MAXNPLANES];
	
	for(int i=0;i<MAXNPLANES;i++){	
		DetPlaneZLayout[i]= new QHBoxLayout;
		DetPlaneZLayout[i]->addWidget(labelList_DetectorPlaneZ[i]);
		DetPlaneZLayout[i]->addWidget(spinboxList_DetectorPlaneZ[i]);
		DetPlaneZLayout[i]->addStretch(1);
	}//end loop nplanes


	QVBoxLayout* OptionFrameLayout= new QVBoxLayout;	
	OptionFrameLayout->addWidget(label_InfoSeparation);
	OptionFrameLayout->addLayout(InputFileLayout);
	OptionFrameLayout->addLayout(VerbosityLayout);
	OptionFrameLayout->addStretch(1);

	QVBoxLayout* DetOptionFrameLayout= new QVBoxLayout;	
	DetOptionFrameLayout->addWidget(label_InfoOptionSeparation);
	for(int i=0;i<MAXNPLANES;i++) DetOptionFrameLayout->addLayout(DetPlaneZLayout[i]);
	DetOptionFrameLayout->addLayout(containerSizeLayout);	
	DetOptionFrameLayout->addLayout(containerPosLayout);		
	DetOptionFrameLayout->addLayout(containerVoxelSizeLayout);
	DetOptionFrameLayout->addStretch(1);

	QVBoxLayout* SelectionFrameLayout= new QVBoxLayout;
	SelectionFrameLayout->addWidget(label_OptionSelectionSeparation);
	SelectionFrameLayout->addLayout(SmearTrackPointLayout);
	//SelectionFrameLayout->addLayout(SmearTOFLayout);
	//SelectionFrameLayout->addLayout(NMaxEventCutLayout);
	//SelectionFrameLayout->addLayout(ThetaCutLayout);
	//SelectionFrameLayout->addLayout(DisplacementCutLayout);	
	//SelectionFrameLayout->addLayout(EnergyCutLayout);
	//SelectionFrameLayout->addLayout(TrackSpeedCutLayout);
	SelectionFrameLayout->addStretch(1);

	
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(OptionFrameLayout);
	mainLayout->addLayout(DetOptionFrameLayout);
	mainLayout->addLayout(SelectionFrameLayout);
	mainLayout->addStretch(1);
	
	QVBoxLayout* mainVLayout = new QVBoxLayout;
	mainVLayout->addLayout(mainLayout);
	mainVLayout->addStretch(1);
  setLayout(mainVLayout);

}//close constructor


void ContainerTab::SetContainerOptions(){

	//## Pass gui options to parser

	//main options
	Gui::fConfigParser->fInputFileName= (comboBox_inputFile->currentText()).toStdString();

	//geometry options
	Gui::fConfigParser->fContainerSizeX= spinbox_containerSizeX->value();
	Gui::fConfigParser->fContainerSizeY= spinbox_containerSizeY->value();
	Gui::fConfigParser->fContainerSizeZ= spinbox_containerSizeZ->value();

	Gui::fConfigParser->fContainerPosX= spinbox_containerPosX->value();
	Gui::fConfigParser->fContainerPosY= spinbox_containerPosY->value();
	Gui::fConfigParser->fContainerPosZ= spinbox_containerPosZ->value();
	
	Gui::fConfigParser->fVoxelSizeX= spinbox_containerVoxelSizeX->value();
	Gui::fConfigParser->fVoxelSizeY= spinbox_containerVoxelSizeY->value();
	Gui::fConfigParser->fVoxelSizeZ= spinbox_containerVoxelSizeZ->value();

	(Gui::fConfigParser->fZPlane).clear();
	(Gui::fConfigParser->fZPlane).resize(0);
	for(int i=0;i<MAXNPLANES;i++){
		double thisZPlane= spinboxList_DetectorPlaneZ[i]->value();
		(Gui::fConfigParser->fZPlane).push_back(thisZPlane);	
	}

	//selection options
	Gui::fConfigParser->fSmearTrackPoints = checkBox_smearTrackPoints->isChecked();
	Gui::fConfigParser->fSigmaPosX= spinbox_detResolutionX->value();
	Gui::fConfigParser->fSigmaPosY= spinbox_detResolutionY->value();
	Gui::fConfigParser->fSigmaPosZ= spinbox_detResolutionZ->value();

	/*
	Gui::fConfigParser->fEMLLSmearTimeOfFlight= checkBox_smearTimeOfFlight->isChecked();
	Gui::fConfigParser->fEMLLTimeOfFlightResolution= spinbox_timeOfFlightResolution->value();
	Gui::fConfigParser->fUseEMLLMaxNEvents= checkBox_maxNEvents->isChecked();
	Gui::fConfigParser->fEMLLMaxNEvents= spinbox_maxNEvents->value();	
	Gui::fConfigParser->fUseEMLLThetaCut= checkBox_useScatteringAngleCut->isChecked();
	Gui::fConfigParser->fEMLLThetaMin= spinbox_minScatteringAngle->value();
	Gui::fConfigParser->fEMLLThetaMax= spinbox_maxScatteringAngle->value();
	Gui::fConfigParser->fUseEMLLDisplacementCut= checkBox_useDisplacementCut->isChecked();
	Gui::fConfigParser->fEMLLDisplacementMin= spinbox_minDisplacement->value();
	Gui::fConfigParser->fEMLLDisplacementMax= spinbox_maxDisplacement->value();
	Gui::fConfigParser->fUseEMLLEnergyCut= checkBox_useEnergyCut->isChecked();
	Gui::fConfigParser->fEMLLEnergyMin= spinbox_minEnergy->value();
	Gui::fConfigParser->fEMLLEnergyMax= spinbox_maxEnergy->value();
	Gui::fConfigParser->fUseEMLLTrackSpeedCut= checkBox_useTrackSpeedCut->isChecked();
	Gui::fConfigParser->fEMLLTrackSpeedMinCut= spinbox_minTrackSpeed->value();
	Gui::fConfigParser->fEMLLTrackSpeedMaxCut= spinbox_maxTrackSpeed->value();
	*/

}//close ContainerTab::SetContainerOptions()


void ContainerTab::BrowseInputFile(){
	
	QString fileName = QFileDialog::getOpenFileName(this,tr("Find Files"), QDir::currentPath(), tr("*.txt *.dat *.out"));

	if (!fileName.isEmpty()) {
  	if (comboBox_inputFile->findText(fileName) == -1) comboBox_inputFile->addItem(fileName);
    comboBox_inputFile->setCurrentIndex(comboBox_inputFile->findText(fileName));
  }
	
}//close ContainerTab::BrowseInputFile()


}//close namespace
