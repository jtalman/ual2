// @(#)root/gui:$Name:  $:$Id: TGView.cxx,v 1.12 2004/10/22 07:31:58 rdm Exp $
// Author: Fons Rademakers   30/6/2000

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
// TGView                                                               //
//                                                                      //
// A TGView provides the infrastructure for text viewer and editor      //
// widgets. It provides a canvas (TGViewFrame) and (optionally) a       //
// vertical and horizontal scrollbar and methods for marking and        //
// scrolling.                                                           //
//                                                                      //
// The TGView (and derivatives) will generate the following             //
// event messages:                                                      //
// kC_TEXTVIEW, kTXT_ISMARKED, widget id, [true|false]                  //
// kC_TEXTVIEW, kTXT_DATACHANGE, widget id, 0                           //
// kC_TEXTVIEW, kTXT_CLICK2, widget id, position (y << 16) | x)         //
// kC_TEXTVIEW, kTXT_CLICK3, widget id, position (y << 16) | x)         //
// kC_TEXTVIEW, kTXT_F3, widget id, true                                //
// kC_TEXTVIEW, kTXT_OPEN, widget id, 0                                 //
// kC_TEXTVIEW, kTXT_CLOSE, widget id, 0                                //
// kC_TEXTVIEW, kTXT_SAVE, widget id, 0                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TGView.h"
#include "TGScrollBar.h"
#include "TGResourcePool.h"
#include "TSystem.h"
#include "TMath.h"
#include "KeySymbols.h"



//______________________________________________________________________________
Bool_t TViewTimer::Notify()
{
   fView->HandleTimer(this);
   Reset();
   return kFALSE;
}


ClassImp(TGViewFrame)

//______________________________________________________________________________
TGViewFrame::TGViewFrame(TGView *v, UInt_t w, UInt_t h, UInt_t options,
                         ULong_t back) :
   TGCompositeFrame(v, w, h, options | kOwnBackground, back)
{
   // Create a editor frame.

   fView = v;

   SetBackgroundColor(back);

   gVirtualX->GrabButton(fId, kAnyButton, kAnyModifier,
                         kButtonPressMask | kButtonReleaseMask |
                         kButtonMotionMask, kNone, kNone);

   AddInput(kKeyPressMask | kEnterWindowMask | kLeaveWindowMask |
            kFocusChangeMask | kStructureNotifyMask);
}


ClassImp(TGView)

//______________________________________________________________________________
TGView::TGView(const TGWindow *p, UInt_t w, UInt_t h, Int_t id,
               UInt_t xMargin, UInt_t yMargin, UInt_t options,
               UInt_t sboptions, ULong_t back)
       : TGCompositeFrame(p, w, h, options, GetDefaultFrameBackground())
{
   // Create an editor view, containing an TGEditorFrame and (optionally)
   // a horizontal and vertical scrollbar.

   fWidgetId    = id;
   fMsgWindow   = p;
   fWidgetFlags = kWidgetWantFocus;

   SetLayoutManager(new TGHorizontalLayout(this));

   fXMargin = xMargin;
   fYMargin = yMargin;
   fScrollVal.fX = 1;
   fScrollVal.fY = 1;

   fClipboard = fClient->GetResourcePool()->GetClipboard();

   fCanvas = new TGViewFrame(this, 10, 10, kChildFrame, back);
   AddFrame(fCanvas);

   if (!(sboptions & kNoHSB)) {
      fHsb = new TGHScrollBar(this, 10, 10, kChildFrame);
      AddFrame(fHsb);
      fHsb->Associate(this);
   } else
      fHsb = 0;

   if (!(sboptions & kNoVSB)) {
      fVsb = new TGVScrollBar(this, 10, 10, kChildFrame);
      AddFrame(fVsb);
      fVsb->Associate(this);
   } else
      fVsb = 0;

   fWhiteGC = GetWhiteGC();
   fWhiteGC.SetGraphicsExposures(kTRUE);

   Clear();
   Layout();

   fScrollTimer = new TViewTimer(this, 75);
   gSystem->AddTimer(fScrollTimer);
}

//______________________________________________________________________________
TGView::~TGView()
{
   // Delete view.

   delete fScrollTimer;

   if (!MustCleanup()) {
      delete fCanvas;
      delete fHsb;
      delete fVsb;
   }
}

//______________________________________________________________________________
void TGView::Clear(Option_t *)
{
   // Clear view.

   fIsMarked  = kFALSE;
   fIsSaved   = kTRUE;
   fScrolling = -1;
   fIsMarking = kFALSE;
   fMarkedStart.fX = fMarkedStart.fY = 0;
   fMarkedEnd.fX   = fMarkedEnd.fY   = 0;
   fMousePos.fX    = fMousePos.fY    = -1;
   fVisible.fX     = fVisible.fY     = 0;
   SetVsbPosition(0);
   SetHsbPosition(0);
}

//______________________________________________________________________________
void TGView::SetVisibleStart(Int_t newTop, Int_t direction)
{
   // Scroll view in specified direction to make newTop the visible location.

   if (direction == kHorizontal) {
      if (newTop / fScrollVal.fX == fVisible.fX / fScrollVal.fX)
         return;
      ScrollCanvas(newTop, kHorizontal);
   } else {
      if (newTop / fScrollVal.fY == fVisible.fY / fScrollVal.fY)
         return;
      ScrollCanvas(newTop, kVertical);
   }
}

//______________________________________________________________________________
Bool_t TGView::HandleExpose(Event_t *event)
{
   // Handle expose events.

   if (event->fWindow == fCanvas->GetId()) {
      DrawRegion(event->fX, event->fY, event->fWidth, event->fHeight);
   } else
      TGCompositeFrame::HandleExpose(event);

   return kTRUE;
}

//______________________________________________________________________________
Bool_t TGView::HandleCrossing(Event_t *event)
{
   // Handle mouse crossing event.

   if (event->fWindow != fCanvas->GetId())
      return kTRUE;

   fMousePos.fY = ToObjYCoord(fVisible.fY + event->fY);
   if (ToScrYCoord(fMousePos.fY+1) >= (Int_t)fCanvas->GetHeight())
      fMousePos.fY--;
   fMousePos.fX = ToObjXCoord(fVisible.fX + event->fX, fMousePos.fY);
   if (fMousePos.fX >= ReturnLineLength(fMousePos.fY))
      fMousePos.fX--;
   if ((event->fState & kButton1Mask) && fIsMarked && fIsMarking) {
      if (event->fType == kLeaveNotify) {
         if (event->fX < 0) {
            fScrolling = 0;
            return kFALSE;
         }
         if (event->fX >= (Int_t)fCanvas->GetWidth()) {
            fScrolling = 1;
            return kFALSE;
         }
         if (event->fY < 0) {
            fScrolling = 2;
            return kFALSE;
         }
         if (event->fY >= (Int_t)fCanvas->GetHeight()) {
            fScrolling = 3;
            return kFALSE;
         }
      } else {
         fScrolling = -1;
         Mark(fMousePos.fX, fMousePos.fY);
      }
   } else
      fIsMarking = kFALSE;

   return kTRUE;
}

//______________________________________________________________________________
Bool_t TGView::HandleTimer(TTimer *)
{
   // Handle scroll timer.

   TGLongPosition size;

   if (fIsMarked) {
      size.fY = ToObjYCoord(fVisible.fY + fCanvas->GetHeight()) - 1;
      size.fX = ToObjXCoord(fVisible.fX + fCanvas->GetWidth(), fMousePos.fY) - 1;
      switch (fScrolling) {
         case -1:
            break;
         case 0:
            if (fVisible.fX == 0) {
               fScrolling = -1;
               break;
            } else {
               SetHsbPosition(fVisible.fX / fScrollVal.fX - 1);
               Mark(ToObjXCoord(fVisible.fX, fMousePos.fY) - 1, fMousePos.fY);
            }
            break;
         case 1:
            if ((Int_t)fCanvas->GetWidth() >= ToScrXCoord(ReturnLineLength(fMousePos.fY), fMousePos.fY)) {
               fScrolling = -1;
               break;
            } else {
               SetHsbPosition(fVisible.fX / fScrollVal.fX + 1);
               Mark(size.fX+1, fMousePos.fY);
            }
            break;
         case 2:
            if (fVisible.fY == 0) {
               fScrolling = -1;
               break;
            } else {
               SetVsbPosition(fVisible.fY / fScrollVal.fY - 1);
               Mark(fMousePos.fX, fMarkedStart.fY - 1);
            }
            break;
         case 3:
            if ((Int_t)fCanvas->GetHeight() >= ToScrYCoord(ReturnLineCount())) {
               fScrolling = -1;
               break;
            } else {
               SetVsbPosition(fVisible.fY / fScrollVal.fY + 1);
               Mark(fMousePos.fX, size.fY + 1);
            }
            break;
         default:
            break;
      }
   }
   return kTRUE;
}

//______________________________________________________________________________
Bool_t TGView::HandleButton(Event_t *event)
{
   // Handle mouse button event in text editor.

   if (event->fWindow != fCanvas->GetId())
      return kTRUE;

   if (event->fCode == kButton1) {
      if (event->fType == kButtonPress) {
         if (fIsMarked) {
            if (event->fState & kKeyShiftMask) {
               fIsMarking = kTRUE;
               TGView::HandleMotion(event);
               return kTRUE;
            }
            fIsMarked = kFALSE;
            UnMark();
         }
         fIsMarked = kTRUE;
         fIsMarking = kTRUE;
         fMousePos.fY = ToObjYCoord(fVisible.fY + event->fY);
         fMousePos.fX = ToObjXCoord(fVisible.fX + event->fX, fMousePos.fY);
         fMarkedStart.fX = fMarkedEnd.fX = fMousePos.fX;
         fMarkedStart.fY = fMarkedEnd.fY = fMousePos.fY;
      } else {
         fScrolling = -1;
         if ((fMarkedStart.fX == fMarkedEnd.fX) &&
             (fMarkedStart.fY == fMarkedEnd.fY)) {
            fIsMarked = kFALSE;
            SendMessage(fMsgWindow, MK_MSG(kC_TEXTVIEW, kTXT_ISMARKED),
                        fWidgetId, kFALSE);
            Marked(kFALSE);
         } else {
            SendMessage(fMsgWindow, MK_MSG(kC_TEXTVIEW, kTXT_ISMARKED),
                        fWidgetId, kTRUE);
            Marked(kTRUE);
         }
         fIsMarking = kFALSE;
      }
   } else if (event->fCode == kButton4) {
      // move three lines up
      if (fVisible.fY > 0) {
         SetVsbPosition(fVisible.fY / fScrollVal.fY - 3);
         Mark(fMousePos.fX, fMarkedStart.fY - 3);
      }
   } else if (event->fCode == kButton5) {
      // move three lines down
      if ((Int_t)fCanvas->GetHeight() < ToScrYCoord(ReturnLineCount())) {
         TGLongPosition size;
         size.fY = ToObjYCoord(fVisible.fY + fCanvas->GetHeight()) - 1;
         SetVsbPosition(fVisible.fY / fScrollVal.fY + 3);
         Mark(fMousePos.fX, size.fY + 3);
      }
   } else if (event->fType == kButtonPress) {
      if (event->fCode == kButton2) {
         SendMessage(fMsgWindow, MK_MSG(kC_TEXTVIEW, kTXT_CLICK2),
                     fWidgetId, (event->fYRoot << 16) | event->fXRoot);
      } else if (event->fCode == kButton3) {
         SendMessage(fMsgWindow, MK_MSG(kC_TEXTVIEW, kTXT_CLICK3),
                     fWidgetId, (event->fYRoot << 16) | event->fXRoot);
      }
   }
   return kTRUE;
}

//______________________________________________________________________________
Bool_t TGView::HandleMotion(Event_t *event)
{
   // Handle mouse motion event in the text editor widget.

   if ((ToObjYCoord(fVisible.fY+event->fY) == fMousePos.fY) &&
       (ToObjXCoord(fVisible.fX+event->fX, ToObjYCoord(fVisible.fY+event->fY)) == fMousePos.fX))
      return kTRUE;

   if (fScrolling != -1)
      return kTRUE;

   fMousePos.fY = ToObjYCoord(fVisible.fY+event->fY);
   if (fMousePos.fY >= ReturnLineCount())
      fMousePos.fY = ReturnLineCount()-1;
   fMousePos.fX = ToObjXCoord(fVisible.fX+event->fX, fMousePos.fY);
   if (fMousePos.fX > ReturnLineLength(fMousePos.fY))
      fMousePos.fX = ReturnLineLength(fMousePos.fY);
   if (event->fWindow != fCanvas->GetId())
      return kTRUE;
   if (!fIsMarked || !fIsMarking)
      return kTRUE;
   if (event->fX < 0)
      return kTRUE;
   if (event->fX >= (Int_t)fCanvas->GetWidth())
      return kTRUE;
   if (event->fY < 0)
      return kTRUE;
   if (event->fY >= (Int_t)fCanvas->GetHeight())
      return kTRUE;
   Mark(fMousePos.fX, fMousePos.fY);
   return kTRUE;
}

//______________________________________________________________________________
Bool_t TGView::ProcessMessage(Long_t msg, Long_t parm1, Long_t)
{
   // Process scrollbar messages.

   switch(GET_MSG(msg)) {
      case kC_HSCROLL:
         switch(GET_SUBMSG(msg)) {
            case kSB_SLIDERTRACK:
            case kSB_SLIDERPOS:
               SetVisibleStart(Int_t(parm1 * fScrollVal.fX), kHorizontal);
               break;
         }
         break;

      case kC_VSCROLL:
         switch(GET_SUBMSG(msg)) {
            case kSB_SLIDERTRACK:
            case kSB_SLIDERPOS:
               SetVisibleStart(Int_t(parm1 * fScrollVal.fY), kVertical);
               break;
         }
         break;

      default:
         break;
   }
   return kTRUE;
}

//______________________________________________________________________________
void TGView::Layout()
{
   // Layout the components of view.

   HLayout();
   VLayout();
}

//______________________________________________________________________________
void TGView::HLayout()
{
   // Horizontal layout of widgets (canvas, scrollbar).

   if (!fHsb) return;

   Int_t tcw, tch;
   Long_t cols;
   tch = fHeight - (fBorderWidth << 1) - fYMargin-1;
   tcw = fWidth - (fBorderWidth << 1) - fXMargin-1;
   if (fVsb && fVsb->IsMapped()) {
      tcw -= fVsb->GetDefaultWidth();
      if (tcw < 0) tcw = 0;
   }
   fCanvas->SetHeight(tch);
   fCanvas->SetWidth(tcw);
   cols = ReturnLongestLineWidth();
   if (cols <= tcw) {
      if (fHsb && fHsb->IsMapped()) {
         SetVisibleStart(0, kHorizontal);
         fHsb->UnmapWindow();
         VLayout();
      }
      fCanvas->MoveResize(fBorderWidth + fXMargin, fBorderWidth + fYMargin, tcw, tch);
   } else {
      if (fHsb) {
         tch -= fHsb->GetDefaultHeight();
         if (tch < 0) tch = 0;
         fHsb->MoveResize(fBorderWidth, fHeight - fHsb->GetDefaultHeight()-fBorderWidth,
                          tcw+1+fBorderWidth, fHsb->GetDefaultHeight());
         fHsb->MapWindow();
         fHsb->SetRange(Int_t(cols/fScrollVal.fX), Int_t(tcw/fScrollVal.fX));
      }
      fCanvas->MoveResize(fBorderWidth + fXMargin, fBorderWidth + fYMargin, tcw, tch);
   }
}

//______________________________________________________________________________
void TGView::VLayout()
{
   // Vertical layout of widgets (canvas, scrollbar).

   Int_t  tcw, tch;
   Long_t lines;

   tch = fHeight - (fBorderWidth << 1) - fYMargin-1;
   tcw = fWidth - (fBorderWidth << 1) - fXMargin-1;
   if (fHsb && fHsb->IsMapped()) {
      tch -= fHsb->GetDefaultHeight();
      if (tch < 0) tch = 0;
   }
   fCanvas->SetHeight(tch);
   fCanvas->SetWidth(tcw);
   lines = ReturnHeighestColHeight();
   if (lines <= tch) {
      if (fVsb && fVsb->IsMapped()) {
         SetVisibleStart(0, kVertical);
         fVsb->UnmapWindow();
         HLayout();
      }
      fCanvas->MoveResize(fBorderWidth + fXMargin, fBorderWidth + fYMargin, tcw, tch);
   } else {
      if (fVsb) {
         tcw -= fVsb->GetDefaultWidth();
         if (tcw < 0) tcw = 0;
         fVsb->MoveResize(fWidth - fVsb->GetDefaultWidth() - fBorderWidth, fBorderWidth,
                          fVsb->GetDefaultWidth(), tch+1+fBorderWidth);
         fVsb->MapWindow();
         fVsb->SetRange(Int_t(lines/fScrollVal.fY), Int_t(tch/fScrollVal.fY));
      }
      fCanvas->MoveResize(fBorderWidth + fXMargin, fBorderWidth + fYMargin, tcw, tch);
   }
}

//______________________________________________________________________________
void TGView::SetSBRange(Int_t direction)
{
   // Set the range for the kVertical or kHorizontal scrollbar.

   if (direction == kVertical) {
      if (!fVsb)
         return;
      if (ReturnHeighestColHeight() <= (Int_t)fCanvas->GetHeight()) {
         if (fVsb->IsMapped())
            VLayout();
         else
            return;
      }
      if (!fVsb->IsMapped())
         VLayout();
      fVsb->SetRange(Int_t(ReturnHeighestColHeight()/fScrollVal.fY),
                     Int_t(fCanvas->GetHeight()/fScrollVal.fY));
   } else {
      if (!fHsb)
         return;
      if (ReturnLongestLineWidth() <= (Int_t)fCanvas->GetWidth()) {
         if (fHsb->IsMapped())
            HLayout();
         else
            return;
      }
      if (!fHsb->IsMapped())
         HLayout();
      fHsb->SetRange(Int_t(ReturnLongestLineWidth()/fScrollVal.fX),
                     Int_t(fCanvas->GetWidth()/fScrollVal.fX));
   }
}

//______________________________________________________________________________
void TGView::SetHsbPosition(Long_t newPos)
{
   // Set position of horizontal scrollbar.

   if (fHsb && fHsb->IsMapped())
     fHsb->SetPosition(Int_t(newPos));
   else
     SetVisibleStart(Int_t(newPos * fScrollVal.fX), kHorizontal);
}

//______________________________________________________________________________
void TGView::SetVsbPosition(Long_t newPos)
{
   // Set position of vertical scrollbar.

   if (fVsb && fVsb->IsMapped())
      fVsb->SetPosition(Int_t(newPos));
   else
      SetVisibleStart(Int_t(newPos * fScrollVal.fY), kVertical);
}

//______________________________________________________________________________
void TGView::DrawBorder()
{
   // Draw the border of the text edit widget.

   switch (fOptions & (kSunkenFrame | kRaisedFrame | kDoubleBorder)) {
      case kSunkenFrame | kDoubleBorder:
         gVirtualX->DrawLine(fId, GetShadowGC()(), 0, 0, fWidth-2, 0);
         gVirtualX->DrawLine(fId, GetShadowGC()(), 0, 0, 0, fHeight-2);
         gVirtualX->DrawLine(fId, GetBlackGC()(), 1, 1, fWidth-3, 1);
         gVirtualX->DrawLine(fId, GetBlackGC()(), 1, 1, 1, fHeight-3);

         gVirtualX->DrawLine(fId, GetHilightGC()(), 0, fHeight-1, fWidth-1, fHeight-1);
         gVirtualX->DrawLine(fId, GetHilightGC()(), fWidth-1, fHeight-1, fWidth-1, 0);
         gVirtualX->DrawLine(fId, GetBckgndGC()(),  1, fHeight-2, fWidth-2, fHeight-2);
         gVirtualX->DrawLine(fId, GetBckgndGC()(),  fWidth-2, 1, fWidth-2, fHeight-2);
         break;

      default:
         TGFrame::DrawBorder();
         break;
   }
}

//______________________________________________________________________________
void TGView::ScrollCanvas(Int_t new_top, Int_t direction)
{
   // Scroll the canvas to new_top in the kVertical or kHorizontal direction.

   Point_t points[4];
   Int_t xsrc, ysrc, xdest, ydest, cpyheight, cpywidth;

   if (direction == kVertical) {
      points[0].fX = points[3].fX = 0;
      points[1].fX = points[2].fX = fCanvas->GetWidth();
      xsrc = xdest = 0;
      cpywidth = 0;
      if (new_top < fVisible.fY) {
         ysrc = 0;
         ydest = Int_t(fVisible.fY - new_top);
         cpyheight = ydest;
         if (ydest > (Int_t)fCanvas->GetHeight())
            ydest = fCanvas->GetHeight();
         points[1].fY = points[0].fY = 0;
         points[3].fY = points[2].fY = ydest ; // -1;
      } else {
         ydest = 0;
         ysrc = Int_t(new_top - fVisible.fY);
         cpyheight= ysrc;
         if (ysrc > (Int_t)fCanvas->GetHeight())
            ysrc = fCanvas->GetHeight();
         points[1].fY = points[0].fY = fCanvas->GetHeight()-ysrc; // +1;
         points[3].fY = points[2].fY = fCanvas->GetHeight();
      }
      fVisible.fY = new_top;
      if (fVisible.fY < 0)
         fVisible.fY = 0;
   } else {
      points[0].fY = points[1].fY = 0;
      points[2].fY = points[3].fY = fCanvas->GetHeight();
      ysrc = ydest = 0;
      cpyheight = 0;
      if (new_top < fVisible.fX) {
         xsrc = 0;
         xdest = Int_t(fVisible.fX - new_top);
         cpywidth = xdest;
         if (xdest < 0)
            xdest = fCanvas->GetWidth();
         points[0].fX = points[3].fX = 0;
         points[1].fX = points[2].fX = xdest ; // -1;
      } else {
         xdest = 0;
         xsrc =  Int_t(new_top - fVisible.fX);
         cpywidth = xsrc;
         if (xsrc > (Int_t)fCanvas->GetWidth())
            xsrc = fCanvas->GetWidth();
         points[0].fX = points[3].fX = fCanvas->GetWidth()-xsrc; // +1;
         points[1].fX = points[2].fX = fCanvas->GetWidth();
      }
      fVisible.fX = new_top;
      if (fVisible.fX < 0)
         fVisible.fX = 0;
   }
   // Copy the scrolled region to its new position
   gVirtualX->CopyArea(fCanvas->GetId(), fCanvas->GetId(), fWhiteGC(),
                       xsrc, ysrc, fCanvas->GetWidth()-cpywidth,
                       fCanvas->GetHeight()-cpyheight, xdest, ydest);
   // Clear the remaining area of any old text
   gVirtualX->ClearArea(fCanvas->GetId(), points[0].fX, points[0].fY,
                        points[2].fX - points[0].fX,
                        points[2].fY - points[0].fY);

   DrawRegion(points[0].fX, points[0].fY,
              points[2].fX - points[0].fX, points[2].fY - points[0].fY);
}
