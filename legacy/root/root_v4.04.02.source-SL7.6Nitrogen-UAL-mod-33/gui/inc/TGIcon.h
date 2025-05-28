// @(#)root/gui:$Name:  $:$Id: TGIcon.h,v 1.5 2004/09/08 08:13:11 brun Exp $
// Author: Fons Rademakers   05/01/98

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGIcon
#define ROOT_TGIcon


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGIcon                                                               //
//                                                                      //
// This class handles GUI icons.                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGDimension
#include "TGDimension.h"
#endif

class TGPicture;


class TGIcon : public TGFrame {

protected:
   const TGPicture  *fPic;     // icon picture

   virtual void DoRedraw();

public:
   TGIcon(const TGWindow *p = 0, const TGPicture *pic = 0, UInt_t w = 1, UInt_t h = 1,
      UInt_t options = kChildFrame, Pixel_t back = GetDefaultFrameBackground()) :
      TGFrame(p, w, h, options, back) { fPic = pic; }
   ~TGIcon();

   const TGPicture *GetPicture() const { return fPic; }
   virtual void SetPicture(const TGPicture *pic);

   virtual TGDimension GetDefaultSize() const;
   virtual void SavePrimitive(ofstream &out, Option_t *option); 

   ClassDef(TGIcon,0)  // Icon GUI class
};

#endif
