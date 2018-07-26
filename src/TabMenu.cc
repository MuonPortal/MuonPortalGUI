#include <TabMenu.h>
#include <Gui.h>
#include <Utilities.h>
#include <QtGui>

#include <iostream>

using namespace std;


TabMenu::TabMenu(QWidget *parent){
 	
	//## Create tabs  
	fTabMenu = new QTabWidget(parent);
  fTabMenu->setObjectName(QString::fromUtf8("tab_Menu"));
  
	
	fContainerTab= new ContainerTab(fTabMenu);
	fContainerTab->setObjectName(QString::fromUtf8("tab_Container"));

	
	fPOCATab= new POCATab(fTabMenu);
	fPOCATab->setObjectName(QString::fromUtf8("tab_POCA"));

	fEMLLTab= new EMLLTab(fTabMenu);
	fEMLLTab->setObjectName(QString::fromUtf8("tab_EMLL"));

	fACFTab= new ACFTab(fTabMenu);
	fACFTab->setObjectName(QString::fromUtf8("tab_ACF"));

	fClusteringTab= new ClusteringTab(fTabMenu);
	fClusteringTab->setObjectName(QString::fromUtf8("tab_Clustering"));
	

	std::string initButton_imagepath= Gui::GUI_BASE_DIR + std::string("/share/initButton.png");
	pushButton_Init = new QPushButton(tr("Init"));
  pushButton_Init->setObjectName(QString::fromUtf8("pushButton_Init"));
	pushButton_Init->setIcon(QIcon(initButton_imagepath.c_str()));
	pushButton_Init->setFixedSize(QSize(60,30));

	std::string checkButton_imagepath= Gui::GUI_BASE_DIR + std::string("/share/checkButton.png");
	QImage image(checkButton_imagepath.c_str());
	image.setText(QString("Standard Config"),QString("Standard Config"));

	

	//## Add tabs to main menu
	
	fTabMenu->addTab(fContainerTab, tr("Geometry"));	
	fTabMenu->addTab(fPOCATab, tr("POCA"));
	fTabMenu->addTab(fACFTab, tr("ACF"));		
	fTabMenu->addTab(fClusteringTab, tr("Clustering"));		
	fTabMenu->addTab(fEMLLTab, tr("EMLL"));			
	

	//## Define object actions
	connect(pushButton_Init, SIGNAL(clicked()), this, SLOT(SetTabMenuOptions()) );	
	connect(pushButton_Init, SIGNAL(clicked()), Gui::fTomographyRecTabMenu, SLOT(EnableRunButton()) );
		
	

	//## Set object layout
	QHBoxLayout *buttonsLayout = new QHBoxLayout;
  buttonsLayout->addWidget(pushButton_Init);
	buttonsLayout->addSpacing(10);
	buttonsLayout->addStretch(1);
	
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(fTabMenu);
	mainLayout->addSpacerItem(new QSpacerItem(0, 500, QSizePolicy::Maximum, QSizePolicy::Maximum));
	mainLayout->addLayout(buttonsLayout);
  mainLayout->addStretch(1);
  setLayout(mainLayout);

}//close constructor


void TabMenu::SetTabMenuOptions(){

	//## Set all tab options
	cout<<"================="<<endl;
	cout<<"==> SET OPTIONS "<<endl;
	fContainerTab->SetContainerOptions();
	fPOCATab->SetPOCAOptions();
	fEMLLTab->SetEMLLOptions();
	fACFTab->SetACFOptions();
	fClusteringTab->SetClusteringOptions();

	
}//close TabMenu::SetTabMenuOptions()



