/**
* @file MPToolkit.cc
* @class MPToolkit
* @brief main class used to handle the entire project
*
* The simulation can run either in a batch mode or in interactive mode using visualization of detectors, tracks, etc...
* To select the run mode, just give in the command line the arguments "--batch" or "--interactive". The program will look for the "batchConfigFile.mac" or "interactiveConfigFile.mac" config files in the config_macros/ directory.
* As example, one can also handle a simulation output, for example giving as argument "--process".
* @author S. Riggi
* @date 05/04/2010
*/
#include <Gui.h>
#include <GuiServer.h>
#include <Logger.h>
#include <ConfigParser.h>

#include <VolumeRenderingViewer.h>


#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include <QApplication>
#include <QCoreApplication>


//## G4 classes
#include <G4DetectorGeometry.h>
#include <G4PhysicsList.h>
#include <G4PrimaryGenerator.h>
#include <SteppingVerbose.h>

#include <G4Navigator.hh>
#include <G4GeometryManager.hh>
#include <G4TransportationManager.hh>
#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <G4Material.hh>
#include <G4VPhysicalVolume.hh>
#include <G4UnitsTable.hh>

#include <G4VisExecutive.hh>


//## VTK classes
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
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

 
using namespace std;
using namespace MuonPortalNS;

//## Define functions
int RunGUI(int argc, char *argv[]);
void Usage(char* exeName);

static const struct option options_tab[] = {
  /* name, has_arg, &flag, val */
  { "help", no_argument, 0, 'h' },
	{ "server", no_argument, 0, 's' },	
	{ "port", required_argument, 0, 'p' },
	{ "verbosity", required_argument, 0, 'v'},
  {(char*)0, (int)0, (int*)0, (int)0}
};

void Usage(char* exeName)
{
	cout<<"=========== USAGE ==========="<<endl;
	cout<<"Usage: "<<exeName<<" [options]"<<endl;
	cout<<endl;
	cout<<"Options:"<<endl;
  cout<<"-h, --help \t Show help message and exit"<<endl;
	cout<<"-s, --server \t Launch TCP server with GUI"<<endl;
	cout<<"-p, --port=[PORT_NUMBER] \t TCP server port number (default=9999)"<<endl;
	cout<<"-v, --verbosity \t Log level (<=0=OFF, 1=FATAL, 2=ERROR, 3=WARN, 4=INFO, >=5=DEBUG)"<<endl;
	cout<<"=============================="<<endl;
	
}//close Usage()

std::string GetStringLogLevel(int verbosity){

	std::string slevel= "";
	if(verbosity<=0) slevel= "FATAL";
	else if(verbosity==1) slevel= "FATAL";
	else if(verbosity==2) slevel= "ERROR";
	else if(verbosity==3) slevel= "WARN";
	else if(verbosity==4) slevel= "INFO";
	else if(verbosity>5) slevel= "DEBUG";
	else slevel= "OFF";

	return slevel;

}//close GetStringLogLevel()

//## Define variables
Gui* gui;
GuiServer* guiServer;
long int portNo= 9999;
bool useGuiServer= false;
int verbosity= 4;


//================================
//==   MUON PORTAL GUI MAIN
//================================
int main(int argc, char *argv[])
{
	//## Parsing options
	int c = 0;
  int option_index = 0;
	
	while((c = getopt_long(argc, argv, "hp:sv:",options_tab, &option_index)) != -1) {
    
    switch (c) {
			case 0 : 
			{
				break;
			}
			case 'h':
			{
      	Usage(argv[0]);	
				exit(0);
			}
    	case 'p':
			{
				portNo= atol(optarg);	
				break;
			}
			case 's':
			{
				useGuiServer= true;
				break;
			}
			case 'v':	
			{
				verbosity= atoi(optarg);	
				break;	
			}
			default:
			{
      	Usage(argv[0]);	
				exit(0);
			}
    }//close switch
	}//close while

	//## Set logging level
	std::string sloglevel= GetStringLogLevel(verbosity);
	LoggerManager::Instance().CreateConsoleLogger(sloglevel,"logger","System.out");

	//## Run application
	RunGUI(argc, argv);

	return 0;

}//close main


int RunGUI(int argc, char *argv[]){

	cout<<"*** GUI ***"<<endl;
	QApplication app(argc, argv);
	app.setApplicationName("MPToolkitGUI");
  
	int status= 0;
	if(useGuiServer){
		INFO_LOG("Running GUI with TCP server running on port "<<portNo<<" ...");
		guiServer= new GuiServer(0,portNo);
		status = app.exec();

		if(guiServer){
  		delete guiServer;
  		guiServer = NULL;
		}

	}//close if
	else{
		INFO_LOG("Running GUI ...");
		gui= new Gui;
		gui->show();
		status= app.exec();

		if(gui){
 	 		delete gui;
  		gui= 0;
		}
	}//close else

  return status;

}//close RunGUI()







