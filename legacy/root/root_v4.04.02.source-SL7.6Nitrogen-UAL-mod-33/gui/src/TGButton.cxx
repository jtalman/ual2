// @(#)root/gui:$Name:  $:$Id: TGButton.cxx,v 1.53 2005/02/08 13:34:07 brun Exp $
// Author: Fons Rademakers   06/01/98

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
// TGButton, TGTextButton, TGPictureButton, TGCheckButton and           //
// TGRadioButton                                                        //
//                                                                      //
// This header defines all GUI button widgets.                          //
//                                                                      //
// TGButton is a button abstract base class. It defines general button  //
// behaviour.                                                           //
//                                                                      //
// Selecting a text or picture button will generate the event:          //
// kC_COMMAND, kCM_BUTTON, button id, user data.                        //
//                                                                      //
// Selecting a check button will generate the event:                    //
// kC_COMMAND, kCM_CHECKBUTTON, button id, user data.                   //
//                                                                      //
// Selecting a radio button will generate the event:                    //
// kC_COMMAND, kCM_RADIOBUTTON, button id, user data.                   //
//                                                                      //
// If a command string has been specified (via SetCommand()) then this  //
// command string will be executed via the interpreter whenever a       //
// button is selected. A command string can contain the macros:         //
// $MSG   -- kC_COMMAND, kCM[CHECK|RADIO]BUTTON packed message          //
//           (use GET_MSG() and GET_SUBMSG() to unpack)                 //
// $PARM1 -- button id                                                  //
// $PARM2 -- user data pointer                                          //
// Before executing these macros are expanded into the respective       //
// Long_t's                                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TGButton.h"
#include "TGWidget.h"
#include "TGPicture.h"
#include "TGToolTip.h"
#include "TGButtonGroup.h"
#include "TGResourcePool.h"
#include "Riostream.h"
#include "TSystem.h"


const TGGC *TGButton::fgHibckgndGC = 0;
const TGGC *TGButton::fgDefaultGC = 0;

const TGFont *TGTextButton::fgDefaultFont = 0;

const TGFont *TGCheckButton::fgDefaultFont = 0;
const TGGC   *TGCheckButton::fgDefaultGC = 0;

const TGFont *TGRadioButton::fgDefaultFont = 0;
const TGGC   *TGRadioButton::fgDefaultGC = 0;

Window_t TGButton::fgReleaseBtn = 0;

ClassImp(TGButton)
ClassImp(TGTextButton)
ClassImp(TGPictureButton)
ClassImp(TGCheckButton)
ClassImp(TGRadioButton)


//______________________________________________________________________________
TGButton::TGButton(const TGWindow *p, Int_t id, GContext_t norm, UInt_t options)
    : TGFrame(p, 1, 1, options)
{
   // Create button base class part.

   fWidgetId    = id;
   fWidgetFlags = kWidgetWantFocus;
   fMsgWindow   = p;
   fUserData    = 0;
   fTip         = 0;
   fGroup       = 0;

   fNormGC   = norm;
   fState    = kButtonUp;
   fStayDown = kFALSE;

   if (p && p->IsA()->InheritsFrom(TGButtonGroup::Class())) {
      TGButtonGroup *bg = (TGButtonGroup*) p;
      bg->Insert(this, id);
   }

   gVirtualX->GrabButton(fId, kButton1, kAnyModifier,
                    kButtonPressMask | kButtonReleaseMask,
                    kNone, kNone);

   AddInput(kEnterWindowMask | kLeaveWindowMask);
   SetWindowName();
}

//______________________________________________________________________________
TGButton::~TGButton()
{
   // Delete button.

   // remove from button group
   if (fGroup) {
      fGroup->Remove(this);
      fGroup = 0;
   }

   delete fTip;
}

//______________________________________________________________________________
void TGButton::SetState(EButtonState state, Bool_t emit)
{
   // Set button state.

   Bool_t was = !IsDown();   // kTRUE if button was off

   if (state != fState) {
      switch (state) {
         case kButtonEngaged:
         case kButtonDown:
            fOptions &= ~kRaisedFrame;
            fOptions |= kSunkenFrame;
            break;
         case kButtonDisabled:
         case kButtonUp:
            fOptions &= ~kSunkenFrame;
            fOptions |= kRaisedFrame;
            break;
      }
      fState = state;
      DoRedraw();
      if (emit || fGroup) EmitSignals(was);
   }
}

//______________________________________________________________________________
void TGButton::SetDown(Bool_t on, Bool_t emit)
{

   // Set button state down according to the parameter 'on'.

   if (GetState() == kButtonDisabled) return;

   SetState(on ? kButtonDown : kButtonUp, emit);
}

//______________________________________________________________________________
void TGButton::SetGroup(TGButtonGroup *group)
{
   // Sets new button-group for this button.

   fGroup = group;
}

//______________________________________________________________________________
Bool_t TGButton::HandleButton(Event_t *event)
{
   // Handle mouse button event.

   Bool_t click = kFALSE;

   if (fTip) fTip->Hide();

   if (fState == kButtonDisabled) return kTRUE;

   Bool_t in = (event->fX >= 0) && (event->fY >= 0) &&
               (event->fX <= (Int_t)fWidth) && (event->fY <= (Int_t)fHeight);

   // We don't need to check the button number as GrabButton will
   // only allow button1 events
   if (event->fType == kButtonPress) {
      fgReleaseBtn = 0;

      if (fState == kButtonEngaged) {
         return kTRUE;
      }
      if (in) SetState(kButtonDown, kTRUE);
   } else { // ButtonRelease
      if (fState == kButtonEngaged) {
         if (in) SetState(kButtonUp, kTRUE);
         click = kTRUE;
      } else {
         click = (fState == kButtonDown) && in;
         if (click && fStayDown) {
            if (in) {
               SetState(kButtonEngaged, kTRUE);
               fgReleaseBtn = 0;
            }
         } else {
            if (in) {
               SetState(kButtonUp, kTRUE);
               fgReleaseBtn = fId;
            }
         }
      }
   }
   if (click) {
      SendMessage(fMsgWindow, MK_MSG(kC_COMMAND, kCM_BUTTON), fWidgetId,
                  (Long_t) fUserData);
      fClient->ProcessLine(fCommand, MK_MSG(kC_COMMAND, kCM_BUTTON), fWidgetId,
                           (Long_t) fUserData);
   }

   return kTRUE;
}

//______________________________________________________________________________
void TGButton::EmitSignals(Bool_t was)
{
   //

   Bool_t now = !IsDown();       // kTRUE if button now is off

   // emit signals
   if (was && !now) {
      Pressed();                 // emit Pressed  = was off , now on
      if (fStayDown) Clicked();  // emit Clicked
   }
   if (!was && now) {
      Released();                // emit Released = was on , now off
      Clicked();                 // emit Clicked
   }
   if ((was != now) && IsToggleButton()) Toggled(!now); // emit Toggled  = was != now
}

//______________________________________________________________________________
Bool_t TGButton::HandleCrossing(Event_t *event)
{
   // Handle mouse crossing event.

   if (fTip) {
      if (event->fType == kEnterNotify)
         fTip->Reset();
      else
         fTip->Hide();
   }

   if ((fgDbw != event->fWindow) || (fgReleaseBtn == event->fWindow)) return kTRUE;

   if (!(event->fState & (kButton1Mask | kButton2Mask | kButton3Mask)))
      return kTRUE;

   if (fState == kButtonEngaged || fState == kButtonDisabled) return kTRUE;

   if (event->fType == kEnterNotify) {
      SetState(kButtonDown, kFALSE);
   } else {
      SetState(kButtonUp, kFALSE);
   }
   return kTRUE;
}

//______________________________________________________________________________
void TGButton::SetToolTipText(const char *text, Long_t delayms)
{
   // Set tool tip text associated with this button. The delay is in
   // milliseconds (minimum 250). To remove tool tip call method with
   // text = 0.

   if (fTip) {
      delete fTip;
      fTip = 0;
   }

   if (text && strlen(text))
      fTip = new TGToolTip(fClient->GetDefaultRoot(), this, text, delayms);
}

//______________________________________________________________________________
const TGGC &TGButton::GetDefaultGC()
{
   if (!fgDefaultGC)
      fgDefaultGC = gClient->GetResourcePool()->GetFrameGC();
   return *fgDefaultGC;
}

//______________________________________________________________________________
const TGGC &TGButton::GetHibckgndGC()
{
   if (!fgHibckgndGC) {
      GCValues_t gval;
      gval.fMask = kGCForeground | kGCBackground | kGCTile |
                   kGCFillStyle  | kGCGraphicsExposures;
      gval.fForeground = gClient->GetResourcePool()->GetFrameHiliteColor();
      gval.fBackground = gClient->GetResourcePool()->GetFrameBgndColor();
      gval.fFillStyle  = kFillTiled;
      gval.fTile       = gClient->GetResourcePool()->GetCheckeredPixmap();
      gval.fGraphicsExposures = kFALSE;
      fgHibckgndGC = gClient->GetGC(&gval, kTRUE);
   }
   return *fgHibckgndGC;
}


//______________________________________________________________________________
TGTextButton::TGTextButton(const TGWindow *p, TGHotString *s, Int_t id,
                           GContext_t norm, FontStruct_t font,
                           UInt_t options) : TGButton(p, id, norm, options)
{
   // Create a text button widget. The hotstring will be adopted and deleted
   // by the text button.

   fLabel = s;
   fFontStruct = font;

   Init();
}

//______________________________________________________________________________
TGTextButton::TGTextButton(const TGWindow *p, const char *s, Int_t id,
                           GContext_t norm, FontStruct_t font,
                           UInt_t options) : TGButton(p, id, norm, options)
{
   // Create a text button widget.

   fLabel = new TGHotString(!p && !s ? GetName() : s);
   fFontStruct = font;

   Init();
}

//______________________________________________________________________________
TGTextButton::TGTextButton(const TGWindow *p, const char *s, const char *cmd,
                           Int_t id, GContext_t norm, FontStruct_t font,
                           UInt_t options) : TGButton(p, id, norm, options)
{
   // Create a text button widget and set cmd string at same time.

   fLabel = new TGHotString(s);
   fFontStruct = font;
   fCommand = cmd;

   Init();
}

//______________________________________________________________________________
void TGTextButton::Init()
{
   // Common initialization used by the different ctors.

   int hotchar, max_ascent, max_descent;

   fTMode      = kTextCenterX | kTextCenterY;
   fHKeycode   = 0;
   fHasOwnFont = kFALSE;

   fTWidth  = gVirtualX->TextWidth(fFontStruct, fLabel->GetString(), fLabel->GetLength());
   gVirtualX->GetFontProperties(fFontStruct, max_ascent, max_descent);
   fTHeight = max_ascent + max_descent;

   Resize(fTWidth + 8, fTHeight + 7);

   if ((hotchar = fLabel->GetHotChar()) != 0) {
      if ((fHKeycode = gVirtualX->KeysymToKeycode(hotchar)) != 0) {
         const TGMainFrame *main = (TGMainFrame *) GetMainFrame();
         main->BindKey(this, fHKeycode, kKeyMod1Mask);
         main->BindKey(this, fHKeycode, kKeyMod1Mask | kKeyShiftMask);
         main->BindKey(this, fHKeycode, kKeyMod1Mask | kKeyLockMask);
         main->BindKey(this, fHKeycode, kKeyMod1Mask | kKeyShiftMask | kKeyLockMask);
      }
   }
   SetWindowName();
}

//______________________________________________________________________________
TGTextButton::~TGTextButton()
{
   // Delete a text button widget.

   if (fHKeycode && (fParent->MustCleanup() != kDeepCleanup)) {
      const TGMainFrame *main = (TGMainFrame *) GetMainFrame();
      main->RemoveBind(this, fHKeycode, kKeyMod1Mask);
      main->RemoveBind(this, fHKeycode, kKeyMod1Mask | kKeyShiftMask);
      main->RemoveBind(this, fHKeycode, kKeyMod1Mask | kKeyLockMask);
      main->RemoveBind(this, fHKeycode, kKeyMod1Mask | kKeyShiftMask | kKeyLockMask);
   }
   if (fLabel) delete fLabel;
   if (fHasOwnFont) delete fClient->GetGCPool()->FindGC(fNormGC);
}

//______________________________________________________________________________
void TGTextButton::SetText(TGHotString *new_label)
{
   // Set new button text.

   int hotchar;
   const TGMainFrame *main = (TGMainFrame *) GetMainFrame();

   if (fLabel) {
      if (fHKeycode) {
         main->RemoveBind(this, fHKeycode, kKeyMod1Mask);
         main->RemoveBind(this, fHKeycode, kKeyMod1Mask | kKeyShiftMask);
         main->RemoveBind(this, fHKeycode, kKeyMod1Mask | kKeyLockMask);
         main->RemoveBind(this, fHKeycode, kKeyMod1Mask | kKeyShiftMask | kKeyLockMask);
      }
      delete fLabel;
   }

   fLabel = new_label;
   if ((hotchar = fLabel->GetHotChar()) != 0) {
      if ((fHKeycode = gVirtualX->KeysymToKeycode(hotchar)) != 0)
         main->BindKey(this, fHKeycode, kKeyMod1Mask);
         main->BindKey(this, fHKeycode, kKeyMod1Mask | kKeyShiftMask);
         main->BindKey(this, fHKeycode, kKeyMod1Mask | kKeyLockMask);
         main->BindKey(this, fHKeycode, kKeyMod1Mask | kKeyShiftMask | kKeyLockMask);
   }

   int max_ascent, max_descent;
   fTWidth = gVirtualX->TextWidth(fFontStruct, fLabel->GetString(), fLabel->GetLength());
   gVirtualX->GetFontProperties(fFontStruct, max_ascent, max_descent);
   fTHeight = max_ascent + max_descent;

   fClient->NeedRedraw(this);
}

//______________________________________________________________________________
void TGTextButton::SetText(const TString &new_label)
{
   // Set new button text.

   SetText(new TGHotString(new_label));
}

//______________________________________________________________________________
void TGTextButton::DoRedraw()
{
   // Draw the text button.

   int x, y;
   int max_ascent, max_descent;

   TGFrame::DoRedraw();

   if (fTMode & kTextLeft)
      x = 4;
   else if (fTMode & kTextRight)
      x = fWidth - fTWidth - 4;
   else
      x = (fWidth - fTWidth) >> 1;

   if (fTMode & kTextTop)
      y = 3;
   else if (fTMode & kTextBottom)
      y = fHeight - fTHeight - 3;
   else
      y = (fHeight - fTHeight) >> 1;

   if (fState == kButtonDown || fState == kButtonEngaged) { ++x; ++y; }
   if (fState == kButtonEngaged) {
      gVirtualX->FillRectangle(fId, GetHibckgndGC()(), 2, 2, fWidth-4, fHeight-4);
      gVirtualX->DrawLine(fId, GetHilightGC()(), 2, 2, fWidth-3, 2);
   }
   gVirtualX->GetFontProperties(fFontStruct, max_ascent, max_descent);
   if (fState == kButtonDisabled) {
      fLabel->Draw(fId, GetHilightGC()(), x+1, y+1 + max_ascent);
      fLabel->Draw(fId, GetShadowGC()(), x, y + max_ascent);
   } else {
      fLabel->Draw(fId, fNormGC, x, y + max_ascent);
   }
}

//______________________________________________________________________________
Bool_t TGTextButton::HandleKey(Event_t *event)
{
   // Handle key event. This function will be called when the hotkey is hit.

   Bool_t click = kFALSE;
   Bool_t was = !IsDown();   // kTRUE if button was off

   if (event->fType == kGKeyPress) {
      gVirtualX->SetKeyAutoRepeat(kFALSE);
   } else {
      gVirtualX->SetKeyAutoRepeat(kTRUE);
   }

   if (fTip && event->fType == kGKeyPress) fTip->Hide();

   if (fState == kButtonDisabled) return kTRUE;

   // We don't need to check the key number as GrabKey will only
   // allow fHotchar events if Alt button is pressed (kKeyMod1Mask)

   if ((event->fType == kGKeyPress) && (event->fState & kKeyMod1Mask)) {
      if (fState == kButtonEngaged) return kTRUE;
      SetState(kButtonDown);
   } else if ((event->fType == kKeyRelease) && (event->fState & kKeyMod1Mask)) {
      if (fState == kButtonEngaged /*&& !allowRelease*/) return kTRUE;
      click = (fState == kButtonDown);
      if (click && fStayDown) {
         SetState(kButtonEngaged);
      } else {
         SetState(kButtonUp);
      }
   }
   if (click) {
      SendMessage(fMsgWindow, MK_MSG(kC_COMMAND, kCM_BUTTON), fWidgetId,
                  (Long_t) fUserData);
      fClient->ProcessLine(fCommand, MK_MSG(kC_COMMAND, kCM_BUTTON), fWidgetId,
                           (Long_t) fUserData);
   }
   EmitSignals(was);

   return kTRUE;
}

//______________________________________________________________________________
FontStruct_t TGTextButton::GetDefaultFontStruct()
{
   if (!fgDefaultFont)
      fgDefaultFont = gClient->GetResourcePool()->GetDefaultFont();
   return fgDefaultFont->GetFontStruct();
}

//______________________________________________________________________________
void TGTextButton::SetFont(FontStruct_t font, Bool_t global)
{
   // Changes text font.
   // If global is true font is changed globally.

   if (font != fFontStruct) {
      FontH_t v = gVirtualX->GetFontHandle(font);
      if (!v) return;

      fFontStruct = font;
      TGGC *gc = gClient->GetResourcePool()->GetGCPool()->FindGC(fNormGC);

      if (global) {
         gc = new TGGC(*gc); // copy
         fHasOwnFont = kTRUE;
      }
      gc->SetFont(v);
      fNormGC = gc->GetGC();
      int max_ascent, max_descent;

      fTWidth  = gVirtualX->TextWidth(fFontStruct, fLabel->GetString(), fLabel->GetLength());
      gVirtualX->GetFontProperties(fFontStruct, max_ascent, max_descent);
      fTHeight = max_ascent + max_descent;
      Resize();
   }
}

//______________________________________________________________________________
void TGTextButton::SetFont(const char *fontName, Bool_t global)
{
   // Changes text font specified by name.
   // If global is true font is changed globally.

   TGFont *font = fClient->GetFont(fontName);
   if (font) {
      SetFont(font->GetFontStruct(), global);
   }
}

//______________________________________________________________________________
void TGTextButton::SetTextColor(Pixel_t color, Bool_t global)
{
   // Changes text color.
   // If global is true color is changed globally.

   TGGC *gc = gClient->GetResourcePool()->GetGCPool()->FindGC(fNormGC);

   if (global) {
      gc = new TGGC(*gc); // copy
      fHasOwnFont = kTRUE;
   }

   gc->SetForeground(color);
   fNormGC = gc->GetGC();
   fClient->NeedRedraw(this);
}

//______________________________________________________________________________
Bool_t TGTextButton::HasOwnFont() const
{
   // Returns kTRUE if text attributes are unique,
   // returns kFALSE if text attributes are shared (global).

   return fHasOwnFont;
}

//______________________________________________________________________________
TGPictureButton::TGPictureButton(const TGWindow *p, const TGPicture *pic,
      Int_t id, GContext_t norm, UInt_t option) : TGButton(p, id, norm, option)
{
   // Create a picture button widget. The picture is not adopted and must
   // later be freed by the user once the picture button is deleted (a single
   // picture reference might be used by other buttons).

   if (!pic) {
      Error("TGPictureButton", "pixmap not found for button %d", id);
      fPic = fClient->GetPicture("mb_question_s.xpm");
   } else {
      fPic = pic;
   }

   if (fPic) {
      fTWidth  = fPic->GetWidth();
      fTHeight = fPic->GetHeight();

      Resize(fTWidth  + (fBorderWidth << 1) + fBorderWidth + 1,
             fTHeight + (fBorderWidth << 1) + fBorderWidth); // *3
   }
   fPicD = 0;
   SetWindowName();
}

//______________________________________________________________________________
TGPictureButton::TGPictureButton(const TGWindow *p, const TGPicture *pic,
      const char *cmd, Int_t id, GContext_t norm, UInt_t option)
   : TGButton(p, id, norm, option)
{
   // Create a picture button widget and set action command. The picture is
   // not adopted and must later be freed by the user once the picture button
   // is deleted (a single picture reference might be used by other
   // buttons).

   if (!pic) {
      Error("TGPictureButton", "pixmap not found for button\n%s",
            cmd ? cmd : "");
      fPic = fClient->GetPicture("mb_question_s.xpm");
   } else {
      fPic = pic;
   }

   fCommand = cmd;

   if (fPic) {
      fTWidth  = fPic->GetWidth();
      fTHeight = fPic->GetHeight();

      Resize(fTWidth  + (fBorderWidth << 1) + fBorderWidth + 1,
             fTHeight + (fBorderWidth << 1) + fBorderWidth); // *3
   }
   fPicD = 0;
   SetWindowName();
}

//______________________________________________________________________________
TGPictureButton::TGPictureButton(const TGWindow *p, const char *pic,
   Int_t id, GContext_t norm, UInt_t option) : TGButton(p, id, norm, option)
{
   // Create a picture button. Where pic is the file name of the picture.

   if (!pic || !strlen(pic)) {
      if (p) Error("TGPictureButton", "pixmap not found for button");
      fPic = fClient->GetPicture("mb_question_s.xpm");
   } else {
      fPic = fClient->GetPicture(pic);
   }

   if (fPic) {
      fTWidth  = fPic->GetWidth();
      fTHeight = fPic->GetHeight();

      Resize(fTWidth  + (fBorderWidth << 1) + fBorderWidth + 1,
             fTHeight + (fBorderWidth << 1) + fBorderWidth); // *3
   }
   fPicD = 0;
   SetWindowName();
}

//______________________________________________________________________________
void TGPictureButton::SetPicture(const TGPicture *new_pic)
{
   // Change a picture in a picture button. The picture is not adopted and
   // must later be freed by the user once the picture button is deleted
   // (a single picture reference might be used by other buttons).

   if (!new_pic) {
      Error("SetPicture", "pixmap not found for button %d\n%s",
            fWidgetId, fCommand.Data());
      return;
   }

   fPic = new_pic;

   fTWidth  = fPic->GetWidth();
   fTHeight = fPic->GetHeight();

   fClient->NeedRedraw(this);
}

//______________________________________________________________________________
void TGPictureButton::DoRedraw()
{
   // Redraw picture button.

   if (!fPic) {
      TGFrame::DoRedraw();
      return;
   }

   int x = (fWidth - fTWidth) >> 1;
   int y = (fHeight - fTHeight) >> 1;

   TGFrame::DoRedraw();
   if (fState == kButtonDown || fState == kButtonEngaged) { ++x; ++y; }
   if (fState == kButtonEngaged) {
      gVirtualX->FillRectangle(fId, GetHibckgndGC()(), 2, 2, fWidth-4, fHeight-4);
      gVirtualX->DrawLine(fId, GetHilightGC()(), 2, 2, fWidth-3, 2);
   }
   const TGPicture *pic = (fState == kButtonDisabled) && fPicD ? fPicD : fPic;

   pic->Draw(fId, fNormGC, x, y);
}


//______________________________________________________________________________
TGCheckButton::TGCheckButton(const TGWindow *p, TGHotString *s, Int_t id,
                             GContext_t norm, FontStruct_t font, UInt_t option)
   : TGTextButton(p, s, id, norm, font, option)
{
   // Create a check button widget. The hotstring will be adopted and deleted
   // by the check button.

   Init();
}

//______________________________________________________________________________
TGCheckButton::TGCheckButton(const TGWindow *p, const char *s, Int_t id,
                             GContext_t norm, FontStruct_t font, UInt_t option)
   : TGTextButton(p, s, id, norm, font, option)
{
   // Create a check button widget.

   Init();
}

//______________________________________________________________________________
TGCheckButton::TGCheckButton(const TGWindow *p, const char *s, const char *cmd,
                             Int_t id, GContext_t norm, FontStruct_t font,
                             UInt_t option) : TGTextButton(p, s, cmd, id, norm, font, option)
{
   // Create a check button widget.

   Init();
}

//______________________________________________________________________________
void TGCheckButton::Init()
{
   // Common initialization.

   fPrevState =
   fState     = kButtonUp;
   fHKeycode = 0;

   int hotchar, max_ascent, max_descent;
   fTWidth  = gVirtualX->TextWidth(fFontStruct, fLabel->GetString(), fLabel->GetLength());
   gVirtualX->GetFontProperties(fFontStruct, max_ascent, max_descent);
   fTHeight = max_ascent + max_descent;

   Resize(fTWidth + 22, fTHeight + 2);

   if ((hotchar = fLabel->GetHotChar()) != 0) {
      if ((fHKeycode = gVirtualX->KeysymToKeycode(hotchar)) != 0) {
         const TGMainFrame *main = (TGMainFrame *) GetMainFrame();
         main->BindKey(this, fHKeycode, kKeyMod1Mask);
         main->BindKey(this, fHKeycode, kKeyMod1Mask | kKeyShiftMask);
         main->BindKey(this, fHKeycode, kKeyMod1Mask | kKeyLockMask);
         main->BindKey(this, fHKeycode, kKeyMod1Mask | kKeyShiftMask | kKeyLockMask);
      }
   }
   SetWindowName();
}

//______________________________________________________________________________
TGCheckButton::~TGCheckButton()
{
   // Delete a check button.

}

//______________________________________________________________________________
void TGCheckButton::SetState(EButtonState state, Bool_t emit)
{
   // Set check button state.

   PSetState(fPrevState = state, emit);
}

//______________________________________________________________________________
void TGCheckButton::EmitSignals(Bool_t /*wasUp*/)
{
   // emit signals

   if (fState == kButtonUp)   Released();            // emit Released
   if (fState == kButtonDown) Pressed();             // emit Pressed
   Clicked();                                        // emit Clicked
   Toggled(fState == kButtonDown);                   // emit Toggled
}

//______________________________________________________________________________
void TGCheckButton::PSetState(EButtonState state, Bool_t emit)
{
   // Set check button state.

   if (state != fState) {
      fState = state;

      if (emit || fGroup) {
         // button signals
         EmitSignals();
      }
      DoRedraw();
   }
}

//______________________________________________________________________________
Bool_t TGCheckButton::HandleButton(Event_t *event)
{
   // Handle mouse button event.

   Bool_t click = kFALSE;

   if (fTip) fTip->Hide();

   if (fState == kButtonDisabled) return kTRUE;

   Bool_t in = (event->fX >= 0) && (event->fY >= 0) &&
               (event->fX <= (Int_t)fWidth) && (event->fY <= (Int_t)fHeight);

   // We don't need to check the button number as GrabButton will
   // only allow button1 events
   if (event->fType == kButtonPress) {
      fgReleaseBtn = 0;
      if (in) fOptions |= kSunkenFrame;
   } else { // ButtonRelease
      if (in) {
         PSetState((fPrevState == kButtonUp) ? kButtonDown : kButtonUp, kFALSE);
         click = (fState != fPrevState);
         fPrevState = fState;
      }
      fgReleaseBtn = fId;
      fOptions &= ~kSunkenFrame;
   }
   if (click) {
      EmitSignals();
      SendMessage(fMsgWindow, MK_MSG(kC_COMMAND, kCM_CHECKBUTTON),
                  fWidgetId, (Long_t) fUserData);
      fClient->ProcessLine(fCommand, MK_MSG(kC_COMMAND, kCM_CHECKBUTTON),
                           fWidgetId, (Long_t) fUserData);
   }

   DoRedraw();
   return kTRUE;
}

//______________________________________________________________________________
Bool_t TGCheckButton::HandleCrossing(Event_t *event)
{
   // Handle mouse crossing event.

   if (fTip) {
      if (event->fType == kEnterNotify)
         fTip->Reset();
      else
         fTip->Hide();
   }

   if ((fgDbw != event->fWindow) || (fgReleaseBtn == event->fWindow)) return kTRUE;

   if (!(event->fState & (kButton1Mask | kButton2Mask | kButton3Mask)))
      return kTRUE;

   if (fState == kButtonDisabled) return kTRUE;

   if (event->fType == kEnterNotify) {
      fOptions |= kSunkenFrame;
   } else {
      fOptions &= ~kSunkenFrame;
   }
   DoRedraw();

   return kTRUE;
}

//______________________________________________________________________________
Bool_t TGCheckButton::HandleKey(Event_t *event)
{
   // Handle key event. This function will be called when the hotkey is hit.

   Bool_t click = kFALSE;

   if (event->fType == kGKeyPress)
      gVirtualX->SetKeyAutoRepeat(kFALSE);
   else
      gVirtualX->SetKeyAutoRepeat(kTRUE);

   if (fTip && event->fType == kGKeyPress) fTip->Hide();

   if (fState == kButtonDisabled) return kTRUE;

   // We don't need to check the key number as GrabKey will only
   // allow fHotchar events if Alt button is pressed (kKeyMod1Mask)

   if ((event->fType == kGKeyPress) && (event->fState & kKeyMod1Mask)) {
      PSetState((fPrevState == kButtonUp) ? kButtonDown : kButtonUp, kTRUE);
   } else if ((event->fType == kKeyRelease) && (event->fState & kKeyMod1Mask)) {
      click = (fState != fPrevState);
      fPrevState = fState;
   }
   if (click) {
      SendMessage(fMsgWindow, MK_MSG(kC_COMMAND, kCM_CHECKBUTTON), fWidgetId,
                  (Long_t) fUserData);
      fClient->ProcessLine(fCommand, MK_MSG(kC_COMMAND, kCM_CHECKBUTTON), fWidgetId,
                           (Long_t) fUserData);
   }
   return kTRUE;
}

//______________________________________________________________________________
void TGCheckButton::DoRedraw()
{
   // Draw the check button widget.

   int x, y, y0, cw;

   TGFrame::DoRedraw();

   cw = 13;
   y0 = (fHeight - cw) >> 1;

   gVirtualX->DrawLine(fId, GetShadowGC()(), 0, y0, cw-2, y0);
   gVirtualX->DrawLine(fId, GetShadowGC()(), 0, y0, 0, y0+cw-2);
   gVirtualX->DrawLine(fId, GetBlackGC()(), 1, y0+1, cw-3, y0+1);
   gVirtualX->DrawLine(fId, GetBlackGC()(), 1, y0+1, 1, y0+cw-3);

   gVirtualX->DrawLine(fId, GetHilightGC()(), 0, y0+cw-1, cw-1, y0+cw-1);
   gVirtualX->DrawLine(fId, GetHilightGC()(), cw-1, y0+cw-1, cw-1, y0);
   gVirtualX->DrawLine(fId, GetBckgndGC()(),  2, y0+cw-2, cw-2, y0+cw-2);
   gVirtualX->DrawLine(fId, GetBckgndGC()(),  cw-2, y0+2, cw-2, y0+cw-2);

   gVirtualX->FillRectangle(fId, GetWhiteGC()(), 2, y0+2, cw-4, cw-4);

   if (fState == kButtonDown) {
      Segment_t seg[6];

      int l = 2;
      int t = y0+2;

      seg[0].fX1 = 1+l; seg[0].fY1 = 3+t; seg[0].fX2 = 3+l; seg[0].fY2 = 5+t;
      seg[1].fX1 = 1+l; seg[1].fY1 = 4+t; seg[1].fX2 = 3+l; seg[1].fY2 = 6+t;
      seg[2].fX1 = 1+l; seg[2].fY1 = 5+t; seg[2].fX2 = 3+l; seg[2].fY2 = 7+t;
      seg[3].fX1 = 3+l; seg[3].fY1 = 5+t; seg[3].fX2 = 7+l; seg[3].fY2 = 1+t;
      seg[4].fX1 = 3+l; seg[4].fY1 = 6+t; seg[4].fX2 = 7+l; seg[4].fY2 = 2+t;
      seg[5].fX1 = 3+l; seg[5].fY1 = 7+t; seg[5].fX2 = 7+l; seg[5].fY2 = 3+t;

      gVirtualX->DrawSegments(fId, GetBlackGC()(), seg, 6);
   }

   x = 20;
   y = y0;

   int max_ascent, max_descent;
   gVirtualX->GetFontProperties(fFontStruct, max_ascent, max_descent);
   if (fState == kButtonDisabled) {
     fLabel->Draw(fId, GetHilightGC()(), x+1, y+1 + max_ascent);
     fLabel->Draw(fId, GetShadowGC()(), x, y + max_ascent);
   } else {
     fLabel->Draw(fId, fNormGC, x, y + max_ascent);
   }
}

//______________________________________________________________________________
FontStruct_t TGCheckButton::GetDefaultFontStruct()
{
   if (!fgDefaultFont)
      fgDefaultFont = gClient->GetResourcePool()->GetDefaultFont();
   return fgDefaultFont->GetFontStruct();
}

//______________________________________________________________________________
const TGGC &TGCheckButton::GetDefaultGC()
{
   if (!fgDefaultGC)
      fgDefaultGC = gClient->GetResourcePool()->GetFrameGC();
   return *fgDefaultGC;
}


//______________________________________________________________________________
TGRadioButton::TGRadioButton(const TGWindow *p, TGHotString *s, Int_t id,
                             GContext_t norm, FontStruct_t font, UInt_t option)
   : TGTextButton(p, s, id, norm, font, option)
{
   // Create a radio button widget. The hotstring will be adopted and deleted
   // by the radio button.

   Init();
}

//______________________________________________________________________________
TGRadioButton::TGRadioButton(const TGWindow *p, const char *s, Int_t id,
                             GContext_t norm, FontStruct_t font, UInt_t option)
   : TGTextButton(p, s, id, norm, font, option)
{
   // Create a radio button widget.

   Init();
}
//______________________________________________________________________________
TGRadioButton::TGRadioButton(const TGWindow *p, const char *s, const char *cmd,
                             Int_t id, GContext_t norm,
                             FontStruct_t font, UInt_t option)
    : TGTextButton(p, s, cmd, id, norm, font, option)
{
   // Create a radio button widget.

   Init();
}

//______________________________________________________________________________
void TGRadioButton::Init()
{
   // Common radio button initialization.

   fPrevState =
   fState     = kButtonUp;
   fHKeycode  = 0;

   fOn  = fClient->GetPicture("rbutton_on.xpm");
   fOff = fClient->GetPicture("rbutton_off.xpm");

   if (!fOn || !fOff)
      Error("TGRadioButton", "rbutton_*.xpm not found");

   int hotchar, max_ascent, max_descent;
   fTWidth = gVirtualX->TextWidth(fFontStruct, fLabel->GetString(), fLabel->GetLength());
   gVirtualX->GetFontProperties(fFontStruct, max_ascent, max_descent);
   fTHeight = max_ascent + max_descent;

   Resize(fTWidth + 22, fTHeight + 2);

   if ((hotchar = fLabel->GetHotChar()) != 0) {
      if ((fHKeycode = gVirtualX->KeysymToKeycode(hotchar)) != 0) {
         const TGMainFrame *main = (TGMainFrame *) GetMainFrame();
         main->BindKey(this, fHKeycode, kKeyMod1Mask);
         main->BindKey(this, fHKeycode, kKeyMod1Mask | kKeyShiftMask);
         main->BindKey(this, fHKeycode, kKeyMod1Mask | kKeyLockMask);
         main->BindKey(this, fHKeycode, kKeyMod1Mask | kKeyShiftMask | kKeyLockMask);
      }
   }

   if (fParent->IsA()->InheritsFrom(TGButtonGroup::Class())) {
      ((TGButtonGroup*)fParent)->SetRadioButtonExclusive(kTRUE);
   }
   SetWindowName();
}

//______________________________________________________________________________
TGRadioButton::~TGRadioButton()
{
   // Delete a radio button.

   if (fOn)  fClient->FreePicture(fOn);
   if (fOff) fClient->FreePicture(fOff);
}

//______________________________________________________________________________
void TGRadioButton::SetState(EButtonState state, Bool_t emit)
{
   // Set radio button state.

   PSetState(fPrevState = state, emit);
}

//______________________________________________________________________________
void TGRadioButton::EmitSignals(Bool_t /*wasUp*/)
{
   // emit signals

   if (fState == kButtonUp)   Released();            // emit Released
   if (fState == kButtonDown) Pressed();             // emit Pressed
   Clicked();                                        // emit Clicked
   Toggled(fState == kButtonDown);                   // emit Toggled
}

//______________________________________________________________________________
void TGRadioButton::PSetState(EButtonState state, Bool_t emit)
{
   // Set radio button state.

   if (state != fState) {
      fPrevState = fState = state;

      if (emit || fGroup) {
         // button signals
         EmitSignals();
      }
      DoRedraw();
   }
}

//______________________________________________________________________________
Bool_t TGRadioButton::HandleButton(Event_t *event)
{
   // Handle mouse button event.

   if (fTip) fTip->Hide();

   if (fState == kButtonDisabled) return kFALSE;


   Bool_t in = (event->fX >= 0) && (event->fY >= 0) &&
               (event->fX <= (Int_t)fWidth) && (event->fY <= (Int_t)fHeight);

   if (event->fType == kButtonRelease) {
      if (in) {
         PSetState(kButtonDown, kFALSE);

         fPrevState = fState;

         EmitSignals();
         SendMessage(fMsgWindow, MK_MSG(kC_COMMAND, kCM_RADIOBUTTON),
                     fWidgetId, (Long_t) fUserData);
         fClient->ProcessLine(fCommand, MK_MSG(kC_COMMAND, kCM_RADIOBUTTON),
                              fWidgetId, (Long_t) fUserData);
      }
      fOptions &= ~kSunkenFrame;
      fgReleaseBtn = fId;
   } else { // 
      fgReleaseBtn = 0;

      if (in) fOptions |= kSunkenFrame;
   }

   DoRedraw();
   return kTRUE;
}

//______________________________________________________________________________
Bool_t TGRadioButton::HandleCrossing(Event_t *event)
{
   // Handle mouse crossing event.

   if (fTip) {
      if (event->fType == kEnterNotify)
         fTip->Reset();
      else
         fTip->Hide();
   }

   if ((fgDbw != event->fWindow) || (fgReleaseBtn == event->fWindow)) return kTRUE;

   if (!(event->fState & (kButton1Mask | kButton2Mask | kButton3Mask)))
      return kTRUE;

   if (fState == kButtonDisabled) return kTRUE;

   if (event->fType == kEnterNotify) {
      fOptions |= kSunkenFrame;
   } else {
      fOptions &= ~kSunkenFrame;
   }
   DoRedraw();

   return kTRUE;
}

//______________________________________________________________________________
Bool_t TGRadioButton::HandleKey(Event_t *event)
{
   // Handle key event. This function will be called when the hotkey is hit.

   if (event->fType == kGKeyPress)
      gVirtualX->SetKeyAutoRepeat(kFALSE);
   else
      gVirtualX->SetKeyAutoRepeat(kTRUE);

  if (fTip && event->fType == kGKeyPress) fTip->Hide();

   if (fState == kButtonDisabled) return kTRUE;

   // We don't need to check the key number as GrabKey will only
   // allow fHotchar events if Alt button is pressed (kKeyMod1Mask)

   if ((event->fType == kGKeyPress) && (event->fState & kKeyMod1Mask)) {
      PSetState(kButtonDown, kTRUE);
      SendMessage(fMsgWindow, MK_MSG(kC_COMMAND, kCM_RADIOBUTTON),
                  fWidgetId, (Long_t) fUserData);
      fClient->ProcessLine(fCommand, MK_MSG(kC_COMMAND, kCM_RADIOBUTTON),
                           fWidgetId, (Long_t) fUserData);
   } else if ((event->fType == kKeyRelease) && (event->fState & kKeyMod1Mask)) {
      fPrevState = fState;
   }
   return kTRUE;
}

//______________________________________________________________________________
void TGRadioButton::DoRedraw()
{
   // Draw a radio button.

   int nlines, tx, ty, y0, pw;

   TGFrame::DoRedraw();

   tx = 20;
   nlines = fLabel->GetLines(fFontStruct, fWidth-tx-1);
   ty = (fHeight - fTHeight*nlines) >> 1;

   pw = 12;
   y0 = ty + ((fTHeight - pw) >> 1);

   if (fState == kButtonDown) {
      if (fOn) fOn->Draw(fId, fNormGC, 0, y0);
   } else {
      if (fOff) fOff->Draw(fId, fNormGC, 0, y0);
   }

   int max_ascent, max_descent;
   gVirtualX->GetFontProperties(fFontStruct, max_ascent, max_descent);

   // ty = max_ascent + 1;
   ty += max_ascent;

   //  fLabel->Draw(fId, fNormGC, tx, ty);
   if (fState == kButtonDisabled) {
      fLabel->DrawWrapped(fId, GetHilightGC()(), tx+1, ty+1, fWidth-tx-1, fFontStruct);
      fLabel->DrawWrapped(fId, GetShadowGC()(), tx, ty, fWidth-tx-1, fFontStruct);
   } else {
      fLabel->DrawWrapped(fId, fNormGC, tx, ty, fWidth-tx-1, fFontStruct);
   }
}

//______________________________________________________________________________
FontStruct_t TGRadioButton::GetDefaultFontStruct()
{
   if (!fgDefaultFont)
      fgDefaultFont = gClient->GetResourcePool()->GetDefaultFont();
   return fgDefaultFont->GetFontStruct();
}

//______________________________________________________________________________
const TGGC &TGRadioButton::GetDefaultGC()
{
   if (!fgDefaultGC)
      fgDefaultGC = gClient->GetResourcePool()->GetFrameGC();
   return *fgDefaultGC;
}

//______________________________________________________________________________
void TGButton::SavePrimitive(ofstream &out, Option_t *option)
{
   // Save a button widget as a C++ statement(s) on output stream out

   char quote = '"';

   if (fState == kButtonDown) {
      out << "   " << GetName() << "->SetState(kButtonDown);"  << endl;
   }
   if (fState == kButtonDisabled) {
      out << "   " << GetName() << "->SetState(kButtonDisabled);"  << endl;
   }
   if (fState == kButtonEngaged) {
      out << "   " << GetName() << "->SetState(kButtonEngaged);"  << endl;
   }
   if (fBackground != fgDefaultFrameBackground) {
      SaveUserColor(out, option);
      out << "   " << GetName() << "->ChangeBackground(ucolor);" << endl;
   }

   if (fTip) {
      out << "   ";
      out << GetName() << "->SetToolTipText(" << quote
          << fTip->GetText()->GetString() << quote << ");"  << endl;
   }
   if (strlen(fCommand)) {
      out << "   " << GetName() << "->SetCommand(" << quote << fCommand
          << quote << ");" << endl;
   }
}

//______________________________________________________________________________
void TGTextButton::SavePrimitive(ofstream &out, Option_t *option)
{
   // Save a text button widget as a C++ statement(s) on output stream out

   char quote = '"';
   const char *text = fLabel->GetString();
   Int_t hotpos = fLabel->GetHotPos();
   Int_t lentext = fLabel->GetLength();
   char *outext = new char[lentext+2];
   Int_t i=0;
   while (lentext) {
      if (hotpos && (i == hotpos-1)) {
         outext[i] = '&';
         i++;
      }
      outext[i] = *text;
      i++;
      text++;
      lentext--;
   }
   outext[i]=0;

   // font + GC
   option = GetName()+5;         // unique digit id of the name
   char ParGC[50], ParFont[50];
   sprintf(ParFont,"%s::GetDefaultFontStruct()",IsA()->GetName());
   sprintf(ParGC,"%s::GetDefaultGC()()",IsA()->GetName());

   if ((GetDefaultFontStruct() != fFontStruct) || (GetDefaultGC()() != fNormGC)) {
      TGFont *ufont = gClient->GetResourcePool()->GetFontPool()->FindFont(fFontStruct);
      if (ufont) {
         ufont->SavePrimitive(out, option);
         sprintf(ParFont,"ufont->GetFontStruct()");
      }

      TGGC *userGC = gClient->GetResourcePool()->GetGCPool()->FindGC(fNormGC);
      if (userGC) {
         userGC->SavePrimitive(out, option);
         sprintf(ParGC,"uGC->GetGC()");
      }
   }

   if (fBackground != GetDefaultFrameBackground()) SaveUserColor(out, option);

   out << "   TGTextButton *";
   out << GetName() << " = new TGTextButton(" << fParent->GetName()
       << "," << quote << outext << quote;

   if (GetOptions() == (kRaisedFrame | kDoubleBorder)) {
      if (fFontStruct == GetDefaultFontStruct()) {
         if (fNormGC == GetDefaultGC()()) {
            if (fWidgetId == -1) {
               out << ");" << endl;
            } else {
               out << "," << fWidgetId <<");" << endl;
            }
         } else {
            out << "," << fWidgetId << "," << ParGC << ");" << endl;
         }
      } else {
         out << "," << fWidgetId << "," << ParGC << "," << ParFont << ");" << endl;
      }
   } else {
      out << "," << fWidgetId << "," << ParGC << "," << ParFont << "," << GetOptionString() << ");" << endl;
   }

   delete [] outext;

   out << "   " << GetName() << "->Resize(" << GetWidth() << "," << GetHeight()
       << ");" << endl;

   TGButton::SavePrimitive(out,option);
}

//______________________________________________________________________________
void TGPictureButton::SavePrimitive(ofstream &out, Option_t *option)
{
   // Save a picture button widget as a C++ statement(s) on output stream out

   if (!fPic) {
      Error("SavePrimitive()", "pixmap not found for picture button %d ", fWidgetId);
      return;
   }

   // GC
   option = GetName()+5;         // unique digit id of the name
   char ParGC[50];
   sprintf(ParGC,"%s::GetDefaultGC()()",IsA()->GetName());

   if (GetDefaultGC()() != fNormGC) {
      TGGC *userGC = gClient->GetResourcePool()->GetGCPool()->FindGC(fNormGC);
      if (userGC) {
         userGC->SavePrimitive(out, option);
         sprintf(ParGC,"uGC->GetGC()");
      }
   }

   char quote = '"';
   const char *picname = fPic->GetName();
   out <<"   TGPictureButton *";
   out << GetName() << " = new TGPictureButton(" << fParent->GetName()
       << ",gClient->GetPicture(" << quote
       << gSystem->ExpandPathName(gSystem->UnixPathName(picname)) << quote << ")";

   if (GetOptions() == (kRaisedFrame | kDoubleBorder)) {
      if (fNormGC == GetDefaultGC()()) {
         if (fWidgetId == -1) {
            out << ");" << endl;
         } else {
            out << "," << fWidgetId << ");" << endl;
         }
      } else {
         out << "," << fWidgetId << "," << ParGC << ");" << endl;
      }
   } else {
      out << "," << fWidgetId << "," << ParGC << "," << GetOptionString()
          << ");" << endl;
   }

   TGButton::SavePrimitive(out,option);
}

//______________________________________________________________________________
void TGCheckButton::SavePrimitive(ofstream &out, Option_t *option)
{
   // Save a check button widget as a C++ statement(s) on output stream out

   char quote = '"';

   const char *text = fLabel->GetString();
   char hotpos = fLabel->GetHotPos();
   Int_t lentext = fLabel->GetLength();
   char *outext = new char[lentext+2];       // should be +2 because of \0
   Int_t i=0;


   while (lentext) {
      if (hotpos && (i == hotpos-1)) {
         outext[i] = '&';
         i++;
      }
      outext[i] = *text;
      i++;
      text++;
      lentext--;
   }
   outext[i]=0;

   out <<"   TGCheckButton *";
   out << GetName() << " = new TGCheckButton(" << fParent->GetName()
       << "," << quote << outext << quote;

   delete [] outext;

   // font + GC
   option = GetName()+5;         // unique digit id of the name
   char ParGC[50], ParFont[50];
   sprintf(ParFont,"%s::GetDefaultFontStruct()",IsA()->GetName());
   sprintf(ParGC,"%s::GetDefaultGC()()",IsA()->GetName());

   if ((GetDefaultFontStruct() != fFontStruct) || (GetDefaultGC()() != fNormGC)) {
      TGFont *ufont = gClient->GetResourcePool()->GetFontPool()->FindFont(fFontStruct);
      if (ufont) {
         ufont->SavePrimitive(out, option);
         sprintf(ParFont,"ufont->GetFontStruct()");
      }

      TGGC *userGC = gClient->GetResourcePool()->GetGCPool()->FindGC(fNormGC);
      if (userGC) {
         userGC->SavePrimitive(out, option);
         sprintf(ParGC,"uGC->GetGC()");
      }
   }

   if (GetOptions() == kChildFrame) {
      if (fFontStruct == GetDefaultFontStruct()) {
         if (fNormGC == GetDefaultGC()()) {
            if (fWidgetId == -1) {
               out << ");" << endl;
            } else {
               out << "," << fWidgetId << ");" << endl;
            }
         } else {
            out << "," << fWidgetId << "," << ParGC << ");" << endl;
         }
      } else {
         out << "," << fWidgetId << "," << ParGC << "," << ParFont << ");" << endl;
      }
   } else {
      out << "," << fWidgetId << "," << ParGC << "," << ParFont << "," << GetOptionString() << ");" << endl;
   }

   TGButton::SavePrimitive(out,option);
}

//______________________________________________________________________________
void TGRadioButton::SavePrimitive(ofstream &out, Option_t *option)
{
   // Save a radio button widget as a C++ statement(s) on output stream out

   char quote = '"';

   const char *text = fLabel->GetString();
   char hotpos = fLabel->GetHotPos();
   Int_t lentext = fLabel->GetLength();
   char *outext = new char[lentext+2];
   Int_t i=0;

   while (lentext) {
      if (hotpos && (i == hotpos-1)) {
         outext[i] = '&';
         i++;
      }
      outext[i] = *text;
      i++; text++; lentext--;
   }
   outext[i]=0;
   out << "   TGRadioButton *";
   out << GetName() << " = new TGRadioButton(" << fParent->GetName()
       << "," << quote << outext << quote;

   delete [] outext;

   // font + GC
   option = GetName()+5;         // unique digit id of the name
   char ParGC[50], ParFont[50];
   sprintf(ParFont,"%s::GetDefaultFontStruct()",IsA()->GetName());
   sprintf(ParGC,"%s::GetDefaultGC()()",IsA()->GetName());

   if ((GetDefaultFontStruct() != fFontStruct) || (GetDefaultGC()() != fNormGC)) {
      TGFont *ufont = gClient->GetResourcePool()->GetFontPool()->FindFont(fFontStruct);
      if (ufont) {
         ufont->SavePrimitive(out, option);
         sprintf(ParFont,"ufont->GetFontStruct()");
      }

      TGGC *userGC = gClient->GetResourcePool()->GetGCPool()->FindGC(fNormGC);
      if (userGC) {
         userGC->SavePrimitive(out, option);
         sprintf(ParGC,"uGC->GetGC()");
      }
   }

   if (GetOptions() == (kChildFrame)) {
      if (fFontStruct == GetDefaultFontStruct()) {
         if (fNormGC == GetDefaultGC()()) {
            if (fWidgetId == -1) {
               out <<");" << endl;
            } else {
               out << "," << fWidgetId << ");" << endl;
            }
         } else {
            out << "," << fWidgetId << "," << ParGC << ");" << endl;
         }
      } else {
         out << "," << fWidgetId << "," << ParGC << "," << ParFont << ");" << endl;
      }
   } else {
      out << "," << fWidgetId << "," << ParGC << "," << ParFont << "," << GetOptionString() << ");" << endl;
   }

   TGButton::SavePrimitive(out,option);
}
