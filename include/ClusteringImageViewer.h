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
* @file ClusteringImageViewer.h
* @class ClusteringImageViewer
* @brief Clustering ROOT image viewer
*
* @author S. Riggi
* @date 23/03/2012
*/
#ifndef ClusteringImageViewer_h
#define ClusteringImageViewer_h 1

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
#include <TGeoManager.h>
#include <TGeoMaterial.h>
#include <TGeoMedium.h>
#include <TGeoVolume.h>

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include <TGFrame.h>

namespace MuonPortalNS {


class ClusteringImageViewer : public TGMainFrame {
	
	public:	

		ClusteringImageViewer(const TGWindow *p,UInt_t w,UInt_t h,std::string filename);

		virtual ~ClusteringImageViewer();

	public:
		void SetGraphicsStyle();
		void ReadData();
		void Init();
		//void Draw(std::vector< std::vector<TPolyMarker3D*> >);
		//void Draw(TPolyMarker3D*,int nClusters);
		void Draw();
		void Draw3D();
		void Modify3D();
		void Slide3D(int);

		void SlideThetaRange();
		void SlideThetaRangeText(char*);
		void ApplyThreshold();

	private:

		static const int MAXNPOINTS= 2000000;
		int nPoints;
		int nSelPoints;
		
		TH1D* colorPaletteHisto;

		int fNClusters;	
		
		std::vector<int> fNClusterPoints;
		std::vector<int> fClusterWeight;
		std::vector< std::vector<TVector3> > fClusterPointList;
		std::vector< std::vector<double> > fClusterPointThetaList;
		//std::vector< std::vector<TPolyMarker3D> > fClusterPointGraphList;
		std::vector<TVector3> posList;
		std::vector<double> thetaList;
		std::vector<int> clusterIdList;

		//static const int MAXNCLUSTERS= 10000;
		//TPolyMarker3D fClusterPointGraphList[MAXNCLUSTERS];
		TPolyMarker3D fClusterPointGraphList[MAXNPOINTS];
		TPolyMarker3D fClusterPointGraphList_sel[MAXNPOINTS];
		//std::vector<TPolyMarker3D*> clusterPolyMarkerList;
		
		double minWeight;
		double maxWeight;

		TRootEmbeddedCanvas* f3DTomographyCanvas;
		TRootEmbeddedCanvas* f3DRendTomographyCanvas;
		TGDoubleHSlider* fSliderThetaRange;
		TGTextEntry* fSliderMinThetaText;
		TGTextBuffer* fSliderMinThetaTextBuffer;	
		TGTextEntry* fSliderMaxThetaText;
		TGTextBuffer* fSliderMaxThetaTextBuffer;	

		TGHSlider* fSlider3D;

		TGNumberEntry* fThresholdNumEntry;
		TGNumberEntry* fEventThresholdNumEntry;

		static const int MAXNIMAGES= 1000;
		int fNImage3D;		
		TASImageNew* image3DList[MAXNIMAGES];
		TASImageNew* currentImage3D;
		//MyTASImage* image3DList[MAXNIMAGES];
		//MyTASImage* currentImage3D;

		std::string fInputFileName;
		TStyle* myStyle;

		TGeoManager *geom;
		TGeoMaterial *matVacuum;
		TGeoMedium *Vacuum;
		TGeoVolume *top;

	ClassDef(ClusteringImageViewer,0)

};

#ifdef __MAKECINT__
#pragma link C++ class ClusteringImageViewer+; 
#endif

}//close namespace

#endif

