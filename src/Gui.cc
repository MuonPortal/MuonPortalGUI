#include <Gui.h>

#include <TabMenu.h>
#include <ConfigParser.h>
#include <Utilities.h>

#include <QtGui>
#include <QUuid>

ConfigParser* Gui::fConfigParser;
TomographyRecTabMenu* Gui::fTomographyRecTabMenu;
TabMenu* Gui::fTabMenu;

std::string Gui::GUI_BASE_DIR;		
std::string Gui::GUI_CONFIG_DIR;		
std::string Gui::GUI_DATA_DIR;
std::string Gui::GUI_SCRIPT_DIR;
std::string Gui::SHELL_DIR;	
std::string Gui::GUI_HOSTNAME;
int Gui::GUI_PORT= 9999;
std::string Gui::GUI_KEY;

std::string Gui::SEI_HOSTNAME;
std::string Gui::SEI_USERNAME;

std::string Gui::SOCK_END_PATTERN= "#END#";
long int Gui::SOCK_LISTEN_PORT= 9930;


Gui::Gui(QWidget *parent)
		 : QWidget(parent)  
{

	//## Set GUI env vars
	bool status= SetGuiEnvVariables();
	if(!status){
		QMessageBox::critical(this, "ERROR", "Gui::Gui(): Environment variables setting failed...exit!");
		QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
		return;	
	}

	//## Init class widgets
	fConfigParser= new ConfigParser; 
	
	
	fAboutTabMenu= new AboutTabMenu(this);
	fAboutTabMenu->setObjectName(QString::fromUtf8("tab_AboutMenu"));
	
	fTomographyRecTabMenu= new TomographyRecTabMenu(this);
	fTomographyRecTabMenu->setObjectName(QString::fromUtf8("tab_TomographyRecMenu"));
	
	fTabMenu= new TabMenu(this);
	fTabMenu->setObjectName(QString::fromUtf8("tab_Menu"));
	

	contentsWidget = new QListWidget(this);
  contentsWidget->setViewMode(QListView::IconMode);
  contentsWidget->setIconSize(QSize(100, 100));
  contentsWidget->setMovement(QListView::Static);
  contentsWidget->setMaximumWidth(400);
	contentsWidget->setFixedWidth(130);
  contentsWidget->setSpacing(12);

  pagesWidget = new QStackedWidget(this);
	pagesWidget->addWidget(fAboutTabMenu);
  pagesWidget->addWidget(fTabMenu);
	pagesWidget->addWidget(fTomographyRecTabMenu);
	
	std::string exitButton_imagepath= Gui::GUI_BASE_DIR + std::string("/share/exitButton.png");
	
	QPushButton* pushButton_Exit = new QPushButton(tr("Exit"));
	pushButton_Exit->setObjectName(QString::fromUtf8("pushButton_Exit"));
	pushButton_Exit->setIcon(QIcon(exitButton_imagepath.c_str()));
	pushButton_Exit->setFixedSize(QSize(60,30));


	createIcons();
  contentsWidget->setCurrentRow(0);


	//## Set object actions
	connect(pushButton_Exit, SIGNAL(clicked()), this, SLOT(close()));
	connect(this, SIGNAL(failure()), this, SLOT(close()));

	//## Set object layout
	QHBoxLayout *horizontalLayout = new QHBoxLayout;
  horizontalLayout->addWidget(contentsWidget);
  horizontalLayout->addWidget(pagesWidget, 1);

  QHBoxLayout *buttonsLayout = new QHBoxLayout;
  buttonsLayout->addStretch(1);
  buttonsLayout->addWidget(pushButton_Exit);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(horizontalLayout);
  mainLayout->addStretch(1);
  mainLayout->addLayout(buttonsLayout);
  setLayout(mainLayout);

  setWindowTitle(tr("MPToolkit GUI"));
	setFixedSize(1100, 700);

	

}//close constructor


bool Gui::SetGuiEnvVariables()
{
	//## Set environmental variables
	//SHELL PATH
	QString shelldir_name= QString::fromAscii(qgetenv("SHELL").data());
	QFileInfo shellfileinfo(shelldir_name);
	if(!shellfileinfo.exists()){
		QMessageBox::critical(this, "ERROR", "Gui::SetGuiEnvVariables(): Cannot get shell path directory (did you export SHELL variable?)...exit!");
		QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
		return false;
	}
	SHELL_DIR= std::string(qPrintable(shelldir_name));


	//ROOT_DIR
	QString dir_name= QString::fromAscii(qgetenv("MUPORTAL_GUI_ROOT").data());
	QDir dir= QDir(dir_name);
	if(!dir.exists()){
		QMessageBox::critical(this, "ERROR", "MUPORTAL_GUI_ROOT env var not set or set to a not-existing directory!");
		QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
		return false;
	}
	GUI_BASE_DIR= std::string(qPrintable(dir_name));
	
	
	//CONFIG_DIR
	QString configdir_name= QString::fromAscii(qgetenv("MUPORTAL_GUI_CONFIG").data());
	QDir configdir= QDir(configdir_name);
	if(!configdir.exists()){
		QMessageBox::critical(this, "ERROR", "Gui::SetGuiEnvVariables(): Config dir does not exist (did you set env var MUPORTAL_GUI_CONFIG?)...exit!");
		QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
		return false;
	}
	QFileInfo configfileinfo(configdir_name);
 	if(configfileinfo.isWritable()){
		configdir.mkdir(configdir_name);		
	}
	else{
		QMessageBox::critical(this, "ERROR", "Gui::SetGuiEnvVariables(): Config dir is not writable (please check permissions!)...exit!");
		QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
		return false;
	}
	GUI_CONFIG_DIR= std::string(qPrintable(configdir_name));



	//DATA_DIR
	QString datadir_name= QString::fromAscii(qgetenv("MUPORTAL_GUI_DATA").data());
	QDir datadir= QDir(datadir_name);
	if(!datadir.exists()){
		QMessageBox::critical(this, "ERROR", "Gui::SetGuiEnvVariables(): Data dir does not exist (did you set var MUPORTAL_GUI_DATA?)...exit!");
		QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
		return false;
	}
	QFileInfo datafileinfo(datadir_name);
 	if(datafileinfo.isWritable()){
		datadir.mkdir(datadir_name);		
	}
	else{
		QMessageBox::critical(this, "ERROR", "Gui::SetGuiEnvVariables(): Data dir is not writable (please check permissions!)...exit!");
		QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
		return false;
	}
	GUI_DATA_DIR= std::string(qPrintable(datadir_name));
	
	
	//SCRIPT_DIR
	QString scriptdir_name= QString::fromAscii(qgetenv("MUPORTAL_GUI_SCRIPT").data());
	QDir scriptdir= QDir(scriptdir_name);
	if(!scriptdir.exists()){
		QMessageBox::critical(this, "ERROR", "Gui::SetGuiEnvVariables(): Script dir does not exist (did you set var MUPORTAL_GUI_SCRIPT?)...exit!");
		QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
		return false;
	}
	QFileInfo scriptfileinfo(scriptdir_name);
 	if(scriptfileinfo.isWritable()){
		scriptdir.mkdir(scriptdir_name);		
	}
	else{
		QMessageBox::critical(this, "ERROR", "Gui::SetGuiEnvVariables(): Script dir is not writable (please check permissions!)...exit!");
		QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
		return false;
	}
	GUI_SCRIPT_DIR= std::string(qPrintable(scriptdir_name));

	
	
	//## Remote elaboration system (SEL) hostname and user	
	QString seihostname= QString::fromAscii(qgetenv("MUPORTAL_SEI_HOST").data());
	if(seihostname == ""){
		QMessageBox::critical(this, "ERROR", "Gui::SetGuiEnvVariables(): Script dir does not exist (did you set var MUPORTAL_SEI_HOST?)...exit!");
		QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
		return false;
	}
	SEI_HOSTNAME= seihostname.toStdString();
 
	QString seiusername= QString::fromAscii(qgetenv("MUPORTAL_SEI_USER").data());
	if(seiusername == ""){
		QMessageBox::critical(this, "ERROR", "Gui::SetGuiEnvVariables(): Script dir does not exist (did you set var MUPORTAL_SEI_USER?)...exit!");
		QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
		return false;
	}
	SEI_USERNAME= seiusername.toStdString();

	

	//## Generate a GUI ID for this gui session
	QUuid uuid= QUuid::createUuid();
	std::string key= (uuid.toString()).toStdString();
	Utilities::StringFindAndReplace(key,std::string("{"),std::string(""));
	Utilities::StringFindAndReplace(key,std::string("}"),std::string(""));
	GUI_KEY= key;	

	cout<<endl;
	cout<<"**** ENV VARS ****"<<endl;
	cout<<"GUI_BASE_DIR: "<<GUI_BASE_DIR<<endl;
	cout<<"GUI_CONFIG_DIR: "<<GUI_CONFIG_DIR<<endl;	
	cout<<"GUI_DATA_DIR: "<<GUI_DATA_DIR<<endl;
	cout<<"GUI_SCRIPT_DIR: "<<GUI_SCRIPT_DIR<<endl;
	cout<<"SEI_HOSTNAME: "<<SEI_HOSTNAME<<endl;
	cout<<"SEI_USERNAME: "<<SEI_USERNAME<<endl;
	cout<<"GUI_KEY: "<<GUI_KEY<<endl;
	cout<<"******************"<<endl;
	cout<<endl;

	return true;

}//close Gui::SetGuiEnvVariables()



void Gui::createIcons(){

	
	std::string info_imagepath= Gui::GUI_BASE_DIR + std::string("/share/infoButton.png");
	std::string config_imagepath= Gui::GUI_BASE_DIR + std::string("/share/configurationButton.png");
	std::string sim_imagepath= Gui::GUI_BASE_DIR + std::string("/share/simButton.png");
	std::string reco_imagepath= Gui::GUI_BASE_DIR + std::string("/share/recoButton.png");
	std::string tomography_imagepath= Gui::GUI_BASE_DIR + std::string("/share/tomographyButton_zoom.png");
	std::string analysis_imagepath= Gui::GUI_BASE_DIR + std::string("/share/analysisButton.png");
	
	QListWidgetItem *aboutButton = new QListWidgetItem(contentsWidget);
  aboutButton->setIcon(QIcon( info_imagepath.c_str() ));
  aboutButton->setText(tr("About Project"));
  aboutButton->setTextAlignment(Qt::AlignHCenter);
  aboutButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	
	QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
  configButton->setIcon(QIcon( config_imagepath.c_str() ));
  configButton->setText(tr("Configuration"));
  configButton->setTextAlignment(Qt::AlignHCenter);
  configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	
	QListWidgetItem *tomographyRecButton = new QListWidgetItem(contentsWidget);
  tomographyRecButton->setIcon(QIcon( tomography_imagepath.c_str() ));
  tomographyRecButton->setText(tr("Tomography"));
  tomographyRecButton->setTextAlignment(Qt::AlignHCenter);
  tomographyRecButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	
  connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}


void Gui::changePage(QListWidgetItem *current, QListWidgetItem *previous){
	
	if (!current) current = previous;
  pagesWidget->setCurrentIndex(contentsWidget->row(current));

}//close Gui::changePage()


