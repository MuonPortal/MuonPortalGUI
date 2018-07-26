/**
* @file MuonEventData.h
* @class MuonEventData
* @brief Define the muon event data structure for the reconstruction
*
* @author S. Riggi
* @date 23/03/2012
*/


#ifndef MuonEventData_h
#define MuonEventData_h 1

#include <VoxelData.h>
#include <TMatrixD.h>

#include <Rtypes.h>
#include <TObject.h>
#include <TVector3.h>

#include <iostream>
#include <vector>


class MuonEventData {

	public:
		/** 
		\brief Class constructor: initialize structures.
 		*/   
  	MuonEventData();
		/** 
		\brief Class destructor: free allocated memory.
 		*/
    ~MuonEventData();
		/** 
		\brief Reset event data
 		*/
		void Reset();
		/** 
		\brief Has voxel data?
 		*/
		bool HasVoxelData(VoxelData aVoxelData, int& pos);

		/** 
		\brief Append voxel data
 		*/
		void AddVoxelData(VoxelData aVoxelData);

		/** 
		\brief Set Covariance matrix
 		*/
		//void SetCovarianceMatrix();
		void SetCovarianceMatrix(std::vector<double> par);

		/** 
		\brief Set the conditional expectations Sx, Sy
 		*/
		//void SetConditionalExpectation();
		void SetConditionalExpectation(std::vector<double> par);

	public:

		unsigned int fEventId;
		double fDeltaThetaX;
		double fDeltaThetaY;
		double fDeltaX;
		double fDeltaY;
		double fPr;

		/** 
		\brief Covariance matrix for this event
 		*/
		double fCx;
		double fCy;
		double fCxy;
	
		/** 
		\brief Error matrix for this event
 		*/
		double fEx;
		double fEy;
		double fExy;

		/** 
		\brief Collection of voxel data for this event
 		*/
		std::vector<VoxelData> fVoxelDataCollection;

		/** 
		\brief Likelihood for this event
 		*/
		double fLikelihood;
};


#endif
