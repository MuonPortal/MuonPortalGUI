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
* @file VoxelData.cc
* @class VoxelData
* @brief Define the voxel data structure for the reconstruction
*
* @author S. Riggi
* @date 23/03/2012
*/

#include <VoxelData.h>
#include <TMatrixD.h>

#include <iostream>

using namespace std;

namespace MuonPortalNS {

VoxelData::VoxelData(){

	fId= 0;
	fWx= 0.;
	fWy= 0.;
	fWxy= 0.;
	fS= 0.;

}//close constructor

VoxelData::~VoxelData(){


}//close destructor

void VoxelData::Append(VoxelData aNewVoxel){


}//close VoxelData::Append()

}//close namespace

