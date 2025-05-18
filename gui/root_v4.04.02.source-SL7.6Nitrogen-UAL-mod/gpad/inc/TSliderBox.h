// @(#)root/gpad:$Name:  $:$Id: TSliderBox.h,v 1.2 2000/06/13 12:14:34 brun Exp $
// Author: Rene Brun   23/11/96

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TSliderBox
#define ROOT_TSliderBox


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSliderBox                                                           //
//                                                                      //
// The moving box of a TSlider                                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef ROOT_TWbox
#include "TWbox.h"
#endif

class TSlider;

class TSliderBox : public TWbox {

protected:
   TSlider   *fSlider;     //Pointer to slider

public:
   TSliderBox();
   TSliderBox(Double_t x1, Double_t y1,Double_t x2 ,Double_t y2,
              Color_t color=18, Short_t bordersize=2 ,Short_t bordermode=-1);
   virtual ~TSliderBox();
   virtual void  ExecuteEvent(Int_t event, Int_t px, Int_t py);
   virtual void  SavePrimitive(ofstream &out, Option_t *option);
   void          SetSlider(TSlider*slider) {fSlider=slider;}

   ClassDef(TSliderBox,1)  //The moving box of a TSlider
};

#endif

