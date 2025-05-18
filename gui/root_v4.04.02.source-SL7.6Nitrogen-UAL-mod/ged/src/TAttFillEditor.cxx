// @(#)root/ged:$Name:  $:$Id: TAttFillEditor.cxx,v 1.7 2004/12/16 09:04:49 brun Exp $
// Author: Ilka Antcheva   10/05/04

/*************************************************************************
 * Copyright (C) 1995-2002, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//  TAttFillEditor                                                      //
//                                                                      //
//  Implements GUI for editing fill attributes.                         //                                             //
//             color and fill style                                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
//Begin_Html
/*
<img src="gif/TAttFillEditor.gif">
*/
//End_Html

#include "TAttFillEditor.h"
#include "TGedPatternSelect.h"
#include "TGColorSelect.h"
#include "TGColorDialog.h"
#include "TGClient.h"
#include "TColor.h"
#include "TVirtualPad.h"

ClassImp(TGedFrame)
ClassImp(TAttFillEditor)

enum {
   kCOLOR,
   kPATTERN
};


//______________________________________________________________________________
TAttFillEditor::TAttFillEditor(const TGWindow *p, Int_t id, Int_t width,
                               Int_t height, UInt_t options, Pixel_t back)
   : TGedFrame(p, id, width, height, options | kVerticalFrame, back)
{
   // Constructor of fill attributes GUI.

   fAttFill = 0;
   
   MakeTitle("Fill");

   TGCompositeFrame *f2 = new TGCompositeFrame(this, 80, 20, kHorizontalFrame);
   fColorSelect = new TGColorSelect(f2, 0, kCOLOR);
   f2->AddFrame(fColorSelect, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
   fColorSelect->Associate(this);
   fPatternSelect = new TGedPatternSelect(f2, 1, kPATTERN);
   f2->AddFrame(fPatternSelect, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
   fPatternSelect->Associate(this);
   AddFrame(f2, new TGLayoutHints(kLHintsTop, 1, 1, 0, 0));


   TClass *cl = TAttFill::Class();
   TGedElement *ge = new TGedElement;
   ge->fGedFrame = this;
   ge->fCanvas = 0;
   cl->GetEditorList()->Add(ge);
}

//______________________________________________________________________________
TAttFillEditor::~TAttFillEditor()
{ 
   // Destructor of fill editor.

   TGFrameElement *el;
   TIter next(GetList());
   
   while ((el = (TGFrameElement *)next())) {
      if (!strcmp(el->fFrame->ClassName(), "TGCompositeFrame"))
         ((TGCompositeFrame *)el->fFrame)->Cleanup();
   }
   Cleanup(); 
}

//______________________________________________________________________________
void TAttFillEditor::ConnectSignals2Slots()
{
   // Connect signals to slots.

   fColorSelect->Connect("ColorSelected(Pixel_t)", "TAttFillEditor", this, "DoFillColor(Pixel_t)");
   fPatternSelect->Connect("PatternSelected(Style_t)", "TAttFillEditor", this, "DoFillPattern(Style_t)");
   fInit = kFALSE;
}

//______________________________________________________________________________
void TAttFillEditor::SetModel(TVirtualPad* pad, TObject* obj, Int_t)
{
   // Pick up the used fill attributes.

   fModel = 0;
   fPad = 0;

   if (obj == 0 || !obj->InheritsFrom("TAttFill") || obj->InheritsFrom("TCurlyLine")) {
      SetActive(kFALSE);
      return;
   }

   fModel = obj;
   fPad = pad;

   fAttFill = dynamic_cast<TAttFill *>(fModel);

   Color_t c = fAttFill->GetFillColor();
   Pixel_t p = TColor::Number2Pixel(c);
   fColorSelect->SetColor(p);

   Style_t s = fAttFill->GetFillStyle();
   fPatternSelect->SetPattern(s);

   if (fInit) ConnectSignals2Slots();
   SetActive();
}

//______________________________________________________________________________
void TAttFillEditor::DoFillColor(Pixel_t color)
{
   // Slot connected to the fill area color.

   fAttFill->SetFillColor(TColor::GetColor(color));
   Update();
}

//______________________________________________________________________________
void TAttFillEditor::DoFillPattern(Style_t pattern)
{
   // Slot connected to the fill area pattern.

   fAttFill->SetFillStyle(pattern);
   Update();
}
