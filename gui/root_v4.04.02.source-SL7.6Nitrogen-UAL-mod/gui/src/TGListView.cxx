// @(#)root/gui:$Name:  $:$Id: TGListView.cxx,v 1.27 2005/01/12 18:39:29 brun Exp $
// Author: Fons Rademakers   17/01/98

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
// TGListView, TGLVContainer and TGLVEntry                              //
//                                                                      //
// A list view is a widget that can contain a number of items           //
// arranged in a grid or list. The items can be represented either      //
// by a string or by an icon.                                           //
//                                                                      //
// The TGListView is user callable. The other classes are service       //
// classes of the list view.                                            //
//                                                                      //
// A list view can generate the following events:                       //
// kC_CONTAINER, kCT_SELCHANGED, total items, selected items.           //
// kC_CONTAINER, kCT_ITEMCLICK, which button, location (y<<16|x).       //
// kC_CONTAINER, kCT_ITEMDBLCLICK, which button, location (y<<16|x).    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TGListView.h"
#include "TGPicture.h"
#include "TGButton.h"
#include "TGScrollBar.h"
#include "TGResourcePool.h"
#include "TList.h"
#include "TMath.h"
#include "TSystem.h"
#include "TGMimeTypes.h"
#include "Riostream.h"

const TGFont *TGLVEntry::fgDefaultFont = 0;
TGGC         *TGLVEntry::fgDefaultGC = 0;

const TGFont *TGListView::fgDefaultFont = 0;
TGGC         *TGListView::fgDefaultGC = 0;


ClassImp(TGLVEntry)
ClassImp(TGLVContainer)
ClassImp(TGListView)

//______________________________________________________________________________
TGLVEntry::TGLVEntry(const TGWindow *p, const TGPicture *bigpic,
                     const TGPicture *smallpic, TGString *name,
                     TGString **subnames, EListViewMode viewMode,
                     UInt_t options, ULong_t back) :
   TGFrame(p, 10, 10, options, back)
{
   // Create a list view item.

   fSelPic = 0;

   fCurrent  =
   fBigPic   = bigpic;
   fSmallPic = smallpic;

   fName = name;
   fSubnames = subnames;
   fUserData = 0;

   fCpos  =
   fJmode = 0;

   fActive = kFALSE;

   fFontStruct = GetDefaultFontStruct();
   fNormGC     = GetDefaultGC()();

   Int_t max_ascent, max_descent;
   fTWidth = gVirtualX->TextWidth(fFontStruct,
                                  fName ? fName->GetString() : "",
                                  fName ? fName->GetLength() : 0);
   gVirtualX->GetFontProperties(fFontStruct, max_ascent, max_descent);
   fTHeight = max_ascent + max_descent;

   if (fSubnames) {
      Int_t i;
      for (i = 0; fSubnames[i] != 0; ++i)
         ;
      fCtw = new int[i+1];
      fCtw[i] = 0;
      for (i = 0; fSubnames[i] != 0; ++i) {
         fCtw[i] = gVirtualX->TextWidth(fFontStruct, fSubnames[i]->GetString(),
                                        fSubnames[i]->GetLength());
      }
   } else {
      fCtw = 0;
   }

   fViewMode = (EListViewMode)-1;
   SetViewMode(viewMode);
}

//______________________________________________________________________________
TGLVEntry::TGLVEntry(const TGLVContainer *p, const TString& name,
                     const TString& cname, TGString **subnames,
                     UInt_t options, Pixel_t back) :
   TGFrame(p, 10, 10, options, back)
{
   // Create a list view item.
   //
   // name - is name of item.
   // cname - is name of icon. In most cases this is class name of object
   //         associated with this item.

   fSelPic = 0;

   fCurrent  =
   fBigPic   = fClient->GetMimeTypeList()->GetIcon(cname, kFALSE);

   if (!fBigPic) {
      fBigPic = fClient->GetPicture("doc_s.xpm");
   }
   fSmallPic = fClient->GetMimeTypeList()->GetIcon(cname, kTRUE);
   if (!fSmallPic) {
      fSmallPic = fClient->GetPicture("doc_t.xpm");
   }

   fName = new TGString(name);
   fSubnames = subnames;
   fUserData = 0;

   fCpos  =
   fJmode = 0;

   fActive = kFALSE;

   fFontStruct = GetDefaultFontStruct();
   fNormGC     = GetDefaultGC()();

   Int_t max_ascent, max_descent;
   fTWidth = gVirtualX->TextWidth(fFontStruct, fName->GetString(), fName->GetLength());
   gVirtualX->GetFontProperties(fFontStruct, max_ascent, max_descent);
   fTHeight = max_ascent + max_descent;

   if (fSubnames) {
      Int_t i;
      for (i = 0; fSubnames[i] != 0; ++i)
         ;
      fCtw = new int[i+1];
      fCtw[i] = 0;
      for (i = 0; fSubnames[i] != 0; ++i) {
         fCtw[i] = gVirtualX->TextWidth(fFontStruct, fSubnames[i]->GetString(),
                                        fSubnames[i]->GetLength());
      }
   } else {
      fCtw = 0;
   }

   fViewMode = (EListViewMode)-1;
   SetViewMode((EListViewMode)p->GetViewMode());
}

//______________________________________________________________________________
TGLVEntry::~TGLVEntry()
{
   // Delete a list view item.

   if (fName) delete fName;
   if (fSelPic) delete fSelPic;
   if (fSubnames) {
      for (Int_t i = 0; fSubnames[i] != 0; ++i) delete fSubnames[i];
      delete [] fSubnames;
      delete [] fCtw;
   }
}

//______________________________________________________________________________
void TGLVEntry::SetSubnames(const char* n1,const char* n2,const char* n3,
                            const char* n4,const char* n5,const char* n6,
                            const char* n7,const char* n8,const char* n9,
                            const char* n10,const char* n11,const char* n12)
{
   // Sets new subnames.

   if (fSubnames) {
      for (Int_t i = 0; fSubnames[i] != 0; ++i) delete fSubnames[i];
      delete [] fSubnames;
      delete [] fCtw;
   }

   Int_t ncol = 0;
   fSubnames = 0;

   if (n12 && strlen(n12)) ncol=12;
   else if (n11 && strlen(n11)) ncol=11;
   else if (n10 && strlen(n10)) ncol=10;
   else if (n9 && strlen(n9)) ncol=9;
   else if (n8 && strlen(n8)) ncol=8;
   else if (n7 && strlen(n7)) ncol=7;
   else if (n6 && strlen(n6)) ncol=6;
   else if (n5 && strlen(n5)) ncol=5;
   else if (n4 && strlen(n4)) ncol=4;
   else if (n3 && strlen(n3)) ncol=3;
   else if (n2 && strlen(n2)) ncol=2;
   else if (n1 && strlen(n1)) ncol=1;

   if (!ncol) return;

   fSubnames = new TGString* [ncol+1];

   if (ncol>11) fSubnames[11] = new TGString(n12);
   if (ncol>10) fSubnames[10] = new TGString(n11);
   if (ncol>9) fSubnames[9] = new TGString(n10);
   if (ncol>8) fSubnames[8] = new TGString(n9);
   if (ncol>7) fSubnames[7] = new TGString(n8);
   if (ncol>6) fSubnames[6] = new TGString(n7);
   if (ncol>5) fSubnames[5] = new TGString(n6);
   if (ncol>4) fSubnames[4] = new TGString(n5);
   if (ncol>3) fSubnames[3] = new TGString(n4);
   if (ncol>2) fSubnames[2] = new TGString(n3);
   if (ncol>1) fSubnames[1] = new TGString(n2);
   if (ncol>0) fSubnames[0] = new TGString(n1);
   fSubnames[ncol] = 0;

   fCtw = new int[ncol];
   fCtw[ncol-1] = 0;

   for (int i = 0; i<ncol; i++) {
      fCtw[i] = gVirtualX->TextWidth(fFontStruct, fSubnames[i]->GetString(),
                                     fSubnames[i]->GetLength());
   }
}

//______________________________________________________________________________
void TGLVEntry::Activate(Bool_t a)
{
   // Make list view item active.

   if (fActive == a) return;
   fActive = a;

   if (fActive) {
      fSelPic = new TGSelectedPicture(gClient, fCurrent);
   } else {
      if (fSelPic) delete fSelPic;
      fSelPic = 0;
   }
   DoRedraw();
}

//______________________________________________________________________________
void TGLVEntry::SetViewMode(EListViewMode viewMode)
{
   // Set the view mode for this list item.

   if (viewMode != fViewMode) {
      fViewMode = viewMode;
      if (viewMode == kLVLargeIcons)
         fCurrent = fBigPic;
      else
         fCurrent = fSmallPic;
      if (fActive) {
         if (fSelPic) delete fSelPic;
         fSelPic = new TGSelectedPicture(gClient, fCurrent);
      }
      gVirtualX->ClearWindow(fId);
      Resize(GetDefaultSize());
      if(fClient) fClient->NeedRedraw(this);
   }
}

//______________________________________________________________________________
void TGLVEntry::DoRedraw()
{
   // Redraw list view item.
   // List view item is placed and layouted in the container frame,
   // but is drawn in viewport.

   DrawCopy(fId, 0, 0);
}

//______________________________________________________________________________
void TGLVEntry::DrawCopy(Handle_t id, Int_t x, Int_t y)
{
   // Draw list view item in other window.
   // List view item is placed and layout in the container frame,
   // but is drawn in viewport.

   Int_t ix, iy, lx, ly;
   Int_t max_ascent, max_descent;

   gVirtualX->GetFontProperties(fFontStruct, max_ascent, max_descent);
   fTWidth = gVirtualX->TextWidth(fFontStruct, fName->GetString(), fName->GetLength());
   fTHeight = max_ascent + max_descent;

   if (fViewMode == kLVLargeIcons) {
      ix = (fWidth - fCurrent->GetWidth()) >> 1;
      iy = 0;
      lx = (fWidth - fTWidth) >> 1;
      ly = fHeight - (fTHeight + 1) - 2;
   } else {
      ix = 0;
      iy = (fHeight - fCurrent->GetHeight()) >> 1;
      lx = fCurrent->GetWidth() + 2;
      ly = (fHeight - (fTHeight + 1)) >> 1;
   }

   if (fActive) {
      if (fSelPic) fSelPic->Draw(id, fNormGC, x + ix, y + iy);
      gVirtualX->SetForeground(fNormGC, fgDefaultSelectedBackground);
      gVirtualX->FillRectangle(id, fNormGC, x + lx, y + ly, fTWidth, fTHeight + 1);
      gVirtualX->SetForeground(fNormGC, fClient->GetResourcePool()->GetSelectedFgndColor());
   } else {
      fCurrent->Draw(id, fNormGC, x + ix, y + iy);
      gVirtualX->SetForeground(fNormGC, fgWhitePixel);
      gVirtualX->FillRectangle(id, fNormGC, x + lx, y + ly, fTWidth, fTHeight + 1);
      gVirtualX->SetForeground(fNormGC, fgBlackPixel);
   }

   fName->Draw(id, fNormGC, x+lx, y+ly + max_ascent);
   gVirtualX->SetForeground(fNormGC, fgBlackPixel);

   if (fViewMode == kLVDetails) {
      if (fSubnames && fCpos && fJmode && fCtw) {
         int i;

         for (i = 0; fSubnames[i] != 0; ++i) {
            if (fJmode[i] == kTextRight)
               lx = fCpos[i+1] - fCtw[i] - 2;
            else if (fJmode[i] == kTextCenterX)
               lx = (fCpos[i] + fCpos[i+1] - fCtw[i]) >> 1;
            else // default to TEXT_LEFT
               lx = fCpos[i] + 2;

            if (x + lx < 0) break; // quick fix for mess in name
            fSubnames[i]->Draw(id, fNormGC, x + lx, y + ly + max_ascent);
         }
      }
   }
}

//______________________________________________________________________________
TGDimension TGLVEntry::GetDefaultSize() const
{
   // Get default size of list item.

   TGDimension size;
   TGDimension isize(fCurrent ? fCurrent->GetWidth() : 0,
                     fCurrent ? fCurrent->GetHeight() : 0);
   TGDimension lsize(fTWidth, fTHeight+1);

   switch (fViewMode) {
      default:
      case kLVLargeIcons:
         size.fWidth = TMath::Max(isize.fWidth, lsize.fWidth);
         size.fHeight = isize.fHeight + lsize.fHeight + 6;
         break;

      case kLVSmallIcons:
      case kLVList:
      case kLVDetails:
         size.fWidth = isize.fWidth + lsize.fWidth + 4;
         size.fHeight = TMath::Max(isize.fHeight, lsize.fHeight);
         break;
   }
   return size;
}

//______________________________________________________________________________
FontStruct_t TGLVEntry::GetDefaultFontStruct()
{
   if (!fgDefaultFont)
      fgDefaultFont = gClient->GetResourcePool()->GetIconFont();
   return fgDefaultFont->GetFontStruct();
}

//______________________________________________________________________________
const TGGC &TGLVEntry::GetDefaultGC()
{
   if (!fgDefaultGC) {
      fgDefaultGC = new TGGC(*gClient->GetResourcePool()->GetFrameGC());
      fgDefaultGC->SetFont(fgDefaultFont->GetFontHandle());
   }
   return *fgDefaultGC;
}


//______________________________________________________________________________
TGLVContainer::TGLVContainer(const TGWindow *p, UInt_t w, UInt_t h,
                             UInt_t options, ULong_t back) :
   TGContainer(p, w, h, options, back)
{
   // Create a list view container. This is the (large) frame that contains
   // all the list items. It will be shown through a TGViewPort (which is
   // created by the TGCanvas derived TGListView).

   fListView = 0;
   fLastActive = 0;
   fCpos = fJmode = 0;

   fViewMode = kLVLargeIcons;
   fItemLayout = new TGLayoutHints(kLHintsExpandY | kLHintsCenterX);

   SetLayoutManager(new TGTileLayout(this, 8));
}

//______________________________________________________________________________
TGLVContainer::TGLVContainer(TGCanvas *p,UInt_t options, ULong_t back) :
   TGContainer(p,options, back)
{
   // Create a list view container. This is the (large) frame that contains
   // all the list items. It will be shown through a TGViewPort (which is
   // created by the TGCanvas derived TGListView).

   fListView = 0;
   fLastActive = 0;
   fCpos = fJmode = 0;

   fViewMode = kLVLargeIcons;
   fItemLayout = new TGLayoutHints(kLHintsExpandY | kLHintsCenterX);

   SetLayoutManager(new TGTileLayout(this, 8));

   if (p->InheritsFrom(TGListView::Class())) fListView = (TGListView*)p;
}

//______________________________________________________________________________
TGLVContainer::~TGLVContainer()
{
   // Delete list view container.

   if (!MustCleanup()) {
      RemoveAll();
      delete fItemLayout;
   }
}

//______________________________________________________________________________
void  TGLVContainer::SetColHeaders(const char* n1,const char* n2,const char* n3,
                                   const char* n4,const char* n5,const char* n6,
                                   const char* n7,const char* n8,const char* n9,
                                   const char* n10,const char* n11,const char* n12)
{
   // set columns headers

   if (!fListView) return;

   Int_t ncol = -1;
   if (n12 && strlen(n12)) ncol=12;
   else if (n11 && strlen(n11)) ncol=11;
   else if (n10 && strlen(n10)) ncol=10;
   else if (n9 && strlen(n9)) ncol=9;
   else if (n8 && strlen(n8)) ncol=8;
   else if (n7 && strlen(n7)) ncol=7;
   else if (n6 && strlen(n6)) ncol=6;
   else if (n5 && strlen(n5)) ncol=5;
   else if (n4 && strlen(n4)) ncol=4;
   else if (n3 && strlen(n3)) ncol=3;
   else if (n2 && strlen(n2)) ncol=2;
   else if (n1 && strlen(n1)) ncol=1;

   if (ncol<0) return;

   fListView->SetHeaders(ncol);
   if (ncol>0) fListView->SetHeader(n1, kTextCenterX, kTextLeft , 0);
   if (ncol>1) fListView->SetHeader(n2, kTextCenterX, kTextLeft , 1);
   if (ncol>2) fListView->SetHeader(n3, kTextCenterX, kTextLeft , 2);
   if (ncol>3) fListView->SetHeader(n4, kTextCenterX, kTextLeft , 3);
   if (ncol>4) fListView->SetHeader(n5, kTextCenterX, kTextLeft , 4);
   if (ncol>5) fListView->SetHeader(n6, kTextCenterX, kTextLeft , 5);
   if (ncol>6) fListView->SetHeader(n7, kTextCenterX, kTextLeft , 6);
   if (ncol>7) fListView->SetHeader(n8, kTextCenterX, kTextLeft , 7);
   if (ncol>8) fListView->SetHeader(n9, kTextCenterX, kTextLeft , 8);
   if (ncol>9) fListView->SetHeader(n10, kTextCenterX, kTextLeft , 9);
   if (ncol>10) fListView->SetHeader(n11, kTextCenterX, kTextLeft , 10);
   if (ncol>11) fListView->SetHeader(n12, kTextCenterX, kTextLeft , 11);

   fListView->Layout();
}

//______________________________________________________________________________
void TGLVContainer::SetViewMode(EListViewMode viewMode)
{
   // Set list view mode for container.

   if (fViewMode != viewMode) {
      TGLayoutHints *oldLayout = fItemLayout;

      fViewMode = viewMode;
      if (fListView) fListView->SetViewMode(viewMode);

      if (viewMode == kLVLargeIcons)
         fItemLayout = new TGLayoutHints(kLHintsExpandY | kLHintsCenterX);
      else
         fItemLayout = new TGLayoutHints(kLHintsLeft | kLHintsCenterY);

      TGFrameElement *el;
      TIter next(fList);
      while ((el = (TGFrameElement *) next())) {
         el->fLayout = fItemLayout;
         ((TGLVEntry *) el->fFrame)->SetViewMode(viewMode);
      }
      delete oldLayout;

      switch (viewMode) {
         default:
         case kLVLargeIcons:
            SetLayoutManager(new TGTileLayout(this, 8));
            break;

         case kLVSmallIcons:
            SetLayoutManager(new TGTileLayout(this, 2));
            break;

         case kLVList:
            SetLayoutManager(new TGListLayout(this, 2));
            break;

         case kLVDetails:
            SetLayoutManager(new TGListDetailsLayout(this, 2));
            break;
      }

      TGCanvas *canvas = (TGCanvas *) this->GetParent()->GetParent();
      canvas->Layout();
   }
}

//______________________________________________________________________________
void TGLVContainer::SetColumns(Int_t *cpos, Int_t *jmode)
{
   // Set column information for list items.

   fCpos  = cpos;
   fJmode = jmode;

   TGFrameElement *el;
   TIter next(fList);
   while ((el = (TGFrameElement *) next())) {
      ((TGLVEntry *) el->fFrame)->SetColumns(fCpos, fJmode);
   }
   Layout();
}

//______________________________________________________________________________
TGDimension TGLVContainer::GetMaxItemSize() const
{
   // Get size of largest item in container.

   TGDimension csize, maxsize(0,0);

   TGFrameElement *el;
   TIter next(fList);
   while ((el = (TGFrameElement *) next())) {
      csize = el->fFrame->GetDefaultSize();
      maxsize.fWidth  = TMath::Max(maxsize.fWidth, csize.fWidth);
      maxsize.fHeight = TMath::Max(maxsize.fHeight, csize.fHeight);
   }
   if (fViewMode == kLVLargeIcons) {
      maxsize.fWidth  += 8;
      maxsize.fHeight += 8;
   } else {
      maxsize.fWidth  += 2;
      maxsize.fHeight += 2;
   }
   return maxsize;
}

//______________________________________________________________________________
Int_t TGLVContainer::GetMaxSubnameWidth(Int_t idx) const
{
   // Get width of largest subname in container.

   if (idx == 0) {
      return GetMaxItemSize().fWidth;
   }

   Int_t width, maxwidth = 0;

   TGFrameElement *el;
   TIter next(fList);
   while ((el = (TGFrameElement *) next())) {
      TGLVEntry *entry = (TGLVEntry *) el->fFrame;
      width = entry->GetSubnameWidth(idx-1);
      maxwidth = TMath::Max(maxwidth, width);
   }
   return maxwidth;
}

//______________________________________________________________________________
void TGLVContainer::RemoveItemWithData(void *userData)
{
   // Remove item with fUserData == userData from container.

   TGFrameElement *el;
   TIter next(fList);
   while ((el = (TGFrameElement *) next())) {
      TGLVEntry *f = (TGLVEntry *) el->fFrame;
      if (f->GetUserData() == userData) {
         RemoveItem(f);
         break;
      }
   }
}

//______________________________________________________________________________
void TGLVContainer::ActivateItem(TGFrameElement* el)
{
   // Select/activate item.

   TGContainer::ActivateItem(el);
   fLastActive = (TGLVEntry*)el->fFrame;
}

//______________________________________________________________________________
TGListView::TGListView(const TGWindow *p, UInt_t w, UInt_t h,
                       UInt_t options, ULong_t back) :
   TGCanvas(p, w, h, options, back)
{
   // Create a list view widget.

   fViewMode   = kLVLargeIcons;
   fNColumns   = 0;
   fColumns    = 0;
   fJmode      = 0;
   fColHeader  = 0;
   fFontStruct = GetDefaultFontStruct();
   fNormGC     = GetDefaultGC()();

   SetDefaultHeaders();
}

//______________________________________________________________________________
TGListView::~TGListView()
{
   // Delete a list view widget.

   if (fNColumns) {
      delete [] fColumns;
      delete [] fJmode;
      for (int i = 0; i < fNColumns; i++)
         delete fColHeader[i];
      delete [] fColHeader;
   }
}

//______________________________________________________________________________
void TGListView::SetHeaders(Int_t ncolumns)
{
   // Set number of headers, i.e. columns that will be shown in detailed view.
   // This method must be followed by exactly ncolumns SetHeader() calls,
   // making sure that every header (i.e. idx) is set (for and example see
   // SetDefaultHeaders()).

   if (ncolumns <= 0) {
      Error("SetHeaders", "number of columns must be > 0");
      return;
   }

   if (fNColumns) {
      delete [] fColumns;
      delete [] fJmode;
      for (int i = 0; i < fNColumns; i++) {
         if (fColHeader[i]) fColHeader[i]->DestroyWindow();
         delete fColHeader[i];
      }
      delete [] fColHeader;
   }

   fNColumns  = ncolumns+1;    // one extra for the blank filler header
   fColumns   = new int[fNColumns];
   fJmode     = new int[fNColumns];
   fColHeader = new TGTextButton* [fNColumns];
   for (int i = 0; i < fNColumns; i++)
      fColHeader[i] = 0;

   // create blank filler header
   fColHeader[fNColumns-1] = new TGTextButton(this, new TGHotString(""), -1,
                                              fNormGC, fFontStruct);
   fColHeader[fNColumns-1]->SetTextJustify(kTextCenterX | kTextCenterY);
   fColHeader[fNColumns-1]->SetState(kButtonDisabled);
   fColHeader[fNColumns-1]->SetState(kButtonDisabled);
   fJmode[fNColumns-1]   = kTextCenterX;
   fColumns[fNColumns-1] = 0;
}

//______________________________________________________________________________
void TGListView::SetHeader(const char *s, Int_t hmode, Int_t cmode, Int_t idx)
{
   // Set header button idx [0-fNColumns>, hmode is the x text alignmode
   // (ETextJustification) for the header text and cmode is the x text
   // alignmode for the item text.

   if (idx < 0 || idx >= fNColumns-1) {
      Error("SetHeader", Form("header index must be [0 - %d>", fNColumns-1));
      return;
   }
   delete fColHeader[idx];
   fColHeader[idx] = new TGTextButton(this, new TGHotString(s),
                                      idx, fNormGC, fFontStruct);
   fColHeader[idx]->SetTextJustify(hmode | kTextCenterY);

   // fJmode and fColumns contain values for columns idx > 0. idx==0 is
   // the small icon with the object name
   if (idx > 0)
      fJmode[idx-1] = cmode;

   if (!fColHeader[0]) return;
   int xl = fBorderWidth + fColHeader[0]->GetDefaultWidth() + 20 + 10;
   for (int i = 1; i < fNColumns; i++) {
      fColumns[i-1] = xl;
      if (!fColHeader[i]) break;
      xl += fColHeader[i]->GetDefaultWidth() + 20;
   }
}

//______________________________________________________________________________
const char *TGListView::GetHeader(Int_t idx) const
{
   // Returns name of header idx. If illegal idx or header not set for idx
   // 0 is returned.

   if (idx >= 0 && idx < fNColumns-1 && fColHeader[idx])
      return fColHeader[idx]->GetText()->GetString();
   return 0;
}

//______________________________________________________________________________
void TGListView::SetDefaultHeaders()
{
   // Default headers are: Name, Attributes, Size, Owner, Group, Modified.
   // The default is good for file system items.

   SetHeaders(6);
   SetHeader("Name",       kTextLeft,    kTextLeft,    0);
   SetHeader("Attributes", kTextCenterX, kTextCenterX, 1);
   SetHeader("Size",       kTextRight,   kTextRight,   2);
   SetHeader("Owner",      kTextCenterX, kTextCenterX, 3);
   SetHeader("Group",      kTextCenterX, kTextCenterX, 4);
   SetHeader("Modified",   kTextCenterX, kTextCenterX, 5);
}

//______________________________________________________________________________
void TGListView::SetViewMode(EListViewMode viewMode)
{
   // Set list view mode.

   TGLVContainer *container;

   if (fViewMode != viewMode) {
      fViewMode = viewMode;
      container = (TGLVContainer *) fVport->GetContainer();
      if (container) container->SetViewMode(viewMode);
      Layout();
   }
}

//______________________________________________________________________________
void TGListView::SetContainer(TGFrame *f)
{
   // Set list view container. Container must be at least of type
   // TGLVContainer.

   if (f->InheritsFrom(TGLVContainer::Class())) {
      TGCanvas::SetContainer(f);
      ((TGLVContainer *) f)->SetColumns(fColumns, fJmode);
      ((TGLVContainer *) f)->SetListView(this);
   } else
      Error("SetContainer", "frame must inherit from TGLVContainer");
}

//______________________________________________________________________________
void TGListView::Layout()
{
   // Layout list view components (container and contents of container).

   Int_t  i, xl = fBorderWidth;
   UInt_t w, h = 0;

   TGLVContainer *container = (TGLVContainer *) fVport->GetContainer();

   if (!container) {
      Error("Layout", "no listview container set yet");
      return;
   }
   fMaxSize = container->GetMaxItemSize();

   if (fViewMode == kLVDetails) {

      h = fColHeader[0]->GetDefaultHeight()-4;
      for (i = 0; i < fNColumns-1; ++i) {
         w = fColHeader[i]->GetDefaultWidth()+20;

         if (i == 0) w = TMath::Max(fMaxSize.fWidth + 10, w);
         if (i > 0)  w = TMath::Max(container->GetMaxSubnameWidth(i) + 40, (Int_t)w);

         fColHeader[i]->MoveResize(xl, fBorderWidth, w, h);
         fColHeader[i]->MapWindow();
         xl += w;
         fColumns[i] = xl-fBorderWidth-2;  // -2 is fSep in the layout routine
      }
      fColHeader[i]->MoveResize(xl, fBorderWidth, fVport->GetWidth()-xl+fBorderWidth, h);
      fColHeader[i]->MapWindow();
      fVScrollbar->RaiseWindow();

      container->SetColumns(fColumns, fJmode);

   } else {
      for (i = 0; i < fNColumns; ++i)
        fColHeader[i]->UnmapWindow();
   }

   TGLayoutManager *lm = container->GetLayoutManager();
   lm->SetDefaultWidth(xl);
   TGCanvas::Layout();

   if (fViewMode == kLVDetails) {
      fColHeader[i]->MoveResize(xl, fBorderWidth, fVport->GetWidth()-xl+fBorderWidth, h);
      fVport->MoveResize(fBorderWidth, fBorderWidth+h, fVport->GetWidth(), fVport->GetHeight()-h);
      fVScrollbar->SetRange(container->GetHeight(), fVport->GetHeight());
   }
}

//______________________________________________________________________________
Bool_t TGListView::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
   // Handle messages generated by the list view components.

   TGLVContainer *cnt = (TGLVContainer*)GetContainer();

   const TGLVEntry *entry;
   void *p = 0;

   entry = (TGLVEntry *) cnt->GetNextSelected(&p);

   switch (GET_SUBMSG(msg)) {
      case kCT_ITEMCLICK:
         if ((cnt->NumSelected() == 1) && (entry != 0)) {
            Int_t x = (Int_t)(parm2 & 0xffff);
            Int_t y = (Int_t)((parm2 >> 16) & 0xffff);
            Clicked((TGLVEntry*)entry, (Int_t)parm1);
            Clicked((TGLVEntry*)entry, (Int_t)parm1, x, y);
         }
         break;
      case kCT_ITEMDBLCLICK:
         if ((cnt->NumSelected() == 1) && (entry!=0)) {
            Int_t x = (Int_t)(parm2 & 0xffff);
            Int_t y = (Int_t)((parm2 >> 16) & 0xffff);
            DoubleClicked((TGLVEntry*)entry, (Int_t)parm1);
            DoubleClicked((TGLVEntry*)entry, (Int_t)parm1, x, y);
         }
         break;
      case kCT_SELCHANGED:
         SelectionChanged();
         break;
      default:
         break;
   }
   return TGCanvas::ProcessMessage(msg, parm1, parm2);
}

//______________________________________________________________________________
void TGListView::Clicked(TGLVEntry *entry, Int_t btn)
{
   // Emit Clicked() signal.

   Long_t args[2];

   args[0] = (Long_t)entry;
   args[1] = btn;

   Emit("Clicked(TGLVEntry*,Int_t)", args);
}

//______________________________________________________________________________
void TGListView::Clicked(TGLVEntry *entry, Int_t btn, Int_t x, Int_t y)
{
   // Emit Clicked() signal.

   Long_t args[4];

   args[0] = (Long_t)entry;
   args[1] = btn;
   args[2] = x;
   args[3] = y;

   Emit("Clicked(TGLVEntry*,Int_t,Int_t,Int_t)", args);
}

//______________________________________________________________________________
void TGListView::DoubleClicked(TGLVEntry *entry, Int_t btn)
{
   // Emit DoubleClicked() signal.

   Long_t args[2];

   args[0] = (Long_t)entry;
   args[1] = btn;

   Emit("DoubleClicked(TGLVEntry*,Int_t)", args);
}

//______________________________________________________________________________
void TGListView::DoubleClicked(TGLVEntry *entry, Int_t btn, Int_t x, Int_t y)
{
   // Emit DoubleClicked() signal.

   Long_t args[4];

   args[0] = (Long_t)entry;
   args[1] = btn;
   args[2] = x;
   args[3] = y;

   Emit("DoubleClicked(TGLVEntry*,Int_t,Int_t,Int_t)", args);
}

//______________________________________________________________________________
FontStruct_t TGListView::GetDefaultFontStruct()
{
   if (!fgDefaultFont)
      fgDefaultFont = gClient->GetResourcePool()->GetIconFont();
   return fgDefaultFont->GetFontStruct();
}

//______________________________________________________________________________
const TGGC &TGListView::GetDefaultGC()
{
   if (!fgDefaultGC) {
      fgDefaultGC = new TGGC(*gClient->GetResourcePool()->GetFrameGC());
      fgDefaultGC->SetFont(fgDefaultFont->GetFontHandle());
   }
   return *fgDefaultGC;
}

//______________________________________________________________________________
void TGListView::SavePrimitive(ofstream &out, Option_t *option)
{
   // Save a list view widget as a C++ statement(s) on output stream out.

   if (fBackground != GetDefaultFrameBackground()) SaveUserColor(out, option);

   out << endl << "   // list view" << endl;
   out <<"   TGListView *";
   out << GetName() << " = new TGListView(" << fParent->GetName()
       << "," << GetWidth() << "," << GetHeight();

   if (fBackground == GetDefaultFrameBackground()) {
      if (GetOptions() == (kSunkenFrame | kDoubleBorder)) {
         out <<");" << endl;
      } else {
         out << "," << GetOptionString() <<");" << endl;
      }
   } else {
      out << "," << GetOptionString() << ",ucolor);" << endl;
   }

   GetContainer()->SavePrimitive(out, option);

   out << endl;
   out << "   " << GetName() << "->SetContainer(" << GetContainer()->GetName()
                << ");" << endl;
   out << "   " << GetName() << "->SetViewMode(";
   switch (fViewMode) {
      case kLVLargeIcons:
         out << "kLVLargeIcons";
         break;
      case kLVSmallIcons:
         out << "kLVSmallIcons";
         break;
      case kLVList:
         out << "kLVList";
         break;
      case kLVDetails:
         out << "kLVDetails";
         break;
   }
   out << ");" << endl;

   out << "   " << GetContainer()->GetName() << "->Resize();" << endl;

   if (fHScrollbar && fHScrollbar->IsMapped()) {
   out << "   " << GetName() << "->SetHsbPosition(" << GetHsbPosition()
       << ");" << endl;
   }

   if (fVScrollbar && fVScrollbar->IsMapped()) {
   out << "   " << GetName() << "->SetVsbPosition(" << GetVsbPosition()
       << ");" << endl;
   }
}

//______________________________________________________________________________
void TGLVContainer::SavePrimitive(ofstream &out, Option_t *option)
{
   // Save a list view container as a C++ statement(s) on output stream out.

   if (fBackground != GetDefaultFrameBackground()) SaveUserColor(out, option);

   out << endl << "   // list view container" << endl;
   out << "   TGLVContainer *";

   if ((fParent->GetParent())->InheritsFrom(TGCanvas::Class())) {
      out << GetName() << " = new TGLVContainer(" << GetCanvas()->GetName();
   } else {
      out << GetName() << " = new TGLVContainer(" << fParent->GetName();
      out << "," << GetWidth() << "," << GetHeight();
   }
   if (fBackground == GetDefaultFrameBackground()) {
      if (GetOptions() == (kSunkenFrame | kDoubleBorder)) {
         out <<");" << endl;
      } else {
         out << "," << GetOptionString() <<");" << endl;
      }
   } else {
      out << "," << GetOptionString() << ",ucolor);" << endl;
   }
}
