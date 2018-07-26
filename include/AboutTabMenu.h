


#ifndef ABOUT_TAB_MENU_H
#define ABOUT_TAB_MENU_H

#include <QDialog>
#include <QObject>

/*
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtCore/QFileInfo>
#include <QtGui/QTabWidget>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtCore/QSize>
*/

class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
class QCheckBox;
class QGroupBox;
class QPushButton;
class QTextEdit;
class QSize;

class AboutTabMenu : public QWidget {
     
	Q_OBJECT

 	public:

  	AboutTabMenu(QWidget *parent = 0);
		
		//void updateImage(); 
    //void paintEvent(QPaintEvent*);
    //void resizeEvent (QResizeEvent*);

	public:
		
	
	private:
		QSize widgetSize;
		QPixmap* pixmap;
		
	//protected:
	//	void paintEvent(QPaintEvent* pe);


};

#endif
