#include <ACFTab.h>
#include <Gui.h>

#include <QtGui>
#include <TString.h>

#include <iostream>

using namespace std;


ACFTab::ACFTab(QWidget *parent)
     : QWidget(parent)
{
	
	fACFConfigFileName= Gui::GUI_CONFIG_DIR + std::string("/ACFConfig.cfg");
	
	//## Define objects
	groupBox_ACF= new QGroupBox(tr("2pt-ACF"));
	groupBox_ACF->setObjectName(QString::fromUtf8("groupBox_ACF"));
	groupBox_ACF->setCheckable(true);
	//groupBox_ACF->setVisible(true);
	groupBox_ACF->setStyleSheet("QGroupBox{font-size: 18px;font-weight: bold;} QGroupBox::title { background-color: transparent; subcontrol-position: top left;padding:2 13px;} ");

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


	QLabel* label_inputRandomFile= new QLabel();
	label_inputRandomFile->setObjectName(QString::fromUtf8("label_inputRandomFile"));
	label_inputRandomFile->setText("Random File");

	comboBox_inputRandomFile= new QComboBox;
	comboBox_inputRandomFile->setObjectName(QString::fromUtf8("comboBox_inputFile"));
	comboBox_inputRandomFile->setEditable(true);
	comboBox_inputRandomFile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	comboBox_inputRandomFile->addItem("data/POCATomRec_simdata_EmptyContainerScenario_last.root");
	comboBox_inputRandomFile->setFixedWidth(450);
	
	pushButton_inputRandomFile = new QPushButton(tr("Browse"));
  pushButton_inputRandomFile->setObjectName(QString::fromUtf8("pushButton_inputRandomFile"));
	pushButton_inputRandomFile->setFixedSize(QSize(80,20));

	QLabel* label_outputFile= new QLabel();
	label_outputFile->setObjectName(QString::fromUtf8("label_outputFile"));
	label_outputFile->setText("Output File");

	comboBox_outputFile= new QComboBox;
	comboBox_outputFile->setObjectName(QString::fromUtf8("comboBox_outputFile"));
	comboBox_outputFile->setEditable(true);
	comboBox_outputFile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	comboBox_outputFile->addItem("ACFOutput.root");
	comboBox_outputFile->setFixedWidth(250);
	
	pushButton_outputFile = new QPushButton(tr("Browse"));
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


	QLabel* label_VoxelSize= new QLabel();
	label_VoxelSize->setObjectName(QString::fromUtf8("label_VoxelSize"));
	label_VoxelSize->setText("Voxel XYZ Size [cm]");

	spinbox_VoxelSizeX= new QDoubleSpinBox;
	spinbox_VoxelSizeX->setObjectName(QString::fromUtf8("spinbox_VoxelSizeX"));
	spinbox_VoxelSizeX->setRange(0., 100.);
  spinbox_VoxelSizeX->setValue(20.0);
	spinbox_VoxelSizeX->setSingleStep(1);	
	spinbox_VoxelSizeX->setDecimals(0);
	spinbox_VoxelSizeX->setFixedWidth(80);

	spinbox_VoxelSizeY= new QDoubleSpinBox;
	spinbox_VoxelSizeY->setObjectName(QString::fromUtf8("spinbox_VoxelSizeY"));
	spinbox_VoxelSizeY->setRange(0., 100.);
  spinbox_VoxelSizeY->setValue(20.);
	spinbox_VoxelSizeY->setSingleStep(1);
	spinbox_VoxelSizeY->setDecimals(0);
	spinbox_VoxelSizeY->setFixedWidth(80);	

	spinbox_VoxelSizeZ= new QDoubleSpinBox;
	spinbox_VoxelSizeZ->setObjectName(QString::fromUtf8("spinbox_VoxelSizeZ"));
	spinbox_VoxelSizeZ->setRange(0., 100.);
  spinbox_VoxelSizeZ->setValue(20.);
	spinbox_VoxelSizeZ->setSingleStep(1);
	spinbox_VoxelSizeZ->setDecimals(0);
	spinbox_VoxelSizeZ->setFixedWidth(80);



	QLabel* label_NCrown= new QLabel();
	label_NCrown->setObjectName(QString::fromUtf8("label_NCrown"));
	label_NCrown->setText("Max NCrown");

	spinbox_NCrownX= new QSpinBox;
	spinbox_NCrownX->setObjectName(QString::fromUtf8("spinbox_NCrownX"));
	spinbox_NCrownX->setRange(0, 10);
  spinbox_NCrownX->setValue(2);
	spinbox_NCrownX->setSingleStep(1);	
	spinbox_NCrownX->setFixedWidth(80);

	spinbox_NCrownY= new QSpinBox;
	spinbox_NCrownY->setObjectName(QString::fromUtf8("spinbox_NCrownY"));
	spinbox_NCrownY->setRange(0, 10);
  spinbox_NCrownY->setValue(2);
	spinbox_NCrownY->setSingleStep(1);
	spinbox_NCrownY->setFixedWidth(80);	

	spinbox_NCrownZ= new QSpinBox;
	spinbox_NCrownZ->setObjectName(QString::fromUtf8("spinbox_NCrownZ"));
	spinbox_NCrownZ->setRange(0, 10);
  spinbox_NCrownZ->setValue(2);
	spinbox_NCrownZ->setSingleStep(1);
	spinbox_NCrownZ->setFixedWidth(80);

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
	
	checkBox_useThetaWeight= new QCheckBox("Use theta weight?");
	checkBox_useThetaWeight->setObjectName(QString::fromUtf8("checkBox_useThetaWeight"));
	checkBox_useThetaWeight->setChecked(true);

	spinbox_thetaWeight= new QDoubleSpinBox;
	spinbox_thetaWeight->setObjectName(QString::fromUtf8("spinbox_thetaWeight"));
	spinbox_thetaWeight->setRange(0, 10);
  spinbox_thetaWeight->setValue(2);
	spinbox_thetaWeight->setSingleStep(1);
	spinbox_thetaWeight->setFixedWidth(80);

	//## Define object actions
	connect(pushButton_inputFile, SIGNAL(clicked()), this, SLOT(BrowseInputFile()) );
	connect(pushButton_inputRandomFile, SIGNAL(clicked()), this, SLOT(BrowseInputRandomFile()) );
	connect(pushButton_outputFile, SIGNAL(clicked()), this, SLOT(BrowseOutputFile()) );


	//## Define object layout
	QHBoxLayout* InputFileLayout= new QHBoxLayout;
	InputFileLayout->addWidget(label_inputFile);
	InputFileLayout->addWidget(comboBox_inputFile);
	InputFileLayout->addWidget(pushButton_inputFile);
	InputFileLayout->addStretch(1);

	QHBoxLayout* InputRandomFileLayout= new QHBoxLayout;
	InputRandomFileLayout->addWidget(label_inputRandomFile);
	InputRandomFileLayout->addWidget(comboBox_inputRandomFile);
	InputRandomFileLayout->addWidget(pushButton_inputRandomFile);
	InputRandomFileLayout->addStretch(1);

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

	QHBoxLayout* VoxelSizeLayout= new QHBoxLayout;
	VoxelSizeLayout->addWidget(label_VoxelSize);
	VoxelSizeLayout->addWidget(spinbox_VoxelSizeX);
	VoxelSizeLayout->addWidget(spinbox_VoxelSizeY);
	VoxelSizeLayout->addWidget(spinbox_VoxelSizeZ);
	VoxelSizeLayout->addStretch(1);

	QHBoxLayout* NCrownLayout= new QHBoxLayout;
	NCrownLayout->addWidget(label_NCrown);
	NCrownLayout->addWidget(spinbox_NCrownX);
	NCrownLayout->addWidget(spinbox_NCrownY);
	NCrownLayout->addWidget(spinbox_NCrownZ);
	NCrownLayout->addStretch(1);

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
	AlarmThresholdLayout->addWidget(spinbox_alarmThreshold);
	AlarmThresholdLayout->addStretch(1);

	QHBoxLayout* ThetaWeightLayout= new QHBoxLayout;
	ThetaWeightLayout->addWidget(checkBox_useThetaWeight);
	ThetaWeightLayout->addWidget(spinbox_thetaWeight);
	ThetaWeightLayout->addStretch(1);

	QVBoxLayout* GroupBoxACFLayout = new QVBoxLayout;
	GroupBoxACFLayout->addSpacerItem(new QSpacerItem(0, 5, QSizePolicy::Maximum, QSizePolicy::Maximum));
	GroupBoxACFLayout->addWidget(label_InfoSeparation);
	//GroupBoxACFLayout->addLayout(InputFileLayout);
	GroupBoxACFLayout->addWidget(checkBox_useQueueRun);
	//GroupBoxACFLayout->addLayout(OutputFileLayout);
	//GroupBoxACFLayout->addLayout(WaitPOCALayout);
	GroupBoxACFLayout->addLayout(InputRandomFileLayout);
	GroupBoxACFLayout->addSpacerItem(new QSpacerItem(0, 15, QSizePolicy::Maximum, QSizePolicy::Maximum));
	GroupBoxACFLayout->addWidget(label_InfoOptionSeparation);	
	GroupBoxACFLayout->addLayout(VerbosityLayout);
	GroupBoxACFLayout->addLayout(VoxelSizeLayout);
	GroupBoxACFLayout->addLayout(NCrownLayout);
	GroupBoxACFLayout->addSpacerItem(new QSpacerItem(0, 5, QSizePolicy::Maximum, QSizePolicy::Maximum));
	GroupBoxACFLayout->addWidget(label_OptionSelectionSeparation);
	GroupBoxACFLayout->addLayout(NMaxEventCutLayout);
	GroupBoxACFLayout->addLayout(ThetaCutLayout);
	GroupBoxACFLayout->addLayout(EnergyCutLayout);
	GroupBoxACFLayout->addSpacerItem(new QSpacerItem(0, 5, QSizePolicy::Maximum, QSizePolicy::Maximum));
	GroupBoxACFLayout->addWidget(label_SelectionImagingSeparation);
	GroupBoxACFLayout->addLayout(ThetaWeightLayout);
	GroupBoxACFLayout->addLayout(AlarmThresholdLayout);
	GroupBoxACFLayout->addStretch(1);
	groupBox_ACF->setLayout(GroupBoxACFLayout);

	//QHBoxLayout* mainLayout = new QHBoxLayout;
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(groupBox_ACF);
	mainLayout->addStretch(1);
	
	QVBoxLayout* mainVLayout = new QVBoxLayout;
	mainVLayout->addLayout(mainLayout);
	mainVLayout->addStretch(1);
  setLayout(mainVLayout);

}//close constructor

void ACFTab::SetACFOptions(){

	Gui::fConfigParser->fUseACFAlgorithm= groupBox_ACF->isChecked();
	Gui::fConfigParser->fACFVerbosity= spinbox_Verbosity->value();
	//Gui::fConfigParser->fACFInputFileName= (comboBox_inputFile->currentText()).toStdString();
	Gui::fConfigParser->fACFInputRandomFileName= (comboBox_inputRandomFile->currentText()).toStdString();
	//Gui::fConfigParser->fACFOutputFileName= (comboBox_outputFile->currentText()).toStdString();
	Gui::fConfigParser->fUseACFMaxNEvents= checkBox_maxNEvents->isChecked();

	Gui::fConfigParser->fUseACFQueueRun= checkBox_useQueueRun->isChecked();

	Gui::fConfigParser->fACFMaxNEvents= spinbox_maxNEvents->value();
	Gui::fConfigParser->fUseACFThetaCut= checkBox_useScatteringAngleCut->isChecked();
	Gui::fConfigParser->fACFThetaMin= spinbox_minScatteringAngle->value();
	Gui::fConfigParser->fACFThetaMax= spinbox_maxScatteringAngle->value();
	Gui::fConfigParser->fUseACFThetaWeight= checkBox_useThetaWeight->isChecked();
	Gui::fConfigParser->fACFThetaWeight= spinbox_thetaWeight->value();
	Gui::fConfigParser->fACFSignalThreshold= spinbox_alarmThreshold->value(); 
	Gui::fConfigParser->fACFVoxelSizeX= spinbox_VoxelSizeX->value();
	Gui::fConfigParser->fACFVoxelSizeY= spinbox_VoxelSizeY->value();
	Gui::fConfigParser->fACFVoxelSizeZ= spinbox_VoxelSizeZ->value();
	Gui::fConfigParser->fACFNCrownX= spinbox_NCrownX->value();
	Gui::fConfigParser->fACFNCrownY= spinbox_NCrownY->value();
	Gui::fConfigParser->fACFNCrownZ= spinbox_NCrownZ->value();
		
	Gui::fConfigParser->fACFWaitPOCAOutput= checkBox_waitPOCA->isChecked();
	Gui::fConfigParser->fUseACFEnergyCut= checkBox_useEnergyCut->isChecked();
	Gui::fConfigParser->fACFEnergyMin= spinbox_minEnergy->value();
	Gui::fConfigParser->fACFEnergyMax= spinbox_maxEnergy->value();
		
	//## Dump config file
	bool status= DumpACFConfigFile();
	if(!status){
		QMessageBox::critical(this->window(), "ERROR", "ACFTab::SetACFOptions(): Cannot dump config options to file...exit!");
		QMetaObject::invokeMethod(this->window(), "close", Qt::QueuedConnection);
	}

}//close ACFTab::SetACFOptions()


bool ACFTab::DumpACFConfigFile(){
	
	//## Write config file
	std::ofstream dump_file;
  dump_file.open(fACFConfigFileName.c_str());
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
	dump_file << "verbosityACF  "<< Gui::fConfigParser->fACFVerbosity <<"     | Verbosity reconstruction"<<endl;
	dump_file << "randomDataFileACF  "<< Gui::fConfigParser->fACFInputRandomFileName <<"     | Random data filename"<<endl;
	dump_file << "###"<<endl;
	
	dump_file << "#####################################"<<endl;
	dump_file << "#### DATA SELECTION                  "<<endl;
	dump_file << "#####################################"<<endl;
	dump_file << "useACFNEventCut  ";
	if(Gui::fConfigParser->fUseACFMaxNEvents) dump_file << "T  ";
	else dump_file << "F  ";
	dump_file << Gui::fConfigParser->fACFMaxNEvents<<"                      | Max number of events to be reconstructed (value,-1=rec all events)"<<endl;

	dump_file << "smearTrackPoints  ";	
	if(Gui::fConfigParser->fSmearTrackPoints) dump_file << "T  ";
	else dump_file << "F  ";
	dump_file << "    | Smear track points with detector resolution (T/F)"<<endl;

	dump_file << "detectorResolution  "<<Gui::fConfigParser->fSigmaPosX<<"  "<< Gui::fConfigParser->fSigmaPosY<< "  "<<Gui::fConfigParser->fSigmaPosZ<<" 	       	    | XYZ Position sigma resolution in cm (value,value,value)"<<endl;


	dump_file << "useACFEnergyCut  ";
	if(Gui::fConfigParser->fUseACFEnergyCut) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file << Gui::fConfigParser->fACFEnergyMin <<"  "<<Gui::fConfigParser->fACFEnergyMax;
	dump_file << "                  | Use kin energy cut (T/F,value in GeV)"<<endl;

	dump_file <<"useACFScatteringAngleCut  ";
	if(Gui::fConfigParser->fUseACFThetaCut) dump_file <<"T  ";
	else dump_file <<"F  ";  
	dump_file <<Gui::fConfigParser->fACFThetaMin<<"  "<< Gui::fConfigParser->fACFThetaMax <<"            | Apply scattering angle cut (T/F, min, max in degree)"<<endl;
	dump_file <<"###"<<endl;
	
	dump_file << "#########################################"<<endl;
	dump_file << "#### ACF RECONSTRUCTION CONFIG"<<endl;
	dump_file << "#########################################"<<endl;
	dump_file << "useACFThetaWeight  ";
	if(Gui::fConfigParser->fUseACFThetaWeight) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file << Gui::fConfigParser->fACFThetaWeight;
	dump_file << "                  | Use theta weight (T/F,value)"<<endl;

	dump_file << "ACFVoxelSize  "<<Gui::fConfigParser->fACFVoxelSizeX<<"  "<<Gui::fConfigParser->fACFVoxelSizeY<<"   "<<Gui::fConfigParser->fACFVoxelSizeZ<<" | ACF voxel size XYZ in cm"<<endl;
	dump_file << "ACFNCrown  "<<Gui::fConfigParser->fACFNCrownX<<"  "<<Gui::fConfigParser->fACFNCrownY<<"   "<<Gui::fConfigParser->fACFNCrownZ<<" | ACF ncrown XYZ"<<endl;

	dump_file << "ACFAlarmThreshold  "<<Gui::fConfigParser->fACFSignalThreshold<<"    | ACF alarm threshold (threshold)"<<endl;
	dump_file <<"###"<<endl;
	dump_file <<"###############################"<<endl;

	dump_file.close();

	return true;

}//close ACFTab::DumpACFConfigFile()



void ACFTab::BrowseInputFile(){
	
	QString fileName = QFileDialog::getOpenFileName(this,tr("Find Files"), QDir::currentPath(), tr("*.txt *.dat *.out"));

	if (!fileName.isEmpty()) {
  	if (comboBox_inputFile->findText(fileName) == -1) comboBox_inputFile->addItem(fileName);
    comboBox_inputFile->setCurrentIndex(comboBox_inputFile->findText(fileName));
  }
	
}//close ACFTab::BrowseInputFile()

void ACFTab::BrowseInputRandomFile(){
	
	QString fileName = QFileDialog::getOpenFileName(this,tr("Find Files"), QDir::currentPath(), tr("*.txt *.dat *.out"));

	if (!fileName.isEmpty()) {
  	if (comboBox_inputRandomFile->findText(fileName) == -1) comboBox_inputRandomFile->addItem(fileName);
    comboBox_inputRandomFile->setCurrentIndex(comboBox_inputRandomFile->findText(fileName));
  }
	
}//close ACFTab::BrowseInputRandomFile()


void ACFTab::BrowseOutputFile(){
	
	QString fileName = QFileDialog::getOpenFileName(this,tr("Find Files"), QDir::currentPath(), tr("*.txt *.dat *.out *.root"));

	if (!fileName.isEmpty()) {
  	if (comboBox_outputFile->findText(fileName) == -1) comboBox_outputFile->addItem(fileName);
    comboBox_outputFile->setCurrentIndex(comboBox_outputFile->findText(fileName));
  }
	
}//close ACFTab::BrowseOutputFile()

