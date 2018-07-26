// ******************************************************************************
// * License and Disclaimer                                                     *
// *                                                                            *
// * Copyright 2018 Simone Riggi																			          *
// *																																	          *
// * This file is part of MuonPortalGUI																          *
// * MuonPortalGUI is free software: you can redistribute it and/or modify it   *
// * under the terms of the GNU General Public License as published by          *
// * the Free Software Foundation, either * version 3 of the License,           *
// * or (at your option) any later version.                                     *
// * MuonPortalGUI is distributed in the hope that it will be useful, but 			*
// * WITHOUT ANY WARRANTY; without even the implied warranty of                 * 
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                       *
// * See the GNU General Public License for more details. You should            * 
// * have received a copy of the GNU General Public License along with          * 
// * MuonPortalGUI. If not, see http://www.gnu.org/licenses/.                   *
// ******************************************************************************

/**
* @file VolumeRenderingViewer.h
* @class VolumeRenderingViewer
* @brief Define the volume rendering VTK viewer
*
* @author S. Riggi
* @date 23/03/2012
*/
#ifndef VolumeRenderingViewer_h
#define VolumeRenderingViewer_h 1

#include <ConfigParser.h>
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

#include <QMainWindow>
#include <QWidget>
#include <QAction>
#include <QVTKWidget.h>

#include <vtkActor.h>
#include <vtkScalarBarActor.h>
#include <vtkVolume.h>
#include <vtkPiecewiseFunction.h>
#include <vtkLookupTable.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkImageData.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkImageReslice.h>

#include <vtkImageActor.h>
#include <vtkImageMapToColors.h>
#include <vtkMatrix4x4.h>
#include <vtkImageReader2.h>
#include <vtkImageViewer2.h>
#include <vtkImageResliceMapper.h>
#include <vtkImageProperty.h>
#include <vtkImageSlice.h>
#include <vtkStructuredPoints.h>
#include <vtkCutter.h>
#include <vtkPlane.h>
#include <vtkImageMapper.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkActor2D.h>
#include <vtkContourFilter.h>

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
class QSlider;
class QMenu;
class QAction;
class QListWidget;

namespace MuonPortalNS {

class VolumeRenderingViewer: public QObject {
    
	Q_OBJECT

  public:
		
		/** 
		\brief Class constructor: initialize structures.
 		*/
		VolumeRenderingViewer();
		/**
		* \brief Class destructor: free allocated memory
		*/
   ~VolumeRenderingViewer();

	public:

		void SetInputFileName(std::string filename){fInputFileName= filename;}
		void SetOutputImageFileName(std::string filename){fOutputImageFileName= filename;}
		void SetDataRange(double min,double max){fDataMin=min;fDataMax=max;}
		void ReadData();
		
		

	private:
	
		double GetTransferFunction(double x,double x0,double slope,double baseline);
		void SetPalette();
		void ReadPaletteFile();
		void ReadInfoData();
		void ReadPointsData();
		void ReadVolumeData();
		void ReadContainerData();
		void BuildVTKObjects();
		void BuildGUIObjects();
		void Init();
		void Filter();
		void DumpInfo();		

	public slots:
		void process();
		
		void Draw(int status=0);
		void SetTransferFunction(bool);
		void SetTransferFunctionOptions();
		void SetCameraOptions();
		void SetCameraDefaultOptions();
		void SetDrawOptions();
		void BrowseInputPaletteFile();
		void SetDataOptions();
		
		void SetVolumeSlice(int);
		void SetVolumeSliceXY(int);
		void SetVolumeSliceXZ(int);
		void SetVolumeSliceYZ(int);
		void SetVolumeSliceOrientation();
		void SetIsoSurfaceValue1(double);
		void SetIsoSurfaceValue2(double);

  	signals:
			void finished();
	    void error();
			void statusMessage(QString msg);

	private slots:

		void Open();
    void Save();
		void Close();
    void About();
		void Update();	


	private:

		QMainWindow* window;
		QWidget* widget;
		QVTKWidget* vtkWidget;
		QVTKWidget* vtkWidget2;	
		//vtkRenderer* renderer;
		QListWidget* infoWidget;
	
		
		TFile* inputFile;
		TTree* info;
		int nVoxelX;
		int nVoxelY;
		int nVoxelZ;
		double XMin;
		double XMax;
		double YMin;
		double YMax;
		double ZMin;
		double ZMax;

		TTree* data;		
		double xx;
		double yy;
		double zz;
		double theta;

		std::string fContainerFileName;
		std::string fContainerFileName_fullpath;

		bool fIsFileNameEmpty;
		bool fIsCorruptedFile;
		bool fIsVolumeDataAvailable;
		bool fIsPointDataAvailable;
		bool fIsContainerDataAvailable;
		bool fDrawVolume;
		bool fDrawPoints;
		bool fDrawBoundingBox;
		bool fDrawPaletteBar;
		bool fDrawContainer;
		bool fDrawVolumeSlice;
		bool fDrawVolumeSliceXY;
		bool fDrawVolumeSliceXZ;
		bool fDrawVolumeSliceYZ;
		
		
		bool fIsCameraChanged;
		double fCameraAzimuth;
		double fCameraElevation;
		double fCameraDefaultAzimuth;
		double fCameraDefaultElevation;
		
		vtkImageReader2* sliceImageReader;
		vtkImageViewer2* imageSliceViewer;
		vtkImageMapToColors* imageSliceColor;
		vtkImageActor* imageSliceActor;
		vtkImageResliceMapper* imageSliceMapper;
		vtkImageProperty* imageSliceProperty;

		vtkActor* containerActor;
		vtkActor* boundingBoxActor;
		vtkScalarBarActor* paletteBarActor;
		vtkVolume* volume;
		vtkVolumeRayCastMapper* volumeMapper;
		

		vtkVolumeProperty* volumeProperty;
		vtkImageData* image;
		vtkImageData* imageScaled;		
		
		vtkImageReslice* imageSlice;
		vtkImageSlice* imageSlice2;
		vtkStructuredPoints* imageSlicePoints;
		
		vtkPlane* slicingPlane;		
		vtkCutter* planeCutter;
		vtkActor* slicingPlaneActor;
		vtkPolyDataMapper* planeCutterMapper;
		
		vtkPlane* slicingPlaneXY;		
		vtkCutter* planeCutterXY;
		vtkActor* slicingPlaneActorXY;
		vtkPolyDataMapper* planeCutterMapperXY;
		vtkPlane* slicingPlaneXZ;		
		vtkCutter* planeCutterXZ;
		vtkActor* slicingPlaneActorXZ;
		vtkPolyDataMapper* planeCutterMapperXZ;
		vtkPlane* slicingPlaneYZ;		
		vtkCutter* planeCutterYZ;
		vtkActor* slicingPlaneActorYZ;		
		vtkPolyDataMapper* planeCutterMapperYZ;
		
		vtkPolyData* pointData;
		vtkCellArray* newVerts;
		vtkActor* pointDataActor;
		vtkPolyDataMapper* pointDataMapper;
		vtkPiecewiseFunction* opacityTransferFunction;
		//vtkPiecewiseFunction* opacityTransferFunction_point;
		vtkColorTransferFunction* colorTransferFunction;
		//vtkColorTransferFunction* colorTransferFunction_point;
		vtkLookupTable* lut;
		vtkLookupTable* lut_slice;
		//vtkLookupTable* lut_point;
		bool fUseOpacityTransferFunction;
		double fOpacityTransferFunctionMean;
		double fOpacityTransferFunctionSlope;
		double fOpacityTransferFunctionBaseline;
		int fNMinEvents;
		double fLgScalarMinCut;
		double fLgScalarMaxCut;
		bool fUseLogScalars;

		vtkContourFilter* contourFilter1;
		vtkContourFilter* contourFilter2;
		vtkPolyDataMapper* contourMapper1;
		vtkPolyDataMapper* contourMapper2;
		vtkProperty* contourProperty1;
		vtkProperty* contourProperty2;
		vtkActor* contourActor1;
		vtkActor* contourActor2;
		

		QPushButton* pushButton_Exit;
		QPushButton* pushButton_Save;
		QPushButton* pushButton_Update;
	
		QGroupBox* groupBox_options;
		QCheckBox* checkBox_showVolume;
		QCheckBox* checkBox_showVolumeSlices;
		QCheckBox* checkBox_showPoints;
		QCheckBox* checkBox_showContainer;
		QCheckBox* checkBox_showBoundingBox;
		QCheckBox* checkBox_showPalette;
			
		QGroupBox* groupBox_dataOptions;
		QSpinBox* spinbox_nMinEvents;
		QDoubleSpinBox* spinbox_scalarMinCut;
		QDoubleSpinBox* spinbox_scalarMaxCut;
		QCheckBox* checkBox_useLogScalars;

		QGroupBox* groupBox_paletteOptions;
		QCheckBox* checkBox_usePaletteTransferFunction;

		QGroupBox* groupBox_slice;
		QGroupBox* groupBox_sliceOptions;
		QSlider* slider_slice;
		QSlider* slider_sliceXY;
		QSlider* slider_sliceXZ;
		QSlider* slider_sliceYZ;
		int fSlicePosition;
		int fMinSlicePosition;
		int fMaxSlicePosition;
		QRadioButton* radioButton_sliceXY;	
		QRadioButton* radioButton_sliceXZ;	
		QRadioButton* radioButton_sliceYZ;	
		QCheckBox* checkBox_sliceXY;	
		QCheckBox* checkBox_sliceXZ;	
		QCheckBox* checkBox_sliceYZ;
	
		bool fDrawIsoSurface;
		bool fDrawIsoSurface1;
		bool fDrawIsoSurface2;
		double fIsoSurfaceScalar1;	
		double fIsoSurfaceScalar2;
		double fIsoSurfaceScalarWidth;
		QGroupBox* groupBox_IsoSurfaceOptions;
		QGroupBox* groupBox_IsoSurface;
		QCheckBox* checkBox_showIsoSurface;
		QCheckBox* checkBox_showIsoSurface1;
		QCheckBox* checkBox_showIsoSurface2;
		QDoubleSpinBox* spinbox_isosurfaceValue1;
		QDoubleSpinBox* spinbox_isosurfaceValue2;			
	
		QCheckBox* checkBox_alphaFilter;
		QDoubleSpinBox* spinbox_alphaFilter;
		QSpinBox* spinbox_ncrownX;
		QSpinBox* spinbox_ncrownY;
		QSpinBox* spinbox_ncrownZ;
		bool fUseAlphaFilter;
		double fFilterAlpha;
		int fNCrownX;
		int fNCrownY;
		int fNCrownZ;
	

		QComboBox* comboBox_inputPaletteFile;
		QCheckBox* checkBox_inputPaletteFile;
		QPushButton* pushButton_inputPaletteFile;
		bool fUsePaletteFile;
		std::string fPaletteFileName;
		int fNColorsInPalette;
		std::vector<double> fColorPaletteStep;
		std::vector<double> fColorPaletteR;
		std::vector<double> fColorPaletteG;
		std::vector<double> fColorPaletteB;
		std::vector<double> fColorPaletteA;
		
		QDoubleSpinBox* spinbox_paletteFcnMean;
		QDoubleSpinBox* spinbox_paletteFcnSlope;
		QDoubleSpinBox* spinbox_paletteFcnBaseline;
		QSlider* slider_paletteFcnMean;
		QSlider* slider_paletteFcnSlope;
		QSlider* slider_paletteFcnBaseline;
		
		QGroupBox* groupBox_cameraOptions;
		QRadioButton* radioButton_cameraXY;	
		QRadioButton* radioButton_cameraXZ;	
		QRadioButton* radioButton_cameraYZ;	
		QRadioButton* radioButton_cameraDefault;	
		//QPushButton* pushButton_cameraDefault;

		
		QMenu* fileMenu;
		QMenu* optionMenu;
    QMenu* helpMenu;
		QMenu* cameraMenu;
		QMenu* isosurfaceMenu;
		QMenu* slicingPlaneMenu;
		QMenu* colorPaletteMenu;
		QAction* openAct;
    QAction* saveAct;
    QAction* saveAsAct;
    QAction* exitAct;
    QAction* aboutAct;
		QAction* updateAct;
		QAction* paletteOpenAct;
    QToolBar* fileToolBar;

		QAction* showVolumeAct;
		QAction* showPointsAct;
		QAction* showContainerAct;
		QAction* showBoundingBoxAct;
		QAction* showPaletteAct;
		QAction* showIsoSurfaceAct;
		QAction* showPlaneSlicesAct;
		QAction* cameraAct;
		QActionGroup* cameraGroupAct;
		QAction* cameraXYAct;
		QAction* cameraXZAct;
		QAction* cameraYZAct;
		QAction* cameraDefaultAct;
		
		QDoubleSpinBox* spinbox_signalThreshold;
		double fSignalThreshold;
		std::vector<TVector3> fThreatPosList;
		std::vector<double> fThreatSignalList;

		std::string fInputFileName;
		std::string fOutputImageFileName;

		TH3D* fRecMap3D;
		TH3D* fFilterRecMap3D;
		TH3D* fDrawRecMap3D;
		
		TH3D* fNEventMap3D;
		bool fUseDataRange;
		double fDataMin;
		double fDataMax;
		double fVolumeDataMin;
		double fVolumeDataMax;
		double fPointDataMin;
		double fPointDataMax;

		bool fIsFailure;
		
		bool fUpdateDraw;
		bool fUpdateDataRead;
		bool fUpdatePrint;
		

};//close class

}//close namespace

#endif


