#include <VolumeRenderingViewer.h>
#include <Gui.h>

#include <QtGui>

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QWidget>
#include <QVTKWidget.h>

#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkQtTableView.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSphereSource.h>
#include <vtkCubeSource.h>
#include <vtkSmartPointer.h>

#include "vtkRenderWindowInteractor.h"
#include "vtkStructuredPointsReader.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkVolumeRayCastMapper.h"
#include "vtkVolume.h"
#include "vtkColorTransferFunction.h"

#include "vtkImageData.h" 
#include "vtkStructuredPoints.h"
#include <vtkSmartPointer.h>
#include <vtkRectilinearGridReader.h>
#include <vtkImageCast.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkImageMathematics.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkLookupTable.h>
#include <vtkScalarBarActor.h>
#include <vtkOutlineCornerFilter.h>
#include <vtkOutlineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkVRMLImporter.h>
#include <vtkTransform.h>
#include <vtkImageReslice.h>
#include <vtkCamera.h>
#include <vtkGL2PSExporter.h>
#include <vtkPNGWriter.h>
#include <vtkWindowToImageFilter.h>

#include <vtkRendererCollection.h>

#include <vtkPolyDataReader.h>
#include <vtkProperty.h>
#include <vtkCellArray.h>

//## ROOT classes
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TF1.h>
#include <TH2.h>
#include <TLegend.h>
#include <TCut.h>
#include <TEventList.h>
#include <TMath.h>
#include <TPad.h>
#include <TVirtualPad.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TMath.h>
#include <TRandom3.h>
#include <TVector3.h>
#include <TVirtualFitter.h>
#include <TPolyLine3D.h>
#include <TGeoTrack.h>
#include <TGraph.h>
#include <TGraph2D.h>
#include <TColor.h>
#include <TRandom.h>
#include <TAttImage.h>
#include <TPolyMarker3D.h>
#include <TView3D.h>

#include <TApplication.h>
#include <TRootEmbeddedCanvas.h>



//## standard classes
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <math.h>
#include <assert.h>
#include <utility>



using namespace std;


VolumeRenderingViewer::VolumeRenderingViewer() {

	
	//## Build VTK objects	
	BuildVTKObjects();

	//## Build objects in GUI
	BuildGUIObjects();

   	
}//close constructor


VolumeRenderingViewer::~VolumeRenderingViewer(){

}//close destructor


void VolumeRenderingViewer::BuildVTKObjects(){

	//## Init VTK objects
	opacityTransferFunction= vtkPiecewiseFunction::New();
	colorTransferFunction= vtkColorTransferFunction::New();
	lut= vtkLookupTable::New();
	lut_slice= vtkLookupTable::New();

	pointData= vtkPolyData::New();
	newVerts = vtkCellArray::New();
	pointDataMapper = vtkPolyDataMapper::New();
	pointDataActor = vtkActor::New();	
	image = vtkImageData::New();	
	imageScaled= vtkImageData::New();	
	volumeMapper = vtkVolumeRayCastMapper::New();
	boundingBoxActor = vtkActor::New();
	containerActor = vtkActor::New();
	imageSliceActor = vtkImageActor::New();	
	imageSliceColor = vtkImageMapToColors::New();
  imageSliceViewer = vtkImageViewer2::New();	
	imageSlice2 = vtkImageSlice::New();
	imageSliceMapper = vtkImageResliceMapper::New();
	imageSliceProperty = vtkImageProperty::New();
	sliceImageReader = vtkImageReader2::New();

	volumeProperty = vtkVolumeProperty::New();
	volume = vtkVolume::New();
	paletteBarActor= vtkScalarBarActor::New();

	//renderer= vtkRenderer::New();
	imageSlicePoints= vtkStructuredPoints::New(); 

	slicingPlane = vtkPlane::New();
	planeCutter = vtkCutter::New();
	slicingPlaneActor = vtkActor::New();
	planeCutterMapper = vtkPolyDataMapper::New();

	slicingPlaneXY = vtkPlane::New();
	planeCutterXY = vtkCutter::New();
	slicingPlaneActorXY = vtkActor::New();
	planeCutterMapperXY = vtkPolyDataMapper::New();

	slicingPlaneXZ = vtkPlane::New();
	planeCutterXZ = vtkCutter::New();
	slicingPlaneActorXZ = vtkActor::New();
	planeCutterMapperXZ = vtkPolyDataMapper::New();

	slicingPlaneYZ = vtkPlane::New();
	planeCutterYZ = vtkCutter::New();
	slicingPlaneActorYZ = vtkActor::New();	
	planeCutterMapperYZ = vtkPolyDataMapper::New();

	contourFilter1= vtkContourFilter::New();
	contourFilter2= vtkContourFilter::New();
	contourMapper1= vtkPolyDataMapper::New();
	contourMapper2= vtkPolyDataMapper::New();
	contourProperty1= vtkProperty::New();
	contourProperty2= vtkProperty::New();
	contourActor1= vtkActor::New();
	contourActor2= vtkActor::New();

}//close VolumeRenderingViewer::BuildVTKObjects()


void VolumeRenderingViewer::BuildGUIObjects(){

	//## Define windows and widgets
	window= new QMainWindow(0);	
	window->setObjectName(QString::fromUtf8("window"));
	window->setFixedSize(1100, 700);
	//window->setFixedSize(1100, 600);
	//window->setFixedSize(2000, 600);

	widget= new QWidget(window);
  widget->setObjectName(QString::fromUtf8("widget"));
	widget->setFixedSize(1100, 700);
	//widget->setFixedSize(1100, 600);
	//widget->setFixedSize(1100, 600);
	
  vtkWidget= new QVTKWidget(widget);
	vtkWidget->setObjectName(QString::fromUtf8("vtkWidget"));
  vtkWidget->setGeometry(QRect(10,20, 700, 400));
  vtkWidget->setFixedSize(700,500);
	
	/*
	vtkWidget2= new QVTKWidget(widget);
	vtkWidget2->setObjectName(QString::fromUtf8("vtkWidget2"));
  vtkWidget2->setGeometry(QRect(10,20, 700, 400));
  vtkWidget2->setFixedSize(700,500);
	*/

	infoWidget = new QListWidget(widget);
	infoWidget->setObjectName(QString::fromUtf8("infoWidget"));
	//infoWidget->setGeometry(QRect(10,20, 700, 400));
  infoWidget->setFixedSize(700,100);
	
	//## Define actions and menus
	QString dir_name= QString::fromAscii(qgetenv("MUPORTAL_GUI_ROOT").data());
	QDir dir= QDir(dir_name);
	if(!dir.exists()){
		cerr<<"VolumeRenderingViewer::BuildGUIObjects(): ERROR: Cannot get environent variable MUPORTAL_GUI_ROOT...exit!"<<endl;
		exit(1);
	}
	std::string GUI_BASE_DIR= std::string(qPrintable(dir_name));

	std::string open_imagepath= GUI_BASE_DIR + std::string("/share/openFileButton.png");
	std::string save_imagepath= GUI_BASE_DIR + std::string("/share/saveButton.png");
	std::string exit_imagepath= GUI_BASE_DIR + std::string("/share/exitButton.png");
	std::string camera_imagepath= GUI_BASE_DIR + std::string("/share/cameraButton.png");
	std::string palette_imagepath= GUI_BASE_DIR + std::string("/share/paletteButton.png");
	std::string update_imagepath= GUI_BASE_DIR + std::string("/share/updateButton.png");
			
	openAct = new QAction(QIcon(open_imagepath.c_str()), tr("&Open..."), this);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip(tr("Open an existing file"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(Open()));

  saveAct = new QAction(QIcon(save_imagepath.c_str()), tr("&Save"), this);
  saveAct->setShortcuts(QKeySequence::Save);
  saveAct->setStatusTip(tr("Save the document to disk"));
 	connect(saveAct, SIGNAL(triggered()), this, SLOT(Save()));

  exitAct = new QAction(QIcon(exit_imagepath.c_str()),tr("&Exit"), this);
  exitAct->setShortcuts(QKeySequence::Quit);
  exitAct->setStatusTip(tr("Exit the application"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(Close()));

	updateAct = new QAction(QIcon(update_imagepath.c_str()),tr("&Update"), this);
  updateAct->setStatusTip(tr("Update the drawing"));
  connect(updateAct, SIGNAL(triggered()), this, SLOT(Update()));


	paletteOpenAct = new QAction(QIcon(palette_imagepath.c_str()), tr("&Open..."), this);
  paletteOpenAct->setShortcuts(QKeySequence::Open);
  paletteOpenAct->setStatusTip(tr("Open a palette file"));
  connect(paletteOpenAct, SIGNAL(triggered()), this, SLOT(BrowseInputPaletteFile()));

	aboutAct = new QAction(tr("&About"), this);
  aboutAct->setStatusTip(tr("Show the application's About box"));
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(About()));

	showVolumeAct = new QAction(tr("&Volume Rendering"), this);
  showVolumeAct->setStatusTip(tr("Turn on/off the volume rendering"));
	showVolumeAct->setCheckable(true);
  connect(showVolumeAct, SIGNAL(triggered()), this, SLOT(SetDrawOptions()));

	showPointsAct = new QAction(tr("&Points"), this);
  showPointsAct->setStatusTip(tr("Turn on/off the point drawing"));
	showPointsAct->setCheckable(true);
  connect(showPointsAct, SIGNAL(triggered()), this, SLOT(SetDrawOptions()));

	showIsoSurfaceAct = new QAction(tr("&IsoSurface"), this);
  showIsoSurfaceAct->setStatusTip(tr("Turn on/off the isosurface drawing"));
	showIsoSurfaceAct->setCheckable(true);
  connect(showIsoSurfaceAct, SIGNAL(triggered()), this, SLOT(SetDrawOptions()));

	showPlaneSlicesAct = new QAction(tr("&Slicing Planes"), this);
  showPlaneSlicesAct->setStatusTip(tr("Turn on/off the plane slice drawing"));
	showPlaneSlicesAct->setCheckable(true);
  connect(showPlaneSlicesAct, SIGNAL(triggered()), this, SLOT(SetDrawOptions()));

	showContainerAct = new QAction(tr("&Container Model"), this);
  showContainerAct->setStatusTip(tr("Turn on/off the container model drawing"));
	showContainerAct->setCheckable(true);
  connect(showContainerAct, SIGNAL(triggered()), this, SLOT(SetDrawOptions()));

	showBoundingBoxAct = new QAction(tr("&Bounding Box"), this);
  showBoundingBoxAct->setStatusTip(tr("Turn on/off the bounding box drawing"));
	showBoundingBoxAct->setCheckable(true);
  connect(showBoundingBoxAct, SIGNAL(triggered()), this, SLOT(SetDrawOptions()));
	
	showPaletteAct = new QAction(tr("&Palette Bar"), this);
  showPaletteAct->setStatusTip(tr("Turn on/off the palette drawing"));
	showPaletteAct->setCheckable(true);
  connect(showPaletteAct, SIGNAL(triggered()), this, SLOT(SetDrawOptions()));


	
	cameraXYAct = new QAction(tr("&XY"), this);
  cameraXYAct->setStatusTip(tr("XY Camera"));
	cameraXYAct->setCheckable(true);
  connect(cameraXYAct, SIGNAL(triggered()), this, SLOT(SetCameraOptions()));

	cameraXZAct = new QAction(tr("&XZ"), this);
  cameraXZAct->setStatusTip(tr("XZ Camera"));
	cameraXZAct->setCheckable(true);
  connect(cameraXZAct, SIGNAL(triggered()), this, SLOT(SetCameraOptions()));

	cameraYZAct = new QAction(tr("&YZ"), this);
  cameraYZAct->setStatusTip(tr("YZ Camera"));
	cameraYZAct->setCheckable(true);
  connect(cameraYZAct, SIGNAL(triggered()), this, SLOT(SetCameraOptions()));

	cameraDefaultAct = new QAction(tr("&default"), this);
  cameraDefaultAct->setStatusTip(tr("Default Camera"));
	cameraDefaultAct->setCheckable(true);
  connect(cameraDefaultAct, SIGNAL(triggered()), this, SLOT(SetCameraOptions()));

	cameraGroupAct = new QActionGroup(this);
  cameraGroupAct->addAction(cameraXYAct);
  cameraGroupAct->addAction(cameraXZAct);
  cameraGroupAct->addAction(cameraYZAct);
  cameraGroupAct->addAction(cameraDefaultAct);
  cameraDefaultAct->setChecked(true);
	

	fileMenu = window->menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
  fileMenu->addSeparator();
	fileMenu->addAction(updateAct);
	fileMenu->addAction(paletteOpenAct);
	fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  window->menuBar()->addSeparator();

	optionMenu = window->menuBar()->addMenu(tr("&Options"));
	//optionMenu->addAction(showVolumeAct);
	//optionMenu->addAction(showPointsAct);
	//optionMenu->addAction(showIsoSurfaceAct);
	//optionMenu->addAction(showPlaneSlicesAct);
	//optionMenu->addAction(showContainerAct);
	//optionMenu->addAction(showBoundingBoxAct);
	//optionMenu->addAction(showPaletteAct);
  //optionMenu->addSeparator();
	
	
	checkBox_showVolume= new QCheckBox("Volume Rendering",optionMenu);
	checkBox_showVolume->setObjectName(QString::fromUtf8("checkBox_showVolume"));
	checkBox_showVolume->setChecked(true);

	checkBox_showVolumeSlices= new QCheckBox("Volume Slices",optionMenu);
	checkBox_showVolumeSlices->setObjectName(QString::fromUtf8("checkBox_showVolumeSlices"));
	checkBox_showVolumeSlices->setChecked(false);

	checkBox_showPoints= new QCheckBox("Points",optionMenu);
	checkBox_showPoints->setObjectName(QString::fromUtf8("checkBox_showPoints"));
	checkBox_showPoints->setChecked(false);

	checkBox_showIsoSurface= new QCheckBox("Iso Surface",optionMenu);
	checkBox_showIsoSurface->setObjectName(QString::fromUtf8("checkBox_showIsoSurface"));
	checkBox_showIsoSurface->setChecked(true);

	
	checkBox_showContainer= new QCheckBox("Container",optionMenu);
	checkBox_showContainer->setObjectName(QString::fromUtf8("checkBox_showContainer"));
	checkBox_showContainer->setChecked(false);

	checkBox_showBoundingBox= new QCheckBox("Bounding Box",optionMenu);
	checkBox_showBoundingBox->setObjectName(QString::fromUtf8("checkBox_showBoundingBox"));
	checkBox_showBoundingBox->setChecked(true);

	checkBox_showPalette= new QCheckBox("Palette Bar",optionMenu);
	checkBox_showPalette->setObjectName(QString::fromUtf8("checkBox_showPalette"));
	checkBox_showPalette->setChecked(true);

	QWidgetAction* VolumeRenderingAction = new QWidgetAction(optionMenu);
	VolumeRenderingAction->setDefaultWidget(checkBox_showVolume);

	QWidgetAction* PointsAction = new QWidgetAction(optionMenu);
	PointsAction->setDefaultWidget(checkBox_showPoints);
	
	QWidgetAction* VolumeSliceAction = new QWidgetAction(optionMenu);
	VolumeSliceAction->setDefaultWidget(checkBox_showVolumeSlices);

	QWidgetAction* IsoSurfaceAction = new QWidgetAction(optionMenu);
	IsoSurfaceAction->setDefaultWidget(checkBox_showIsoSurface);

	QWidgetAction* ContainerModelAction = new QWidgetAction(optionMenu);
	ContainerModelAction->setDefaultWidget(checkBox_showContainer);

	QWidgetAction* BoundingBoxAction = new QWidgetAction(optionMenu);
	BoundingBoxAction->setDefaultWidget(checkBox_showBoundingBox);

	QWidgetAction* PaletteBarAction = new QWidgetAction(optionMenu);
	PaletteBarAction->setDefaultWidget(checkBox_showPalette);

	optionMenu->addAction(VolumeRenderingAction);
	optionMenu->addAction(PointsAction);
	//optionMenu->addAction(VolumeSliceAction);
	//optionMenu->addAction(IsoSurfaceAction);
	
	//isosurface menu options
	isosurfaceMenu= optionMenu->addMenu(tr("&IsoSurface"));
	
	groupBox_IsoSurface = new QGroupBox(tr("IsoSurface"),isosurfaceMenu);	
	groupBox_IsoSurface->setCheckable(true);
	groupBox_IsoSurface->setChecked(false);

	checkBox_showIsoSurface1= new QCheckBox(tr("Contour 1"),isosurfaceMenu);
	checkBox_showIsoSurface1->setObjectName(QString::fromUtf8("checkBox_showIsoSurface1"));
	checkBox_showIsoSurface1->setChecked(true);

	checkBox_showIsoSurface2= new QCheckBox(tr("Contour 2"),isosurfaceMenu);
	checkBox_showIsoSurface2->setObjectName(QString::fromUtf8("checkBox_showIsoSurface2"));
	checkBox_showIsoSurface2->setChecked(false);

	QVBoxLayout* IsoSurfaceBoxLayout = new QVBoxLayout;
  IsoSurfaceBoxLayout->addWidget(checkBox_showIsoSurface1);
  IsoSurfaceBoxLayout->addWidget(checkBox_showIsoSurface2);
  IsoSurfaceBoxLayout->addStretch(1);
  groupBox_IsoSurface->setLayout(IsoSurfaceBoxLayout);

	QWidgetAction* IsoSurfaceBoxAction = new QWidgetAction(isosurfaceMenu);
	IsoSurfaceBoxAction->setDefaultWidget(groupBox_IsoSurface);

	QWidgetAction* IsoContour1Action = new QWidgetAction(isosurfaceMenu);
	IsoContour1Action->setDefaultWidget(checkBox_showIsoSurface1);
	
	QWidgetAction* IsoContour2Action = new QWidgetAction(isosurfaceMenu);
	IsoContour2Action->setDefaultWidget(checkBox_showIsoSurface2);

	isosurfaceMenu->addAction(IsoSurfaceBoxAction);
	isosurfaceMenu->addAction(IsoContour1Action);
	isosurfaceMenu->addAction(IsoContour2Action);

	
	//slicing plane menu options
	slicingPlaneMenu= optionMenu->addMenu(tr("&Slicing Plane"));
	
	groupBox_slice = new QGroupBox(tr("Slicing Plane"),slicingPlaneMenu);	
	groupBox_slice->setCheckable(true);
	groupBox_slice->setChecked(false);
  
	checkBox_sliceXY= new QCheckBox(tr("XY"),slicingPlaneMenu);
	checkBox_sliceXY->setObjectName(QString::fromUtf8("checkBox_sliceXY"));
	checkBox_sliceXY->setChecked(true);
		
	checkBox_sliceXZ= new QCheckBox(tr("XZ"),slicingPlaneMenu);
	checkBox_sliceXZ->setObjectName(QString::fromUtf8("checkBox_sliceXZ"));
	checkBox_sliceXZ->setChecked(false);

	checkBox_sliceYZ= new QCheckBox(tr("YZ"),slicingPlaneMenu);
	checkBox_sliceYZ->setObjectName(QString::fromUtf8("checkBox_sliceYZ"));
	checkBox_sliceYZ->setChecked(false);

	QVBoxLayout* SliceBoxLayout = new QVBoxLayout;
  SliceBoxLayout->addWidget(checkBox_sliceXY);
  SliceBoxLayout->addWidget(checkBox_sliceXZ);
  SliceBoxLayout->addWidget(checkBox_sliceYZ);
  SliceBoxLayout->addStretch(1);
  groupBox_slice->setLayout(SliceBoxLayout);

	QWidgetAction* SlicingPlaneGroupAction = new QWidgetAction(slicingPlaneMenu);
	SlicingPlaneGroupAction->setDefaultWidget(groupBox_slice);
	
	QWidgetAction* SlicingPlaneXYAction = new QWidgetAction(slicingPlaneMenu);
	SlicingPlaneXYAction->setDefaultWidget(checkBox_sliceXY);
	
	QWidgetAction* SlicingPlaneXZAction = new QWidgetAction(slicingPlaneMenu);
	SlicingPlaneXZAction->setDefaultWidget(checkBox_sliceXZ);

	QWidgetAction* SlicingPlaneYZAction = new QWidgetAction(slicingPlaneMenu);
	SlicingPlaneYZAction->setDefaultWidget(checkBox_sliceYZ);

	slicingPlaneMenu->addAction(SlicingPlaneGroupAction);
	slicingPlaneMenu->addAction(SlicingPlaneXYAction);
	slicingPlaneMenu->addAction(SlicingPlaneXZAction);
	slicingPlaneMenu->addAction(SlicingPlaneYZAction);


	

	//## Add other options to menu
	optionMenu->addAction(ContainerModelAction);
	optionMenu->addAction(BoundingBoxAction);
	optionMenu->addAction(PaletteBarAction);
	optionMenu->addSeparator();

	//palette menu options
	colorPaletteMenu= optionMenu->addMenu(QIcon(palette_imagepath.c_str()),tr("&Color Palette"));
		
	checkBox_inputPaletteFile= new QCheckBox(tr("Use Palette File?"),colorPaletteMenu);
	checkBox_inputPaletteFile->setObjectName(QString::fromUtf8("checkBox_inputPaletteFile"));
	checkBox_inputPaletteFile->setChecked(false);

	checkBox_usePaletteTransferFunction= new QCheckBox(tr("Use Transfer Function?"),colorPaletteMenu);
	checkBox_usePaletteTransferFunction->setObjectName(QString::fromUtf8("checkBox_usePaletteTransferFunction"));
	checkBox_usePaletteTransferFunction->setChecked(false);

	QWidgetAction* PaletteFileAction = new QWidgetAction(colorPaletteMenu);
	PaletteFileAction->setDefaultWidget(checkBox_inputPaletteFile);

	QWidgetAction* TransferFunctionAction = new QWidgetAction(colorPaletteMenu);
	TransferFunctionAction->setDefaultWidget(checkBox_usePaletteTransferFunction);

	colorPaletteMenu->addAction(PaletteFileAction);
	colorPaletteMenu->addAction(TransferFunctionAction);

	optionMenu->addSeparator();

	//camera menu options
	cameraMenu= optionMenu->addMenu(QIcon(camera_imagepath.c_str()),tr("&Camera"));
  //cameraMenu->addAction(cameraXYAct);
  //cameraMenu->addAction(cameraXZAct);
	//cameraMenu->addAction(cameraYZAct);
	//cameraMenu->addAction(cameraDefaultAct);
  
	radioButton_cameraXY= new QRadioButton(tr("XY"),cameraMenu);
	radioButton_cameraXY->setObjectName(QString::fromUtf8("radioButton_cameraXY"));
	radioButton_cameraXY->setChecked(false);

	radioButton_cameraXZ= new QRadioButton(tr("XZ"),cameraMenu);
	radioButton_cameraXZ->setObjectName(QString::fromUtf8("radioButton_cameraXZ"));
	radioButton_cameraXZ->setChecked(false);

	radioButton_cameraYZ= new QRadioButton(tr("YZ"),cameraMenu);
	radioButton_cameraYZ->setObjectName(QString::fromUtf8("radioButton_cameraYZ"));
	radioButton_cameraYZ->setChecked(false);

	radioButton_cameraDefault= new QRadioButton(tr("default"),cameraMenu);
	radioButton_cameraDefault->setObjectName(QString::fromUtf8("radioButton_cameraDefault"));
	radioButton_cameraDefault->setChecked(true);
	
	QWidgetAction* cameraXYAction = new QWidgetAction(cameraMenu);
	cameraXYAction->setDefaultWidget(radioButton_cameraXY);

	QWidgetAction* cameraXZAction = new QWidgetAction(cameraMenu);
	cameraXZAction->setDefaultWidget(radioButton_cameraXZ);

	QWidgetAction* cameraYZAction = new QWidgetAction(cameraMenu);
	cameraYZAction->setDefaultWidget(radioButton_cameraYZ);	

	QWidgetAction* cameraDefaultAction = new QWidgetAction(cameraMenu);
	cameraDefaultAction->setDefaultWidget(radioButton_cameraDefault);
	
	cameraMenu->addAction(cameraXYAction);
	cameraMenu->addAction(cameraXZAction);
	cameraMenu->addAction(cameraYZAction);
	cameraMenu->addAction(cameraDefaultAction);

	

	window->menuBar()->addSeparator();

  helpMenu = window->menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAct);
  

	//## Create toolbar
	fileToolBar = window->addToolBar(tr("File"));
  fileToolBar->addAction(openAct);
  fileToolBar->addAction(saveAct);
	fileToolBar->addSeparator();
	fileToolBar->addAction(updateAct);
	fileToolBar->addAction(paletteOpenAct);
	fileToolBar->addSeparator();
	fileToolBar->addAction(exitAct);

	
	//## Define buttons and other QT objects
	
	groupBox_dataOptions= new QGroupBox(tr("Data Options"));
	groupBox_dataOptions->setObjectName(QString::fromUtf8("groupBox_dataOptions"));
	groupBox_dataOptions->setStyleSheet("QGroupBox{font-size: 12px;font-weight: bold;}");
	groupBox_dataOptions->setCheckable(false);
	//groupBox_dataOptions->setVisible(true);

	QLabel* label_nMinEvents= new QLabel();
	label_nMinEvents->setObjectName(QString("label_nMinEvents"));
	label_nMinEvents->setText("Nmin");

	spinbox_nMinEvents= new QSpinBox;
	spinbox_nMinEvents->setObjectName(QString::fromUtf8("spinbox_nMinEvents"));
	spinbox_nMinEvents->setRange(0,1000);
  spinbox_nMinEvents->setValue(0);
	spinbox_nMinEvents->setSingleStep(1);
	spinbox_nMinEvents->setFixedWidth(120);

	QLabel* label_scalarCut= new QLabel();
	label_scalarCut->setObjectName(QString("label_scalarCut"));
	label_scalarCut->setText("lg(Scalar) min/max");

	spinbox_scalarMinCut= new QDoubleSpinBox;
	spinbox_scalarMinCut->setObjectName(QString::fromUtf8("spinbox_scalarMinCut"));
	spinbox_scalarMinCut->setRange(-99,99);
  spinbox_scalarMinCut->setValue(-99);//-8
	spinbox_scalarMinCut->setSingleStep(0.1);
	spinbox_scalarMinCut->setFixedWidth(120);

	spinbox_scalarMaxCut= new QDoubleSpinBox;
	spinbox_scalarMaxCut->setObjectName(QString::fromUtf8("spinbox_scalarMaxCut"));
	spinbox_scalarMaxCut->setRange(-99,99);
  spinbox_scalarMaxCut->setValue(99);//-2
	spinbox_scalarMaxCut->setSingleStep(0.1);
	spinbox_scalarMaxCut->setFixedWidth(120);


	checkBox_useLogScalars= new QCheckBox("Use Log10(Scalars)?");
	checkBox_useLogScalars->setObjectName(QString::fromUtf8("checkBox_useLogScalars"));
	checkBox_useLogScalars->setChecked(false);
	

	
	checkBox_alphaFilter= new QCheckBox(tr("Alpha Filter"));
	checkBox_alphaFilter->setObjectName(QString::fromUtf8("checkBox_alphaFilter"));
	checkBox_alphaFilter->setChecked(false);

	spinbox_alphaFilter= new QDoubleSpinBox;
	spinbox_alphaFilter->setObjectName(QString::fromUtf8("spinbox_alphaFilter"));
	spinbox_alphaFilter->setRange(0,1);
  spinbox_alphaFilter->setValue(0.04);
	spinbox_alphaFilter->setSingleStep(0.1);
	spinbox_alphaFilter->setFixedWidth(120);

	QLabel* label_ncrown= new QLabel();
	label_ncrown->setObjectName(QString("label_ncrown"));
	label_ncrown->setText("NCrown XYZ");

	spinbox_ncrownX= new QSpinBox;
	spinbox_ncrownX->setObjectName(QString::fromUtf8("spinbox_ncrownX"));
	spinbox_ncrownX->setRange(0,5);
  spinbox_ncrownX->setValue(1);
	spinbox_ncrownX->setSingleStep(1);
	spinbox_ncrownX->setFixedWidth(60);

	spinbox_ncrownY= new QSpinBox;
	spinbox_ncrownY->setObjectName(QString::fromUtf8("spinbox_ncrownY"));
	spinbox_ncrownY->setRange(0,5);
  spinbox_ncrownY->setValue(1);
	spinbox_ncrownY->setSingleStep(1);
	spinbox_ncrownY->setFixedWidth(60);

	spinbox_ncrownZ= new QSpinBox;
	spinbox_ncrownZ->setObjectName(QString::fromUtf8("spinbox_ncrownZ"));
	spinbox_ncrownZ->setRange(0,5);
  spinbox_ncrownZ->setValue(1);
	spinbox_ncrownZ->setSingleStep(1);
	spinbox_ncrownZ->setFixedWidth(60);

	QLabel* label_signalThreshold= new QLabel();
	label_signalThreshold->setObjectName(QString("label_signalThreshold"));
	label_signalThreshold->setText("Alert Threshold");

	spinbox_signalThreshold= new QDoubleSpinBox;
	spinbox_signalThreshold->setObjectName(QString::fromUtf8("spinbox_signalThreshold"));
	spinbox_signalThreshold->setRange(-1000,1000);
  spinbox_signalThreshold->setValue(-3.5);
	spinbox_signalThreshold->setSingleStep(0.1);
	spinbox_signalThreshold->setFixedWidth(120);

	groupBox_paletteOptions= new QGroupBox(tr("Palette Options"));
	groupBox_paletteOptions->setObjectName(QString::fromUtf8("groupBox_paletteOptions"));
	groupBox_paletteOptions->setStyleSheet("QGroupBox{font-size: 12px;font-weight: bold;}");
	groupBox_paletteOptions->setCheckable(false);
	//groupBox_paletteOptions->setVisible(true);
	
	

	comboBox_inputPaletteFile= new QComboBox;
	comboBox_inputPaletteFile->setObjectName(QString::fromUtf8("comboBox_inputPaletteFile"));
	comboBox_inputPaletteFile->setEditable(true);
	comboBox_inputPaletteFile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	comboBox_inputPaletteFile->addItem("script/paletteVisIVO2.dat");
	comboBox_inputPaletteFile->setFixedWidth(200);
	
	pushButton_inputPaletteFile= new QPushButton(tr("Browse"));
	pushButton_inputPaletteFile->setObjectName(QString::fromUtf8("pushButton_inputPaletteFile"));
	pushButton_inputPaletteFile->setFixedSize(QSize(60,30));
	
	
	
	QLabel* label_paletteFcnMean= new QLabel();
	label_paletteFcnMean->setObjectName(QString("label_paletteFcnMean"));
	label_paletteFcnMean->setText("mean");

	spinbox_paletteFcnMean= new QDoubleSpinBox;
	spinbox_paletteFcnMean->setObjectName(QString::fromUtf8("spinbox_paletteFcnMean"));
	spinbox_paletteFcnMean->setRange(0,255);
  spinbox_paletteFcnMean->setValue(50);
	spinbox_paletteFcnMean->setSingleStep(1);
	spinbox_paletteFcnMean->setFixedWidth(80);

	slider_paletteFcnMean = new QSlider(Qt::Horizontal);
	slider_paletteFcnMean->setFixedSize(130,30);
	slider_paletteFcnMean->setValue(50);
	slider_paletteFcnMean->setRange(0,255);
  slider_paletteFcnMean->setFocusPolicy(Qt::StrongFocus);
  slider_paletteFcnMean->setTickPosition(QSlider::TicksAbove);
  slider_paletteFcnMean->setTickInterval(10);
  slider_paletteFcnMean->setSingleStep(1);

	QLabel* label_paletteFcnSlope= new QLabel();
	label_paletteFcnSlope->setObjectName(QString("label_paletteFcnSlope"));
	label_paletteFcnSlope->setText("slope");

	spinbox_paletteFcnSlope= new QDoubleSpinBox;
	spinbox_paletteFcnSlope->setObjectName(QString::fromUtf8("spinbox_paletteFcnSlope"));
	spinbox_paletteFcnSlope->setRange(1.e-6,200);
  spinbox_paletteFcnSlope->setValue(10);
	spinbox_paletteFcnSlope->setSingleStep(1);
	spinbox_paletteFcnSlope->setFixedWidth(80);

	slider_paletteFcnSlope = new QSlider(Qt::Horizontal);
	slider_paletteFcnSlope->setFixedSize(130,30);
	slider_paletteFcnSlope->setValue(10);
	slider_paletteFcnSlope->setRange(1.e-6,200);
  slider_paletteFcnSlope->setFocusPolicy(Qt::StrongFocus);
  slider_paletteFcnSlope->setTickPosition(QSlider::TicksAbove);
  slider_paletteFcnSlope->setTickInterval(10);
  slider_paletteFcnSlope->setSingleStep(1);

	QLabel* label_paletteFcnBaseline= new QLabel();
	label_paletteFcnBaseline->setObjectName(QString("label_paletteFcnBaseline"));
	label_paletteFcnBaseline->setText("baseline");
	
	spinbox_paletteFcnBaseline= new QDoubleSpinBox;
	spinbox_paletteFcnBaseline->setObjectName(QString::fromUtf8("spinbox_paletteFcnSlope"));
	spinbox_paletteFcnBaseline->setRange(0,100);
  spinbox_paletteFcnBaseline->setValue(0.1);
	spinbox_paletteFcnBaseline->setSingleStep(0.1);
	spinbox_paletteFcnBaseline->setFixedWidth(80);

	slider_paletteFcnBaseline = new QSlider(Qt::Horizontal);
	slider_paletteFcnBaseline->setFixedSize(130,30);
	slider_paletteFcnBaseline->setValue(0.1);
	slider_paletteFcnBaseline->setRange(0,100);
  slider_paletteFcnBaseline->setFocusPolicy(Qt::StrongFocus);
  slider_paletteFcnBaseline->setTickPosition(QSlider::TicksAbove);
  slider_paletteFcnBaseline->setTickInterval(1);
  slider_paletteFcnBaseline->setSingleStep(0.1);


	
	
	groupBox_sliceOptions= new QGroupBox(tr("Slicing Plane Options"));
	groupBox_sliceOptions->setObjectName(QString::fromUtf8("groupBox_sliceOptions"));
	groupBox_sliceOptions->setStyleSheet("QGroupBox{font-size: 12px;font-weight: bold;}");
	groupBox_sliceOptions->setCheckable(false);
	//groupBox_sliceOptions->setVisible(true);

	fSlicePosition= 0;
	fMinSlicePosition= -30;
	fMaxSlicePosition= 30;

	
	QLabel* label_sliceXY= new QLabel();
	label_sliceXY->setObjectName(QString("label_sliceXY"));
	label_sliceXY->setText("Slice XY");

	slider_sliceXY= new QSlider(Qt::Horizontal);
	slider_sliceXY->setFixedSize(200,30);
	slider_sliceXY->setValue(fSlicePosition);
	slider_sliceXY->setRange(fMinSlicePosition,fMaxSlicePosition);
  slider_sliceXY->setFocusPolicy(Qt::StrongFocus);
  slider_sliceXY->setTickPosition(QSlider::TicksAbove);
  slider_sliceXY->setTickInterval(1);
  slider_sliceXY->setSingleStep(1);

	QLabel* label_sliceXZ= new QLabel();
	label_sliceXZ->setObjectName(QString("label_sliceXZ"));
	label_sliceXZ->setText("Slice XZ");

	slider_sliceXZ= new QSlider(Qt::Horizontal);
	slider_sliceXZ->setFixedSize(200,30);
	slider_sliceXZ->setValue(fSlicePosition);
	slider_sliceXZ->setRange(fMinSlicePosition,fMaxSlicePosition);
  slider_sliceXZ->setFocusPolicy(Qt::StrongFocus);
  slider_sliceXZ->setTickPosition(QSlider::TicksAbove);
  slider_sliceXZ->setTickInterval(1);
  slider_sliceXZ->setSingleStep(1);

	QLabel* label_sliceYZ= new QLabel();
	label_sliceYZ->setObjectName(QString("label_sliceYZ"));
	label_sliceYZ->setText("Slice YZ");

	slider_sliceYZ= new QSlider(Qt::Horizontal);
	slider_sliceYZ->setFixedSize(200,30);
	slider_sliceYZ->setValue(fSlicePosition);
	slider_sliceYZ->setRange(fMinSlicePosition,fMaxSlicePosition);
  slider_sliceYZ->setFocusPolicy(Qt::StrongFocus);
  slider_sliceYZ->setTickPosition(QSlider::TicksAbove);
  slider_sliceYZ->setTickInterval(1);
  slider_sliceYZ->setSingleStep(1);

	

	groupBox_IsoSurfaceOptions= new QGroupBox(tr("Isosurface Options"));
	groupBox_IsoSurfaceOptions->setObjectName(QString::fromUtf8("groupBox_IsoSurfaceOptions"));
	groupBox_IsoSurfaceOptions->setStyleSheet("QGroupBox{font-size: 12px;font-weight: bold;}");
	groupBox_IsoSurfaceOptions->setCheckable(false);
	//groupBox_IsoSurfaceOptions->setVisible(true);

	
	QLabel* label_isosurfaceValue1= new QLabel();
	label_isosurfaceValue1->setObjectName(QString("label_isosurfaceValue1"));
	label_isosurfaceValue1->setText("Iso Level 1");

	spinbox_isosurfaceValue1= new QDoubleSpinBox;
	spinbox_isosurfaceValue1->setObjectName(QString::fromUtf8("spinbox_isosurfaceValue1"));
	spinbox_isosurfaceValue1->setRange(0,255);
  spinbox_isosurfaceValue1->setValue(140);
	spinbox_isosurfaceValue1->setSingleStep(10);
	spinbox_isosurfaceValue1->setFixedWidth(100);

	
	QLabel* label_isosurfaceValue2= new QLabel();
	label_isosurfaceValue2->setObjectName(QString("label_isosurfaceValue2"));
	label_isosurfaceValue2->setText("Iso Level 2");

	spinbox_isosurfaceValue2= new QDoubleSpinBox;
	spinbox_isosurfaceValue2->setObjectName(QString::fromUtf8("spinbox_isosurfaceValue2"));
	spinbox_isosurfaceValue2->setRange(0,255);
  spinbox_isosurfaceValue2->setValue(60);
	spinbox_isosurfaceValue2->setSingleStep(10);
	spinbox_isosurfaceValue2->setFixedWidth(100);


	
  //## Set object actions	
	connect(checkBox_showContainer, SIGNAL(toggled(bool)), this, SLOT(SetDrawOptions()));
 	connect(checkBox_showBoundingBox, SIGNAL(toggled(bool)), this, SLOT(SetDrawOptions()));
 	connect(checkBox_showPalette, SIGNAL(toggled(bool)), this, SLOT(SetDrawOptions()));
 	connect(checkBox_showVolume, SIGNAL(toggled(bool)), this, SLOT(SetDrawOptions()));
	connect(checkBox_showVolumeSlices, SIGNAL(toggled(bool)), this, SLOT(SetDrawOptions()));
	connect(groupBox_IsoSurface, SIGNAL(toggled(bool)), this, SLOT(SetDrawOptions()));	
	connect(checkBox_showIsoSurface, SIGNAL(toggled(bool)), this, SLOT(SetDrawOptions()));
	connect(checkBox_showIsoSurface1, SIGNAL(toggled(bool)), this, SLOT(SetDrawOptions()));
	connect(checkBox_showIsoSurface2, SIGNAL(toggled(bool)), this, SLOT(SetDrawOptions()));
 	connect(checkBox_showPoints, SIGNAL(toggled(bool)), this, SLOT(SetDrawOptions()));

	connect(groupBox_slice, SIGNAL(toggled(bool)), this, SLOT(SetDrawOptions()));	
	connect(checkBox_sliceXY, SIGNAL(toggled(bool)), this, SLOT(SetDrawOptions()));
	connect(checkBox_sliceXZ, SIGNAL(toggled(bool)), this, SLOT(SetDrawOptions()));
	connect(checkBox_sliceYZ, SIGNAL(toggled(bool)), this, SLOT(SetDrawOptions()));
	
	connect(pushButton_inputPaletteFile, SIGNAL(clicked()), this, SLOT(BrowseInputPaletteFile()));
	connect(checkBox_usePaletteTransferFunction, SIGNAL(toggled(bool)), this, SLOT(SetTransferFunction(bool)));
	connect(slider_paletteFcnMean,SIGNAL(valueChanged(int)),this, SLOT(SetTransferFunctionOptions())); 
	connect(slider_paletteFcnSlope,SIGNAL(valueChanged(int)),this, SLOT(SetTransferFunctionOptions())); 
	connect(slider_paletteFcnBaseline,SIGNAL(valueChanged(int)),this, SLOT(SetTransferFunctionOptions())); 
	
	connect(slider_sliceXY, SIGNAL(valueChanged(int)), this,SLOT(SetVolumeSliceXY(int)));
	connect(slider_sliceXZ, SIGNAL(valueChanged(int)), this,SLOT(SetVolumeSliceXZ(int)));
	connect(slider_sliceYZ, SIGNAL(valueChanged(int)), this,SLOT(SetVolumeSliceYZ(int)));
	
	connect(spinbox_paletteFcnMean,SIGNAL(valueChanged(double)),this, SLOT(SetTransferFunctionOptions())); 
	connect(spinbox_paletteFcnSlope,SIGNAL(valueChanged(double)),this, SLOT(SetTransferFunctionOptions())); 
	connect(spinbox_paletteFcnBaseline,SIGNAL(valueChanged(double)),this, SLOT(SetTransferFunctionOptions()));
	connect(spinbox_nMinEvents,SIGNAL(valueChanged(int)),this, SLOT(SetDataOptions()));
	connect(spinbox_scalarMinCut,SIGNAL(valueChanged(double)),this, SLOT(SetDataOptions()));
	connect(spinbox_scalarMaxCut,SIGNAL(valueChanged(double)),this, SLOT(SetDataOptions()));
	connect(checkBox_useLogScalars,SIGNAL(toggled(bool)),this, SLOT(SetDataOptions()));

	
	connect(radioButton_cameraXY,SIGNAL(toggled(bool)),this, SLOT(SetCameraOptions())); 
	connect(radioButton_cameraXZ,SIGNAL(toggled(bool)),this, SLOT(SetCameraOptions())); 
	connect(radioButton_cameraYZ,SIGNAL(toggled(bool)),this, SLOT(SetCameraOptions())); 
	connect(radioButton_cameraDefault,SIGNAL(toggled(bool)),this, SLOT(SetCameraOptions())); 
		
	connect(spinbox_isosurfaceValue1,SIGNAL(valueChanged(double)),this, SLOT(SetIsoSurfaceValue1(double))); 	
	connect(spinbox_isosurfaceValue2,SIGNAL(valueChanged(double)),this, SLOT(SetIsoSurfaceValue2(double))); 	

	connect(spinbox_alphaFilter,SIGNAL(valueChanged(double)),this, SLOT(SetDataOptions()));
	connect(checkBox_alphaFilter,SIGNAL(toggled(bool)),this, SLOT(SetDataOptions()));
	connect(spinbox_ncrownX,SIGNAL(valueChanged(int)),this, SLOT(SetDataOptions())); 	
	connect(spinbox_ncrownY,SIGNAL(valueChanged(int)),this, SLOT(SetDataOptions())); 	
	connect(spinbox_ncrownZ,SIGNAL(valueChanged(int)),this, SLOT(SetDataOptions())); 	
	connect(spinbox_signalThreshold,SIGNAL(valueChanged(double)),this, SLOT(SetDataOptions())); 	
	

	//## Set object layout	
	QHBoxLayout* sliceOptionXYLayout = new QHBoxLayout;
	//sliceOptionXYLayout->addWidget(checkBox_sliceXY);	
	sliceOptionXYLayout->addWidget(label_sliceXY);
  sliceOptionXYLayout->addWidget(slider_sliceXY);
  sliceOptionXYLayout->addStretch(1);

	QHBoxLayout* sliceOptionXZLayout = new QHBoxLayout;
	//sliceOptionXZLayout->addWidget(checkBox_sliceXZ);
	sliceOptionXZLayout->addWidget(label_sliceXZ);
  sliceOptionXZLayout->addWidget(slider_sliceXZ);
  sliceOptionXZLayout->addStretch(1);

	QHBoxLayout* sliceOptionYZLayout = new QHBoxLayout;
	//sliceOptionYZLayout->addWidget(checkBox_sliceYZ);		
	sliceOptionYZLayout->addWidget(label_sliceYZ);
  sliceOptionYZLayout->addWidget(slider_sliceYZ);
  sliceOptionYZLayout->addStretch(1);

	QVBoxLayout* sliceOptionLayout = new QVBoxLayout;
	//sliceOptionLayout->addWidget(slider_slice);
  //sliceOptionLayout->addLayout(sliceButtonLayout);
	sliceOptionLayout->addLayout(sliceOptionXYLayout);
	sliceOptionLayout->addLayout(sliceOptionXZLayout);	
	sliceOptionLayout->addLayout(sliceOptionYZLayout);			
	sliceOptionLayout->addStretch(1);
	groupBox_sliceOptions->setLayout(sliceOptionLayout);


	QHBoxLayout* isosurfaceOption1Layout = new QHBoxLayout;
	//isosurfaceOption1Layout->addWidget(checkBox_showIsoSurface1);
	isosurfaceOption1Layout->addWidget(label_isosurfaceValue1);
  isosurfaceOption1Layout->addWidget(spinbox_isosurfaceValue1);
  isosurfaceOption1Layout->addStretch(1);

	QHBoxLayout* isosurfaceOption2Layout = new QHBoxLayout;
	//isosurfaceOption2Layout->addWidget(checkBox_showIsoSurface2);
	isosurfaceOption2Layout->addWidget(label_isosurfaceValue2);
  isosurfaceOption2Layout->addWidget(spinbox_isosurfaceValue2);
  isosurfaceOption2Layout->addStretch(1);

	QVBoxLayout* isosurfaceOptionLayout = new QVBoxLayout;
	isosurfaceOptionLayout->addLayout(isosurfaceOption1Layout);
	isosurfaceOptionLayout->addLayout(isosurfaceOption2Layout);			
	sliceOptionLayout->addStretch(1);
	groupBox_IsoSurfaceOptions->setLayout(isosurfaceOptionLayout);
	
	
	QHBoxLayout* NMinEventsLayout= new QHBoxLayout;
	NMinEventsLayout->addWidget(label_nMinEvents);
	NMinEventsLayout->addWidget(spinbox_nMinEvents);
	NMinEventsLayout->addStretch(1);

	QHBoxLayout* ScalarCutLayout= new QHBoxLayout;
	ScalarCutLayout->addWidget(label_scalarCut);
	ScalarCutLayout->addWidget(spinbox_scalarMinCut);
	ScalarCutLayout->addWidget(spinbox_scalarMaxCut);
	ScalarCutLayout->addStretch(1);

	QHBoxLayout* AlphaFilterLayout = new QHBoxLayout;
	AlphaFilterLayout->addWidget(checkBox_alphaFilter);
  AlphaFilterLayout->addWidget(spinbox_alphaFilter);
  AlphaFilterLayout->addStretch(1);

	QHBoxLayout* NCrownLayout = new QHBoxLayout;
	NCrownLayout->addWidget(label_ncrown);
  NCrownLayout->addWidget(spinbox_ncrownX);
	NCrownLayout->addWidget(spinbox_ncrownY);
  NCrownLayout->addWidget(spinbox_ncrownZ);
  NCrownLayout->addStretch(1);

	QHBoxLayout* SignalThresholdLayout = new QHBoxLayout;
	SignalThresholdLayout->addWidget(label_signalThreshold);
  SignalThresholdLayout->addWidget(spinbox_signalThreshold);
  SignalThresholdLayout->addStretch(1);

	QVBoxLayout* DataOptionLayout = new QVBoxLayout;
	DataOptionLayout->addLayout(NMinEventsLayout);
	DataOptionLayout->addLayout(ScalarCutLayout);
	DataOptionLayout->addWidget(checkBox_useLogScalars);
	DataOptionLayout->addLayout(AlphaFilterLayout);
	DataOptionLayout->addLayout(NCrownLayout);
	DataOptionLayout->addLayout(SignalThresholdLayout);
	DataOptionLayout->addStretch(1);
  groupBox_dataOptions->setLayout(DataOptionLayout);
	
	
  
	QHBoxLayout* paletteOptionLayout1= new QHBoxLayout;
	paletteOptionLayout1->addWidget(label_paletteFcnMean);
	//paletteOptionLayout1->addWidget(slider_paletteFcnMean);
	paletteOptionLayout1->addWidget(spinbox_paletteFcnMean);
	paletteOptionLayout1->addStretch(1);

	QHBoxLayout* paletteOptionLayout2= new QHBoxLayout;
	paletteOptionLayout2->addWidget(label_paletteFcnSlope);
	//paletteOptionLayout2->addWidget(slider_paletteFcnSlope);
	paletteOptionLayout2->addWidget(spinbox_paletteFcnSlope);
	paletteOptionLayout2->addStretch(1);

	QHBoxLayout* paletteOptionLayout3= new QHBoxLayout;
	paletteOptionLayout3->addWidget(label_paletteFcnBaseline);
	//paletteOptionLayout3->addWidget(slider_paletteFcnBaseline);
	paletteOptionLayout3->addWidget(spinbox_paletteFcnBaseline);
	paletteOptionLayout3->addStretch(1);

	QVBoxLayout* paletteOptionLayout= new QVBoxLayout;
	//paletteOptionLayout->addLayout(paletteFileLayout);
	//paletteOptionLayout->addWidget(checkBox_usePaletteTransferFunction);	
	paletteOptionLayout->addLayout(paletteOptionLayout1);
	paletteOptionLayout->addLayout(paletteOptionLayout2);
	paletteOptionLayout->addLayout(paletteOptionLayout3);
	paletteOptionLayout->addStretch(1);
	groupBox_paletteOptions->setLayout(paletteOptionLayout);

	
	QVBoxLayout* optionFrameLayout = new QVBoxLayout;
  //optionFrameLayout->addWidget(groupBox_options);
	optionFrameLayout->addWidget(groupBox_sliceOptions);
	optionFrameLayout->addWidget(groupBox_IsoSurfaceOptions);
	optionFrameLayout->addWidget(groupBox_dataOptions);
	optionFrameLayout->addWidget(groupBox_paletteOptions);
	//optionFrameLayout->addWidget(groupBox_cameraOptions);
	optionFrameLayout->addStretch(1);

	QVBoxLayout* vtkWidgetLayout= new QVBoxLayout;
	vtkWidgetLayout->addWidget(vtkWidget);
	vtkWidgetLayout->addWidget(infoWidget);
	vtkWidgetLayout->addStretch(1);

	//QVBoxLayout* infoWidgetLayout= new QVBoxLayout;
	//infoWidgetLayout->addWidget(infoWidget);
	//infoWidgetLayout->addStretch(1);

	QHBoxLayout* windowLayout = new QHBoxLayout;
  windowLayout->addLayout(optionFrameLayout);
	windowLayout->addLayout(vtkWidgetLayout);
	//windowLayout->addWidget(vtkWidget);
	//windowLayout->addWidget(vtkWidget2);
	windowLayout->addStretch(1);

  QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(windowLayout);
	//mainLayout->addLayout(infoWidgetLayout);
	mainLayout->addStretch(1);
  //mainLayout->addLayout(buttonsLayout);	
  widget->setLayout(mainLayout);
	
	window->setCentralWidget(widget);
  window->setWindowTitle(tr("Muon Tomography Viewer"));
	//window->show();

}//close VolumeRenderingViewer::BuildGUIObjects()



void VolumeRenderingViewer::Init(){

	//## Init parameters
	fOutputImageFileName= "VolRendFig.png";
	
	fUseOpacityTransferFunction= false;
	fOpacityTransferFunctionMean= 50;
	fOpacityTransferFunctionSlope= 10;
	fOpacityTransferFunctionBaseline= 0.1;

	fIsFileNameEmpty= false;
	fIsCorruptedFile= false;
	fIsVolumeDataAvailable= false;
	fIsPointDataAvailable= false;
	fIsContainerDataAvailable= false;
	fIsCameraChanged= true;
	fCameraAzimuth= 20;
	fCameraElevation= 20;
	fCameraDefaultAzimuth= 20;
	fCameraDefaultElevation= 20;
	fDrawVolume= true;
	fDrawPoints= false;
	fDrawBoundingBox= true;
	fDrawPaletteBar= true;
	fDrawContainer= false;
	fDrawVolumeSlice= false;
	fDrawVolumeSliceXY= true;
	fDrawVolumeSliceXZ= false;
	fDrawVolumeSliceYZ= false;
	fDrawIsoSurface= false;
	fDrawIsoSurface1= true;
	fDrawIsoSurface2= false;
	fIsoSurfaceScalar1= 80;
	fIsoSurfaceScalar2= 40;
	fIsoSurfaceScalarWidth= 10;
	fUsePaletteFile= false;	
	fNColorsInPalette= 0;
	fPaletteFileName= "scripts/paletteVisIVO2.dat";
	fNMinEvents= 0;
	fLgScalarMinCut= -99;//-8;
	fLgScalarMaxCut= 99;//-2
	fUseLogScalars= false;	
	fContainerFileName= "ContainerImage.vtk";
	fUseAlphaFilter= false;
	fFilterAlpha= 0.04;	

	fSignalThreshold= -3.2;
	fThreatPosList.clear();
	fThreatPosList.resize(0);
	fThreatSignalList.clear();
	fThreatSignalList.resize(0);

	fUpdateDraw= false;
	fUpdateDataRead= false;
	fUpdatePrint= false;

	//## Check input file 
	if(fInputFileName==""){
		QString errmsg = QString("VolumeRenderingViewer::Init(): WARNING: No input file given... ");
		emit(statusMessage(errmsg));
		emit error();
		fIsFileNameEmpty= true;
	}
	else{
		fIsFileNameEmpty= false;

		inputFile= new TFile(fInputFileName.c_str(),"READ");
		if(!inputFile || inputFile->IsZombie()){
			QString errmsg = QString("VolumeRenderingViewer::Init(): ERROR: Cannot open input file ");
			errmsg+= QString::fromStdString(fInputFileName);
			errmsg+= QString::fromStdString(std::string(" ... exit!"));
			emit(statusMessage(errmsg));
			emit error();
			fIsFailure= true;
			fIsCorruptedFile= true;
			return;
  	}

		//## Check info tree
		info= (TTree*)inputFile->Get("InfoRun");
		if(!info) info= (TTree*)inputFile->Get("Info");	
	
		if(!info){
			QString errmsg = QString("VolumeRenderingViewer::Init(): ERROR: Cannot get info tree...exit!");
			emit(statusMessage(errmsg));
			emit error();
			fIsFailure= true;
			fIsCorruptedFile= true;
			return;
 	 }

		//## Check data tree
		data= (TTree*)inputFile->Get("data");
		if(!data) {
			cerr<<"VolumeRenderingViewer::Init(): WARNING: Cannot get points data...point rendering won't be performed!"<<endl;
			fIsPointDataAvailable= false;	
		}
		else fIsPointDataAvailable= true;	

		//## Check data histos
		fRecMap3D= (TH3D*)inputFile->Get("RecMap3D");
		fNEventMap3D= (TH3D*)inputFile->Get("NEventMap3D");
		if(!fRecMap3D || !fNEventMap3D){
			cerr<<"VolumeRenderingViewer::Init(): WARNING: Cannot get volume data...volume rendering won't be performed!"<<endl;
			fIsVolumeDataAvailable= false;		
		}
		else {
			fIsVolumeDataAvailable= true;	

			fFilterRecMap3D= (TH3D*)fRecMap3D->Clone("fFilterRecMap3D");
			fFilterRecMap3D->Reset();

			fDrawRecMap3D= (TH3D*)fRecMap3D->Clone("fDrawRecMap3D");
		}

	}//close else file check


	//## Check container file
	fContainerFileName_fullpath= std::string(qPrintable(QDir::currentPath())) + std::string("/share/") + fContainerFileName;
	
	std::ifstream containerFileStream;
	containerFileStream.open(fContainerFileName_fullpath.c_str());
	if(!containerFileStream.good()) {
		cerr<<"VolumeRenderingViewer::Init(): WARNING: Cannot get container data...container won't be visualized!"<<endl;
		fIsContainerDataAvailable= false;	
  }	
	else fIsContainerDataAvailable= true;	

	//## Build VTK objects	
	//BuildVTKObjects();

	//## Build objects in GUI
	//BuildGUIObjects();

}//close VolumeRenderingViewer::Init()



void VolumeRenderingViewer::process(){

	Init();
	ReadData();	
	Draw();
	DumpInfo();

	window->show();

}//close VolumeRenderingViewer::process()


void VolumeRenderingViewer::ReadInfoData(){

	if(info){
		cout<<"--> Reading info data ...";
		info->SetBranchAddress("NVoxelX",&nVoxelX);
		info->SetBranchAddress("NVoxelY",&nVoxelY);
		info->SetBranchAddress("NVoxelZ",&nVoxelZ);
		info->SetBranchAddress("XMin",&XMin);
		info->SetBranchAddress("YMin",&YMin);
		info->SetBranchAddress("ZMin",&ZMin);
		info->SetBranchAddress("XMax",&XMax);
		info->SetBranchAddress("YMax",&YMax);
		info->SetBranchAddress("ZMax",&ZMax);
		info->GetEntry(0);
		cout<<"done!"<<endl;
	}

}//close VolumeRenderingViewer::ReadInfoData()


void VolumeRenderingViewer::ReadPointsData(){

	cout<<"--> Reading points data ...";
	data->SetBranchAddress("POCAX",&xx);
	data->SetBranchAddress("POCAY",&yy);
	data->SetBranchAddress("POCAZ",&zz);
	data->SetBranchAddress("ScatteringAngle",&theta);

	//double wmin= pow(data->GetMinimum("ScatteringAngle"),2);
	//double wmax= pow(data->GetMaximum("ScatteringAngle"),2);
	
	double wmin= data->GetMinimum("ScatteringAngle");
	double wmax= data->GetMaximum("ScatteringAngle");

	int nentries= data->GetEntries();

	vtkFloatArray* radiusArrays= vtkFloatArray::New();
  vtkFloatArray* xAxis= vtkFloatArray::New();
  vtkFloatArray* yAxis= vtkFloatArray::New();
  vtkFloatArray* zAxis= vtkFloatArray::New();
  
  xAxis->SetNumberOfTuples(nentries);
  yAxis->SetNumberOfTuples(nentries);
  zAxis->SetNumberOfTuples(nentries);
  radiusArrays->SetNumberOfTuples(nentries);

  xAxis->SetName("X");
  yAxis->SetName("Y");
  zAxis->SetName("Z");
	radiusArrays->SetName("S");

	vtkPoints* points= vtkPoints::New();
  points->SetNumberOfPoints(nentries);
	
	for(int i=0;i<nentries;i++){
		data->GetEntry(i);
		
		if(xx<XMin || xx>XMax || yy<YMin || yy>YMax || zz<ZMin || zz>ZMax) {
			//cout<<"--> Skip point outside container ("<<xx<<","<<yy<<","<<zz<<")"<<endl;
			continue;
		}
		
		//double w= pow(theta,2);	
		double w= theta;
		double lgw= log10(w);

		double w_norm= (w-wmin)/(wmax-wmin)*255;
		double lgw_norm= (lgw-log10(wmin))/(log10(wmax)-log10(wmin))*255;
						
	
		//double x_norm= (xx-XMin)/(XMax-XMin) * (nVoxelX-1) + 1;
		//double y_norm= (yy-YMin)/(YMax-YMin) * (nVoxelY-1) + 1;
		//double z_norm= (zz-ZMin)/(ZMax-ZMin) * (nVoxelZ-1) + 1;
		double x_norm= (xx-XMin)/(XMax-XMin) * (nVoxelX) ;
		double y_norm= (yy-YMin)/(YMax-YMin) * (nVoxelY) ;
		double z_norm= (zz-ZMin)/(ZMax-ZMin) * (nVoxelZ) ;

		x_norm-= nVoxelX/2.;
		y_norm-= nVoxelY/2.;
		z_norm-= nVoxelZ/2.;
			
		y_norm*= -1;

    xAxis->SetValue(i,x_norm);
    yAxis->SetValue(i,y_norm);
    zAxis->SetValue(i,z_norm);
		//radiusArrays->SetValue(i,w);
		//radiusArrays->SetValue(i,w_norm);
		radiusArrays->SetValue(i,lgw_norm);

		float pointValues[3];
		pointValues[0]= x_norm;
		pointValues[1]= y_norm;
		pointValues[2]= z_norm;
			
		points->SetPoint(i,pointValues);
			
		//cout<<"w_norm="<<w_norm<<endl;
  }//end loop events

	double m_xRange[2] ,m_yRange[2] , m_zRange[2];
	xAxis->GetRange(m_xRange);  
  yAxis->GetRange(m_yRange);  
  zAxis->GetRange(m_zRange);  
 
	double scalingFactors[3];
  scalingFactors[0]= scalingFactors[1]= scalingFactors[2]= 0;
    
	//## Set point data
	pointData->SetPoints(points);
	pointData->GetPointData()->SetScalars(radiusArrays);
  points->Delete();

	int nPoints = pointData->GetNumberOfPoints();

	//## Set vert
	newVerts->EstimateSize(nPoints,1);

  for(int i=0; i<nPoints; i++){
    newVerts->InsertNextCell(1);
    newVerts->InsertCellPoint(i);
  }
  pointData->SetVerts(newVerts);
  
  xAxis->Delete();
  yAxis->Delete();
  zAxis->Delete();

	//## Define mapper and actor
	pointDataMapper->SetInput(pointData);
	pointDataActor->SetMapper(pointDataMapper);

	double b[2];
	pointData->GetPointData()->SetActiveScalars("S");
  pointData->GetPointData()->GetScalars("S")->GetRange(b);
	fPointDataMin= b[0];
	fPointDataMax= b[1];
	
	cout<<"done!"<<endl;

}//close VolumeRenderingViewer::ReadPointsData()


void VolumeRenderingViewer::ReadVolumeData(){

	cout<<"--> Reading volume data ...";
	
	//## Check if use filtered map
	fDrawRecMap3D->Reset();
	fThreatPosList.clear();
	fThreatPosList.resize(0);
	fThreatSignalList.clear();
	fThreatSignalList.resize(0);

	if(fUseAlphaFilter){
		Filter();	
		fDrawRecMap3D->Reset();
		for(int k=0;k<fRecMap3D->GetNbinsZ();k++){
			for(int j=0;j<fRecMap3D->GetNbinsY();j++){
				for(int i=0;i<fRecMap3D->GetNbinsX();i++){		
					fDrawRecMap3D->SetBinContent(i+1,j+1,k+1,fFilterRecMap3D->GetBinContent(i+1,j+1,k+1));
				}
			}
		}
		//fFilterRecMap3D->Copy(*fDrawRecMap3D);
	}	
	else{	
		for(int k=0;k<fRecMap3D->GetNbinsZ();k++){
			for(int j=0;j<fRecMap3D->GetNbinsY();j++){
				for(int i=0;i<fRecMap3D->GetNbinsX();i++){		
					fDrawRecMap3D->SetBinContent(i+1,j+1,k+1,fRecMap3D->GetBinContent(i+1,j+1,k+1));
				}
			}
		}
		//fRecMap3D->Copy(*fDrawRecMap3D);
	}	

	float x, y, z, scalar;
	double m_range[2]; 
  m_range[0]= 0;
  m_range[1] = 0;

	int fNEvents= fRecMap3D->GetNbinsZ()*fRecMap3D->GetNbinsY()*fRecMap3D->GetNbinsX(); 
	int fNEventsXY= fRecMap3D->GetNbinsY()*fRecMap3D->GetNbinsX();
	int fNEventsXZ= fRecMap3D->GetNbinsX()*fRecMap3D->GetNbinsZ();
	int fNEventsYZ= fRecMap3D->GetNbinsY()*fRecMap3D->GetNbinsZ();
	

	vtkFloatArray* volumeField = vtkFloatArray::New();
  volumeField->SetNumberOfTuples(fNEvents);
	volumeField->SetName("S");

	vtkFloatArray* volumeField_scaled = vtkFloatArray::New();
  volumeField_scaled->SetNumberOfTuples(fNEvents);
	volumeField_scaled->SetName("S");

	


	//## Find min & max in data
	double dataMin= 1.e+99;
	double dataMax= -1.e+99;
	for(int k=0;k<fRecMap3D->GetNbinsZ();k++){
		for(int j=0;j<fRecMap3D->GetNbinsY();j++){
			for(int i=0;i<fRecMap3D->GetNbinsX();i++){		
				double x= fRecMap3D->GetXaxis()->GetBinCenter(i+1);
				double y= fRecMap3D->GetYaxis()->GetBinCenter(j+1);
				double z= fRecMap3D->GetZaxis()->GetBinCenter(k+1);
				//double binContent= fRecMap3D->GetBinContent(i+1,j+1,k+1);
				double binContent= fDrawRecMap3D->GetBinContent(i+1,j+1,k+1);
				
				double neventsInBin= fNEventMap3D->GetBinContent(i+1,j+1,k+1);
				if(neventsInBin>fNMinEvents && binContent>pow(10,fLgScalarMinCut) && binContent<pow(10,fLgScalarMaxCut) ){
				//if(neventsInBin>fNMinEvents){	
						if(binContent>dataMax) dataMax= binContent;
						if(binContent<dataMin) dataMin= binContent;
				}
			}//end loop x bins
		}//end loop y bins
	}//end loop z bins
	double logdataMin= log10(dataMin);
	double logdataMax= log10(dataMax);
	cout<<"min/max= "<<dataMin<<"/"<<dataMax<<endl;
	cout<<"lg min/max= "<<logdataMin<<"/"<<logdataMax<<endl;


	int npoints= 0;
	
	for(int k=0;k<fRecMap3D->GetNbinsZ();k++){
		for(int j=0;j<fRecMap3D->GetNbinsY();j++){
			for(int i=0;i<fRecMap3D->GetNbinsX();i++){	
	
				double x= fRecMap3D->GetXaxis()->GetBinCenter(i+1);
				double y= fRecMap3D->GetYaxis()->GetBinCenter(j+1);
				double z= fRecMap3D->GetZaxis()->GetBinCenter(k+1);
				//double binContent= fRecMap3D->GetBinContent(i+1,j+1,k+1);
				double binContent= fDrawRecMap3D->GetBinContent(i+1,j+1,k+1);
				
				double binContentScaled= 255.*(binContent-dataMin)/(dataMax-dataMin);
				double lgBinContent= log10(binContent);				
				double lgBinContentScaled= 255.*(lgBinContent-logdataMin)/(logdataMax-logdataMin);
		
				double neventsInBin= fNEventMap3D->GetBinContent(i+1,j+1,k+1);

				//cout<<"(x,y,z)= ("<<x<<","<<y<<","<<z<<") Lambda="<<binContent<<"  LambdaScaled="<<binContentScaled<<endl;
				//if(fabs(z-250)<20 && fabs(y)<20 && fabs(x)<150){
				
				//if(lgBinContent>-3){
					//cout<<"(x,y,z)= ("<<x<<","<<y<<","<<z-250<<") ("<<i+1<<","<<j+1<<","<<k+1<<")  Lambda="<<binContent<<"  LogLambda="<<lgBinContent<<"  LambdaScaled="<<lgBinContentScaled<<"  neventsInBin="<<neventsInBin<<endl;
				//}
							
				if(neventsInBin>fNMinEvents && binContent>pow(10,fLgScalarMinCut) && binContent<pow(10,fLgScalarMaxCut)) {
				//if(neventsInBin>fNMinEvents) {

					if(fUseLogScalars){	
						volumeField->SetValue(npoints,lgBinContent);
						volumeField_scaled->SetValue(npoints,lgBinContentScaled);
					}
					else{
						volumeField->SetValue(npoints,binContent);
						volumeField_scaled->SetValue(npoints,binContentScaled);
					}

					if(lgBinContent>fSignalThreshold){
						fThreatPosList.push_back(TVector3(x,y,z));
						fThreatSignalList.push_back(lgBinContent);
					}

				}//close if
				else {
					if(fUseLogScalars){	
						volumeField->SetValue(npoints,logdataMin);
						volumeField_scaled->SetValue(npoints,0.);
					}
					else{
						volumeField->SetValue(npoints,dataMin);
						volumeField_scaled->SetValue(npoints,0.);
					}

				}//close else			
				
				npoints++;
				

			}//end loop y bins
		}//end loop x bins
	}//end loop z bins

	
	image->SetScalarTypeToFloat();
	image->SetOrigin(-nVoxelX/2.,-nVoxelY/2.,-nVoxelZ/2.);
  image->SetExtent(1,nVoxelX,1,nVoxelY,1,nVoxelZ);
  image->SetSpacing(1,1,1);
  image->GetPointData()->SetScalars(volumeField);

	imageScaled->SetScalarTypeToFloat();
	imageScaled->SetOrigin(-nVoxelX/2.,-nVoxelY/2.,-nVoxelZ/2.);
  imageScaled->SetExtent(1,nVoxelX,1,nVoxelY,1,nVoxelZ);
  imageScaled->SetSpacing(1,1,1);
  imageScaled->GetPointData()->SetScalars(volumeField_scaled);
 
  volumeField->GetRange(m_range); 
  volumeField->Delete();
  
  double max = m_range[1];
  double min = m_range[0];
		
	vtkImageMathematics* m_math= vtkImageMathematics::New();
  m_math->SetInput(image);
  m_math->SetOperationToAddConstant();
  m_math->SetConstantC(-min);
  float defaultRange= 255.0;
 	double norm = 255.0/(max - min);

	vtkImageMathematics *m_math2= vtkImageMathematics::New();
  m_math2->SetOperationToMultiplyByK();
  m_math2->SetConstantK(norm);
  m_math2->SetInputConnection(m_math->GetOutputPort());
	
	vtkImageCast* m_charData = vtkImageCast::New();
  m_charData->SetOutputScalarTypeToUnsignedChar();
  m_charData->SetInputConnection(m_math2->GetOutputPort());
  m_charData->Update();
  m_charData->GetOutput()->GetScalarRange(m_range);
	fVolumeDataMin= m_range[0];
	fVolumeDataMax= m_range[1];
	cout<<"fVolumeDataMin="<<fVolumeDataMin<<"  fVolumeDataMax="<<fVolumeDataMax<<endl;
		
	vtkVolumeRayCastCompositeFunction* compositeFunction = vtkVolumeRayCastCompositeFunction::New();
  	
	//## Define volume mapper
	volumeMapper->SetVolumeRayCastFunction(compositeFunction);
  volumeMapper->SetInputConnection(m_charData->GetOutputPort());

	
  //#############################
	//## Define image slices
	//#############################

	int extent1, extent2, extent3, extent4, extent5, extent6;
	image->GetExtent(extent1, extent2, extent3, extent4, extent5, extent6);

	double center[3];
	double origin[3]= {-nVoxelX/2.,-nVoxelY/2.,-nVoxelZ/2.};
	int spacing[3]= {1,1,1};
	int extent[6]= {1,nVoxelX,1,nVoxelY,1,nVoxelZ};
  center[0] = origin[0] + spacing[0] * 0.5 * (extent[0] + extent[1]); 
  center[1] = origin[1] + spacing[1] * 0.5 * (extent[2] + extent[3]); 
  center[2] = origin[2] + spacing[2] * 0.5 * (extent[4] + extent[5]);
	
	//###  METHOD 1: vtkCutter (see VISIVO) ###
	double maxVoxelSize= std::max(std::max(nVoxelX,nVoxelY),nVoxelZ);
	fMinSlicePosition= -maxVoxelSize/2;
	fMaxSlicePosition= maxVoxelSize/2;
	//slider_slice->setRange(fMinSlicePosition,fMaxSlicePosition);
	slider_sliceXY->setRange(-nVoxelZ/2,nVoxelZ/2);
	slider_sliceXZ->setRange(-nVoxelY/2,nVoxelY/2);
	slider_sliceYZ->setRange(-nVoxelX/2,nVoxelX/2);

	imageSlicePoints->SetDimensions(nVoxelX,nVoxelY,nVoxelZ);
  imageSlicePoints->GetPointData()->SetScalars(volumeField);
		
	slicingPlane->SetNormal(0,0,1);//z plane
	//slicingPlane->SetOrigin(-nVoxelX/2,-nVoxelY/2,nVoxelZ/2); 
	slicingPlane->SetOrigin(0,0,0); 

	slicingPlaneXY->SetNormal(0,0,1);//z plane
	slicingPlaneXY->SetOrigin(0,0,0); 

	slicingPlaneXZ->SetNormal(0,1,0);//y plane
	slicingPlaneXZ->SetOrigin(0,0,0);

	slicingPlaneYZ->SetNormal(1,0,0);//x plane
	slicingPlaneYZ->SetOrigin(0,0,0);  

  planeCutter->SetCutFunction(slicingPlane);
  //planeCutter->SetInput(imageSlicePoints);
	//planeCutter->SetInput(image);
	planeCutter->SetInput(imageScaled);

	planeCutter->SetGenerateCutScalars(0);
	//planeCutter->SetNumberOfContours(100);
	planeCutter->Update();

	
	planeCutterXY->SetCutFunction(slicingPlaneXY);
  planeCutterXY->SetInput(imageScaled);
	planeCutterXY->SetGenerateCutScalars(0);
	planeCutterXY->Update();

	planeCutterXZ->SetCutFunction(slicingPlaneXZ);
  planeCutterXZ->SetInput(imageScaled);
	planeCutterXZ->SetGenerateCutScalars(0);
	planeCutterXZ->Update();

	planeCutterYZ->SetCutFunction(slicingPlaneYZ);
  planeCutterYZ->SetInput(imageScaled);
	planeCutterYZ->SetGenerateCutScalars(0);
	planeCutterYZ->Update();


	//planeCutterMapper->SetInput((vtkPolyData*)planeCutter->GetOutput());
	planeCutterMapper->SetInputConnection(planeCutter->GetOutputPort());

	
  planeCutterMapper->SetScalarRange(0,255);
	//planeCutterMapper->SetColorMode(1);
  //planeCutterMapper->SetResolveCoincidentTopology(0);
  //planeCutterMapper->SetScalarMode(0);
  //planeCutterMapper->SetImmediateModeRendering(0);
  //planeCutterMapper->SetScalarVisibility(1);
  //planeCutterMapper->SetUseLookupTableScalarRange(0);


	planeCutterMapperXY->SetInputConnection(planeCutterXY->GetOutputPort());
  planeCutterMapperXY->SetScalarRange(0,255);

	planeCutterMapperXZ->SetInputConnection(planeCutterXZ->GetOutputPort());
  planeCutterMapperXZ->SetScalarRange(0,255);

	planeCutterMapperYZ->SetInputConnection(planeCutterYZ->GetOutputPort());
  planeCutterMapperYZ->SetScalarRange(0,255);

	/*
	//###  METHOD 2: vtkImageReslice ###
	double axialElements[16] = {
           1, 0, 0, 0,
           0, 1, 0, 0,
           0, 0, 1, 0,
           0, 0, 0, 1 };
	vtkMatrix4x4* resliceAxes = vtkMatrix4x4::New();
  resliceAxes->DeepCopy(axialElements);
  resliceAxes->SetElement(0, 3, center[0]);
  resliceAxes->SetElement(1, 3, center[1]);
  resliceAxes->SetElement(2, 3, center[2]);

	imageSlice = vtkImageReslice::New();
	//imageSlice->SetInputConnection(sliceImageReader->GetOutputPort());
  imageSlice->SetOutputExtent(extent[0],extent[1],extent[2],extent[3],extent[4],extent[5]);
  //imageSlice->SetInputData(image);
	imageSlice->SetInput(image);
	//imageSlice->SetInputConnection(image->GetProducerPort());
	imageSlice->SetOutputDimensionality(2);
  imageSlice->SetResliceAxes(resliceAxes);
  imageSlice->SetInterpolationModeToLinear();
  imageSlice->Update();


	imageSliceMapper->SetInputConnection(image->GetProducerPort());
  //imageSliceMapper->SetInputData(colorImage)
	//imageSliceMapper->SliceFacesCameraOn();
	//imageSliceMapper->SliceAtFocalPointOn();
	//imageSliceMapper->BorderOff();
	*/
	
	//###################################
	//##  Define ISOSURFACE
	//###################################
	contourFilter1->SetInput(imageScaled);
	contourFilter1->SetNumberOfContours(1);

	double minContour= std::max(fIsoSurfaceScalar1-fIsoSurfaceScalarWidth/2.,0.);
	double maxContour= std::min(fIsoSurfaceScalar1+fIsoSurfaceScalarWidth/2.,255.);
	contourFilter1->GenerateValues(1,minContour,maxContour);
  //contourFilter1->SetValue(0,100);  
  contourFilter1->SetComputeNormals(1);
  contourFilter1->SetComputeScalars(1);

	contourFilter2->SetInput(imageScaled);
	minContour= std::max(fIsoSurfaceScalar2-fIsoSurfaceScalarWidth/2.,0.);
	maxContour= std::min(fIsoSurfaceScalar2+fIsoSurfaceScalarWidth/2.,255.);
	contourFilter2->GenerateValues(1,minContour,maxContour);
  //contourFilter2->SetValue(0,60);  
  contourFilter2->SetComputeNormals(1);
  contourFilter2->SetComputeScalars(1);

	
	
	//#######################################
	//## Define bounding box actor
	//#######################################
	vtkOutlineFilter* corner= vtkOutlineFilter::New();
	corner->SetInput(image);
	corner->ReleaseDataFlagOn();

	vtkPolyDataMapper* boundingBoxMapper = vtkPolyDataMapper::New();
	boundingBoxMapper->SetInput(corner->GetOutput());

	vtkProperty* boundingBoxProperty = vtkProperty::New();
	boundingBoxProperty->SetColor(1,1,1);
	boundingBoxProperty->SetAmbient(1);
	boundingBoxProperty->SetOpacity(0.5);
	boundingBoxProperty->SetRepresentationToWireframe();
	boundingBoxProperty->SetInterpolationToFlat();

	boundingBoxActor->SetMapper(boundingBoxMapper);
	boundingBoxActor->SetProperty(boundingBoxProperty);
	boundingBoxActor->SetPickable(false);
	boundingBoxActor->SetVisibility(true);	

	cout<<"done!"<<endl;

}//close VolumeRenderingViewer::ReadVolumeData()


void VolumeRenderingViewer::ReadContainerData(){

	cout<<"--> Reading container data ...";
	//## Container actor	
	
	vtkPolyDataReader* containerReader = vtkPolyDataReader::New();
	containerReader->SetFileName(fContainerFileName_fullpath.c_str());
	containerReader->Update();
 
  vtkPolyDataMapper* containerMapper = vtkPolyDataMapper::New();
	containerMapper->SetInputConnection(containerReader->GetOutputPort());
 
	containerActor->GetProperty()->SetOpacity(0.2); 
  containerActor->SetMapper(containerMapper);

	cout<<"done!"<<endl;	

}//close VolumeRenderingViewer::ReadContainerData()


void VolumeRenderingViewer::ReadData(){

	//## Read info data
	if(!fIsFileNameEmpty && !fIsCorruptedFile) ReadInfoData();

	//## Read points data
	if(!fIsFileNameEmpty && !fIsCorruptedFile && fIsPointDataAvailable) ReadPointsData();
	
	//## Read volume data
	if(!fIsFileNameEmpty && !fIsCorruptedFile && fIsVolumeDataAvailable) ReadVolumeData();
	
	//## Read container data
	if(!fIsFileNameEmpty && !fIsCorruptedFile && fIsContainerDataAvailable) ReadContainerData();
	
}//close ReadData()



void VolumeRenderingViewer::Draw(int){

	cout<<"--> Drawing data ..."<<endl;

	//## Set palette and opacity
	if(!fIsFileNameEmpty && !fIsCorruptedFile) SetPalette();
	
	//## Set point data properties
	if(fIsPointDataAvailable){
		pointDataMapper->SetLookupTable(lut); 
  	pointDataMapper->SetScalarVisibility(1);
  	pointDataMapper->UseLookupTableScalarRangeOn();	
		pointDataMapper->SetScalarRange(fPointDataMin,fPointDataMax);
		
  	pointDataActor->SetMapper(pointDataMapper);	
		
	}//close if point data available

	
	//## Define volume properties and actors
	if(fIsVolumeDataAvailable){
		
		//## Define volume rendering properties
		volumeProperty->SetColor(colorTransferFunction);
  	volumeProperty->SetScalarOpacity(opacityTransferFunction);
  	//volumeProperty->ShadeOn();
  	volumeProperty->SetInterpolationTypeToLinear();

  	volume->SetMapper(volumeMapper);
  	volume->SetProperty(volumeProperty);

		//## Define color palette bar actor
		paletteBarActor->SetTitle("S");
		paletteBarActor->SetLabelFormat("%.3g");
		paletteBarActor->SetOrientationToHorizontal();
		paletteBarActor->SetPosition(0.1,0);
		paletteBarActor->SetPosition2(0.8,0.1);
		paletteBarActor->SetLookupTable(lut);
		paletteBarActor->SetVisibility(1);

		//## Set properties for volume slice
		//## METHOD 1: vtkCutter
		planeCutterMapper->SetLookupTable(lut_slice);
		slicingPlaneActor->SetMapper(planeCutterMapper);

		planeCutterMapperXY->SetLookupTable(lut_slice);
		slicingPlaneActorXY->SetMapper(planeCutterMapperXY);

		planeCutterMapperXZ->SetLookupTable(lut_slice);
		slicingPlaneActorXZ->SetMapper(planeCutterMapperXZ);

		planeCutterMapperYZ->SetLookupTable(lut_slice);
		slicingPlaneActorYZ->SetMapper(planeCutterMapperYZ);

		/*
		//## METHOD 2: 
		imageSliceColor->SetLookupTable(lut);
		imageSliceColor->SetOutputFormatToRGBA();
  	imageSliceColor->SetInputConnection(imageSlice->GetOutputPort());

  	imageSliceActor->SetInput(imageSliceColor->GetOutput());
		//imageSliceActor->GetMapper()->SetInputConnection(imageSliceColor->GetOutputPort());
    
		//## METHOD 3: vtkImageViewer2
		imageSliceViewer->GetWindowLevel()->SetOutputFormatToRGBA();	
		imageSliceViewer->GetWindowLevel()->SetLookupTable(lut);
		imageSliceViewer->SetInputConnection(image->GetProducerPort());
		//imageSliceViewer->SetInputConnection(sliceImageReader->GetOutputPort());
		imageSliceViewer->SetSliceOrientationToXY();		
		imageSliceViewer->SetSlice(13);
		//imageSliceViewer->SetColorWindow(256.);
		//imageSliceViewer->SetColorLevel(127.5);
		

		//## METHOD 4: vtkImageSlice
		// Setting of vtkImageProperty
		//imageSliceProperty->SetColorWindow(2000);
		//imageSliceProperty->SetColorLevel(1000);
		//imageSliceProperty->SetAmbient(0.0);
		//imageSliceProperty->SetDiffuse(1.0);
		imageSliceProperty->SetOpacity(1.0);
		imageSliceProperty->SetLookupTable(lut);
  	imageSliceProperty->SetInterpolationTypeToLinear();
		//imageSliceProperty->SetInterpolationTypeToCubic();

		imageSlice2->SetMapper(imageSliceMapper);
		imageSlice2->SetProperty(imageSliceProperty);
		*/


		contourMapper1->SetInput(contourFilter1->GetOutput());
  	contourMapper1->SetScalarRange(0,255);
		contourMapper1->SetLookupTable(lut_slice);
		//contourMapper1->SetNumberOfPieces(1);
  	//contourMapper1->SetColorMode(0);
 		//contourMapper1->SetResolveCoincidentTopology(0);
  	//contourMapper1->SetScalarMode(0);
 		//contourMapper1->SetImmediateModeRendering(1);
  	//contourMapper1->SetScalarVisibility(0);
  	//contourMapper1->SetUseLookupTableScalarRange(0);	
		//contourMapper1->SetSetScalarModeToUseCellData();

		contourMapper2->SetInput(contourFilter2->GetOutput());
  	contourMapper2->SetScalarRange(0,255);  	
		contourMapper2->SetLookupTable(lut_slice);
		//contourMapper2->SetNumberOfPieces(1);
  	//contourMapper2->SetColorMode(0);
 		//contourMapper2->SetResolveCoincidentTopology(0);
  	//contourMapper2->SetScalarMode(0);
 		//contourMapper2->SetImmediateModeRendering(1);
  	//contourMapper2->SetScalarVisibility(0);
  	//contourMapper2->SetUseLookupTableScalarRange(0);
		//contourMapper2->SetSetScalarModeToUseCellData();

		contourProperty1->SetColor(1,0,0);//red
		contourProperty1->SetRepresentation(2);//0: points, 1:wireframe, 2:solid

		contourProperty2->SetColor(0,1,0);//green
		contourProperty2->SetRepresentation(2);//0: points, 1:wireframe, 2:solid

		contourActor1->SetMapper(contourMapper1);
   	//contourActor1->SetProperty(contourProperty1);
   	contourActor1->SetOrigin(0 , 0 , 0);
   	contourActor1->SetPosition(0 , 0 , 0);
   	contourActor1->SetScale(1 , 1 , 1);
   	contourActor1->SetVisibility(1);

		contourActor2->SetMapper(contourMapper2);
   	//contourActor2->SetProperty(contourProperty2);
   	contourActor2->SetOrigin(0 , 0 , 0);
   	contourActor2->SetPosition(0 , 0 , 0);
   	contourActor2->SetScale(1 , 1 , 1);
   	contourActor2->SetVisibility(1);
	}//close if volume data available



	//## Define renderer
	vtkRenderer* renderer= vtkRenderer::New();
	
	//## Add actors
  renderer->SetBackground(0,0,0);//black bkg
	//renderer->SetBackground(1,1,1);//white bkg
		

	//if(fDrawVolumeSlice && fIsVolumeDataAvailable && imageSliceActor) {		
		//renderer->AddActor(slicingPlaneActor);//METHOD 1
		//renderer->AddActor(imageSliceActor);//METHOD 2
		//renderer->AddActor(imageSliceViewer->GetImageActor());//METHOD 3
		//renderer->AddViewProp(imageSlice2);//METHOD 4
	//}

	//## SLICES
	if(fDrawVolumeSlice && fIsVolumeDataAvailable){
		if(fDrawVolumeSliceXY && slicingPlaneActorXY) renderer->AddActor(slicingPlaneActorXY);
		if(fDrawVolumeSliceXZ && slicingPlaneActorXZ) renderer->AddActor(slicingPlaneActorXZ);		
		if(fDrawVolumeSliceYZ && slicingPlaneActorYZ) renderer->AddActor(slicingPlaneActorYZ);
	}

	//## VOLUME RENDERING
	if(fDrawVolume && fIsVolumeDataAvailable && volume) {
		renderer->AddActor(volume);
	}

	//## ISO-SURFACES
	if(fDrawIsoSurface && fIsVolumeDataAvailable){
		if(fDrawIsoSurface1 && contourActor1) renderer->AddActor(contourActor1);	
		if(fDrawIsoSurface2 && contourActor2) renderer->AddActor(contourActor2);	
	}
	

	//## POINTS
	if(fDrawPoints && fIsPointDataAvailable && pointDataActor) {
		renderer->AddActor(pointDataActor);	
	}

	//## CONTAINER
	if(fDrawContainer && fIsContainerDataAvailable && containerActor) {
		renderer->AddActor(containerActor);
	}
	
	//## PALETTE
	if(!fIsFileNameEmpty && !fIsCorruptedFile && fDrawPaletteBar && paletteBarActor) {
		renderer->AddActor(paletteBarActor);
	}

	//## BOUNDING BOX
  if(!fIsFileNameEmpty && !fIsCorruptedFile && fDrawBoundingBox && boundingBoxActor) {	
		renderer->AddActor(boundingBoxActor);
	}	
	
	
	//## Set camera view
	renderer->ResetCamera();	
	renderer->GetActiveCamera()->Azimuth(fCameraAzimuth);
	renderer->GetActiveCamera()->Elevation(fCameraElevation);
	renderer->GetActiveCamera()->Zoom(1.4);
	

	vtkWidget->GetRenderWindow()->AddRenderer(renderer);
	vtkWidget->GetRenderWindow()->Render();

	
	//## Draw volume slices in separate vtk widget
	/*
	vtkRenderer* renderer2 = imageSliceViewer->GetRenderer();
	if(fDrawBoundingBox && boundingBoxActor) renderer2->AddActor(boundingBoxActor);

	renderer2->ResetCamera();

	//vtkRenderWindow* renderWindow = imageSliceViewer->GetRenderWindow();
	//vtkWidget2->SetRenderWindow(renderWindow);	

	//imageSliceViewer->Render();
	vtkWidget2->GetRenderWindow()->AddRenderer(renderer2);
	vtkWidget2->GetRenderWindow()->Render();
	//imageSliceViewer->SetupInteractor(renderWindow->GetInteractor());
	*/

}//close VolumeRenderingViewer::Draw()



void VolumeRenderingViewer::Filter(){

	cout<<"--> Filtering..."<<endl;

	fFilterRecMap3D->Reset();
	//fRecMap3D->Copy(*fFilterRecMap3D);

	double NCrown= (2*fNCrownX+1)*(2*fNCrownY+1)*(2*fNCrownZ+1);
	int alphaN= floor(fFilterAlpha*NCrown);
	
	int Nx= fRecMap3D->GetNbinsX();
	int Ny= fRecMap3D->GetNbinsY();
	int Nz= fRecMap3D->GetNbinsZ();
	
	int nTotBins= Nx*Ny*Nz;
	cout<<"NCrown="<<NCrown<<"  alphaN="<<alphaN<<endl;

	for(int iz=0;iz<Nz;iz++){
		for(int iy=0;iy<Ny;iy++){
			for(int ix=0;ix<Nx;ix++){
			
				int xBinId= ix+1;
				int yBinId= iy+1;
				int zBinId= iz+1;
				int globalBinId= fRecMap3D->GetBin(xBinId,yBinId,zBinId);
				double S= fRecMap3D->GetBinContent(xBinId,yBinId,zBinId);
				int N= fNEventMap3D->GetBinContent(xBinId,yBinId,zBinId);

				//cout<<"gBinId="<<globalBinId<<"  ("<<ix<<","<<iy<<","<<iz<<")  (x,y,z)=("<<RecMap3D->GetXaxis()->GetBinCenter(xBinId)<<","<<RecMap3D->GetYaxis()->GetBinCenter(yBinId)<<","<<RecMap3D->GetZaxis()->GetBinCenter(zBinId)<<") S="<<S<<endl;

				if(S<=0 || N<fNMinEvents){
					fFilterRecMap3D->SetBinContent(xBinId,yBinId,zBinId,S);
					continue;
				}


				int NPixelsInCrown= 0;
				std::vector<double> pixelValueList;
				pixelValueList.clear();
				pixelValueList.resize(0);

				//## Get pixel values in crown
				for(int k=zBinId-fNCrownZ;k<=zBinId+fNCrownZ;k++){
					for(int j=yBinId-fNCrownY;j<=yBinId+fNCrownY;j++){
						for(int i=xBinId-fNCrownX;i<=xBinId+fNCrownX;i++){
					
							int gBin= fRecMap3D->GetBin(i,j,k);
							if(fRecMap3D->IsBinOverflow(gBin) || fRecMap3D->IsBinUnderflow(gBin)) continue;
							
							double crownS= fRecMap3D->GetBinContent(i,j,k);
							int crownN= fNEventMap3D->GetBinContent(i,j,k);

							if(crownS<=0 || crownN<fNMinEvents) continue;
 
							pixelValueList.push_back(crownS);

							NPixelsInCrown++;
							//cout<<"("<<i-1<<","<<j-1<<","<<k-1<<") (x,y,z)=("<<RecMap3D->GetXaxis()->GetBinCenter(i)<<","<<RecMap3D->GetYaxis()->GetBinCenter(j)<<","<<RecMap3D->GetZaxis()->GetBinCenter(k)<<") crownS="<<crownS<<"  NPixelsInCrown="<<NPixelsInCrown<<endl;
						}//end loop i
					}//end loop j
				}//end loop k
	
				if(NPixelsInCrown<=0){
					fFilterRecMap3D->SetBinContent(xBinId,yBinId,zBinId,S);
					continue;	
				}

				//## Order pixels by ascending values
				std::sort(pixelValueList.begin(), pixelValueList.end());

				//## Apply the alpha-trimmed filter and recalculate filtered pixel value
				int start= alphaN;
				int end= pixelValueList.size()-alphaN;
				double sumS= 0;	
				int nFilteredPixels= 0;
				for(unsigned int l=start;l<end;l++){
					sumS+= pixelValueList[l];	
					nFilteredPixels++;
				}//end loop filtered pixels
				
				double SFiltered= sumS/(double)(nFilteredPixels);
				fFilterRecMap3D->SetBinContent(xBinId,yBinId,zBinId,SFiltered);
				
				//cout<<"gBinId="<<globalBinId<<"  ("<<xBinId<<","<<yBinId<<","<<zBinId<<")  S="<<S<<"  SFiltered="<<SFiltered<<"  NPixelsInCrown="<<NPixelsInCrown<<"  nFilteredPixels="<<nFilteredPixels<<endl;

			}//end loop x bins 
		}//end loop y bins 
	}//end loop z bins 

	cout<<"Filter(): min/max="<<fFilterRecMap3D->GetMinimum(0)<<"/"<<fFilterRecMap3D->GetMaximum()<<endl;

	
}//close VolumeRenderingViewer::Filter()


double VolumeRenderingViewer::GetTransferFunction(double x,double x0,double slope,double baseline){

	double expArg= (x-x0)/slope;
	double arg= exp(expArg);
	double fcn= -1./(arg+1.+baseline) + 1; 
	
	return fcn;

}//close GetTransferFunction()

void VolumeRenderingViewer::ReadPaletteFile(){

	std::ifstream inputPaletteFileStream;
	inputPaletteFileStream.open(fPaletteFileName.c_str());
	if(!inputPaletteFileStream.good()) {
		cerr<<"VolumeRenderingViewer::ReadPaletteFile(): ERROR: Cannot open palette file...return!"<<endl;
		return;
  }

	fNColorsInPalette= 0;
	std::string s;
	while(getline(inputPaletteFileStream,s)) {
  	fNColorsInPalette++;
  }
   
	inputPaletteFileStream.clear();
	inputPaletteFileStream.seekg(ios::beg);//rewind for reading

	
	double step, color_r, color_g, color_b, color_alpha;
	fColorPaletteStep.clear();
	fColorPaletteStep.resize(0);
	fColorPaletteR.clear();
	fColorPaletteR.resize(0);
	fColorPaletteG.clear();
	fColorPaletteG.resize(0);
	fColorPaletteB.clear();
	fColorPaletteB.resize(0);
	fColorPaletteA.clear();
	fColorPaletteA.resize(0);


	for(int i=0;i<fNColorsInPalette;i++){
		std::string parsedline;
		std::getline(inputPaletteFileStream,parsedline);
		std::stringstream line(parsedline);
		line >> step >> color_r >> color_g >> color_b >> color_alpha;
		fColorPaletteStep.push_back(step);
		fColorPaletteR.push_back(color_r);
		fColorPaletteG.push_back(color_g);
		fColorPaletteB.push_back(color_b);
		fColorPaletteA.push_back(color_alpha);
		
	}//end loop colors
	
	inputPaletteFileStream.close();

}//close VolumeRenderingViewer::ReadPaletteFile()


void VolumeRenderingViewer::SetPalette(){

	//#########################
	//###   PALETTE
	//#########################
	//Reset all tables
	
	colorTransferFunction->SetColorSpaceToRGB();
	colorTransferFunction->RemoveAllPoints();
	
	opacityTransferFunction->RemoveAllPoints();
	
	//## Set min/max of tables
	double volumeDataRange[2];
	volumeDataRange[0]= fVolumeDataMin;
	volumeDataRange[1]= fVolumeDataMax;
	
	//cout<<"volumeDataRange("<<volumeDataRange[0]<<","<<volumeDataRange[1]<<")"<<endl;

	//## Create lookup tables
	lut->SetTableRange(volumeDataRange); 
  lut->SetScaleToLinear();
 	lut->Build();

	lut_slice->SetTableRange(volumeDataRange); 
	lut_slice->SetScaleToLinear();
  //lut_slice->SetScaleToLog10();
 	lut_slice->Build();
	
	
	//## Set default colors & opacity in palette
	int nColorsInPalette= 11;
	double R_PaletteColor[]= {0.150,0.250,0.450,0.670,0.880,1,1,1,0.97,0.85,0.65};
	double G_PaletteColor[]= {0.306,0.630,0.853,0.973,1,1,0.880,0.679,0.430,0.150,0};
	double B_PaletteColor[]= {1,1,1,1,1,0.75,0.6,0.45,0.37,0.196,0.130};
	//double Alpha_PaletteColor[]= {0.03,0.03,0.03,0.03,0.1,0.1,0.1,0.1,0.5,0.8,1.0};
	double PaletteStep[]= {0,5,10,15,20,25,30,40,60,90,100};
	//double Alpha_PaletteColor[]= {0.01,0.01,0.01,0.01,0.1,0.5,1.0,1.0,1.0,1.0,1.0};
	double Alpha_PaletteColor[]= {0.01,0.01,0.01,0.01,0.1,0.1,0.1,0.1,0.5,0.8,1.0};
	//double PaletteStep[]= {0,51,76,102,127,178,190,204,217,242,255};
	 
	int maxIdPalette= PaletteStep[nColorsInPalette-1]+1;
	
	if(fUsePaletteFile){
		nColorsInPalette= fNColorsInPalette;
		maxIdPalette= fColorPaletteStep[nColorsInPalette-1]+1;
	}

 	lut->SetNumberOfTableValues(maxIdPalette);
	lut_slice->SetNumberOfTableValues(maxIdPalette);
	
 	int lastPalId= -1;
	double lastPalR= 0.;
  double lastPalG= 0.;
  double lastPalB= 0.;
  double lastPalA= 0.;
	double curPalR= 0.;
  double curPalG= 0.;
  double curPalB= 0.;
  double curPalA= 0.;
	
  for(int i=0; i<nColorsInPalette; i++){

		int steps= 0;
		if(fUsePaletteFile){
			steps= fColorPaletteStep[i]-lastPalId;

			if(fUseOpacityTransferFunction){
				double opacityFcn= GetTransferFunction(lastPalId,fOpacityTransferFunctionMean,fOpacityTransferFunctionSlope,fOpacityTransferFunctionBaseline);
				opacityTransferFunction->AddPoint(fColorPaletteStep[i], opacityFcn);
			} 
			else {
				opacityTransferFunction->AddPoint(fColorPaletteStep[i], fColorPaletteA[i]);
			}

			for(int j=0;j<steps;j++){
	  		lastPalId++;
	  		curPalR= ((j+1)*((fColorPaletteR[i]-lastPalR)/steps)) + lastPalR;
	  		curPalG= ((j+1)*((fColorPaletteG[i]-lastPalG)/steps)) + lastPalG;
	  		curPalB= ((j+1)*((fColorPaletteB[i]-lastPalB)/steps)) + lastPalB;
	
				if(fUseOpacityTransferFunction){
					curPalA= GetTransferFunction(lastPalId,fOpacityTransferFunctionMean,fOpacityTransferFunctionSlope,fOpacityTransferFunctionBaseline);	
				}
				else{
					curPalA= ((j+1)*((fColorPaletteA[i]-lastPalA)/steps)) + lastPalA;
				}			
			
	  		lut->SetTableValue(lastPalId,curPalR,curPalG,curPalB,curPalA);
				lut_slice->SetTableValue(lastPalId,curPalR,curPalG,curPalB,curPalA);

			}//end loop palette step  
			
			lastPalR= curPalR;  
			lastPalG= curPalG;  
			lastPalB= curPalB;  
			lastPalA= curPalA; 

		}//close if palette file	
		else{
			steps= PaletteStep[i]-lastPalId;

			if(fUseOpacityTransferFunction){
				double opacityFcn= GetTransferFunction(lastPalId,fOpacityTransferFunctionMean,fOpacityTransferFunctionSlope,fOpacityTransferFunctionBaseline);
				opacityTransferFunction->AddPoint(PaletteStep[i], opacityFcn);
			} 
			else {
				opacityTransferFunction->AddPoint(PaletteStep[i], Alpha_PaletteColor[i]);
			}

			for(int j=0;j<steps;j++){
	  		lastPalId++;
	  		curPalR= ((j+1)*((R_PaletteColor[i]-lastPalR)/steps)) + lastPalR;
	  		curPalG= ((j+1)*((G_PaletteColor[i]-lastPalG)/steps)) + lastPalG;
	  		curPalB= ((j+1)*((B_PaletteColor[i]-lastPalB)/steps)) + lastPalB;
	
				if(fUseOpacityTransferFunction){
					curPalA= GetTransferFunction(lastPalId,fOpacityTransferFunctionMean,fOpacityTransferFunctionSlope,fOpacityTransferFunctionBaseline);	
				}
				else{
					curPalA= ((j+1)*((Alpha_PaletteColor[i]-lastPalA)/steps)) + lastPalA;
				}			
			
	  		lut->SetTableValue(lastPalId,curPalR,curPalG,curPalB,curPalA);
				lut_slice->SetTableValue(lastPalId,curPalR,curPalG,curPalB,1.0);
			}//end loop palette step  
			
			lastPalR= curPalR;  
			lastPalG= curPalG;  
			lastPalB= curPalB;  
			lastPalA= curPalA; 
		}//close else

  }//end loop colors in palette	

	//## Set colors in palette
 	int numOfColors = lut->GetNumberOfTableValues();
  double step = (volumeDataRange[1] - volumeDataRange[0])/numOfColors;
		
  for(double i = volumeDataRange[0]; i <= volumeDataRange[1];i+=step){
    double color[3];
    double alpha;

    lut->GetColor(i,color);
    colorTransferFunction->AddRGBPoint(i,color[0], color[1], color[2]);
  }
  
  lut->Build();
	lut_slice->Build();

}//close SetPalette()


void VolumeRenderingViewer::SetTransferFunction(bool status){

	//fUseOpacityTransferFunction= groupBox_paletteOptions->isChecked();
	fUseOpacityTransferFunction= checkBox_usePaletteTransferFunction->isChecked();
	
	Draw();

}//close VolumeRenderingViewer::SetTransferFunction()

void VolumeRenderingViewer::SetTransferFunctionOptions(){

	//fUseOpacityTransferFunction= groupBox_paletteOptions->isChecked();
	fUseOpacityTransferFunction= checkBox_usePaletteTransferFunction->isChecked();
	
	//fOpacityTransferFunctionMean= slider_paletteFcnMean->value();
	//fOpacityTransferFunctionSlope= slider_paletteFcnSlope->value();
	//fOpacityTransferFunctionBaseline= slider_paletteFcnBaseline->value();
	fOpacityTransferFunctionMean= spinbox_paletteFcnMean->value();
	fOpacityTransferFunctionSlope= spinbox_paletteFcnSlope->value();
	fOpacityTransferFunctionBaseline= spinbox_paletteFcnBaseline->value();

	fUpdateDraw= true;

	//## Re-draw volumes
	//Draw();

}//close VolumeRenderingViewer::SetTransferFunctionOptions()


void VolumeRenderingViewer::SetDataOptions(){

	//## Get data options
	fNMinEvents= spinbox_nMinEvents->value();
	fLgScalarMinCut= spinbox_scalarMinCut->value();
	fLgScalarMaxCut= spinbox_scalarMaxCut->value();
	fUseLogScalars= checkBox_useLogScalars->isChecked();
	
	fUseAlphaFilter= checkBox_alphaFilter->isChecked();
	fFilterAlpha= spinbox_alphaFilter->value();	
	fNCrownX= spinbox_ncrownX->value(); 
	fNCrownY= spinbox_ncrownX->value(); 
	fNCrownZ= spinbox_ncrownX->value(); 	

	fSignalThreshold= spinbox_signalThreshold->value();

	//## Trigger both data read and draw
	fUpdateDraw= true;
	fUpdateDataRead= true;
	fUpdatePrint= true;

}//close VolumeRenderingViewer::SetDataOptions()


void VolumeRenderingViewer::Update(){

	//## Re-read volume data
	if(fUpdateDataRead){		
		ReadVolumeData();
		fUpdateDataRead= false;
	}

	//## Re-draw volumes
	if(fUpdateDraw){	
		Draw();
		fUpdateDraw= false;
	}

	//## Re-print list of threat voxels
	if(fUpdatePrint){
		DumpInfo();	
	}

}//close VolumeRenderingViewer::Update()


void VolumeRenderingViewer::DumpInfo(){

	//## Clear existing entries in info widget
	infoWidget->clear();
	
	//## Add new entries to widget
	QStringList text;

	text.append(tr("==== THREAT LIST ===="));
	text.append(tr("--> %1 threats!").arg(fThreatPosList.size()));
	
  for(unsigned int i=0;i<fThreatPosList.size();i++){
		double threatPosX= fThreatPosList[i].X();
		double threatPosY= fThreatPosList[i].Y();
		double threatPosZ= fThreatPosList[i].Z();
		double threatSignal= fThreatSignalList[i];
		
  	text.append(tr("Threat no. %1: Pos(%2,%3,%4), S=%5").arg(i+1).arg(threatPosX).arg(threatPosY).arg(threatPosZ).arg(threatSignal));
	}
	text.append(tr("======================================="));

  infoWidget->insertItems(0,text);
	//infoWidget->update();

}//close VolumeRenderingViewer::DumpInfo()



void VolumeRenderingViewer::SetCameraOptions(){

	cout<<"VolumeRenderingViewer::SetCameraOptions(): INFO: Camera changed..."<<endl;

	fIsCameraChanged= true;

	if(radioButton_cameraXY->isChecked()){
		fCameraAzimuth= 0;
		fCameraElevation= 0;
	}
	else if(radioButton_cameraXZ->isChecked()){
		fCameraAzimuth= 90;
		fCameraElevation= 0;
	}
	else if(radioButton_cameraYZ->isChecked()){
		fCameraAzimuth= 0;
		fCameraElevation= 90;
	}
	else if(radioButton_cameraDefault->isChecked()){
		fCameraAzimuth= fCameraDefaultAzimuth;
		fCameraElevation= fCameraDefaultElevation;
	}
	else{

	}	
	
	Draw();

}//close VolumeRenderingViewer::SetCameraOptions()

void VolumeRenderingViewer::SetCameraDefaultOptions(){

	fIsCameraChanged= true;
	fCameraAzimuth= fCameraDefaultAzimuth;
	fCameraElevation= fCameraDefaultElevation;
	Draw();

}//close VolumeRenderingViewer::SetCameraOptions()


void VolumeRenderingViewer::SetVolumeSliceOrientation(){

	/*
	if(radioButton_sliceXY->isChecked()){
		slicingPlane->SetNormal(0,0,1);//z plane
		slicingPlane->SetOrigin(0,0,fSlicePosition); 
	}
	else if(radioButton_sliceXZ->isChecked()){
		slicingPlane->SetNormal(0,1,0);//y plane
		slicingPlane->SetOrigin(0,fSlicePosition,0); 	
	}
	else if(radioButton_sliceYZ->isChecked()){	
		slicingPlane->SetNormal(1,0,0);//x plane
		slicingPlane->SetOrigin(fSlicePosition,0,0); 
	}
	
	Draw();
	*/

}//close VolumeRenderingViewer::SetVolumeSliceOrientation()

void VolumeRenderingViewer::SetVolumeSliceXY(int slicePos){

	slicingPlaneXY->SetOrigin(0,0,slicePos); 
	planeCutterXY->SetCutFunction(slicingPlaneXY);
  planeCutterXY->SetInput(imageScaled);

	planeCutterXY->SetGenerateCutScalars(0);
	planeCutterXY->Update();

	planeCutterMapperXY->SetInputConnection(planeCutterXY->GetOutputPort());
  planeCutterMapperXY->SetScalarRange(0,255);

	Draw();

}//close VolumeRenderingViewer::SetVolumeSliceXY()


void VolumeRenderingViewer::SetVolumeSliceXZ(int slicePos){

	slicingPlaneXZ->SetOrigin(0,slicePos,0); 
	planeCutterXZ->SetCutFunction(slicingPlaneXZ);
  planeCutterXZ->SetInput(imageScaled);

	planeCutterXZ->SetGenerateCutScalars(0);
	planeCutterXZ->Update();

	planeCutterMapperXZ->SetInputConnection(planeCutterXZ->GetOutputPort());
  planeCutterMapperXZ->SetScalarRange(0,255);

	Draw();

}//close VolumeRenderingViewer::SetVolumeSliceXZ()


void VolumeRenderingViewer::SetVolumeSliceYZ(int slicePos){

	slicingPlaneYZ->SetOrigin(slicePos,0,0); 
	planeCutterYZ->SetCutFunction(slicingPlaneYZ);
  planeCutterYZ->SetInput(imageScaled);

	planeCutterYZ->SetGenerateCutScalars(0);
	planeCutterYZ->Update();

	planeCutterMapperYZ->SetInputConnection(planeCutterYZ->GetOutputPort());
  planeCutterMapperYZ->SetScalarRange(0,255);

	Draw();

}//close VolumeRenderingViewer::SetVolumeSliceXZ()


void VolumeRenderingViewer::SetVolumeSlice(int slicePos){

	/*
	if(radioButton_sliceXY->isChecked()){
		slicingPlane->SetNormal(0,0,1);//z plane
		slicingPlane->SetOrigin(0,0,slicePos); 
	}
	else if(radioButton_sliceXZ->isChecked()){
		slicingPlane->SetNormal(0,1,0);//y plane
		slicingPlane->SetOrigin(0,slicePos,0); 	
	}
	else if(radioButton_sliceYZ->isChecked()){	
		slicingPlane->SetNormal(1,0,0);//x plane
		slicingPlane->SetOrigin(slicePos,0,0); 
	}
	
	cout<<"slicePos="<<slicePos<<endl;
	
  planeCutter->SetCutFunction(slicingPlane);
  planeCutter->SetInput(imageScaled);

	planeCutter->SetGenerateCutScalars(0);
	planeCutter->Update();

	planeCutterMapper->SetInputConnection(planeCutter->GetOutputPort());
  planeCutterMapper->SetScalarRange(0,255);
	
	fSlicePosition= slicePos;

	//## Re-draw volumes
	Draw();
	*/

}//close VolumeRenderingViewer::SetVolumeSlice()


void VolumeRenderingViewer::SetIsoSurfaceValue1(double value){

	
	double minContour= std::max(value-fIsoSurfaceScalarWidth/2.,0.);
	double maxContour= std::min(value+fIsoSurfaceScalarWidth/2.,255.);
	contourFilter1->GenerateValues(1,minContour,maxContour);
	Draw();

}//close VolumeRenderingViewer::SetIsoSurfaceValue1()

void VolumeRenderingViewer::SetIsoSurfaceValue2(double value){

	double minContour= std::max(value-fIsoSurfaceScalarWidth/2.,0.);
	double maxContour= std::min(value+fIsoSurfaceScalarWidth/2.,255.);
	contourFilter2->GenerateValues(1,minContour,maxContour);
	Draw();

}//close VolumeRenderingViewer::SetIsoSurfaceValue2()



void VolumeRenderingViewer::SetDrawOptions(){

	fIsCameraChanged= false;
	
	fDrawVolume= checkBox_showVolume->isChecked();
	fDrawPoints= checkBox_showPoints->isChecked();
	fDrawBoundingBox= checkBox_showBoundingBox->isChecked();
	fDrawPaletteBar= checkBox_showPalette->isChecked();
	fDrawContainer= checkBox_showContainer->isChecked();
	//fDrawVolumeSlice= checkBox_showVolumeSlices->isChecked();
	//fDrawVolumeSlice= groupBox_sliceOptions->isChecked();
	fDrawVolumeSlice= groupBox_slice->isChecked();
	fDrawVolumeSliceXY= checkBox_sliceXY->isChecked();
	fDrawVolumeSliceXZ= checkBox_sliceXZ->isChecked();
	fDrawVolumeSliceYZ= checkBox_sliceYZ->isChecked();

	//fDrawIsoSurface= checkBox_showIsoSurface->isChecked();
	fDrawIsoSurface= groupBox_IsoSurface->isChecked();
	fDrawIsoSurface1= checkBox_showIsoSurface1->isChecked();
	fDrawIsoSurface2= checkBox_showIsoSurface2->isChecked();
	
	
	Draw();

}//close VolumeRenderingViewer::SetCameraOptions()


void VolumeRenderingViewer::BrowseInputPaletteFile(){
	
	QString fileName = QFileDialog::getOpenFileName(window,tr("Find Files"), QDir::currentPath(), tr("*.txt *.dat *.out"));

	if (!fileName.isEmpty()) {
  	if (comboBox_inputPaletteFile->findText(fileName) == -1) comboBox_inputPaletteFile->addItem(fileName);
    comboBox_inputPaletteFile->setCurrentIndex(comboBox_inputPaletteFile->findText(fileName));
  }

	fPaletteFileName= (comboBox_inputPaletteFile->currentText()).toStdString();
	fUsePaletteFile= checkBox_inputPaletteFile->isChecked();
	if(fUsePaletteFile){	
		ReadPaletteFile();
		Draw();
	}

}//close VolumeRenderingViewer::BrowseInputPaletteFile()




void VolumeRenderingViewer::Open(){
   
	QString fileName = QFileDialog::getOpenFileName(window,tr("Find File"), QDir::currentPath(), tr("*.root"));

  if (!fileName.isEmpty()) {
		fInputFileName= fileName.toStdString();
		process();
	}
	
}//close VolumeRenderingViewer::open()


void VolumeRenderingViewer::Save(){

	//## Find filename	
	QString fileName = QFileDialog::getSaveFileName(window,tr("Save File"), QDir::currentPath(), tr("*.png"));
	
	if (!fileName.isEmpty()) {
  	fOutputImageFileName= fileName.toStdString();

		//## Export to PNG
		int magnification= 1;
		vtkWindowToImageFilter* w2i= vtkWindowToImageFilter::New();
		w2i->SetInput(vtkWidget->GetRenderWindow());
		w2i->SetMagnification(magnification);
		w2i->Update();

		vtkPNGWriter* writer = vtkPNGWriter::New();
  	writer->SetFileName(fOutputImageFileName.c_str());
  	writer->SetInput(w2i->GetOutput());
  	writer->Write();
  }

	
}//close Save()


void VolumeRenderingViewer::Close(){

	if(window) window->close();

	emit finished();
	
}//close Close()


void VolumeRenderingViewer::About(){

	QMessageBox::about(window, tr("About Application"),
             tr("The <b>Application</b> example demonstrates how to "
                "write modern GUI applications using Qt, with a menu bar, "
                "toolbars, and a status bar."));
 
}//close VolumeRenderingViewer::about()


