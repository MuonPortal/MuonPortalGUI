

#include <GuiServer.h>
#include <Gui.h>
#include <QtGui>

GuiServer::GuiServer(QObject *parent, long int portNo)
	: QObject(parent), port(portNo)   
{
    
	//## Create the TCP server
	cout<<"GuiServer::GuiServer(): INFO: Creating a tcp server listening on port "<<portNo<<endl;
	server = new QTcpServer(this);

	//QHostAddress host("192.168.3.89");
	
	// whenever a user connects, it will emit signal
  connect(server, SIGNAL(newConnection()),this, SLOT(newConnection()));

  if(!server->listen(QHostAddress::Any, portNo)) {
  	qDebug() << "Server could not start";
		QMessageBox::critical(0, tr("Gui Server"), tr("Unable to start the server: %1.").arg(server->errorString())  );
  	exit(1);
  }
  else {
  	qDebug() << "Server started!";
  }

	//## Create and show the GUI
	gui= new Gui;
	if(gui) {
		gui->show();
	}
	else{
		qDebug() << "Cannot start the gui";
		QMessageBox::critical(0, tr("Gui Server"), tr("Unable to start the gui"));
  	exit(1);
	}

}//close constructor


void GuiServer::newConnection(){

	//## Need to grab the socket
  socket = server->nextPendingConnection();
	connect(socket, SIGNAL(readyRead()),this, SLOT(processIncomingData()));

}//close GuiServer::newConnection()


void GuiServer::processIncomingData(){

	//## Read incoming data from client	
	char buffer[1024] = {0};
	socket->read(buffer, socket->bytesAvailable());
	
	std::string message= buffer;
	cout<< "Server received a message from a client: " <<message<<endl;
	
	//## Reply to client	
	socket->write("Hi client, I got your message!\r\n");
  socket->flush();
  socket->waitForBytesWritten(3000);
	
	//## Close the socket
  socket->close();

}//close GuiServer::processIncomingData()




