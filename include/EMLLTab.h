#ifndef EMLL_TAB_H
#define EMLL_TAB_H

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


class EMLLTab : public QWidget {
     
	Q_OBJECT

 	public:
  	EMLLTab(QWidget *parent = 0);
 
	public slots:

		void BrowseInputFile();
		void BrowseOutputFile();
		void BrowseClusteringFile();

	signals:
			void failure();
		

	public:

		void SetEMLLOptions();
		
	private:

		bool DumpEMLLConfigFile();
		std::string fEMLLConfigFileName;		
		

		QGroupBox* groupBox_EMLL;
		QComboBox* comboBox_inputFile;
		QPushButton* pushButton_inputFile;	
		QComboBox* comboBox_outputFile;
		QPushButton* pushButton_outputFile;	

		QCheckBox* checkBox_useQueueRun;

		QCheckBox* checkBox_useParallelAlgorithm;
		QSpinBox* spinbox_NParallelProc;

		QSpinBox* spinbox_NLLIteration;
		QScienceSpinBox* spinbox_initScatteringDensity;
		QDoubleSpinBox* spinbox_referenceMomentum;
		QDoubleSpinBox* spinbox_averageMomentum;
	
		QCheckBox* checkBox_waitClustering;
		QComboBox* comboBox_clusteringFile;
		QPushButton* pushButton_clusteringFile;	

		QCheckBox* checkBox_maxNEvents;
		QSpinBox* spinbox_maxNEvents;
		QSpinBox* spinbox_Verbosity;

		QCheckBox* checkBox_useDetectorCovariance;	
		QScienceSpinBox* spinbox_Ex;
		QScienceSpinBox* spinbox_Ey;
		QScienceSpinBox* spinbox_Exy;
		QCheckBox* checkBox_useRegularization;
		QScienceSpinBox* spinbox_regularizationParameter;
		QCheckBox* checkBox_useMedianUpdate;
		QCheckBox* checkBox_useResidualStopping;
		QDoubleSpinBox* spinbox_residualGoal;
		QSpinBox* spinbox_nIterForResidualStopping;
		QCheckBox* checkBox_useDeltaLLStopping;
		QDoubleSpinBox* spinbox_deltaLLGoal;
		QSpinBox* spinbox_nIterForDeltaLLStopping;

		QCheckBox* checkBox_useScatteringAngleCut;
		QDoubleSpinBox* spinbox_minScatteringAngle;
		QDoubleSpinBox* spinbox_maxScatteringAngle;
		QCheckBox* checkBox_useDisplacementCut;
		QDoubleSpinBox* spinbox_minDisplacement;
		QDoubleSpinBox* spinbox_maxDisplacement;
		QCheckBox* checkBox_useTopBottomEvents;
		QCheckBox* checkBox_useSideEvents;
		QCheckBox* checkBox_usePOCAInLL;

		QCheckBox* checkBox_useTrueMomentum;
		QCheckBox* checkBox_useEnergyCut;
		QDoubleSpinBox* spinbox_minEnergy;
		QDoubleSpinBox* spinbox_maxEnergy;

		QCheckBox* checkBox_smearTimeOfFlight;
		QDoubleSpinBox* spinbox_timeOfFlightResolution;

		QCheckBox* checkBox_useTrackSpeedCut;
		QDoubleSpinBox* spinbox_minTrackSpeed;
		QDoubleSpinBox* spinbox_maxTrackSpeed;

		QSpinBox* spinbox_nMinEventInVoxelForUpdate;
		QCheckBox* checkBox_useFilterVoxelEvents;

		QCheckBox* checkBox_useQuantileUpdate;
		QDoubleSpinBox* spinbox_quantileProb;
		
		QDoubleSpinBox* spinbox_alarmThreshold;
		QSpinBox* spinbox_NEventThreshold;
		QComboBox* comboBox_drawStyle;

};

#endif
