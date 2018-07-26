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
* @file EMLLTab.cc
* @class EMLLTab
* @brief GUI EM-ML algorithm tab menu
* 
* @author S. Riggi
* @date 25/04/2010
*/
#include <EMLLTab.h>
#include <Logger.h>
#include <Gui.h>

#include <QtGui>
#include <TString.h>

#include <iostream>

using namespace std;

namespace MuonPortalNS {

EMLLTab::EMLLTab(QWidget *parent)
     : QWidget(parent)
{
	
	//## Set config file name	
	fEMLLConfigFileName= Gui::GUI_CONFIG_DIR + std::string("/EMMLConfig.cfg");
	
	//## Define objects
	groupBox_EMLL= new QGroupBox(tr("EM-LL Algorithm"));
	groupBox_EMLL->setObjectName(QString::fromUtf8("groupBox_EMLL"));
	groupBox_EMLL->setCheckable(true);
	//groupBox_EMLL->setVisible(true);
	groupBox_EMLL->setStyleSheet("QGroupBox{font-size: 18px;font-weight: bold;} QGroupBox::title { background-color: transparent; subcontrol-position: top left;padding:2 13px;} ");

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
	comboBox_inputFile->addItem( QString::fromStdString(Gui::GUI_BASE_DIR) + "/data/simdata_MuonWritingAtCenterScenario.out");
	comboBox_inputFile->addItem("SampleScenario2.dat");
	comboBox_inputFile->addItem("SampleScenario3.dat");
	comboBox_inputFile->setFixedWidth(250);
	
	pushButton_inputFile = new QPushButton(tr("Browse"));
  pushButton_inputFile->setObjectName(QString::fromUtf8("pushButton_inputFile"));
	pushButton_inputFile->setFixedSize(QSize(80,20));


	QLabel* label_outputFile= new QLabel();
	label_outputFile->setObjectName(QString::fromUtf8("label_outputFile"));
	label_outputFile->setText("Output File");

	comboBox_outputFile= new QComboBox;
	comboBox_outputFile->setObjectName(QString::fromUtf8("comboBox_outputFile"));
	comboBox_outputFile->setEditable(true);
	comboBox_outputFile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	comboBox_outputFile->addItem("EMLLOutput.root");
	comboBox_outputFile->setFixedWidth(250);
	
	pushButton_outputFile = new QPushButton(tr("Browse"));
  pushButton_outputFile->setObjectName(QString::fromUtf8("pushButton_outputFile"));
	pushButton_outputFile->setFixedSize(QSize(80,20));

	checkBox_waitClustering= new QCheckBox("Use clustering info?");
	checkBox_waitClustering->setObjectName(QString::fromUtf8("checkBox_waitClustering"));
	checkBox_waitClustering->setChecked(false);	


	QLabel* label_clusteringFile= new QLabel();
	label_clusteringFile->setObjectName(QString::fromUtf8("label_clusteringFile"));
	label_clusteringFile->setText("Clustering File");

	comboBox_clusteringFile= new QComboBox;
	comboBox_clusteringFile->setObjectName(QString::fromUtf8("comboBox_clusteringFile"));
	comboBox_clusteringFile->setEditable(true);
	comboBox_clusteringFile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	comboBox_clusteringFile->addItem("FOFOutput.root");
	comboBox_clusteringFile->setFixedWidth(250);
	
	pushButton_clusteringFile = new QPushButton(tr("Browse"));
  pushButton_clusteringFile->setObjectName(QString::fromUtf8("pushButton_clusteringFile"));
	pushButton_clusteringFile->setFixedSize(QSize(80,20));


	checkBox_useParallelAlgorithm= new QCheckBox("Use parallel algorithm (NProc)?");
	checkBox_useParallelAlgorithm->setObjectName(QString::fromUtf8("checkBox_useParallelAlgorithm"));
	checkBox_useParallelAlgorithm->setChecked(false);	

	spinbox_NParallelProc= new QSpinBox;
	spinbox_NParallelProc->setObjectName(QString::fromUtf8("spinbox_NParallelProc"));
	spinbox_NParallelProc->setRange(1, 100);
  spinbox_NParallelProc->setValue(1);
	spinbox_NParallelProc->setSingleStep(1);
	spinbox_NParallelProc->setFixedWidth(80);

	checkBox_useQueueRun= new QCheckBox("Use queue system?");
	checkBox_useQueueRun->setObjectName(QString::fromUtf8("checkBox_useQueueRun"));
	checkBox_useQueueRun->setChecked(false);	

	QLabel* label_InfoOptionSeparation = new QLabel("Options");
	label_InfoOptionSeparation->setFrameStyle(QFrame::HLine | QFrame::Raised);
	label_InfoOptionSeparation->setStyleSheet("QLabel { color : red; }");

	QLabel* label_Verbosity= new QLabel();
	label_Verbosity->setObjectName(QString::fromUtf8("label_Verbosity"));
	label_Verbosity->setText("Verbosity");

	spinbox_Verbosity= new QSpinBox;
	spinbox_Verbosity->setObjectName(QString::fromUtf8("spinbox_Verbosity"));
	spinbox_Verbosity->setRange(0, 3);
  spinbox_Verbosity->setValue(1);
	spinbox_Verbosity->setSingleStep(1);
	spinbox_Verbosity->setFixedWidth(80);


	QLabel* label_NLLIteration= new QLabel();
	label_NLLIteration->setObjectName(QString::fromUtf8("label_NLLIteration"));
	label_NLLIteration->setText("NIterations");

	spinbox_NLLIteration= new QSpinBox;
	spinbox_NLLIteration->setObjectName(QString::fromUtf8("spinbox_NLLIteration"));
	spinbox_NLLIteration->setRange(0, 1.e+99);
  spinbox_NLLIteration->setValue(1000);
	spinbox_NLLIteration->setSingleStep(1);
	spinbox_NLLIteration->setFixedWidth(100);

	QLabel* label_initScatteringDensity= new QLabel();
	label_initScatteringDensity->setObjectName(QString::fromUtf8("label_initScatteringDensity"));
	label_initScatteringDensity->setText("Init Lambda [rad^2/cm]");

	spinbox_initScatteringDensity= new QScienceSpinBox;
	spinbox_initScatteringDensity->setObjectName(QString::fromUtf8("spinbox_initScatteringDensity"));
	spinbox_initScatteringDensity->setRange(0, 100);
  spinbox_initScatteringDensity->setValue(6.11172e-09);
	spinbox_initScatteringDensity->setSingleStep(1.e-09);
	spinbox_initScatteringDensity->setDecimals(5);
	spinbox_initScatteringDensity->setFixedWidth(120);

	QLabel* label_referenceMomentum= new QLabel();
	label_referenceMomentum->setObjectName(QString::fromUtf8("label_referenceMomentum"));
	label_referenceMomentum->setText("p0-p [GeV]");

	spinbox_referenceMomentum= new QDoubleSpinBox;
	spinbox_referenceMomentum->setObjectName(QString::fromUtf8("spinbox_referenceMomentum"));
	spinbox_referenceMomentum->setRange(0, 100);
  spinbox_referenceMomentum->setValue(1.1006);
	spinbox_referenceMomentum->setSingleStep(0.1);
	spinbox_referenceMomentum->setFixedWidth(80);

	spinbox_averageMomentum= new QDoubleSpinBox;
	spinbox_averageMomentum->setObjectName(QString::fromUtf8("spinbox_averageMomentum"));
	spinbox_averageMomentum->setRange(0, 100);
  spinbox_averageMomentum->setValue(1.1006);
	spinbox_averageMomentum->setSingleStep(0.1);
	spinbox_averageMomentum->setFixedWidth(80);


	checkBox_useResidualStopping= new QCheckBox("Use residual stop (goal,niter)?");
	checkBox_useResidualStopping->setObjectName(QString::fromUtf8("checkBox_useResidualStopping"));
	checkBox_useResidualStopping->setChecked(true);
	
	spinbox_residualGoal= new QDoubleSpinBox;
	spinbox_residualGoal->setObjectName(QString::fromUtf8("spinbox_residualGoal"));
	spinbox_residualGoal->setRange(0.,1.);
  spinbox_residualGoal->setValue(0.01);	
	spinbox_residualGoal->setDecimals(5);
	spinbox_residualGoal->setSingleStep(0.001);
	spinbox_residualGoal->setFixedWidth(100);

	spinbox_nIterForResidualStopping= new QSpinBox;
	spinbox_nIterForResidualStopping->setObjectName(QString::fromUtf8("spinbox_nIterForResidualStopping"));
	spinbox_nIterForResidualStopping->setRange(0, 10000);
  spinbox_nIterForResidualStopping->setValue(5);
	spinbox_nIterForResidualStopping->setSingleStep(1);
	spinbox_nIterForResidualStopping->setFixedWidth(80);

	checkBox_useDeltaLLStopping= new QCheckBox("Use LL stop (goal,niter)?");
	checkBox_useDeltaLLStopping->setObjectName(QString::fromUtf8("checkBox_useDeltaLLStopping"));
	checkBox_useDeltaLLStopping->setChecked(false);

	spinbox_deltaLLGoal= new QDoubleSpinBox;
	spinbox_deltaLLGoal->setObjectName(QString::fromUtf8("spinbox_deltaLLGoal"));
	spinbox_deltaLLGoal->setRange(0, 10);
  spinbox_deltaLLGoal->setValue(1);
	spinbox_deltaLLGoal->setDecimals(5);
	spinbox_deltaLLGoal->setSingleStep(1.e-6);
	spinbox_deltaLLGoal->setFixedWidth(120);

	spinbox_nIterForDeltaLLStopping= new QSpinBox;
	spinbox_nIterForDeltaLLStopping->setObjectName(QString::fromUtf8("spinbox_nIterForDeltaLLStopping"));
	spinbox_nIterForDeltaLLStopping->setRange(0, 10000);
  spinbox_nIterForDeltaLLStopping->setValue(5);
	spinbox_nIterForDeltaLLStopping->setSingleStep(1);
	spinbox_nIterForDeltaLLStopping->setFixedWidth(80);


	QLabel* label_OptionSelectionSeparation = new QLabel("Selection");
	label_OptionSelectionSeparation->setFrameStyle(QFrame::HLine | QFrame::Raised);
	label_OptionSelectionSeparation->setStyleSheet("QLabel { color : green; }");

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

	
	checkBox_useTrueMomentum= new QCheckBox("Use true momentum?");
	checkBox_useTrueMomentum->setObjectName(QString::fromUtf8("checkBox_useTrueMomentum"));
	checkBox_useTrueMomentum->setChecked(false);

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


	checkBox_smearTimeOfFlight= new QCheckBox("Smear TOF?");
	checkBox_smearTimeOfFlight->setObjectName(QString::fromUtf8("checkBox_smearTimeOfFlight"));
	checkBox_smearTimeOfFlight->setChecked(false);
		
	spinbox_timeOfFlightResolution= new QDoubleSpinBox;
	spinbox_timeOfFlightResolution->setObjectName(QString::fromUtf8("spinbox_timeOfFlightResolution"));
	spinbox_timeOfFlightResolution->setRange(0, 100);
  spinbox_timeOfFlightResolution->setValue(5);
	spinbox_timeOfFlightResolution->setSingleStep(0.1);
	spinbox_timeOfFlightResolution->setFixedWidth(80);

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

	spinbox_nMinEventInVoxelForUpdate= new QSpinBox;
	spinbox_nMinEventInVoxelForUpdate->setObjectName(QString::fromUtf8("spinbox_nMinEventInVoxelForUpdate"));
	spinbox_nMinEventInVoxelForUpdate->setRange(0, 1000);
  spinbox_nMinEventInVoxelForUpdate->setValue(10);
	spinbox_nMinEventInVoxelForUpdate->setSingleStep(1);
	spinbox_nMinEventInVoxelForUpdate->setFixedWidth(80);

	checkBox_useFilterVoxelEvents= new QCheckBox("Filter voxel events?");
	checkBox_useFilterVoxelEvents->setObjectName(QString::fromUtf8("checkBox_useFilterVoxelEvents"));
	checkBox_useFilterVoxelEvents->setChecked(true);
	
	checkBox_maxNEvents= new QCheckBox("Use max NEvents?");
	checkBox_maxNEvents->setObjectName(QString::fromUtf8("checkBox_maxNEvents"));
	checkBox_maxNEvents->setChecked(true);

	checkBox_useQuantileUpdate= new QCheckBox("Use quantile update?");
	checkBox_useQuantileUpdate->setObjectName(QString::fromUtf8("checkBox_useQuantileUpdate"));
	checkBox_useQuantileUpdate->setChecked(false);

	spinbox_quantileProb= new QDoubleSpinBox;
	spinbox_quantileProb->setObjectName(QString::fromUtf8("spinbox_quantileProb"));
	spinbox_quantileProb->setRange(0, 1);
  spinbox_quantileProb->setValue(0.5);
	spinbox_quantileProb->setSingleStep(0.1);
	spinbox_quantileProb->setFixedWidth(80);

	spinbox_maxNEvents= new QSpinBox;
	spinbox_maxNEvents->setObjectName(QString::fromUtf8("spinbox_maxNEvents"));
	spinbox_maxNEvents->setRange(0, 1.e+8);
  spinbox_maxNEvents->setValue(500000);
	spinbox_maxNEvents->setSingleStep(1);
	spinbox_maxNEvents->setFixedWidth(120);

	checkBox_useTopBottomEvents= new QCheckBox("Use top/bottom events?");
	checkBox_useTopBottomEvents->setObjectName(QString::fromUtf8("checkBox_useTopBottomEvents"));
	checkBox_useTopBottomEvents->setChecked(true);

	checkBox_useSideEvents= new QCheckBox("Use side events?");
	checkBox_useSideEvents->setObjectName(QString::fromUtf8("checkBox_useSideEvents"));
	checkBox_useSideEvents->setChecked(false);

	checkBox_usePOCAInLL= new QCheckBox("Use POCA?");
	checkBox_usePOCAInLL->setObjectName(QString::fromUtf8("checkBox_usePOCAInLL"));
	checkBox_usePOCAInLL->setChecked(true);

	checkBox_useDetectorCovariance= new QCheckBox("Use Edet (Ex, Ey, Exy)?");
	checkBox_useDetectorCovariance->setObjectName(QString::fromUtf8("checkBox_useDetectorCovariance"));
	checkBox_useDetectorCovariance->setChecked(false);	
    
	spinbox_Ex= new QScienceSpinBox;
	spinbox_Ex->setObjectName(QString::fromUtf8("spinbox_Ex"));
	spinbox_Ex->setRange(0, 100);
  spinbox_Ex->setValue(0.000120728);
	spinbox_Ex->setSingleStep(0.001);
	spinbox_Ex->setDecimals(5);
	spinbox_Ex->setFixedWidth(120);
	
	spinbox_Ey= new QScienceSpinBox;
	spinbox_Ey->setObjectName(QString::fromUtf8("spinbox_Ey"));
	spinbox_Ey->setRange(0, 100);
  spinbox_Ey->setValue(4.19991);
	spinbox_Ey->setSingleStep(0.001);
	spinbox_Ey->setDecimals(5);
	spinbox_Ey->setFixedWidth(120);

	spinbox_Exy= new QScienceSpinBox;
	spinbox_Exy->setObjectName(QString::fromUtf8("spinbox_Exy"));
	spinbox_Exy->setRange(-100, 100);
  spinbox_Exy->setValue(0.00061289);
	spinbox_Exy->setSingleStep(0.001);
	spinbox_Exy->setDecimals(5);
	spinbox_Exy->setFixedWidth(120);

	checkBox_useRegularization= new QCheckBox("Use regularization?");
	checkBox_useRegularization->setObjectName(QString::fromUtf8("checkBox_useRegularization"));
	checkBox_useRegularization->setChecked(false);	

	spinbox_regularizationParameter= new QScienceSpinBox;
	spinbox_regularizationParameter->setObjectName(QString::fromUtf8("spinbox_regularizationParameter"));
	spinbox_regularizationParameter->setRange(0, 1.e+20);
  spinbox_regularizationParameter->setValue(1.e+8);
	spinbox_regularizationParameter->setSingleStep(100);
	spinbox_regularizationParameter->setDecimals(2);
	spinbox_regularizationParameter->setFixedWidth(100);

	checkBox_useMedianUpdate= new QCheckBox("Use median update?");
	checkBox_useMedianUpdate->setObjectName(QString::fromUtf8("checkBox_useMedianUpdate"));
	checkBox_useMedianUpdate->setChecked(false);	


	QLabel* label_SelectionImagingSeparation = new QLabel("Analysis & Draw Options");
	label_SelectionImagingSeparation->setFrameStyle(QFrame::HLine | QFrame::Raised);
	label_SelectionImagingSeparation->setStyleSheet("QLabel { color : blue; }");

	
	
	QLabel* label_alarmThreshold= new QLabel();
	label_alarmThreshold->setObjectName(QString::fromUtf8("label_alarmThreshold"));
	label_alarmThreshold->setText("Alarm Threshold [NEventThr,LambdaThr]");

	spinbox_alarmThreshold= new QDoubleSpinBox;
	spinbox_alarmThreshold->setObjectName(QString::fromUtf8("spinbox_alarmThreshold"));
	spinbox_alarmThreshold->setRange(-6, 2);
  spinbox_alarmThreshold->setValue(-3.2);
	spinbox_alarmThreshold->setSingleStep(0.1);
	spinbox_alarmThreshold->setFixedWidth(80);
	
	spinbox_NEventThreshold= new QSpinBox;
	spinbox_NEventThreshold->setObjectName(QString::fromUtf8("spinbox_NEventThreshold"));
	spinbox_NEventThreshold->setRange(0, 10000);
  spinbox_NEventThreshold->setValue(100);
	spinbox_NEventThreshold->setSingleStep(1);
	spinbox_NEventThreshold->setFixedWidth(80);
	
	QLabel* label_drawStyle= new QLabel();
	label_drawStyle->setObjectName(QString::fromUtf8("label_drawStyle"));
	label_drawStyle->setText("Draw style");

	comboBox_drawStyle= new QComboBox;
	comboBox_drawStyle->setObjectName(QString::fromUtf8("comboBox_drawStyle"));
	comboBox_drawStyle->setEditable(false);
	comboBox_drawStyle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	comboBox_drawStyle->addItem("COLZ");
	comboBox_drawStyle->addItem("LEGO");
	comboBox_drawStyle->addItem("CONT4Z");
	comboBox_drawStyle->setFixedWidth(100);

	//## Define object actions
	connect(pushButton_inputFile, SIGNAL(clicked()), this, SLOT(BrowseInputFile()) );
	connect(pushButton_outputFile, SIGNAL(clicked()), this, SLOT(BrowseOutputFile()) );
	connect(pushButton_clusteringFile, SIGNAL(clicked()), this, SLOT(BrowseClusteringFile()) );
	
	//## Define object layout
	QHBoxLayout* InputFileLayout= new QHBoxLayout;
	InputFileLayout->addWidget(label_inputFile);
	InputFileLayout->addWidget(comboBox_inputFile);
	InputFileLayout->addWidget(pushButton_inputFile);
	InputFileLayout->addStretch(1);

	QHBoxLayout* OutputFileLayout= new QHBoxLayout;
	OutputFileLayout->addWidget(label_outputFile);
	OutputFileLayout->addWidget(comboBox_outputFile);
	OutputFileLayout->addWidget(pushButton_outputFile);
	OutputFileLayout->addStretch(1);	

	QHBoxLayout* WaitClusteringLayout= new QHBoxLayout;
	WaitClusteringLayout->addWidget(checkBox_waitClustering);
	//WaitClusteringLayout->addWidget(label_clusteringFile);
	//WaitClusteringLayout->addWidget(comboBox_clusteringFile);
	//WaitClusteringLayout->addWidget(pushButton_clusteringFile);
	WaitClusteringLayout->addStretch(1);

	QHBoxLayout* ParallelAlgorithmLayout= new QHBoxLayout;
	ParallelAlgorithmLayout->addWidget(checkBox_useParallelAlgorithm);
	ParallelAlgorithmLayout->addWidget(spinbox_NParallelProc);
	ParallelAlgorithmLayout->addStretch(1);
	
	QHBoxLayout* VerbosityLayout= new QHBoxLayout;
	VerbosityLayout->addWidget(label_Verbosity);
	VerbosityLayout->addWidget(spinbox_Verbosity);
	VerbosityLayout->addStretch(1);

	QHBoxLayout* NIterationLayout= new QHBoxLayout;
	NIterationLayout->addWidget(label_NLLIteration);
	NIterationLayout->addWidget(spinbox_NLLIteration);
	NIterationLayout->addStretch(1);

	QHBoxLayout* LambdaInitLayout= new QHBoxLayout;
	LambdaInitLayout->addWidget(label_initScatteringDensity);
	LambdaInitLayout->addWidget(spinbox_initScatteringDensity);
	LambdaInitLayout->addStretch(1);

	QHBoxLayout* ReferenceMomentumLayout= new QHBoxLayout;
	ReferenceMomentumLayout->addWidget(label_referenceMomentum);
	ReferenceMomentumLayout->addWidget(spinbox_referenceMomentum);
	ReferenceMomentumLayout->addWidget(spinbox_averageMomentum);
	ReferenceMomentumLayout->addStretch(1);

	QHBoxLayout* DetCovarianceLayout= new QHBoxLayout;
	DetCovarianceLayout->addWidget(checkBox_useDetectorCovariance);
	DetCovarianceLayout->addWidget(spinbox_Ex);
	DetCovarianceLayout->addWidget(spinbox_Ey);
	DetCovarianceLayout->addWidget(spinbox_Exy);
	DetCovarianceLayout->addStretch(1);
	
	QHBoxLayout* RegularizationLayout= new QHBoxLayout;
	RegularizationLayout->addWidget(checkBox_useRegularization);
	RegularizationLayout->addWidget(spinbox_regularizationParameter);
	RegularizationLayout->addStretch(1);

	QHBoxLayout* MedianUpdateLayout= new QHBoxLayout;
	MedianUpdateLayout->addWidget(checkBox_useMedianUpdate);
	MedianUpdateLayout->addStretch(1);

	QHBoxLayout* ResidualStopLayout= new QHBoxLayout;
	ResidualStopLayout->addWidget(checkBox_useResidualStopping);
	ResidualStopLayout->addWidget(spinbox_residualGoal);
	ResidualStopLayout->addWidget(spinbox_nIterForResidualStopping);
	ResidualStopLayout->addStretch(1);

	QHBoxLayout* DeltaLLStopLayout= new QHBoxLayout;
	DeltaLLStopLayout->addWidget(checkBox_useDeltaLLStopping);
	DeltaLLStopLayout->addWidget(spinbox_deltaLLGoal);
	DeltaLLStopLayout->addWidget(spinbox_nIterForDeltaLLStopping);
	DeltaLLStopLayout->addStretch(1);
	
	QHBoxLayout* EventCutLayout= new QHBoxLayout;
	EventCutLayout->addWidget(checkBox_useTopBottomEvents);
	EventCutLayout->addWidget(checkBox_useSideEvents);
	EventCutLayout->addWidget(checkBox_usePOCAInLL);
	EventCutLayout->addStretch(1);

	QHBoxLayout* NMaxEventCutLayout= new QHBoxLayout;
	NMaxEventCutLayout->addWidget(checkBox_maxNEvents);
	NMaxEventCutLayout->addWidget(spinbox_maxNEvents);
	NMaxEventCutLayout->addStretch(1);

	QHBoxLayout* FilterVoxelEventLayout= new QHBoxLayout;
	FilterVoxelEventLayout->addWidget(checkBox_useFilterVoxelEvents);
	FilterVoxelEventLayout->addWidget(spinbox_nMinEventInVoxelForUpdate);
	FilterVoxelEventLayout->addStretch(1);

	QHBoxLayout* QuantileUpdateLayout= new QHBoxLayout;
	QuantileUpdateLayout->addWidget(checkBox_useQuantileUpdate);
	QuantileUpdateLayout->addWidget(spinbox_quantileProb);
	QuantileUpdateLayout->addStretch(1);
	
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

	QHBoxLayout* SmearTOFLayout= new QHBoxLayout;	
	SmearTOFLayout->addWidget(checkBox_smearTimeOfFlight);
	SmearTOFLayout->addWidget(spinbox_timeOfFlightResolution);
	SmearTOFLayout->addStretch(1);

	QHBoxLayout* TrackSpeedCutLayout= new QHBoxLayout;	
	TrackSpeedCutLayout->addWidget(checkBox_useTrackSpeedCut);
	TrackSpeedCutLayout->addWidget(spinbox_minTrackSpeed);
	TrackSpeedCutLayout->addWidget(spinbox_maxTrackSpeed);
	TrackSpeedCutLayout->addStretch(1);

	QHBoxLayout* AlarmThresholdLayout= new QHBoxLayout;
	AlarmThresholdLayout->addWidget(label_alarmThreshold);
	AlarmThresholdLayout->addWidget(spinbox_NEventThreshold);
	AlarmThresholdLayout->addWidget(spinbox_alarmThreshold);
	AlarmThresholdLayout->addStretch(1);

	QHBoxLayout* DrawStyleLayout= new QHBoxLayout;
	DrawStyleLayout->addWidget(label_drawStyle);
	DrawStyleLayout->addWidget(comboBox_drawStyle);
	DrawStyleLayout->addStretch(1);

	
	QVBoxLayout* OptionFrameLayout= new QVBoxLayout;	
	OptionFrameLayout->addWidget(label_InfoOptionSeparation);
	OptionFrameLayout->addLayout(VerbosityLayout);
	OptionFrameLayout->addLayout(NIterationLayout);
	OptionFrameLayout->addLayout(LambdaInitLayout);
	OptionFrameLayout->addLayout(ReferenceMomentumLayout);
	OptionFrameLayout->addWidget(checkBox_useTrueMomentum);
	OptionFrameLayout->addLayout(DetCovarianceLayout);
	OptionFrameLayout->addLayout(RegularizationLayout);
	OptionFrameLayout->addLayout(MedianUpdateLayout);
	OptionFrameLayout->addLayout(QuantileUpdateLayout);
	OptionFrameLayout->addLayout(ResidualStopLayout);
	OptionFrameLayout->addLayout(DeltaLLStopLayout);
	OptionFrameLayout->addStretch(1);

	QVBoxLayout* SelectionFrameLayout= new QVBoxLayout;
	SelectionFrameLayout->addWidget(label_OptionSelectionSeparation);
	SelectionFrameLayout->addLayout(NMaxEventCutLayout);
	SelectionFrameLayout->addLayout(EventCutLayout);
	SelectionFrameLayout->addLayout(ThetaCutLayout);
	SelectionFrameLayout->addLayout(DisplacementCutLayout);	
	SelectionFrameLayout->addLayout(EnergyCutLayout);
	SelectionFrameLayout->addLayout(SmearTOFLayout);
	SelectionFrameLayout->addLayout(TrackSpeedCutLayout);
	SelectionFrameLayout->addLayout(FilterVoxelEventLayout);
	
	SelectionFrameLayout->addStretch();

	QHBoxLayout* MiddleFrameLayout= new QHBoxLayout;
	MiddleFrameLayout->addLayout(OptionFrameLayout);
	MiddleFrameLayout->addStretch(1);
	MiddleFrameLayout->addLayout(SelectionFrameLayout);
	MiddleFrameLayout->addStretch(1);

	QVBoxLayout* GroupBoxEMLLLayout = new QVBoxLayout;
	GroupBoxEMLLLayout->addSpacerItem(new QSpacerItem(200, 5, QSizePolicy::Maximum, QSizePolicy::Maximum));
	GroupBoxEMLLLayout->addWidget(label_InfoSeparation);
	//GroupBoxEMLLLayout->addLayout(InputFileLayout);
	//GroupBoxEMLLLayout->addLayout(OutputFileLayout);
	GroupBoxEMLLLayout->addLayout(WaitClusteringLayout);
	GroupBoxEMLLLayout->addLayout(ParallelAlgorithmLayout);
	GroupBoxEMLLLayout->addWidget(checkBox_useQueueRun);
	GroupBoxEMLLLayout->addSpacerItem(new QSpacerItem(0, 15, QSizePolicy::Maximum, QSizePolicy::Maximum));
	
	GroupBoxEMLLLayout->addLayout(MiddleFrameLayout);
	/*
	GroupBoxEMLLLayout->addWidget(label_InfoOptionSeparation);
	GroupBoxEMLLLayout->addLayout(VerbosityLayout);
	GroupBoxEMLLLayout->addLayout(NIterationLayout);
	GroupBoxEMLLLayout->addLayout(LambdaInitLayout);
	GroupBoxEMLLLayout->addLayout(ReferenceMomentumLayout);
	GroupBoxEMLLLayout->addLayout(DetCovarianceLayout);
	GroupBoxEMLLLayout->addLayout(RegularizationLayout);
	GroupBoxEMLLLayout->addLayout(MedianUpdateLayout);
	GroupBoxEMLLLayout->addLayout(ResidualStopLayout);
	GroupBoxEMLLLayout->addLayout(DeltaLLStopLayout);
	GroupBoxEMLLLayout->addSpacerItem(new QSpacerItem(0, 5, QSizePolicy::Maximum, QSizePolicy::Maximum));
	GroupBoxEMLLLayout->addWidget(label_OptionSelectionSeparation);
	GroupBoxEMLLLayout->addLayout(NMaxEventCutLayout);
	GroupBoxEMLLLayout->addLayout(EventCutLayout);
	GroupBoxEMLLLayout->addLayout(ThetaCutLayout);
	GroupBoxEMLLLayout->addLayout(DisplacementCutLayout);
	GroupBoxEMLLLayout->addLayout(EnergyCutLayout);
	GroupBoxEMLLLayout->addLayout(FilterVoxelEventLayout);
	*/

	GroupBoxEMLLLayout->addSpacerItem(new QSpacerItem(0, 5, QSizePolicy::Maximum, QSizePolicy::Maximum));
	GroupBoxEMLLLayout->addWidget(label_SelectionImagingSeparation);
	GroupBoxEMLLLayout->addLayout(AlarmThresholdLayout);
	GroupBoxEMLLLayout->addLayout(DrawStyleLayout);
	GroupBoxEMLLLayout->addStretch(1);
	groupBox_EMLL->setLayout(GroupBoxEMLLLayout);

	//QHBoxLayout* mainLayout = new QHBoxLayout;
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(groupBox_EMLL);
	mainLayout->addStretch(1);

	QVBoxLayout* mainVLayout = new QVBoxLayout;
	mainVLayout->addLayout(mainLayout);
	mainVLayout->addStretch(1);
  setLayout(mainVLayout);

}//close constructor


void EMLLTab::SetEMLLOptions(){

	//## Pass gui options to parser
	Gui::fConfigParser->fUseEMLLAlgorithm= groupBox_EMLL->isChecked();
	Gui::fConfigParser->fEMLLVerbosity= spinbox_Verbosity->value();
	//Gui::fConfigParser->fEMLLInputFileName= (comboBox_inputFile->currentText()).toStdString();
	//Gui::fConfigParser->fEMLLOutputFileName= (comboBox_outputFile->currentText()).toStdString();
	Gui::fConfigParser->fEMLLUseClusteringInfo= checkBox_waitClustering->isChecked();
	//Gui::fConfigParser->fEMLLClusteringFileName= (comboBox_clusteringFile->currentText()).toStdString();

	Gui::fConfigParser->fUseEMLLQueueRun= checkBox_useQueueRun->isChecked();

	Gui::fConfigParser->fUseEMLLParallelAlgorithm= checkBox_useParallelAlgorithm->isChecked();
	Gui::fConfigParser->fEMLLNParallelProc= spinbox_NParallelProc->value();

	Gui::fConfigParser->fNIterations= spinbox_NLLIteration->value();
	Gui::fConfigParser->fInitialScatteringDensity= spinbox_initScatteringDensity->value();
	Gui::fConfigParser->fAverageMomentum= spinbox_averageMomentum->value();
	Gui::fConfigParser->fReferenceMomentum= spinbox_referenceMomentum->value();
	Gui::fConfigParser->fUseDetectorCovariance= checkBox_useDetectorCovariance->isChecked();
	Gui::fConfigParser->fEx= spinbox_Ex->value();
	Gui::fConfigParser->fEy= spinbox_Ey->value();
	Gui::fConfigParser->fExy= spinbox_Exy->value();		
	Gui::fConfigParser->fUseMedianUpdate= checkBox_useMedianUpdate->isChecked();
	Gui::fConfigParser->fUseRegularization= checkBox_useRegularization->isChecked();
	Gui::fConfigParser->fBeta= spinbox_regularizationParameter->value();
	Gui::fConfigParser->fUseResErrStopping= checkBox_useResidualStopping->isChecked();
	Gui::fConfigParser->fResErrTolerance= spinbox_residualGoal->value(); 
	Gui::fConfigParser->fNIterForResErrStopping= spinbox_nIterForResidualStopping->value();
	Gui::fConfigParser->fUseLogLikeStopping= checkBox_useDeltaLLStopping->isChecked();
	Gui::fConfigParser->fLogLikeTolerance= spinbox_deltaLLGoal->value(); 
	Gui::fConfigParser->fNIterForLogLikeStopping= spinbox_nIterForDeltaLLStopping->value();
	Gui::fConfigParser->fUseEMLLMaxNEvents= checkBox_maxNEvents->isChecked();
	Gui::fConfigParser->fEMLLMaxNEvents= spinbox_maxNEvents->value();	
	Gui::fConfigParser->fUseEMLLThetaCut= checkBox_useScatteringAngleCut->isChecked();
	Gui::fConfigParser->fEMLLThetaMin= spinbox_minScatteringAngle->value();
	Gui::fConfigParser->fEMLLThetaMax= spinbox_maxScatteringAngle->value();
	Gui::fConfigParser->fUseEMLLDisplacementCut= checkBox_useDisplacementCut->isChecked();
	Gui::fConfigParser->fEMLLDisplacementMin= spinbox_minDisplacement->value();
	Gui::fConfigParser->fEMLLDisplacementMax= spinbox_maxDisplacement->value();
	Gui::fConfigParser->fUsePOCAInLL= checkBox_usePOCAInLL->isChecked();
	Gui::fConfigParser->fUseTopBottomEvents= checkBox_useTopBottomEvents->isChecked();
	Gui::fConfigParser->fUseSideEvents= checkBox_useSideEvents->isChecked();
	Gui::fConfigParser->fEMLLSignalThreshold= spinbox_alarmThreshold->value();
	Gui::fConfigParser->fEMLLNEventThreshold= spinbox_NEventThreshold->value();
	Gui::fConfigParser->fEMLLDrawStyleName= (comboBox_drawStyle->currentText()).toStdString();

	Gui::fConfigParser->fUseEMLLTrueMomentum= checkBox_useTrueMomentum->isChecked();
	Gui::fConfigParser->fUseEMLLEnergyCut= checkBox_useEnergyCut->isChecked();
	Gui::fConfigParser->fEMLLEnergyMin= spinbox_minEnergy->value();
	Gui::fConfigParser->fEMLLEnergyMax= spinbox_maxEnergy->value();

	Gui::fConfigParser->fUseEMLLTrackSpeedCut= checkBox_useTrackSpeedCut->isChecked();
	Gui::fConfigParser->fEMLLTrackSpeedMinCut= spinbox_minTrackSpeed->value();
	Gui::fConfigParser->fEMLLTrackSpeedMaxCut= spinbox_maxTrackSpeed->value();

	Gui::fConfigParser->fEMLLSmearTimeOfFlight= checkBox_smearTimeOfFlight->isChecked();
	Gui::fConfigParser->fEMLLTimeOfFlightResolution= spinbox_timeOfFlightResolution->value();

	Gui::fConfigParser->fNMinEventInVoxelForUpdate= spinbox_nMinEventInVoxelForUpdate->value();
	Gui::fConfigParser->fUseFilterVoxelEvents= checkBox_useFilterVoxelEvents->isChecked();

	Gui::fConfigParser->fUseEMLLQuantileUpdate = checkBox_useQuantileUpdate->isChecked();
	Gui::fConfigParser->fEMLLQuantileProb = spinbox_quantileProb->value();

	//## Dump config file
	bool status= DumpEMLLConfigFile();
	if(!status){
		std::string errMsg("Cannot dump config options to file...exit!");
		ERROR_LOG(errMsg);
		QMessageBox::critical(this->window(), "ERROR", errMsg.c_str());
		QMetaObject::invokeMethod(this->window(), "close", Qt::QueuedConnection);
	}

}//close EMLLTab::SetEMLLOptions()


bool EMLLTab::DumpEMLLConfigFile(){

	//## Write config file
	std::ofstream dump_file;
  dump_file.open(fEMLLConfigFileName.c_str());
	if(!dump_file.is_open()){
		return false;	
	}
	
	dump_file << "################################"<<endl;
	dump_file << "###    GEOMETRY CONFIG          "<<endl;
	dump_file << "################################"<<endl;
	dump_file << "#### DETECTOR PLANE Z POSITIONS "<<endl;
	for(int j=0;j<Gui::fConfigParser->fZPlane.size();j++){
		dump_file << "planeZ  "<< Gui::fConfigParser->fZPlane[j] <<"	                            | Plane Z coordinate"<<endl;
	}
	dump_file << "###################################"<<endl;
	dump_file << "###                                "<<endl;
	dump_file << "### CONTAINER OPTIONS              "<<endl;
	dump_file << "containerSize  "<< Gui::fConfigParser->fContainerSizeX << "  "<<Gui::fConfigParser->fContainerSizeY<<"  "<<Gui::fConfigParser->fContainerSizeZ<<"  | Container size (in cm) "<<endl;	
	dump_file << "containerPosition  "<<Gui::fConfigParser->fContainerPosX << "  "<<Gui::fConfigParser->fContainerPosY<<"  "<<Gui::fConfigParser->fContainerPosZ<< "| Container position in the world (in cm)"<<endl;
	dump_file << "voxelSize  "<<Gui::fConfigParser->fVoxelSizeX << "  "<<Gui::fConfigParser->fVoxelSizeY<<"  "<<Gui::fConfigParser->fVoxelSizeZ<< "| Voxel size (in cm)"<<endl;
	dump_file << " ###################################"<<endl;

	dump_file << "####################################"<<endl;
	dump_file << "###    RECONSTRUCTION CONFIG        "<<endl;
	dump_file << "####################################"<<endl;
	dump_file << "verbosityEMLL  "<< Gui::fConfigParser->fEMLLVerbosity <<"     | Verbosity reconstruction"<<endl;
	//dump_file << "inputAsciiRecoFile  "<< Gui::fConfigParser->fEMLLInputFileName <<"     | Verbosity reconstruction"<<endl;
	//dump_file << "outputRecoFile  "<< Gui::fConfigParser->fEMLLOutputFileName <<"    | Output file with rec info"<<endl;
	dump_file << "###"<<endl;
	
	dump_file << "#####################################"<<endl;
	dump_file << "#### DATA SELECTION                  "<<endl;
	dump_file << "#####################################"<<endl;
	dump_file << "useEMLLNEventCut  ";
	if(Gui::fConfigParser->fUseEMLLMaxNEvents) dump_file << "T  ";
	else dump_file << "F  ";
	dump_file << Gui::fConfigParser->fEMLLMaxNEvents<<"                      | Max number of events to be reconstructed (value,-1=rec all events)"<<endl;

	dump_file << "smearTrackPoints  ";	
	if(Gui::fConfigParser->fSmearTrackPoints) dump_file << "T  ";
	else dump_file << "F  ";
	dump_file << "    | Smear track points with detector resolution (T/F)"<<endl;

	dump_file << "detectorResolution  "<<Gui::fConfigParser->fSigmaPosX<<"  "<< Gui::fConfigParser->fSigmaPosY<< "  "<<Gui::fConfigParser->fSigmaPosZ<<" 	       	    | XYZ Position sigma resolution in cm (value,value,value)"<<endl;

	dump_file << "useEMLLEnergyCut  ";
	if(Gui::fConfigParser->fUseEMLLEnergyCut) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file << Gui::fConfigParser->fEMLLEnergyMin <<"  "<<Gui::fConfigParser->fEMLLEnergyMax;
	dump_file << "                  | Use kin energy cut (T/F,value in GeV)"<<endl;

	dump_file <<"smearEMLLTimeOfFlight  ";
	if(Gui::fConfigParser->fEMLLSmearTimeOfFlight) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file <<Gui::fConfigParser->fEMLLTimeOfFlightResolution<<"                   | Smear time of flight with resolution (T/F,value in ns)"<<endl;
	
	dump_file <<"useEMLLTrackSpeedCut  ";
	if(Gui::fConfigParser->fUseEMLLTrackSpeedCut) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file << Gui::fConfigParser->fEMLLTrackSpeedMinCut <<"  "<< Gui::fConfigParser->fEMLLTrackSpeedMaxCut <<"                   | Use track speed cut (T/F, value, value in units of c)"<<endl;

	
	dump_file <<"useEMLLScatteringAngleCut  ";
	if(Gui::fConfigParser->fUseEMLLThetaCut) dump_file <<"T  ";
	else dump_file <<"F  ";  
	dump_file <<Gui::fConfigParser->fEMLLThetaMin<<"  "<< Gui::fConfigParser->fEMLLThetaMax <<"            | Apply scattering angle cut (T/F, min, max in degree)"<<endl;

	dump_file <<"useEMLLDisplacementCut  ";
	if(Gui::fConfigParser->fUseEMLLDisplacementCut) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file <<Gui::fConfigParser->fEMLLDisplacementMin<<"  "<< Gui::fConfigParser->fEMLLDisplacementMax <<"             | Apply displacement cut (T/F, min, max in cm)"<<endl;

	dump_file <<"useTopBottomEvents  ";
	if(Gui::fConfigParser->fUseTopBottomEvents) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file <<"                       | Use events entering the container from top-bottom sides (T/F)"<<endl;

	dump_file <<"useSideEvents  ";
	if(Gui::fConfigParser->fUseSideEvents) dump_file <<"T  ";                           
	else dump_file <<"F  ";
	dump_file <<"    | Use events entering the container from side (T/F)"<<endl;    
	
	dump_file <<"##############################"<<endl;
	dump_file <<"### EM-ML ALGORITHM CONFIG    "<<endl;
	dump_file <<"##############################"<<endl;
	dump_file <<"nIteration  "<<Gui::fConfigParser->fNIterations<<"                           | No. iterations"<<endl;
	dump_file <<"useTrueMomentum  ";
	if(Gui::fConfigParser->fUseEMLLTrueMomentum) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file <<"                          | Use true momentum (T/F)"<<endl;
	dump_file <<"referenceMomentum  "<<Gui::fConfigParser->fReferenceMomentum<<"               | Nominal momentum in GeV"<<endl;
	dump_file <<"averageMomentum  "<<Gui::fConfigParser->fAverageMomentum<<"                           | Average momentum in GeV"<<endl;
	dump_file <<"initScatteringDensity   "<<Gui::fConfigParser->fInitialScatteringDensity<<"      | Initial scattering density (mrad^2/cm, p0=3 GeV) Air=0.000822582  Al=2.81015  Fe=14.2248  Concrete=2.164  Tin=20.7233  U=78.9565  Pl=83.6386"<<endl;

	dump_file <<"useDetectorCovariance  ";
	if(Gui::fConfigParser->fUseDetectorCovariance) dump_file<<"T  ";
	else dump_file<<"F  ";    
	dump_file <<Gui::fConfigParser->fEx<<"  "<< Gui::fConfigParser->fEy <<"  "<< Gui::fConfigParser->fExy<<"   | Use detector covariance matrix (T/F, Ex in rad^2,Ey in cm^2,Exy in rad*cm)"<<endl;

	dump_file <<"useRegularization  ";
	if(Gui::fConfigParser->fUseRegularization) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file <<Gui::fConfigParser->fBeta<<"                  | Use regularization (T/F, value reg param) "<<endl;

	dump_file <<"useResErrStopping  ";
	if(Gui::fConfigParser->fUseResErrStopping) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file <<Gui::fConfigParser->fResErrTolerance<<"  "<<Gui::fConfigParser->fNIterForResErrStopping<<"                | Use residual stopping (T/F,tolerance,no. matching iterations) "<<endl;

	dump_file <<"useLogLikeStopping  ";
	if(Gui::fConfigParser->fUseLogLikeStopping) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file <<Gui::fConfigParser->fLogLikeTolerance<<"  "<<Gui::fConfigParser->fNIterForLogLikeStopping<<"               | Use LogLikelihood stopping (T/F,tolerance,no. matching iterations)  "<<endl;

	dump_file <<"usePOCAInLL  ";
	if(Gui::fConfigParser->fUsePOCAInLL) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file <<"                             | Use POCA as starting point for the LL (T/F)"<<endl;

	dump_file <<"NMinEventInVoxelForUpdate  "<<Gui::fConfigParser->fNMinEventInVoxelForUpdate<<"                | Min event no. to update pixel value (value)"<<endl;
	dump_file <<"useFilterVoxelEvents  ";
	if(Gui::fConfigParser->fUseFilterVoxelEvents) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file <<"                      | Filter voxel events (T/F)"<<endl;
		
	dump_file <<"useQuantileUpdate  ";
	if(Gui::fConfigParser->fUseEMLLQuantileUpdate) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file <<Gui::fConfigParser->fEMLLQuantileProb<<"                    | Use quantile update rule (T/F, value)"<<endl;

	dump_file <<"useClusteringInfo  ";
	if(Gui::fConfigParser->fEMLLUseClusteringInfo) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file <<Gui::fConfigParser->fEMLLClusteringFileName   <<"    | Use clustering info (T/F, filename)"<<endl;

	dump_file <<"EMLLAlarmThreshold  "<<Gui::fConfigParser->fEMLLNEventThreshold<<"  "<<Gui::fConfigParser->fEMLLSignalThreshold<<"    | Voxel alarm threshold (nev,lgLambda)"<<endl;
	dump_file <<"###"<<endl;
	dump_file <<"###############################"<<endl;

  dump_file.close();

	return true;

}//close DumpEMLLConfigFile()


void EMLLTab::BrowseInputFile(){
	
	QString fileName = QFileDialog::getOpenFileName(this,tr("Find Files"), QDir::currentPath(), tr("*.txt *.dat *.out"));

	if (!fileName.isEmpty()) {
  	if (comboBox_inputFile->findText(fileName) == -1) comboBox_inputFile->addItem(fileName);
    comboBox_inputFile->setCurrentIndex(comboBox_inputFile->findText(fileName));
  }
	
}//close EMLLTab::BrowseInputFile()

void EMLLTab::BrowseOutputFile(){
	
	QString fileName = QFileDialog::getOpenFileName(this,tr("Find Files"), QDir::currentPath(), tr("*.txt *.dat *.out *.root"));

	if (!fileName.isEmpty()) {
  	if (comboBox_outputFile->findText(fileName) == -1) comboBox_outputFile->addItem(fileName);
    comboBox_outputFile->setCurrentIndex(comboBox_outputFile->findText(fileName));
  }
	
}//close EMLLTab::BrowseOutputFile()


void EMLLTab::BrowseClusteringFile(){
	
	QString fileName = QFileDialog::getOpenFileName(this,tr("Find Files"), QDir::currentPath(), tr("*.root"));

	if (!fileName.isEmpty()) {
  	if (comboBox_clusteringFile->findText(fileName) == -1) comboBox_clusteringFile->addItem(fileName);
    comboBox_clusteringFile->setCurrentIndex(comboBox_clusteringFile->findText(fileName));
  }
	
}//close EMLLTab::BrowseClusteringFile()

}//close namespace
