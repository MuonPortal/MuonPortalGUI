#ifndef EMLLImageViewer_h
#define EMLLImageViewer_h 1


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
//#include <MyImage.h>

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

class EMLLImageViewer : public TGMainFrame {
	
	public:	

		EMLLImageViewer(const TGWindow *p,UInt_t w,UInt_t h,std::string filename);

		virtual ~EMLLImageViewer();

	public:

		void SetGraphicsStyle();
		void ReadData();
		void Init();
		void DrawXY();
		void ModifyXY();
		void SlideXY(int);
		void DrawXZ();
		void ModifyXZ();
		void SlideXZ(int);
		void DrawYZ();
		void ModifyYZ();
		void SlideYZ(int);

		void Draw3D();
		void Slide3D(int);
		void Modify3D();
	
		void DrawXYZ();
		void DoubleSlideXYZ();
		void DoubleSlideXYZText(char*);
		
		void ApplyThresholdXY();
		void ApplyThresholdXZ();
		void ApplyThresholdYZ();
		
	
	private:

		static const int MAXNPOINTS= 2000000;
		int nPoints;
		int nSelPoints;
		TPolyMarker3D fDataGraphList[MAXNPOINTS];
		TVector3 fDataPointList[MAXNPOINTS];
		double fDataPointThetaList[MAXNPOINTS];
		TH1D* colorPaletteHisto;

		TRootEmbeddedCanvas* fXYTomographyCanvas;
		TRootEmbeddedCanvas* fXZTomographyCanvas;
		TRootEmbeddedCanvas* fYZTomographyCanvas;
		TRootEmbeddedCanvas* fXYZTomographyCanvas;
		TRootEmbeddedCanvas* f3DTomographyCanvas;
		TGHSlider* fSliderXY;
		TGHSlider* fSliderXZ;
		TGHSlider* fSliderYZ;
		TGHSlider* fSliderXYZ;
		TGDoubleHSlider* fDoubleSliderXYZ;
		TGHSlider* fSlider3D;

		TGTextEntry* fSliderTextXY;
		TGTextBuffer* fSliderTextBufferXY;
		TGTextEntry* fSliderTextXZ;
		TGTextBuffer* fSliderTextBufferXZ;
		TGTextEntry* fSliderTextYZ;
		TGTextBuffer* fSliderTextBufferYZ;
		TGTextEntry* fSliderTextXYZ;
		
		TGTextEntry* fDoubleSliderMinTextXYZ;
		TGTextBuffer* fDoubleSliderMinTextBufferXYZ;	
		TGTextEntry* fDoubleSliderMaxTextXYZ;
		TGTextBuffer* fDoubleSliderMaxTextBufferXYZ;

		TGNumberEntry* fThresholdNumEntryXY;
		TGNumberEntry* fEventThresholdNumEntryXY;
		TGNumberEntry* fThresholdNumEntryXZ;
		TGNumberEntry* fEventThresholdNumEntryXZ;
		TGNumberEntry* fThresholdNumEntryYZ;
		TGNumberEntry* fEventThresholdNumEntryYZ;

		std::string fInputFileName;
		bool isFileReadCall;

		static const int MAXNIMAGES= 1000;
		int fNImageXY;	
		int fNImageXZ;	
		int fNImageYZ;	
		int fNImage3D;	
			
		TASImageNew* imageXYList[MAXNIMAGES];
		TASImageNew* imageXZList[MAXNIMAGES];
		TASImageNew* imageYZList[MAXNIMAGES];
		TASImageNew* image3DList[MAXNIMAGES];
		TASImageNew* currentImageXY;
		TASImageNew* currentImageXZ;
		TASImageNew* currentImageYZ;
		TASImageNew* currentImage3D;

		/*
		MyTASImage* imageXYList[MAXNIMAGES];
		MyTASImage* imageXZList[MAXNIMAGES];
		MyTASImage* imageYZList[MAXNIMAGES];
		MyTASImage* image3DList[MAXNIMAGES];
		MyTASImage* currentImageXY;
		MyTASImage* currentImageXZ;
		MyTASImage* currentImageYZ;
		MyTASImage* currentImage3D;
		*/
		
		TH2D* histoXYList[MAXNIMAGES];
		TH2D* histoXZList[MAXNIMAGES];
		TH2D* histoYZList[MAXNIMAGES];
		TH2D* eventhistoXYList[MAXNIMAGES];
		TH2D* eventhistoXZList[MAXNIMAGES];
		TH2D* eventhistoYZList[MAXNIMAGES];
		TH2D* currentHistoXY;
		TH2D* currentHistoXZ;
		TH2D* currentHistoYZ;		
		TH3D* fRecMap3D;
		double minSignal;
		double maxSignal;


		TImagePalette* currentPalette;
		TStyle* myStyle;
		
	ClassDef(EMLLImageViewer,0)

};

#ifdef __MAKECINT__
#pragma link C++ class EMLLImageViewer+; 
#endif


#endif


