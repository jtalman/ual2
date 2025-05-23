// @(#)root/guibuilder:$Name:  $:$Id: TGuiBldHintsButton.h,v 1.2 2004/09/21 10:09:18 brun Exp $
// Author: Valeriy Onuchin   12/09/04

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGuiBldHintsButton
#define ROOT_TGuiBldHintsButton


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGuiBldHintsButton                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef ROOT_TGButton
#include "TGButton.h"
#endif


class TGuiBldHintsButton : public TGButton {

protected:
   virtual void DrawExpandX();
   virtual void DrawExpandY();
   virtual void DrawCenterX();
   virtual void DrawCenterY();
   virtual void DrawTopLeft();
   virtual void DrawTopRight();
   virtual void DrawBottomLeft();
   virtual void DrawBottomRight();

   virtual void DoRedraw();

public:
   TGuiBldHintsButton(const TGWindow *p, Int_t id);
   virtual ~TGuiBldHintsButton() {}

   ClassDef(TGuiBldHintsButton,0) //button used to edit layout hints
};

#endif
