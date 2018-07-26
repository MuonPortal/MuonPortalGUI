
#ifndef MyThread_h
#define MyThread_h


#include <QObject>
#include <QThread>
#include <iostream>

using namespace std;

class MyThread : public QThread {

	Q_OBJECT

	public:
	
		MyThread();

	public slots:
	
		void stop(){
			cout<<"====  THREAD SToP ===="<<endl;
			emit isstopped();
		}
  

	signals:
    void isstopped();

  private:
     
};


#endif
