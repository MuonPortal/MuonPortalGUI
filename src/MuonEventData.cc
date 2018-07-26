/**
* @file MuonEventData.cc
* @class MuonEventData
* @brief Define the muon event data structure for the reconstruction
*
* @author S. Riggi
* @date 23/03/2012
*/

#include <MuonEventData.h>
#include <Utilities.h>

#include <TMatrixD.h>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;


MuonEventData::MuonEventData(){

	fEventId= 0;

	fVoxelDataCollection.clear();
	fVoxelDataCollection.resize(0);

	fCx= 0;
	fCy= 0;
	fCxy= 0;

	fDeltaThetaX= 0;
	fDeltaThetaY= 0;
	fDeltaX= 0;
	fDeltaY= 0;
	fPr= 1.;

}//close constructor

MuonEventData::~MuonEventData(){


}//close destructor


void MuonEventData::Reset(){

	//## Reset scintillator hit info
	fVoxelDataCollection.clear();
	fVoxelDataCollection.resize(0);

	fEventId= 0;

	fCx= 0;
	fCy= 0;
	fCxy= 0;
	fEx= 0;
	fEy= 0;
	fExy= 0;

	fDeltaThetaX= 0;
	fDeltaThetaY= 0;
	fDeltaX= 0;
	fDeltaY= 0;
	fPr= 1.;

}//close MuonEventData::Reset()

bool MuonEventData::HasVoxelData(VoxelData aVoxelData, int& pos){

	//## Search if passed voxel already exists in the collection
	pos= -1;
	
	if(fVoxelDataCollection.empty()) {
		pos= -1;
		return false;
	}

	std::vector<VoxelData>::iterator it= std::find(fVoxelDataCollection.begin(), fVoxelDataCollection.end(), aVoxelData);
	
	if(it == fVoxelDataCollection.end()){
		pos= -1;
		return false;	
	}	
	else{
		pos = it - fVoxelDataCollection.begin();
		return true;
	}

	return false;

}//close MuonEventData::HasVoxelData()


void MuonEventData::AddVoxelData(VoxelData aVoxelData){
	
	fVoxelDataCollection.push_back(aVoxelData);


}//close MuonEventData::AddVoxelData()




void MuonEventData::SetCovarianceMatrix(std::vector<double> par){

	//## Calculate covariance matrix
	TMatrixD scatteringMatrixSum(2,2);
	scatteringMatrixSum.Zero();

	TMatrixD E(2,2);
	E(0,0)= fEx;
	E(1,1)= fEy;
	E(0,1)= fExy;
	E(1,0)= E(0,1);

	
	//cout<<"--> Muon Event no. "<<fEventId<<endl;

	for(unsigned int j=0;j<fVoxelDataCollection.size();j++){
		VoxelData voxel= fVoxelDataCollection[j];
		int voxelId= voxel.fId;
		TMatrixD W= TMatrixD(2,2);
		W(0,0)= voxel.fWx;
		W(1,1)= voxel.fWy;
		W(0,1)= voxel.fWxy;
		W(1,0)= W(0,1);
		
		//cout<<"== W"<<j<<" =="<<endl;
		//W.Print();
		
		scatteringMatrixSum+= par[voxelId]*W;	
	
	}//end loop voxels in this event

	
	TMatrixD C= TMatrixD(2,2);
	C= E + fPr*fPr*scatteringMatrixSum;
	
	//## Store covariance matrix
	fCx= C(0,0);
	fCy= C(1,1);
	fCxy= C(0,1);
	
	//cout<<"fPr="<<fPr<<" C=("<<fCx<<","<<fCxy<<","<<fCxy<<","<<fCy<<")"<<endl;
	
	
}//close MuonEventData::SetCovarianceMatrix()



void MuonEventData::SetConditionalExpectation(std::vector<double> par){

	int nVoxelsInEvent= (int)fVoxelDataCollection.size();
	
	//## Calculate S= 2xlambda + (a-b)*(pxlambda)^2 
	//## a= Dt CInv W CInv D
	//## b= Tr(CInv W)
	TMatrixD Dx= TMatrix(2,1);
	Dx(0,0)= fDeltaThetaX;
	Dx(1,0)= fDeltaX;

	TMatrixD Dxt= TMatrix(1,2);
	Dxt= TMatrixD(TMatrixD::kTransposed, Dx);
	
	TMatrixD Dy= TMatrix(2,1);
	Dy(0,0)= fDeltaThetaY;
	Dy(1,0)= fDeltaY;

	TMatrixD Dyt= TMatrix(1,2);
	Dyt= TMatrixD(TMatrixD::kTransposed, Dy);


	double CDet= fCx*fCy-fCxy*fCxy;	
	if (CDet<=0) {
		cerr<<"MuonEventData::SetConditionalExpectation(): WARNING: Covariance matrix inversion failed (determinant is zero or negative)!"<<endl;
		exit(1);
	}
	TMatrixD CInv(2,2);
	CInv(0,0)= fCy/CDet;
	CInv(1,1)= fCx/CDet;
	CInv(0,1)= -fCxy/CDet;	
	CInv(1,0)= CInv(0,1);

	//cout<<"*** TEST CINV ***"<<endl;
	TMatrixD C(2,2);
	C(0,0)= fCx;
	C(1,1)= fCy;
	C(0,1)= fCxy;	
	C(1,0)= C(0,1);

	TMatrixD CCInv(2,2);
	CCInv= C*CInv;
	//CCInv.Print();
	//cout<<endl;

	

	double LogLikelihood_x= 0.5*(log(CDet) + (Dxt*CInv*Dx)(0,0) );
	double LogLikelihood_y= 0.5*(log(CDet) + (Dyt*CInv*Dy)(0,0) );
	fLikelihood= LogLikelihood_x + LogLikelihood_y;

	//cout<<"*** Dx ***"<<endl;
	//Dx.Print();

	for(unsigned int j=0;j<fVoxelDataCollection.size();j++){
		VoxelData voxel= fVoxelDataCollection[j];
		int voxelId= voxel.fId;

		TMatrixD W= TMatrixD(2,2);
		W(0,0)= voxel.fWx;
		W(1,1)= voxel.fWy;
		W(0,1)= voxel.fWxy;
		W(1,0)= W(0,1);

		double b= Utilities::Trace( CInv*W );
		//cout<<"*** CInv *** "<<endl;
		//CInv.Print();
		//cout<<"*** W *** "<<endl;
		//W.Print();
		//cout<<"*** CInv W *** "<<endl;
		//(CInv*W).Print();
		//cout<<"Trace= "<<b<<endl;

		TMatrixD ProdMatrix(1,1);
		ProdMatrix.Zero();
		ProdMatrix= Dxt*CInv*W*CInv*Dx;
		double ax= ProdMatrix(0,0);
		double Sx= 2.*par[voxelId] + (ax-b)* pow(fPr*par[voxelId],2);	
		
		ProdMatrix.Zero();
		ProdMatrix= Dyt*CInv*W*CInv*Dy;
		double ay= ProdMatrix(0,0);
		double Sy= 2.*par[voxelId] + (ay-b)* pow(fPr*par[voxelId],2);	
		
		double S= (Sx+Sy)/2.; 
		
		//cout<<"voxelId="<<voxelId<<"  ay="<<ay<<"  by="<<by<<"  Sy="<<Sy<<endl;
		//## DEBUG
		/* 
		if(voxelId==2850){
			cout<<"Voxel no. "<<voxelId<<"  Event no. "<<fEventId<<"  Sx="<<Sx<<"  Sy="<<Sy<<"  S="<<S<<endl;	
			cout<<"W"<<endl;
			W.Print();
			cout<<"Dx"<<endl;
			Dx.Print();
			cout<<"Dy"<<endl;
			Dy.Print();
			cout<<"CInv"<<endl;
			CInv.Print();
		}
		*/

		//## add S info to each voxel
		fVoxelDataCollection[j].fS= S;
		
	}//end loop voxels in event


}//close MuonEventData::SetConditionalExp()



