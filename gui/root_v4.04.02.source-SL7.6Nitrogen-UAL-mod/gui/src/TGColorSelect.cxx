// @(#)root/gui:$Name:  $:$Id: TGColorSelect.cxx,v 1.9 2004/10/18 12:32:12 brun Exp $
// Author: Bertrand Bellenot + Fons Rademakers   22/08/02

/*************************************************************************
 * Copyright (C) 1995-2002, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/
/**************************************************************************

    This file is part of xclass.
    Copyright (C) 2000, 2001, Hector Peraza.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

**************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGColorFrame, TG16ColorSelector, TGColorPopup and TGColorSelect.     //
//                                                                      //
// The TGColorFrame is a small frame with border showing a specific     //
// color.                                                               //
//                                                                      //
// The TG16ColorSelector is a composite frame with 16 TGColorFrames.    //
//                                                                      //
// The TGColorPopup is a popup containing a TG16ColorSelector and a     //
// "More..." button which popups up a TGColorDialog allowing custom     //
// color selection.                                                     //
//                                                                      //
// The TGColorSelect widget is like a checkbutton but instead of the    //
// check mark there is color area with a little down arrow. When        //
// clicked on the arrow the TGColorPopup pops up.                       //
//                                                                      //
// Selecting a color in this widget will generate the event:            //
// kC_COLORSEL, kCOL_SELCHANGED, widget id, pixel.                      //
// and the signal:                                                      //
// ColorSelected(Pixel_t color)                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TGClient.h"
#include "TGMsgBox.h"
#include "TGGC.h"
#include "TGColorSelect.h"
#include "TGColorDialog.h"
#include "TGResourcePool.h"
#include "TG3DLine.h"
#include "TColor.h"
#include "Riostream.h"

ClassImp(TGColorFrame)
ClassImp(TG16ColorSelector)
ClassImp(TGColorPopup)
ClassImp(TGColorSelect)


//________________________________________________________________________________
TGColorFrame::TGColorFrame(const TGWindow *p, ULong_t color, Int_t /*n*/) :
   TGFrame(p, 20, 20, kOwnBackground, color)
{
   SetBackgroundColor(color);

   fColor = color;

   AddInput(kButtonPressMask | kButtonReleaseMask);
   fMsgWindow  = p;
   fActive = kFALSE;

   fGrayGC = GetShadowGC()();
}

//________________________________________________________________________________
Bool_t TGColorFrame::HandleButton(Event_t *event)
{
   if (event->fType == kButtonPress) {
      SendMessage(fMsgWindow, MK_MSG(kC_COLORSEL, kCOL_CLICK), event->fCode, fColor);
   } else {    // kButtonRelease
      SendMessage(fMsgWindow, MK_MSG(kC_COLORSEL, kCOL_SELCHANGED), event->fCode, fColor);
   }

   return kTRUE;
}

//________________________________________________________________________________
void TGColorFrame::DrawBorder()
{
   gVirtualX->DrawRectangle(fId, GetBckgndGC()(), 0, 0, fWidth - 1, fHeight - 1);
   Draw3dRectangle(kDoubleBorder | kSunkenFrame, 1, 1, fWidth - 2, fHeight - 2);
}

//________________________________________________________________________________
TG16ColorSelector::TG16ColorSelector(const TGWindow *p) :
   TGCompositeFrame(p, 10, 10)
{
   SetLayoutManager(new TGMatrixLayout(this, 4, 4, 1, 1));

   fCe[0]  = new TGColorFrame(this, TColor::Number2Pixel(0), 0);
   fCe[1]  = new TGColorFrame(this, TColor::Number2Pixel(1), 1);
   fCe[2]  = new TGColorFrame(this, TColor::Number2Pixel(2), 2);
   fCe[3]  = new TGColorFrame(this, TColor::Number2Pixel(3), 3);
   fCe[4]  = new TGColorFrame(this, TColor::Number2Pixel(4), 4);
   fCe[5]  = new TGColorFrame(this, TColor::Number2Pixel(5), 5);
   fCe[6]  = new TGColorFrame(this, TColor::Number2Pixel(6), 6);
   fCe[7]  = new TGColorFrame(this, TColor::Number2Pixel(7), 7);
   fCe[8]  = new TGColorFrame(this, TColor::Number2Pixel(8), 8);
   fCe[9]  = new TGColorFrame(this, TColor::Number2Pixel(9), 9);
   fCe[10] = new TGColorFrame(this, TColor::RGB2Pixel(0xDD, 0xBA, 0x87), 10);
   fCe[11] = new TGColorFrame(this, TColor::RGB2Pixel(0xBC, 0x9E, 0x82), 11);
   fCe[12] = new TGColorFrame(this, TColor::RGB2Pixel(0x84, 0xC1, 0xA3), 12);
   fCe[13] = new TGColorFrame(this, TColor::RGB2Pixel(0xC0, 0xB6, 0xAC), 13);
   fCe[14] = new TGColorFrame(this, TColor::RGB2Pixel(0x66, 0x66, 0x66), 14);
   fCe[15] = new TGColorFrame(this, TColor::RGB2Pixel(0x99, 0x99, 0x99), 15);


   for (Int_t i = 0; i < 16; i++)
      AddFrame(fCe[i], new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));

   fMsgWindow  = p;
   fActive = -1;
}

//________________________________________________________________________________
TG16ColorSelector::~TG16ColorSelector()
{
   Cleanup();
}

//________________________________________________________________________________
void TG16ColorSelector::SetActive(Int_t newat)
{
   if (fActive != newat) {
      if ((fActive >= 0) && (fActive < 16)) {
         fCe[fActive]->SetActive(kFALSE);
      }
      fActive = newat;
      if ((fActive >= 0) && (fActive < 16)) {
         fCe[fActive]->SetActive(kTRUE);
      }
   }
}

//________________________________________________________________________________
Bool_t TG16ColorSelector::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
   switch (GET_MSG(msg)) {
      case kC_COLORSEL:
         switch (GET_SUBMSG(msg)) {
            case kCOL_SELCHANGED:
               switch (parm1) {
                  case kButton1:
                     SendMessage(fMsgWindow,
                                 MK_MSG(kC_COLORSEL, kCOL_SELCHANGED),
                                 parm1, parm2);
                     break;
               }
               break;

            case kCOL_CLICK:
               switch (parm1) {
                  case kButton1:
                     SetActive(parm2);
                     break;
               }
               break;
         }
   }

   return kTRUE;
}

//________________________________________________________________________________
TGColorPopup::TGColorPopup(const TGWindow *p, const TGWindow *m, ULong_t color) :
   TGCompositeFrame(p, 10, 10, kDoubleBorder | kRaisedFrame | kOwnBackground,
                    GetDefaultFrameBackground())
{
   fMsgWindow = m;
   fCurrentColor = color;

   SetWindowAttributes_t wattr;

   wattr.fMask = kWAOverrideRedirect;  // | kWASaveUnder ;
   wattr.fOverrideRedirect = kTRUE;
   //wattr.fSaveUnder = kTRUE;
   gVirtualX->ChangeWindowAttributes(fId, &wattr);

   AddInput(kStructureNotifyMask);

   fActive = -1;
   fLaunchDialog = kFALSE;

   TG16ColorSelector *cs = new TG16ColorSelector(this);
   AddFrame(cs, new TGLayoutHints(kLHintsCenterX, 1, 1, 1, 1));
   AddFrame(new TGHorizontal3DLine(this),
            new TGLayoutHints(kLHintsExpandX | kLHintsCenterY, 2, 2, 2, 2));
   TGTextButton *other = new TGTextButton(this, "Other...", 102);
   other->SetToolTipText("Popups up Color Selector");
   other->Associate(this);
   AddFrame(other, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 2, 2, 2, 2));

   MapSubwindows();

   Resize(cs->GetDefaultWidth() + 6, cs->GetDefaultHeight() +
          other->GetDefaultHeight());
}

//________________________________________________________________________________
TGColorPopup::~TGColorPopup()
{
   Cleanup();
}

//________________________________________________________________________________
void TGColorPopup::EndPopup()
{
   gVirtualX->GrabPointer(0, 0, 0, 0, kFALSE);  // ungrab pointer
   UnmapWindow();
}

//________________________________________________________________________________
void TGColorPopup::PlacePopup(Int_t x, Int_t y, UInt_t w, UInt_t h)
{
   Int_t rx, ry;
   UInt_t rw, rh;

   // Parent is root window for the popup:
   gVirtualX->GetWindowSize(fParent->GetId(), rx, ry, rw, rh);

   if (x < 0) x = 0;
   if (x + fWidth > rw) x = rw - fWidth;
   if (y < 0) y = 0;
   if (y + fHeight > rh) y = rh - fHeight;

   MoveResize(x, y, w, h);
   MapSubwindows();
   Layout();
   MapRaised();

   gVirtualX->GrabPointer(fId, kButtonPressMask | kButtonReleaseMask |
                          kPointerMotionMask, kNone, kNone);

   fLaunchDialog = kFALSE;

   gClient->WaitForUnmap(this);
   EndPopup();

   if (fLaunchDialog) {
      Int_t retc;
      ULong_t color = fCurrentColor;

      new TGColorDialog(gClient->GetDefaultRoot(), this, &retc, &color);

      if (retc == kMBOk) {
         fCurrentColor = color;
         SendMessage(fMsgWindow, MK_MSG(kC_COLORSEL, kCOL_SELCHANGED),
                     -1, color);
      }
   }
}

//________________________________________________________________________________
Bool_t TGColorPopup::HandleButton(Event_t *event)
{
   if (event->fX < 0 || event->fX >= (Int_t) fWidth ||
       event->fY < 0 || event->fY >= (Int_t) fHeight) {
      if (event->fType == kButtonRelease)
         UnmapWindow();
   } else {
      TGFrame *f = GetFrameFromPoint(event->fX, event->fY);
      if (f && f != this) {
         TranslateCoordinates(f, event->fX, event->fY, event->fX, event->fY);
         f->HandleButton(event);
      }
   }
   return kTRUE;
}

//________________________________________________________________________________
Bool_t TGColorPopup::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
   switch (GET_MSG(msg)) {
      case kC_COLORSEL:
         switch (GET_SUBMSG(msg)) {
            case kCOL_SELCHANGED:
               SendMessage(fMsgWindow, MK_MSG(kC_COLORSEL, kCOL_SELCHANGED),
                           parm1, parm2);
               UnmapWindow();
               break;

            default:
               break;
         }
         break;

      case kC_COMMAND:
         switch (GET_SUBMSG(msg)) {
            case kCM_BUTTON:
               if (parm1 == 102) {
                  fLaunchDialog = kTRUE;
                  UnmapWindow();
               }
               break;
         }
         break;
   }
   return kTRUE;
}

//________________________________________________________________________________
TGColorSelect::TGColorSelect(const TGWindow *p, ULong_t color, Int_t id) :
   TGCheckButton(p, "", id)
{
   fColor = color;
   fColorPopup = 0;
   fDrawGC = *fClient->GetResourcePool()->GetFrameGC();

   Enable();
   SetState(kButtonUp);
   AddInput(kButtonPressMask | kButtonReleaseMask);
   SetColor(fColor);
}

//________________________________________________________________________________
TGColorSelect::~TGColorSelect()
{
   delete fColorPopup;
}

//________________________________________________________________________________
Bool_t TGColorSelect::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
   switch (GET_MSG(msg)) {
      case kC_COLORSEL:
         switch (GET_SUBMSG(msg)) {
            case kCOL_SELCHANGED:
               {
                  SetColor(parm2);
                  SendMessage(fMsgWindow, MK_MSG(kC_COLORSEL, kCOL_SELCHANGED),
                              parm1, parm2);
               }
               break;

            default:
               break;
         }
         break;
   }
   return kTRUE;
}

//________________________________________________________________________________
Bool_t TGColorSelect::HandleButton(Event_t *event)
{
   TGFrame::HandleButton(event);
   if (!IsEnabled()) return kTRUE;

   if (event->fCode != kButton1) return kFALSE;

   if ((event->fType == kButtonPress) && HasFocus())
      WantFocus();

   if (event->fType == kButtonPress) {
      if (fState != kButtonDown) {
         fPrevState = fState;
         SetState(kButtonDown);
       }
   } else {
      if (fState != kButtonUp) {
         SetState(kButtonUp);

         Window_t wdummy;
         Int_t ax, ay;

         if (!fColorPopup)
            fColorPopup = new TGColorPopup(gClient->GetDefaultRoot(), this, fColor);

         gVirtualX->TranslateCoordinates(fId, gClient->GetDefaultRoot()->GetId(),
                                         0, fHeight, ax, ay, wdummy);

         fColorPopup->PlacePopup(ax, ay, fColorPopup->GetDefaultWidth(),
                                         fColorPopup->GetDefaultHeight());
      }
   }
   return kTRUE;
}

//______________________________________________________________________________
void TGColorSelect::Enable()
{
   // Set state of widget. If kTRUE=enabled, kFALSE=disabled.

   SetFlags(kWidgetIsEnabled);
   fClient->NeedRedraw(this);
}

//______________________________________________________________________________
void TGColorSelect::Disable()
{
   // Set state of widget. If kTRUE=enabled, kFALSE=disabled.

   ClearFlags(kWidgetIsEnabled);
   fClient->NeedRedraw(this);
}

//________________________________________________________________________________
void TGColorSelect::DoRedraw()
{
   Int_t  x, y;
   UInt_t w, h;

   TGButton::DoRedraw();

   if (IsEnabled()) {

      // color rectangle

      x = fBorderWidth + 2;
      y = fBorderWidth + 2;  // 1;
      w = 22;
      h = fHeight - (fBorderWidth * 2) - 4;  // -3;  // 14

      if (fState == kButtonDown) { ++x; ++y; }

      gVirtualX->DrawRectangle(fId, GetShadowGC()(), x, y, w - 1, h - 1);
      gVirtualX->FillRectangle(fId, fDrawGC(), x + 1, y + 1, w - 2, h - 2);

      // separator

      x = fWidth - 6 - fBorderWidth - 6;
      y = fBorderWidth + 1;
      h = fHeight - fBorderWidth - 1;  // actually y1

      if (fState == kButtonDown) { ++x; ++y; }

      gVirtualX->DrawLine(fId, GetShadowGC()(),  x, y, x, h - 2);
      gVirtualX->DrawLine(fId, GetHilightGC()(), x + 1, y, x + 1, h - 1);
      gVirtualX->DrawLine(fId, GetHilightGC()(), x, h - 1, x + 1, h - 1);

      // arrow

      x = fWidth - 6 - fBorderWidth - 2;
      y = (fHeight - 4) / 2 + 1;

      if (fState == kButtonDown) { ++x; ++y; }

      DrawTriangle(GetShadowGC()(), x, y);

   } else {

      // sunken rectangle

      x = fBorderWidth + 2;
      y = fBorderWidth + 2;  // 1;
      w = 22;
      h = fHeight - (fBorderWidth * 2) - 4;  // 3;

      Draw3dRectangle(kSunkenFrame, x, y, w, h);

      // separator

      x = fWidth - 6 - fBorderWidth - 6;
      y = fBorderWidth + 1;
      h = fHeight - fBorderWidth - 1;  // actually y1

      gVirtualX->DrawLine(fId, GetShadowGC()(),  x, y, x, h - 2);
      gVirtualX->DrawLine(fId, GetHilightGC()(), x + 1, y, x + 1, h - 1);
      gVirtualX->DrawLine(fId, GetHilightGC()(), x, h - 1, x + 1, h - 1);

      // sunken arrow

      x = fWidth - 6 - fBorderWidth - 2;
      y = (fHeight - 4) / 2 + 1;

      DrawTriangle(GetHilightGC()(), x + 1, y + 1);
      DrawTriangle(GetShadowGC()(), x, y);
   }
}

//________________________________________________________________________________
void TGColorSelect::DrawTriangle(GContext_t gc, Int_t x, Int_t y)
{
   Point_t points[3];

   points[0].fX = x;
   points[0].fY = y;
   points[1].fX = x + 5;
   points[1].fY = y;
   points[2].fX = x + 2;
   points[2].fY = y + 3;

   gVirtualX->FillPolygon(fId, gc, points, 3);
}

//________________________________________________________________________________
void TGColorSelect::SetColor(ULong_t color)
{
   // Set color.

   fColor = color;
   fDrawGC.SetForeground(color);
   gClient->NeedRedraw(this);
   ColorSelected(fColor);   // emit a signal
}

//______________________________________________________________________________
void TGColorSelect::SavePrimitive(ofstream &out, Option_t *)
{
    // Save a color select widget as a C++ statement(s) on output stream out

   char quote = '"';
   ULong_t color = GetColor();
   const char *colorname = TColor::PixelAsHexString(color);
   gClient->GetColorByName(colorname, color);

   out << endl << "   // color select widget" << endl;
   out << "   ULong_t ColPar;" << endl;
   out << "   gClient->GetColorByName(" << quote << colorname << quote
       << ", ColPar);" << endl;

   out <<"   TGColorSelect *";
   out << GetName() << " = new TGColorSelect(" << fParent->GetName()
       << ", ColPar, " << WidgetId() << ");" << endl;

   if (!IsEnabled()) {
      out << "   " << GetName() << "->Disable();" << endl;
   }
   out << endl;
}
