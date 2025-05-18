// @(#)root/gui:$Name:  $:$Id: TGShutter.cxx,v 1.9 2004/12/07 15:34:27 brun Exp $
// Author: Fons Rademakers   18/9/2000

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGShutter, TGShutterItem                                             //
//                                                                      //
// A shutter widget contains a set of shutter items that can be         //
// open and closed like a shutter.                                      //
// This widget is usefull to group a large number of options in         //
// a number of categories.                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TGShutter.h"
#include "TGButton.h"
#include "TList.h"
#include "TTimer.h"
#include "Riostream.h"


ClassImp(TGShutterItem)
ClassImp(TGShutter)

//______________________________________________________________________________
TGShutter::TGShutter(const TGWindow *p, UInt_t options) :
   TGCompositeFrame(p, 10, 10, options)
{
   // Create shutter frame.

   fSelectedItem        = 0;
   fClosingItem         = 0;
   fHeightIncrement     = 1;
   fClosingHeight       = 0;
   fClosingHadScrollbar = kFALSE;
   fTimer               = 0;
   fTrash               = new TList;
}

//______________________________________________________________________________
TGShutter::~TGShutter()
{
   // Cleanup shutter widget.

   if (fTimer) delete fTimer;

   if (!MustCleanup()) {
      fTrash->Delete();
   }
   delete fTrash;
   fTrash = 0;
}

//______________________________________________________________________________
void TGShutter::AddItem(TGShutterItem *item)
{
   // Add shutter item to shutter frame.

   TGLayoutHints *hints = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY);
   AddFrame(item, hints);
   fTrash->Add(hints);
   if (!fSelectedItem)
      fSelectedItem = item;
}

//______________________________________________________________________________
Bool_t TGShutter::ProcessMessage(Long_t /*msg*/, Long_t parm1, Long_t /*parm2*/)
{
   // Handle shutter messages.

   if (!fList) return kFALSE;

   TGFrameElement *el;
   TGShutterItem  *child, *item = 0;

   TIter next(fList);
   while ((el = (TGFrameElement *) next())) {
      child = (TGShutterItem *) el->fFrame;
      if (parm1 == child->WidgetId()) {
         item = child;
         break;
      }
   }

   if (!item) return kFALSE;

   if (!fSelectedItem)
      fSelectedItem = (TGShutterItem*) ((TGFrameElement*)fList->First())->fFrame;
   if (fSelectedItem == item) return kTRUE;

   fHeightIncrement = 1;
   fClosingItem = fSelectedItem;
   fClosingHeight = fClosingItem->GetHeight();
   fClosingHeight -= fClosingItem->fButton->GetDefaultHeight();
   fSelectedItem = item;
   if (!fTimer) fTimer = new TTimer(this, 6); //10);
   fTimer->Reset();
   fTimer->TurnOn();

   return kTRUE;
}

//______________________________________________________________________________
Bool_t TGShutter::HandleTimer(TTimer *)
{
   // Shutter item animation.

   if (!fClosingItem) return kFALSE;
   fClosingHeight -= fHeightIncrement;
   fHeightIncrement += 5;
   if (fClosingHeight > 0) {
      fTimer->Reset();
   } else {
      fClosingItem   = 0;
      fClosingHeight = 0;
      fTimer->TurnOff();
   }
   Layout();

   return kTRUE;
}

//______________________________________________________________________________
void TGShutter::Layout()
{
   // Layout shutter items.

   TGFrameElement *el;
   TGShutterItem  *child;
   Int_t y, bh, exh;

   if (!fList) return;

   if (!fSelectedItem)
      fSelectedItem = (TGShutterItem*) ((TGFrameElement*)GetList()->First())->fFrame;

   exh = Int_t(fHeight - (fBorderWidth << 1));
   TIter next(fList);
   while ((el = (TGFrameElement *) next())) {
      child = (TGShutterItem *) el->fFrame;
      bh = child->fButton->GetDefaultHeight();
      exh -= bh;
   }

   y = fBorderWidth;
   next.Reset();
   while ((el = (TGFrameElement *) next())) {
      child = (TGShutterItem *) el->fFrame;
      bh = child->fButton->GetDefaultHeight();
      if (child == fSelectedItem) {
         if (fClosingItem)
            child->fCanvas->SetScrolling(TGCanvas::kCanvasNoScroll);
         else
            child->fCanvas->SetScrolling(TGCanvas::kCanvasScrollVertical);
         child->ShowFrame(child->fCanvas);
         child->MoveResize(fBorderWidth, y, fWidth - (fBorderWidth << 1),
                           exh - fClosingHeight + bh);
         y += exh - fClosingHeight + bh;
      } else if (child == fClosingItem) {
         child->fCanvas->SetScrolling(TGCanvas::kCanvasNoScroll);
         child->MoveResize(fBorderWidth, y, fWidth - (fBorderWidth << 1),
                           fClosingHeight + bh);
         y += fClosingHeight + bh;
      } else {
         child->MoveResize(fBorderWidth, y, fWidth - (fBorderWidth << 1), bh);
         child->HideFrame(child->fCanvas);
         y += bh;
      }
   }
}

//______________________________________________________________________________
void TGShutter::SetSelectedItem(TGShutterItem *item)
{
   // Set item to be the currently open shutter item.

   fSelectedItem = item;
   Layout();
}

//______________________________________________________________________________
TGShutterItem *TGShutter::GetItem(const char *name)
{
   // returns a shutter item by name (name is hot string of shutter item) 

   TGFrameElement *el;
   TGShutterItem  *item = 0;

   TIter next(fList);

   while ((el = (TGFrameElement *) next())) {
      TGTextButton *btn;
      item = (TGShutterItem *)el->fFrame;
      btn = (TGTextButton*)item->GetButton();
      if (btn->GetString() == name) return item;
   }

   return item;
}

//______________________________________________________________________________
TGShutterItem::TGShutterItem(const TGWindow *p, TGHotString *s, Int_t id,
                             UInt_t options) :
   TGVerticalFrame (p, 10, 10, options), TGWidget (id)
{
   // Create a shutter item.

   fButton = new TGTextButton(this, s, id);
   fCanvas = new TGCanvas(this, 10, 10, kChildFrame);
   fContainer = new TGVerticalFrame(fCanvas->GetViewPort(), 10, 10, kOwnBackground);
   fCanvas->SetContainer(fContainer);
   fContainer->SetBackgroundColor(fClient->GetShadow(GetDefaultFrameBackground()));

   AddFrame(fButton, fL1 = new TGLayoutHints(kLHintsTop | kLHintsExpandX));
   AddFrame(fCanvas, fL2 = new TGLayoutHints(kLHintsExpandY | kLHintsExpandX));

   fButton->Associate((TGFrame *) p);
}

//______________________________________________________________________________
TGShutterItem::~TGShutterItem()
{
   // Clan up shutter item.

   if (!MustCleanup()) {
      delete fL1;
      delete fL2;
      delete fButton;
      delete fContainer;
      delete fCanvas;
   }
}

//______________________________________________________________________________
void TGShutterItem::SavePrimitive(ofstream &out, Option_t *option)
{
    // Save a shutter item widget as a C++ statement(s) on output stream out

   char quote = '"';
   TGTextButton *b = (TGTextButton *)fButton;
   const char *text = b->GetText()->GetString();
   char hotpos = b->GetText()->GetHotPos();
   Int_t lentext = b->GetText()->GetLength();
   char *outext = new char[lentext+2];       // should be +2 because of \0
   Int_t i=0;


   while (lentext) {
      if (i == hotpos-1) {
         outext[i] = '&';
         i++;
      }
      outext[i] = *text;
      i++;
      text++;
      lentext--;
   }
   outext[i]=0;

   out << endl;
   out << "   // " << quote << outext << quote << " shutter item " << endl;
   out << "   TGShutterItem *";
   out << GetName() << " = new TGShutterItem(" << fParent->GetName()
       << ", new TGHotString(" << quote << outext << quote << "),"
       << fButton->WidgetId() << "," << GetOptionString() << ");" << endl;

   delete [] outext;

   TList *List = ((TGCompositeFrame *)GetContainer())->GetList();

   if (!List) return;

   out << "   TGCompositeFrame *" << GetContainer()->GetName()
       << " = (TGCompositeFrame *)" << GetName() << "->GetContainer();" << endl;

   TGFrameElement *el;
   TIter next(List);

   while ((el = (TGFrameElement *) next())) {
      el->fFrame->SavePrimitive(out, option);
      out << "   " << GetContainer()->GetName() <<"->AddFrame(" << el->fFrame->GetName();
      el->fLayout->SavePrimitive(out, option);
      out << ");"<< endl;
   }
}

//______________________________________________________________________________
void TGShutter::SavePrimitive(ofstream &out, Option_t *option)
{
    // Save a shutter widget as a C++ statement(s) on output stream out.

   out << endl;
   out << "   // shutter" << endl;

   out << "   TGShutter *";
   out << GetName() << " = new TGShutter(" << fParent->GetName() << ","
       << GetOptionString() << ");" << endl;

   if (!fList) return;

   TGFrameElement *el;
   TIter next(fList);

   while ((el = (TGFrameElement *) next())) {
      el->fFrame->SavePrimitive(out, option);
      out << "   " << GetName() <<"->AddItem(" << el->fFrame->GetName();
      //el->fLayout->SavePrimitive(out, option);
      out << ");"<< endl;
   }

   out << "   " << GetName() << "->SetSelectedItem("
       << GetSelectedItem()->GetName() << ");" << endl;
   out << "   " <<GetName()<< "->Resize("<<GetWidth()<<","<<GetHeight()<<");"<<endl;

}

