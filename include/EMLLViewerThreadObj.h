#ifndef EMLLViewerThreadObj_h
#define EMLLViewerThreadObj_h 1


#include <QDialog>
#include <QObject>


#include <vector>
#include <map>
#include <string>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TFile.h>
#include <TTree.h>
#include <TMatrixD.h>
#include <TF1.h>
#include <TGraph.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TImage.h>
#include <TASImage.h>

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

class QObject;
class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
class QCheckBox;
class QGroupBox;
class QPushButton;
class QTextEdit;
class QSpinBox;
class QDoubleSpinBox;
class QComboBox;
class QLineEdit;
class QRadioButton;
class QProgressBar;
class QMovie;
class QLabel;
class QUdpSocket;
class QHostAddress;
class QtNetwork;
class QThread;



class EMLLViewerThreadObj: public QObject {
    
	Q_OBJECT

  public:
		
		/** 
		\brief Class constructor: initialize structures.
 		*/
    EMLLViewerThreadObj();
		/**
		* \brief Class destructor: free allocated memory
		*/
   ~EMLLViewerThreadObj();

		
	public slots:
		void process();
		
  	signals:
			void finished();
	    void error();
			void statusMessage(QString msg);
			void statusProgress(int progress);
			void alarm();

	public:

		
		void SetInputFileName(std::string filename){fInputFileName= filename;}
		
	private:

		//## FileName
		std::string fInputFileName;	
		

};

#endif 
