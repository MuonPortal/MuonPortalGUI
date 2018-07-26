#include "TASImageNew.h"

#include "TASImagePlugin.h"
#include "TROOT.h"
#include "TMath.h"
#include "TSystem.h"
#include "TVirtualX.h"
#include "TVirtualPad.h"
#include "TArrayD.h"
#include "TVectorD.h"
#include "TVirtualPS.h"
#include "TGaxis.h"
#include "TColor.h"
#include "TObjArray.h"
#include "TArrayL.h"
#include "TPoint.h"
#include "TFrame.h"
#include "TTF.h"
#include "TRandom.h"
#include "Riostream.h"
#include "THashTable.h"
#include "TPluginManager.h"
#include "TEnv.h"
#include "TStyle.h"
#include "TText.h"
#include "RConfigure.h"
#include "TVirtualPadPainter.h"

#ifndef WIN32
#ifndef R__HAS_COCOA
#   include <X11/Xlib.h>
#endif
#else
#   include "Windows4root.h"
#endif
extern "C" {
#ifndef WIN32
#ifdef R__HAS_COCOA
#   define X_DISPLAY_MISSING 1
#endif
#   include <afterbase.h>
#else
#   include <win32/config.h>
#   include <win32/afterbase.h>
#   define X_DISPLAY_MISSING 1
#endif
#   include <afterimage.h>
#   include <bmp.h>
#   include <draw.h>
}

// auxiliary functions for general polygon filling
#include "MyTASPolyUtils.c"

ClassImp(TASImageNew)


TASImageNew::TASImageNew() 
	: TASImage() 
{

}


TASImageNew::TASImageNew(UInt_t w, UInt_t h) 
	: TASImage(w, h)
{
   
}


TASImageNew::TASImageNew(const char *file, EImageFileTypes type) 
	: TASImage(file,type)
{
  
}


TASImageNew::TASImageNew(const char *name, const Double_t *imageData, UInt_t width,
                   UInt_t height, TImagePalette *palette,bool isFixRange,double min,double max) 
	: TASImage(name,imageData,width,height,palette)
{
   //SetDefaults();
   SetImage(imageData,width, height, palette,isFixRange,min,max);
}


TASImageNew::TASImageNew(const char *name, const TArrayD &imageData, UInt_t width,
                   TImagePalette *palette,bool isFixRange,double min,double max) 
	: TASImage(name,imageData,width,palette)
{
   //SetDefaults();
   SetImage(imageData, width, palette,isFixRange,min,max);
}


TASImageNew::TASImageNew(const char *name, const TVectorD &imageData, UInt_t width,
                   TImagePalette *palette,bool isFixRange,double min,double max) 
	: TASImage(name,imageData,width,palette)	
{
   //SetDefaults();
   SetImage(imageData, width, palette,isFixRange,min,max);
}


TASImageNew::TASImageNew(const TASImageNew &img) : TASImage(img)
{
   
}

TASImageNew::~TASImageNew()
{
  
}


void TASImageNew::DestroyImage() {
   if (fImage) {
      destroy_asimage(&fImage);
   }

   if (fIsGray && fGrayImage) {
      destroy_asimage(&fGrayImage);
   }

   fIsGray     = kFALSE;
   fGrayImage  = 0;
   fImage      = 0;
}


void TASImageNew::SetImage(const Double_t *imageData, UInt_t width, UInt_t height,
                        TImagePalette *palette,bool isFixRange,double min,double max)
{
   TAttImage::SetPalette(palette);

   if (!InitVisual()) {
      Warning("SetImage", "Visual not initiated");
      return;
   }

   DestroyImage();
   delete fScaledImage;
   fScaledImage = 0;

	

   // get min and max value of image
   fMinValue = fMaxValue = *imageData;
   for (Int_t pixel = 1; pixel < Int_t(width * height); pixel++) {
      if (fMinValue > *(imageData + pixel)) fMinValue = *(imageData + pixel);
      if (fMaxValue < *(imageData + pixel)) fMaxValue = *(imageData + pixel);
   }

	 if(isFixRange){
	   fMinValue= min;
		 fMaxValue= max;
	 }	

   // copy ROOT palette to asImage palette
   const TImagePalette &pal = GetPalette();

   ASVectorPalette asPalette;

   asPalette.npoints = pal.fNumPoints;
   Int_t col;
   for (col = 0; col < 4; col++)
      asPalette.channels[col] = new UShort_t[asPalette.npoints];

   memcpy(asPalette.channels[0], pal.fColorBlue,  pal.fNumPoints * sizeof(UShort_t));
   memcpy(asPalette.channels[1], pal.fColorGreen, pal.fNumPoints * sizeof(UShort_t));
   memcpy(asPalette.channels[2], pal.fColorRed,   pal.fNumPoints * sizeof(UShort_t));
   memcpy(asPalette.channels[3], pal.fColorAlpha, pal.fNumPoints * sizeof(UShort_t));

   asPalette.points = new Double_t[asPalette.npoints];
   for (Int_t point = 0; point < Int_t(asPalette.npoints); point++)
      asPalette.points[point] = fMinValue + (fMaxValue - fMinValue) * pal.fPoints[point];

   fImage = create_asimage_from_vector(fgVisual, (Double_t*)imageData, width,
                                       height, &asPalette, ASA_ASImage,
                                       GetImageCompression(), GetImageQuality());

   delete [] asPalette.points;
   for (col = 0; col < 4; col++)
      delete [] asPalette.channels[col];

   fZoomUpdate = 0;
   fZoomOffX   = 0;
   fZoomOffY   = 0;
   fZoomWidth  = width;
   fZoomHeight = height;
   fPaletteEnabled = kTRUE;
}


void TASImageNew::SetImage(const TArrayD &imageData, UInt_t width, TImagePalette *palette,bool isFixRange,double min,double max)
{
   SetImage(imageData.GetArray(), width, imageData.GetSize() / width, palette,isFixRange,min,max);
}


void TASImageNew::SetImage(const TVectorD &imageData, UInt_t width, TImagePalette *palette,bool isFixRange,double min,double max)
{
   SetImage(imageData.GetMatrixArray(), width,
            imageData.GetNoElements() / width, palette,isFixRange,min,max);
}

