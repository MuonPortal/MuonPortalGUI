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
* @file ACFImageViewer.cc
* @class ACFImageViewer
* @brief ACF ROOT image viewer
*
* @author S. Riggi
* @date 23/03/2012
*/

#include <ACFImageViewer.h>

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
#include <TLine.h>

#include <TApplication.h>
#include <TRootEmbeddedCanvas.h>
#include <TGClient.h>
#include <TGFrame.h>
#include <TGTab.h>
#include <TGFileDialog.h>
#include <TGComboBox.h>
#include <TGNumberEntry.h>
#include <TGMsgBox.h>
#include <TGButton.h>
#include <TGButtonGroup.h>
#include <TGLabel.h>

#include <TGeoVolume.h>
#include <TGeoManager.h>
#include <TGeoMedium.h>
#include <TGeoMatrix.h>

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

ClassImp(MuonPortalNS::ACFImageViewer)

namespace MuonPortalNS {

ACFImageViewer::ACFImageViewer(const TGWindow *p,UInt_t w,UInt_t h,std::string filename)
	: TGMainFrame(p,w,h){

	fInputFileName= filename;
	
	SetGraphicsStyle();
	
	
	//## Define styles
	const double width = w;
  const double height = h;
  
	TGLayoutHints* topRightLayout = new TGLayoutHints(kLHintsTop | kLHintsRight, 10,10,10,1);
	TGLayoutHints* topCenterLayout = new TGLayoutHints(kLHintsTop | kLHintsCenterX | kLHintsCenterY,10,10,10,1);
	TGLayoutHints* topLeftLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft,10,10,10,1);
	TGLayoutHints* bottomRightLayout = new TGLayoutHints(kLHintsBottom | kLHintsRight, 10,10,10,1);
	TGLayoutHints* bottomCenterLayout = new TGLayoutHints(kLHintsBottom | kLHintsCenterX | kLHintsCenterY,10,10,10,1);
	TGLayoutHints* bottomLeftLayout = new TGLayoutHints(kLHintsBottom | kLHintsLeft,10,10,10,1);
	TGLayoutHints* expandLayout = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1);
	TGLayoutHints* centerLayout = new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10,10,10,1);	
	TGLayoutHints* leftLayout = new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 10,10,10,1);
	TGLayoutHints* rightLayout = new TGLayoutHints(kLHintsRight | kLHintsCenterY, 10,10,10,1);
	TGLayoutHints* topLayout = new TGLayoutHints(kLHintsTop, 10,10,10,1);
	TGLayoutHints* bottomLayout = new TGLayoutHints(kLHintsBottom, 10,10,10,1);
	TGLayoutHints* canvasLayout = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1);
	//TGLayoutHints* bottomLayout = new TGLayoutHints(kLHintsBottom|kLHintsCenterY,3,3,3,3);

	
	

	//## Define object sizes
	double mainFrameWidth= 1400;
	double mainFrameHeight= 500;
	double tomographyFrameWidth= mainFrameWidth;
	double tomographyFrameHeight= 500;
	double tomographySubFrameWidth= 500;
	double tomographySubFrameHeight= tomographyFrameHeight;

	double canvasWidth= 700;
	double canvasHeight= 500;

	//## Define main frames
	TGVerticalFrame* MainFrameContainer= new TGVerticalFrame(this,mainFrameWidth,mainFrameHeight);
	
	//tomography frame
	TGHorizontalFrame* TomographyFrameContainer = new TGHorizontalFrame(MainFrameContainer,tomographyFrameWidth,tomographyFrameHeight);
	
	//lower button frame
	TGHorizontalFrame* buttonStatusFrame= new TGHorizontalFrame(MainFrameContainer, mainFrameWidth,30);
	
	MainFrameContainer->AddFrame(TomographyFrameContainer,new TGLayoutHints(kLHintsTop | kLHintsCenterY | kLHintsCenterX,2,2,2,2));
	MainFrameContainer->AddFrame(buttonStatusFrame,new TGLayoutHints(kLHintsTop | kLHintsCenterY | kLHintsCenterX,2,2,2,2));
	AddFrame(MainFrameContainer,centerLayout);



	//#################################
	//##      TOMOGRAPHY FRAME
	//#################################

	
	//canvas
	fACFCanvas = new TRootEmbeddedCanvas("2pt-ACF", TomographyFrameContainer, canvasWidth, canvasHeight);
	TomographyFrameContainer->AddFrame(fACFCanvas, new TGLayoutHints(kLHintsCenterY | kLHintsTop,2,2,2,2));

	//canvas
	fACFWeightedCanvas = new TRootEmbeddedCanvas("2pt-ACF weighted", TomographyFrameContainer, canvasWidth, canvasHeight);
	TomographyFrameContainer->AddFrame(fACFWeightedCanvas, new TGLayoutHints(kLHintsCenterY | kLHintsTop,2,2,2,2));

	
	//#################################
	//##      LOWER FRAME
	//#################################
	//exit button
	TGTextButton* exitButton = new TGTextButton(buttonStatusFrame,"&Exit ","gApplication->Terminate()");
	buttonStatusFrame->AddFrame(exitButton, centerLayout);


	
	//## Read data
	Init();
	ReadData();
	Draw();

	
	// Sets window name and shows the main frame
	SetWindowName("ACF Viewer");
	MapSubwindows();
	Resize(GetDefaultSize());
	MapWindow();

}//close constructor


ACFImageViewer::~ACFImageViewer(){

	// Clean up used widgets: frames, buttons, layouthints
	

}//close destructor

void ACFImageViewer::Init(){

	
}//close Init()

void ACFImageViewer::ReadData(){


	//## Get data to read from file
	TFile* inputFile= new TFile(fInputFileName.c_str(),"READ");
	if(!inputFile || inputFile->IsZombie()){
		cerr<<"POCAImageViewer::ReadData(): ERROR: Cannot open file "<<fInputFileName.c_str()<<" to read...exit!"<<endl;
		gApplication->Terminate();
	}

	ACFGraph_LS= (TGraph*)inputFile->Get("ACFGraph_LS");
	ACFGraph_LS->SetLineColor(kBlack);
		
	ACFGraph_H= (TGraph*)inputFile->Get("ACFGraph_H");
	ACFGraph_H->SetLineColor(kBlue+1);
		
	ACFGraph_DP= (TGraph*)inputFile->Get("ACFGraph_DP");
	ACFGraph_DP->SetLineColor(kRed);
		
	ACFGraph_PH= (TGraph*)inputFile->Get("ACFGraph_PH");
	ACFGraph_PH->SetLineColor(kGreen+1);

	ACFWeightedGraph_LS= (TGraph*)inputFile->Get("ACFWeightedGraph_LS");
	ACFWeightedGraph_LS->SetLineColor(kBlack);
	
	ACFWeightedGraph_H= (TGraph*)inputFile->Get("ACFWeightedGraph_H");
	ACFWeightedGraph_H->SetLineColor(kBlue+1);
	
	ACFWeightedGraph_DP= (TGraph*)inputFile->Get("ACFWeightedGraph_DP");
	ACFWeightedGraph_DP->SetLineColor(kRed);
	
	ACFWeightedGraph_PH= (TGraph*)inputFile->Get("ACFWeightedGraph_PH");
	ACFWeightedGraph_PH->SetLineColor(kGreen+1);
	
}//close ReadData()

void ACFImageViewer::Draw(){
	
	
	TCanvas* canvas = fACFCanvas->GetCanvas();
	if(!canvas){
		cerr<<"ACFImageViewer::DrawXY(): ERROR: Cannot read current canvas...exit!";
		gApplication->Terminate();
	}
	canvas->cd();
	canvas->SetEditable(true);
	canvas->Clear();
		
	TH2D* PlotBkg1= new TH2D("PlotBkg1","",100,0,50,100,-2,30);
	PlotBkg1->SetXTitle("r [cm]");
	PlotBkg1->GetXaxis()->SetTitleSize(0.08);
	PlotBkg1->GetXaxis()->SetLabelSize(0.06);
	PlotBkg1->GetXaxis()->SetTitleOffset(0.7);	
	PlotBkg1->SetYTitle("#xi(r)");
	PlotBkg1->GetYaxis()->SetTitleSize(0.08);
	PlotBkg1->GetYaxis()->SetLabelSize(0.06);
	PlotBkg1->GetYaxis()->SetTitleOffset(0.8);
	
	TLegend* PlotLegend= new TLegend(0.5,0.7,0.6,0.9);
	PlotLegend->SetFillColor(0);
	PlotLegend->SetTextSize(0.045);
	PlotLegend->SetTextFont(52);
	PlotLegend->AddEntry(ACFGraph_LS,"Landy-Szalay","l");
	PlotLegend->AddEntry(ACFGraph_H,"Hamilton","l");
	PlotLegend->AddEntry(ACFGraph_PH,"Peebles-Hauser","l");
	PlotLegend->AddEntry(ACFGraph_DP,"Davis-Peebles","l");
	
	TLine* refLine= new TLine(0.1,0,49,0);
	refLine->SetLineStyle(kDashed);

	PlotBkg1->Draw();
	ACFGraph_LS->Draw("l");
	ACFGraph_H->Draw("l");
	ACFGraph_DP->Draw("l");
	ACFGraph_PH->Draw("l");
	refLine->Draw("same");
	PlotLegend->Draw("same");

	canvas->Modified();
  canvas->Update();
  

	TCanvas* canvas2= fACFWeightedCanvas->GetCanvas();
	if(!canvas2){
		cerr<<"ACFImageViewer::Draw(): ERROR: Cannot read current canvas...exit!";
		gApplication->Terminate();
	}
	canvas2->cd();
	canvas2->SetEditable(true);
	canvas2->Clear();

	TH2D* PlotBkg2= new TH2D("PlotBkg2","",100,0,50,100,-2,30);
	PlotBkg2->SetXTitle("r [cm]");
	PlotBkg2->GetXaxis()->SetTitleSize(0.08);
	PlotBkg2->GetXaxis()->SetLabelSize(0.06);
	PlotBkg2->GetXaxis()->SetTitleOffset(0.7);	
	PlotBkg2->SetYTitle("#xi_{w}(r)");
	PlotBkg2->GetYaxis()->SetTitleSize(0.08);
	PlotBkg2->GetYaxis()->SetLabelSize(0.06);
	PlotBkg2->GetYaxis()->SetTitleOffset(0.8);

	TLegend* PlotLegend2= new TLegend(0.5,0.7,0.6,0.9);
	PlotLegend2->SetFillColor(0);
	PlotLegend2->SetTextSize(0.045);
	PlotLegend2->SetTextFont(52);
	PlotLegend2->AddEntry(ACFWeightedGraph_LS,"Landy-Szalay","l");
	PlotLegend2->AddEntry(ACFWeightedGraph_H,"Hamilton","l");
	PlotLegend2->AddEntry(ACFWeightedGraph_PH,"Peebles-Hauser","l");
	PlotLegend2->AddEntry(ACFWeightedGraph_DP,"Davis-Peebles","l");
	

	PlotBkg2->Draw();	
	ACFWeightedGraph_LS->Draw("l");
	ACFWeightedGraph_H->Draw("l");
	ACFWeightedGraph_DP->Draw("l");
	ACFWeightedGraph_PH->Draw("l");
	refLine->Draw("same");
	PlotLegend->Draw("same");

}//close ACFImageViewer::Draw()


void ACFImageViewer::SetGraphicsStyle(){

	myStyle= new TStyle("myStyle","left centered");
	myStyle->SetCanvasDefH(700); 
  myStyle->SetCanvasDefW(700); 
	//myStyle->SetCanvasPreferGL(1);

	myStyle->SetFrameBorderMode(0);
	myStyle->SetCanvasBorderMode(0);
  myStyle->SetPadBorderMode(0);
  myStyle->SetPadColor(0);
  myStyle->SetCanvasColor(kWhite);
  myStyle->SetTitleFillColor(0);
  myStyle->SetTitleBorderSize(1);
  myStyle->SetStatColor(0);
  myStyle->SetStatBorderSize(1);
  //myStyle->SetOptTitle(0);
  myStyle->SetOptStat(0);
  myStyle->SetOptFit(0);
  //myStyle->SetPalette(90,0);
  myStyle->SetTitleBorderSize(0);//border size of Title PavelLabel
  myStyle->SetTitleX(0.1f);
	myStyle->SetTitleW(0.8f);
  myStyle->SetStatY(0.975);                
  myStyle->SetStatX(0.95);                
  myStyle->SetStatW(0.2);                
  myStyle->SetStatH(0.15);                
  myStyle->SetTitleXOffset(1);
  myStyle->SetTitleYOffset(1);
  //myStyle->SetMarkerStyle(20);
  //myStyle->SetMarkerSize(0.5);
  myStyle->SetFuncWidth(1.);
  myStyle->SetPadTopMargin(0.08);
  myStyle->SetPadBottomMargin(0.17);
  //myStyle->SetPadLeftMargin(0.15);
  //myStyle->SetPadRightMargin(0.15);
	myStyle->SetPadLeftMargin(0.15);
  myStyle->SetPadRightMargin(0.17);	
  myStyle->SetTitleSize(0.06,"X");
  myStyle->SetTitleSize(0.06,"Y");
	myStyle->SetTitleFont(52,"X");
	myStyle->SetTitleFont(52,"Y");
	myStyle->SetTitleFont(52,"Z");
	myStyle->SetTitleFont(42,"X");
	myStyle->SetTitleFont(42,"Y");
	myStyle->SetTitleFont(42,"Z");
  myStyle->SetErrorX(0.);
	myStyle->cd();

	//gStyle->SetPalette(1);
	//gStyle->SetOptLogz(1);
	gStyle->SetTitleFont(52,"X");
	gStyle->SetTitleFont(52,"Y");
	gStyle->SetTitleFont(52,"Z");
	gStyle->SetTitleFont(42,"X");
	gStyle->SetTitleFont(42,"Y");
	gStyle->SetTitleFont(42,"Z");
	//gStyle->SetHistTopMargin(0);

	const int ncolors= 2000;	
	const int Number= 12;
	int myTemperaturePalette[ncolors];
  double r[]    = {0.164, 0.150, 0.250, 0.450, 0.670, 0.880, 1.000, 1.000,1.000,0.970,0.850,0.650};
  double g[]    = {0.043, 0.306, 0.630, 0.853, 0.973, 1.000,1.000,0.880,0.679,0.430,0.150,0.000};
  double b[]    = {0.850,1.000,1.000,1.000,1.000,1.000,0.750,0.600,0.450,0.370,0.196,0.130};
  double stop[] = {0.,0.2,0.3,0.4,0.5,0.7,0.75,0.8,0.85,0.9,0.95,1};
  int FI = TColor::CreateGradientColorTable(Number, stop, r, g, b, ncolors);
  for (int i=0;i<ncolors;i++) {
		myTemperaturePalette[i] = FI+i;
	}

	myStyle->SetPalette(ncolors,myTemperaturePalette);

	
}//close function

}//close namespace
