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
* @file AdvancedViewer.cc
* @class AdvancedViewer
* @brief ROOT image viewer
*
* @author S. Riggi
* @date 23/03/2012
*/

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
using namespace MuonPortalNS;

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

