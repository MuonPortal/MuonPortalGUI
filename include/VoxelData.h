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

using namespace std;

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


#endif
