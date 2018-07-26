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
* @file POCAImageViewer.cc
* @class POCAImageViewer
* @brief POCA ROOT image viewer
*
* @author S. Riggi
* @date 23/03/2012
*/


#include <POCAImageViewer.h>

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

ClassImp(MuonPortalNS::POCAImageViewer)

namespace MuonPortalNS {

POCAImageViewer::POCAImageViewer(const TGWindow *p,UInt_t w,UInt_t h,std::string filename)
	: TGMainFrame(p,w,h){

	fInputFileName= filename;
	isFileReadCall= false;
	currentImageXY= 0;
	currentImageXZ= 0;
	currentImageYZ= 0;
	currentImage3D= 0;
	currentHistoXY= 0;
	currentHistoXZ= 0;
	currentHistoYZ= 0;
	currentPalette= 0;

	SetGraphicsStyle();
	
	
	//## Define styles
	const double width = w;
  const double height = h;
  const double kCanvasHeight = 0.38;

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
	double mainFrameWidth= 1000;
	double mainFrameHeight= 700;
	double tomographyFrameWidth= mainFrameWidth;
	double tomographyFrameHeight= 300;
	double tomographySubFrameWidth= 400;
	double tomographySubFrameHeight= tomographyFrameHeight;

	double tomography3DFrameWidth= mainFrameWidth;
	double tomography3DFrameHeight= 300;
	double tomography3DSubFrameWidth= tomography3DFrameHeight;
	double tomography3DSubFrameHeight= tomography3DFrameHeight;

	double canvasWidth= 400;
	double canvasHeight= 250;

	//## Define main frames
	TGVerticalFrame* MainFrameContainer= new TGVerticalFrame(this,mainFrameWidth,mainFrameHeight);
	
	//tomography frame
	TGHorizontalFrame* TomographyFrameContainer = new TGHorizontalFrame(MainFrameContainer,tomographyFrameWidth,tomographyFrameHeight);
	TGVerticalFrame* xyTomographyFrame = new TGVerticalFrame(TomographyFrameContainer,tomographySubFrameWidth,tomographySubFrameHeight);
	TGVerticalFrame* xzTomographyFrame = new TGVerticalFrame(TomographyFrameContainer,tomographySubFrameWidth,tomographySubFrameHeight);
	TGVerticalFrame* yzTomographyFrame = new TGVerticalFrame(TomographyFrameContainer,tomographySubFrameWidth,tomographySubFrameHeight);
	TomographyFrameContainer->AddFrame(xyTomographyFrame, new TGLayoutHints(kLHintsTop,2,2,2,2));
	TomographyFrameContainer->AddFrame(xzTomographyFrame, new TGLayoutHints(kLHintsTop,2,2,2,2));
	TomographyFrameContainer->AddFrame(yzTomographyFrame, new TGLayoutHints(kLHintsTop,2,2,2,2));
	
	//3D frame
	TGHorizontalFrame* Tomography3DFrameContainer = new TGHorizontalFrame(MainFrameContainer,tomography3DFrameWidth,tomography3DFrameHeight);
	TGVerticalFrame* xyzTomographyFrame = new TGVerticalFrame(Tomography3DFrameContainer,tomography3DSubFrameWidth,tomography3DSubFrameHeight);
	TGVerticalFrame* volRendTomographyFrame = new TGVerticalFrame(Tomography3DFrameContainer,tomography3DSubFrameWidth,tomography3DSubFrameHeight);
	TGVerticalFrame* threatListFrame = new TGVerticalFrame(Tomography3DFrameContainer,tomography3DSubFrameWidth,tomography3DSubFrameHeight);
	Tomography3DFrameContainer->AddFrame(xyzTomographyFrame,new TGLayoutHints(kLHintsTop,2,2,2,2));
	Tomography3DFrameContainer->AddFrame(volRendTomographyFrame,new TGLayoutHints(kLHintsTop,2,2,2,2));
	Tomography3DFrameContainer->AddFrame(threatListFrame,new TGLayoutHints(kLHintsTop,2,2,2,2));

	//lower button frame
	TGHorizontalFrame* buttonStatusFrame= new TGHorizontalFrame(MainFrameContainer, mainFrameWidth,30);
	
	MainFrameContainer->AddFrame(TomographyFrameContainer,new TGLayoutHints(kLHintsTop | kLHintsCenterY | kLHintsCenterX,2,2,2,2));
	MainFrameContainer->AddFrame(Tomography3DFrameContainer,new TGLayoutHints(kLHintsTop | kLHintsCenterY | kLHintsCenterX,2,2,2,2));
	MainFrameContainer->AddFrame(buttonStatusFrame,new TGLayoutHints(kLHintsTop | kLHintsCenterY | kLHintsCenterX,2,2,2,2));
	AddFrame(MainFrameContainer,centerLayout);

	



	//#################################
	//##      TOMOGRAPHY FRAME
	//#################################

	//#######  XY #######
	//canvas
	fXYTomographyCanvas = new TRootEmbeddedCanvas("XY Tomography", xyTomographyFrame, canvasWidth, canvasHeight);
	xyTomographyFrame->AddFrame(fXYTomographyCanvas, new TGLayoutHints(kLHintsCenterY | kLHintsTop,2,2,2,2));

	//option frame
	TGVerticalFrame* xyOptionFrame = new TGVerticalFrame(xyTomographyFrame,400,120,kFixedWidth);
	TGGroupFrame* optionButtonXY= new TGGroupFrame(xyOptionFrame,"XY Tomography Options",kVerticalFrame);
	optionButtonXY->SetWidth(300);
	optionButtonXY->SetHeight(120);
  xyOptionFrame->AddFrame(optionButtonXY, new TGLayoutHints(kLHintsCenterY | kLHintsCenterX | kLHintsTop,2,2,2,2));
	xyTomographyFrame->AddFrame(xyOptionFrame,new TGLayoutHints(kLHintsCenterY | kLHintsCenterX | kLHintsTop,2,2,2,2));

	//slider frame group
	TGHorizontalFrame* xyTomographySliderFrame= new TGHorizontalFrame(optionButtonXY, 300,60,kFixedWidth);
	optionButtonXY->AddFrame(xyTomographySliderFrame,new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	//slider label		
	TGLabel* sliderXYLabel = new TGLabel(xyTomographySliderFrame, "Y [cm]");
	sliderXYLabel->SetWidth(5);
	xyTomographySliderFrame->AddFrame(sliderXYLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	
	//slider	
	fSliderXY= new TGHSlider(xyTomographySliderFrame,100, kSlider2 | kScaleBoth);
	fSliderXY->Connect("PositionChanged(Int_t)","POCAImageViewer",this,"SlideXY(Int_t)");
	xyTomographySliderFrame->AddFrame(fSliderXY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	
	//slider text entry		
	fSliderTextBufferXY= new TGTextBuffer(40);	
	fSliderTextXY = new TGTextEntry(xyTomographySliderFrame, fSliderTextBufferXY);
	fSliderTextXY->SetWidth(40);
	fSliderTextXY->SetToolTipText("Y position [cm]");
	xyTomographySliderFrame->AddFrame(fSliderTextXY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	
	//modify button
	TGTextButton* modifyXYTomographyButton = new TGTextButton(xyTomographySliderFrame,"&palette");
	modifyXYTomographyButton->SetWidth(10);
	modifyXYTomographyButton->Connect("Clicked()","POCAImageViewer",this,"ModifyXY()");
	xyTomographySliderFrame->AddFrame(modifyXYTomographyButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


	//threshold frame container
	TGHorizontalFrame* xyTomographyThresholdFrame= new TGHorizontalFrame(optionButtonXY, 300,60,kFixedWidth);
	optionButtonXY->AddFrame(xyTomographyThresholdFrame,new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	
	//spinbox Threshold
	TGLabel* fThresholdNumEntryLabelXY = new TGLabel(xyTomographyThresholdFrame, "Thr");
	fThresholdNumEntryLabelXY->SetWidth(20);
	fThresholdNumEntryLabelXY->SetHeight(20);
	xyTomographyThresholdFrame->AddFrame(fThresholdNumEntryLabelXY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	fThresholdNumEntryXY = new TGNumberEntry(xyTomographyThresholdFrame, 0, 9,999, TGNumberFormat::kNESReal,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0, 99999);
	fThresholdNumEntryXY->SetWidth(60);
	fThresholdNumEntryXY->SetHeight(20);
	fThresholdNumEntryXY->Connect("ValueSet(Long_t)", "POCAImageViewer", this, "ApplyThresholdXY()");
	(fThresholdNumEntryXY->GetNumberEntry())->Connect("ReturnPressed()", "POCAImageViewer", this,"ApplyThresholdXY()");
	xyTomographyThresholdFrame->AddFrame(fThresholdNumEntryXY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


	//spinbox NThreshold
	TGLabel* fEventThresholdNumEntryLabelXY = new TGLabel(xyTomographyThresholdFrame, "NThr");
	fEventThresholdNumEntryLabelXY->SetWidth(20);
	xyTomographyThresholdFrame->AddFrame(fEventThresholdNumEntryLabelXY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	fEventThresholdNumEntryXY = new TGNumberEntry(xyTomographyThresholdFrame, 0, 9,999, TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0, 99999);
	fEventThresholdNumEntryXY->SetWidth(60);
	fEventThresholdNumEntryXY->Connect("ValueSet(Long_t)", "POCAImageViewer", this, "ApplyThresholdXY()");
	(fEventThresholdNumEntryXY->GetNumberEntry())->Connect("ReturnPressed()", "POCAImageViewer", this,"ApplyThresholdXY()");
	xyTomographyThresholdFrame->AddFrame(fEventThresholdNumEntryXY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	


	//#######  XZ #######
	//canvas
	fXZTomographyCanvas = new TRootEmbeddedCanvas("XZ Tomography", xzTomographyFrame, canvasWidth, canvasHeight);
	xzTomographyFrame->AddFrame(fXZTomographyCanvas, new TGLayoutHints(kLHintsCenterY | kLHintsTop,2,2,2,2));

	//option frame
	TGVerticalFrame* xzOptionFrame = new TGVerticalFrame(xzTomographyFrame,400,120,kFixedWidth);
	TGGroupFrame* optionButtonXZ= new TGGroupFrame(xzOptionFrame,"XZ Tomography Options",kVerticalFrame);
	optionButtonXZ->SetWidth(300);
	optionButtonXZ->SetHeight(120);
  xzOptionFrame->AddFrame(optionButtonXZ, new TGLayoutHints(kLHintsCenterY | kLHintsCenterX | kLHintsTop,2,2,2,2));
	xzTomographyFrame->AddFrame(xzOptionFrame,new TGLayoutHints(kLHintsCenterY | kLHintsCenterX | kLHintsTop,2,2,2,2));

	//slider frame group
	TGHorizontalFrame* xzTomographySliderFrame= new TGHorizontalFrame(optionButtonXZ, 300,60,kFixedWidth);
	optionButtonXZ->AddFrame(xzTomographySliderFrame,new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	//slider label		
	TGLabel* sliderXZLabel = new TGLabel(xzTomographySliderFrame, "Y [cm]");
	sliderXZLabel->SetWidth(5);
	xzTomographySliderFrame->AddFrame(sliderXZLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	
	//slider	
	fSliderXZ= new TGHSlider(xzTomographySliderFrame,100, kSlider2 | kScaleBoth);
	fSliderXZ->Connect("PositionChanged(Int_t)","POCAImageViewer",this,"SlideXZ(Int_t)");
	xzTomographySliderFrame->AddFrame(fSliderXZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	
	//slider text entry		
	fSliderTextBufferXZ= new TGTextBuffer(40);	
	fSliderTextXZ = new TGTextEntry(xzTomographySliderFrame, fSliderTextBufferXZ);
	fSliderTextXZ->SetWidth(40);
	fSliderTextXZ->SetToolTipText("Y position [cm]");
	xzTomographySliderFrame->AddFrame(fSliderTextXZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	
	//modify button
	TGTextButton* modifyXZTomographyButton = new TGTextButton(xzTomographySliderFrame,"&palette");
	modifyXZTomographyButton->SetWidth(10);
	modifyXZTomographyButton->Connect("Clicked()","POCAImageViewer",this,"ModifyXZ()");
	xzTomographySliderFrame->AddFrame(modifyXZTomographyButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


	//threshold frame container
	TGHorizontalFrame* xzTomographyThresholdFrame= new TGHorizontalFrame(optionButtonXZ, 300,60,kFixedWidth);
	optionButtonXZ->AddFrame(xzTomographyThresholdFrame,new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	
	//spinbox Threshold
	TGLabel* fThresholdNumEntryLabelXZ = new TGLabel(xzTomographyThresholdFrame, "Thr");
	fThresholdNumEntryLabelXZ->SetWidth(20);
	fThresholdNumEntryLabelXZ->SetHeight(20);
	xzTomographyThresholdFrame->AddFrame(fThresholdNumEntryLabelXZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	fThresholdNumEntryXZ = new TGNumberEntry(xzTomographyThresholdFrame, 0, 9,999, TGNumberFormat::kNESReal,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0, 99999);
	fThresholdNumEntryXZ->SetWidth(60);
	fThresholdNumEntryXZ->SetHeight(20);
	fThresholdNumEntryXZ->Connect("ValueSet(Long_t)", "POCAImageViewer", this, "ApplyThresholdXZ()");
	(fThresholdNumEntryXZ->GetNumberEntry())->Connect("ReturnPressed()", "POCAImageViewer", this,"ApplyThresholdXZ()");
	xzTomographyThresholdFrame->AddFrame(fThresholdNumEntryXZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


	//spinbox NThreshold
	TGLabel* fEventThresholdNumEntryLabelXZ = new TGLabel(xzTomographyThresholdFrame, "NThr");
	fEventThresholdNumEntryLabelXZ->SetWidth(20);
	xzTomographyThresholdFrame->AddFrame(fEventThresholdNumEntryLabelXZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	fEventThresholdNumEntryXZ = new TGNumberEntry(xzTomographyThresholdFrame, 0, 9,999, TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0, 99999);
	fEventThresholdNumEntryXZ->SetWidth(60);
	fEventThresholdNumEntryXZ->Connect("ValueSet(Long_t)", "POCAImageViewer", this, "ApplyThresholdXZ()");
	(fEventThresholdNumEntryXZ->GetNumberEntry())->Connect("ReturnPressed()", "POCAImageViewer", this,"ApplyThresholdXZ()");
	xzTomographyThresholdFrame->AddFrame(fEventThresholdNumEntryXZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	
	
	//#######  YZ #######
	//canvas
	fYZTomographyCanvas = new TRootEmbeddedCanvas("YZ Tomography", yzTomographyFrame, canvasWidth, canvasHeight);
	yzTomographyFrame->AddFrame(fYZTomographyCanvas, new TGLayoutHints(kLHintsCenterY | kLHintsTop,2,2,2,2));

	//option frame
	TGVerticalFrame* yzOptionFrame = new TGVerticalFrame(yzTomographyFrame,400,120,kFixedWidth);
	TGGroupFrame* optionButtonYZ= new TGGroupFrame(yzOptionFrame,"YZ Tomography Options",kVerticalFrame);
	optionButtonYZ->SetWidth(300);
	optionButtonYZ->SetHeight(120);
  yzOptionFrame->AddFrame(optionButtonYZ, new TGLayoutHints(kLHintsCenterY | kLHintsCenterX | kLHintsTop,2,2,2,2));
	yzTomographyFrame->AddFrame(yzOptionFrame,new TGLayoutHints(kLHintsCenterY | kLHintsCenterX | kLHintsTop,2,2,2,2));

	//slider frame group
	TGHorizontalFrame* yzTomographySliderFrame= new TGHorizontalFrame(optionButtonYZ, 300,60,kFixedWidth);
	optionButtonYZ->AddFrame(yzTomographySliderFrame,new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	//slider label		
	TGLabel* sliderYZLabel = new TGLabel(yzTomographySliderFrame, "Y [cm]");
	sliderYZLabel->SetWidth(5);
	yzTomographySliderFrame->AddFrame(sliderYZLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	
	//slider	
	fSliderYZ= new TGHSlider(yzTomographySliderFrame,100, kSlider2 | kScaleBoth);
	fSliderYZ->Connect("PositionChanged(Int_t)","POCAImageViewer",this,"SlideYZ(Int_t)");
	yzTomographySliderFrame->AddFrame(fSliderYZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	
	//slider text entry		
	fSliderTextBufferYZ= new TGTextBuffer(40);	
	fSliderTextYZ = new TGTextEntry(yzTomographySliderFrame, fSliderTextBufferYZ);
	fSliderTextYZ->SetWidth(40);
	fSliderTextYZ->SetToolTipText("Y position [cm]");
	yzTomographySliderFrame->AddFrame(fSliderTextYZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	
	//modify button
	TGTextButton* modifyYZTomographyButton = new TGTextButton(yzTomographySliderFrame,"&palette");
	modifyYZTomographyButton->SetWidth(10);
	modifyYZTomographyButton->Connect("Clicked()","POCAImageViewer",this,"ModifyYZ()");
	yzTomographySliderFrame->AddFrame(modifyYZTomographyButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


	//threshold frame container
	TGHorizontalFrame* yzTomographyThresholdFrame= new TGHorizontalFrame(optionButtonYZ, 300,60,kFixedWidth);
	optionButtonYZ->AddFrame(yzTomographyThresholdFrame,new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	
	//spinbox Threshold
	TGLabel* fThresholdNumEntryLabelYZ = new TGLabel(yzTomographyThresholdFrame, "Thr");
	fThresholdNumEntryLabelYZ->SetWidth(20);
	fThresholdNumEntryLabelYZ->SetHeight(20);
	yzTomographyThresholdFrame->AddFrame(fThresholdNumEntryLabelYZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	fThresholdNumEntryYZ = new TGNumberEntry(yzTomographyThresholdFrame, 0, 9,999, TGNumberFormat::kNESReal,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0, 99999);
	fThresholdNumEntryYZ->SetWidth(60);
	fThresholdNumEntryYZ->SetHeight(20);
	fThresholdNumEntryYZ->Connect("ValueSet(Long_t)", "POCAImageViewer", this, "ApplyThresholdYZ()");
	(fThresholdNumEntryYZ->GetNumberEntry())->Connect("ReturnPressed()", "POCAImageViewer", this,"ApplyThresholdYZ()");
	yzTomographyThresholdFrame->AddFrame(fThresholdNumEntryYZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


	//spinbox NThreshold
	TGLabel* fEventThresholdNumEntryLabelYZ = new TGLabel(yzTomographyThresholdFrame, "NThr");
	fEventThresholdNumEntryLabelYZ->SetWidth(20);
	yzTomographyThresholdFrame->AddFrame(fEventThresholdNumEntryLabelYZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	fEventThresholdNumEntryYZ = new TGNumberEntry(yzTomographyThresholdFrame, 0, 9,999, TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0, 99999);
	fEventThresholdNumEntryYZ->SetWidth(60);
	fEventThresholdNumEntryYZ->Connect("ValueSet(Long_t)", "POCAImageViewer", this, "ApplyThresholdYZ()");
	(fEventThresholdNumEntryYZ->GetNumberEntry())->Connect("ReturnPressed()", "POCAImageViewer", this,"ApplyThresholdYZ()");
	yzTomographyThresholdFrame->AddFrame(fEventThresholdNumEntryYZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));







	//#################################
	//##      3D FRAME
	//#################################
	//###### XYZ tomography #######
	//canvas
	fXYZTomographyCanvas= new TRootEmbeddedCanvas("XYZ Tomography", xyzTomographyFrame,canvasWidth,canvasHeight);
	xyzTomographyFrame->AddFrame(fXYZTomographyCanvas, new TGLayoutHints(kLHintsCenterY | kLHintsTop,2,2,2,2));

	//slider frame group
	TGHorizontalFrame* xyzTomographySliderFrame= new TGHorizontalFrame(xyzTomographyFrame, 400,120,kFixedWidth);

	TGGroupFrame* sliderButtonXYZ= new TGGroupFrame(xyzTomographySliderFrame,"XYZ Tomography Options",kHorizontalFrame);
	xyzTomographySliderFrame->AddFrame(sliderButtonXYZ, new TGLayoutHints(kLHintsCenterX | kLHintsTop,2,2,2,2));


	
	//double slider label		
	TGLabel* doublesliderXYZLabel = new TGLabel(sliderButtonXYZ, "ThetaCut [deg]");
	doublesliderXYZLabel->SetWidth(10);
	sliderButtonXYZ->AddFrame(doublesliderXYZLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	fDoubleSliderXYZ = new TGDoubleHSlider(sliderButtonXYZ, 150, kDoubleScaleBoth, -1,kHorizontalFrame);
	//fDoubleSliderXYZ->Connect("PointerPositionChanged()", "POCAImageViewer",this,"DoubleSlideXYZ()");
  fDoubleSliderXYZ->Connect("PositionChanged()", "POCAImageViewer",this,"DoubleSlideXYZ()");
 	fDoubleSliderXYZ->SetRange(0,100);
	sliderButtonXYZ->AddFrame(fDoubleSliderXYZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	//double slider text entry		
	fDoubleSliderMinTextBufferXYZ= new TGTextBuffer(40);	
	fDoubleSliderMinTextXYZ = new TGTextEntry(sliderButtonXYZ, fDoubleSliderMinTextBufferXYZ);
	fDoubleSliderMinTextXYZ->Connect("TextChanged(char*)", "POCAImageViewer",this,"DoubleSlideXYZText(char*)");
	fDoubleSliderMinTextXYZ->SetWidth(40);
	fDoubleSliderMinTextXYZ->SetToolTipText("ThetaMin [deg]");
	sliderButtonXYZ->AddFrame(fDoubleSliderMinTextXYZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	fDoubleSliderMaxTextBufferXYZ= new TGTextBuffer(40);	
	fDoubleSliderMaxTextXYZ = new TGTextEntry(sliderButtonXYZ, fDoubleSliderMaxTextBufferXYZ);
	fDoubleSliderMaxTextXYZ->Connect("TextChanged(char*)", "POCAImageViewer",this,"DoubleSlideXYZText(char*)");
	fDoubleSliderMaxTextXYZ->SetWidth(40);
	fDoubleSliderMaxTextXYZ->SetToolTipText("ThetaMax [deg]");
	sliderButtonXYZ->AddFrame(fDoubleSliderMaxTextXYZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	xyzTomographyFrame->AddFrame(xyzTomographySliderFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	

	//#######  3D rendering #######
	//canvas
	f3DTomographyCanvas= new TRootEmbeddedCanvas("3D Tomography", volRendTomographyFrame,canvasWidth,canvasHeight);
	volRendTomographyFrame->AddFrame(f3DTomographyCanvas, new TGLayoutHints(kLHintsCenterY | kLHintsTop,2,2,2,2));
		
	//slider
	TGHorizontalFrame* volRendTomographySliderFrame= new TGHorizontalFrame(volRendTomographyFrame, 400,120,kFixedWidth);
	TGGroupFrame* sliderButton3D= new TGGroupFrame(volRendTomographySliderFrame,"3D Rendering",kHorizontalFrame);
  volRendTomographySliderFrame->AddFrame(sliderButton3D, new TGLayoutHints(kLHintsCenterX | kLHintsTop,2,2,2,2));
	
	fSlider3D= new TGHSlider(sliderButton3D,100,kSlider2| kScaleBoth);
	fSlider3D->Connect("PositionChanged(Int_t)","POCAImageViewer",this,"Slide3D(Int_t)");
	sliderButton3D->AddFrame(fSlider3D, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	

	//modify button
	TGTextButton* modify3DTomographyButton = new TGTextButton(sliderButton3D,"&palette");
	modify3DTomographyButton->SetWidth(20);
	modify3DTomographyButton->Connect("Clicked()","POCAImageViewer",this,"Modify3D()");
	sliderButton3D->AddFrame(modify3DTomographyButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	
	volRendTomographyFrame->AddFrame(volRendTomographySliderFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	
	//#######  THREAT LIST #######
	TGListBox* ThreatListBox = new TGListBox(threatListFrame);
	//ThreatListBox->SetWidth(200);
	//ThreatListBox->SetHeight(300);
	ThreatListBox->SetWidth(canvasWidth);
	ThreatListBox->SetHeight(325);
	char tmp[20];
	for (int i = 0; i < 1000; ++i)  {
     sprintf(tmp, "Entry %i", i);
     ThreatListBox->AddEntry(tmp, i);
  }
	//ThreatListBox->Resize(200,300);

  threatListFrame->AddFrame(ThreatListBox, new TGLayoutHints(kLHintsCenterY | kLHintsTop,2,2,2,2));
	
	//#################################
	//##      LOWER FRAME
	//#################################
	//exit button
	TGTextButton* exitButton = new TGTextButton(buttonStatusFrame,"&Exit ","gApplication->Terminate()");
	buttonStatusFrame->AddFrame(exitButton, centerLayout);

	

	
	
	//## Read data
	Init();
	ReadData();
	

	//## Set initial slider position
	fSliderXY->SetRange(0,fNImageXY-1);
	fSliderXY->SetPosition(fNImageXY/2);
	fSliderXZ->SetRange(0,fNImageXZ-1);
	fSliderXZ->SetPosition(fNImageXZ/2);
	fSliderYZ->SetRange(0,fNImageYZ-1);
	fSliderYZ->SetPosition(fNImageYZ/2);
	fDoubleSliderXYZ->SetRange(0,100);
	fDoubleSliderXYZ->SetPosition(0,100);
	fSlider3D->SetRange(0,fNImage3D-1);
	fSlider3D->SetPosition(0);

	
	fSliderTextBufferXY->Clear();
 	fSliderTextBufferXY->AddText(0,Form("z=%.1f cm",-240./2.+fNImageXY/2*10.));
 	fSliderTextXY->SetCursorPosition(fSliderTextXY->GetCursorPosition());
  fSliderTextXY->Deselect();
  gClient->NeedRedraw(fSliderTextXY);

	fSliderTextBufferXZ->Clear();
 	fSliderTextBufferXZ->AddText(0,Form("z=%.1f cm",-240./2.+fNImageXZ/2*10.));
 	fSliderTextXZ->SetCursorPosition(fSliderTextXZ->GetCursorPosition());
  fSliderTextXZ->Deselect();
  gClient->NeedRedraw(fSliderTextXZ);

	fSliderTextBufferYZ->Clear();
 	fSliderTextBufferYZ->AddText(0,Form("z=%.1f cm",-600./2.+fNImageYZ/2*10.));
 	fSliderTextYZ->SetCursorPosition(fSliderTextYZ->GetCursorPosition());
  fSliderTextYZ->Deselect();
  gClient->NeedRedraw(fSliderTextYZ);
	
	fDoubleSliderMinTextBufferXYZ->Clear();
 	fDoubleSliderMinTextBufferXYZ->AddText(0,Form("%.1f",0.));
 	fDoubleSliderMinTextXYZ->SetCursorPosition(fDoubleSliderMinTextXYZ->GetCursorPosition());
  fDoubleSliderMinTextXYZ->Deselect();
  gClient->NeedRedraw(fDoubleSliderMinTextXYZ);

	fDoubleSliderMaxTextBufferXYZ->Clear();
 	fDoubleSliderMaxTextBufferXYZ->AddText(0,Form("%.1f",100.));
 	fDoubleSliderMaxTextXYZ->SetCursorPosition(fDoubleSliderMaxTextXYZ->GetCursorPosition());
  fDoubleSliderMaxTextXYZ->Deselect();
  gClient->NeedRedraw(fDoubleSliderMaxTextXYZ);

	// Sets window name and shows the main frame
	SetWindowName("POCA Viewer");
	MapSubwindows();
	Resize(GetDefaultSize());
	MapWindow();

}//close constructor


POCAImageViewer::~POCAImageViewer(){

	// Clean up used widgets: frames, buttons, layouthints
	

}//close destructor

void POCAImageViewer::Init(){

	cout<<"POCAImageViewer::Init(): INFO: Initializing tmography section images..."<<endl;
	for(int i=0;i<MAXNIMAGES;i++){
		
		TString imageName= Form("POCAImageXY%d",i+1);
		imageXYList[i]= new TASImageNew(700,500);
		//imageXYList[i]= new MyTASImage(700,500);
		//imageXYList[i]= new MyImage(700,500);
		imageXYList[i]->SetNameTitle(imageName,imageName);
			
		imageName= Form("POCAImageXZ%d",i+1);
		imageXZList[i]= new TASImageNew(700,500);
		//imageXZList[i]= new MyTASImage(700,500);
		//imageXZList[i]= new MyImage(700,500);
		imageXZList[i]->SetNameTitle(imageName,imageName);
		
		imageName= Form("POCAImageYZ%d",i+1);
		imageYZList[i]= new TASImageNew(700,500);	
		//imageYZList[i]= new MyTASImage(700,500);
		//imageYZList[i]= new MyImage(700,500);
		imageYZList[i]->SetNameTitle(imageName,imageName);

		imageName= Form("POCAImage3D%d",i+1);
		image3DList[i]= new TASImageNew(700,500);
		//image3DList[i]= new MyTASImage(700,500);
		//image3DList[i]= new MyImage(700,500);
		image3DList[i]->SetNameTitle(imageName,imageName);
	}//end loop images

	cout<<"POCAImageViewer::Init(): INFO: done!"<<endl;
	
}//close Init()

void POCAImageViewer::ReadData(){

	isFileReadCall= true;
	fNImageXY= 0;
	fNImageXZ= 0;	
	fNImageYZ= 0;
	fNImage3D= 0;

	//## Read palette	
	cout<<"POCAImageViewer::ReadData(): INFO: Reading image palette file..."<<endl;
	TFile* paletteFile= new TFile("myPalette.pal.root","READ");
	currentPalette= (TImagePalette*)paletteFile->Get("TImagePalette");
	if(!currentPalette){
		cerr<<"POCAImageViewer::ReadData(): ERROR: Cannot open file "<<fInputFileName.c_str()<<" to read...exit!"<<endl;
		gApplication->Terminate();
	}
	

	//## Get data to read from file
	cout<<"POCAImageViewer::ReadData(): INFO: Reading data file..."<<endl;
	TFile* inputFile= new TFile(fInputFileName.c_str(),"READ");
	if(!inputFile || inputFile->IsZombie()){
		cerr<<"POCAImageViewer::ReadData(): ERROR: Cannot open file "<<fInputFileName.c_str()<<" to read...exit!"<<endl;
		gApplication->Terminate();
	}


	//## Get data tree
	double x,y,z;
	double theta;

	TTree* data= (TTree*)inputFile->Get("data");
	data->SetBranchAddress("POCAX",&x);
	data->SetBranchAddress("POCAY",&y);
	data->SetBranchAddress("POCAZ",&z);
	data->SetBranchAddress("ScatteringAngle",&theta);

	
	nPoints= 0;
	nSelPoints= 0;
	int nColors= myStyle->GetNumberOfColors();
	double minWeight= log10(pow(data->GetMinimum("ScatteringAngle"),2));
	double maxWeight= log10(pow(data->GetMaximum("ScatteringAngle"),2));
	colorPaletteHisto= new TH1D("colorPaletteHisto","colorPaletteHisto",nColors-1,minWeight,maxWeight);

	cout<<"POCAImageViewer::ReadData(): INFO: Reading " << data->GetEntries() << " events..."<<endl;

	for(int i=0;i<data->GetEntries();i++){
		data->GetEntry(i);	
		fDataPointList[i]= TVector3(x,-y,z); 
		fDataPointThetaList[i]= theta; 

		double w= log10(pow(theta,2));
		int paletteBin= colorPaletteHisto->FindBin(w)-1;
		int colorId= myStyle->GetColorPalette(paletteBin);
		TColor* color = gROOT->GetColor(colorId);
   	color->SetAlpha(0.01);
		//fDataGraphList[i]= new TPolyMarker3D(1);
		fDataGraphList[i].SetPoint(0,x,-y,z);
		fDataGraphList[i].SetMarkerColor(color->GetNumber());
		fDataGraphList[i].SetMarkerStyle(1);
		fDataGraphList[i].SetMarkerSize(1);
		nPoints++;
		nSelPoints++;
	}
		
	//## Get data histos & images	
	cout<<"POCAImageViewer::ReadData(): INFO: Reading histo maps..."<<endl;

	fRecMap3D= (TH3D*)inputFile->Get("RecMap3D");
	minSignal= fRecMap3D->GetMinimum(0);
	maxSignal= fRecMap3D->GetMaximum();

	for(int i=0;i<MAXNIMAGES;i++){
		
		TString histoName= Form("RecMap_XFix%d",i+1);
		TString histoName2= Form("NEventMap_XFix%d",i+1);
		
		if( (TH2D*)inputFile->Get(histoName) ) {
			histoYZList[i]= (TH2D*)inputFile->Get(histoName);
			eventhistoYZList[i]= (TH2D*)inputFile->Get(histoName2);
			
			double* binContentArray= (Double_t*)histoYZList[i]->GetArray();
			int N= (histoYZList[i]->GetNbinsX() + 2) * (histoYZList[i]->GetNbinsY() + 2);
			double binContentArrayYZ[N];
			for(int s=0;s<N;s++) {
				double thisBinContent= binContentArray[s];
				
				if(thisBinContent>0) binContentArrayYZ[s]= log10(thisBinContent);
				else binContentArrayYZ[s]= -999;	
			}	
			imageYZList[i]->SetImage( binContentArrayYZ, histoYZList[i]->GetNbinsX() + 2, histoYZList[i]->GetNbinsY() + 2, currentPalette,true,log10(minSignal),log10(maxSignal));	
			if(currentPalette) imageYZList[i]->SetPalette(currentPalette);
			imageYZList[i]->SetEditable(true);
			imageYZList[i]->SetConstRatio(false);
			fNImageYZ++;
		}

		histoName= Form("RecMap_YFix%d",i+1);
		histoName2= Form("NEventMap_YFix%d",i+1);
		
		if( (TH2D*)inputFile->Get(histoName) ) {
			histoXZList[i]= (TH2D*)inputFile->Get(histoName);
			eventhistoXZList[i]= (TH2D*)inputFile->Get(histoName2);
			
			double* binContentArray= (Double_t *)histoXZList[i]->GetArray();
			int N= (histoXZList[i]->GetNbinsX() + 2) * (histoXZList[i]->GetNbinsY() + 2);
			double binContentArrayXZ[N];
			for(int s=0;s<N;s++) {
				double thisBinContent= binContentArray[s];
				if(thisBinContent>0) binContentArrayXZ[s]= log10(thisBinContent);
				else binContentArrayXZ[s]= -999;	
			}
			imageXZList[i]->SetImage( binContentArrayXZ, histoXZList[i]->GetNbinsX() + 2, histoXZList[i]->GetNbinsY() + 2, currentPalette,true,log10(minSignal),log10(maxSignal));	
			if(currentPalette) imageXZList[i]->SetPalette(currentPalette);
			imageXZList[i]->SetEditable(true);
			imageXZList[i]->SetConstRatio(false);
			fNImageXZ++;
		}
		
		histoName= Form("RecMap_ZFix%d",i+1);
		histoName2= Form("NEventMap_ZFix%d",i+1);

		if( (TH2D*)inputFile->Get(histoName) ) {
			histoXYList[i]= (TH2D*)inputFile->Get(histoName);
			eventhistoXYList[i]= (TH2D*)inputFile->Get(histoName2);
			
			double* binContentArray= (Double_t*)histoXYList[i]->GetArray(); 
			int N= (histoXYList[i]->GetNbinsX() + 2) * (histoXYList[i]->GetNbinsY() + 2);		
			double binContentArrayXY[N];
			for(int s=0;s<N;s++) {
				double thisBinContent= binContentArray[s];
				if(thisBinContent>0) binContentArrayXY[s]= log10(thisBinContent);
				else binContentArrayXY[s]= -999;	
			}

			imageXYList[i]->SetImage( binContentArrayXY, histoXYList[i]->GetNbinsX() + 2, histoXYList[i]->GetNbinsY() + 2, currentPalette,true,log10(minSignal),log10(maxSignal));	
			if(currentPalette) imageXYList[i]->SetPalette(currentPalette);
			imageXYList[i]->SetEditable(true);
			imageXYList[i]->SetConstRatio(false);	
			fNImageXY++;
		}
		
	
		
		TString imageName= Form("VolRendImg%d",i+1);
		//if( (MyTASImage*)inputFile->Get(imageName) ){
		if( (TASImageNew*)inputFile->Get(imageName) ){
			cout<<"POCAImageViewer::ReadData(): INFO: Reading VolRendImg no. "<<i<<"..."<<endl;
			image3DList[i]= (TASImageNew*)inputFile->Get(imageName);
			//image3DList[i]= (MyTASImage*)inputFile->Get(imageName);
			image3DList[i]->SetPaletteEnabled(false);
			image3DList[i]->SetEditable(true);
			//image3DList[i]->Scale(700,500);
			image3DList[i]->SetConstRatio(false);
			fNImage3D++;
			cout<<"done!"<<endl;
		}
		
		
	}//end loop images

	
	if(fNImageXY<=0 || fNImageXZ<=0 || fNImageYZ<=0 || fNImage3D<=0){
		cerr<<"POCAImageViewer::ReadData(): ERROR: No images to draw...exit!";
		gApplication->Terminate();
	}
	else{
		cout<<"POCAImageViewer::ReadData(): INFO: (Nxy,Nxz,Nyz,N3D)=("<<fNImageXY<<","<<fNImageXZ<<","<<fNImageYZ<<","<<fNImage3D<<") images read..."<<endl;
		currentImageXY= imageXYList[fNImageXY/2];
		currentImageXZ= imageXZList[fNImageXZ/2];
		currentImageYZ= imageYZList[fNImageYZ/2];
		currentImage3D= image3DList[0];
		currentHistoXY= histoXYList[fNImageXY/2];
		currentHistoXZ= histoXZList[fNImageXZ/2];
		currentHistoYZ= histoYZList[fNImageYZ/2];

		if(!currentImageXY){
			cerr<<"POCAImageViewer::ReadData(): ERROR: Cannot read current image XY...exit!";
			gApplication->Terminate();
		}
		if(!currentImageXZ){
			cerr<<"POCAImageViewer::ReadData(): ERROR: Cannot read current image XZ...exit!";
			gApplication->Terminate();
		}
		if(!currentImageYZ){
			cerr<<"POCAImageViewer::ReadData(): ERROR: Cannot read current image YZ...exit!";
			gApplication->Terminate();
		}
		if(!currentImage3D){
			cerr<<"POCAImageViewer::ReadData(): ERROR: Cannot read current image YZ...exit!";
			gApplication->Terminate();
		}
		DrawXY();
		DrawXZ();
		DrawYZ();
		DrawXYZ();
		Draw3D();
	}
	cout<<"end read data"<<endl;
	

}//close ReadData()

void POCAImageViewer::DrawXY(){
	
	
	TCanvas* canvas = fXYTomographyCanvas->GetCanvas();
	if(!canvas){
		cerr<<"POCAImageViewer::DrawXY(): ERROR: Cannot read current canvas...exit!";
		gApplication->Terminate();
	}
	canvas->cd();
	canvas->SetEditable(true);
	canvas->Clear();
		
	TPad* pad = new TPad("pad","pad",0,0,1,1);
	pad->SetTitle("pad");
	pad->SetFillColor(0);
	pad->SetFillStyle(4000);
  pad->Draw();
  pad->cd();
	
	pad->SetBottomMargin(0.01);
	pad->SetTopMargin(0.01);	
	pad->SetLeftMargin(0.01);
	pad->SetRightMargin(0.1);

	currentImageXY->Draw("Z");
	
	
	canvas->Modified();
  canvas->Update();
  
}//close POCAImageViewer::DrawXY()

void POCAImageViewer::DrawXZ(){
	
	TCanvas* canvas = fXZTomographyCanvas->GetCanvas();
	if(!canvas){
		cerr<<"POCAImageViewer::DrawXZ(): ERROR: Cannot read current canvas...exit!";
		gApplication->Terminate();
	}
	canvas->cd();
	canvas->SetEditable(true);
	canvas->Clear();

	TPad* pad = new TPad("pad","pad",0,0,1,1);
	pad->SetTitle("pad");
	pad->SetFillColor(0);
	pad->SetFillStyle(4000);
  pad->Draw();
  pad->cd();
	
	pad->SetBottomMargin(0.01);
	pad->SetTopMargin(0.01);	
	pad->SetLeftMargin(0.01);
	pad->SetRightMargin(0.1);

	currentImageXZ->Draw("Z");	
	
	canvas->Modified();
  canvas->Update();
  //canvas->SetEditable(false);
	
}//close POCAImageViewer::DrawXY()
	
void POCAImageViewer::DrawYZ(){
	
	TCanvas* canvas = fYZTomographyCanvas->GetCanvas();
	if(!canvas){
		cerr<<"POCAImageViewer::DrawXZ(): ERROR: Cannot read current canvas...exit!";
		gApplication->Terminate();
	}
	canvas->cd();
	canvas->SetEditable(true);
	canvas->Clear();

	TPad* pad = new TPad("pad","pad",0,0,1,1);
	pad->SetTitle("pad");
	pad->SetFillColor(0);
	pad->SetFillStyle(4000);
  pad->Draw();
  pad->cd();
	
	pad->SetBottomMargin(0.01);
	pad->SetTopMargin(0.01);	
	pad->SetLeftMargin(0.01);
	pad->SetRightMargin(0.1);

	currentImageYZ->Draw("Z");	
	
	canvas->Modified();
  canvas->Update();
  //canvas->SetEditable(false);
	
}//close POCAImageViewer::DrawYZ()


void POCAImageViewer::Draw3D(){
	
	TCanvas* canvas = f3DTomographyCanvas->GetCanvas();
	if(!canvas){
		cerr<<"POCAImageViewer::Draw3D(): ERROR: Cannot read current canvas...exit!";
		gApplication->Terminate();
	}
	canvas->cd();
	canvas->SetEditable(true);	
	canvas->Clear();

	currentImage3D->Draw("X");	
	
	canvas->Modified();
  canvas->Update();
  //canvas->SetEditable(false);
	
}//close POCAImageViewer::Draw3D()


void POCAImageViewer::DrawXYZ(){

	TCanvas* canvas = fXYZTomographyCanvas->GetCanvas();
	if(!canvas){
		cerr<<"POCAImageViewer::DrawXYZ(): ERROR: Cannot read current canvas...exit!";
		gApplication->Terminate();
	}
	canvas->cd();
	canvas->SetEditable(true);
	canvas->Clear();
	canvas->SetFillColor(kBlack);
	
	TPad* pad = new TPad("pad","pad",0.0,0.,1,1);
	pad->SetTitle("pad");
	pad->SetFillColor(kBlack);
	pad->SetFillStyle(4000);
  pad->Draw();
  pad->cd();

	TView* view = TView3D::CreateView(1);
	view->SetRange(-200,-200,-200,200,200,200);
	
	
	for(int i=0;i<nSelPoints;i++){
		fDataGraphList[i].Draw("P same");
	}
	
	canvas->Modified();
  canvas->Update();
  //canvas->SetEditable(false);
	
}//close POCAImageViewer::DrawXYZ()


void POCAImageViewer::ModifyXY(){

	currentImageXY->StartPaletteEditor();
	
}//close POCAImageViewer::ModifyXY()


void POCAImageViewer::ModifyXZ(){

	currentImageXZ->StartPaletteEditor();
	
}//close POCAImageViewer::ModifyXZ()

void POCAImageViewer::ModifyYZ(){

	currentImageYZ->StartPaletteEditor();
	
}//close POCAImageViewer::ModifyYZ()

void POCAImageViewer::Modify3D(){

	currentImage3D->StartPaletteEditor();
	
}//close POCAImageViewer::Modify3D()


void POCAImageViewer::SlideXY(int pos){

	if(pos>=0 && pos<=fNImageXY-1){
		currentImageXY= imageXYList[pos];
		currentHistoXY= histoXYList[pos];

		fSliderTextBufferXY->Clear();
   	fSliderTextBufferXY->AddText(0,Form("z=%.1f cm",-240./2.+pos*10.));
  	fSliderTextXY->SetCursorPosition(fSliderTextXY->GetCursorPosition());
   	fSliderTextXY->Deselect();
   	gClient->NeedRedraw(fSliderTextXY);
		
		DrawXY();	
	}

}//close POCAImageViewer::SlideXY()

void POCAImageViewer::SlideXZ(int pos){

	if(pos>=0 && pos<=fNImageXZ-1){
		currentImageXZ= imageXZList[pos];
		
		fSliderTextBufferXZ->Clear();
   	fSliderTextBufferXZ->AddText(0,Form("z=%.1f cm",-240./2.+pos*10.));
  	fSliderTextXZ->SetCursorPosition(fSliderTextXZ->GetCursorPosition());
   	fSliderTextXZ->Deselect();
   	gClient->NeedRedraw(fSliderTextXZ);

		DrawXZ();	
	}

}//close POCAImageViewer::SlideXZ()

void POCAImageViewer::SlideYZ(int pos){

	if(pos>=0 && pos<=fNImageYZ-1){
		currentImageYZ= imageYZList[pos];

		fSliderTextBufferYZ->Clear();
   	fSliderTextBufferYZ->AddText(0,Form("z=%.1f cm",-600./2.+pos*10.));
  	fSliderTextYZ->SetCursorPosition(fSliderTextYZ->GetCursorPosition());
   	fSliderTextYZ->Deselect();
   	gClient->NeedRedraw(fSliderTextYZ);

		DrawYZ();	
	}

}//close POCAImageViewer::SlideYZ()




void POCAImageViewer::DoubleSlideXYZText(char*){

	fDoubleSliderXYZ->SetPosition(atof(fDoubleSliderMinTextBufferXYZ->GetString()),atof(fDoubleSliderMaxTextBufferXYZ->GetString()));
	//## Recalculate 3d image	
	nSelPoints= 0;	
	double thetaMin= fDoubleSliderXYZ->GetMinPosition();
	double thetaMax= fDoubleSliderXYZ->GetMaxPosition();
	cout<<"thetaMin="<<thetaMin<<"  thetaMax="<<thetaMax<<endl;
		
	for(int i=0;i<nPoints;i++){
			if(fDataPointThetaList[i]<thetaMin || fDataPointThetaList[i]>thetaMax) continue; 
			
			double x= fDataPointList[i].X();
			double y= fDataPointList[i].Y();
			double z= fDataPointList[i].Z();
			double theta= fDataPointThetaList[i];

			double w= log10(pow(theta,2));
			int paletteBin= colorPaletteHisto->FindBin(w)-1;
			int colorId= myStyle->GetColorPalette(paletteBin);
			TColor* color = gROOT->GetColor(colorId);
   		color->SetAlpha(0.01);
			fDataGraphList[nSelPoints].SetPoint(0,x,y,z);
			fDataGraphList[nSelPoints].SetMarkerColor(color->GetNumber());
			fDataGraphList[nSelPoints].SetMarkerStyle(1);
			fDataGraphList[nSelPoints].SetMarkerSize(1);
			nSelPoints++;
		}
		
		DrawXYZ();

}

void POCAImageViewer::DoubleSlideXYZ(){

	
	char buf[32];
	sprintf(buf, "%.3f", fDoubleSliderXYZ->GetMinPosition());
  fDoubleSliderMinTextBufferXYZ->Clear();
  fDoubleSliderMinTextBufferXYZ->AddText(0, buf);
  fDoubleSliderMinTextXYZ->SetCursorPosition(fDoubleSliderMinTextXYZ->GetCursorPosition());
  fDoubleSliderMinTextXYZ->Deselect();
  gClient->NeedRedraw(fDoubleSliderMinTextXYZ);

	char buf2[32];
	sprintf(buf2, "%.3f", fDoubleSliderXYZ->GetMaxPosition());
  fDoubleSliderMaxTextBufferXYZ->Clear();
  fDoubleSliderMaxTextBufferXYZ->AddText(0, buf2);
  fDoubleSliderMaxTextXYZ->SetCursorPosition(fDoubleSliderMaxTextXYZ->GetCursorPosition());
  fDoubleSliderMaxTextXYZ->Deselect();
  gClient->NeedRedraw(fDoubleSliderMaxTextXYZ);
	/*
	fDoubleSliderMinTextBufferXYZ->Clear();
  fDoubleSliderMinTextBufferXYZ->AddText(0,Form("%.1f",fDoubleSliderXYZ->GetMinPosition()));
  fDoubleSliderMinTextXYZ->SetCursorPosition(fDoubleSliderMinTextXYZ->GetCursorPosition());
  fDoubleSliderMinTextXYZ->Deselect();
  gClient->NeedRedraw(fDoubleSliderMinTextXYZ);

	fDoubleSliderMaxTextBufferXYZ->Clear();
  fDoubleSliderMaxTextBufferXYZ->AddText(0,Form("%.1f",fDoubleSliderXYZ->GetMaxPosition()));
  fDoubleSliderMaxTextXYZ->SetCursorPosition(fDoubleSliderMaxTextXYZ->GetCursorPosition());
  fDoubleSliderMaxTextXYZ->Deselect();
  gClient->NeedRedraw(fDoubleSliderMaxTextXYZ);
	*/
	//## Recalculate 3d image	
	nSelPoints= 0;	
	double thetaMin= fDoubleSliderXYZ->GetMinPosition();
	double thetaMax= fDoubleSliderXYZ->GetMaxPosition();
		
	for(int i=0;i<nPoints;i++){
			if(fDataPointThetaList[i]<thetaMin || fDataPointThetaList[i]>thetaMax) continue; 
			
			double x= fDataPointList[i].X();
			double y= fDataPointList[i].Y();
			double z= fDataPointList[i].Z();
			double theta= fDataPointThetaList[i];

			double w= log10(pow(theta,2));
			int paletteBin= colorPaletteHisto->FindBin(w)-1;
			int colorId= myStyle->GetColorPalette(paletteBin);
			TColor* color = gROOT->GetColor(colorId);
   		color->SetAlpha(0.01);
			fDataGraphList[nSelPoints].SetPoint(0,x,y,z);
			fDataGraphList[nSelPoints].SetMarkerColor(color->GetNumber());
			fDataGraphList[nSelPoints].SetMarkerStyle(1);
			fDataGraphList[nSelPoints].SetMarkerSize(1);
			nSelPoints++;
		}
		
		DrawXYZ();

}//close POCAImageViewer::DoubleSlideXYZ()

void POCAImageViewer::Slide3D(int pos){

	if(pos>=0 && pos<=fNImage3D-1){
		currentImage3D= image3DList[pos];
		Draw3D();	
	}

}//close POCAImageViewer::Slide3D()


void POCAImageViewer::ApplyThresholdXY(){

	double thresholdValue= fThresholdNumEntryXY->GetNumber();
	int eventthresholdValue= fEventThresholdNumEntryXY->GetNumber();
		
	//## Recompute image after applying the threshold cut
	for(int i=0;i<fNImageXY;i++){
		
		TString histoName= Form("histoXYList%d_copy",i+1);
		TH2D* histoXYList_copy= (TH2D*)histoXYList[i]->Clone(histoName);

		for(int ix=0;ix<histoXYList_copy->GetNbinsX();ix++){
			for(int iy=0;iy<histoXYList_copy->GetNbinsY();iy++){
				double binContent= histoXYList_copy->GetBinContent(ix+1,iy+1);	
				double nevent= eventhistoXYList[i]->GetBinContent(ix+1,iy+1);		
				if(binContent<thresholdValue || nevent<eventthresholdValue) histoXYList_copy->SetBinContent(ix+1,iy+1,0.);
			}//end loop bins X
		}//end loop bins X
		
		double* binContentArray= (Double_t*)histoXYList_copy->GetArray();
		int N= (histoXYList_copy->GetNbinsX() + 2) * (histoXYList_copy->GetNbinsY() + 2);	
		double binContentArrayXY[N];
		for(int s=0;s<N;s++) {
			double thisBinContent= binContentArray[s];
			if(thisBinContent>0) binContentArrayXY[s]= log10(thisBinContent);
			else binContentArrayXY[s]= -999;	
		}

		imageXYList[i]->SetImage( binContentArrayXY, histoXYList_copy->GetNbinsX() + 2, histoXYList_copy->GetNbinsY() + 2, currentPalette,true,log10(minSignal),log10(maxSignal));	
		imageXYList[i]->SetPalette(currentPalette);
		imageXYList[i]->SetEditable(true);
		imageXYList[i]->SetConstRatio(false);	
	}

	DrawXY();

}//close ApplyThresholdXY()



void POCAImageViewer::ApplyThresholdXZ(){

	double thresholdValue= fThresholdNumEntryXZ->GetNumber();
	int eventthresholdValue= fEventThresholdNumEntryXZ->GetNumber();
		
	//## Recompute image after applying the threshold cut
	for(int i=0;i<fNImageXZ;i++){
		
		TString histoName= Form("histoXZList%d_copy",i+1);
		TH2D* histoXZList_copy= (TH2D*)histoXZList[i]->Clone(histoName);

		for(int ix=0;ix<histoXZList_copy->GetNbinsX();ix++){
			for(int iy=0;iy<histoXZList_copy->GetNbinsY();iy++){
				double binContent= histoXZList_copy->GetBinContent(ix+1,iy+1);	
				double nevent= eventhistoXZList[i]->GetBinContent(ix+1,iy+1);		
				if(binContent<thresholdValue || nevent<eventthresholdValue) histoXZList_copy->SetBinContent(ix+1,iy+1,0.);
			}//end loop bins X
		}//end loop bins X
		
		double* binContentArray= (Double_t*)histoXZList_copy->GetArray();
		int N= (histoXZList_copy->GetNbinsX() + 2) * (histoXZList_copy->GetNbinsY() + 2);	
		double binContentArrayXZ[N];
		for(int s=0;s<N;s++) {
			double thisBinContent= binContentArray[s];
			if(thisBinContent>0) binContentArrayXZ[s]= log10(thisBinContent);
			else binContentArrayXZ[s]= -999;	
		}

		imageXZList[i]->SetImage( binContentArrayXZ, histoXZList_copy->GetNbinsX() + 2, histoXZList_copy->GetNbinsY() + 2, currentPalette,true,log10(minSignal),log10(maxSignal));	
		imageXZList[i]->SetPalette(currentPalette);
		imageXZList[i]->SetEditable(true);
		imageXZList[i]->SetConstRatio(false);	
	}

	DrawXZ();

}//close ApplyThresholdXZ()


void POCAImageViewer::ApplyThresholdYZ(){

	double thresholdValue= fThresholdNumEntryYZ->GetNumber();
	int eventthresholdValue= fEventThresholdNumEntryYZ->GetNumber();
		
	//## Recompute image after applying the threshold cut
	for(int i=0;i<fNImageYZ;i++){
		
		TString histoName= Form("histoYZList%d_copy",i+1);
		TH2D* histoYZList_copy= (TH2D*)histoYZList[i]->Clone(histoName);

		for(int ix=0;ix<histoYZList_copy->GetNbinsX();ix++){
			for(int iy=0;iy<histoYZList_copy->GetNbinsY();iy++){
				double binContent= histoYZList_copy->GetBinContent(ix+1,iy+1);	
				double nevent= eventhistoYZList[i]->GetBinContent(ix+1,iy+1);		
				if(binContent<thresholdValue || nevent<eventthresholdValue) histoYZList_copy->SetBinContent(ix+1,iy+1,0.);
			}//end loop bins X
		}//end loop bins X
		
		double* binContentArray= (Double_t*)histoYZList_copy->GetArray();
		int N= (histoYZList_copy->GetNbinsX() + 2) * (histoYZList_copy->GetNbinsY() + 2);	
		double binContentArrayYZ[N];
		for(int s=0;s<N;s++) {
			double thisBinContent= binContentArray[s];
			if(thisBinContent>0) binContentArrayYZ[s]= log10(thisBinContent);
			else binContentArrayYZ[s]= -999;	
		}

		imageYZList[i]->SetImage( binContentArrayYZ, histoYZList_copy->GetNbinsX() + 2, histoYZList_copy->GetNbinsY() + 2, currentPalette,true,log10(minSignal),log10(maxSignal));	
		imageYZList[i]->SetPalette(currentPalette);
		imageYZList[i]->SetEditable(true);
		imageYZList[i]->SetConstRatio(false);	
	}

	DrawYZ();

}//close ApplyThresholdYZ()

void POCAImageViewer::SetGraphicsStyle(){

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
