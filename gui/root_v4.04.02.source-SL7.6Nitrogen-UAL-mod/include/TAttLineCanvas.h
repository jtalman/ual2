// @(#)root/gpad:$Name:  $:$Id: TAttLineCanvas.h,v 1.2 2003/03/05 07:16:07 brun Exp $
// Author: Rene Brun   03/07/96

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TAttLineCanvas
#define ROOT_TAttLineCanvas


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TAttLineCanvas                                                       //
//                                                                      //
// A specialized dialog canvas to set line attributes.                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TDialogCanvas
#include "TDialogCanvas.h"
#endif

class TAttLineCanvas : public TDialogCanvas {

public:
   TAttLineCanvas();
   TAttLineCanvas(const char *name, const char *title, Int_t ww=400, Int_t wh=600);
   virtual        ~TAttLineCanvas();
   virtual void   UpdateLineAttributes(Int_t col, Int_t sty, Int_t width);

   ClassDef(TAttLineCanvas,0)  //A specialized dialog canvas to set line attributes.
};

#endif

