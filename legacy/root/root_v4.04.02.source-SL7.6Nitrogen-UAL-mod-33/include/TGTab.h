// @(#)root/gui:$Name:  $:$Id: TGTab.h,v 1.12 2005/04/19 09:10:59 brun Exp $
// Author: Fons Rademakers   13/01/98

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGTab
#define ROOT_TGTab


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGTab, TGTabElement, TGTabLayout                                     //
//                                                                      //
// A tab widget contains a set of composite frames each with a little   //
// tab with a name (like a set of folders with tabs).                   //
//                                                                      //
// The TGTab is user callable. The TGTabElement and TGTabLayout are     //
// is a service classes of the tab widget.                              //
//                                                                      //
// Clicking on a tab will bring the associated composite frame to the   //
// front and generate the following event:                              //
// kC_COMMAND, kCM_TAB, tab id, 0.                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGWidget
#include "TGWidget.h"
#endif

class TList;
class TGTabElement;
class TGTab;

class TGTabLayout : public TGLayoutManager {

protected:
   TGTab    *fMain;      // container frame
   TList    *fList;      // list of frames to arrange

public:
   TGTabLayout(TGTab *main);

   virtual void Layout();
   virtual TGDimension GetDefaultSize() const;
   virtual void SavePrimitive(ofstream &out, Option_t *option);

   ClassDef(TGTabLayout,0)  // Layout manager for TGTab widget
};



class TGTab : public TGCompositeFrame, public TGWidget {

protected:
   Int_t               fCurrent;        // index of current tab
   UInt_t              fTabh;           // tab height
   TGCompositeFrame   *fContainer;      // main container
   TList              *fRemoved;        // list of removed tabs
   FontStruct_t        fFontStruct;     // font
   GContext_t          fNormGC;         // drawing context

   void ChangeTab(Int_t tabIndex);

   static const TGFont *fgDefaultFont;
   static const TGGC   *fgDefaultGC;

public:
   static FontStruct_t  GetDefaultFontStruct();
   static const TGGC   &GetDefaultGC();

   TGTab(const TGWindow *p = 0, UInt_t w = 1, UInt_t h = 1,
         GContext_t norm = GetDefaultGC()(),
         FontStruct_t font = GetDefaultFontStruct(),
         UInt_t options = kChildFrame,
         Pixel_t back = GetDefaultFrameBackground());
   virtual ~TGTab();

   virtual TGCompositeFrame *AddTab(TGString *text);
   virtual TGCompositeFrame *AddTab(const char *text);
   virtual void              RemoveTab(Int_t tabIndex);
   virtual Bool_t            SetTab(Int_t tabIndex);
   virtual Bool_t            SetTab(const char *name);
   virtual void              DrawBorder() { }

   TGCompositeFrame *GetContainer() const { return fContainer; }
   Int_t             GetCurrent() const { return fCurrent; }
   TGCompositeFrame *GetTabContainer(Int_t tabIndex) const;
   TGCompositeFrame *GetTabContainer(const char *name) const;
   TGTabElement     *GetTabTab(Int_t tabIndex) const;
   TGTabElement     *GetTabTab(const char *name) const;
   TGCompositeFrame *GetCurrentContainer() const { return GetTabContainer(fCurrent); }
   TGTabElement     *GetCurrentTab() const { return GetTabTab(fCurrent); }
   UInt_t            GetTabHeight() const { return fTabh; }
   Int_t             GetNumberOfTabs() const;
   virtual void      SetEnabled(Int_t tabIndex, Bool_t on = kTRUE);
   Bool_t            IsEnabled(Int_t tabIndex) const;

   virtual void      SavePrimitive(ofstream &out, Option_t *option);

   virtual void Selected(Int_t id) { Emit("Selected(Int_t)", id); }  //*SIGNAL*

   ClassDef(TGTab,0)  // Tab widget
};



class TGTabElement : public TGFrame {

protected:
   TGString        *fText;            // text on tab
   GContext_t       fNormGC;          // graphics context for drawing tab
   FontStruct_t     fFontStruct;      // font used for tab
   UInt_t           fTWidth;          // width of tab text
   UInt_t           fTHeight;         // height of tab text
   Bool_t           fEnabled;         // enabled or disabled

public:
   TGTabElement(const TGWindow *p = 0, TGString *text = 0, UInt_t w = 1, UInt_t h = 1,
                GContext_t norm = TGTab::GetDefaultGC()(), 
                FontStruct_t font = TGTab::GetDefaultFontStruct(),
                UInt_t options = kRaisedFrame,
                Pixel_t back = GetDefaultFrameBackground());
   virtual ~TGTabElement();

   virtual void        DrawBorder();
   virtual TGDimension GetDefaultSize() const;
   const TGString     *GetText() const { return fText; }
   const char         *GetString() const { return fText->GetString(); }
   virtual Bool_t      HandleButton(Event_t *event);
   void                SetText(TGString *text);
   virtual void        SetEnabled(Bool_t on = kTRUE) { fEnabled = on; }
   Bool_t              IsEnabled() const { return fEnabled; }

   ClassDef(TGTabElement,0)  // Little tab on tab widget
};


#endif
