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
* @file ACFImageViewer.h
* @class ACFImageViewer
* @brief ACF ROOT image viewer
*
* @author S. Riggi
* @date 23/03/2012
*/
#ifndef ACFImageViewer_h
#define ACFImageViewer_h 1


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

#include <TGFrame.h>

namespace MuonPortalNS {

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

}//close namespace

#endif


