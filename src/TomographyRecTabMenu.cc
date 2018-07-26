#include <TomographyRecTabMenu.h>
#include <Gui.h>
#include <Utilities.h>
#include <QtGui>

#include <QUdpSocket>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDataStream>
#include <QHostAddress>
#include <QThread>
#include <QtNetwork>
#include <QScriptEngine>
#include <QScriptValue>
#include <QPropertyAnimation>

#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <qjson/qjson_export.h>
#include <qjson/qobjecthelper.h>
#include <qjson/serializerrunnable.h>
#include <qjson/parserrunnable.h>

#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>


using namespace std;
using namespace MuonPortalNS;

TomographyRecTabMenu::TomographyRecTabMenu(QWidget *parent){
 	
	fCurrentScanIndex= 0;	 	
	fIsServerRunning= false;

	webServiceUrl= new QUrl("http://strips.to:18800/alarms");
	networkManager = new QNetworkAccessManager(this);
	networkRequest= new QNetworkRequest(*webServiceUrl);
	networkRequest->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


	//## Create objects	
	fTomographyMainTabMenu = new QTabWidget(parent);
  fTomographyMainTabMenu->setObjectName(QString::fromUtf8("tab_TomographyMainTabMenu"));
  


	
	std::string initButton_imagepath= Gui::GUI_BASE_DIR + std::string("/share/initButton.png");
	std::string runButton_imagepath= Gui::GUI_BASE_DIR + std::string("/share/redButton.png");
	std::string stopButton_imagepath= Gui::GUI_BASE_DIR + std::string("/share/stopButtonImage.png");
	std::string viewButton_imagepath= Gui::GUI_BASE_DIR + std::string("/share/viewButton.png");
	std::string redBlinkingButton_imagepath= Gui::GUI_BASE_DIR + std::string("/share/redLight.gif");
	std::string redBlinkingStaticButton_imagepath= Gui::GUI_BASE_DIR + std::string("/share/redLight_staticRed.gif");

	std::string greenBlinkingButton_imagepath= Gui::GUI_BASE_DIR + std::string("/share/greenLight.gif");
	std::string alarmButton_imagepath= Gui::GUI_BASE_DIR + std::string("/share/alarmLight.gif");
	std::string alarmText_imagepath= Gui::GUI_BASE_DIR + std::string("/share/alarmText.gif");
	std::string alarm_soundpath= Gui::GUI_BASE_DIR + std::string("/share/alarmSound.wav");
	
	
	//mMediaObject = new Phonon::MediaObject(this);
	//mMediaObject->setCurrentSource(Phonon::MediaSource(alarm_soundpath.c_str()));
	
	//mAudioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	//Phonon::createPath(mMediaObject, mAudioOutput);
	

	checkBox_runMode= new QCheckBox("Manual mode?");
	checkBox_runMode->setObjectName(QString::fromUtf8("checkBox_runMode"));
	checkBox_runMode->setChecked(true);

	checkBox_batchMode= new QCheckBox("Batch run?");
	checkBox_batchMode->setObjectName(QString::fromUtf8("checkBox_batchMode"));
	checkBox_batchMode->setChecked(false);

	QLabel* label_runExecMode= new QLabel();		
	label_runExecMode->setObjectName(QString::fromUtf8("label_runExecMode"));
	label_runExecMode->setText("Exec mode");
	

	comboBox_runExecMode= new QComboBox;
	comboBox_runExecMode->setObjectName(QString::fromUtf8("comboBox_runExecMode"));
	comboBox_runExecMode->setEditable(false);
	comboBox_runExecMode->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	comboBox_runExecMode->addItem("local-interactive");
	comboBox_runExecMode->addItem("local-batch");
	comboBox_runExecMode->addItem("remote-batch");
	comboBox_runExecMode->setFixedWidth(180);

	pushButton_Run = new QPushButton(tr("Run"));
  pushButton_Run->setObjectName(QString::fromUtf8("pushButton_Run"));
	pushButton_Run->setIcon(QIcon(runButton_imagepath.c_str()));
	pushButton_Run->setFixedSize(QSize(60,30));
	pushButton_Run->setEnabled(false);


	//## Status group
	
	for(int i=0;i<MAXNSCANS;i++){

		QString objTitle = QString("Scan %1").arg(i+1);
		QString objName = QString("groupBoxList_display%1").arg(i+1);
		groupBoxList_display[i]= new QGroupBox(objTitle);
		groupBoxList_display[i]->setObjectName(objName);
		groupBoxList_display[i]->setCheckable(false);
		groupBoxList_display[i]->setStyleSheet("QGroupBox{font-size: 14px;font-weight: bold;} QGroupBox::title { background-color: transparent; subcontrol-position: top left;padding:2 13px;} ");
		
		movieList_fileStatus_Red[i] = new QMovie(redBlinkingButton_imagepath.c_str());
		movieList_fileStatus_Green[i] = new QMovie(greenBlinkingButton_imagepath.c_str());
		movieList_LLRecoStatus_Red[i] = new QMovie(redBlinkingButton_imagepath.c_str());
		movieList_LLRecoStatus_Green[i] = new QMovie(greenBlinkingButton_imagepath.c_str());
		movieList_POCARecoStatus_Red[i] = new QMovie(redBlinkingButton_imagepath.c_str());
		movieList_POCARecoStatus_Green[i] = new QMovie(greenBlinkingButton_imagepath.c_str());
		movieList_ACFRecoStatus_Red[i] = new QMovie(redBlinkingButton_imagepath.c_str());
		movieList_ACFRecoStatus_Green[i] = new QMovie(greenBlinkingButton_imagepath.c_str());
		movieList_ClusteringRecoStatus_Red[i] = new QMovie(redBlinkingButton_imagepath.c_str());
		movieList_ClusteringRecoStatus_Green[i] = new QMovie(greenBlinkingButton_imagepath.c_str());
		movieList_Alarm_POCAReco[i] = new QMovie(alarmButton_imagepath.c_str());
		movieList_AlarmText_POCAReco[i] = new QMovie(alarmText_imagepath.c_str());
		movieList_Alarm_EMTomographyReco[i] = new QMovie(alarmButton_imagepath.c_str());
		movieList_AlarmText_EMTomographyReco[i] = new QMovie(alarmText_imagepath.c_str());
		movieList_Alarm_ACFReco[i] = new QMovie(alarmButton_imagepath.c_str());
		movieList_AlarmText_ACFReco[i] = new QMovie(alarmText_imagepath.c_str());
		movieList_Alarm_ClusteringReco[i] = new QMovie(alarmButton_imagepath.c_str());
		movieList_AlarmText_ClusteringReco[i] = new QMovie(alarmText_imagepath.c_str());

		movieList_fileStatus_StaticRed[i] = new QMovie(redBlinkingStaticButton_imagepath.c_str());
		movieList_LLRecoStatus_StaticRed[i] = new QMovie(redBlinkingStaticButton_imagepath.c_str());
		movieList_POCARecoStatus_StaticRed[i] = new QMovie(redBlinkingStaticButton_imagepath.c_str());
		movieList_ACFRecoStatus_StaticRed[i] = new QMovie(redBlinkingStaticButton_imagepath.c_str());
		movieList_ClusteringRecoStatus_StaticRed[i] = new QMovie(redBlinkingStaticButton_imagepath.c_str());
		
		objName = QString("imageLabelList_fileStatus%1").arg(i+1);
		imageLabelList_fileStatus[i]= new QLabel();		
		imageLabelList_fileStatus[i]->setObjectName(objName);
		imageLabelList_fileStatus[i]->setMovie(movieList_fileStatus_Red[i]);
		movieList_fileStatus_Red[i]->start();
		movieList_fileStatus_Red[i]->stop();

		objName = QString("imageLabelList_LLRecoStatus%1").arg(i+1);
		imageLabelList_LLRecoStatus[i]= new QLabel();		
		imageLabelList_LLRecoStatus[i]->setObjectName(objName);
		imageLabelList_LLRecoStatus[i]->setMovie(movieList_LLRecoStatus_Red[i]);
		movieList_LLRecoStatus_Red[i]->start();
		movieList_LLRecoStatus_Red[i]->stop();

		objName = QString("imageLabelList_POCARecoStatus%1").arg(i+1);
		imageLabelList_POCARecoStatus[i]= new QLabel();		
		imageLabelList_POCARecoStatus[i]->setObjectName(objName);
		imageLabelList_POCARecoStatus[i]->setMovie(movieList_POCARecoStatus_Red[i]);
		movieList_POCARecoStatus_Red[i]->start();
		movieList_POCARecoStatus_Red[i]->stop();
	
		objName = QString("imageLabelList_ACFRecoStatus%1").arg(i+1);
		imageLabelList_ACFRecoStatus[i]= new QLabel();		
		imageLabelList_ACFRecoStatus[i]->setObjectName(objName);
		imageLabelList_ACFRecoStatus[i]->setMovie(movieList_ACFRecoStatus_Red[i]);
		movieList_ACFRecoStatus_Red[i]->start();
		movieList_ACFRecoStatus_Red[i]->stop();

		objName = QString("imageLabelList_ClusteringRecoStatus%1").arg(i+1);
		imageLabelList_ClusteringRecoStatus[i]= new QLabel();		
		imageLabelList_ClusteringRecoStatus[i]->setObjectName(objName);
		imageLabelList_ClusteringRecoStatus[i]->setMovie(movieList_ClusteringRecoStatus_Red[i]);
		movieList_ClusteringRecoStatus_Red[i]->start();
		movieList_ClusteringRecoStatus_Red[i]->stop();
		
		objName = QString("imageLabelList_Alarm_POCAReco%1").arg(i+1);
		imageLabelList_Alarm_POCAReco[i]= new QLabel();		
		imageLabelList_Alarm_POCAReco[i]->setObjectName(objName);
		imageLabelList_Alarm_POCAReco[i]->setMovie(movieList_Alarm_POCAReco[i]);
		//imageLabelList_Alarm_POCAReco[i]->setVisible(false);
		imageLabelList_Alarm_POCAReco[i]->hide();
		movieList_Alarm_POCAReco[i]->start();
		
		objName = QString("imageLabelList_AlarmText_POCAReco%1").arg(i+1);
		imageLabelList_AlarmText_POCAReco[i]= new QLabel();		
		imageLabelList_AlarmText_POCAReco[i]->setObjectName(objName);
		imageLabelList_AlarmText_POCAReco[i]->setMovie(movieList_AlarmText_POCAReco[i]);
		//imageLabelList_AlarmText_POCAReco[i]->setVisible(false);
		imageLabelList_AlarmText_POCAReco[i]->hide();
		movieList_AlarmText_POCAReco[i]->start();
		

		objName = QString("imageLabelList_Alarm_EMTomographyReco%1").arg(i+1);
		imageLabelList_Alarm_EMTomographyReco[i]= new QLabel();		
		imageLabelList_Alarm_EMTomographyReco[i]->setObjectName(objName);
		imageLabelList_Alarm_EMTomographyReco[i]->setMovie(movieList_Alarm_EMTomographyReco[i]);
		//imageLabelList_Alarm_EMTomographyReco[i]->setVisible(false);
		imageLabelList_Alarm_EMTomographyReco[i]->hide();
		movieList_Alarm_EMTomographyReco[i]->start();
		
		objName = QString("imageLabelList_AlarmText_EMTomographyReco%1").arg(i+1);
		imageLabelList_AlarmText_EMTomographyReco[i]= new QLabel();		
		imageLabelList_AlarmText_EMTomographyReco[i]->setObjectName(objName);
		imageLabelList_AlarmText_EMTomographyReco[i]->setMovie(movieList_AlarmText_EMTomographyReco[i]);
		//imageLabelList_AlarmText_EMTomographyReco[i]->setVisible(false);
		imageLabelList_AlarmText_EMTomographyReco[i]->hide();
		movieList_AlarmText_EMTomographyReco[i]->start();
		

		objName = QString("imageLabelList_Alarm_ACFReco%1").arg(i+1);
		imageLabelList_Alarm_ACFReco[i]= new QLabel();		
		imageLabelList_Alarm_ACFReco[i]->setObjectName(objName);
		imageLabelList_Alarm_ACFReco[i]->setMovie(movieList_Alarm_ACFReco[i]);
		//imageLabelList_Alarm_ACFReco[i]->setVisible(false);
		imageLabelList_Alarm_ACFReco[i]->hide();
		movieList_Alarm_ACFReco[i]->start();
		
		objName = QString("imageLabelList_AlarmText_ACFReco%1").arg(i+1);
		imageLabelList_AlarmText_ACFReco[i]= new QLabel();		
		imageLabelList_AlarmText_ACFReco[i]->setObjectName(objName);
		imageLabelList_AlarmText_ACFReco[i]->setMovie(movieList_AlarmText_ACFReco[i]);
		//imageLabelList_AlarmText_ACFReco[i]->setVisible(false);
		imageLabelList_AlarmText_ACFReco[i]->hide();
		movieList_AlarmText_ACFReco[i]->start();
		

		objName = QString("imageLabelList_Alarm_ClusteringReco%1").arg(i+1);
		imageLabelList_Alarm_ClusteringReco[i]= new QLabel();		
		imageLabelList_Alarm_ClusteringReco[i]->setObjectName(objName);
		imageLabelList_Alarm_ClusteringReco[i]->setMovie(movieList_Alarm_ClusteringReco[i]);
		//imageLabelList_Alarm_ClusteringReco[i]->setVisible(false);	
		imageLabelList_Alarm_ClusteringReco[i]->hide();
		movieList_Alarm_ClusteringReco[i]->start();
		
		objName = QString("imageLabelList_AlarmText_ClusteringReco%1").arg(i+1);
		imageLabelList_AlarmText_ClusteringReco[i]= new QLabel();		
		imageLabelList_AlarmText_ClusteringReco[i]->setObjectName(objName);
		imageLabelList_AlarmText_ClusteringReco[i]->setMovie(movieList_AlarmText_ClusteringReco[i]);
		//imageLabelList_AlarmText_ClusteringReco[i]->setVisible(false);
		imageLabelList_AlarmText_ClusteringReco[i]->hide();
		movieList_AlarmText_ClusteringReco[i]->start();

		objName = QString("progressBarList_fileStatus%1").arg(i+1);
		progressBarList_fileStatus[i]= new QProgressBar();	
		progressBarList_fileStatus[i]->setObjectName(objName);
		progressBarList_fileStatus[i]->setOrientation(Qt::Horizontal);
		progressBarList_fileStatus[i]->setStyleSheet("QProgressBar{background: #F4ECEC; border: 1px solid grey;border-radius: 3px;text-align: center;} QProgressBar::chunk{background-color:#F0C2C2;width: 10px;margin: 0.5px; text-align: center;}");	
		//progressBarList_fileStatus[i]->setTextVisible(true);
		progressBarList_fileStatus[i]->setRange(0,100);
		progressBarList_fileStatus[i]->setValue(0.);

		objName = QString("progressBarList_LLRecoStatus%1").arg(i+1);
		progressBarList_LLRecoStatus[i]= new QProgressBar();	
		progressBarList_LLRecoStatus[i]->setObjectName(objName);
		progressBarList_LLRecoStatus[i]->setOrientation(Qt::Horizontal);
		progressBarList_LLRecoStatus[i]->setStyleSheet("QProgressBar{background: #F4ECEC; border: 1px solid grey;border-radius: 3px;text-align: center;} QProgressBar::chunk{background-color:#F0C2C2;width: 10px;margin: 0.5px; text-align: center;}");	
		//progressBarList_LLRecoStatus[i]->setTextVisible(true);
		progressBarList_LLRecoStatus[i]->setRange(0,100);
		progressBarList_LLRecoStatus[i]->setValue(0.);

		objName = QString("progressBarList_POCARecoStatus%1").arg(i+1);
		progressBarList_POCARecoStatus[i]= new QProgressBar();	
		progressBarList_POCARecoStatus[i]->setObjectName(objName);
		progressBarList_POCARecoStatus[i]->setOrientation(Qt::Horizontal);
		progressBarList_POCARecoStatus[i]->setStyleSheet("QProgressBar{background: #F4ECEC; border: 1px solid grey;border-radius: 3px;text-align: center;} QProgressBar::chunk{background-color:#F0C2C2;width: 10px;margin: 0.5px; text-align: center;}");	
		//progressBarList_POCARecoStatus[i]->setTextVisible(true);
		progressBarList_POCARecoStatus[i]->setRange(0,100);
		progressBarList_POCARecoStatus[i]->setValue(0.);

		objName = QString("progressBarList_ACFRecoStatus%1").arg(i+1);
		progressBarList_ACFRecoStatus[i]= new QProgressBar();	
		progressBarList_ACFRecoStatus[i]->setObjectName(objName);
		progressBarList_ACFRecoStatus[i]->setOrientation(Qt::Horizontal);
		progressBarList_ACFRecoStatus[i]->setStyleSheet("QProgressBar{background: #F4ECEC; border: 1px solid grey;border-radius: 3px;text-align: center;} QProgressBar::chunk{background-color:#F0C2C2;width: 10px;margin: 0.5px; text-align: center;}");	
		//progressBarList_ACFRecoStatus[i]->setTextVisible(true);
		progressBarList_ACFRecoStatus[i]->setRange(0,100);
		progressBarList_ACFRecoStatus[i]->setValue(0.);

		objName = QString("progressBarList_ClusteringRecoStatus%1").arg(i+1);
		progressBarList_ClusteringRecoStatus[i]= new QProgressBar();	
		progressBarList_ClusteringRecoStatus[i]->setObjectName(objName);
		progressBarList_ClusteringRecoStatus[i]->setOrientation(Qt::Horizontal);
		progressBarList_ClusteringRecoStatus[i]->setStyleSheet("QProgressBar{background: #F4ECEC; border: 1px solid grey;border-radius: 3px;text-align: center;} QProgressBar::chunk{background-color:#F0C2C2;width: 10px;margin: 0.5px; text-align: center;}");	
		//progressBarList_ClusteringRecoStatus[i]->setTextVisible(true);
		progressBarList_ClusteringRecoStatus[i]->setRange(0,100);
		progressBarList_ClusteringRecoStatus[i]->setValue(0.);

		
		objName = QString("label_fileStatus%1").arg(i+1);
		labelList_fileStatus[i]= new QLabel();
		labelList_fileStatus[i]->setObjectName(objName);
		labelList_fileStatus[i]->setText("Data Transfer");
		
		objName = QString("label_LLRecoStatus%1").arg(i+1);
		labelList_LLRecoStatus[i]= new QLabel();
		labelList_LLRecoStatus[i]->setObjectName(objName);
		labelList_LLRecoStatus[i]->setText("LL Reco");
		

		objName = QString("label_POCARecoStatus%1").arg(i+1);
		labelList_POCARecoStatus[i]= new QLabel();
		labelList_POCARecoStatus[i]->setObjectName(objName);
		labelList_POCARecoStatus[i]->setText("POCA Reco");
		
		objName = QString("label_ACFRecoStatus%1").arg(i+1);
		labelList_ACFRecoStatus[i]= new QLabel();
		labelList_ACFRecoStatus[i]->setObjectName(objName);
		labelList_ACFRecoStatus[i]->setText("2pt-ACF Reco");
		
		objName = QString("label_ClusteringRecoStatus%1").arg(i+1);
		labelList_ClusteringRecoStatus[i]= new QLabel();
		labelList_ClusteringRecoStatus[i]->setObjectName(objName);
		labelList_ClusteringRecoStatus[i]->setText("Clustering Reco");
		
		
		objName = QString("label_fileStatus%1").arg(i+1);
		labelList_statusBar_fileStatus[i]= new QLabel();
		labelList_statusBar_fileStatus[i]->setObjectName(objName);
		labelList_statusBar_fileStatus[i]->setText("");

		objName = QString("statusBar_fileStatus%1").arg(i+1);
		statusBarList_fileStatus[i]= new QStatusBar();
		statusBarList_fileStatus[i]->setStyleSheet("QStatusBar::item {background: #F4ECEC; border: 1px solid grey;border-radius: 3px;text-align: center;}");
		statusBarList_fileStatus[i]->addPermanentWidget(labelList_statusBar_fileStatus[i], 150);
		statusBarList_fileStatus[i]->setFixedSize(QSize(400,30));
		
		objName = QString("label_LLRecoStatus%1").arg(i+1);
		labelList_statusBar_LLRecoStatus[i]= new QLabel();
		labelList_statusBar_LLRecoStatus[i]->setObjectName(objName);
		labelList_statusBar_LLRecoStatus[i]->setText("");

		objName = QString("statusBar_LLRecoStatus%1").arg(i+1);
		statusBarList_LLRecoStatus[i]= new QStatusBar();
		statusBarList_LLRecoStatus[i]->setStyleSheet("QStatusBar::item {background: #F4ECEC; border: 1px solid grey;border-radius: 3px;text-align: center;} ");
		statusBarList_LLRecoStatus[i]->addPermanentWidget(labelList_statusBar_LLRecoStatus[i], 150);
		statusBarList_LLRecoStatus[i]->setFixedSize(QSize(400,30));
		
		objName = QString("label_POCARecoStatus%1").arg(i+1);
		labelList_statusBar_POCARecoStatus[i]= new QLabel();
		labelList_statusBar_POCARecoStatus[i]->setObjectName(objName);
		labelList_statusBar_POCARecoStatus[i]->setText("");

		objName = QString("statusBar_POCARecoStatus%1").arg(i+1);
		statusBarList_POCARecoStatus[i]= new QStatusBar();
		statusBarList_POCARecoStatus[i]->setStyleSheet("QStatusBar::item {background: #F4ECEC; border: 1px solid grey;border-radius: 3px;text-align: center;} ");
		statusBarList_POCARecoStatus[i]->addPermanentWidget(labelList_statusBar_POCARecoStatus[i], 150);
		statusBarList_POCARecoStatus[i]->setFixedSize(QSize(400,30));
		
		objName = QString("label_ACFRecoStatus%1").arg(i+1);
		labelList_statusBar_ACFRecoStatus[i]= new QLabel();
		labelList_statusBar_ACFRecoStatus[i]->setObjectName(objName);
		labelList_statusBar_ACFRecoStatus[i]->setText("");

		objName = QString("statusBar_ACFRecoStatus%1").arg(i+1);
		statusBarList_ACFRecoStatus[i]= new QStatusBar();
		statusBarList_ACFRecoStatus[i]->setStyleSheet("QStatusBar::item {background: #F4ECEC; border: 1px solid grey;border-radius: 3px;text-align: center;}");
		statusBarList_ACFRecoStatus[i]->addPermanentWidget(labelList_statusBar_ACFRecoStatus[i], 150);
		statusBarList_ACFRecoStatus[i]->setFixedSize(QSize(400,30));
		
		objName = QString("label_ClusteringRecoStatus%1").arg(i+1);
		labelList_statusBar_ClusteringRecoStatus[i]= new QLabel();
		labelList_statusBar_ClusteringRecoStatus[i]->setObjectName(objName);
		labelList_statusBar_ClusteringRecoStatus[i]->setText("");

		objName = QString("statusBar_ClusteringRecoStatus%1").arg(i+1);
		statusBarList_ClusteringRecoStatus[i]= new QStatusBar();
		statusBarList_ClusteringRecoStatus[i]->setStyleSheet("QStatusBar::item {background: #F4ECEC; border: 1px solid grey;border-radius: 3px;text-align: center;}");
		statusBarList_ClusteringRecoStatus[i]->addPermanentWidget(labelList_statusBar_ClusteringRecoStatus[i], 150);
		statusBarList_ClusteringRecoStatus[i]->setFixedSize(QSize(400,30));
		
	
		objName = QString("pushButtonList_LLRecoStatus%1").arg(i+1);
		pushButtonList_LLRecoStatus[i]= new QPushButton();	
		pushButtonList_LLRecoStatus[i]->setObjectName(objName);
		pushButtonList_LLRecoStatus[i]->setIcon(QIcon(viewButton_imagepath.c_str()));
		pushButtonList_LLRecoStatus[i]->setFixedSize(QSize(60,30));
		pushButtonList_LLRecoStatus[i]->setEnabled(false);
		pushButtonList_LLRecoStatus[i]->setText(QString("View"));

		objName = QString("pushButtonList_POCARecoStatus%1").arg(i+1);
		pushButtonList_POCARecoStatus[i]= new QPushButton();	
		pushButtonList_POCARecoStatus[i]->setObjectName(objName);
		pushButtonList_POCARecoStatus[i]->setIcon(QIcon(viewButton_imagepath.c_str()));
		pushButtonList_POCARecoStatus[i]->setFixedSize(QSize(60,30));
		pushButtonList_POCARecoStatus[i]->setEnabled(false);
		pushButtonList_POCARecoStatus[i]->setText(QString("View"));


		objName = QString("pushButtonList_ACFRecoStatus%1").arg(i+1);
		pushButtonList_ACFRecoStatus[i]= new QPushButton();	
		pushButtonList_ACFRecoStatus[i]->setObjectName(objName);
		pushButtonList_ACFRecoStatus[i]->setIcon(QIcon(viewButton_imagepath.c_str()));
		pushButtonList_ACFRecoStatus[i]->setFixedSize(QSize(60,30));
		pushButtonList_ACFRecoStatus[i]->setEnabled(false);
		pushButtonList_ACFRecoStatus[i]->setText(QString("View"));

		objName = QString("pushButtonList_ClusteringRecoStatus%1").arg(i+1);
		pushButtonList_ClusteringRecoStatus[i]= new QPushButton();	
		pushButtonList_ClusteringRecoStatus[i]->setObjectName(objName);
		pushButtonList_ClusteringRecoStatus[i]->setIcon(QIcon(viewButton_imagepath.c_str()));
		pushButtonList_ClusteringRecoStatus[i]->setFixedSize(QSize(60,30));
		pushButtonList_ClusteringRecoStatus[i]->setEnabled(false);
		pushButtonList_ClusteringRecoStatus[i]->setText(QString("View"));


		
		objName = QString("stopThreadButtonList_LLRecoStatus%1").arg(i+1);
		stopThreadButtonList_LLRecoStatus[i]= new QPushButton();	
		stopThreadButtonList_LLRecoStatus[i]->setObjectName(objName);
		stopThreadButtonList_LLRecoStatus[i]->setIcon(QIcon(stopButton_imagepath.c_str()));
		stopThreadButtonList_LLRecoStatus[i]->setFixedSize(QSize(60,30));
		stopThreadButtonList_LLRecoStatus[i]->setEnabled(false);
		stopThreadButtonList_LLRecoStatus[i]->setText(QString("Stop"));

		objName = QString("stopThreadButtonList_POCARecoStatus%1").arg(i+1);
		stopThreadButtonList_POCARecoStatus[i]= new QPushButton();	
		stopThreadButtonList_POCARecoStatus[i]->setObjectName(objName);
		stopThreadButtonList_POCARecoStatus[i]->setIcon(QIcon(stopButton_imagepath.c_str()));
		stopThreadButtonList_POCARecoStatus[i]->setFixedSize(QSize(60,30));
		stopThreadButtonList_POCARecoStatus[i]->setEnabled(false);
		stopThreadButtonList_POCARecoStatus[i]->setText(QString("Stop"));

		objName = QString("stopThreadButtonList_ACFRecoStatus%1").arg(i+1);
		stopThreadButtonList_ACFRecoStatus[i]= new QPushButton();	
		stopThreadButtonList_ACFRecoStatus[i]->setObjectName(objName);
		stopThreadButtonList_ACFRecoStatus[i]->setIcon(QIcon(stopButton_imagepath.c_str()));
		stopThreadButtonList_ACFRecoStatus[i]->setFixedSize(QSize(60,30));
		stopThreadButtonList_ACFRecoStatus[i]->setEnabled(false);
		stopThreadButtonList_ACFRecoStatus[i]->setText(QString("Stop"));

		objName = QString("stopThreadButtonList_ClusteringRecoStatus%1").arg(i+1);
		stopThreadButtonList_ClusteringRecoStatus[i]= new QPushButton();	
		stopThreadButtonList_ClusteringRecoStatus[i]->setObjectName(objName);
		stopThreadButtonList_ClusteringRecoStatus[i]->setIcon(QIcon(stopButton_imagepath.c_str()));
		stopThreadButtonList_ClusteringRecoStatus[i]->setFixedSize(QSize(60,30));
		stopThreadButtonList_ClusteringRecoStatus[i]->setEnabled(false);
		stopThreadButtonList_ClusteringRecoStatus[i]->setText(QString("Stop"));

		objName = QString("pushButtonList_POCAVolRendering%1").arg(i+1);
		pushButtonList_POCAVolRendering[i]= new QPushButton();	
		pushButtonList_POCAVolRendering[i]->setObjectName(objName);
		pushButtonList_POCAVolRendering[i]->setIcon(QIcon(viewButton_imagepath.c_str()));
		//pushButtonList_POCAVolRendering[i]->setFixedSize(QSize(130,30));
		pushButtonList_POCAVolRendering[i]->setFixedSize(QSize(60,30));
		pushButtonList_POCAVolRendering[i]->setEnabled(false);
		pushButtonList_POCAVolRendering[i]->setText(QString("View"));

		objName = QString("pushButtonList_LLVolRendering%1").arg(i+1);
		pushButtonList_LLVolRendering[i]= new QPushButton();	
		pushButtonList_LLVolRendering[i]->setObjectName(objName);
		pushButtonList_LLVolRendering[i]->setIcon(QIcon(viewButton_imagepath.c_str()));
		//pushButtonList_LLVolRendering[i]->setFixedSize(QSize(130,30));	
		pushButtonList_LLVolRendering[i]->setFixedSize(QSize(60,30));
		pushButtonList_LLVolRendering[i]->setEnabled(false);
		pushButtonList_LLVolRendering[i]->setText(QString("View"));

		objName = QString("pushButtonList_ClusteringVolRendering%1").arg(i+1);
		pushButtonList_ClusteringVolRendering[i]= new QPushButton();	
		pushButtonList_ClusteringVolRendering[i]->setObjectName(objName);
		pushButtonList_ClusteringVolRendering[i]->setIcon(QIcon(viewButton_imagepath.c_str()));
		//pushButtonList_ClusteringVolRendering[i]->setFixedSize(QSize(130,30));
		pushButtonList_ClusteringVolRendering[i]->setFixedSize(QSize(60,30));
		pushButtonList_ClusteringVolRendering[i]->setEnabled(false);
		pushButtonList_ClusteringVolRendering[i]->setText(QString("View"));


		pushButtonList_POCAViewer.push_back(pushButtonList_POCAVolRendering[i]);
		pushButtonList_LLViewer.push_back(pushButtonList_LLVolRendering[i]);
		pushButtonList_ACFViewer.push_back(pushButtonList_ACFRecoStatus[i]);
		pushButtonList_ClusteringViewer.push_back(pushButtonList_ClusteringVolRendering[i]);

		
		//mMediaObject_POCAReco[i] = new Phonon::MediaObject(this);
		//mMediaObject_POCAReco[i]->setCurrentSource(Phonon::MediaSource(alarm_soundpath.c_str()));
		//mAudioOutput_POCAReco[i] = new Phonon::AudioOutput(Phonon::MusicCategory, this);
		//Phonon::createPath(mMediaObject_POCAReco[i], mAudioOutput_POCAReco[i]);

		//mMediaObject_LLReco[i] = new Phonon::MediaObject(this);
		//mMediaObject_LLReco[i]->setCurrentSource(Phonon::MediaSource(alarm_soundpath.c_str()));
		//mAudioOutput_LLReco[i] = new Phonon::AudioOutput(Phonon::MusicCategory, this);
		//Phonon::createPath(mMediaObject_LLReco[i], mAudioOutput_LLReco[i]);

		//mMediaObject_ClusteringReco[i] = new Phonon::MediaObject(this);
		//mMediaObject_ClusteringReco[i]->setCurrentSource(Phonon::MediaSource(alarm_soundpath.c_str()));
		//mAudioOutput_ClusteringReco[i] = new Phonon::AudioOutput(Phonon::MusicCategory, this);
		//Phonon::createPath(mMediaObject_ClusteringReco[i], mAudioOutput_ClusteringReco[i]);

		//mMediaObject_ACFReco[i] = new Phonon::MediaObject(this);
		//mMediaObject_ACFReco[i]->setCurrentSource(Phonon::MediaSource(alarm_soundpath.c_str()));
		//mAudioOutput_ACFReco[i] = new Phonon::AudioOutput(Phonon::MusicCategory, this);
		//Phonon::createPath(mMediaObject_ACFReco[i], mAudioOutput_ACFReco[i]);
		
	}//end loop
		
	
	//## Set object actions	
	connect(pushButton_Run, SIGNAL(clicked()), this, SLOT(SetInitImageStatus()) );
	connect(pushButton_Run, SIGNAL(clicked()), this, SLOT(Run()) );
	connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(serviceReplyFinished(QNetworkReply*)));
	
	connect(this,SIGNAL(downloadStartSig(QString)), this, SLOT(downloadStart(QString)) );
	connect(this,SIGNAL(EMMLStartSig(QString)), this, SLOT(EMMLStart(QString)) );
	connect(this,SIGNAL(POCAStartSig(QString)), this, SLOT(POCAStart(QString)) );
	connect(this,SIGNAL(FOFStartSig(QString)), this, SLOT(FOFStart(QString)) );
	connect(this,SIGNAL(ACFStartSig(QString)), this, SLOT(ACFStart(QString)) );
	connect(this,SIGNAL(viewerStartSig(QString)), this, SLOT(viewerStart(QString)) );
	
	for(int i=0;i<MAXNSCANS;i++){
		connect(pushButtonList_LLVolRendering[i], SIGNAL(clicked()), this, SLOT(StartLLVolRenderingViewer()) );
		connect(pushButtonList_POCAVolRendering[i], SIGNAL(clicked()), this, SLOT(StartPOCAVolRenderingViewer()) );
		connect(pushButtonList_ClusteringVolRendering[i], SIGNAL(clicked()), this, SLOT(StartClusteringVolRenderingViewer()) );
	}


	//## Set object layout
	QHBoxLayout* POCAAlarmBoxGroupLayout[MAXNSCANS];
	QHBoxLayout* ClusteringAlarmBoxGroupLayout[MAXNSCANS];
	QHBoxLayout* LLAlarmBoxGroupLayout[MAXNSCANS];
	QHBoxLayout* ACFAlarmBoxGroupLayout[MAXNSCANS];
	
	
	for(int i=0;i<MAXNSCANS;i++){
		
		
		POCAAlarmBoxGroupLayout[i]= new QHBoxLayout;	
		POCAAlarmBoxGroupLayout[i]->addWidget(imageLabelList_Alarm_POCAReco[i]);
		POCAAlarmBoxGroupLayout[i]->addWidget(imageLabelList_AlarmText_POCAReco[i]);
		POCAAlarmBoxGroupLayout[i]->addStretch(1);
		
		ClusteringAlarmBoxGroupLayout[i]= new QHBoxLayout;	
		ClusteringAlarmBoxGroupLayout[i]->addWidget(imageLabelList_Alarm_ClusteringReco[i]);
		ClusteringAlarmBoxGroupLayout[i]->addWidget(imageLabelList_AlarmText_ClusteringReco[i]);
		ClusteringAlarmBoxGroupLayout[i]->addStretch(1);

		ACFAlarmBoxGroupLayout[i]= new QHBoxLayout;	
		ACFAlarmBoxGroupLayout[i]->addWidget(imageLabelList_Alarm_ACFReco[i]);
		ACFAlarmBoxGroupLayout[i]->addWidget(imageLabelList_AlarmText_ACFReco[i]);
		ACFAlarmBoxGroupLayout[i]->addStretch(1);

		LLAlarmBoxGroupLayout[i]= new QHBoxLayout;	
		LLAlarmBoxGroupLayout[i]->addWidget(imageLabelList_Alarm_EMTomographyReco[i]);
		LLAlarmBoxGroupLayout[i]->addWidget(imageLabelList_AlarmText_EMTomographyReco[i]);
		LLAlarmBoxGroupLayout[i]->addStretch(1);
		

		int counter= 1;
		QString objName = QString("DisplayBoxGroupLayout%1").arg(i+1);
		DisplayBoxGroupLayout[i]= new QGridLayout;	
		DisplayBoxGroupLayout[i]->setObjectName(objName);
		DisplayBoxGroupLayout[i]->addWidget(imageLabelList_fileStatus[i],counter,1);
		DisplayBoxGroupLayout[i]->addWidget(labelList_fileStatus[i],counter,2);	
		DisplayBoxGroupLayout[i]->addWidget(progressBarList_fileStatus[i],counter,3,Qt::AlignCenter);
		DisplayBoxGroupLayout[i]->addWidget(statusBarList_fileStatus[i],counter,4);
		counter++;

		DisplayBoxGroupLayout[i]->addWidget(imageLabelList_LLRecoStatus[i],counter,1);	
		DisplayBoxGroupLayout[i]->addWidget(labelList_LLRecoStatus[i],counter,2);
		DisplayBoxGroupLayout[i]->addWidget(progressBarList_LLRecoStatus[i],counter,3,Qt::AlignCenter);
		DisplayBoxGroupLayout[i]->addWidget(statusBarList_LLRecoStatus[i],counter,4);
		//DisplayBoxGroupLayout[i]->addWidget(stopThreadButtonList_LLRecoStatus[i],counter,5);
		DisplayBoxGroupLayout[i]->addWidget(pushButtonList_LLVolRendering[i],counter,5);
		DisplayBoxGroupLayout[i]->addLayout(LLAlarmBoxGroupLayout[i],counter,6);
		counter++;

		DisplayBoxGroupLayout[i]->addWidget(imageLabelList_POCARecoStatus[i],counter,1);	
		DisplayBoxGroupLayout[i]->addWidget(labelList_POCARecoStatus[i],counter,2);
		DisplayBoxGroupLayout[i]->addWidget(progressBarList_POCARecoStatus[i],counter,3,Qt::AlignCenter);
		DisplayBoxGroupLayout[i]->addWidget(statusBarList_POCARecoStatus[i],counter,4);	
		//DisplayBoxGroupLayout[i]->addWidget(stopThreadButtonList_POCARecoStatus[i],counter,5);
		DisplayBoxGroupLayout[i]->addWidget(pushButtonList_POCAVolRendering[i],counter,5);
		DisplayBoxGroupLayout[i]->addLayout(POCAAlarmBoxGroupLayout[i],counter,6);
		counter++;

		DisplayBoxGroupLayout[i]->addWidget(imageLabelList_ACFRecoStatus[i],counter,1);	
		DisplayBoxGroupLayout[i]->addWidget(labelList_ACFRecoStatus[i],counter,2);
		DisplayBoxGroupLayout[i]->addWidget(progressBarList_ACFRecoStatus[i],counter,3,Qt::AlignCenter);		
		DisplayBoxGroupLayout[i]->addWidget(statusBarList_ACFRecoStatus[i],counter,4);	
		//DisplayBoxGroupLayout[i]->addWidget(stopThreadButtonList_ACFRecoStatus[i],counter,5);
		DisplayBoxGroupLayout[i]->addWidget(pushButtonList_ACFRecoStatus[i],counter,5);
		DisplayBoxGroupLayout[i]->addLayout(ACFAlarmBoxGroupLayout[i],counter,6);
		counter++;

		DisplayBoxGroupLayout[i]->addWidget(imageLabelList_ClusteringRecoStatus[i],counter,1);	
		DisplayBoxGroupLayout[i]->addWidget(labelList_ClusteringRecoStatus[i],counter,2);	
		DisplayBoxGroupLayout[i]->addWidget(progressBarList_ClusteringRecoStatus[i],counter,3,Qt::AlignCenter);
		DisplayBoxGroupLayout[i]->addWidget(statusBarList_ClusteringRecoStatus[i],counter,4);
		//DisplayBoxGroupLayout[i]->addWidget(stopThreadButtonList_ClusteringRecoStatus[i],counter,5);
		DisplayBoxGroupLayout[i]->addWidget(pushButtonList_ClusteringVolRendering[i],counter,5);
		DisplayBoxGroupLayout[i]->addLayout(ClusteringAlarmBoxGroupLayout[i],counter,6);	
		counter++;

		groupBoxList_display[i]->setLayout(DisplayBoxGroupLayout[i]);
		
	}//end loop frames
	

	
	QVBoxLayout* upLayout = new QVBoxLayout;
	for(int i=0;i<MAXNSCANS;i++) upLayout->addWidget(groupBoxList_display[i]);
	upLayout->addStretch(1);

	QHBoxLayout* runExecLayout= new QHBoxLayout;
	runExecLayout->addWidget(label_runExecMode);
	runExecLayout->addWidget(comboBox_runExecMode);
	runExecLayout->addStretch(1);

	QHBoxLayout* runModeLayout = new QHBoxLayout;
	runModeLayout->addWidget(checkBox_runMode);
	runModeLayout->addLayout(runExecLayout);
	runModeLayout->addStretch(1);

	QVBoxLayout* buttonLayout = new QVBoxLayout;
	buttonLayout->addLayout(runModeLayout);
	buttonLayout->addWidget(pushButton_Run);
	buttonLayout->addStretch(1);

	QVBoxLayout* mainLayout = new QVBoxLayout;	
	mainLayout->addLayout(upLayout);	
	mainLayout->addSpacerItem(new QSpacerItem(0, 200, QSizePolicy::Maximum, QSizePolicy::Maximum));
	mainLayout->addLayout(buttonLayout);
	mainLayout->addStretch(1);
	setLayout(mainLayout);
	

}//close constructor


TomographyRecTabMenu::~TomographyRecTabMenu(){

	
}//close destructor


void TomographyRecTabMenu::SetTomographyRecOptions(){

	
}//close TomographyRecTabMenu::SetTomographyRecOptions()


void TomographyRecTabMenu::downloadStart(QString msg){

	//## Parse message
	QJson::Parser parser;	
	bool ok;
	
	QByteArray data;
	data.append(QString(msg));
	QVariantList result = parser.parse(data, &ok).toList();
	
	if(!ok){
		cerr<<"TomographyRecTabMenu::downloadStart(). ERROR: Cannot parse JSON string message...exit!"<<endl;
		return;
	}
	
	std::string inputFileName= "";
	std::string outputFileName= "";
	std::string jobDirName= "";
	std::string hostName= "";
	std::string userName= "";
	int portNo= -1;
	std::string guihostName= "";
	int guiportNo= -1;
	std::string guiKey= "";
	int retryAttempts= 0;
	bool isLocalDownload= false;
	int scanId= 1;
	std::string containerId= "XXX";
	long int timeStamp= 0;

	
	foreach(QVariant record, result) {
    QVariantMap map = record.toMap();	
		scanId= map.value("scanId").toInt();	
		containerId= (map.value("containerId").toString()).toStdString();
		timeStamp= (long)(map.value("timeStamp").toLongLong());
    inputFileName= (map.value("inputFileName").toString()).toStdString();
		outputFileName= (map.value("outputFileName").toString()).toStdString();	
		hostName= (map.value("hostName").toString()).toStdString();
		userName= (map.value("userName").toString()).toStdString();
		portNo= map.value("portNo").toInt();
		retryAttempts= map.value("retryAttempts").toInt();
		isLocalDownload= map.value("isLocalDownload").toInt();
		jobDirName= (map.value("jobDirName").toString()).toStdString();	
		guihostName= (map.value("guihostName").toString()).toStdString();
		guiportNo= map.value("guiportNo").toInt();
		guiKey= (map.value("guiKey").toString()).toStdString();
	}

	cout<<"TomographyRecTabMenu::downloadStart(): INFO: inputFileName="<<inputFileName<<" jobDirName="<<jobDirName<<"  outputFileName="<<outputFileName<<"  hostName="<<hostName<<"  userName="<<userName<<"  portNo="<<portNo<<"  guihostName="<<guihostName<<"  guiportNo="<<guiportNo<<"  guiKey="<<guiKey<<"  isLocalDownload="<<isLocalDownload<<"  scanId="<<scanId<<"  containerId="<<containerId<<"  timeStamp="<<timeStamp<<endl;
	
	
	//## Form script name
	std::string scriptName= Gui::GUI_SCRIPT_DIR + std::string("/DownloaderSubmitter.sh");
	QString exeFilePath = scriptName.c_str();

	QFileInfo scriptFileInfo(scriptName.c_str());
	if(!scriptFileInfo.exists()){
		cerr<<"TomographyRecTabMenu::downloadStart(): ERROR: Downloader script file does not exists...exit!"<<endl;
		return;
	}	

	//## Form args for batch script
	std::string inputFileArg= std::string("--input=") + inputFileName;
	std::string outputFileArg= std::string("--output=") + outputFileName;
	std::string hostNameArg= std::string("--host=") + hostName;
	std::string userNameArg= std::string("--user=") + userName;
	std::string portNoArg= Form("--port=%d",portNo);
	std::string guiHostNameArg= std::string("--guihost=") + guihostName;
	std::string guiPortNoArg= Form("--guiport=%d",guiportNo);
	std::string guiKeyArg= std::string("--guikey=") + guiKey;
	std::string nRetryArg= Form("--nretry=%d",retryAttempts);
	std::string scanIdArg= Form("--scanId=%d",scanId);
	std::string containerIdArg= std::string("--containerId=") + containerId;
	std::string timeStampArg= Form("--timeStamp=%ld",timeStamp);
	std::string localDownloadArg= "";
	if(fRunExecMode==eLocalBatch) {
		localDownloadArg= "--local";
	}

	QStringList args;
  args << scriptName.c_str() << inputFileArg.c_str() << outputFileArg.c_str() << hostNameArg.c_str() << userNameArg.c_str() << portNoArg.c_str() << guiHostNameArg.c_str() << guiPortNoArg.c_str() << guiKeyArg.c_str() << nRetryArg.c_str() << scanIdArg.c_str() << containerIdArg.c_str() << timeStampArg.c_str() << localDownloadArg.c_str();
		
	QString shell_path= QString::fromStdString(Gui::SHELL_DIR);

	//## Execute downloader script
	bool process_status= QProcess::startDetached(shell_path, args);	
		
	if(!process_status){
		cerr<<"TomographyRecTabMenu::downloadStart(): ERROR: Cannot start downloader process...exit!"<<endl;
		return;
	}
	
}//close TomographyRecTabMenu::downloadStart()



void TomographyRecTabMenu::EMMLStart(QString msg){

	//## Parse string
	QJson::Parser parser;	
	bool ok;
	
	QByteArray data;
	data.append(QString(msg));
	QVariantList result = parser.parse(data, &ok).toList();
	
	if(!ok){
		cerr<<"TomographyRecTabMenu::EMMLStart(). ERROR: Cannot parse JSON string message...exit!"<<endl;
		return;
	}
	
	std::string inputFileName= "";
	std::string outputFileName= "";
	std::string inputFileName_EMML= "";
	std::string outputFileName_EMML= "";
	std::string configFileName_EMML= "";
	std::string jobDirName= "";
	std::string guihostName= "";
	int guiportNo= -1;
	std::string guiKey= "";
	int scanId= 1;
	std::string containerId= "XXX";
	long int timeStamp= 0;

	
	foreach(QVariant record, result) {
    QVariantMap map = record.toMap();	
		scanId= map.value("scanId").toInt();	
		containerId= (map.value("containerId").toString()).toStdString();
		timeStamp= (long)(map.value("timeStamp").toLongLong());
    inputFileName= (map.value("inputFileName").toString()).toStdString();
		outputFileName= (map.value("outputFileName").toString()).toStdString();	
		jobDirName= (map.value("jobDirName").toString()).toStdString();	
		guihostName= (map.value("guihostName").toString()).toStdString();
		guiportNo= map.value("guiportNo").toInt();
		guiKey= (map.value("guiKey").toString()).toStdString();
	}

		 
	cout<<"TomographyRecTabMenu::EMMLStart(): INFO: inputFileName="<<inputFileName<<" jobDirName="<<jobDirName<<"  outputFileName="<<outputFileName<<"  guihostName="<<guihostName<<"  guiportNo="<<guiportNo<<"  guiKey="<<guiKey<<"  scanId="<<scanId<<"  containerId="<<containerId<<"  timeStamp="<<timeStamp<<endl;
	
	//## Copy config file into the output job directory
	std::string configFileName= Gui::GUI_CONFIG_DIR + std::string("/EMMLConfig.cfg");

	if(fRunExecMode==eLocalInteractive || fRunExecMode==eLocalBatch){
		
		std::string cpProcExe= Form("cp %s %s",configFileName.c_str(),jobDirName.c_str());
		Utilities::ExecSystemCommand(cpProcExe.c_str());
		cout<<"TomographyRecTabMenu::POCAStart(): INFO: cpProcExe: "<<cpProcExe<<endl;

	}
	else if(fRunExecMode==eRemoteBatch){//remote copy
	
		std::string scpProcExe= Form("scp %s %s@%s:%s",configFileName.c_str(),Gui::SEI_USERNAME.c_str(),Gui::SEI_HOSTNAME.c_str(),jobDirName.c_str());
		Utilities::ExecSystemCommand(scpProcExe.c_str());
		cout<<"TomographyRecTabMenu::EMMLStart(): INFO: scpProcExe: "<<scpProcExe<<endl;

	}
	else{
		cerr<<"TomographyRecTabMenu::EMMLStart(): INFO: Invalid run option ("<<fRunMode<<") ...exit!"<<endl;
		return;
	}

			
	//## Set input and output filename for EMML
	inputFileName_EMML= jobDirName + std::string("/") + outputFileName;
	outputFileName_EMML= jobDirName + std::string("/EMMLOutput_") + Utilities::GetBaseFileNameNoPath(outputFileName) + std::string(".root");
	configFileName_EMML= jobDirName + std::string("/EMMLConfig.cfg");

	cout<<"TomographyRecTabMenu::EMMLStart(): INFO: inputFileName_EMML="<<inputFileName_EMML<<"  outputFileName_EMML="<<outputFileName_EMML<<"  configFileName_EMML="<<configFileName_EMML<<endl;

	//Form script name
	std::string scriptName= Gui::GUI_SCRIPT_DIR + std::string("/EMMLReconstructorSubmitter.sh");
	QString exeFilePath = scriptName.c_str();
	
	QFileInfo scriptFileInfo(scriptName.c_str());
	if(!scriptFileInfo.exists()){
		cerr<<"TomographyRecTabMenu::EMMLStart(): ERROR: EMML submitter script file does not exists...exit!"<<endl;
		return;
	}	

	//Form args for batch script
	std::string inputFileArg= std::string("--input=") + inputFileName_EMML;
	std::string outputFileArg= std::string("--output=") + outputFileName_EMML;
	std::string configFileArg= std::string("--config=") + configFileName_EMML;
	std::string guiHostNameArg= std::string("--guihost=") + guihostName;
	std::string guiPortNoArg= Form("--guiport=%d",guiportNo);
	std::string guiKeyArg= std::string("--guikey=") + guiKey;
	std::string scanIdArg= Form("--scanId=%d",scanId);
	std::string containerIdArg= std::string("--containerId=") + containerId;
	std::string timeStampArg= Form("--timeStamp=%ld",timeStamp);
	std::string localRunArg= "";
	if(fRunExecMode==eLocalInteractive || fRunExecMode==eLocalBatch) localRunArg= "--local";
	std::string mpiVersionArg= "--useMPIVersion";
	std::string nprocArg= "--nprocMPI=2";

		

	QStringList args;
  args << scriptName.c_str() << inputFileArg.c_str() << outputFileArg.c_str() << configFileArg.c_str() << guiHostNameArg.c_str() << guiPortNoArg.c_str() << guiKeyArg.c_str() << scanIdArg.c_str() << containerIdArg.c_str() << timeStampArg.c_str() << localRunArg.c_str() << mpiVersionArg.c_str() << nprocArg.c_str();
		
	QString shell_path= QString::fromStdString(Gui::SHELL_DIR);

	//## Execute EMML script
	bool process_status= QProcess::startDetached(shell_path, args);	
		
	if(!process_status){
		cerr<<"TomographyRecTabMenu::EMMLStart(): ERROR: Cannot start EMMLStart process...exit!"<<endl;
		return;
	}
	

}//close TomographyRecTabMenu::EMMLStart()


void TomographyRecTabMenu::POCAStart(QString msg){

	//## Parse string
	QJson::Parser parser;	
	bool ok;
	
	QByteArray data;
	data.append(QString(msg));
	QVariantList result = parser.parse(data, &ok).toList();
	
	if(!ok){
		cerr<<"TomographyRecTabMenu::POCAStart(). ERROR: Cannot parse JSON string message...exit!"<<endl;
		return;
	}
	
	std::string inputFileName= "";
	std::string outputFileName= "";
	std::string inputFileName_POCA= "";
	std::string outputFileName_POCA= "";
	std::string configFileName_POCA= "";
	std::string jobDirName= "";
	std::string guihostName= "";
	int guiportNo= -1;
	std::string guiKey= "";
	int scanId= 1;
	std::string containerId= "XXX";
	long int timeStamp= 0;

	
	foreach(QVariant record, result) {
    QVariantMap map = record.toMap();	
		scanId= map.value("scanId").toInt();	
		containerId= (map.value("containerId").toString()).toStdString();
		timeStamp= (long)(map.value("timeStamp").toLongLong());
    inputFileName= (map.value("inputFileName").toString()).toStdString();
		outputFileName= (map.value("outputFileName").toString()).toStdString();	
		jobDirName= (map.value("jobDirName").toString()).toStdString();	
		guihostName= (map.value("guihostName").toString()).toStdString();
		guiportNo= map.value("guiportNo").toInt();
		guiKey= (map.value("guiKey").toString()).toStdString();
	}

		 
	cout<<"TomographyRecTabMenu::POCAStart(): INFO: inputFileName="<<inputFileName<<" jobDirName="<<jobDirName<<"  outputFileName="<<outputFileName<<"  guihostName="<<guihostName<<"  guiportNo="<<guiportNo<<"  guiKey="<<guiKey<<"  scanId="<<scanId<<"  containerId="<<containerId<<"  timeStamp="<<timeStamp<<endl;
	
	//## Copy config file into the output job directory
	std::string configFileName= Gui::GUI_CONFIG_DIR + std::string("/POCAConfig.cfg");

	if(fRunExecMode==eLocalInteractive || fRunExecMode==eLocalBatch){
		
		std::string cpProcExe= Form("cp %s %s",configFileName.c_str(),jobDirName.c_str());
		Utilities::ExecSystemCommand(cpProcExe.c_str());
		cout<<"TomographyRecTabMenu::POCAStart(): INFO: cpProcExe: "<<cpProcExe<<endl;

	}
	else if(fRunExecMode==eRemoteBatch){//remote copy
	
		std::string scpProcExe= Form("scp %s %s@%s:%s",configFileName.c_str(),Gui::SEI_USERNAME.c_str(),Gui::SEI_HOSTNAME.c_str(),jobDirName.c_str());
		Utilities::ExecSystemCommand(scpProcExe.c_str());
		cout<<"TomographyRecTabMenu::POCAStart(): INFO: scpProcExe: "<<scpProcExe<<endl;

	}
	else{
		cerr<<"TomographyRecTabMenu::POCAStart(): INFO: Invalid run option ("<<fRunMode<<") ...exit!"<<endl;
		return;
	}

			
	//## Set input and output filename for POCA
	inputFileName_POCA= jobDirName + std::string("/") + outputFileName;
	outputFileName_POCA= jobDirName + std::string("/POCAOutput_") + Utilities::GetBaseFileNameNoPath(outputFileName) + std::string(".root");
	configFileName_POCA= jobDirName + std::string("/POCAConfig.cfg");

	cout<<"TomographyRecTabMenu::POCAStart(): INFO: inputFileName_POCA="<<inputFileName_POCA<<"  outputFileName_POCA="<<outputFileName_POCA<<"  configFileName_POCA="<<configFileName_POCA<<endl;

	//Form script name
	std::string scriptName= Gui::GUI_SCRIPT_DIR + std::string("/POCAReconstructorSubmitter.sh");
	QString exeFilePath = scriptName.c_str();
	
	QFileInfo scriptFileInfo(scriptName.c_str());
	if(!scriptFileInfo.exists()){
		cerr<<"TomographyRecTabMenu::POCAStart(): ERROR: POCA submitter script file does not exists...exit!"<<endl;
		return;
	}	

	//Form args for batch script
	std::string inputFileArg= std::string("--input=") + inputFileName_POCA;
	std::string outputFileArg= std::string("--output=") + outputFileName_POCA;
	std::string configFileArg= std::string("--config=") + configFileName_POCA;
	std::string guiHostNameArg= std::string("--guihost=") + guihostName;
	std::string guiPortNoArg= Form("--guiport=%d",guiportNo);
	std::string guiKeyArg= std::string("--guikey=") + guiKey;
	std::string scanIdArg= Form("--scanId=%d",scanId);
	std::string containerIdArg= std::string("--containerId=") + containerId;
	std::string timeStampArg= Form("--timeStamp=%ld",timeStamp);
	std::string localRunArg= "";
	if(fRunExecMode==eLocalInteractive || fRunExecMode==eLocalBatch) localRunArg= "--local";
		

	QStringList args;
  args << scriptName.c_str() << inputFileArg.c_str() << outputFileArg.c_str() << configFileArg.c_str() << guiHostNameArg.c_str() << guiPortNoArg.c_str() << guiKeyArg.c_str() << scanIdArg.c_str() << containerIdArg.c_str() << timeStampArg.c_str() << localRunArg.c_str();
		
	QString shell_path= QString::fromStdString(Gui::SHELL_DIR);

	//## Execute POCA script
	bool process_status= QProcess::startDetached(shell_path, args);	
		
	if(!process_status){
		cerr<<"TomographyRecTabMenu::POCAStart(): ERROR: Cannot start POCAStart process...exit!"<<endl;
		return;
	}
	

}//close TomographyRecTabMenu::POCAStart()


void TomographyRecTabMenu::ACFStart(QString msg){

	//## Parse string
	QJson::Parser parser;	
	bool ok;
	
	QByteArray data;
	data.append(QString(msg));
	QVariantList result = parser.parse(data, &ok).toList();
	
	if(!ok){
		cerr<<"TomographyRecTabMenu::ACFStart(). ERROR: Cannot parse JSON string message...exit!"<<endl;
		return;
	}
	
	std::string inputFileName= "";
	std::string outputFileName= "";
	std::string inputFileName_ACF= "";
	std::string outputFileName_ACF= "";
	std::string configFileName_ACF= "";
	std::string jobDirName= "";
	std::string guihostName= "";
	int guiportNo= -1;
	std::string guiKey= "";
	int scanId= 1;
	std::string containerId= "XXX";
	long int timeStamp= 0;

	
	foreach(QVariant record, result) {
    QVariantMap map = record.toMap();	
		scanId= map.value("scanId").toInt();	
		containerId= (map.value("containerId").toString()).toStdString();
		timeStamp= (long)(map.value("timeStamp").toLongLong());
    inputFileName= (map.value("inputFileName").toString()).toStdString();
		outputFileName= (map.value("outputFileName").toString()).toStdString();	
		jobDirName= (map.value("jobDirName").toString()).toStdString();	
		guihostName= (map.value("guihostName").toString()).toStdString();
		guiportNo= map.value("guiportNo").toInt();
		guiKey= (map.value("guiKey").toString()).toStdString();
	}

		 
	cout<<"TomographyRecTabMenu::ACFStart(): INFO: inputFileName="<<inputFileName<<" jobDirName="<<jobDirName<<"  outputFileName="<<outputFileName<<"  guihostName="<<guihostName<<"  guiportNo="<<guiportNo<<"  guiKey="<<guiKey<<"  scanId="<<scanId<<"  containerId="<<containerId<<"  timeStamp="<<timeStamp<<endl;
	
	//## Copy config file into the output job directory
	std::string configFileName= Gui::GUI_CONFIG_DIR + std::string("/ACFConfig.cfg");

	if(fRunExecMode==eLocalInteractive || fRunExecMode==eLocalBatch){
		
		std::string cpProcExe= Form("cp %s %s",configFileName.c_str(),jobDirName.c_str());
		Utilities::ExecSystemCommand(cpProcExe.c_str());
		cout<<"TomographyRecTabMenu::ACFStart(): INFO: cpProcExe: "<<cpProcExe<<endl;

	}
	else if(fRunExecMode==eRemoteBatch){//remote copy
	
		std::string scpProcExe= Form("scp %s %s@%s:%s",configFileName.c_str(),Gui::SEI_USERNAME.c_str(),Gui::SEI_HOSTNAME.c_str(),jobDirName.c_str());
		Utilities::ExecSystemCommand(scpProcExe.c_str());
		cout<<"TomographyRecTabMenu::ACFStart(): INFO: scpProcExe: "<<scpProcExe<<endl;

	}
	else{
		cerr<<"TomographyRecTabMenu::ACFStart(): INFO: Invalid run option ("<<fRunMode<<") ...exit!"<<endl;
		return;
	}

			
	//## Set input and output filename for ACF
	inputFileName_ACF= jobDirName + std::string("/") + Utilities::GetBaseFileNameNoPath(outputFileName) + std::string(".root");
	outputFileName_ACF= jobDirName + std::string("/ACFOutput_") + Utilities::GetBaseFileNameNoPath(inputFileName) + std::string(".root");
	configFileName_ACF= jobDirName + std::string("/ACFConfig.cfg");


	//std::string inputFileName_ACF= outputFileName.toStdString();
	//std::string outputFileName_ACF= jobDir.toStdString() + std::string("/ACFOutput_") + Utilities::GetBaseFileNameNoPath(inputFileName.toStdString()) + std::string(".root");

	cout<<"TomographyRecTabMenu::ACFStart(): INFO: inputFileName_ACF="<<inputFileName_ACF<<"  outputFileName_ACF="<<outputFileName_ACF<<"  configFileName_ACF="<<configFileName_ACF<<endl;

	//Form script name
	std::string scriptName= Gui::GUI_SCRIPT_DIR + std::string("/ACFReconstructorSubmitter.sh");
	QString exeFilePath = scriptName.c_str();
	
	QFileInfo scriptFileInfo(scriptName.c_str());
	if(!scriptFileInfo.exists()){
		cerr<<"TomographyRecTabMenu::ACFStart(): ERROR: ACF submitter script file does not exists...exit!"<<endl;
		return;
	}	

	//Form args for batch script
	std::string inputFileArg= std::string("--input=") + inputFileName_ACF;
	std::string outputFileArg= std::string("--output=") + outputFileName_ACF;
	std::string configFileArg= std::string("--config=") + configFileName_ACF;
	std::string guiHostNameArg= std::string("--guihost=") + guihostName;
	std::string guiPortNoArg= Form("--guiport=%d",guiportNo);
	std::string guiKeyArg= std::string("--guikey=") + guiKey;
	std::string scanIdArg= Form("--scanId=%d",scanId);
	std::string containerIdArg= std::string("--containerId=") + containerId;
	std::string timeStampArg= Form("--timeStamp=%ld",timeStamp);
	std::string localRunArg= "";
	if(fRunExecMode==eLocalInteractive || fRunExecMode==eLocalBatch) localRunArg= "--local";
		

	QStringList args;
  args << scriptName.c_str() << inputFileArg.c_str() << outputFileArg.c_str() << configFileArg.c_str() << guiHostNameArg.c_str() << guiPortNoArg.c_str() << guiKeyArg.c_str() << scanIdArg.c_str() << containerIdArg.c_str() << timeStampArg.c_str() << localRunArg.c_str();
		
	QString shell_path= QString::fromStdString(Gui::SHELL_DIR);

	//## Execute ACF script
	bool process_status= QProcess::startDetached(shell_path, args);	
		
	if(!process_status){
		cerr<<"TomographyRecTabMenu::ACFStart(): ERROR: Cannot start ACFStart process...exit!"<<endl;
		return;
	}

}//close TomographyRecTabMenu::ACFStart()


void TomographyRecTabMenu::FOFStart(QString msg){

	//## Parse string
	QJson::Parser parser;	
	bool ok;
	
	QByteArray data;
	data.append(QString(msg));
	QVariantList result = parser.parse(data, &ok).toList();
	
	if(!ok){
		cerr<<"TomographyRecTabMenu::FOFStart(). ERROR: Cannot parse JSON string message...exit!"<<endl;
		return;
	}
	
	std::string inputFileName= "";
	std::string outputFileName= "";
	std::string inputFileName_FOF= "";
	std::string outputFileName_FOF= "";
	std::string configFileName_FOF= "";
	std::string jobDirName= "";
	std::string guihostName= "";
	int guiportNo= -1;
	std::string guiKey= "";
	int scanId= 1;
	std::string containerId= "XXX";
	long int timeStamp= 0;

	
	foreach(QVariant record, result) {
    QVariantMap map = record.toMap();	
		scanId= map.value("scanId").toInt();	
		containerId= (map.value("containerId").toString()).toStdString();
		timeStamp= (long)(map.value("timeStamp").toLongLong());
    inputFileName= (map.value("inputFileName").toString()).toStdString();
		outputFileName= (map.value("outputFileName").toString()).toStdString();	
		jobDirName= (map.value("jobDirName").toString()).toStdString();	
		guihostName= (map.value("guihostName").toString()).toStdString();
		guiportNo= map.value("guiportNo").toInt();
		guiKey= (map.value("guiKey").toString()).toStdString();
	}

		 
	cout<<"TomographyRecTabMenu::FOFStart(): INFO: inputFileName="<<inputFileName<<" jobDirName="<<jobDirName<<"  outputFileName="<<outputFileName<<"  guihostName="<<guihostName<<"  guiportNo="<<guiportNo<<"  guiKey="<<guiKey<<"  scanId="<<scanId<<"  containerId="<<containerId<<"  timeStamp="<<timeStamp<<endl;
	
	//## Copy config file into the output job directory
	std::string configFileName= Gui::GUI_CONFIG_DIR + std::string("/FOFConfig.cfg");

	if(fRunExecMode==eLocalInteractive || fRunExecMode==eLocalBatch){
		
		std::string cpProcExe= Form("cp %s %s",configFileName.c_str(),jobDirName.c_str());
		Utilities::ExecSystemCommand(cpProcExe.c_str());
		cout<<"TomographyRecTabMenu::FOFStart(): INFO: cpProcExe: "<<cpProcExe<<endl;

	}
	else if(fRunExecMode==eRemoteBatch){//remote copy
	
		std::string scpProcExe= Form("scp %s %s@%s:%s",configFileName.c_str(),Gui::SEI_USERNAME.c_str(),Gui::SEI_HOSTNAME.c_str(),jobDirName.c_str());
		Utilities::ExecSystemCommand(scpProcExe.c_str());
		cout<<"TomographyRecTabMenu::FOFStart(): INFO: scpProcExe: "<<scpProcExe<<endl;

	}
	else{
		cerr<<"TomographyRecTabMenu::FOFStart(): INFO: Invalid run option ("<<fRunMode<<") ...exit!"<<endl;
		return;
	}

			
	//## Set input and output filename for FOF
	inputFileName_FOF= jobDirName + std::string("/") + Utilities::GetBaseFileNameNoPath(outputFileName) + std::string(".root");
	outputFileName_FOF= jobDirName + std::string("/FOFOutput_") + Utilities::GetBaseFileNameNoPath(inputFileName) + std::string(".root");
	configFileName_FOF= jobDirName + std::string("/FOFConfig.cfg");


	//std::string inputFileName_FOF= outputFileName.toStdString();
	//std::string outputFileName_FOF= jobDir.toStdString() + std::string("/FOFOutput_") + Utilities::GetBaseFileNameNoPath(inputFileName.toStdString()) + std::string(".root");

	cout<<"TomographyRecTabMenu::FOFStart(): INFO: inputFileName_FOF="<<inputFileName_FOF<<"  outputFileName_FOF="<<outputFileName_FOF<<"  configFileName_FOF="<<configFileName_FOF<<endl;

	//Form script name
	std::string scriptName= Gui::GUI_SCRIPT_DIR + std::string("/FOFReconstructorSubmitter.sh");
	QString exeFilePath = scriptName.c_str();
	
	QFileInfo scriptFileInfo(scriptName.c_str());
	if(!scriptFileInfo.exists()){
		cerr<<"TomographyRecTabMenu::FOFStart(): ERROR: FOF submitter script file does not exists...exit!"<<endl;
		return;
	}	

	//Form args for batch script
	std::string inputFileArg= std::string("--input=") + inputFileName_FOF;
	std::string outputFileArg= std::string("--output=") + outputFileName_FOF;
	std::string configFileArg= std::string("--config=") + configFileName_FOF;
	std::string guiHostNameArg= std::string("--guihost=") + guihostName;
	std::string guiPortNoArg= Form("--guiport=%d",guiportNo);
	std::string guiKeyArg= std::string("--guikey=") + guiKey;
	std::string scanIdArg= Form("--scanId=%d",scanId);
	std::string containerIdArg= std::string("--containerId=") + containerId;
	std::string timeStampArg= Form("--timeStamp=%ld",timeStamp);
	std::string localRunArg= "";
	if(fRunExecMode==eLocalInteractive || fRunExecMode==eLocalBatch) localRunArg= "--local";
		

	QStringList args;
  args << scriptName.c_str() << inputFileArg.c_str() << outputFileArg.c_str() << configFileArg.c_str() << guiHostNameArg.c_str() << guiPortNoArg.c_str() << guiKeyArg.c_str() << scanIdArg.c_str() << containerIdArg.c_str() << timeStampArg.c_str() << localRunArg.c_str();
		
	QString shell_path= QString::fromStdString(Gui::SHELL_DIR);

	//## Execute FOF script
	bool process_status= QProcess::startDetached(shell_path, args);	
		
	if(!process_status){
		cerr<<"TomographyRecTabMenu::FOFStart(): ERROR: Cannot start FOFStart process...exit!"<<endl;
		return;
	}
	

}//close TomographyRecTabMenu::FOFStart()



void TomographyRecTabMenu::viewerStart(QString msg){

	//## Parse string
	QJson::Parser parser;	
	bool ok;
	
	QByteArray data;
	data.append(QString(msg));
	QVariantList result = parser.parse(data, &ok).toList();
	
	if(!ok){
		cerr<<"TomographyRecTabMenu::viewerStart(): ERROR: Cannot parse JSON string message...exit!"<<endl;
		return;
	}
	
	std::string inputFileName= "";
	std::string outputFileName= "";
	std::string jobDirName= "";
	std::string guihostName= "";
	int guiportNo= -1;
	std::string guiKey= "";
	int scanId= 1;
	std::string containerId= "XXX";
	long int timeStamp= 0;

	
	foreach(QVariant record, result) {
    QVariantMap map = record.toMap();	
		scanId= (map.value("scanId").toString()).toInt();	
		containerId= (map.value("containerId").toString()).toStdString();
		timeStamp= (map.value("timeStamp").toString()).toLong();	
    inputFileName= (map.value("inputFileName").toString()).toStdString();
		outputFileName= (map.value("outputFileName").toString()).toStdString();	
		jobDirName= (map.value("jobDirName").toString()).toStdString();	
		guihostName= (map.value("guihostName").toString()).toStdString();
		guiportNo= (map.value("guiportNo").toString()).toInt();
		guiKey= (map.value("guiKey").toString()).toStdString();
	}

		 
	cout<<"TomographyRecTabMenu::viewerStart(): INFO: inputFileName="<<inputFileName<<" jobDirName="<<jobDirName<<"  outputFileName="<<outputFileName<<"  guihostName="<<guihostName<<"  guiportNo="<<guiportNo<<"  guiKey="<<guiKey<<"  scanId="<<scanId<<"  containerId="<<containerId<<"  timeStamp="<<timeStamp<<endl;
	
	//## Download viewer input file
	std::string scpProcExe= Form("scp %s %s@%s:%s",inputFileName.c_str(),Gui::SEI_USERNAME.c_str(),Gui::SEI_HOSTNAME.c_str(),jobDirName.c_str());
	Utilities::ExecSystemCommand(scpProcExe.c_str());
	cout<<"TomographyRecTabMenu::viewerStart(): INFO: scpProcExe: "<<scpProcExe<<endl;

	//## Start 3D viewer 
	QThread* viewerThread= new QThread();
	VolumeRenderingViewer* viewer= new VolumeRenderingViewer();
	viewer->SetInputFileName(inputFileName);
	viewer->SetOutputImageFileName(outputFileName);
  viewer->SetDataRange(pow(10,-7),pow(10,-1));

	viewerThread->start();	
	
	connect(viewerThread, SIGNAL(started()), viewer, SLOT(process()));//launch image viewer
	connect(viewer, SIGNAL(finished()), viewerThread, SLOT(quit()));//quit thread after finished signal
	connect(viewer, SIGNAL(finished()), viewer, SLOT(deleteLater()));//delete object after finished signal	

}//close TomographyRecTabMenu::viewerStart()




void TomographyRecTabMenu::Run(){

	//## Find run mode
	fRunMode= eManual;
	if( checkBox_runMode->isChecked() ){
		if(checkBox_batchMode->isChecked())
			fRunMode= eManualBatch;
		else 
			fRunMode= eManual;
	}
	else{
		if(checkBox_batchMode->isChecked())
			fRunMode= eAutoBatch;
		else 
			fRunMode= eAuto;
	}

	fRunExecMode= eLocalInteractive;	
	std::string runExecChoice= (comboBox_runExecMode->currentText()).toStdString();
	if(runExecChoice=="local-interactive" ){
		fRunExecMode= eLocalInteractive;
	}
	else if(runExecChoice=="local-batch"){
		fRunExecMode= eLocalBatch;
	}
	else if(runExecChoice=="remote-batch"){
		fRunExecMode= eRemoteBatch;
	}

	cout<<"TomographyRecTabMenu::Run(): INFO: RunMode: "<<fRunMode<<"  RunExecMode: "<<fRunExecMode<<endl;	


	//### Create a TCP server listening for communications from the reconstruction algorithms
	if(!fIsServerRunning){
		cout<<"TomographyRecTabMenu::Run(): INFO: Creating tcp server..."<<endl;
		server= new QTcpServer(this);
		server->listen(QHostAddress::Any, Gui::SOCK_LISTEN_PORT);
		fIsServerRunning= server->isListening();
		connect(server, SIGNAL(newConnection()),this, SLOT(acceptConnection()));
	}

	//### Start task if in manual mode
	if(fRunMode == eManual) {
		if(fRunExecMode==eLocalInteractive){
			ManualRunTomographyReconstruction();
		}
		else if(fRunExecMode==eLocalBatch || fRunExecMode==eRemoteBatch){
			ManualBatchRunTomographyReconstruction();
		}
	}


}//close TomographyRecTabMenu::Run()





void TomographyRecTabMenu::ManualRunTomographyReconstruction(){


	fCurrentScanIndex= 0;

	//## Set run config
	std::string hostName= Utilities::GetIpAddress();//Utilities::GetHostName();
	//std::string hostName= "localhost";//REMOVE!!!
	std::string userName= Utilities::GetUserName();
	int portNo= Gui::GUI_PORT;

	std::string guihostName= Utilities::GetIpAddress();//Utilities::GetHostName();
	int guiportNo= Gui::SOCK_LISTEN_PORT;
	std::string guiKey= Gui::GUI_KEY;

	int retryAttempts= 3; 
	bool isLocalDownload= true;
	int scanId= 1;
	std::string containerId= Utilities::GetUUID();
	long int timeStamp= Utilities::GetTimeStamp();
	
	std::string inputFileName= Gui::fConfigParser->fInputFileName;
	std::string inputFileBaseName= Utilities::GetBaseFileName(inputFileName);
	std::string inputFileBaseNameNoExt= Utilities::GetBaseFileNameNoExt(inputFileName);
	std::string jobDirName= Gui::GUI_DATA_DIR + Form("/data-%s_SCAN%d_ID%s_TIME%ld",inputFileBaseNameNoExt.c_str(),scanId,containerId.c_str(),timeStamp);

	std::string outputFileName= inputFileBaseName;

	cout<<"TomographyRecTabMenu::ManualRunTomographyReconstruction(): jobDirName= "<<jobDirName<<"  inputFileName="<<inputFileName<<"  outputFileName="<<outputFileName<<endl;

	std::string inputFileName_POCA= "";
	std::string outputFileName_POCA= "";
	std::string inputFileName_ACF= "";
	std::string outputFileName_ACF= "";
	std::string inputFileName_FOF= "";
	std::string outputFileName_FOF= "";
	std::string inputFileName_EMML= "";
	std::string outputFileName_EMML= "";

	//## Make the output job directory
	QString mkdirProcExe= QString("mkdir ") + QString::fromStdString(jobDirName); 
	Utilities::ExecSystemCommand((mkdirProcExe.toStdString()).c_str());

	//## Define and start local download thread
	downloaderThread= new QThread();
	downloaderThreadObj= new DownloaderThreadObj;
	downloaderThreadObj->SetScanId(scanId);
	downloaderThreadObj->SetContainerId(containerId);
	downloaderThreadObj->SetTimeStamp(timeStamp);		
	downloaderThreadObj->SetInputFileName(inputFileName);			
	downloaderThreadObj->SetOutputFileName(outputFileName);
	downloaderThreadObj->SetDestinationPath(jobDirName);
	downloaderThreadObj->SetRemoteHostName(hostName);
	downloaderThreadObj->SetRemoteUserName(userName);	
	downloaderThreadObj->SetRemotePort(portNo);
	downloaderThreadObj->SetGuiHostName(guihostName);
	downloaderThreadObj->SetGuiPort(guiportNo);
	downloaderThreadObj->SetGuiKey(guiKey);
	downloaderThreadObj->SetMaxRetryAttempts(retryAttempts);
	downloaderThreadObj->SetLocalDownload(isLocalDownload);

	downloaderThreadObj->moveToThread(downloaderThread);
	downloaderThread->start();

	connect(downloaderThread, SIGNAL(started()), this, SLOT(SetBusyDownloadStatus()));//turn status light red blinking
	connect(downloaderThread, SIGNAL(started()), downloaderThreadObj, SLOT(process()));//launch download task
	connect(downloaderThreadObj, SIGNAL(finished()), downloaderThread, SLOT(quit()));//quit thread after finished signal
	connect(downloaderThreadObj, SIGNAL(finished()), downloaderThreadObj, SLOT(deleteLater()));//delete object after finished signal
	connect(downloaderThread, SIGNAL(finished()), downloaderThread, SLOT(deleteLater()));//delete thread after finished signal
	connect(downloaderThread, SIGNAL(finished()), this, SLOT(SetCompleteDownloadStatus()));//turn status light green
	connect(downloaderThreadObj, SIGNAL(error()), this, SLOT(SetFailureDownloadStatus()));//turn status light green
	connect(downloaderThreadObj, SIGNAL(error()), downloaderThread, SLOT(quit()));//quit thread after error signal
	connect(downloaderThreadObj, SIGNAL(error()), downloaderThreadObj, SLOT(deleteLater()));//delete object after error signal
	connect(downloaderThreadObj, SIGNAL(error()), downloaderThread, SLOT(deleteLater()));//delete thread after error signal

	connect(downloaderThreadObj, SIGNAL(downloadProgress(int)), progressBarList_fileStatus[fCurrentScanIndex], SLOT(setValue(int)));//bar progress status	
	connect(downloaderThreadObj, SIGNAL(statusMessage(QString)), labelList_statusBar_fileStatus[fCurrentScanIndex], SLOT(setText(QString)) );
	
	
	//## Define and start local POCA thread
	if(Gui::fConfigParser->fUsePOCAAlgorithm) {
		cout<<"TomographyRecTabMenu::ManualRunTomographyReconstruction(): INFO: Define and start local POCA thread ..."<<endl;
		
		inputFileName_POCA= jobDirName + std::string("/") + outputFileName;
		outputFileName_POCA= jobDirName + std::string("/POCAOutput_") + Utilities::GetBaseFileNameNoPath(outputFileName) + std::string(".root");
		fPOCAOutputFileNameList[fCurrentScanIndex]= outputFileName_POCA;

		cout<<"TomographyRecTabMenu::ManualRunTomographyReconstruction(): INFO: inputFileName_POCA="<<inputFileName_POCA<<"  outputFileName_POCA="<<outputFileName_POCA<<endl;

		pocaRecoThread= new QThread();
		pocaRecoThreadObj= new POCAThreadObj;
		pocaRecoThreadObj->SetScanId(scanId);
		pocaRecoThreadObj->SetContainerId(containerId);
		pocaRecoThreadObj->SetTimeStamp(timeStamp);		
		pocaRecoThreadObj->SetInputFileName(inputFileName_POCA);			
		pocaRecoThreadObj->SetOutputFileName(outputFileName_POCA);
		pocaRecoThreadObj->SetGuiHostName(guihostName);
		pocaRecoThreadObj->SetGuiPort(guiportNo);
		pocaRecoThreadObj->SetGuiKey(guiKey);
		
		pocaRecoThreadObj->moveToThread(pocaRecoThread);
	
		connect(downloaderThreadObj, SIGNAL(finished()), pocaRecoThread, SLOT(start()));//connect end download with POCA reco start
		
		connect(pocaRecoThread, SIGNAL(started()), this, SLOT(SetBusyPOCARecoStatus()));//turn status light red blinking
		connect(pocaRecoThread, SIGNAL(started()), this, SLOT(EnablePOCAStopButton()));//launch image processing
		connect(pocaRecoThread, SIGNAL(started()), pocaRecoThreadObj, SLOT(process()));//launch image processing
		connect(pocaRecoThreadObj, SIGNAL(finished()), pocaRecoThread, SLOT(quit()));//quit thread after finished signal
		connect(pocaRecoThreadObj, SIGNAL(finished()), pocaRecoThreadObj, SLOT(deleteLater()));//delete object after finished signal
		connect(pocaRecoThread, SIGNAL(finished()), pocaRecoThread, SLOT(deleteLater()));//delete thread after finished signal
		connect(pocaRecoThreadObj, SIGNAL(finished()), this, SLOT(SetCompletePOCARecoStatus()));//turn status light green
		connect(pocaRecoThreadObj, SIGNAL(finished()), this, SLOT(EnablePOCAViewerButton()));//enable viewer button

		connect(pocaRecoThreadObj, SIGNAL(error()), pocaRecoThread, SLOT(quit()));//quit thread after error signal
		connect(pocaRecoThreadObj, SIGNAL(error()), pocaRecoThreadObj, SLOT(deleteLater()));//delete object after error signal
		connect(pocaRecoThreadObj, SIGNAL(error()), this, SLOT(SetFailurePOCARecoStatus()));//turn status light red

		connect(pocaRecoThreadObj, SIGNAL(statusProgress(int)), progressBarList_POCARecoStatus[fCurrentScanIndex], SLOT(setValue(int)));//bar progress status
		connect(pocaRecoThreadObj, SIGNAL(statusMessage(QString)), labelList_statusBar_POCARecoStatus[fCurrentScanIndex], SLOT(setText(QString)) );
		connect(pocaRecoThreadObj, SIGNAL(alarm()), this, SLOT(displayAlarm_POCAReco()) );
		connect(pocaRecoThreadObj, SIGNAL(alarmStatusMessage(QString)), this, SLOT(sendAlarm_POCAReco(QString)) );
		connect(pocaRecoThreadObj, SIGNAL(alarm()), this, SLOT(playAlarmSound_POCAReco()) );

		
		//connect(pushButtonList_POCAVolRendering[fCurrentScanIndex], SIGNAL(clicked()), this, SLOT(StartPOCAVolRenderingViewer()) );//viewer 3D
		

	
		//## Define and start local ACF thread
		if(Gui::fConfigParser->fUseACFAlgorithm) {
	
			cout<<"TomographyRecTabMenu::ManualRunTomographyReconstruction(): INFO: Define and start local ACF thread ..."<<endl;
	
			inputFileName_ACF= outputFileName_POCA;
			outputFileName_ACF= jobDirName + std::string("/ACFOutput_") + Utilities::GetBaseFileNameNoPath(outputFileName) + std::string(".root");
			fACFOutputFileNameList[fCurrentScanIndex]= outputFileName_ACF;

			cout<<"TomographyRecTabMenu::ManualRunTomographyReconstruction(): INFO: inputFileName_ACF="<<inputFileName_ACF<<"  outputFileName_ACF="<<outputFileName_ACF<<endl;

			acfAnalysisThread= new QThread();
			acfAnalysisThreadObj= new ACFAnalysisThreadObj;
			
			acfAnalysisThreadObj->SetScanId(scanId);
			acfAnalysisThreadObj->SetContainerId(containerId);
			acfAnalysisThreadObj->SetTimeStamp(timeStamp);		
			acfAnalysisThreadObj->SetInputFileName(inputFileName_ACF);			
			acfAnalysisThreadObj->SetOutputFileName(outputFileName_ACF);
			acfAnalysisThreadObj->SetGuiHostName(guihostName);
			acfAnalysisThreadObj->SetGuiPort(guiportNo);
			acfAnalysisThreadObj->SetGuiKey(guiKey);	

			acfAnalysisThreadObj->moveToThread(acfAnalysisThread);
		
			connect(pocaRecoThreadObj, SIGNAL(finished()), acfAnalysisThread, SLOT(start()));//connect end POCA with ACF reco start

			connect(acfAnalysisThread, SIGNAL(started()), this, SLOT(SetBusyACFRecoStatus()));//turn status light red blinking
			connect(acfAnalysisThread, SIGNAL(started()), this, SLOT(EnableACFStopButton()));//launch image processing
			connect(acfAnalysisThread, SIGNAL(started()), acfAnalysisThreadObj, SLOT(process()));//launch image processing
			connect(acfAnalysisThreadObj, SIGNAL(finished()), acfAnalysisThread, SLOT(quit()));//quit thread after finished signal
			connect(acfAnalysisThreadObj, SIGNAL(finished()), acfAnalysisThreadObj, SLOT(deleteLater()));//delete object after finished signal
			connect(acfAnalysisThread, SIGNAL(finished()), acfAnalysisThread, SLOT(deleteLater()));//delete thread after finished signal
			connect(acfAnalysisThreadObj, SIGNAL(finished()), this, SLOT(SetCompleteACFRecoStatus()));//turn status light green
			connect(acfAnalysisThreadObj, SIGNAL(finished()), this, SLOT(EnableACFRecoViewerButton()));//enable viewer button

			connect(acfAnalysisThreadObj, SIGNAL(statusProgress(int)), progressBarList_ACFRecoStatus[fCurrentScanIndex], SLOT(setValue(int)));//bar progress status
			connect(acfAnalysisThreadObj, SIGNAL(statusMessage(QString)), labelList_statusBar_ACFRecoStatus[fCurrentScanIndex], SLOT(setText(QString)) );
			connect(acfAnalysisThreadObj, SIGNAL(alarm()), this, SLOT(displayAlarm_ACFReco()) );
			connect(acfAnalysisThreadObj, SIGNAL(alarmStatusMessage(QString)), this, SLOT(sendAlarm_ACFReco(QString)) );
			connect(acfAnalysisThreadObj, SIGNAL(alarm()), this, SLOT(playAlarmSound_ACFReco()) );

			connect(pushButtonList_ACFRecoStatus[fCurrentScanIndex], SIGNAL(clicked()), this, SLOT(StartACFRecoViewer()) );
	
		}//close if use ACF


		//## Define and start local FOF thread
		if(Gui::fConfigParser->fUseClusteringAlgorithm) {
			cout<<"TomographyRecTabMenu::ManualRunTomographyReconstruction(): INFO: Define and start local FOF thread ..."<<endl;
	
			inputFileName_FOF= outputFileName_POCA;
			outputFileName_FOF= jobDirName + std::string("/FOFOutput_") + Utilities::GetBaseFileNameNoPath(outputFileName) + std::string(".root");
			fClusteringOutputFileNameList[fCurrentScanIndex]= outputFileName_FOF;

			cout<<"TomographyRecTabMenu::ManualRunTomographyReconstruction(): INFO: inputFileName_FOF="<<inputFileName_FOF<<"  outputFileName_FOF="<<outputFileName_FOF<<endl;

			fofThread= new QThread();
			fofThreadObj= new FOFThreadObj;	
			fofThreadObj->SetScanId(scanId);
			fofThreadObj->SetContainerId(containerId);
			fofThreadObj->SetTimeStamp(timeStamp);		
			fofThreadObj->SetInputFileName(inputFileName_FOF);			
			fofThreadObj->SetOutputFileName(outputFileName_FOF);
			fofThreadObj->SetGuiHostName(guihostName);
			fofThreadObj->SetGuiPort(guiportNo);
			fofThreadObj->SetGuiKey(guiKey);	

			fofThreadObj->moveToThread(fofThread);

			connect(pocaRecoThreadObj, SIGNAL(finished()), fofThread, SLOT(start()));//connect end POCA with FOF reco start

			connect(fofThread, SIGNAL(started()), this, SLOT(SetBusyClusteringRecoStatus()));//turn status light red blinking
			connect(fofThread, SIGNAL(started()), this, SLOT(EnableClusteringStopButton()));//launch image processing
			connect(fofThread, SIGNAL(started()), fofThreadObj, SLOT(process()));//launch image processing
			connect(fofThreadObj, SIGNAL(finished()), fofThread, SLOT(quit()));//quit thread after finished signal
			connect(fofThreadObj, SIGNAL(finished()), fofThreadObj, SLOT(deleteLater()));//delete object after finished signal
			connect(fofThread, SIGNAL(finished()), fofThread, SLOT(deleteLater()));//delete thread after finished signal
			connect(fofThreadObj, SIGNAL(finished()), this, SLOT(SetCompleteClusteringRecoStatus()));//turn status light green
			connect(fofThreadObj, SIGNAL(finished()), this, SLOT(EnableClusteringRecoViewerButton()));//enable viewer button

			connect(fofThreadObj, SIGNAL(statusProgress(int)), progressBarList_ClusteringRecoStatus[fCurrentScanIndex], SLOT(setValue(int)));//bar progress status
			connect(fofThreadObj, SIGNAL(statusMessage(QString)), labelList_statusBar_ClusteringRecoStatus[fCurrentScanIndex], SLOT(setText(QString)) );
			connect(fofThreadObj, SIGNAL(alarm()), this, SLOT(displayAlarm_ClusteringReco()) );
			connect(fofThreadObj, SIGNAL(alarmStatusMessage(QString)), this, SLOT(sendAlarm_ClusteringReco(QString)) );
			connect(fofThreadObj, SIGNAL(alarm()), this, SLOT(playAlarmSound_ClusteringReco()) );

			connect(pushButtonList_ClusteringVolRendering[fCurrentScanIndex], SIGNAL(clicked()), this, SLOT(StartClusteringVolRenderingViewer()) );
			
		}//close if use FOF


	}//close if use POCA


	

	//## Create EM reco thread
	if(Gui::fConfigParser->fUseEMLLAlgorithm) {
		cout<<"TomographyRecTabMenu::ManualRunTomographyReconstruction(): INFO: Define and start local EMML thread ..."<<endl;
	
		inputFileName_EMML= jobDirName + std::string("/") + outputFileName;
		outputFileName_EMML= jobDirName + std::string("/EMMLOutput_") + Utilities::GetBaseFileNameNoPath(outputFileName) + std::string(".root");
		fEMLLOutputFileNameList[fCurrentScanIndex]= outputFileName_EMML;

		cout<<"TomographyRecTabMenu::ManualRunTomographyReconstruction(): INFO: inputFileName_EMML="<<inputFileName_EMML<<"  outputFileName_EMML="<<outputFileName_EMML<<endl;


		emTomographyRecoThread= new QThread();	
		emTomographyRecoThreadObj= new EMTomographyRecoThreadObj;
		emTomographyRecoThreadObj->SetScanId(scanId);
		emTomographyRecoThreadObj->SetContainerId(containerId);
		emTomographyRecoThreadObj->SetTimeStamp(timeStamp);		
		emTomographyRecoThreadObj->SetInputFileName(inputFileName_EMML);			
		emTomographyRecoThreadObj->SetOutputFileName(outputFileName_EMML);
		emTomographyRecoThreadObj->SetClusteringFileName(outputFileName_FOF);
		emTomographyRecoThreadObj->SetGuiHostName(guihostName);
		emTomographyRecoThreadObj->SetGuiPort(guiportNo);
		emTomographyRecoThreadObj->SetGuiKey(guiKey);	

		emTomographyRecoThreadObj->moveToThread(emTomographyRecoThread);
	
		if(Gui::fConfigParser->fEMLLWaitClusteringOutput){
			if(Gui::fConfigParser->fUseClusteringAlgorithm){
				connect(fofThreadObj, SIGNAL(finished()), emTomographyRecoThread, SLOT(start()));//connect end FOF with EM reco start
			}
			else{
				cerr<<"TomographyRecTabMenu::ManualRunTomographyReconstruction(): WARNING: You must activate FOF when EMLLWaitClusteringOutput is selected...cannot perform EMML analysis!"<<endl;
			}
		}
		else{
			connect(downloaderThreadObj, SIGNAL(finished()), emTomographyRecoThread, SLOT(start()));//connect end download with EM reco start
		}
		
		
		connect(emTomographyRecoThread, SIGNAL(started()), this, SLOT(SetBusyEMTomographyRecoStatus()));//turn status light red blinking
		//connect(emTomographyRecoThread, SIGNAL(started()), this, SLOT(EnableLLStopButton()));//launch image processing	
		
		//for(int i=0;i<MAXNSCANS;i++) connect(stopThreadButtonList_LLRecoStatus[i], SIGNAL(clicked()), emTomographyRecoThread, SLOT(stop()));//emit stopped signal in thread
		//connect(emTomographyRecoThread, SIGNAL(isstopped()), emTomographyRecoThreadObj, SLOT(stop()));//catch stop signal and break processing
			
		connect(emTomographyRecoThread, SIGNAL(started()), emTomographyRecoThreadObj, SLOT(process()));//launch image processing
		
		connect(emTomographyRecoThreadObj, SIGNAL(finished()), emTomographyRecoThread, SLOT(quit()));//quit thread after finished signal
		connect(emTomographyRecoThreadObj, SIGNAL(finished()), emTomographyRecoThreadObj, SLOT(deleteLater()));//delete object after finished signal
		connect(emTomographyRecoThread, SIGNAL(finished()), emTomographyRecoThread, SLOT(deleteLater()));//delete thread after finished signal
		connect(emTomographyRecoThreadObj, SIGNAL(finished()), this, SLOT(SetCompleteEMTomographyRecoStatus()));//turn status light green
		connect(emTomographyRecoThreadObj, SIGNAL(finished()), this, SLOT(EnableLLRecoViewerButton()));//enable viewer button
		connect(emTomographyRecoThreadObj, SIGNAL(error()), emTomographyRecoThread, SLOT(quit()));//quit thread after error signal
		connect(emTomographyRecoThreadObj, SIGNAL(error()), emTomographyRecoThreadObj, SLOT(deleteLater()));//delete object after error signal
		connect(emTomographyRecoThreadObj, SIGNAL(error()), this, SLOT(SetFailureEMTomographyRecoStatus()));//turn status light red

		connect(emTomographyRecoThreadObj, SIGNAL(statusProgress(int)), progressBarList_LLRecoStatus[fCurrentScanIndex], SLOT(setValue(int)));//bar progress status
		connect(emTomographyRecoThreadObj, SIGNAL(statusMessage(QString)), labelList_statusBar_LLRecoStatus[fCurrentScanIndex], SLOT(setText(QString)) );
		connect(emTomographyRecoThreadObj, SIGNAL(alarm()), this, SLOT(displayAlarm_LLReco()) );
		connect(emTomographyRecoThreadObj, SIGNAL(alarmStatusMessage(QString)), this, SLOT(sendAlarm_LLReco(QString)) );
		connect(emTomographyRecoThreadObj, SIGNAL(alarm()), this, SLOT(playAlarmSound_LLReco()) );

		connect(pushButtonList_LLVolRendering[fCurrentScanIndex], SIGNAL(clicked()), this, SLOT(StartLLVolRenderingViewer()) );
			
	}//close if use LL algorithm
	
	

}//close TomographyRecTabMenu::ManualRunTomographyReconstruction()




void TomographyRecTabMenu::ManualBatchRunTomographyReconstruction(){

	//## Set run config
	std::string hostName= Utilities::GetIpAddress();//Utilities::GetHostName();
	//std::string hostName= "localhost";//REMOVE!!!
	std::string userName= Utilities::GetUserName();
	int portNo= Gui::GUI_PORT;

	std::string guihostName= Utilities::GetIpAddress();//Utilities::GetHostName();
	int guiportNo= Gui::SOCK_LISTEN_PORT;
	std::string guiKey= Gui::GUI_KEY;

	int retryAttempts= 3; 
	bool isLocalDownload= true;
	int scanId= 1;
	std::string containerId= Utilities::GetUUID();
	long int timeStamp= Utilities::GetTimeStamp();
	
	std::string inputFileName= Gui::fConfigParser->fInputFileName;
	std::string inputFileBaseName= Utilities::GetBaseFileName(inputFileName);
	std::string inputFileBaseNameNoExt= Utilities::GetBaseFileNameNoExt(inputFileName);
	std::string jobDirName= Gui::GUI_DATA_DIR + Form("/data-%s_SCAN%d_ID%s_TIME%ld",inputFileBaseNameNoExt.c_str(),scanId,containerId.c_str(),timeStamp);

	std::string outputFileName= inputFileBaseName;

	cout<<"TomographyRecTabMenu::ManualBatchRunTomographyReconstruction(): jobDirName= "<<jobDirName<<"  inputFileName="<<inputFileName<<"  outputFileName="<<outputFileName<<endl;

	
	//## Make the output job directory
	if(fRunExecMode==eRemoteBatch){
		QString mkdirProcExe= QString("mkdir ") + QString::fromStdString(jobDirName); 
		Utilities::ExecSystemCommand((mkdirProcExe.toStdString()).c_str());
	}

	//## Form download message
	QVariantList downloadConfigMsg;
	QVariantMap downloadConfigMsgField;

	downloadConfigMsgField.insert("scanId",QVariant(scanId));
	downloadConfigMsgField.insert("containerId",QVariant(containerId.c_str()));
	downloadConfigMsgField.insert("timeStamp",QVariant((long long)(timeStamp)));
	downloadConfigMsgField.insert("inputFileName", QVariant(inputFileName.c_str()) );
	downloadConfigMsgField.insert("outputFileName", QVariant(outputFileName.c_str()) );
	downloadConfigMsgField.insert("jobDirName",QVariant(jobDirName.c_str()));
	downloadConfigMsgField.insert("hostName",QVariant(hostName.c_str()));
	downloadConfigMsgField.insert("userName",QVariant(userName.c_str()));
	downloadConfigMsgField.insert("portNo",QVariant(portNo));
	downloadConfigMsgField.insert("guihostName",QVariant(guihostName.c_str()));
	downloadConfigMsgField.insert("guiportNo",QVariant(guiportNo));
	downloadConfigMsgField.insert("guiKey",QVariant(guiKey.c_str()));
	downloadConfigMsgField.insert("retryAttempts",QVariant(retryAttempts));
	downloadConfigMsgField.insert("isLocalDownload",QVariant(isLocalDownload));
		
	downloadConfigMsg << downloadConfigMsgField;

	QJson::Serializer serializer;	
	serializer.setIndentMode(QJson::IndentCompact);
	bool ok;
	QByteArray msg= serializer.serialize(downloadConfigMsg,&ok);

	cout<<"TomographyRecTabMenu::ManualBatchRunTomographyReconstruction(): INFO: Download config message: "<<qPrintable(QString(msg))<<endl;
		
	//## Define and start local download thread
	//## Other tasks are started triggered by download finish 
	if(ok){
		emit downloadStartSig(QString(msg));
	}
	else{
		cerr<<"TomographyRecTabMenu::ManualBatchRunTomographyReconstruction(): ERROR: Cannot serialize json message to be sent to download task...exit!"<<endl;
		return;
	}

}//close TomographyRecTabMenu::ManualBatchRunTomographyReconstruction()




void TomographyRecTabMenu::acceptConnection(){

	tcpSocket = server->nextPendingConnection();
  connect(tcpSocket, SIGNAL(readyRead()),this, SLOT(SocketReadyReadAction()));

}//close TomographyRecTabMenu::acceptConnection()



void TomographyRecTabMenu::SocketReadyReadAction() {

	//## Get received data info
	qint64 nbytes= tcpSocket->bytesAvailable();
	QByteArray dataRecv= tcpSocket->readAll();
	//cout<<"nbytes="<<nbytes<<"  dataRecv="<<qPrintable(dataRecv)<<endl;

	//## Append received data to buffer
  fBuffer.append(dataRecv);

	//## Bufferize received data until receiving end of transmission pattern
  BufferizeSocketData();

}//close TomographyRecTabMenu::SocketReadyReadAction()
 

void TomographyRecTabMenu::BufferizeSocketData(){

	//## Loop until receiving the end of transmission pattern
  forever {
    if(fBuffer.size()<1) return;
    
		int pos= fBuffer.indexOf("#END#");
		if(pos==-1) return;
		QByteArray record = fBuffer.left(pos);
		int buffer_size= fBuffer.size();
		//cout<<"pos="<<pos<<"  record= "<<qPrintable(record)<<"  buffer="<<qPrintable(fBuffer)<<" bufsize="<<buffer_size<<endl;

    ProcessSocketData(record);
		//fBuffer.clear();
		fBuffer.remove(0,pos+5);
		//cout<<"buffer="<<qPrintable(fBuffer)<<endl;
  }
 
}//close TomographyRecTabMenu::BufferizeSocketData()


void TomographyRecTabMenu::sendAlarmMessage(QString message){

	cout<<"TomographyRecTabMenu::sendAlarmMessage(): INFO: message= "<<qPrintable(message)<<endl;
  client->write((message.toStdString()).c_str(), 13);

}//close TomographyRecTabMenu::sendAlarmMessage()



void TomographyRecTabMenu::logHandler(QString data){

	cout<<"TomographyRecTabMenu::logHandler(): INFO: Start logging actions..."<<endl;

	
	//## Parse JSON message
	QJson::Parser parser;
	bool ok;
	QByteArray arraydata;
	arraydata.append(data);
	QVariantList result = parser.parse(arraydata, &ok).toList();
	
	if(!ok){
		cerr<<"TomographyRecTabMenu::logHandler(). ERROR: Cannot parse JSON string message...exit!"<<endl;
		return;
	}
	
	QString task = "";
	QString command = "";
	QString jobDir = "";
  QString filename = "";
	QString inputFileName = "";
	QString outputFileName = "";
	QString path = "";
	QString host = "";
	QString user = "";
	QString id = "XXX";
  int nscan = 0;
  int status = 0;
	QString logMessage = "";
	int progress = 0;
	long int timeStamp= 0;
	QString guiKey = "";
	QString guiHostName = "";
	int guiPort = -1;

	foreach(QVariant record, result) {
    QVariantMap map = record.toMap();	
		task= map.value("task").toString();
		command= map.value("command").toString();
		jobDir= map.value("jobDirName").toString();
		filename= map.value("filename").toString();
		inputFileName= map.value("inputFileName").toString();
		outputFileName= map.value("outputFileName").toString();
		path= map.value("path").toString();
		host= map.value("host").toString();
		user= map.value("user").toString();
		id= map.value("containerId").toString();
		nscan= map.value("scanId").toInt();	
		status= map.value("status").toInt();	
		logMessage= map.value("log").toString();
		progress= map.value("progress").toInt();	
		timeStamp= map.value("timeStamp").toLongLong();	
		guiKey= map.value("guiKey").toString();
		guiHostName= map.value("guiHostName").toString();
		guiPort= map.value("guiPort").toInt();
	}//end foreach loop


	//## Check current GUI key with job gui key
	if(guiKey.toStdString() != Gui::GUI_KEY){
		cout<<"TomographyRecTabMenu::logHandler(): WARNING: Task gui key is different from current gui key...do not update GUI!"<<endl;
		return;	
	}


	//## Check current scan id
	if(nscan<1 || nscan>MAXNSCANS){
		cerr<<"TomographyRecTabMenu::logHandler(): ERROR: Current scan index is not in range [0,3]...exit!"<<endl;
		return;
	}


	//########  DOWNLOAD TASK ###############
	if(task=="DOWNLOADER"){

		
		if(status==DownloaderThreadObj::eRunning){//running actions
			imageLabelList_fileStatus[nscan-1]->setMovie(movieList_fileStatus_Red[nscan-1]);
			
			if( movieList_fileStatus_Red[nscan-1]->state() != QMovie::Running ){
				movieList_fileStatus_Red[nscan-1]->start();
			}

		}
		else if(status==DownloaderThreadObj::eFinished){//finish actions
			
			imageLabelList_fileStatus[nscan-1]->setMovie(movieList_fileStatus_Green[nscan-1]);
			movieList_fileStatus_Green[nscan-1]->start();
			movieList_fileStatus_Green[nscan-1]->stop();

			//## Trigger POCA start
			if(Gui::fConfigParser->fUsePOCAAlgorithm){
				emit POCAStartSig(data);
			}

			//## Trigger EMML start
			if(Gui::fConfigParser->fUseEMLLAlgorithm && !Gui::fConfigParser->fEMLLWaitClusteringOutput){
				emit EMMLStartSig(data);
			}

		}
		else if(status==DownloaderThreadObj::eFailure){//failure actions
			imageLabelList_fileStatus[nscan-1]->setMovie(movieList_fileStatus_Red[nscan-1]);
			movieList_fileStatus_Red[nscan-1]->start();
			movieList_fileStatus_Red[nscan-1]->stop();
		}
		
		//Set progress bar
		progressBarList_fileStatus[nscan-1]->setValue(progress);

		//Set status message 
		labelList_statusBar_fileStatus[nscan-1]->setText(logMessage);

	}//close downloader task


	//########  POCA TASK ###############
	else if(task=="POCA"){

		//Set run status led
		if(status==POCAThreadObj::eRunning){
			imageLabelList_POCARecoStatus[nscan-1]->setMovie(movieList_POCARecoStatus_Red[nscan-1]);
			
			if( movieList_POCARecoStatus_Red[nscan-1]->state() != QMovie::Running ){
				movieList_POCARecoStatus_Red[nscan-1]->start();
			}

		}
		else if(status==POCAThreadObj::eFinished){
			
			imageLabelList_POCARecoStatus[nscan-1]->setMovie(movieList_POCARecoStatus_Green[nscan-1]);
			movieList_POCARecoStatus_Green[nscan-1]->start();
			movieList_POCARecoStatus_Green[nscan-1]->stop();

			//## Trigger FOF
			if(Gui::fConfigParser->fUseClusteringAlgorithm){
				emit FOFStartSig(data);
			}

			//## Trigger ACF
			if(Gui::fConfigParser->fUseACFAlgorithm){
				emit ACFStartSig(data);
			}


			//Enable 3D viewer button
			pushButtonList_POCARecoStatus[nscan-1]->setEnabled(true);
			pushButtonList_POCAVolRendering[nscan-1]->setEnabled(true);
	
			//Local download of output reco file
			std::string viewerInputFileName= outputFileName.toStdString();

			if(fRunMode==eAuto || fRunMode==eAutoBatch){//remote copy
				std::string scpProcExe= Form("scp %s@%s:%s",Gui::SEI_USERNAME.c_str(),Gui::SEI_HOSTNAME.c_str(),(outputFileName.toStdString()).c_str(),Gui::GUI_DATA_DIR.c_str());
				Utilities::ExecSystemCommand(scpProcExe.c_str());
				cout<<"TomographyRecTabMenu::logHandler(): INFO: scpProcExe: "<<scpProcExe<<endl;

				viewerInputFileName= Gui::GUI_DATA_DIR + std::string("/") + Utilities::GetBaseFileName(outputFileName.toStdString());			
			}
			

			//Set input filename for 3D viewer
			fPOCAOutputFileNameList[nscan-1]= viewerInputFileName;
			
			
		}
		else if(status==POCAThreadObj::eFailure){
			
			imageLabelList_POCARecoStatus[nscan-1]->setMovie(movieList_POCARecoStatus_Red[nscan-1]);
			movieList_POCARecoStatus_Red[nscan-1]->start();
			movieList_POCARecoStatus_Red[nscan-1]->stop();
		}
		
		//Set progress bar
		progressBarList_POCARecoStatus[nscan-1]->setValue(progress);
		
		//Set status message 
		labelList_statusBar_POCARecoStatus[nscan-1]->setText(logMessage);
		
	}//close POCA task


	//########  FOF TASK ###############
	else if(task=="FOF"){

		//Set run status led
		if(status==FOFThreadObj::eRunning){
			imageLabelList_ClusteringRecoStatus[nscan-1]->setMovie(movieList_ClusteringRecoStatus_Red[nscan-1]);
			
			if( movieList_ClusteringRecoStatus_Red[nscan-1]->state() != QMovie::Running ){
				movieList_ClusteringRecoStatus_Red[nscan-1]->start();
			}

		}
		else if(status==FOFThreadObj::eFinished){
			
			imageLabelList_ClusteringRecoStatus[nscan-1]->setMovie(movieList_ClusteringRecoStatus_Green[nscan-1]);
			movieList_ClusteringRecoStatus_Green[nscan-1]->start();
			movieList_ClusteringRecoStatus_Green[nscan-1]->stop();

			//## Trigger EM start
			if(Gui::fConfigParser->fUseEMLLAlgorithm && Gui::fConfigParser->fEMLLWaitClusteringOutput){
				emit EMMLStart(data);
			}

			//Enable 3D viewer button
			pushButtonList_ClusteringRecoStatus[nscan-1]->setEnabled(true);
			pushButtonList_ClusteringVolRendering[nscan-1]->setEnabled(true);
	
			//Local download of output reco file
			std::string viewerInputFileName= outputFileName.toStdString();

			if(fRunMode==eAuto || fRunMode==eAutoBatch){//remote copy
				std::string scpProcExe= Form("scp %s@%s:%s",Gui::SEI_USERNAME.c_str(),Gui::SEI_HOSTNAME.c_str(),(outputFileName.toStdString()).c_str(),Gui::GUI_DATA_DIR.c_str());
				Utilities::ExecSystemCommand(scpProcExe.c_str());
				cout<<"TomographyRecTabMenu::logHandler(): INFO: scpProcExe: "<<scpProcExe<<endl;

				viewerInputFileName= Gui::GUI_DATA_DIR + std::string("/") + Utilities::GetBaseFileName(outputFileName.toStdString());			
			}
			

			//Set input filename for 3D viewer
			fClusteringOutputFileNameList[nscan-1]= viewerInputFileName;
			
			
		}
		else if(status==FOFThreadObj::eFailure){
			
			imageLabelList_ClusteringRecoStatus[nscan-1]->setMovie(movieList_ClusteringRecoStatus_Red[nscan-1]);
			movieList_ClusteringRecoStatus_Red[nscan-1]->start();
			movieList_ClusteringRecoStatus_Red[nscan-1]->stop();
		}
		
		//Set progress bar
		progressBarList_ClusteringRecoStatus[nscan-1]->setValue(progress);
		
		//Set status message 
		labelList_statusBar_ClusteringRecoStatus[nscan-1]->setText(logMessage);
		
	}//close FOF task



	//########  ACF TASK ###############
	else if(task=="ACF"){

		//Set run status led
		if(status==ACFAnalysisThreadObj::eRunning){
			imageLabelList_ACFRecoStatus[nscan-1]->setMovie(movieList_ACFRecoStatus_Red[nscan-1]);
			
			if( movieList_ACFRecoStatus_Red[nscan-1]->state() != QMovie::Running ){
				movieList_ACFRecoStatus_Red[nscan-1]->start();
			}

		}
		else if(status==ACFAnalysisThreadObj::eFinished){
			
			imageLabelList_ACFRecoStatus[nscan-1]->setMovie(movieList_ACFRecoStatus_Green[nscan-1]);
			movieList_ACFRecoStatus_Green[nscan-1]->start();
			movieList_ACFRecoStatus_Green[nscan-1]->stop();

			
			//Enable 3D viewer button
			pushButtonList_ACFRecoStatus[nscan-1]->setEnabled(true);
			pushButtonList_ACFRecoStatus[nscan-1]->setEnabled(true);
	
			//Local download of output reco file
			std::string viewerInputFileName= outputFileName.toStdString();

			if(fRunMode==eAuto || fRunMode==eAutoBatch){//remote copy
				std::string scpProcExe= Form("scp %s@%s:%s",Gui::SEI_USERNAME.c_str(),Gui::SEI_HOSTNAME.c_str(),(outputFileName.toStdString()).c_str(),Gui::GUI_DATA_DIR.c_str());
				Utilities::ExecSystemCommand(scpProcExe.c_str());
				cout<<"TomographyRecTabMenu::logHandler(): INFO: scpProcExe: "<<scpProcExe<<endl;

				viewerInputFileName= Gui::GUI_DATA_DIR + std::string("/") + Utilities::GetBaseFileName(outputFileName.toStdString());			
			}
			

			//Set input filename for 3D viewer
			fACFOutputFileNameList[nscan-1]= viewerInputFileName;
			
			
		}
		else if(status==ACFAnalysisThreadObj::eFailure){
			
			imageLabelList_ACFRecoStatus[nscan-1]->setMovie(movieList_ACFRecoStatus_Red[nscan-1]);
			movieList_ACFRecoStatus_Red[nscan-1]->start();
			movieList_ACFRecoStatus_Red[nscan-1]->stop();
		}
		
		//Set progress bar
		progressBarList_ACFRecoStatus[nscan-1]->setValue(progress);
		
		//Set status message 
		labelList_statusBar_ACFRecoStatus[nscan-1]->setText(logMessage);
		
	}//close ACF task


	//########  EM ML TASK ###############
	else if(task=="EMML"){

		//Set run status led
		if(status==EMTomographyRecoThreadObj::eRunning){
			imageLabelList_LLRecoStatus[nscan-1]->setMovie(movieList_LLRecoStatus_Red[nscan-1]);
			
			if( movieList_LLRecoStatus_Red[nscan-1]->state() != QMovie::Running ){
				movieList_LLRecoStatus_Red[nscan-1]->start();
			}

		}
		else if(status==EMTomographyRecoThreadObj::eFinished){
			
			imageLabelList_LLRecoStatus[nscan-1]->setMovie(movieList_LLRecoStatus_Green[nscan-1]);
			movieList_LLRecoStatus_Green[nscan-1]->start();
			movieList_LLRecoStatus_Green[nscan-1]->stop();

			//Enable 3D viewer button
			pushButtonList_LLRecoStatus[nscan-1]->setEnabled(true);
			pushButtonList_LLVolRendering[nscan-1]->setEnabled(true);
	
			//Local download of output reco file
			std::string viewerInputFileName= outputFileName.toStdString();

			if(fRunMode==eAuto || fRunMode==eAutoBatch){//remote copy
				std::string scpProcExe= Form("scp %s@%s:%s",Gui::SEI_USERNAME.c_str(),Gui::SEI_HOSTNAME.c_str(),(outputFileName.toStdString()).c_str(),Gui::GUI_DATA_DIR.c_str());
				Utilities::ExecSystemCommand(scpProcExe.c_str());
				cout<<"TomographyRecTabMenu::logHandler(): INFO: scpProcExe: "<<scpProcExe<<endl;

				viewerInputFileName= Gui::GUI_DATA_DIR + std::string("/") + Utilities::GetBaseFileName(outputFileName.toStdString());			
			}
			

			//Set input filename for 3D viewer
			fEMLLOutputFileNameList[nscan-1]= viewerInputFileName;
			
			
		}
		else if(status==EMTomographyRecoThreadObj::eFailure){
			//SetFailureDownloadStatus();

			imageLabelList_LLRecoStatus[nscan-1]->setMovie(movieList_LLRecoStatus_Red[nscan-1]);
			movieList_LLRecoStatus_Red[nscan-1]->start();
			movieList_LLRecoStatus_Red[nscan-1]->stop();
		}
		
		//Set progress bar
		progressBarList_LLRecoStatus[nscan-1]->setValue(progress);
		
		//Set status message 
		labelList_statusBar_LLRecoStatus[nscan-1]->setText(logMessage);
		
	}//close EMML task

	
}//close TomographyRecTabMenu::logHandler()



void TomographyRecTabMenu::alarmHandler(QString data){

	cout<<"TomographyRecTabMenu::alarmHandler(): INFO: Start logging actions..."<<endl;

	
	//## Parse JSON message
	QJson::Parser parser;
	bool ok;
	QByteArray arraydata;
	arraydata.append(data);
	QVariantList result = parser.parse(arraydata, &ok).toList();
	
	if(!ok){
		cerr<<"TomographyRecTabMenu::alarmHandler(). ERROR: Cannot parse JSON string message...exit!"<<endl;
		return;
	}
	
	QString task = "";
	QString command = "";
  QString filename = "";
	QString inputFileName = "";
	QString outputFileName = "";
	QString path = "";
	QString host = "";
	QString user = "";
	QString id = "XXX";
  int nscan = 0;
  int status = 0;
	QString logMessage = "";
	int progress = 0;
	long int timeStamp= 0;
	QString guiKey = "";
	QString guiHostName = "";
	int guiPort = -1;

	foreach(QVariant record, result) {
    QVariantMap map = record.toMap();	
		task= map.value("task").toString();
		command= map.value("command").toString();
		filename= map.value("filename").toString();
		inputFileName= map.value("inputFileName").toString();
		outputFileName= map.value("outputFileName").toString();
		path= map.value("path").toString();
		host= map.value("host").toString();
		user= map.value("user").toString();
		id= map.value("containerId").toString();
		nscan= map.value("scanId").toInt();	
		status= map.value("status").toInt();	
		logMessage= map.value("log").toString();
		progress= map.value("progress").toInt();	
		timeStamp= map.value("timeStamp").toLongLong();	
		guiKey= map.value("guiKey").toString();
		guiHostName= map.value("guiHostName").toString();
		guiPort= map.value("guiPort").toInt();
	}//end foreach loop


	//## Check current GUI key with job gui key
	if(guiKey.toStdString() != Gui::GUI_KEY){
		cout<<"TomographyRecTabMenu::alarmHandler(): WARNING: Task gui key is different from current gui key...do not update GUI!"<<endl;
		return;	
	}


	//## Check current scan id
	if(nscan<1 || nscan>MAXNSCANS){
		cerr<<"TomographyRecTabMenu::alarmHandler(): ERROR: Current scan index is not in range [0,3]...exit!"<<endl;
		return;
	}


	
	//########  EM ML TASK ###############
	if(task=="EMML"){

		//Display alarm message in case of reco alarm
		imageLabelList_Alarm_EMTomographyReco[nscan-1]->setVisible(true);
		imageLabelList_AlarmText_EMTomographyReco[nscan-1]->setVisible(true);

		//Play alarm sound in case of reco alarm
		//if(mMediaObject_LLReco[nscan-1]){
		//	mMediaObject_LLReco[nscan-1]->play();
		//}

		//Post alarm message to web service database
		//QUrl params;
		//params.addQueryItem("alarm2", (data.toStdString()).c_str());
		//networkManager->post(*networkRequest, params.encodedQuery());


	}//close EMML task

	if(task=="POCA"){
		//Display alarm message in case of reco alarm
		imageLabelList_Alarm_POCAReco[nscan-1]->setVisible(true);
		imageLabelList_AlarmText_POCAReco[nscan-1]->setVisible(true);

		//Play alarm sound in case of reco alarm
		//if(mMediaObject_POCAReco[nscan-1]){
		//	mMediaObject_POCAReco[nscan-1]->play();
		//}

		//Post alarm message to web service database
		//QUrl params;
		//params.addQueryItem("alarm2", (data.toStdString()).c_str());
		//networkManager->post(*networkRequest, params.encodedQuery());

	}//close POCA task

	if(task=="FOF"){
		//Display alarm message in case of reco alarm
		imageLabelList_Alarm_ClusteringReco[nscan-1]->setVisible(true);
		imageLabelList_AlarmText_ClusteringReco[nscan-1]->setVisible(true);

		//Play alarm sound in case of reco alarm
		//if(mMediaObject_ClusteringReco[nscan-1]){
		//	mMediaObject_ClusteringReco[nscan-1]->play();
		//}

		//Post alarm message to web service database
		//QUrl params;
		//params.addQueryItem("alarm2", (data.toStdString()).c_str());
		//networkManager->post(*networkRequest, params.encodedQuery());

	}//close FOF task
	
	if(task=="ACF"){
		//Display alarm message in case of reco alarm
		imageLabelList_Alarm_ACFReco[nscan-1]->setVisible(true);
		imageLabelList_AlarmText_ACFReco[nscan-1]->setVisible(true);

		//Play alarm sound in case of reco alarm
		//if(mMediaObject_ACFReco[nscan-1]){
		//	mMediaObject_ACFReco[nscan-1]->play();
		//}

		//Post alarm message to web service database
		//QUrl params;
		//params.addQueryItem("alarm2", (data.toStdString()).c_str());
		//networkManager->post(*networkRequest, params.encodedQuery());

	}//close FOF task
	

}//close TomographyRecTabMenu::alarmHandler()


//void TomographyRecTabMenu::ProcessSocketData(){
void TomographyRecTabMenu::ProcessSocketData(QByteArray data){

	
	//## Skip reading and processing socket data if in manual mode
	if(fRunExecMode==eLocalInteractive) {	
		cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: Skip socket processing as running in local interactive mode..."<<endl;
		return;
	}

	//## Parse JSON message
	QJson::Parser parser;
	bool ok;
	QVariantList result = parser.parse(data, &ok).toList();
	

	if(!ok){
		cerr<<"TomographyRecTabMenu::ProcessSocketData(). ERROR: Cannot parse JSON string message...exit!"<<endl;
		return;
	}
	
	QString task = "";
	QString command = "";
  QString filename = "";
	QString inputFileName = "";
	QString outputFileName = "";
	QString path = "";
	QString host = "";
	QString user = "";
	QString guikey = "";
	QString guihostname = "";
	int guiport;
	QString id = "XXX";
  int nscan = 1;
  int status = 0;
	QString logMessage = "";
	int progress = 0;
	long int time= 0;
	QString jobdir = "";

	
	foreach(QVariant record, result) {
    QVariantMap map = record.toMap();	
		task= map.value("task").toString();
		command= map.value("command").toString();
		filename= map.value("filename").toString();
		inputFileName= map.value("inputFileName").toString();
		outputFileName= map.value("outputFileName").toString();
		path= map.value("path").toString();
		host= map.value("host").toString();
		user= map.value("user").toString();
		id= map.value("containerId").toString();
		nscan= map.value("scanId").toInt();	
		status= map.value("status").toInt();	
		logMessage= map.value("log").toString();
		progress= map.value("progress").toInt();	
		time= map.value("timeStamp").toLongLong();	
    guikey= map.value("guiKey").toString();
		guihostname= map.value("guihostName").toString();
		guiport= map.value("guiportNo").toInt();
		jobdir= map.value("jobDirName").toString();
	}//end foreach loop

		 
	
	cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: command="<<qPrintable(command)<<endl;
	cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: task="<<qPrintable(task)<<endl;
	cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: filename="<<qPrintable(filename)<<endl;
	cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: inputFileName="<<qPrintable(inputFileName)<<endl;
	cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: outputFileName="<<qPrintable(outputFileName)<<endl;
	cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: path="<<qPrintable(path)<<endl;
	cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: host="<<qPrintable(host)<<endl;
	cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: user="<<qPrintable(user)<<endl;
	cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: id="<<qPrintable(id)<<endl;
	cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: nscan="<<nscan<<endl;
	cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: status="<<status<<endl;
	cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: logMessage="<<qPrintable(logMessage)<<endl;	
	cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: progress="<<progress<<endl;	
	cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: timeStamp="<<time<<endl;
	cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: guiKey="<<qPrintable(guikey)<<endl;
	cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: guiHostName="<<qPrintable(guihostname)<<endl;
	cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: guiPort="<<guiport<<endl;
	cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: jobdir="<<qPrintable(jobdir)<<endl;
	
	
	if(nscan<1 || nscan>MAXNSCANS){
		cerr<<"TomographyRecTabMenu::ProcessSocketData(): ERROR: Current scan index is not in range [1,3]...exit!"<<endl;
		//exit(1);	
		return;
	}

	

	//## Skip further processing if receiving a LOG message in auto mode
	//## since the gui update is automatically done via QT signal-slot 	
	if(command=="LOG" && fRunExecMode==eLocalInteractive){
		cout<<"TomographyRecTabMenu::ProcessSocketData(). INFO: Skip further processing as received a LOG command in interactive mode..."<<endl;
		return;
	}


	//## Start tasks when receiving a READY command
	if(command=="READY"){
		cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: Received a READY message from remote host..."<<endl;

		//Form download message
		QVariantList jsonMessage;
		QVariantMap jsonMessageField;

		std::string inputFileName= filename.toStdString();
		//std::string outputFileName= Gui::GUI_DATA_DIR + std::string("/") + Utilities::GetBaseFileName(inputFileName);
		std::string outputFileName= Utilities::GetBaseFileName(inputFileName);
		
		std::string hostName= host.toStdString();
		std::string userName= user.toStdString();

		int portNo= Gui::GUI_PORT;
		std::string guihostName= Utilities::GetIpAddress();
		int guiportNo= Gui::SOCK_LISTEN_PORT;
		std::string guiKey= Gui::GUI_KEY;
		int retryAttempts= 3; 
		bool isLocalDownload= false;
 
		int scanId= nscan;
		std::string containerId= id.toStdString();
		long int timeStamp= time;
		
		jsonMessageField.insert("scanId",QVariant(scanId));
		jsonMessageField.insert("containerId",QVariant(containerId.c_str()) );
		jsonMessageField.insert("timeStamp",QVariant((long long)(timeStamp)) );
		jsonMessageField.insert("inputFileName",QVariant(inputFileName.c_str()));
		jsonMessageField.insert("outputFileName",QVariant(outputFileName.c_str()));
		jsonMessageField.insert("hostName",QVariant(hostName.c_str()));
		jsonMessageField.insert("userName",QVariant(userName.c_str()));
		jsonMessageField.insert("portNo",QVariant(portNo));
		jsonMessageField.insert("guihostName",QVariant(guihostName.c_str()));
		jsonMessageField.insert("guiportNo",QVariant(guiportNo));
		jsonMessageField.insert("guiKey",QVariant(guiKey.c_str()));
		jsonMessageField.insert("retryAttempts",QVariant(retryAttempts));
		jsonMessageField.insert("isLocalDownload",QVariant(isLocalDownload));
		
		jsonMessage << jsonMessageField;

		QJson::Serializer serializer;	
		serializer.setIndentMode(QJson::IndentCompact);
		bool ok;
		QByteArray msg= serializer.serialize(jsonMessage,&ok);

		cout<<"TomographyRecTabMenu::ProcessSocketData(): INFO: Download config message: "<<qPrintable(QString(msg))<<endl;
		
		if(ok){
			emit downloadStartSig(QString(msg));
		}
		else{
			cerr<<"TomographyRecTabMenu::ProcessSocketData(): ERROR: Cannot serialize json message to be sent to download task...exit!"<<endl;
			return;
		}

	}//close if READY


	//## Handle log message to update the GUI
	if(command=="LOG"){
		logHandler(data);
	}//close if LOG

	//## Handle alarm messages
	else if(command=="ALARM"){
		alarmHandler(data);
	}//close if ALARM

	else{
		cerr<<"TomographyRecTabMenu::ProcessSocketData(): ERROR: Unknown command received...exit!"<<endl;
		return;
	}

}//close TomographyRecTabMenu::ProcessSocketData()




void TomographyRecTabMenu::SetBusyDownloadStatus(){

	cout<<"TomographyRecTabMenu::SetBusyDownloadStatus()"<<endl;
	imageLabelList_fileStatus[fCurrentScanIndex]->setMovie(movieList_fileStatus_Red[fCurrentScanIndex]);
	movieList_fileStatus_Red[fCurrentScanIndex]->start();
	
}//close TomographyRecTabMenu::SetBusyDownloadStatus()

void TomographyRecTabMenu::SetBusyEMTomographyRecoStatus(){

	imageLabelList_LLRecoStatus[fCurrentScanIndex]->setMovie(movieList_LLRecoStatus_Red[fCurrentScanIndex]);
	movieList_LLRecoStatus_Red[fCurrentScanIndex]->start();

	std::string busyAlarmStatus= "-1";
	QUrl params;
	params.addQueryItem("alarm2", busyAlarmStatus.c_str());
		
	networkManager->post(*networkRequest, params.encodedQuery());

}//close TomographyRecTabMenu::SetBusyEMTomographyRecoStatus()


void TomographyRecTabMenu::SetBusyPOCARecoStatus(){

	imageLabelList_POCARecoStatus[fCurrentScanIndex]->setMovie(movieList_POCARecoStatus_Red[fCurrentScanIndex]);
	movieList_POCARecoStatus_Red[fCurrentScanIndex]->start();

	std::string busyAlarmStatus= "-1";
	QUrl params;
	params.addQueryItem("alarm1", busyAlarmStatus.c_str());
		
	networkManager->post(*networkRequest, params.encodedQuery());

}//close TomographyRecTabMenu::SetBusyACFRecoStatus()

void TomographyRecTabMenu::SetBusyACFRecoStatus(){

	imageLabelList_ACFRecoStatus[fCurrentScanIndex]->setMovie(movieList_ACFRecoStatus_Red[fCurrentScanIndex]);
	movieList_ACFRecoStatus_Red[fCurrentScanIndex]->start();

	std::string busyAlarmStatus= "-1";
	QUrl params;
	params.addQueryItem("alarm4", busyAlarmStatus.c_str());
		
	networkManager->post(*networkRequest, params.encodedQuery());

}//close TomographyRecTabMenu::SetBusyACFRecoStatus()

void TomographyRecTabMenu::SetBusyClusteringRecoStatus(){

	imageLabelList_ClusteringRecoStatus[fCurrentScanIndex]->setMovie(movieList_ClusteringRecoStatus_Red[fCurrentScanIndex]);
	movieList_ClusteringRecoStatus_Red[fCurrentScanIndex]->start();

	std::string busyAlarmStatus= "-1";
	QUrl params;
	params.addQueryItem("alarm3", busyAlarmStatus.c_str());
		
	networkManager->post(*networkRequest, params.encodedQuery());

}//close TomographyRecTabMenu::SetBusyClusteringRecoStatus()


void TomographyRecTabMenu::SetFailureDownloadStatus(){

	cout<<"TomographyRecTabMenu::SetFailureDownloadStatus()"<<endl;
	imageLabelList_fileStatus[fCurrentScanIndex]->setMovie(movieList_fileStatus_Red[fCurrentScanIndex]);
	movieList_fileStatus_Red[fCurrentScanIndex]->start();
	movieList_fileStatus_Red[fCurrentScanIndex]->stop();

}//close TomographyRecTabMenu::SetFailureDownloadStatus()

void TomographyRecTabMenu::SetFailurePOCARecoStatus(){

	imageLabelList_POCARecoStatus[fCurrentScanIndex]->setMovie(movieList_POCARecoStatus_Red[fCurrentScanIndex]);
	movieList_POCARecoStatus_Red[fCurrentScanIndex]->start();
	movieList_POCARecoStatus_Red[fCurrentScanIndex]->stop();
	
}//close TomographyRecTabMenu::SetFailurePOCARecoStatus()

void TomographyRecTabMenu::SetFailureEMTomographyRecoStatus(){

	cout<<"TomographyRecTabMenu::SetFailureEMTomographyRecoStatus()"<<endl;
	imageLabelList_LLRecoStatus[fCurrentScanIndex]->setMovie(movieList_LLRecoStatus_Red[fCurrentScanIndex]);
	movieList_LLRecoStatus_Red[fCurrentScanIndex]->start();
	movieList_LLRecoStatus_Red[fCurrentScanIndex]->stop();
	
}//close TomographyRecTabMenu::SetFailureEMTomographyRecoStatus()



void TomographyRecTabMenu::SetFailureACFRecoStatus(){

	imageLabelList_ACFRecoStatus[fCurrentScanIndex]->setMovie(movieList_ACFRecoStatus_Red[fCurrentScanIndex]);
	movieList_ACFRecoStatus_Red[fCurrentScanIndex]->start();
	movieList_ACFRecoStatus_Red[fCurrentScanIndex]->stop();
	
}//close TomographyRecTabMenu::SetFailureACFRecoStatus()

void TomographyRecTabMenu::SetFailureClusteringRecoStatus(){

	imageLabelList_ClusteringRecoStatus[fCurrentScanIndex]->setMovie(movieList_ClusteringRecoStatus_Red[fCurrentScanIndex]);
	movieList_ClusteringRecoStatus_Red[fCurrentScanIndex]->start();
	movieList_ClusteringRecoStatus_Red[fCurrentScanIndex]->stop();
	
}//close TomographyRecTabMenu::SetFailureClusteringRecoStatus()


void TomographyRecTabMenu::SetInitImageStatus(){

	cout<<"TomographyRecTabMenu::SetInitImageStatus()"<<endl;
	for(int i=0;i<MAXNSCANS;i++){
		//imageLabelList_fileStatus[i]->setMovie(movieList_fileStatus_Red[i]);
		//movieList_fileStatus_Red[i]->start();
		//movieList_fileStatus_Red[i]->stop();	

		imageLabelList_fileStatus[i]->setMovie(movieList_fileStatus_StaticRed[i]);
		movieList_fileStatus_StaticRed[i]->start();
		movieList_fileStatus_StaticRed[i]->stop();	

		imageLabelList_LLRecoStatus[i]->setMovie(movieList_LLRecoStatus_Red[i]);
		movieList_LLRecoStatus_Red[i]->start();
		movieList_LLRecoStatus_Red[i]->stop();

		
		//imageLabelList_POCARecoStatus[i]->setMovie(movieList_POCARecoStatus_Red[i]);
		//movieList_POCARecoStatus_Red[i]->start();
		//movieList_POCARecoStatus_Red[i]->stop();

		imageLabelList_POCARecoStatus[i]->setMovie(movieList_POCARecoStatus_StaticRed[i]);
		movieList_POCARecoStatus_StaticRed[i]->start();
		movieList_POCARecoStatus_StaticRed[i]->stop();	

		imageLabelList_ACFRecoStatus[i]->setMovie(movieList_ACFRecoStatus_Red[i]);
		movieList_ACFRecoStatus_Red[i]->start();
		movieList_ACFRecoStatus_Red[i]->stop();

		imageLabelList_ClusteringRecoStatus[i]->setMovie(movieList_ClusteringRecoStatus_Red[i]);
		movieList_ClusteringRecoStatus_Red[i]->start();
		movieList_ClusteringRecoStatus_Red[i]->stop();
	
		
		imageLabelList_Alarm_POCAReco[i]->hide();
		imageLabelList_AlarmText_POCAReco[i]->hide();

		imageLabelList_Alarm_EMTomographyReco[i]->hide();
		imageLabelList_AlarmText_EMTomographyReco[i]->hide();

		imageLabelList_Alarm_ACFReco[i]->hide();
		imageLabelList_AlarmText_ACFReco[i]->hide();

		imageLabelList_Alarm_ClusteringReco[i]->hide();
		imageLabelList_AlarmText_ClusteringReco[i]->hide();
		
		/*
		imageLabelList_Alarm_POCAReco[i]->setVisible(false);
		imageLabelList_AlarmText_POCAReco[i]->setVisible(false);

		imageLabelList_Alarm_EMTomographyReco[i]->setVisible(false);
		imageLabelList_AlarmText_EMTomographyReco[i]->setVisible(false);

		imageLabelList_Alarm_ACFReco[i]->setVisible(false);
		imageLabelList_AlarmText_ACFReco[i]->setVisible(false);

		imageLabelList_Alarm_ClusteringReco[i]->setVisible(false);
		imageLabelList_AlarmText_ClusteringReco[i]->setVisible(false);
		*/

		pushButtonList_POCARecoStatus[i]->setEnabled(false);
		pushButtonList_LLRecoStatus[i]->setEnabled(false);
		pushButtonList_ACFRecoStatus[i]->setEnabled(false);
		pushButtonList_ClusteringRecoStatus[i]->setEnabled(false);

		stopThreadButtonList_LLRecoStatus[i]->setEnabled(false);
		stopThreadButtonList_POCARecoStatus[i]->setEnabled(false);
		stopThreadButtonList_ClusteringRecoStatus[i]->setEnabled(false);
		stopThreadButtonList_ACFRecoStatus[i]->setEnabled(false);

		std::string initAlarmStatus= "-99";
		QUrl params;
		params.addQueryItem("alarm1", initAlarmStatus.c_str());
		params.addQueryItem("alarm2", initAlarmStatus.c_str());
		params.addQueryItem("alarm3", initAlarmStatus.c_str());
		params.addQueryItem("alarm4", initAlarmStatus.c_str());
	
		networkManager->post(*networkRequest, params.encodedQuery());
	}
	
}//close TomographyRecTabMenu::SetInitImageStatus()


void TomographyRecTabMenu::SetCompleteDownloadStatus(){

	imageLabelList_fileStatus[fCurrentScanIndex]->setMovie(movieList_fileStatus_Green[fCurrentScanIndex]);
	movieList_fileStatus_Green[fCurrentScanIndex]->start();
	movieList_fileStatus_Green[fCurrentScanIndex]->stop();
	
}//close TomographyRecTabMenu::SetCompleteDownloadStatus()

void TomographyRecTabMenu::SetCompletePOCARecoStatus(){

	imageLabelList_POCARecoStatus[fCurrentScanIndex]->setMovie(movieList_POCARecoStatus_Green[fCurrentScanIndex]);
	movieList_POCARecoStatus_Green[fCurrentScanIndex]->start();
	movieList_POCARecoStatus_Green[fCurrentScanIndex]->stop();
	
}//close TomographyRecTabMenu::SetCompletePOCARecoStatus()

void TomographyRecTabMenu::SetCompleteEMTomographyRecoStatus(){

	imageLabelList_LLRecoStatus[fCurrentScanIndex]->setMovie(movieList_LLRecoStatus_Green[fCurrentScanIndex]);
	movieList_LLRecoStatus_Green[fCurrentScanIndex]->start();
	movieList_LLRecoStatus_Green[fCurrentScanIndex]->stop();
	
}//close TomographyRecTabMenu::SetCompleteEMTomographyRecoStatus()


void TomographyRecTabMenu::SetCompleteACFRecoStatus(){

	imageLabelList_ACFRecoStatus[fCurrentScanIndex]->setMovie(movieList_ACFRecoStatus_Green[fCurrentScanIndex]);
	movieList_ACFRecoStatus_Green[fCurrentScanIndex]->start();
	movieList_ACFRecoStatus_Green[fCurrentScanIndex]->stop();
	
}//close TomographyRecTabMenu::SetCompleteACFRecoStatus()


void TomographyRecTabMenu::SetCompleteClusteringRecoStatus(){

	imageLabelList_ClusteringRecoStatus[fCurrentScanIndex]->setMovie(movieList_ClusteringRecoStatus_Green[fCurrentScanIndex]);
	movieList_ClusteringRecoStatus_Green[fCurrentScanIndex]->start();
	movieList_ClusteringRecoStatus_Green[fCurrentScanIndex]->stop();
	
}//close TomographyRecTabMenu::SetCompleteClusteringRecoStatus()

void TomographyRecTabMenu::playAlarmSound_POCAReco(){

	/*
	if(mMediaObject_POCAReco[fCurrentScanIndex]){
		mMediaObject_POCAReco[fCurrentScanIndex]->play();
	}
	*/

}//close playAlarmSound_POCAReco()

void TomographyRecTabMenu::playAlarmSound_LLReco(){

	/*
	if(mMediaObject_LLReco[fCurrentScanIndex]){
		mMediaObject_LLReco[fCurrentScanIndex]->play();
	}
	*/

}//close playAlarmSound_LLReco()


void TomographyRecTabMenu::playAlarmSound_ClusteringReco(){

	/*
	if(mMediaObject_ClusteringReco[fCurrentScanIndex]) {
		mMediaObject_ClusteringReco[fCurrentScanIndex]->play();
	}
	*/
	
}//close playAlarmSound_ClusteringReco()


void TomographyRecTabMenu::playAlarmSound_ACFReco(){

	/*
	if(mMediaObject_ACFReco[fCurrentScanIndex]){
		mMediaObject_ACFReco[fCurrentScanIndex]->play();
	}
	*/

}//close playAlarmSound_ACFReco()


void TomographyRecTabMenu::serviceReplyFinished(QNetworkReply* currentReply){

	QString data = (QString)currentReply->readAll();
	QScriptEngine engine;
  QScriptValue result = engine.evaluate("(" + data + ")");

	cout<<"TomographyRecTabMenu::serviceReplyFinished(): INFO: data: "<<qPrintable(data)<<endl;
	
	
}//close TomographyRecTabMenu::serviceReplyFinished()


void TomographyRecTabMenu::displayAlarm_POCAReco(){

	imageLabelList_Alarm_POCAReco[fCurrentScanIndex]->setVisible(true);
	imageLabelList_AlarmText_POCAReco[fCurrentScanIndex]->setVisible(true);

	std::string msg= "ALARM in this file!";
	QString alarmMessage= QString::fromStdString(msg);
	emit(alarm(alarmMessage));

	if(!checkBox_runMode->isChecked()){
		client= new QTcpSocket(this);
		connect(client, SIGNAL(connected()),this, SLOT(sendAlarmMessage(alarmMessage)));
		//connect(this, SIGNAL(alarm(QString)),this, SLOT(sendAlarmMessage(QString)));
		client->connectToHost(CCMIpAddress,CCMPortNumber);	
	}

	
}//close TomographyRecTabMenu::displayAlarm_POCAReco()




void TomographyRecTabMenu::sendAlarm_POCAReco(QString message){

	//## Define post data to be passed to the web-service
	std::string alarmStatus= message.toStdString();
	cout<<"alarmStatus="<<alarmStatus<<endl;

	//QByteArray postData;	
	//postData.append("temp1=string&");
	//networkManager->post(QNetworkRequest(serviceUrl), postData);


	QUrl params;
	params.addQueryItem("alarm1", alarmStatus.c_str());
	
	networkManager->post(*networkRequest, params.encodedQuery());

	
}//close TomographyRecTabMenu::sendAlarm_POCAReco()


void TomographyRecTabMenu::sendAlarm_LLReco(QString message){

	//## Define post data to be passed to the web-service
	std::string alarmStatus= message.toStdString();
	cout<<"alarmStatus="<<alarmStatus<<endl;

	QUrl params;
	params.addQueryItem("alarm2", alarmStatus.c_str());
	
	networkManager->post(*networkRequest, params.encodedQuery());
	
}//close TomographyRecTabMenu::sendAlarm_LLReco()


void TomographyRecTabMenu::sendAlarm_ClusteringReco(QString message){

	//## Define post data to be passed to the web-service
	std::string alarmStatus= message.toStdString();
	cout<<"alarmStatus="<<alarmStatus<<endl;

	QUrl params;
	params.addQueryItem("alarm3", alarmStatus.c_str());
	
	networkManager->post(*networkRequest, params.encodedQuery());
	
}//close TomographyRecTabMenu::sendAlarm_ClusteringReco()



void TomographyRecTabMenu::sendAlarm_ACFReco(QString message){

	//## Define post data to be passed to the web-service
	std::string alarmStatus= message.toStdString();
	cout<<"alarmStatus="<<alarmStatus<<endl;

	QUrl params;
	params.addQueryItem("alarm4", alarmStatus.c_str());
	
	networkManager->post(*networkRequest, params.encodedQuery());
	
}//close TomographyRecTabMenu::sendAlarm_ACFReco()


void TomographyRecTabMenu::displayAlarm_LLReco(){

	imageLabelList_Alarm_EMTomographyReco[fCurrentScanIndex]->setVisible(true);
	imageLabelList_AlarmText_EMTomographyReco[fCurrentScanIndex]->setVisible(true);

	std::string msg= "ALARM in this file!";
	QString alarmMessage= QString::fromStdString(msg);
	emit(alarm(alarmMessage));

	if(!checkBox_runMode->isChecked()){
		client= new QTcpSocket(this);
		connect(client, SIGNAL(connected()),this, SLOT(sendAlarmMessage(alarmMessage)));
		//connect(this, SIGNAL(alarm(QString)),this, SLOT(sendAlarmMessage(QString)));
		client->connectToHost(CCMIpAddress,CCMPortNumber);	
	}

}//close TomographyRecTabMenu::displayAlarm_EMTomographyReco()


void TomographyRecTabMenu::displayAlarm_ACFReco(){

	imageLabelList_Alarm_ACFReco[fCurrentScanIndex]->setVisible(true);
	imageLabelList_AlarmText_ACFReco[fCurrentScanIndex]->setVisible(true);

	std::string msg= "ALARM in this file!";
	QString alarmMessage= QString::fromStdString(msg);
	emit(alarm(alarmMessage));

	if(!checkBox_runMode->isChecked()){
		client= new QTcpSocket(this);
		connect(client, SIGNAL(connected()),this, SLOT(sendAlarmMessage(alarmMessage)));
		//connect(this, SIGNAL(alarm(QString)),this, SLOT(sendAlarmMessage(QString)));
		client->connectToHost(CCMIpAddress,CCMPortNumber);	
	}

}//close TomographyRecTabMenu::displayAlarm_ACFReco()


void TomographyRecTabMenu::displayAlarm_ClusteringReco(){

	imageLabelList_Alarm_ClusteringReco[fCurrentScanIndex]->setVisible(true);
	imageLabelList_AlarmText_ClusteringReco[fCurrentScanIndex]->setVisible(true);

	std::string msg= "ALARM in this file!";
	QString alarmMessage= QString::fromStdString(msg);
	emit(alarm(alarmMessage));

	if(!checkBox_runMode->isChecked()){
		client= new QTcpSocket(this);
		connect(client, SIGNAL(connected()),this, SLOT(sendAlarmMessage(alarmMessage)));
		//connect(this, SIGNAL(alarm(QString)),this, SLOT(sendAlarmMessage(QString)));
		client->connectToHost(CCMIpAddress,CCMPortNumber);	
	}

	
}//close TomographyRecTabMenu::displayAlarm_ClusteringReco()



void TomographyRecTabMenu::EnablePOCAViewerButton(){

	cout<<"TomographyRecTabMenu::EnablePOCAViewerButton()"<<endl;
	pushButtonList_POCARecoStatus[fCurrentScanIndex]->setEnabled(true);
	pushButtonList_POCAVolRendering[fCurrentScanIndex]->setEnabled(true);

}//close TomographyRecTabMenu::EnablePOCAViewerButton()

void TomographyRecTabMenu::EnableLLRecoViewerButton(){
	
	cout<<"TomographyRecTabMenu::EnableLLViewerButton()"<<endl;
	pushButtonList_LLRecoStatus[fCurrentScanIndex]->setEnabled(true);
	pushButtonList_LLVolRendering[fCurrentScanIndex]->setEnabled(true);

}//close TomographyRecTabMenu::EnableLLRecoViewerButton()

void TomographyRecTabMenu::EnableClusteringRecoViewerButton(){

	pushButtonList_ClusteringRecoStatus[fCurrentScanIndex]->setEnabled(true);
	pushButtonList_ClusteringVolRendering[fCurrentScanIndex]->setEnabled(true);

}//close TomographyRecTabMenu::EnableClusteringRecoViewerButton()

void TomographyRecTabMenu::EnableACFRecoViewerButton(){

	pushButtonList_ACFRecoStatus[fCurrentScanIndex]->setEnabled(true);

}//close TomographyRecTabMenu::EnableACFRecoViewerButton()


void TomographyRecTabMenu::StartPOCAViewer(){

	
	QThread* pocaViewerThread= new QThread();
	POCAViewerThreadObj* pocaViewerThreadObj= new POCAViewerThreadObj;

	if(pocaViewerThread){
		pocaViewerThread->quit();
	}


	pocaViewerThreadObj->SetInputFileName(Gui::fConfigParser->fPOCAOutputFileName);
	pocaViewerThreadObj->moveToThread(pocaViewerThread);
	pocaViewerThread->start();	

	connect(pocaViewerThread, SIGNAL(started()), pocaViewerThreadObj, SLOT(process()));//launch image viewer
	//connect(pocaViewerThreadObj, SIGNAL(finished()), pocaViewerThread, SLOT(quit()));//quit thread after finished signal
	//connect(pocaViewerThreadObj, SIGNAL(finished()), pocaViewerThreadObj, SLOT(deleteLater()));//delete object after finished signal	

}//close TomographyRecTabMenu::StartPOCAViewer()



void TomographyRecTabMenu::StartPOCAVolRenderingViewer(){


	QPushButton* button_pressed = static_cast<QPushButton*>(sender());
 	int pos = pushButtonList_POCAViewer.indexOf(button_pressed);
	std::string inputFileName= fPOCAOutputFileNameList[pos];
	cout<<"pos="<<pos<<"  inputFileName="<<inputFileName<<endl;
	
	//std::string imageOutputFileName= std::string("VolRendFig_") + Gui::fConfigParser->fPOCAOutputFileName + std::string(".png");
	std::string imageOutputFileName= std::string("VolRendFig_") + inputFileName + std::string(".png");

	QThread* viewerThread= new QThread();
	VolumeRenderingViewer* viewer= new VolumeRenderingViewer();
	//viewer->SetInputFileName(Gui::fConfigParser->fPOCAOutputFileName);
	viewer->SetInputFileName(inputFileName);
	viewer->SetOutputImageFileName(imageOutputFileName);
  viewer->SetDataRange(pow(10,-7),pow(10,-1));

	//viewer->moveToThread(viewerThread);
	//QCoreApplication::postEvent(viewer, new QEvent( QEvent::User ) );

	viewerThread->start();	
	
	connect(viewerThread, SIGNAL(started()), viewer, SLOT(process()));//launch image viewer
	connect(viewer, SIGNAL(finished()), viewerThread, SLOT(quit()));//quit thread after finished signal
	connect(viewer, SIGNAL(finished()), viewer, SLOT(deleteLater()));//delete object after finished signal	

}//close TomographyRecTabMenu::StartPOCAViewer()


void TomographyRecTabMenu::StartLLRecoViewer(){

	QPushButton* button_pressed = static_cast<QPushButton*>(sender());
 	int pos = pushButtonList_LLViewer.indexOf(button_pressed);
	std::string inputFileName= fEMLLOutputFileNameList[pos];
	
	QThread* llViewerThread= new QThread();
	EMLLViewerThreadObj* llViewerThreadObj= new EMLLViewerThreadObj;
	//llViewerThreadObj->SetInputFileName(Gui::fConfigParser->fEMLLOutputFileName);
	llViewerThreadObj->SetInputFileName(inputFileName);
	llViewerThreadObj->moveToThread(llViewerThread);
	llViewerThread->start();	
	connect(llViewerThread, SIGNAL(started()), llViewerThreadObj, SLOT(process()));//launch image viewer

}//close TomographyRecTabMenu::StartLLRecoViewer()


void TomographyRecTabMenu::StartLLVolRenderingViewer(){

	QPushButton* button_pressed = static_cast<QPushButton*>(sender());
 	int pos = pushButtonList_LLViewer.indexOf(button_pressed);
	std::string imageInputFileName= fEMLLOutputFileNameList[pos];

	//std::string imageOutputFileName= std::string("VolRendFig_") + Gui::fConfigParser->fEMLLOutputFileName + std::string(".png");
	//std::string imageOutputFileName= std::string("VolRendFig_") + imageInputFileName + std::string(".png");
	std::string imageOutputFileName= Gui::GUI_DATA_DIR + std::string("/VolRendFig_") + imageInputFileName + std::string(".png");
	cout<<"TomographyRecTabMenu::StartLLVolRenderingViewer(): INFO: imageInputFileName= "<<imageInputFileName<<"  imageOutputFileName="<<imageOutputFileName<<endl;

	QThread* viewerThread= new QThread();
	VolumeRenderingViewer* viewer= new VolumeRenderingViewer();
	//viewer->SetInputFileName(Gui::fConfigParser->fEMLLOutputFileName);
	viewer->SetInputFileName(imageInputFileName);
	viewer->SetOutputImageFileName(imageOutputFileName);
  viewer->SetDataRange(pow(10,-7),pow(10,-1));

	//viewer->moveToThread(viewerThread);
	//QCoreApplication::postEvent(viewer, new QEvent( QEvent::User ) );

	viewerThread->start();	
	
	connect(viewerThread, SIGNAL(started()), viewer, SLOT(process()));//launch image viewer
	connect(viewer, SIGNAL(finished()), viewerThread, SLOT(quit()));//quit thread after finished signal
	connect(viewer, SIGNAL(finished()), viewer, SLOT(deleteLater()));//delete object after finished signal	

}//close TomographyRecTabMenu::StartLLVolRenderingViewer()




void TomographyRecTabMenu::StartClusteringRecoViewer(){

	QPushButton* button_pressed = static_cast<QPushButton*>(sender());
 	int pos = pushButtonList_ClusteringViewer.indexOf(button_pressed);
	std::string inputFileName= fClusteringOutputFileNameList[pos];


	QThread* clusteringViewerThread= new QThread();
	ClusteringViewerThreadObj* clusteringViewerThreadObj= new ClusteringViewerThreadObj;
	//clusteringViewerThreadObj->SetInputFileName(Gui::fConfigParser->fClusteringOutputFileName);
	clusteringViewerThreadObj->SetInputFileName(inputFileName);
	clusteringViewerThreadObj->moveToThread(clusteringViewerThread);
	clusteringViewerThread->start();	
	connect(clusteringViewerThread, SIGNAL(started()), clusteringViewerThreadObj, SLOT(process()));//launch image viewer
	
}//close TomographyRecTabMenu::StartClusteringRecoViewer()

void TomographyRecTabMenu::StartClusteringVolRenderingViewer(){

	QPushButton* button_pressed = static_cast<QPushButton*>(sender());
 	int pos = pushButtonList_ClusteringViewer.indexOf(button_pressed);
	std::string inputFileName= fClusteringOutputFileNameList[pos];

	//std::string imageOutputFileName= std::string("VolRendFig_") + Gui::fConfigParser->fClusteringOutputFileName + std::string(".png");
	std::string imageOutputFileName= std::string("VolRendFig_") + inputFileName + std::string(".png");

	QThread* viewerThread= new QThread();
	VolumeRenderingViewer* viewer= new VolumeRenderingViewer();
	//viewer->SetInputFileName(Gui::fConfigParser->fClusteringOutputFileName);
	viewer->SetInputFileName(inputFileName);
	viewer->SetOutputImageFileName(imageOutputFileName);
  viewer->SetDataRange(pow(10,-7),pow(10,-1));

	//viewer->moveToThread(viewerThread);
	//QCoreApplication::postEvent(viewer, new QEvent( QEvent::User ) );

	viewerThread->start();	
	
	connect(viewerThread, SIGNAL(started()), viewer, SLOT(process()));//launch image viewer
	connect(viewer, SIGNAL(finished()), viewerThread, SLOT(quit()));//quit thread after finished signal
	connect(viewer, SIGNAL(finished()), viewer, SLOT(deleteLater()));//delete object after finished signal	

}//close TomographyRecTabMenu::StartClusteringVolRenderingViewer()


void TomographyRecTabMenu::StartACFRecoViewer(){

	QPushButton* button_pressed = static_cast<QPushButton*>(sender());
 	int pos = pushButtonList_ACFViewer.indexOf(button_pressed);
	std::string inputFileName= fACFOutputFileNameList[pos];


	QThread* acfViewerThread= new QThread();
	ACFViewerThreadObj* acfViewerThreadObj= new ACFViewerThreadObj;
	//acfViewerThreadObj->SetInputFileName(Gui::fConfigParser->fACFOutputFileName);
	acfViewerThreadObj->SetInputFileName(inputFileName);
	acfViewerThreadObj->moveToThread(acfViewerThread);
	acfViewerThread->start();	
	connect(acfViewerThread, SIGNAL(started()), acfViewerThreadObj, SLOT(process()));//launch image viewer

	
}//close TomographyRecTabMenu::StartACFRecoViewer()



void TomographyRecTabMenu::stopLLReco(){

	if(emTomographyRecoThread && emTomographyRecoThreadObj){
		emTomographyRecoThread->terminate();
		emTomographyRecoThread->quit();
		emTomographyRecoThread->deleteLater();
		emTomographyRecoThreadObj->deleteLater();
	}

}//close TomographyRecTabMenu::stopLLReco()

void TomographyRecTabMenu::stopPOCAReco(){

	if(pocaRecoThread && pocaRecoThreadObj){
		pocaRecoThread->quit();
		pocaRecoThread->deleteLater();
		pocaRecoThreadObj->deleteLater();
	}

}//close TomographyRecTabMenu::stopPOCAReco()


void TomographyRecTabMenu::stopClusteringReco(){

	if(fofThread && fofThreadObj){
		fofThread->quit();
		fofThread->deleteLater();
		fofThreadObj->deleteLater();
	}

}//close TomographyRecTabMenu::stopClusteringReco()


void TomographyRecTabMenu::stopACFReco(){

	if(acfAnalysisThread && acfAnalysisThreadObj){
		acfAnalysisThread->quit();
		acfAnalysisThread->deleteLater();
		acfAnalysisThreadObj->deleteLater();
	}

}//close TomographyRecTabMenu::stopACFReco()


void TomographyRecTabMenu::EnableRunButton(){
	pushButton_Run->setEnabled(true);
}

void TomographyRecTabMenu::EnablePOCAStopButton(){
	stopThreadButtonList_POCARecoStatus[fCurrentScanIndex]->setEnabled(true);
}

void TomographyRecTabMenu::EnableLLStopButton(){
	stopThreadButtonList_LLRecoStatus[fCurrentScanIndex]->setEnabled(true);
}
		
void TomographyRecTabMenu::EnableACFStopButton(){
	stopThreadButtonList_ACFRecoStatus[fCurrentScanIndex]->setEnabled(true);
}
		
void TomographyRecTabMenu::EnableClusteringStopButton(){
	stopThreadButtonList_ClusteringRecoStatus[fCurrentScanIndex]->setEnabled(true);
}
		

