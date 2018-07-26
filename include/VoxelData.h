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
* @file VoxelData.h
* @class VoxelData
* @brief Define the voxel data structure for the reconstruction
*
* @author S. Riggi
* @date 23/03/2012
*/


#ifndef VoxelData_h
#define VoxelData_h 1

#include <Rtypes.h>
#include <TObject.h>
#include <TVector3.h>
#include <TMatrixD.h>

#include <iostream>
#include <vector>

namespace MuonPortalNS {

class VoxelData {

	public:
		/** 
		\brief Class constructor: initialize structures.
 		*/   
  	VoxelData();
		/** 
		\brief Class destructor: free allocated memory.
 		*/
    ~VoxelData();

		/** 
		\brief Append info to voxel data
 		*/
		void Append(VoxelData aNewVoxel);
		
		/** 
		\brief Operator to compare two voxel data objects
 		*/
		bool operator==(const VoxelData& aVoxel) const {
			return (fId==aVoxel.fId); 
    }
		
	
	public:

		/** 
		\brief Voxel index
 		*/
		unsigned int fId;
		/** 
		\brief Scattering covariance matrix for this voxel
 		*/		
		double fWx;
		double fWy;
		double fWxy;

		/** 
		\brief Conditional exp for this voxel
 		*/
		double fS;
};

}//close namespace

#endif
