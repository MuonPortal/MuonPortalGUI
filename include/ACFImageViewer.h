
#ifndef ACFImageViewer_h
#define ACFImageViewer_h 1

//#include "MyTASImage.h"
#include "TASImageNew.h"

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

#include <TAttImage.h>
#include <TGSlider.h>
#include <TGTextEntry.h>
#include <TGNumberEntry.h>
#include <TGTextBuffer.h>
#include <TGTripleSlider.h>

#include <TRootEmbeddedCanvas.h>
#include <TPolyMarker3D.h>

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;


#include <TGFrame.h>

class ACFImageViewer : public TGMainFrame {
	
	public:	

		ACFImageViewer(const TGWindow *p,UInt_t w,UInt_t h,std::string filename);

		virtual ~ACFImageViewer();

	public:

		void SetGraphicsStyle();
		void ReadData();
		void Init();
		void Draw();
		
	
	private:

		TRootEmbeddedCanvas* fACFCanvas;
		TRootEmbeddedCanvas* fACFWeightedCanvas;
		
		std::string fInputFileName;
		
		TGraph* ACFGraph_LS;
		TGraph* ACFGraph_H;
		TGraph* ACFGraph_DP;
		TGraph* ACFGraph_PH;
		
		TGraph* ACFWeightedGraph_LS;
		TGraph* ACFWeightedGraph_H;
		TGraph* ACFWeightedGraph_DP;
		TGraph* ACFWeightedGraph_PH;
		
		TStyle* myStyle;
		
	ClassDef(ACFImageViewer,0)

};

#ifdef __MAKECINT__
#pragma link C++ class ACFImageViewer+; 
#endif

#endif


