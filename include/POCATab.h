#ifndef POCA_TAB_H
#define POCA_TAB_H

#include <QDialog>
#include <QScienceSpinBox.h>
#include <QObject>


class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
class QCheckBox;
class QGroupBox;
class QPushButton;
class QTextEdit;
class QComboBox;
class QSpinBox;
class QDoubleSpinBox;
class QRadioButton;


class POCATab : public QWidget {
     
	Q_OBJECT

 	public:
  	POCATab(QWidget *parent = 0);
 
	public slots:
		
		void BrowseInputFile();	
		void BrowseOutputFile();	
		
	public:
		void SetPOCAOptions();
		
	private:

		bool DumpPOCAConfigFile();
		std::string fPOCAConfigFileName;		
		
		
		QGroupBox* groupBox_POCA;

		QComboBox* comboBox_inputFile;
		QPushButton* pushButton_inputFile;	
		QComboBox* comboBox_outputFile;
		QPushButton* pushButton_outputFile;	

		QCheckBox* checkBox_useQueueRun;
	
		QScienceSpinBox* spinbox_POCATolerance;

		QCheckBox* checkBox_maxNEvents;
		QSpinBox* spinbox_maxNEvents;

		QCheckBox* checkBox_useScatteringAngleCut;
		QDoubleSpinBox* spinbox_minScatteringAngle;
		QDoubleSpinBox* spinbox_maxScatteringAngle;

		QDoubleSpinBox* spinbox_alarmThreshold;
		QSpinBox* spinbox_NEventThreshold;
		QComboBox* comboBox_drawStyle;

		QCheckBox* checkBox_useThetaWeight;
		QDoubleSpinBox* spinbox_thetaWeight;
		QCheckBox* checkBox_useAverageSignal;
		
		QCheckBox* checkBox_useEnergyCut;
		QDoubleSpinBox* spinbox_minEnergy;
		QDoubleSpinBox* spinbox_maxEnergy;
		
};

#endif
