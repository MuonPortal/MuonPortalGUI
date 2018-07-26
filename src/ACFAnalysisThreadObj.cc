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
* @file ACFAnalysisThreadObj.cc
* @class ACFAnalysisThreadObj
* @brief Autocorrelation algorithm processing thread
* 
* @author S. Riggi
* @date 25/04/2010
*/
#include <ACFAnalysisThreadObj.h>
#include <ConfigParser.h>

#include <Gui.h>
#include <Utilities.h>
#include <QtGui>
#include <QString>

#include <QUdpSocket>
#include <QDataStream>
#include <QHostAddress>
#include <QThread>
#include <QTcpSocket>

#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <qjson/qjson_export.h>
#include <qjson/qobjecthelper.h>
#include <qjson/serializerrunnable.h>
#include <qjson/parserrunnable.h>


#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1D.h>
#include <TF1.h>
#include <TH2.h>
#include <TH3.h>
#include <TLegend.h>
#include <TCut.h>
#include <TEventList.h>
#include <TMath.h>
#include <TPad.h>
#include <TVirtualPad.h>
#include <TCanvas.h>
#include <TStyle.h>

#include <TSystem.h>
#include <TGraphErrors.h>
#include <TMath.h>
#include <TDirectory.h>
#include <TStyle.h>
#include <TPolyLine3D.h>
#include <TPolyMarker3D.h>
#include <TPaveText.h>
#include <TVirtualFitter.h>
#include <TObjArray.h>

#include <TMatrixD.h>
#include <TColor.h>
#include <TApplication.h>
#include <TVector3.h>
#include <TView3D.h>
#include <TMarker.h>
#include <TPaletteAxis.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdexcept>


using namespace std;

namespace MuonPortalNS {

ACFAnalysisThreadObj::ACFAnalysisThreadObj() {

	//## Connect status signal to socket sender
	connect(this,SIGNAL(logSig(QString)), this, SLOT(sender(QString)) );
	connect(this,SIGNAL(alarmSig(QString)), this, SLOT(sender(QString)) );

	fInputFileName= "";
	fRandomDataFileName= "data/simdata_EmptyContainerScenario_last.out";
	fContainerXStart= -300.;
	fContainerXEnd= 300.;
	fContainerYStart= -120.;
	fContainerYEnd= 120.;
	fContainerZStart= -120.;
	fContainerZEnd= 120.;

	fVoxelSizeX= 20.;
	fVoxelSizeY= 20.;
	fVoxelSizeZ= 20.;
	fNVoxelX = (int)(fContainerSizeX/fVoxelSizeX); 
	fNVoxelY = (int)(fContainerSizeY/fVoxelSizeY);
	fNVoxelZ = (int)(fContainerSizeZ/fVoxelSizeZ);
	fNVoxel= fNVoxelX*fNVoxelY*fNVoxelZ;
	
	fNCrownX= 2;
	fNCrownY= 2;
	fNCrownZ= 2;
	fNTotBins= (fNVoxelX+2)*(fNVoxelY+2)*(fNVoxelZ+2);
	
	fRScaleMin= 0;
	fRScaleMax= 50;
	fRScaleStep= 1;

	fThetaMin= 2;
	fThetaMax= 100;
	fThetaWeight= 2;

}//close constructor
  
   
ACFAnalysisThreadObj::~ACFAnalysisThreadObj() {


}//close destructor


bool ACFAnalysisThreadObj::Init(){

	//## Set graphics style
	SetGraphicsStyle();

	//## Get config info
	fVerbosity= ConfigParser::fACFVerbosity;
	//fInputFileName= ConfigParser::fACFInputFileName;	
	fRandomDataFileName= ConfigParser::fACFInputRandomFileName;	
	//if(ConfigParser::fACFWaitPOCAOutput) fInputFileName= ConfigParser::fPOCAOutputFileName;	
	//fOutputFileName= ConfigParser::fACFOutputFileName;

	fUseMaxNEvents= ConfigParser::fUseACFMaxNEvents;	
	fMaxNEvents= ConfigParser::fACFMaxNEvents;
	fUseThetaCut= ConfigParser::fUseACFThetaCut;	
	fThetaMin= ConfigParser::fACFThetaMin;
	fThetaMax= ConfigParser::fACFThetaMax;
	fUseEnergyCut= ConfigParser::fUseACFEnergyCut;
	fEnergyMinCut= ConfigParser::fACFEnergyMin;
	fEnergyMaxCut= ConfigParser::fACFEnergyMax;
	fUseThetaWeight= ConfigParser::fUseACFThetaWeight;	
	fThetaWeight= ConfigParser::fACFThetaWeight;
	fSignalThreshold= ConfigParser::fACFSignalThreshold;
	fVoxelSizeX= ConfigParser::fACFVoxelSizeX;
	fVoxelSizeY= ConfigParser::fACFVoxelSizeY;
	fVoxelSizeZ= ConfigParser::fACFVoxelSizeZ;
	fNCrownX= ConfigParser::fACFNCrownX;
	fNCrownY= ConfigParser::fACFNCrownY;
	fNCrownZ= ConfigParser::fACFNCrownZ;

	fContainerSizeX= ConfigParser::fContainerSizeX;
	fContainerSizeY= ConfigParser::fContainerSizeY;
	fContainerSizeZ= ConfigParser::fContainerSizeZ;
	fContainerXStart= -fContainerSizeX/2;
	fContainerXEnd= fContainerXStart + fContainerSizeX;
	fContainerYStart= -fContainerSizeY/2;
	fContainerYEnd= fContainerYStart+ fContainerSizeY;
	fContainerZStart= -fContainerSizeZ/2;
	fContainerZEnd= fContainerZStart+ fContainerSizeZ;
	
	fNVoxelX = (int)(fContainerSizeX/fVoxelSizeX); 
	fNVoxelY = (int)(fContainerSizeY/fVoxelSizeY);
	fNVoxelZ = (int)(fContainerSizeZ/fVoxelSizeZ);
	fNVoxel= fNVoxelX*fNVoxelY*fNVoxelZ;
	fNTotBins= (fNVoxelX+2)*(fNVoxelY+2)*(fNVoxelZ+2);
	
	//## Open data files
	fInputFile= new TFile(fInputFileName.c_str(),"READ");	
	if(!fInputFile || fInputFile->IsZombie() ){
		cerr<<"ACFAnalysisThreadObj::Init(): ERROR: Cannot open input data file...exit!"<<endl;
		QString errmsg = QString("ACFAnalysisThreadObj::Init(): ERROR: Cannot open input random data file ");
		errmsg+= QString::fromStdString(fInputFileName);
		errmsg+= QString::fromStdString(std::string(" ... exit!"));
		emit(statusMessage(errmsg));
		emit error();

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= errmsg.toStdString();
		fLogMessage.progress= (int)(progressPercentage);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);

		return false;
	}

	fRandomDataFile= new TFile(fRandomDataFileName.c_str(),"READ");	
	if(!fRandomDataFile || fRandomDataFile->IsZombie() ){
		cerr<<"ACFAnalysisThreadObj::Init(): ERROR: Cannot open input random data file...exit!"<<endl;
		
		QString errmsg = QString("ACFAnalysisThreadObj::Init(): ERROR: Cannot open input random data file ");
		errmsg+= QString::fromStdString(fRandomDataFileName);
		errmsg+= QString::fromStdString(std::string(" ... exit!"));
		emit(statusMessage(errmsg));
		emit error();

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= errmsg.toStdString();
		fLogMessage.progress= (int)(progressPercentage);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);

		return false;
	}
	
	//## Define data histos and graphs
	fACFGraph_LS= new TGraph;
	fACFGraph_LS->SetName("ACFGraph_LS");
	fACFGraph_LS->SetMarkerColor(kBlack);
	fACFGraph_LS->SetMarkerStyle(21);
	fACFGraph_LS->SetMarkerSize(1.1);
	fACFGraph_LS->SetLineColor(kBlack);

	fACFGraph_H= new TGraph;
	fACFGraph_H->SetName("ACFGraph_H");
	fACFGraph_H->SetMarkerColor(kBlack);
	fACFGraph_H->SetMarkerStyle(21);
	fACFGraph_H->SetMarkerSize(1.1);
	fACFGraph_H->SetLineColor(kBlack);

	fACFGraph_DP= new TGraph;
	fACFGraph_DP->SetName("ACFGraph_DP");
	fACFGraph_DP->SetMarkerColor(kBlack);
	fACFGraph_DP->SetMarkerStyle(21);
	fACFGraph_DP->SetMarkerSize(1.1);
	fACFGraph_DP->SetLineColor(kBlack);

	fACFGraph_PH= new TGraph;
	fACFGraph_PH->SetName("ACFGraph_PH");
	fACFGraph_PH->SetMarkerColor(kBlack);
	fACFGraph_PH->SetMarkerStyle(21);
	fACFGraph_PH->SetMarkerSize(1.1);
	fACFGraph_PH->SetLineColor(kBlack);


	fACFWeightedGraph_LS= new TGraph;
	fACFWeightedGraph_LS->SetName("ACFWeightedGraph_LS");
	fACFWeightedGraph_LS->SetMarkerColor(kBlack);
	fACFWeightedGraph_LS->SetMarkerStyle(21);
	fACFWeightedGraph_LS->SetMarkerSize(1.1);
	fACFWeightedGraph_LS->SetLineColor(kBlack);

	fACFWeightedGraph_H= new TGraph;
	fACFWeightedGraph_H->SetName("ACFWeightedGraph_H");
	fACFWeightedGraph_H->SetMarkerColor(kBlack);
	fACFWeightedGraph_H->SetMarkerStyle(21);
	fACFWeightedGraph_H->SetMarkerSize(1.1);
	fACFWeightedGraph_H->SetLineColor(kBlack);

	fACFWeightedGraph_PH= new TGraph;
	fACFWeightedGraph_PH->SetName("ACFWeightedGraph_PH");
	fACFWeightedGraph_PH->SetMarkerColor(kBlack);
	fACFWeightedGraph_PH->SetMarkerStyle(21);
	fACFWeightedGraph_PH->SetMarkerSize(1.1);
	fACFWeightedGraph_PH->SetLineColor(kBlack);

	fACFWeightedGraph_DP= new TGraph;
	fACFWeightedGraph_DP->SetName("ACFWeightedGraph_DP");
	fACFWeightedGraph_DP->SetMarkerColor(kBlack);
	fACFWeightedGraph_DP->SetMarkerStyle(21);
	fACFWeightedGraph_DP->SetMarkerSize(1.1);
	fACFWeightedGraph_DP->SetLineColor(kBlack);


	int nBins= (int)((fRScaleMax-fRScaleMin)/fRScaleStep);
	
	fDDPairHisto= new TH1D("DDPairHisto","DDPairHisto",nBins,fRScaleMin,fRScaleMax);
	fDDPairHisto->Sumw2();

	fRRPairHisto= new TH1D("RRPairHisto","RRPairHisto",nBins,fRScaleMin,fRScaleMax);
	fRRPairHisto->Sumw2();
	
	fDRPairHisto= new TH1D("DRPairHisto","DRPairHisto",nBins,fRScaleMin,fRScaleMax);
	fDRPairHisto->Sumw2();

	fDDPairWeightedHisto= new TH1D("DDPairWeightedHisto","DDPairWeightedHisto",nBins,fRScaleMin,fRScaleMax);
	fDDPairWeightedHisto->Sumw2();

	fRRPairWeightedHisto= new TH1D("RRPairWeightedHisto","RRPairWeightedHisto",nBins,fRScaleMin,fRScaleMax);
	fRRPairWeightedHisto->Sumw2();
	
	fDRPairWeightedHisto= new TH1D("DRPairWeightedHisto","DRPairWeightedHisto",nBins,fRScaleMin,fRScaleMax);
	fDRPairWeightedHisto->Sumw2();

	fDataMap3D= new TH3D("DataMap3D","DataMap3D",fNVoxelX,fContainerXStart,fContainerXEnd,fNVoxelY,fContainerYStart,fContainerYEnd,fNVoxelZ,fContainerZStart,fContainerZEnd);
	fDataMap3D->Sumw2();

	fRandomDataMap3D= new TH3D("RandomDataMap3D","RandomDataMap3D",fNVoxelX,fContainerXStart,fContainerXEnd,fNVoxelY,fContainerYStart,fContainerYEnd,fNVoxelZ,fContainerZStart,fContainerZEnd);
	fRandomDataMap3D->Sumw2();

	fAlarmRScaleList.clear();	
	fAlarmRScaleList.resize(0);

	for(int i=0;i<fNTotBins;i++) {
		VoxelData voxeldata;
		(voxeldata.fPointList).clear();
		(voxeldata.fPointList).resize(0);
		(voxeldata.fThetaList).clear();
		(voxeldata.fThetaList).resize(0);
		fVoxelDataList.push_back(voxeldata);

		VoxelData voxelrandomdata;
		(voxelrandomdata.fPointList).clear();
		(voxelrandomdata.fPointList).resize(0);
		(voxelrandomdata.fThetaList).clear();
		(voxelrandomdata.fThetaList).resize(0);
		fVoxelRandomDataList.push_back(voxelrandomdata);
	}//end loop voxels
		
	return true;

}//close Init()


bool ACFAnalysisThreadObj::ReadData(){
		
	cout<<"ACFAnalysisThreadObj::ReadData(): INFO: Start reading data..."<<endl;

	double POCAX, POCAY, POCAZ, ScatteringAngle, KinEnergy;

	TTree* Data= (TTree*)fInputFile->Get("data");
	if(!Data || Data->IsZombie()){
		cerr<<"ACFAnalysisThreadObj::ReadData(): ERROR: Cannot open data tree...exit!"<<endl;
		QString errmsg = QString("ACFAnalysisThreadObj::ReadData(): ERROR: Cannot open data tree...exit!");
		emit(statusMessage(errmsg));
		emit error();

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= errmsg.toStdString();
		fLogMessage.progress= (int)(progressPercentage);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
		
		return false;
	}

	TTree* RandomData= (TTree*)fRandomDataFile->Get("data");
	if(!RandomData || RandomData->IsZombie()){
		cerr<<"ACFAnalysisThreadObj::ReadData(): ERROR: Cannot open random data tree...exit!"<<endl;
		QString errmsg = QString("ACFAnalysisThreadObj::ReadData(): ERROR: Cannot open random data tree...exit!");
		emit(statusMessage(errmsg));
		emit error();

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= errmsg.toStdString();
		fLogMessage.progress= (int)(progressPercentage);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);

		return false;
	}

	Data->SetBranchAddress("POCAX",&POCAX);
	Data->SetBranchAddress("POCAY",&POCAY);
	Data->SetBranchAddress("POCAZ",&POCAZ);
	Data->SetBranchAddress("ScatteringAngle",&ScatteringAngle);
	Data->SetBranchAddress("KinEnergy",&KinEnergy);

	RandomData->SetBranchAddress("POCAX",&POCAX);
	RandomData->SetBranchAddress("POCAY",&POCAY);
	RandomData->SetBranchAddress("POCAZ",&POCAZ);
	RandomData->SetBranchAddress("ScatteringAngle",&ScatteringAngle);
	RandomData->SetBranchAddress("KinEnergy",&KinEnergy);

	cout<<"ACFAnalysisThreadObj::ReadData(): INFO: Reading data..."<<endl;
	
	fNWeighted= 0;
	fN= 0;

	for(int i=0;i<Data->GetEntries();i++){
		Data->GetEntry(i);

		if(i%100000==0) cout<<"--> "<<i<<"/"<<Data->GetEntries()<<" entries read..."<<endl;
		
		if(fUseThetaCut && (ScatteringAngle<fThetaMin || ScatteringAngle>fThetaMax) ) continue;
		if(fUseEnergyCut && (KinEnergy<fEnergyMinCut || KinEnergy>fEnergyMaxCut) ) continue;
		if(POCAX<fContainerXStart || POCAX>fContainerXEnd) continue;
		if(POCAY<fContainerYStart || POCAY>fContainerYEnd) continue;
		if(POCAZ<fContainerZStart || POCAZ>fContainerZEnd) continue;
		
		fNWeighted+= pow(ScatteringAngle,fThetaWeight);
		fN++;

		int gBin= fDataMap3D->FindBin(POCAX,POCAY,POCAZ);
		TVector3 P= TVector3(POCAX,POCAY,POCAZ);
		(fVoxelDataList[gBin-1].fPointList).push_back( P );
		(fVoxelDataList[gBin-1].fThetaList).push_back( ScatteringAngle );

	}//end loop entries

	cout<<"--> NData="<<fN<<"  NDataWeighted="<<fNWeighted<<endl;
	

	cout<<"ACFAnalysisThreadObj::ReadData(): INFO: Reading random data..."<<endl;
	fNrWeighted= 0;
	fNr= 0;

	for(int i=0;i<RandomData->GetEntries();i++){
		RandomData->GetEntry(i);
		if(i%100000==0) cout<<"--> "<<i<<"/"<<RandomData->GetEntries()<<" entries read..."<<endl;

		if(fUseThetaCut && (ScatteringAngle<fThetaMin || ScatteringAngle>fThetaMax) ) continue;
		if(fUseEnergyCut && (KinEnergy<fEnergyMinCut || KinEnergy>fEnergyMaxCut) ) continue;
		if(POCAX<fContainerXStart || POCAX>fContainerXEnd) continue;
		if(POCAY<fContainerYStart || POCAY>fContainerYEnd) continue;
		if(POCAZ<fContainerZStart || POCAZ>fContainerZEnd) continue;
		
		fNrWeighted+= pow(ScatteringAngle,fThetaWeight);
		fNr++;

		int gBin= fRandomDataMap3D->FindBin(POCAX,POCAY,POCAZ);
		TVector3 P= TVector3(POCAX,POCAY,POCAZ);
		(fVoxelRandomDataList[gBin-1].fPointList).push_back( P );
		(fVoxelRandomDataList[gBin-1].fThetaList).push_back( ScatteringAngle );
	
	}//end loop entries

	cout<<"--> Nr="<<fNr<<"  NrWeighted="<<fNrWeighted<<endl;
		
	return true;

}//close ReadData()



bool ACFAnalysisThreadObj::ACFRun(){

	bool** IsPairCompared;
	IsPairCompared = new bool*[fNTotBins];
	for(int i=0; i<fNTotBins; i++){
		IsPairCompared[i]= new bool[fNTotBins];
	}

	for(int i=0;i<fNTotBins;i++) {
		for(int j=0;j<fNTotBins;j++) {
			IsPairCompared[i][j]= false;		
		}
	}
	

	
	//###############
	//## DD pairs
	//###############
	cout<<"PairFinder(): INFO: Calculating data-data pairs..."<<endl;
	
	for(int k=0;k<fDataMap3D->GetNbinsZ();k++){
		for(int j=0;j<fDataMap3D->GetNbinsY();j++){
			for(int i=0;i<fDataMap3D->GetNbinsX();i++){
					
				int xBinId= i+1;
				int yBinId= j+1;
				int zBinId= k+1;
				double xPos= fDataMap3D->GetXaxis()->GetBinCenter(xBinId);
				double yPos= fDataMap3D->GetYaxis()->GetBinCenter(yBinId);
				double zPos= fDataMap3D->GetZaxis()->GetBinCenter(zBinId);
				
				int gBinId= fDataMap3D->GetBin(xBinId,yBinId,zBinId);
		
				
				//## Start pair calculation inside the current voxel			
				std::vector<TVector3> PList= (fVoxelDataList[gBinId-1]).fPointList;
				std::vector<double> ThetaList= (fVoxelDataList[gBinId-1]).fThetaList;
				int NPoints= PList.size();		

				IsPairCompared[gBinId-1][gBinId-1]= 1;

				if(NPoints<=0) continue;
 
				//cout<<"CurrentVoxel "<<gBinId<<" ("<<xBinId<<","<<yBinId<<","<<zBinId<<")  Pos("<<xPos<<","<<yPos<<","<<zPos<<")  NPoints="<<NPoints<<endl;
				
				
				for(int s=0;s<NPoints-1;s++){
					
					TVector3 P1= PList[s];
					double Theta1= ThetaList[s];
					
					for(int t=s+1;t<NPoints;t++){
						
						TVector3 P2= PList[t];
						double Theta2= ThetaList[t];
						
						double d= (P1-P2).Mag();
						double thetaSum= pow(Theta1,fThetaWeight) + pow(Theta2,fThetaWeight);	
						double thetaSum2= Theta1 + Theta2;
						//cout<<"CurrentCurrentPairs: d="<<d<<"  thetaSum="<<thetaSum<<endl;

						fDDPairHisto->Fill(d);
						fDDPairWeightedHisto->Fill(d,thetaSum);
						
					}//end loop events in voxel
				}//end loop events in voxel
				

				
				//## Start pair calculation in neightbour voxels
				for(int iz=zBinId-fNCrownZ;iz<=zBinId+fNCrownZ;iz++){
					for(int iy=yBinId-fNCrownY;iy<=yBinId+fNCrownY;iy++){
						for(int ix=xBinId-fNCrownX;ix<=xBinId+fNCrownX;ix++){

							int gBinId_next= fDataMap3D->GetBin(ix,iy,iz);
							double xPos_next= fDataMap3D->GetXaxis()->GetBinCenter(ix);
							double yPos_next= fDataMap3D->GetYaxis()->GetBinCenter(iy);
							double zPos_next= fDataMap3D->GetZaxis()->GetBinCenter(iz);

							
							//cout<<"before isCompared bin="<<gBinId_next<<" ("<<ix<<","<<iy<<","<<iz<<")  Pos("<<xPos_next<<","<<yPos_next<<","<<zPos_next<<endl;
							if( fDataMap3D->IsBinOverflow(gBinId_next) || fDataMap3D->IsBinUnderflow(gBinId_next) ) {
								//cout<<"skip bin "<<gBinId_next<<" (overflow/underflow bin)..."<<endl;	
								continue;//not existing voxel bin...skip voxel!
							}		
							if(gBinId_next==gBinId) {
								//cout<<"skip bin "<<gBinId_next<<" (equal to previous bin)..."<<endl;
								continue;//center voxel, alredy scanned...skip voxel!
							}
							if(IsPairCompared[gBinId-1][gBinId_next-1]==1) {
								//cout<<"skip bin "<<gBinId_next<<" (already compared)..."<<endl;
								continue;//already compared...skip voxel!
							}						
							//cout<<"after isCompared"<<endl;

							//## Start pair calculation inside the next voxel
							IsPairCompared[gBinId-1][gBinId_next-1]= 1;	
							IsPairCompared[gBinId_next-1][gBinId-1]= 1;	
							
							std::vector<TVector3> PList_next= (fVoxelDataList[gBinId_next-1]).fPointList;
							std::vector<double> ThetaList_next= (fVoxelDataList[gBinId_next-1]).fThetaList;
							int NPoints_next= PList_next.size();
							if(NPoints_next<=0) continue;

							//cout<<"--> NextVoxel "<<gBinId_next<<" ("<<ix<<","<<iy<<","<<iz<<")  Pos("<<xPos_next<<","<<yPos_next<<","<<zPos_next<<")  NPoints="<<NPoints_next<<endl;

							//## Start pair calculation inside current-next voxel
							for(int s=0;s<NPoints;s++){
								TVector3 P1= PList[s];
								double Theta1= ThetaList[s];
								
								for(int t=0;t<NPoints_next;t++){	
									TVector3 P2_next= PList_next[t];
									double Theta2_next= ThetaList_next[t];
									
									double d= (P1-P2_next).Mag();
									double thetaSum= pow(Theta1,fThetaWeight)+pow(Theta2_next,fThetaWeight);
									double thetaSum2= Theta1 + Theta2_next;
									//cout<<"CurrentNextPairs: d="<<d<<"  thetaSum="<<thetaSum<<endl;

									fDDPairHisto->Fill(d);
									fDDPairWeightedHisto->Fill(d,thetaSum);
									
								}//end loop events in next voxel
							}//end loop events in next voxel


						}//end loop bins ix
					}//end loop bins iy
				}//end loop bins iz
				

			}//end loop bins x		
		}//end loop bins y
	}//end loop bins z
	


	
	//###############
	//## RR pairs
	//###############
	cout<<"PairFinder(): INFO: Calculating random-random pairs..."<<endl;
	for(int i=0;i<fNTotBins;i++) {
		for(int j=0;j<fNTotBins;j++) {
			IsPairCompared[i][j]= false;		
		}
	}

	for(int k=0;k<fRandomDataMap3D->GetNbinsZ();k++){
		for(int j=0;j<fRandomDataMap3D->GetNbinsY();j++){
			for(int i=0;i<fRandomDataMap3D->GetNbinsX();i++){
					
				int xBinId= i+1;
				int yBinId= j+1;
				int zBinId= k+1;
				double xPos= fRandomDataMap3D->GetXaxis()->GetBinCenter(xBinId);
				double yPos= fRandomDataMap3D->GetYaxis()->GetBinCenter(yBinId);
				double zPos= fRandomDataMap3D->GetZaxis()->GetBinCenter(zBinId);
				
				int gBinId= fRandomDataMap3D->GetBin(xBinId,yBinId,zBinId);
		
				IsPairCompared[gBinId-1][gBinId-1]= 1;
							
				//## Start pair calculation inside the current voxel				
				std::vector<TVector3> PList= (fVoxelRandomDataList[gBinId-1]).fPointList;
				std::vector<double> ThetaList= (fVoxelRandomDataList[gBinId-1]).fThetaList;
				int NPoints= PList.size();		
				if(NPoints<=0) continue;
 
				//cout<<"Voxel "<<gBinId<<" ("<<xBinId<<","<<yBinId<<","<<zBinId<<")  Pos("<<xPos<<","<<yPos<<","<<zPos<<")  NPoints="<<NPoints<<endl;
		
				for(int s=0;s<NPoints-1;s++){
					
					TVector3 P1= PList[s];
					double Theta1= ThetaList[s];
					
					for(int t=s+1;t<NPoints;t++){
						
						TVector3 P2= PList[t];
						double Theta2= ThetaList[t];
						
						double d= (P1-P2).Mag();
						double thetaSum= pow(Theta1,fThetaWeight)+pow(Theta2,fThetaWeight);
						double thetaSum2= Theta1 + Theta2;
						//cout<<"CurrentCurrentPairs: d="<<d<<"  thetaSum="<<thetaSum<<endl;

						fRRPairHisto->Fill(d);
						fRRPairWeightedHisto->Fill(d,thetaSum);
						
					}//end loop events in voxel
				}//end loop events in voxel

				
				//## Start pair calculation in neightbour voxels
				for(int iz=zBinId-fNCrownZ;iz<=zBinId+fNCrownZ;iz++){
					for(int iy=yBinId-fNCrownY;iy<=yBinId+fNCrownY;iy++){
						for(int ix=xBinId-fNCrownX;ix<=xBinId+fNCrownX;ix++){

							int gBinId_next= fRandomDataMap3D->GetBin(ix,iy,iz);
							double xPos_next= fRandomDataMap3D->GetXaxis()->GetBinCenter(ix);
							double yPos_next= fRandomDataMap3D->GetYaxis()->GetBinCenter(iy);
							double zPos_next= fRandomDataMap3D->GetZaxis()->GetBinCenter(iz);

							
							//cout<<"before isCompared bin="<<gBinId_next<<" ("<<ix<<","<<iy<<","<<iz<<")  Pos("<<xPos_next<<","<<yPos_next<<","<<zPos_next<<endl;
							if( fRandomDataMap3D->IsBinOverflow(gBinId_next) || fRandomDataMap3D->IsBinUnderflow(gBinId_next) ) {
								//cout<<"skip bin "<<gBinId_next<<" (overflow/underflow bin)..."<<endl;	
								continue;//not existing voxel bin...skip voxel!
							}		
							if(gBinId_next==gBinId) {
								//cout<<"skip bin "<<gBinId_next<<" (equal to previous bin)..."<<endl;
								continue;//center voxel, alredy scanned...skip voxel!
							}
							if(IsPairCompared[gBinId-1][gBinId_next-1]==1) {
								//cout<<"skip bin "<<gBinId_next<<" (already compared)..."<<endl;
								continue;//already compared...skip voxel!
							}						
							//cout<<"after isCompared"<<endl;

							IsPairCompared[gBinId-1][gBinId_next-1]= 1;	
							IsPairCompared[gBinId_next-1][gBinId-1]= 1;	

							std::vector<TVector3> PList_next= (fVoxelRandomDataList[gBinId_next-1]).fPointList;
							std::vector<double> ThetaList_next= (fVoxelRandomDataList[gBinId_next-1]).fThetaList;
							int NPoints_next= PList_next.size();
							if(NPoints_next<=0) continue;
							
							//cout<<"--> NextVoxel "<<gBinId_next<<" ("<<ix<<","<<iy<<","<<iz<<")  Pos("<<xPos_next<<","<<yPos_next<<","<<zPos_next<<")  NPoints="<<NPoints_next<<endl;

							//## Start pair calculation inside current-next voxel
							for(int s=0;s<NPoints;s++){
								TVector3 P1= PList[s];
								double Theta1= ThetaList[s];
								
								for(int t=0;t<NPoints_next;t++){	
									TVector3 P2_next= PList_next[t];
									double Theta2_next= ThetaList_next[t];
									
									double d= (P1-P2_next).Mag();
									double thetaSum= pow(Theta1,fThetaWeight)+pow(Theta2_next,fThetaWeight);
									double thetaSum2= Theta1 + Theta2_next;
									//cout<<"CurrentNextPairs: d="<<d<<"  thetaSum="<<thetaSum<<endl;

									fRRPairHisto->Fill(d);
									fRRPairWeightedHisto->Fill(d,thetaSum);
									
								}//end loop events in next voxel
							}//end loop events in next voxel


						}//end loop bins ix
					}//end loop bins iy
				}//end loop bins iz
				

			}//end loop bins x		
		}//end loop bins y
	}//end loop bins z
	


	
	//###############
	//## DR pairs
	//###############
	cout<<"PairFinder(): INFO: Calculating data-random pairs..."<<endl;
	for(int i=0;i<fNTotBins;i++) {
		for(int j=0;j<fNTotBins;j++) {
			IsPairCompared[i][j]= false;		
		}
	}

	for(int k=0;k<fDataMap3D->GetNbinsZ();k++){
		for(int j=0;j<fDataMap3D->GetNbinsY();j++){
			for(int i=0;i<fDataMap3D->GetNbinsX();i++){
					
				int xBinId= i+1;
				int yBinId= j+1;
				int zBinId= k+1;
				double xPos= fDataMap3D->GetXaxis()->GetBinCenter(xBinId);
				double yPos= fDataMap3D->GetYaxis()->GetBinCenter(yBinId);
				double zPos= fDataMap3D->GetZaxis()->GetBinCenter(zBinId);
				
				int gBinId= fDataMap3D->GetBin(xBinId,yBinId,zBinId);
		
				IsPairCompared[gBinId-1][gBinId-1]= 1;
				
				//## Start pair calculation inside the current voxel			
				std::vector<TVector3> PList= (fVoxelDataList[gBinId-1]).fPointList;
				std::vector<double> ThetaList= (fVoxelDataList[gBinId-1]).fThetaList;
				int NPoints= PList.size();		
				
				std::vector<TVector3> RandomPList= (fVoxelRandomDataList[gBinId-1]).fPointList;
				std::vector<double> RandomThetaList= (fVoxelRandomDataList[gBinId-1]).fThetaList;
				int NRandomPoints= RandomPList.size();		
				
				//cout<<"Current Voxel "<<gBinId<<" ("<<xBinId<<","<<yBinId<<","<<zBinId<<")  Pos("<<xPos<<","<<yPos<<","<<zPos<<")  NPoints="<<NPoints<<endl;
		
				for(int s=0;s<NPoints;s++){
					
					TVector3 P1= PList[s];
					double Theta1= ThetaList[s];
					
					for(int t=0;t<NRandomPoints;t++){
						
						TVector3 P2= RandomPList[t];
						double Theta2= RandomThetaList[t];
						
						double d= (P1-P2).Mag();
						double thetaSum= pow(Theta1,fThetaWeight)+pow(Theta2,fThetaWeight);
						double thetaSum2= Theta1 + Theta2;
						//cout<<"CurrentCurrentPairs: d="<<d<<"  thetaSum="<<thetaSum<<endl;

						fDRPairHisto->Fill(d);
						fDRPairWeightedHisto->Fill(d,thetaSum);
						
					}//end loop events in voxel
				}//end loop events in voxel

				
				//## Start pair calculation in neightbour voxels
				for(int iz=zBinId-fNCrownZ;iz<=zBinId+fNCrownZ;iz++){
					for(int iy=yBinId-fNCrownY;iy<=yBinId+fNCrownY;iy++){
						for(int ix=xBinId-fNCrownX;ix<=xBinId+fNCrownX;ix++){

							int gBinId_next= fRandomDataMap3D->GetBin(ix,iy,iz);
							double xPos_next= fRandomDataMap3D->GetXaxis()->GetBinCenter(ix);
							double yPos_next= fRandomDataMap3D->GetYaxis()->GetBinCenter(iy);
							double zPos_next= fRandomDataMap3D->GetZaxis()->GetBinCenter(iz);

							
							//cout<<"before isCompared bin="<<gBinId_next<<" ("<<ix<<","<<iy<<","<<iz<<")  Pos("<<xPos_next<<","<<yPos_next<<","<<zPos_next<<endl;
							if( fRandomDataMap3D->IsBinOverflow(gBinId_next) || fRandomDataMap3D->IsBinUnderflow(gBinId_next) ) {
								//cout<<"skip bin "<<gBinId_next<<" (overflow/underflow bin)..."<<endl;	
								continue;//not existing voxel bin...skip voxel!
							}		
							if(gBinId_next==gBinId) {
								//cout<<"skip bin "<<gBinId_next<<" (equal to previous bin)..."<<endl;
								continue;//center voxel, alredy scanned...skip voxel!
							}
							if(IsPairCompared[gBinId-1][gBinId_next-1]==1) {
								//cout<<"skip bin "<<gBinId_next<<" (already compared)..."<<endl;
								continue;//already compared...skip voxel!
							}						
							//cout<<"after isCompared"<<endl;

							IsPairCompared[gBinId-1][gBinId_next-1]= 1;	
							//IsPairCompared[gBinId_next-1][gBinId-1]= 1;	


							//## Start pair calculation inside the next voxel
							std::vector<TVector3> RandomPList_next= (fVoxelRandomDataList[gBinId_next-1]).fPointList;
							std::vector<double> RandomThetaList_next= (fVoxelRandomDataList[gBinId_next-1]).fThetaList;
							int NRandomPoints_next= RandomPList_next.size();
							
							//cout<<"--> NextVoxel "<<gBinId_next<<" ("<<ix<<","<<iy<<","<<iz<<")  Pos("<<xPos_next<<","<<yPos_next<<","<<zPos_next<<")  NPoints="<<NRandomPoints_next<<endl;
		
							//## Start pair calculation inside current-next voxel
							for(int s=0;s<NPoints;s++){
								TVector3 P1= PList[s];
								double Theta1= ThetaList[s];
								
								for(int t=0;t<NRandomPoints_next;t++){	
									TVector3 P2_next= RandomPList_next[t];
									double Theta2_next= RandomThetaList_next[t];
									
									double d= (P1-P2_next).Mag();
									double thetaSum= pow(Theta1,fThetaWeight)+pow(Theta2_next,fThetaWeight);
									double thetaSum2= Theta1 + Theta2_next;

									//cout<<"CurrentNextPairs: d="<<d<<"  thetaSum="<<thetaSum<<endl;

									fDRPairHisto->Fill(d);
									fDRPairWeightedHisto->Fill(d,thetaSum);
						
								}//end loop events in next voxel
							}//end loop events in next voxel


						}//end loop bins ix
					}//end loop bins iy
				}//end loop bins iz
				

			}//end loop bins x		
		}//end loop bins y
	}//end loop bins z

	

	cout<<"ACFAnalysis(): INFO: Calculate autocorrelation estimator..."<<endl;
	
	double N= fN;
	double Nr= fNr;
	double NDD= N*(N-1)/2.;
	double NRR= Nr*(Nr-1)/2.; 	
	double NDR= Nr*N;

	double NWeighted= fNWeighted;
	double NrWeighted= fNrWeighted;
	
	double NDDWeighted= N*(N-1)/2.;
	double NRRWeighted= Nr*(Nr-1)/2.; 	
	double NDRWeighted= Nr*N;

	bool isAlarm= false;

	for(int i=0;i<fDDPairHisto->GetNbinsX();i++){
		double rscale= fDDPairHisto->GetBinCenter(i+1);
		fDD= fDDPairHisto->GetBinContent(i+1);
		fRR= fRRPairHisto->GetBinContent(i+1);
		fDR= fDRPairHisto->GetBinContent(i+1);

		fDDWeighted= fDDPairWeightedHisto->GetBinContent(i+1);
		fRRWeighted= fRRPairWeightedHisto->GetBinContent(i+1);
		fDRWeighted= fDRPairWeightedHisto->GetBinContent(i+1);

		double ACF_LS= 1 + (NRR/NDD)*(fDD/fRR) - 2.*(NRR/NDR)*(fDR/fRR);
		double ACF_H= (NDR*NDR)/(NDD*NRR)*(fDD*fRR)/(fDR*fDR) - 1;
		double ACF_DP= (NDR/NDD)*(fDD/fDR) - 1;
		double ACF_PH= (NRR/NDD)*(fDD/fRR) - 1;
		
		double ACFWeighted_LS= 1 + (NRRWeighted/NDDWeighted)*(fDDWeighted/fRRWeighted) - 2.*(NRRWeighted/NDRWeighted)*(fDRWeighted/fRRWeighted);
		double ACFWeighted_H= (NDRWeighted*NDRWeighted)/(NDDWeighted*NRRWeighted)*(fDDWeighted*fRRWeighted)/(fDRWeighted*fDRWeighted) - 1;
		double ACFWeighted_DP= (NDRWeighted/NDDWeighted)*(fDDWeighted/fDRWeighted) - 1;
		double ACFWeighted_PH= (NRRWeighted/NDDWeighted)*(fDDWeighted/fRRWeighted) - 1;
		
		cout<<"*** ACF RESULTS ***"<<endl;
		cout<<"R="<<rscale<<endl;
		cout<<"DD="<<fDD<<"  RR="<<fRR<<"  DR="<<fDR<<endl;
		cout<<"DDWeighted="<<fDDWeighted<<"  RRWeighted="<<fRRWeighted<<"  DRWeighted="<<fDRWeighted<<endl;
		cout<<"ACF_LS= "<<ACF_LS<<"  weighted="<<ACFWeighted_LS<<endl;
		cout<<"ACF_H= "<<ACF_H<<"  weighted="<<ACFWeighted_H<<endl;
		cout<<"ACF_DP= "<<ACF_DP<<"  weighted="<<ACFWeighted_DP<<endl;
		cout<<"ACF_PH= "<<ACF_PH<<"  weighted="<<ACFWeighted_PH<<endl;
		cout<<"********************"<<endl;

		fACFGraph_LS->SetPoint(i,rscale,ACF_LS);
		fACFWeightedGraph_LS->SetPoint(i,rscale,ACFWeighted_LS);
		
		fACFGraph_H->SetPoint(i,rscale,ACF_H);
		fACFWeightedGraph_H->SetPoint(i,rscale,ACFWeighted_H);
		
		fACFGraph_DP->SetPoint(i,rscale,ACF_DP);
		fACFWeightedGraph_DP->SetPoint(i,rscale,ACFWeighted_DP);
		
		fACFGraph_PH->SetPoint(i,rscale,ACF_PH);
		fACFWeightedGraph_PH->SetPoint(i,rscale,ACFWeighted_PH);
		
		if(ACFWeighted_LS>fSignalThreshold){
			isAlarm= true;
			fAlarmRScaleList.push_back(rscale);
		}

	}//end loop steps

	QString alarmMsg = QString("0");
	if(isAlarm) {
		emit alarm();

		alarmMsg= QString("1");
		emit(alarmStatusMessage(alarmMsg));

		fAlarmMessage.status= eRunning;
		fAlarmMessage.alarmRScaleList= fAlarmRScaleList;
		fAlarmMessage.alarmMessage= Form("--> ALARM (%d alarm scales)!!!",(int)(fAlarmRScaleList.size()));
		
		QString alarmMessage= makeAlarmMessage(fAlarmMessage);
		emit alarmSig(alarmMessage);
	}

	return true;

}//close ACFRun()



void ACFAnalysisThreadObj::process(){

//## Set message 
	fLogMessage.taskName= "ACF";
	fLogMessage.command= "LOG";	
	fLogMessage.inputFileName= fInputFileName;		
	fLogMessage.outputFileName= fOutputFileName;	
	fLogMessage.jobDirName= fDestinationPath;
	fLogMessage.scanId= fScanId;
	fLogMessage.containerId= fContainerId;
	fLogMessage.timeStamp= fTimeStamp;			
	fLogMessage.status= eRunning;
	fLogMessage.logMessage= "ACF imaging started...";
	fLogMessage.progress= 0;
	fLogMessage.guiKey= fGuiKey;
	fLogMessage.guiHostName= fGuiHostName;
	fLogMessage.guiPort= fGuiPort;

	fAlarmMessage.taskName= "ACF";
	fAlarmMessage.command= "ALARM";	
	fAlarmMessage.inputFileName= fInputFileName;		
	fAlarmMessage.outputFileName= fOutputFileName;	
	fAlarmMessage.jobDirName= fDestinationPath;
	fAlarmMessage.scanId= fScanId;
	fAlarmMessage.containerId= fContainerId;
	fAlarmMessage.timeStamp= fTimeStamp;			
	fAlarmMessage.status= eRunning;
	fAlarmMessage.alarmMessage= "OK";
	fAlarmMessage.guiKey= fGuiKey;
	fAlarmMessage.guiHostName= fGuiHostName;
	fAlarmMessage.guiPort= fGuiPort;

	QString logMessage= "";
	QString msg= "";
 
	progressPercentage= 0;
	nRecStages= 4;//Init + ReadData + Run + Draw + Save
	stagePercentage= 1./(double)(nRecStages)*100.;
	fIsFailure= false;

	emit(statusProgress((int)(progressPercentage)));

	//## Init
	bool init_status= Init();
	//progressPercentage+= stagePercentage;
	//emit(statusProgress((int)(progressPercentage)));
	
	if(!init_status){
	
		msg = QString("--> ACF Image Reco: Init data failed!");
		emit(statusMessage(msg));

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= 0;

		logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
		
		return;

	}
	else{

		msg = QString("--> ACF Image Reco: Init data ... done!");
		emit(statusMessage(msg));

		progressPercentage+= stagePercentage;
		emit(statusProgress((int)(progressPercentage)));

		fLogMessage.status= eRunning;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= (int)(progressPercentage);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}


	//## Read data
	bool read_status= ReadData();
	//progressPercentage+= stagePercentage;
	//emit(statusProgress((int)(progressPercentage)));

	if(!read_status){
	
		msg = QString("--> ACF Image Reco: Read data failed!");
		emit(statusMessage(msg));

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= 0;

		logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
		
		return;

	}
	else{

		msg = QString("--> ACF Image Reco: Read data ... done!");
		emit(statusMessage(msg));

		progressPercentage+= stagePercentage;
		emit(statusProgress((int)(progressPercentage)));

		fLogMessage.status= eRunning;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= (int)(progressPercentage);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}

	//## ACF calculation
	bool run_status= ACFRun();
	//progressPercentage+= stagePercentage;
	//emit(statusProgress((int)(progressPercentage)));

	if(!run_status){
	
		msg = QString("--> ACF Image Reco: Run algo stage failed!");
		emit(statusMessage(msg));

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= 0;

		logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
		
		return;

	}
	else{

		msg = QString("--> ACF Image Reco: Run algo stage ... done!");
		emit(statusMessage(msg));

		progressPercentage+= stagePercentage;
		emit(statusProgress((int)(progressPercentage)));

		fLogMessage.status= eRunning;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= (int)(progressPercentage);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}


	//## Save
	bool save_status= Save();	
	//progressPercentage+= stagePercentage;
	//emit(statusProgress(max((int)(progressPercentage),100)));

	if(!save_status){
	
		msg = QString("--> ACF Image Reco: Save stage failed!");
		emit(statusMessage(msg));

		fLogMessage.status= eFailure;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= 0;

		logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
		
		return;

	}
	else{

		msg = QString("--> ACF Image Reco: Save stage ... done!");
		emit(statusMessage(msg));
		
		progressPercentage+= stagePercentage;
		emit(statusProgress(max((int)(progressPercentage),100)));

		fLogMessage.status= eRunning;
		fLogMessage.logMessage= msg.toStdString();
		fLogMessage.progress= max((int)(progressPercentage),100);

		QString logMessage= makeLogMessage(fLogMessage);
		emit logSig(logMessage);
	}


	emit finished();	
	msg = QString("--> ACF Image Reco: Terminated with success!");
	emit(statusMessage(msg));	

	fLogMessage.status= eFinished;
	fLogMessage.logMessage= msg.toStdString();
	fLogMessage.progress= progressPercentage;

	logMessage= makeLogMessage(fLogMessage);
	emit logSig(logMessage);
	
}//close ACFAnalysisThreadObj::process()


bool ACFAnalysisThreadObj::Save(){

	fOutputFile= new TFile(fOutputFileName.c_str(),"RECREATE");
	fOutputFile->cd();

	fDDPairHisto->Write();
	fDDPairWeightedHisto->Write();
	
	fDRPairHisto->Write();
	fDRPairWeightedHisto->Write();
	
	fRRPairHisto->Write();
	fRRPairWeightedHisto->Write();
	
	fACFGraph_LS->Write();	
	fACFWeightedGraph_LS->Write();
	fACFGraph_H->Write();	
	fACFWeightedGraph_H->Write();	
	fACFGraph_DP->Write();	
	fACFWeightedGraph_DP->Write();	
	fACFGraph_PH->Write();
	fACFWeightedGraph_PH->Write();	
	
	fOutputFile->Close();

	return true;

}//close Save()


QString ACFAnalysisThreadObj::makeLogMessage(LOG_MESSAGE msg){

	//## Create the json message to be sent 
	QVariantList jsonContainer;
 	QVariantMap json;
 	json.insert("task", QVariant(msg.taskName.c_str()) );
	json.insert("command", QVariant(msg.command.c_str()) );
	json.insert("inputFileName", QVariant(msg.inputFileName.c_str()) );
	json.insert("outputFileName", QVariant(msg.outputFileName.c_str()) );
	json.insert("jobDirName", QVariant(msg.jobDirName.c_str()) );
	json.insert("scanId", QVariant(msg.scanId) );
 	json.insert("containerId", QVariant(msg.containerId.c_str()) );
	json.insert("timeStamp", QVariant( (long long)(msg.timeStamp) ) );
	json.insert("status", QVariant(msg.status) );
	json.insert("log", QVariant(msg.logMessage.c_str()) );
	json.insert("progress", QVariant(msg.progress) );
	json.insert("guiKey", QVariant(msg.guiKey.c_str()) );	
	json.insert("guihostName", QVariant(msg.guiHostName.c_str()) );	
	json.insert("guiportNo", QVariant(msg.guiPort) );
	jsonContainer << json;

 	QJson::Serializer serializer;
	serializer.setIndentMode(QJson::IndentCompact);
 	bool ok;
 	QByteArray data = serializer.serialize(jsonContainer, &ok);
	data.append("#END#");

	return QString(data);

}//close ACFAnalysisThreadObj::makeLogMessage()


QString ACFAnalysisThreadObj::makeAlarmMessage(ALARM_MESSAGE msg){

	//## Create the json message to be sent 
	int nAlarmVoxels= (msg.alarmRScaleList).size();

	QVariantList jsonContainer;
	QVariantList rscaleListContainer;

 	QVariantMap json;
 	json.insert("task", QVariant(msg.taskName.c_str()) );
	json.insert("command", QVariant(msg.command.c_str()) );
	json.insert("inputFileName", QVariant(msg.inputFileName.c_str()) );
	json.insert("outputFileName", QVariant(msg.outputFileName.c_str()) );
	json.insert("jobDirName", QVariant(msg.jobDirName.c_str()) );
	json.insert("scanId", QVariant(msg.scanId) );
 	json.insert("containerId", QVariant(msg.containerId.c_str()) );
	json.insert("timeStamp", QVariant( (long long)(msg.timeStamp) ) );
	json.insert("status", QVariant(msg.status) );
	json.insert("log", QVariant(msg.alarmMessage.c_str()) );
		
	for(unsigned int j=0;j<nAlarmVoxels;j++){
		rscaleListContainer << QVariant(msg.alarmRScaleList[j]);
	}
	json.insert("rscale",rscaleListContainer);

	json.insert("guiKey", QVariant(msg.guiKey.c_str()) );	
	json.insert("guihostName", QVariant(msg.guiHostName.c_str()) );	
	json.insert("guiportNo", QVariant(msg.guiPort) );
	jsonContainer << json;

 	QJson::Serializer serializer;
	serializer.setIndentMode(QJson::IndentCompact);
 	bool ok;
 	QByteArray data = serializer.serialize(jsonContainer, &ok);
	data.append(Gui::SOCK_END_PATTERN.c_str());

	return QString(data);

}//close ACFAnalysisThreadObj::makeAlarmMessage()




bool ACFAnalysisThreadObj::sender(QString message){

	
	bool status= true;	
	QByteArray data;
	data.append(message);
	
	//## Create the tcp socket for communication
	cout<<"ACFAnalysisThreadObj::sender(): INFO: Create a tcp client communicating with host "<<fGuiHostName<<" on port "<<fGuiPort<<endl;
	socket= new QTcpSocket(this);
	connect(socket, SIGNAL(connected()),this, SLOT(connected()));
	socket->connectToHost(QString::fromStdString(fGuiHostName),fGuiPort);	
	

	//## Send message	
	if( socket->waitForConnected() ) {
		socket->write(data);
		socket->flush();
		socket->waitForBytesWritten(3000);
  }
	
	socket->close();
	
	return status;

}//close ACFAnalysisThreadObj::sender()


void ACFAnalysisThreadObj::SetGraphicsStyle(){

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
  myStyle->SetPalette(90,0);
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
  myStyle->SetPadBottomMargin(0.15);
  //myStyle->SetPadLeftMargin(0.15);
  //myStyle->SetPadRightMargin(0.15);
	myStyle->SetPadLeftMargin(0.10);
  myStyle->SetPadRightMargin(0.15);	
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

	int fPaletteId= 1;
	gStyle->SetPalette(fPaletteId);
	//gStyle->SetOptLogz(1);
	gStyle->SetTitleFont(52,"X");
	gStyle->SetTitleFont(52,"Y");
	gStyle->SetTitleFont(52,"Z");
	gStyle->SetTitleFont(42,"X");
	gStyle->SetTitleFont(42,"Y");
	gStyle->SetTitleFont(42,"Z");

	int fNColourContours= 99;

	static const UInt_t Number = 12;
  static Double_t Red[Number]    = { 0.164, 0.150, 0.250, 0.450, 0.670, 0.880, 1.000, 1.000,1.000,0.970,0.850,0.650};
  static Double_t Green[Number]  = { 0.043, 0.306, 0.630, 0.853, 0.973, 1.000,1.000,0.880,0.679,0.430,0.150,0.000};
  static Double_t Blue[Number]   = { 0.850,1.000,1.000,1.000,1.000,1.000,0.750,0.600,0.450,0.370,0.196,0.130};
	double Length[Number] = { 0.,0.2,0.3,0.4,0.5,0.7,0.75,0.8,0.85,0.9,0.95,1};// viola, blu,celeste,verde,giallo,rosso
	TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,fNColourContours);

}//close SetGraphicsStyle()

}//close namespace
