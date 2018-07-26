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
* @file TASImageNew.h
* @class TASImageNew
* @brief Define a TAS image
*
* @author S. Riggi
* @date 23/03/2012
*/


#ifndef _TAS_Image_New
#define _TAS_Image_New

#include <TASImage.h>

namespace MuonPortalNS {

class TASImageNew : public TASImage {

	public:
		TASImageNew();
   	TASImageNew(UInt_t w, UInt_t h);
   	TASImageNew(const char *file, EImageFileTypes type = kUnknown);
		TASImageNew(const char *name, const Double_t *imageData, UInt_t width, UInt_t height, TImagePalette *palette = 0,bool isFixRange=false,double min=0,double max=0);
   	TASImageNew(const char *name, const TArrayD &imageData, UInt_t width, TImagePalette *palette = 0,bool isFixRange=false,double min=0,double max=0);
   	TASImageNew(const char *name, const TVectorD &imageData, UInt_t width, TImagePalette *palette = 0,bool isFixRange=false,double min=0,double max=0);

		TASImageNew(const TASImageNew &img);
   	virtual ~TASImageNew();	
	
	public:
		void  SetImage(const Double_t *imageData, UInt_t width, UInt_t height, TImagePalette *palette = 0,bool isFixRange=false,double min=0,double max=0);
   	void  SetImage(const TArrayD &imageData, UInt_t width, TImagePalette *palette = 0,bool isFixRange=false,double min=0,double max=0);
  	void  SetImage(const TVectorD &imageData, UInt_t width, TImagePalette *palette = 0,bool isFixRange=false,double min=0,double max=0);
		
	protected:
		void DestroyImage();
		
	ClassDef(TASImageNew,1) 

};//close class

#ifdef __MAKECINT__
#pragma link C++ class TASImageNew+; 
#endif

}//close namespace

#endif
