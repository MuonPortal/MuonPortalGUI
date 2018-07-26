#include <EMLLViewerThreadObj.h>

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


#include <iomanip>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <math.h>
#include <assert.h>

using namespace std;


EMLLViewerThreadObj::EMLLViewerThreadObj(){

}//close constructor

EMLLViewerThreadObj::~EMLLViewerThreadObj(){

}//close destructor


void EMLLViewerThreadObj::process(){

	std::string command= std::string("./Viewer --likelihood ") + fInputFileName;
	
	system(command.c_str());
	
	emit finished();	

}//close EMLLViewerThreadObj::process()




