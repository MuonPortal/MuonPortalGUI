#ifndef GUI_SERVER_H
#define GUI_SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>

#include <Gui.h>

class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
class QCheckBox;
class QGroupBox;
class QPushButton;
class QTextEdit;
class QListWidget;
class QListWidgetItem;
class QStackedWidget;

class GuiServer : public QObject {
    
	Q_OBJECT

	public:
    explicit GuiServer(QObject* parent = 0, long int portNo= 9999);
    
	public:
		void SetPort(long int portNumber) {port = portNumber;}
		void SetAllowedHosts(std::string host) {hostname = host;}

	signals:
    
	public slots:
  	void newConnection();
		void processIncomingData();

	private:
    QTcpServer* server;
		QTcpSocket* socket;
		long int port;
		std::string hostname;
		Gui* gui;

};

#endif // GUI_SERVER_H


