
#ifndef TAB_MENU_H
#define TAB_MENU_H

#include <QDialog>
#include <QObject>

#include <ContainerTab.h>
#include <POCATab.h>
#include <EMLLTab.h>
#include <ACFTab.h>
#include <ClusteringTab.h>

class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
class QCheckBox;
class QGroupBox;
class QPushButton;
class QTextEdit;
class QComboBox;

class TabMenu : public QWidget {
     
	Q_OBJECT

 	public:

  	TabMenu(QWidget *parent = 0);

	public slots:
		void SetTabMenuOptions();

	public:

		
	private:

		QTabWidget* fTabMenu;
		
		ContainerTab* fContainerTab;
		POCATab* fPOCATab;
		EMLLTab* fEMLLTab;
		ACFTab* fACFTab;
		ClusteringTab* fClusteringTab;

		QPushButton* pushButton_Init;	
		
};

#endif
