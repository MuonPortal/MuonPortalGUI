#include <ClusteringImageViewer.h>

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
#include <TAxis3D.h>

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

ClassImp(ClusteringImageViewer)

ClusteringImageViewer::ClusteringImageViewer(const TGWindow *p,UInt_t w,UInt_t h,std::string filename)
	: TGMainFrame(p,w,h){

	fInputFileName= filename;
	currentImage3D= 0;

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
	
	//3D frame
	TGHorizontalFrame* TomographyFrameContainer = new TGHorizontalFrame(MainFrameContainer,tomographyFrameWidth,tomographyFrameHeight);
	TGVerticalFrame* xyzTomographyFrame = new TGVerticalFrame(TomographyFrameContainer,tomographySubFrameWidth,tomographySubFrameHeight);
	TGVerticalFrame* xyzrendTomographyFrame = new TGVerticalFrame(TomographyFrameContainer,tomographySubFrameWidth,tomographySubFrameHeight);
	TomographyFrameContainer->AddFrame(xyzTomographyFrame,new TGLayoutHints(kLHintsTop,2,2,2,2));
	TomographyFrameContainer->AddFrame(xyzrendTomographyFrame,new TGLayoutHints(kLHintsTop,2,2,2,2));

	//lower button frame
	TGHorizontalFrame* buttonStatusFrame= new TGHorizontalFrame(MainFrameContainer, mainFrameWidth,30);
	
	MainFrameContainer->AddFrame(TomographyFrameContainer,new TGLayoutHints(kLHintsTop | kLHintsCenterY | kLHintsCenterX,2,2,2,2));
	MainFrameContainer->AddFrame(buttonStatusFrame,new TGLayoutHints(kLHintsTop | kLHintsCenterY | kLHintsCenterX,2,2,2,2));
	AddFrame(MainFrameContainer,centerLayout);

	//#################################
	//##      TOMOGRAPHY FRAME
	//#################################
	//canvas
	f3DTomographyCanvas = new TRootEmbeddedCanvas("Cluster 3D", xyzTomographyFrame, canvasWidth, canvasHeight);
	xyzTomographyFrame->AddFrame(f3DTomographyCanvas, new TGLayoutHints(kLHintsCenterY | kLHintsTop,2,2,2,2));

	//option frame
	TGVerticalFrame* OptionFrame = new TGVerticalFrame(xyzTomographyFrame,500,120,kFixedWidth);
	
	TGGroupFrame* optionButton= new TGGroupFrame(OptionFrame,"Options",kVerticalFrame);
	optionButton->SetWidth(400);
	optionButton->SetHeight(120);
  OptionFrame->AddFrame(optionButton, new TGLayoutHints(kLHintsCenterY | kLHintsCenterX | kLHintsTop,2,2,2,2));
	xyzTomographyFrame->AddFrame(OptionFrame,new TGLayoutHints(kLHintsCenterY | kLHintsCenterX | kLHintsTop,2,2,2,2));


	//slider frame group
	TGHorizontalFrame* sliderFrame= new TGHorizontalFrame(optionButton, 400,60,kFixedWidth);
	optionButton->AddFrame(sliderFrame,new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	//slider label		
	TGLabel* SliderThetaRangeLabel = new TGLabel(sliderFrame, "ThetaCut [deg]");
	SliderThetaRangeLabel->SetWidth(10);
	sliderFrame->AddFrame(SliderThetaRangeLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	//slider
	fSliderThetaRange = new TGDoubleHSlider(sliderFrame, 150, kDoubleScaleBoth, -1,kHorizontalFrame);
	fSliderThetaRange->Connect("PositionChanged()", "ClusteringImageViewer",this,"SlideThetaRange()");
 	fSliderThetaRange->SetRange(0,100);
	sliderFrame->AddFrame(fSliderThetaRange, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	//slider text entry		
	fSliderMinThetaTextBuffer= new TGTextBuffer(40);	
	fSliderMinThetaText = new TGTextEntry(sliderFrame, fSliderMinThetaTextBuffer);
	fSliderMinThetaText->Connect("TextChanged(char*)", "ClusteringImageViewer",this,"SlideThetaRangeText(char*)");
	fSliderMinThetaText->SetWidth(40);
	fSliderMinThetaText->SetToolTipText("ThetaMin [deg]");
	sliderFrame->AddFrame(fSliderMinThetaText, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


	fSliderMaxThetaTextBuffer= new TGTextBuffer(40);	
	fSliderMaxThetaText = new TGTextEntry(sliderFrame, fSliderMaxThetaTextBuffer);
	fSliderMaxThetaText->Connect("TextChanged(char*)", "ClusteringImageViewer",this,"SlideThetaRangeText(char*)");
	fSliderMaxThetaText->SetWidth(40);
	fSliderMaxThetaText->SetToolTipText("ThetaMax [deg]");
	sliderFrame->AddFrame(fSliderMaxThetaText, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	//threshold frame container
	TGHorizontalFrame* thresholdFrame= new TGHorizontalFrame(optionButton, 300,60,kFixedWidth);
	optionButton->AddFrame(thresholdFrame,new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	
	//spinbox Threshold
	TGLabel* thresholdNumEntryLabel = new TGLabel(thresholdFrame, "Thr");
	thresholdNumEntryLabel->SetWidth(20);
	thresholdNumEntryLabel->SetHeight(20);
	thresholdFrame->AddFrame(thresholdNumEntryLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	fThresholdNumEntry = new TGNumberEntry(thresholdFrame, 0, 9,999, TGNumberFormat::kNESReal,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0, 99999);
	fThresholdNumEntry->SetWidth(60);
	fThresholdNumEntry->SetHeight(20);
	fThresholdNumEntry->Connect("ValueSet(Long_t)", "ClusteringImageViewer", this, "ApplyThreshold()");
	(fThresholdNumEntry->GetNumberEntry())->Connect("ReturnPressed()", "ClusteringImageViewer", this,"ApplyThreshold()");
	thresholdFrame->AddFrame(fThresholdNumEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


	//spinbox NThreshold
	TGLabel* eventThresholdNumEntryLabel = new TGLabel(thresholdFrame, "NThr");
	eventThresholdNumEntryLabel->SetWidth(20);
	thresholdFrame->AddFrame(eventThresholdNumEntryLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

	fEventThresholdNumEntry = new TGNumberEntry(thresholdFrame, 0, 9,999, TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0, 99999);
	fEventThresholdNumEntry->SetWidth(60);
	fEventThresholdNumEntry->Connect("ValueSet(Long_t)", "ClusteringImageViewer", this, "ApplyThreshold()");
	(fEventThresholdNumEntry->GetNumberEntry())->Connect("ReturnPressed()", "ClusteringImageViewer", this,"ApplyThreshold()");
	thresholdFrame->AddFrame(fEventThresholdNumEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));



	//#######  3D rendering #######
	//canvas
	f3DRendTomographyCanvas= new TRootEmbeddedCanvas("3D Tomography", xyzrendTomographyFrame,canvasWidth,canvasHeight);
	xyzrendTomographyFrame->AddFrame(f3DRendTomographyCanvas, new TGLayoutHints(kLHintsCenterY | kLHintsTop,2,2,2,2));
		
	//slider
	TGHorizontalFrame* volRendTomographySliderFrame= new TGHorizontalFrame(xyzrendTomographyFrame, 400,120,kFixedWidth);
	TGGroupFrame* sliderButton3D= new TGGroupFrame(volRendTomographySliderFrame,"3D Rendering",kHorizontalFrame);
  volRendTomographySliderFrame->AddFrame(sliderButton3D, new TGLayoutHints(kLHintsCenterX | kLHintsTop,2,2,2,2));
	
	fSlider3D= new TGHSlider(sliderButton3D,100,kSlider2| kScaleBoth);
	fSlider3D->Connect("PositionChanged(Int_t)","ClusteringImageViewer",this,"Slide3D(Int_t)");
	sliderButton3D->AddFrame(fSlider3D, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	

	//modify button
	TGTextButton* modify3DTomographyButton = new TGTextButton(sliderButton3D,"&palette");
	modify3DTomographyButton->SetWidth(20);
	modify3DTomographyButton->Connect("Clicked()","ClusteringImageViewer",this,"Modify3D()");
	sliderButton3D->AddFrame(modify3DTomographyButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	
	xyzrendTomographyFrame->AddFrame(volRendTomographySliderFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


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
	//Draw(fClusterPointGraphList);
	//Draw(fClusterPointGraphList,fNClusters);

	fSliderThetaRange->SetPosition(0,100);
	
	fSliderMinThetaTextBuffer->Clear();
 	fSliderMinThetaTextBuffer->AddText(0,Form("%.1f",0.));
 	fSliderMinThetaText->SetCursorPosition(fSliderThetaRange->GetMinPosition());
  fSliderMinThetaText->Deselect();
  gClient->NeedRedraw(fSliderMinThetaText);

	fSliderMaxThetaTextBuffer->Clear();
 	fSliderMaxThetaTextBuffer->AddText(0,Form("%.1f",100.));
 	fSliderMaxThetaText->SetCursorPosition(fSliderThetaRange->GetMaxPosition());
  fSliderMaxThetaText->Deselect();
  gClient->NeedRedraw(fSliderMaxThetaText);

	// Sets window name and shows the main frame
	SetWindowName("FOF Viewer");
	MapSubwindows();
	Resize(GetDefaultSize());
	MapWindow();

}//close constructor

ClusteringImageViewer::~ClusteringImageViewer(){

}//close destructor


void ClusteringImageViewer::Init(){

	fNClusterPoints.clear();
	fNClusterPoints.resize(0);

	fClusterWeight.clear();
	fClusterWeight.resize(0);

	for(int i=0;i<MAXNIMAGES;i++){
		TString imageName= Form("ClusteringImage3D%d",i+1);
		image3DList[i]= new TASImageNew(700,500);
		//image3DList[i]= new MyTASImage(700,500);
		image3DList[i]->SetNameTitle(imageName,imageName);
	}//end loop images

	
	geom = new TGeoManager("geom", "Simple geometry");
	matVacuum = new TGeoMaterial("Vacuum", 0,0,0);
	Vacuum = new TGeoMedium("Vacuum",1, matVacuum);
	top = geom->MakeBox("TOP", Vacuum, 600., 300., 300.);
	top->SetLineColor(kGray);
	top->SetVisibility(true);
	top->SetTransparency(0.5);

  geom->SetTopVolume(top);
	geom->CloseGeometry();
	geom->SetVisLevel(4);

}//close Init()

void ClusteringImageViewer::ReadData(){

	//## Get data to read from file
	TFile* inputFile= new TFile(fInputFileName.c_str(),"READ");
	if(!inputFile || inputFile->IsZombie()){
		cerr<<"POCAImageViewer::ReadData(): ERROR: Cannot open file "<<fInputFileName.c_str()<<" to read...exit!"<<endl;
		gApplication->Terminate();
	}
	
	int nClusters;	
	int ClusterId;
	int nClusterPoints;
	double x;
	double y;
	double z;
	double theta;
	
	TTree* info= (TTree*)inputFile->Get("Info");
	if(!info) info= (TTree*)inputFile->Get("InfoRun");
	info->SetBranchAddress("nClusters",&fNClusters);
	info->GetEntry(0);

	//## Allocate vectors
	for(int i=0;i<fNClusters;i++){
		fNClusterPoints.push_back(0);
		fClusterWeight.push_back(0);

		fClusterPointList.push_back( std::vector<TVector3>() );
		fClusterPointThetaList.push_back( std::vector<double>() );
		//fClusterPointGraphList.push_back( std::vector<TPolyMarker3D>() );
		//clusterPolyMarkerList.push_back(new TPolyMarker3D);
	}//end loop nClusters


	TTree* data= (TTree*)inputFile->Get("ClusterInfo");
	data->SetBranchAddress("ClusterId",&ClusterId);
	data->SetBranchAddress("X",&x);
	data->SetBranchAddress("Y",&y);
	data->SetBranchAddress("Z",&z);
	data->SetBranchAddress("Theta",&theta);
	
	int nEvents= data->GetEntries();

	nPoints= nEvents;
	nSelPoints= 0;
	int nColors= myStyle->GetNumberOfColors();
	minWeight= 1.e+99;
	maxWeight= -1.e+99;
	
	
	for(int i=0;i<nEvents;i++){
		data->GetEntry(i);

		if(ClusterId==1) cout<<"P("<<x<<","<<y<<","<<z<<")"<<endl;
		posList.push_back(TVector3(x,y,z));
		thetaList.push_back(theta);
		clusterIdList.push_back(ClusterId);

		
		fClusterWeight[ClusterId-1]+= pow(theta,2);
		fClusterPointList[ClusterId-1].push_back(TVector3(x,y,z));
		fClusterPointThetaList[ClusterId-1].push_back(theta);

		//TPolyMarker3D* aPoint= new TPolyMarker3D(1);
		//aPoint->SetPoint(0,x,y,z);
		//TPolyMarker3D aPoint;
		//aPoint.SetPoint(0,x,y,z);
		//fClusterPointGraphList[ClusterId-1].push_back(aPoint);	
		//fClusterPointGraphList[ClusterId-1].SetPoint(fNClusterPoints[ClusterId-1],x,y,z);
		//clusterPolyMarkerList[ClusterId-1]->SetPoint(fNClusterPoints[ClusterId-1],x,y,z);

		fClusterPointGraphList[i].SetPoint(0,x,y,z);
		fClusterPointGraphList_sel[i].SetPoint(0,x,y,z);
		
		nSelPoints++;
		fNClusterPoints[ClusterId-1]++;
	}//end loop events

	for(int i=0;i<fNClusters;i++) {
		//fClusterWeight[i]= log10(fClusterWeight[i]);
		fClusterWeight[i]/= (double)fNClusterPoints[i];
		if(fClusterWeight[i]<minWeight) minWeight= fClusterWeight[i];
		if(fClusterWeight[i]>maxWeight) maxWeight= fClusterWeight[i];
	}//end loop clusters
	cout<<"--> min/max cluster weight="<<minWeight<<"/"<<maxWeight<<"  logScale: "<<log10(minWeight)<<"/"<<log10(maxWeight)<<endl;

	colorPaletteHisto= new TH1D("colorPaletteHisto","colorPaletteHisto",nColors-1,minWeight,maxWeight);


	for(int i=0;i<nEvents;i++){
		int ClusterId= clusterIdList[i];
		double w= fClusterWeight[ClusterId-1];	
		int paletteBin= colorPaletteHisto->FindBin(w)-1;
		int colorId= myStyle->GetColorPalette(paletteBin);
		TColor* color = gROOT->GetColor(colorId);
   	color->SetAlpha(0.01);

		fClusterPointGraphList[i].SetMarkerColor(color->GetNumber());
		fClusterPointGraphList[i].SetMarkerStyle(1);
		fClusterPointGraphList[i].SetMarkerSize(1);
		
		fClusterPointGraphList_sel[i].SetMarkerColor(color->GetNumber());
		fClusterPointGraphList_sel[i].SetMarkerStyle(1);
		fClusterPointGraphList_sel[i].SetMarkerSize(1);
	}//end loop events

	//for(int i=0;i<fNClusters;i++) {
		//double w= fClusterWeight[i];
		//int paletteBin= colorPaletteHisto->FindBin(w)-1;
		//int colorId= myStyle->GetColorPalette(paletteBin);
		//TColor* color = gROOT->GetColor(colorId);
   	//color->SetAlpha(0.01);

		//fClusterPointGraphList[i].SetMarkerColor(color->GetNumber());
		//fClusterPointGraphList[i].SetMarkerStyle(1);
		//fClusterPointGraphList[i].SetMarkerSize(1);

		//clusterPolyMarkerList[i]->SetMarkerColor(color->GetNumber());
		//clusterPolyMarkerList[i]->SetMarkerStyle(1);
		//clusterPolyMarkerList[i]->SetMarkerSize(1);
		
		//for(int j=0;j<fClusterPointGraphList[i].size();j++) {	
			//fClusterPointGraphList[i][j]->SetMarkerColor(color->GetNumber());
			//fClusterPointGraphList[i][j]->SetMarkerStyle(1);
			//fClusterPointGraphList[i][j]->SetMarkerSize(1);

			//fClusterPointGraphList[i][j].SetMarkerColor(color->GetNumber());
			//fClusterPointGraphList[i][j].SetMarkerStyle(1);
			//fClusterPointGraphList[i][j].SetMarkerSize(1);
		//}//end loop points per cluster
		
	//}//end loop clusters

	
	cout<<"--> min/max cluster weight="<<minWeight<<"/"<<maxWeight<<endl;

	fNImage3D= 0;

	for(int i=0;i<MAXNIMAGES;i++){
		
		TString imageName= Form("PointImg%d",i+1);
		//if( (MyTASImage*)inputFile->Get(imageName) ){
		if( (TASImageNew*)inputFile->Get(imageName) ){
			//image3DList[i]= (MyTASImage*)inputFile->Get(imageName);
			image3DList[i]= (TASImageNew*)inputFile->Get(imageName);
			image3DList[i]->SetPaletteEnabled(false);
			image3DList[i]->SetEditable(true);
			//image3DList[i]->Scale(700,500);
			image3DList[i]->SetConstRatio(false);
			fNImage3D++;
		}
		
	}//end loop images	


	if(fNImage3D<=0){
		cerr<<"ClusteringImageViewer::ReadData(): ERROR: No images to draw...exit!";
		gApplication->Terminate();
	}
	else{
		cout<<"ClusteringImageViewer::ReadData(): INFO: (N3D)=("<<fNImage3D<<") images read..."<<endl;
		currentImage3D= image3DList[0];
	
		if(!currentImage3D){
			cerr<<"ClusteringImageViewer::ReadData(): ERROR: Cannot read current image YZ...exit!";
			gApplication->Terminate();
		}
		Draw3D();
	}
	
}//close ReadData()


void ClusteringImageViewer::Draw(){
//void ClusteringImageViewer::Draw(std::vector< std::vector<TPolyMarker3D*> > clusterPoints){
//void ClusteringImageViewer::Draw(std::vector< std::vector<TPolyMarker3D> > clusterPoints){
//void ClusteringImageViewer::Draw(TPolyMarker3D* clusterPoints,int nClusters){

	
	TCanvas* canvas = f3DTomographyCanvas->GetCanvas();
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
	
	top->Draw("ogle");
	
	
	for(int i=0;i<nSelPoints;i++) {	
		fClusterPointGraphList_sel[i].Draw("P same");
	}//end loop sel points
	

	TAxis3D::ToggleRulers(); // Brings the 3D axice up
	//TAxis3D::ToggleZoom();
	TAxis3D *axis = TAxis3D::GetPadAxis(); // Ask axis pointer
  if (axis) {
    //TAxis3D::ToggleRulers();    // To pop axice down
		axis->GetXaxis()->SetTitle("X");
		axis->GetYaxis()->SetTitle("Y");
		axis->GetZaxis()->SetTitle("Z");
		axis->SetXTitle("X");
		axis->SetYTitle("Y");	
		axis->SetZTitle("Z");

    //axis->SetLabelColor(kGray); // Paint the axice labels with blue color
    //axis->SetAxisColor(kGray);   // Paint the axice itself with blue color
    //TAxis3D::ToggleRulers();     // To pop axice up
  }
	
	canvas->Modified();
  canvas->Update();

}//close Draw()


void ClusteringImageViewer::Draw3D(){

	TCanvas* canvas = f3DRendTomographyCanvas->GetCanvas();
	if(!canvas){
		cerr<<"ClusteringImageViewer::Draw3D(): ERROR: Cannot read current canvas...exit!";
		gApplication->Terminate();
	}
	canvas->cd();
	canvas->SetEditable(true);	
	canvas->Clear();

	currentImage3D->Draw("X");	
	
	canvas->Modified();
  canvas->Update();

}//close Draw3D()


void ClusteringImageViewer::Modify3D(){

	currentImage3D->StartPaletteEditor();
	
}//close ClusteringImageViewer::Modify3D()

void ClusteringImageViewer::Slide3D(int pos){

	if(pos>=0 && pos<=fNImage3D-1){
		currentImage3D= image3DList[pos];
		Draw3D();	
	}

}//close ClusteringImageViewer::Slide3D()


void ClusteringImageViewer::SlideThetaRange(){
	

	
	char buf[32];
	sprintf(buf, "%.3f", fSliderThetaRange->GetMinPosition());
  fSliderMinThetaTextBuffer->Clear();
  fSliderMinThetaTextBuffer->AddText(0, buf);
  fSliderMinThetaText->SetCursorPosition(fSliderMinThetaText->GetCursorPosition());
  fSliderMinThetaText->Deselect();
  gClient->NeedRedraw(fSliderMinThetaText);

	char buf2[32];
	sprintf(buf2, "%.3f", fSliderThetaRange->GetMaxPosition());
  fSliderMaxThetaTextBuffer->Clear();
  fSliderMaxThetaTextBuffer->AddText(0, buf2);
  fSliderMaxThetaText->SetCursorPosition(fSliderMaxThetaText->GetCursorPosition());
  fSliderMaxThetaText->Deselect();
  gClient->NeedRedraw(fSliderMaxThetaText);
	

	double thetaMin= fSliderThetaRange->GetMinPosition();
	double thetaMax= fSliderThetaRange->GetMaxPosition();

	nSelPoints= 0;
	std::vector<int> nClusterPoints;
	std::vector<int> clusterWeight;
	std::vector<int> selclusterIdList;
	for(int i=0;i<fNClusters;i++){
		nClusterPoints.push_back(0);
		clusterWeight.push_back(0);
	}//end loop nClusters

	
	for(int i=0;i<nPoints;i++){
		TVector3 pos= posList[i];
		double theta= thetaList[i];
		int clusId= clusterIdList[i];

		if(theta<thetaMin || theta>thetaMax) continue;		

		clusterWeight[clusId-1]+= pow(theta,2);
		nClusterPoints[clusId-1]++;

		double oldx,oldy,oldz;
		fClusterPointGraphList[i].GetPoint(0,oldx,oldy,oldz);
		fClusterPointGraphList_sel[nSelPoints].SetPoint(0,oldx,oldy,oldz);
		selclusterIdList.push_back(clusId);

		nSelPoints++;
	}//end loop events

	
	for(int i=0;i<fNClusters;i++) {
		if(nClusterPoints[i]>0) clusterWeight[i]/= (double)nClusterPoints[i];
		else clusterWeight[i]= 0;
	}//end loop clusters

	
	for(int i=0;i<nSelPoints;i++){
		int clusterId= selclusterIdList[i];
		double w= clusterWeight[clusterId-1];
		int paletteBin= colorPaletteHisto->FindBin(w)-1;
		int colorId= myStyle->GetColorPalette(paletteBin);
		TColor* color = gROOT->GetColor(colorId);
   	color->SetAlpha(0.01);

		fClusterPointGraphList_sel[i].SetMarkerColor(color->GetNumber());
		fClusterPointGraphList_sel[i].SetMarkerStyle(1);
		fClusterPointGraphList_sel[i].SetMarkerSize(1);
	}//end loop sel points
	
	
	Draw();

}//close SlideThetaRange

void ClusteringImageViewer::SlideThetaRangeText(char*){

	fSliderThetaRange->SetPosition(atof(fSliderMinThetaTextBuffer->GetString()),atof(fSliderMaxThetaTextBuffer->GetString()));

	SlideThetaRange();
	
}//close SlideThetaRangeText

void ClusteringImageViewer::ApplyThreshold(){

	
	double thresholdValue= fThresholdNumEntry->GetNumber();
	int eventthresholdValue= fEventThresholdNumEntry->GetNumber();
	
	nSelPoints= 0;

	for(int i=0;i<nPoints;i++){
		int ClusterId= clusterIdList[i];
		double w= fClusterWeight[ClusterId-1];	
		int npts= fClusterPointList[ClusterId-1].size();	
		//cout<<"--> Cluster no. "<<ClusterId<<"  w="<<w<<"  npts="<<npts<<endl;

		if(w<thresholdValue || npts<eventthresholdValue) continue; 

		int paletteBin= colorPaletteHisto->FindBin(w)-1;
		int colorId= myStyle->GetColorPalette(paletteBin);
		TColor* color = gROOT->GetColor(colorId);
		color->SetAlpha(0.01);

		double oldx,oldy,oldz;
		fClusterPointGraphList[i].GetPoint(0,oldx,oldy,oldz);
		fClusterPointGraphList_sel[nSelPoints].SetPoint(0,oldx,oldy,oldz);
		fClusterPointGraphList_sel[nSelPoints].SetMarkerColor(color->GetNumber());
		fClusterPointGraphList_sel[nSelPoints].SetMarkerStyle(1);
		fClusterPointGraphList_sel[nSelPoints].SetMarkerSize(1);
		
		nSelPoints++;

	}//end loop events

	Draw();
	
}//close ApplyThreshold


void ClusteringImageViewer::SetGraphicsStyle(){

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

