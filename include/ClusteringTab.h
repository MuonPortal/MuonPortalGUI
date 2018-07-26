#ifndef CLUSTERING_TAB_H
#define CLUSTERING_TAB_H

#include <QDialog>
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
class QObject;

class ClusteringTab : public QWidget {
     
	Q_OBJECT

 	public:
  	ClusteringTab(QWidget* parent = 0);
 
	public slots:
		void BrowseInputFile();
		void BrowseOutputFile();
		
	public:

		void SetClusteringOptions();
		
	private:

		bool DumpClusteringConfigFile();
		std::string fClusteringConfigFileName;		
		
		QGroupBox* groupBox_FOFClustering;

		QComboBox* comboBox_inputFile;
		QPushButton* pushButton_inputFile;	
		QComboBox* comboBox_outputFile;
		QPushButton* pushButton_outputFile;	

		QCheckBox* checkBox_waitPOCA;
		QSpinBox* spinbox_Verbosity;
		QCheckBox* checkBox_useQueueRun;
	

		QDoubleSpinBox* spinbox_epsilon;
		QDoubleSpinBox* spinbox_minNpts;

		QCheckBox* checkBox_useScatteringAngleCut;
		QDoubleSpinBox* spinbox_minScatteringAngle;
		QDoubleSpinBox* spinbox_maxScatteringAngle;
	
		QCheckBox* checkBox_maxNEvents;
		QSpinBox* spinbox_maxNEvents;

		QSpinBox* spinbox_NEventThreshold;
		QDoubleSpinBox* spinbox_alarmThreshold;
		QCheckBox* checkBox_useThetaWeight;
		QDoubleSpinBox* spinbox_thetaWeight;
		QCheckBox* checkBox_useAverageSignal;

		QCheckBox* checkBox_useEnergyCut;
		QDoubleSpinBox* spinbox_minEnergy;
		QDoubleSpinBox* spinbox_maxEnergy;

};

#endif
