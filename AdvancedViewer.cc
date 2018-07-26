

#include <VolumeRenderingViewer.h>
#include <QApplication>
#include <QCoreApplication>
#include <QtGui>

#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

using namespace std;


void usage();



int main(int argc, char *argv[]){

	std::string inputFileName= "";

	if(argc<2){
		cerr<<"WARNING: Missing input filename argument..."<<endl;
		usage();
	}
	else{
		cout<<"INFO: Visualizing data file "<<argv[1]<<" ..."<<endl;
		inputFileName= std::string(argv[1]);
	}
	
	QApplication app(argc, argv);
	app.setApplicationName("Muon Tomography Viewer");

	VolumeRenderingViewer* viewer= new VolumeRenderingViewer();
	viewer->SetInputFileName(inputFileName);
	viewer->SetOutputImageFileName("VolRendFig.png");
  viewer->SetDataRange(pow(10,-7),pow(10,-1));
	viewer->process();

	return app.exec();

}//close main



void usage(){

	cout<<endl;
	cout<<"*** PROGRAM USAGE ***"<<endl;
	cout<<"AdvancedViewer [input file]"<<endl;
	cout<<""<<endl;
	
}//close usage()

