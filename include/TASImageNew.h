#ifndef _TAS_Image_New
#define _TAS_Image_New

#include <TASImage.h>


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

#endif
