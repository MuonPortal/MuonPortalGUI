#ifndef ACF_TAB_H
#define ACF_TAB_H

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


class ACFTab : public QWidget {
     
	Q_OBJECT

 	public:
  	ACFTab(QWidget *parent = 0);
 
	public slots:
		
		void BrowseInputFile();	
		void BrowseOutputFile();	
		void BrowseInputRandomFile();

	public:

		void SetACFOptions();
		bool DumpACFConfigFile();
		
	private:

		std::string fACFConfigFileName;
		
		QGroupBox* groupBox_ACF;

		QComboBox* comboBox_inputFile;
		QPushButton* pushButton_inputFile;
		QComboBox* comboBox_inputRandomFile;
		QPushButton* pushButton_inputRandomFile;		
		QComboBox* comboBox_outputFile;
		QPushButton* pushButton_outputFile;	
	
		QCheckBox* checkBox_waitPOCA;
		QCheckBox* checkBox_useQueueRun;
	
		
		QSpinBox* spinbox_Verbosity;
		QDoubleSpinBox* spinbox_minRScale;
		QDoubleSpinBox* spinbox_maxRScale;
		QDoubleSpinBox* spinbox_VoxelSizeX;
		QDoubleSpinBox* spinbox_VoxelSizeY;
		QDoubleSpinBox* spinbox_VoxelSizeZ;
		QSpinBox* spinbox_NCrownX;
		QSpinBox* spinbox_NCrownY;
		QSpinBox* spinbox_NCrownZ;

		QCheckBox* checkBox_maxNEvents;
		QSpinBox* spinbox_maxNEvents;

		QCheckBox* checkBox_useScatteringAngleCut;
		QDoubleSpinBox* spinbox_minScatteringAngle;
		QDoubleSpinBox* spinbox_maxScatteringAngle;

		QDoubleSpinBox* spinbox_alarmThreshold;
		QCheckBox* checkBox_useThetaWeight;
		QDoubleSpinBox* spinbox_thetaWeight;

		QCheckBox* checkBox_useEnergyCut;
		QDoubleSpinBox* spinbox_minEnergy;
		QDoubleSpinBox* spinbox_maxEnergy;

};

#endif
