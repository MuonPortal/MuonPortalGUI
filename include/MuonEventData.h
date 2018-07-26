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

namespace MuonPortalNS {

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

};//close class

}//close namespace

#endif
