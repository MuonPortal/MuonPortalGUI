/**
* @file VoxelData.h
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



