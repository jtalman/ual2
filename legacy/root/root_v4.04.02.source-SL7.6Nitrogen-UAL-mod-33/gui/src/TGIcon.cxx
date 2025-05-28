// @(#)root/gui:$Name:  $:$Id: TGIcon.cxx,v 1.9 2003/12/12 18:21:06 rdm Exp $
// Author: Fons Rademakers   05/01/98

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/
/**************************************************************************

    This source is based on Xclass95, a Win95-looking GUI toolkit.
    Copyright (C) 1996, 1997 David Barth, Ricky Ralston, Hector Peraza.

    Xclass95 is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

**************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGIcon                                                               //
//                                                                      //
// This class handles GUI icons.                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TGIcon.h"
#include "TGPicture.h"
#include "TSystem.h"
#include "Riostream.h"

ClassImp(TGIcon)

//______________________________________________________________________________
TGIcon::~TGIcon()
{
   // Delete icon and free picture.

   if (fPic) fClient->FreePicture(fPic);
}

//______________________________________________________________________________
void TGIcon::SetPicture(const TGPicture *pic)
{
   // Set icon picture.

   fPic = pic;
   gVirtualX->ClearWindow(fId);
   fClient->NeedRedraw(this);
}

//______________________________________________________________________________
TGDimension TGIcon::GetDefaultSize() const
{
   // Return size of icon.

   return TGDimension((fPic) ? fPic->GetWidth()  : fWidth,
                      (fPic) ? fPic->GetHeight() : fHeight);
}

//______________________________________________________________________________
void TGIcon::DoRedraw()
{
   // Redraw picture.

   if (fPic) fPic->Draw(fId, GetBckgndGC()(), 0, 0);
}

//______________________________________________________________________________
void TGIcon::SavePrimitive(ofstream &out, Option_t *option)
{
    // Save an icon widget as a C++ statement(s) on output stream out.

   char quote = '"';

   if (fBackground != GetDefaultFrameBackground()) SaveUserColor(out, option);

   if (!fPic) {
      Error("SavePrimitive()", "icon pixmap not found ");
      return;
   }

   const char *picname = fPic->GetName();

   out <<"   TGIcon *";
   out << GetName() << " = new TGIcon(" << fParent->GetName()
       << ",gClient->GetPicture(" << quote
	      << gSystem->ExpandPathName(gSystem->UnixPathName(picname))                       // if no path
       << quote << ")" << "," << GetWidth() << "," << GetHeight();

   if (fBackground == GetDefaultFrameBackground()) {
      if (!GetOptions()) {
         out <<");" << endl;
      } else {
         out << "," << GetOptionString() <<");" << endl;
      }
   } else {
      out << "," << GetOptionString() << ",ucolor);" << endl;
   }
}
