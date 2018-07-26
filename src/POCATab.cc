#include <POCATab.h>
#include <Gui.h>

#include <QtGui>
#include <TString.h>

#include <iostream>

using namespace std;


POCATab::POCATab(QWidget *parent)
     : QWidget(parent)
{
	
	//## Set config file name	
	fPOCAConfigFileName= Gui::GUI_CONFIG_DIR + std::string("/POCAConfig.cfg");
	

	//## Define objects
	groupBox_POCA= new QGroupBox(tr("POCA Algorithm"));
	groupBox_POCA->setObjectName(QString::fromUtf8("groupBox_POCA"));
	groupBox_POCA->setCheckable(true);
	//groupBox_POCA->setVisible(true);
	groupBox_POCA->setStyleSheet("QGroupBox{font-size: 18px;font-weight: bold;} QGroupBox::title { background-color: transparent; subcontrol-position: top left;padding:2 13px;} ");

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
	comboBox_outputFile->addItem("POCAOutput.root");
	comboBox_outputFile->setFixedWidth(250);
	
	pushButton_outputFile = new QPushButton(tr("Browse"));
  pushButton_outputFile->setObjectName(QString::fromUtf8("pushButton_outputFile"));
	pushButton_outputFile->setFixedSize(QSize(80,20));


	checkBox_useQueueRun= new QCheckBox("Use queue system?");
	checkBox_useQueueRun->setObjectName(QString::fromUtf8("checkBox_useQueueRun"));
	checkBox_useQueueRun->setChecked(false);

	QLabel* label_InfoOptionSeparation = new QLabel("Options");
	label_InfoOptionSeparation->setFrameStyle(QFrame::HLine | QFrame::Raised);
	label_InfoOptionSeparation->setStyleSheet("QLabel { color : red; }");

	QLabel* label_POCATolerance= new QLabel();
	label_POCATolerance->setObjectName(QString::fromUtf8("label_POCATolerance"));
	label_POCATolerance->setText("POCA Tolerance");

	spinbox_POCATolerance= new QScienceSpinBox;
	spinbox_POCATolerance->setObjectName(QString::fromUtf8("spinbox_POCATolerance"));
	spinbox_POCATolerance->setRange(0, 1.e-3);
  spinbox_POCATolerance->setValue(1.e-8);
	spinbox_POCATolerance->setSingleStep(1.e-7);
	spinbox_POCATolerance->setDecimals(2);
	spinbox_POCATolerance->setFixedWidth(100);

	QLabel* label_OptionSelectionSeparation = new QLabel("Selection");
	label_OptionSelectionSeparation->setFrameStyle(QFrame::HLine | QFrame::Raised);
	label_OptionSelectionSeparation->setStyleSheet("QLabel { color : green; }");

	
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

	QLabel* label_SelectionImagingSeparation = new QLabel("Analysis & Draw Options");
	label_SelectionImagingSeparation->setFrameStyle(QFrame::HLine | QFrame::Raised);
	label_SelectionImagingSeparation->setStyleSheet("QLabel { color : blue; }");

	
	QLabel* label_alarmThreshold= new QLabel();
	label_alarmThreshold->setObjectName(QString::fromUtf8("label_alarmThreshold"));
	label_alarmThreshold->setText("Alarm Threshold [NEventThr,LambdaThr]");

	spinbox_alarmThreshold= new QDoubleSpinBox;
	spinbox_alarmThreshold->setObjectName(QString::fromUtf8("spinbox_alarmThreshold"));
	spinbox_alarmThreshold->setRange(0, 10000);
  spinbox_alarmThreshold->setValue(100);
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

	QHBoxLayout* POCAToleranceLayout= new QHBoxLayout;
	POCAToleranceLayout->addWidget(label_POCATolerance);
	POCAToleranceLayout->addWidget(spinbox_POCATolerance);
	POCAToleranceLayout->addStretch(1);


	QHBoxLayout* NMaxEventCutLayout= new QHBoxLayout;
	NMaxEventCutLayout->addWidget(checkBox_maxNEvents);
	NMaxEventCutLayout->addWidget(spinbox_maxNEvents);
	NMaxEventCutLayout->addStretch(1);
	
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

	QHBoxLayout* AlarmThresholdLayout= new QHBoxLayout;
	AlarmThresholdLayout->addWidget(label_alarmThreshold);
	AlarmThresholdLayout->addWidget(spinbox_NEventThreshold);
	AlarmThresholdLayout->addWidget(spinbox_alarmThreshold);
	AlarmThresholdLayout->addStretch(1);

	QHBoxLayout* ThetaWeightLayout= new QHBoxLayout;
	ThetaWeightLayout->addWidget(checkBox_useThetaWeight);
	ThetaWeightLayout->addWidget(spinbox_thetaWeight);
	ThetaWeightLayout->addStretch(1);

	QHBoxLayout* DrawStyleLayout= new QHBoxLayout;
	DrawStyleLayout->addWidget(label_drawStyle);
	DrawStyleLayout->addWidget(comboBox_drawStyle);
	DrawStyleLayout->addStretch(1);

	QVBoxLayout* GroupBoxPOCALayout = new QVBoxLayout;
	//GroupBoxPOCALayout->addSpacerItem(new QSpacerItem(0, 5, QSizePolicy::Maximum, QSizePolicy::Maximum));
	//GroupBoxPOCALayout->addWidget(label_InfoSeparation);
	//GroupBoxPOCALayout->addLayout(InputFileLayout);
	//GroupBoxPOCALayout->addLayout(OutputFileLayout);
	GroupBoxPOCALayout->addWidget(checkBox_useQueueRun);
	GroupBoxPOCALayout->addSpacerItem(new QSpacerItem(0, 15, QSizePolicy::Maximum, QSizePolicy::Maximum));
	GroupBoxPOCALayout->addWidget(label_InfoOptionSeparation);
	GroupBoxPOCALayout->addLayout(POCAToleranceLayout);
	
	GroupBoxPOCALayout->addSpacerItem(new QSpacerItem(0, 5, QSizePolicy::Maximum, QSizePolicy::Maximum));
	GroupBoxPOCALayout->addWidget(label_OptionSelectionSeparation);
	GroupBoxPOCALayout->addLayout(NMaxEventCutLayout);
	GroupBoxPOCALayout->addLayout(ThetaCutLayout);
	GroupBoxPOCALayout->addLayout(EnergyCutLayout);
	GroupBoxPOCALayout->addSpacerItem(new QSpacerItem(0, 5, QSizePolicy::Maximum, QSizePolicy::Maximum));
	GroupBoxPOCALayout->addWidget(label_SelectionImagingSeparation);
	GroupBoxPOCALayout->addLayout(ThetaWeightLayout);
	GroupBoxPOCALayout->addWidget(checkBox_useAverageSignal);
	GroupBoxPOCALayout->addLayout(AlarmThresholdLayout);
	GroupBoxPOCALayout->addLayout(DrawStyleLayout);
	GroupBoxPOCALayout->addStretch(1);
	groupBox_POCA->setLayout(GroupBoxPOCALayout);

	//QHBoxLayout* mainLayout = new QHBoxLayout;
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(groupBox_POCA);
	mainLayout->addStretch(1);

	QVBoxLayout* mainVLayout = new QVBoxLayout;
	mainVLayout->addLayout(mainLayout);
	mainVLayout->addStretch(1);
  setLayout(mainVLayout);

}//close constructor


void POCATab::SetPOCAOptions(){

	Gui::fConfigParser->fUsePOCAAlgorithm= groupBox_POCA->isChecked();
	Gui::fConfigParser->fUsePOCAQueueRun= checkBox_useQueueRun->isChecked();

	//Gui::fConfigParser->fPOCAVerbosity= 1;
	//Gui::fConfigParser->fPOCAInputFileName= (comboBox_inputFile->currentText()).toStdString();
	//Gui::fConfigParser->fPOCAOutputFileName= (comboBox_outputFile->currentText()).toStdString();
	Gui::fConfigParser->fPOCATolerance= spinbox_POCATolerance->value();
	Gui::fConfigParser->fUsePOCAMaxNEvents= checkBox_maxNEvents->isChecked();	
	Gui::fConfigParser->fPOCAMaxNEvents= spinbox_maxNEvents->value();	
	Gui::fConfigParser->fUsePOCAThetaCut= checkBox_useScatteringAngleCut->isChecked();	
	Gui::fConfigParser->fPOCAThetaMin= spinbox_minScatteringAngle->value();
	Gui::fConfigParser->fPOCAThetaMax= spinbox_maxScatteringAngle->value();
	Gui::fConfigParser->fUsePOCAThetaWeight= checkBox_useThetaWeight->isChecked();	
	Gui::fConfigParser->fPOCAThetaWeight= spinbox_thetaWeight->value();
	Gui::fConfigParser->fUsePOCAAverageSignal= checkBox_useAverageSignal->isChecked();
	Gui::fConfigParser->fPOCASignalThreshold= spinbox_alarmThreshold->value();
	Gui::fConfigParser->fPOCANEventThreshold= spinbox_NEventThreshold->value();
	Gui::fConfigParser->fPOCADrawStyleName= (comboBox_drawStyle->currentText()).toStdString();
	Gui::fConfigParser->fUsePOCAEnergyCut= checkBox_useEnergyCut->isChecked();
	Gui::fConfigParser->fPOCAEnergyMin= spinbox_minEnergy->value();
	Gui::fConfigParser->fPOCAEnergyMax= spinbox_maxEnergy->value();

	//## Dump config file
	bool status= DumpPOCAConfigFile();
	if(!status){
		QMessageBox::critical(this->window(), "ERROR", "POCATab::SetPOCAOptions(): Cannot dump config options to file...exit!");
		QMetaObject::invokeMethod(this->window(), "close", Qt::QueuedConnection);
	}

}//close POCATab::SetPOCAOptions()


bool POCATab::DumpPOCAConfigFile(){

	//## Write config file
	std::ofstream dump_file;
  dump_file.open(fPOCAConfigFileName.c_str());
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
	dump_file << "verbosityPOCA  "<< Gui::fConfigParser->fPOCAVerbosity <<"     | Verbosity reconstruction"<<endl;
	dump_file << "###"<<endl;
	
	dump_file << "#####################################"<<endl;
	dump_file << "#### DATA SELECTION                  "<<endl;
	dump_file << "#####################################"<<endl;
	dump_file << "usePOCANEventCut  ";
	if(Gui::fConfigParser->fUsePOCAMaxNEvents) dump_file << "T  ";
	else dump_file << "F  ";
	dump_file << Gui::fConfigParser->fPOCAMaxNEvents<<"                      | Max number of events to be reconstructed (value,-1=rec all events)"<<endl;

	dump_file << "smearTrackPoints  ";	
	if(Gui::fConfigParser->fSmearTrackPoints) dump_file << "T  ";
	else dump_file << "F  ";
	dump_file << "    | Smear track points with detector resolution (T/F)"<<endl;

	dump_file << "detectorResolution  "<<Gui::fConfigParser->fSigmaPosX<<"  "<< Gui::fConfigParser->fSigmaPosY<< "  "<<Gui::fConfigParser->fSigmaPosZ<<" 	       	    | XYZ Position sigma resolution in cm (value,value,value)"<<endl;


	dump_file << "usePOCAEnergyCut  ";
	if(Gui::fConfigParser->fUsePOCAEnergyCut) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file << Gui::fConfigParser->fPOCAEnergyMin <<"  "<<Gui::fConfigParser->fPOCAEnergyMax;
	dump_file << "                  | Use kin energy cut (T/F,value in GeV)"<<endl;

	dump_file <<"usePOCAScatteringAngleCut  ";
	if(Gui::fConfigParser->fUsePOCAThetaCut) dump_file <<"T  ";
	else dump_file <<"F  ";  
	dump_file <<Gui::fConfigParser->fPOCAThetaMin<<"  "<< Gui::fConfigParser->fPOCAThetaMax <<"            | Apply scattering angle cut (T/F, min, max in degree)"<<endl;
	dump_file <<"###"<<endl;
	
	dump_file << "#########################################"<<endl;
	dump_file << "#### POCA RECONSTRUCTION CONFIG"<<endl;
	dump_file << "#########################################"<<endl;
	dump_file << "usePOCAThetaWeight  ";
	if(Gui::fConfigParser->fUsePOCAThetaWeight) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file << Gui::fConfigParser->fPOCAThetaWeight;
	dump_file << "                  | Use theta weight (T/F,value)"<<endl;

	dump_file << "usePOCAAverageSignal  ";
	if(Gui::fConfigParser->fUsePOCAAverageSignal) dump_file <<"T  ";
	else dump_file <<"F  ";
	dump_file << "                  | Use average signal (T/F)"<<endl;

	dump_file << "POCAAlarmThreshold  "<<Gui::fConfigParser->fPOCANEventThreshold<<"  "<<Gui::fConfigParser->fPOCASignalThreshold<<"    | POCA alarm threshold (nev,bin weight threshold)"<<endl;
	dump_file <<"###"<<endl;
	dump_file <<"###############################"<<endl;

	dump_file.close();

	return true;

}//close DumpPOCAConfigFile()


void POCATab::BrowseInputFile(){
	
	QString fileName = QFileDialog::getOpenFileName(this,tr("Find Files"), QDir::currentPath(), tr("*.txt *.dat *.out"));

	if (!fileName.isEmpty()) {
  	if (comboBox_inputFile->findText(fileName) == -1) comboBox_inputFile->addItem(fileName);
    comboBox_inputFile->setCurrentIndex(comboBox_inputFile->findText(fileName));
  }
	
}//close POCATab::BrowseInputFile()

void POCATab::BrowseOutputFile(){
	
	QString fileName = QFileDialog::getOpenFileName(this,tr("Find Files"), QDir::currentPath(), tr("*.txt *.dat *.out *.root"));

	if (!fileName.isEmpty()) {
  	if (comboBox_outputFile->findText(fileName) == -1) comboBox_outputFile->addItem(fileName);
    comboBox_outputFile->setCurrentIndex(comboBox_outputFile->findText(fileName));
  }
	
}//close POCATab::BrowseOutputFile()



