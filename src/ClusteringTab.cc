#include <ClusteringTab.h>
#include <Gui.h>

#include <QtGui>
#include <TString.h>

#include <iostream>

using namespace std;


ClusteringTab::ClusteringTab(QWidget *parent)
     : QWidget(parent)
{
	
	//## Set config file name	
	fClusteringConfigFileName= Gui::GUI_CONFIG_DIR + std::string("/FOFConfig.cfg");
	
	//## Define objects
	groupBox_FOFClustering= new QGroupBox(tr("FOF Clustering"));
	groupBox_FOFClustering->setObjectName(QString::fromUtf8("groupBox_FOFClustering"));
	groupBox_FOFClustering->setCheckable(true);
	//groupBox_FOFClustering->setVisible(true);
	groupBox_FOFClustering->setStyleSheet("QGroupBox{font-size: 18px;font-weight: bold;} QGroupBox::title { background-color: transparent; subcontrol-position: top left;padding:2 13px;} ");

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
	comboBox_inputFile->addItem("SampleScenario1.dat");
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
	comboBox_outputFile->addItem("FOFOutput.root");
	comboBox_outputFile->setFixedWidth(250);
	
	pushButton_outputFile= new QPushButton(tr("Browse"));
  pushButton_outputFile->setObjectName(QString::fromUtf8("pushButton_outputFile"));
	pushButton_outputFile->setFixedSize(QSize(80,20));

	checkBox_waitPOCA= new QCheckBox("Wait POCA process?");
	checkBox_waitPOCA->setObjectName(QString::fromUtf8("checkBox_waitPOCA"));
	checkBox_waitPOCA->setChecked(true);	
	
	checkBox_useQueueRun= new QCheckBox("Use queue system?");
	checkBox_useQueueRun->setObjectName(QString::fromUtf8("checkBox_useQueueRun"));
	checkBox_useQueueRun->setChecked(false);

	QLabel* label_Verbosity= new QLabel();
	label_Verbosity->setObjectName(QString::fromUtf8("label_Verbosity"));
	label_Verbosity->setText("Verbosity");

	spinbox_Verbosity= new QSpinBox;
	spinbox_Verbosity->setObjectName(QString::fromUtf8("spinbox_Verbosity"));
	spinbox_Verbosity->setRange(0, 3);
  spinbox_Verbosity->setValue(1);
	spinbox_Verbosity->setSingleStep(1);
	spinbox_Verbosity->setFixedWidth(80);


	QLabel* label_OptionSelectionSeparation = new QLabel("Selection");
	label_OptionSelectionSeparation->setFrameStyle(QFrame::HLine | QFrame::Raised);
	label_OptionSelectionSeparation->setStyleSheet("QLabel { color : green; }");

	QLabel* label_epsilon= new QLabel();
	label_epsilon->setObjectName(QString::fromUtf8("label_epsilon"));
	label_epsilon->setText("Epsilon [cm]");

	spinbox_epsilon= new QDoubleSpinBox;
	spinbox_epsilon->setObjectName(QString::fromUtf8("spinbox_epsilon"));
	spinbox_epsilon->setRange(0, 100);
  spinbox_epsilon->setValue(5);
	spinbox_epsilon->setSingleStep(1);
	spinbox_epsilon->setFixedWidth(80);

	QLabel* label_minNpts= new QLabel();
	label_minNpts->setObjectName(QString::fromUtf8("label_minNpts"));
	label_minNpts->setText("minNpts");
	
	spinbox_minNpts= new QDoubleSpinBox;
	spinbox_minNpts->setObjectName(QString::fromUtf8("spinbox_minNpts"));
	spinbox_minNpts->setRange(0, 1000);
  spinbox_minNpts->setValue(10);
	spinbox_minNpts->setSingleStep(1);
	spinbox_minNpts->setFixedWidth(80);

	checkBox_useScatteringAngleCut= new QCheckBox("Theta Cut [deg]");
	checkBox_useScatteringAngleCut->setObjectName(QString::fromUtf8("checkBox_useScatteringAngleCut"));
	checkBox_useScatteringAngleCut->setChecked(true);
		
	spinbox_minScatteringAngle= new QDoubleSpinBox;
	spinbox_minScatteringAngle->setObjectName(QString::fromUtf8("spinbox_minScatteringAngle"));
	spinbox_minScatteringAngle->setRange(0, 10000);
  spinbox_minScatteringAngle->setValue(2);
	spinbox_minScatteringAngle->setSingleStep(0.5);
	spinbox_minScatteringAngle->setFixedWidth(80);

	spinbox_maxScatteringAngle= new QDoubleSpinBox;
	spinbox_maxScatteringAngle->setObjectName(QString::fromUtf8("spinbox_maxScatteringAngle"));
	spinbox_maxScatteringAngle->setRange(0, 10000);
  spinbox_maxScatteringAngle->setValue(100);
	spinbox_maxScatteringAngle->setSingleStep(0.5);
	spinbox_maxScatteringAngle->setFixedWidth(80);

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

	checkBox_maxNEvents= new QCheckBox("Use max NEvents?");
	checkBox_maxNEvents->setObjectName(QString::fromUtf8("checkBox_maxNEvents"));
	checkBox_maxNEvents->setChecked(true);

	spinbox_maxNEvents= new QSpinBox;
	spinbox_maxNEvents->setObjectName(QString::fromUtf8("spinbox_maxNEvents"));
	spinbox_maxNEvents->setRange(0, 1.e+8);
  spinbox_maxNEvents->setValue(500000);
	spinbox_maxNEvents->setSingleStep(1);
	spinbox_maxNEvents->setFixedWidth(120);

	QLabel* label_InfoOptionSeparation = new QLabel("Options");
	label_InfoOptionSeparation->setFrameStyle(QFrame::HLine | QFrame::Raised);
	label_InfoOptionSeparation->setStyleSheet("QLabel { color : red; }");

	QLabel* label_SelectionImagingSeparation = new QLabel("Analysis & Draw Options");
	label_SelectionImagingSeparation->setFrameStyle(QFrame::HLine | QFrame::Raised);
	label_SelectionImagingSeparation->setStyleSheet("QLabel { color : blue; }");


	QLabel* label_alarmThreshold= new QLabel();
	label_alarmThreshold->setObjectName(QString::fromUtf8("label_alarmThreshold"));
	label_alarmThreshold->setText("Alarm Threshold [NEventThr,LambdaThr]");

	spinbox_alarmThreshold= new QDoubleSpinBox;
	spinbox_alarmThreshold->setObjectName(QString::fromUtf8("spinbox_alarmThreshold"));
	spinbox_alarmThreshold->setRange(0, 100);
  spinbox_alarmThreshold->setValue(0.5);
	spinbox_alarmThreshold->setSingleStep(1);
	spinbox_alarmThreshold->setFixedWidth(100);

	spinbox_NEventThreshold= new QSpinBox;
	spinbox_NEventThreshold->setObjectName(QString::fromUtf8("spinbox_NEventThreshold"));
	spinbox_NEventThreshold->setRange(0, 10000);
  spinbox_NEventThreshold->setValue(0);
	spinbox_NEventThreshold->setSingleStep(1);
	spinbox_NEventThreshold->setFixedWidth(80);
	
	checkBox_useThetaWeight= new QCheckBox("Use theta weight?");
	checkBox_useThetaWeight->setObjectName(QString::fromUtf8("checkBox_useThetaWeight"));
	checkBox_useThetaWeight->setChecked(true);

	spinbox_thetaWeight= new QDoubleSpinBox;
	spinbox_thetaWeight->setObjectName(QString::fromUtf8("spinbox_thetaWeight"));
	spinbox_thetaWeight->setRange(0, 10);
  spinbox_thetaWeight->setValue(2);
	spinbox_thetaWeight->setSingleStep(1);
	spinbox_thetaWeight->setFixedWidth(80);

	checkBox_useAverageSignal= new QCheckBox("Use average signal?");
	checkBox_useAverageSignal->setObjectName(QString::fromUtf8("checkBox_useAverageSignal"));
	checkBox_useAverageSignal->setChecked(false);

	//## Define object actions
	connect(pushButton_inputFile, SIGNAL(clicked()), this, SLOT(BrowseInputFile()) );
	connect(pushButton_outputFile, SIGNAL(clicked()), this, SLOT(BrowseOutputFile()) );

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

	QHBoxLayout* WaitPOCALayout= new QHBoxLayout;
	WaitPOCALayout->addWidget(checkBox_waitPOCA);
	WaitPOCALayout->addStretch(1);

	QHBoxLayout* VerbosityLayout= new QHBoxLayout;
	VerbosityLayout->addWidget(label_Verbosity);
	VerbosityLayout->addWidget(spinbox_Verbosity);
	VerbosityLayout->addStretch(1);


	QHBoxLayout* EpsilonLayout= new QHBoxLayout;
	EpsilonLayout->addWidget(label_epsilon);
	EpsilonLayout->addWidget(spinbox_epsilon);
	EpsilonLayout->addStretch(1);

	QHBoxLayout* MinNPtsLayout= new QHBoxLayout;
	MinNPtsLayout->addWidget(label_minNpts);
	MinNPtsLayout->addWidget(spinbox_minNpts);
	MinNPtsLayout->addStretch(1);
	
	QHBoxLayout* ThetaCutLayout= new QHBoxLayout;
	ThetaCutLayout->addWidget(checkBox_useScatteringAngleCut);
	ThetaCutLayout->addWidget(spinbox_minScatteringAngle);
	ThetaCutLayout->addWidget(spinbox_maxScatteringAngle);
	ThetaCutLayout->addStretch(1);

	QHBoxLayout* EnergyCutLayout= new QHBoxLayout;	
	EnergyCutLayout->addWidget(checkBox_useEnergyCut);
	EnergyCutLayout->addWidget(spinbox_minEnergy);
	EnergyCutLayout->addWidget(spinbox_maxEnergy);
	EnergyCutLayout->addStretch(1);

	QHBoxLayout* NMaxEventCutLayout= new QHBoxLayout;
	NMaxEventCutLayout->addWidget(checkBox_maxNEvents);
	NMaxEventCutLayout->addWidget(spinbox_maxNEvents);
	NMaxEventCutLayout->addStretch(1);

	QHBoxLayout* AlarmThresholdLayout= new QHBoxLayout;
	AlarmThresholdLayout->addWidget(label_alarmThreshold);
	AlarmThresholdLayout->addWidget(spinbox_NEventThreshold);
	AlarmThresholdLayout->addWidget(spinbox_alarmThreshold);
	AlarmThresholdLayout->addStretch(1);

	QHBoxLayout* ThetaWeightLayout= new QHBoxLayout;
	ThetaWeightLayout->addWidget(checkBox_useThetaWeight);
	ThetaWeightLayout->addWidget(spinbox_thetaWeight);
	ThetaWeightLayout->addStretch(1);


	QVBoxLayout* GroupBoxClusteringLayout = new QVBoxLayout;
	//GroupBoxClusteringLayout->addSpacerItem(new QSpacerItem(0, 5, QSizePolicy::Maximum, QSizePolicy::Maximum));
	//GroupBoxClusteringLayout->addWidget(label_InfoSeparation);
	//GroupBoxClusteringLayout->addLayout(InputFileLayout);
	//GroupBoxClusteringLayout->addLayout(OutputFileLayout);
	//GroupBoxClusteringLayout->addLayout(WaitPOCALayout);
	GroupBoxClusteringLayout->addWidget(checkBox_useQueueRun);
	GroupBoxClusteringLayout->addSpacerItem(new QSpacerItem(0, 15, QSizePolicy::Maximum, QSizePolicy::Maximum));
	GroupBoxClusteringLayout->addWidget(label_InfoOptionSeparation);
	GroupBoxClusteringLayout->addLayout(VerbosityLayout);
	GroupBoxClusteringLayout->addLayout(EpsilonLayout);
	GroupBoxClusteringLayout->addLayout(MinNPtsLayout);
	GroupBoxClusteringLayout->addSpacerItem(new QSpacerItem(0, 5, QSizePolicy::Maximum, QSizePolicy::Maximum));
	GroupBoxClusteringLayout->addWidget(label_OptionSelectionSeparation);
	GroupBoxClusteringLayout->addLayout(NMaxEventCutLayout);
	GroupBoxClusteringLayout->addLayout(ThetaCutLayout);	
	GroupBoxClusteringLayout->addLayout(EnergyCutLayout);
	GroupBoxClusteringLayout->addSpacerItem(new QSpacerItem(0, 5, QSizePolicy::Maximum, QSizePolicy::Maximum));
	GroupBoxClusteringLayout->addWidget(label_SelectionImagingSeparation);
	GroupBoxClusteringLayout->addLayout(ThetaWeightLayout);
	GroupBoxClusteringLayout->addWidget(checkBox_useAverageSignal);
	GroupBoxClusteringLayout->addLayout(AlarmThresholdLayout);
	GroupBoxClusteringLayout->addStretch(1);
	groupBox_FOFClustering->setLayout(GroupBoxClusteringLayout);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	//mainLayout->addLayout();
	mainLayout->addWidget(groupBox_FOFClustering);
	mainLayout->addStretch(1);

	QVBoxLayout* mainVLayout = new QVBoxLayout;
	mainVLayout->addLayout(mainLayout);
	mainVLayout->addStretch(1);
  setLayout(mainVLayout);

}//close constructor

void ClusteringTab::SetClusteringOptions(){

	Gui::fConfigParser->fUseClusteringAlgorithm= groupBox_FOFClustering->isChecked();
	Gui::fConfigParser->fClusteringVerbosity= spinbox_Verbosity->value();
	//Gui::fConfigParser->fClusteringWaitPOCAOutput= checkBox_waitPOCA->isChecked();
	//Gui::fConfigParser->fClusteringInputFileName= (comboBox_inputFile->currentText()).toStdString();
	//Gui::fConfigParser->fClusteringOutputFileName= (comboBox_outputFile->currentText()).toStdString();

	Gui::fConfigParser->fUseClusteringQueueRun= checkBox_useQueueRun->isChecked();

	Gui::fConfigParser->fUseClusteringMaxNEvents= checkBox_maxNEvents->isChecked();
	Gui::fConfigParser->fClusteringMaxNEvents= spinbox_maxNEvents->value();
	Gui::fConfigParser->fUseClusteringThetaCut= checkBox_useScatteringAngleCut->isChecked();
	Gui::fConfigParser->fClusteringThetaMin= spinbox_minScatteringAngle->value();
	Gui::fConfigParser->fClusteringThetaMax= spinbox_maxScatteringAngle->value();
	Gui::fConfigParser->fUseClusteringThetaWeight= checkBox_useThetaWeight->isChecked();
	Gui::fConfigParser->fClusteringThetaWeight= spinbox_thetaWeight->value();
	Gui::fConfigParser->fUseClusteringAverageSignal= checkBox_useAverageSignal->isChecked();
	Gui::fConfigParser->fClusteringSignalThreshold= spinbox_alarmThreshold->value();
	Gui::fConfigParser->fClusteringNEventThreshold= spinbox_NEventThreshold->value();
	Gui::fConfigParser->fClusteringEpsilon= spinbox_epsilon->value();
	Gui::fConfigParser->fClusteringMinNPts= spinbox_minNpts->value();
	Gui::fConfigParser->fUseClusteringEnergyCut= checkBox_useEnergyCut->isChecked();
	Gui::fConfigParser->fClusteringEnergyMin= spinbox_minEnergy->value();
	Gui::fConfigParser->fClusteringEnergyMax= spinbox_maxEnergy->value();

	//## Dump config file
	bool status= DumpClusteringConfigFile();
	if(!status){
		QMessageBox::critical(this->window(), "ERROR", "ClusteringTab::SetClusteringOptions(): Cannot dump config options to file...exit!");
		QMetaObject::invokeMethod(this->window(), "close", Qt::QueuedConnection);
	}

}//close ClusteringTab::SetClusteringOptions()


bool ClusteringTab::DumpClusteringConfigFile(){

	//## Write config file
	std::ofstream dump_file;
  dump_file.open(fClusteringConfigFileName.c_str());
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
	dump_file << "verbosityClustering  "<< Gui::fConfigParser->fClusteringVerbosity <<"     | Verbosity reconstruction"<<endl;
	dump_file << "###"<<endl;
	
	dump_file << "#####################################"<<endl;
	dump_file << "#### DATA SELECTION                  "<<endl;
	dump_file << "#####################################"<<endl;
	dump_file << "useClusteringNEventCut  ";
	if(Gui::fConfigParser->fUseClusteringMaxNEvents) dump_file << "T  ";
	else dump_file << "F  ";
	dump_file << Gui::fConfigParser->fClusteringMaxNEvents<<"                      | Max number of events to be reconstructed (value,-1=rec all events)"<<endl;

	dump_file << "smearTrackPoints  ";	
	if(Gui::fConfigParser->fSmearTrackPoints) dump_file << "T  ";
	else dump_file << "F  ";
	dump_file << "    | Smear track points with detector resolution (T/F)"<<endl;

	dump_file << "detectorResolution  "<<Gui::fConfigParser->fSigmaPosX<<"  "<< Gui::fConfigParser->fSigmaPosY<< "  "<<Gui::fConfigParser->fSigmaPosZ<<" 	       	    | XYZ Position sigma resolution in cm (value,value,value)"<<endl;


	dump_file << "useClusteringEnergyCut  ";
	if(Gui::fConfigParser->fUseClusteringEnergyCut) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file << Gui::fConfigParser->fClusteringEnergyMin <<"  "<<Gui::fConfigParser->fClusteringEnergyMax;
	dump_file << "                  | Use kin energy cut (T/F,value in GeV)"<<endl;

	dump_file <<"useClusteringScatteringAngleCut  ";
	if(Gui::fConfigParser->fUseClusteringThetaCut) dump_file <<"T  ";
	else dump_file <<"F  ";  
	dump_file <<Gui::fConfigParser->fClusteringThetaMin<<"  "<< Gui::fConfigParser->fClusteringThetaMax <<"            | Apply scattering angle cut (T/F, min, max in degree)"<<endl;
	dump_file <<"###"<<endl;
	
	dump_file << "#########################################"<<endl;
	dump_file << "#### CLUSTERING RECONSTRUCTION CONFIG"<<endl;
	dump_file << "#########################################"<<endl;
	dump_file << "useClusteringThetaWeight  ";
	if(Gui::fConfigParser->fUseClusteringThetaWeight) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file << Gui::fConfigParser->fClusteringThetaWeight;
	dump_file << "                  | Use theta weight (T/F,value)"<<endl;

	dump_file << "useClusteringAverageSignal  ";
	if(Gui::fConfigParser->fUseClusteringAverageSignal) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file << "                  | Use average signal (T/F)"<<endl;

	dump_file << "ClusteringEpsilon  "<< Gui::fConfigParser->fClusteringEpsilon <<"     | Threshold for clustering in cm (value)"<<endl;
	dump_file << "ClusteringMinNPts  "<< Gui::fConfigParser->fClusteringMinNPts <<"     | Min number of points for clustering (value)"<<endl;
	dump_file << "ClusteringAlarmThreshold  "<<Gui::fConfigParser->fClusteringNEventThreshold<<"  "<<Gui::fConfigParser->fClusteringSignalThreshold<<"    | Clustering alarm threshold (nev,cluster weight threshold)"<<endl;
	dump_file <<"###"<<endl;
	dump_file <<"###############################"<<endl;

	dump_file.close();

	return true;

}//close ClusteringTab::DumpClusteringConfigFile()


void ClusteringTab::BrowseInputFile(){
	
	QString fileName = QFileDialog::getOpenFileName(this,tr("Find Files"), QDir::currentPath(), tr("*.txt *.dat *.out"));

	if (!fileName.isEmpty()) {
  	if (comboBox_inputFile->findText(fileName) == -1) comboBox_inputFile->addItem(fileName);
    comboBox_inputFile->setCurrentIndex(comboBox_inputFile->findText(fileName));
  }
	
}//close ClusteringTab::BrowseInputFile()


void ClusteringTab::BrowseOutputFile(){
	
	QString fileName = QFileDialog::getOpenFileName(this,tr("Find Files"), QDir::currentPath(), tr("*.txt *.dat *.out *.root"));

	if (!fileName.isEmpty()) {
  	if (comboBox_outputFile->findText(fileName) == -1) comboBox_outputFile->addItem(fileName);
    comboBox_outputFile->setCurrentIndex(comboBox_outputFile->findText(fileName));
  }
	
}//close ClusteringTab::BrowseOutputFile()

