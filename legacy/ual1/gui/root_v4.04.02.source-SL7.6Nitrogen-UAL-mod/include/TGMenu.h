// @(#)root/gui:$Name:  $:$Id: TGMenu.h,v 1.27 2004/09/08 08:13:11 brun Exp $
// Author: Fons Rademakers   09/01/98

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGMenu
#define ROOT_TGMenu


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGMenuBar, TGPopupMenu, TGMenuTitle and TGMenuEntry                  //
//                                                                      //
// This header contains all different menu classes.                     //
//                                                                      //
// Selecting a menu item will generate the event:                       //
// kC_COMMAND, kCM_MENU, menu id, user data.                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGString
#include "TGString.h"
#endif
#ifndef ROOT_TGPicture
#include "TGPicture.h"
#endif
#ifndef ROOT_TGWidget
#include "TGWidget.h"
#endif


//--- Menu entry status mask

enum EMenuEntryState {
   kMenuActiveMask     = BIT(0),
   kMenuEnableMask     = BIT(1),
   kMenuDefaultMask    = BIT(2),
   kMenuCheckedMask    = BIT(3),
   kMenuRadioMask      = BIT(4),
   kMenuHideMask       = BIT(5),
   kMenuRadioEntryMask = BIT(6)
};

//--- Menu entry types

enum EMenuEntryType {
   kMenuSeparator,
   kMenuLabel,
   kMenuEntry,
   kMenuPopup
};


class TGPopupMenu;
class TGMenuBar;
class TGMenuTitle;
class TTimer;


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGMenuEntry                                                          //
//                                                                      //
// This class contains all information about a menu entry.              //
// It is a fully protected class used internally by TGPopupMenu.        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TGMenuEntry : public TObject {

friend class TGPopupMenu;

protected:
   Int_t             fEntryId;   // the entry id (used for event processing)
   void             *fUserData;  // pointer to user data structure
   EMenuEntryType    fType;      // type of entry
   Int_t             fStatus;    // entry status (OR of EMenuEntryState)
   Int_t             fEx, fEy;   // position of entry
   UInt_t            fEw, fEh;   // width and height of entry
   TGHotString      *fLabel;     // menu entry label
   const TGPicture  *fPic;       // menu entry icon
   TGPopupMenu      *fPopup;     // pointer to popup menu (in case of cascading menus)

public:
   TGMenuEntry() {fUserData=0; fLabel=0; fPic=0; fPopup=0;}
   virtual ~TGMenuEntry() { if (fLabel) delete fLabel; }

   Int_t          GetEntryId() const { return fEntryId; }
   const char    *GetName() const { return fLabel ? fLabel->GetString() : 0; }
   virtual Int_t  GetStatus() const { return fStatus; }
   EMenuEntryType GetType() const { return fType; }
   TGPopupMenu   *GetPopup() const { return fPopup; }
   TGHotString   *GetLabel() const  { return fLabel; }

   ClassDef(TGMenuEntry,0);
};


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGPopupMenu                                                          //
//                                                                      //
// This class creates a popup menu object. Popup menu's are attached    //
// to TGMenuBar objects.                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TGPopupMenu : public TGFrame {

friend class TGMenuTitle;
friend class TGMenuBar;

protected:
   TList             *fEntryList;     // list of menu entries
   TGMenuEntry       *fCurrent;       // currently selected menu entry
   Bool_t             fStick;         // stick mode (popup menu stays sticked on screen)
   Bool_t             fHasGrab;       // true if menu has grabbed pointer
   Bool_t             fPoppedUp;      // true if menu is currently popped up
   UInt_t             fXl;            // Max width of all menu entries
   UInt_t             fMenuWidth;     // width of popup menu
   UInt_t             fMenuHeight;    // height of popup menu
   TTimer            *fDelay;         // delay before poping up cascading menu
   GContext_t         fNormGC;        // normal drawing graphics context
   GContext_t         fSelGC;         // graphics context for drawing selections
   GContext_t         fSelbackGC;     // graphics context for drawing selection background
   FontStruct_t       fFontStruct;    // font to draw menu entries
   FontStruct_t       fHifontStruct;  // font to draw highlighted entries
   Cursor_t           fDefaultCursor; // right pointing cursor
   const TGWindow    *fMsgWindow;     // window which handles menu events
   TGMenuBar         *fMenuBar;       // menu bar (if any)

   static const TGFont *fgDefaultFont;
   static const TGFont *fgHilightFont;
   static const TGGC   *fgDefaultGC;
   static const TGGC   *fgDefaultSelectedGC;
   static const TGGC   *fgDefaultSelectedBackgroundGC;

   void DrawTrianglePattern(GContext_t gc, Int_t l, Int_t t, Int_t r, Int_t b);
   void DrawCheckMark(GContext_t gc, Int_t l, Int_t t, Int_t r, Int_t b);
   void DrawRCheckMark(GContext_t gc, Int_t l, Int_t t, Int_t r, Int_t b);
   virtual void DoRedraw();
   virtual void DrawEntry(TGMenuEntry *entry);
   virtual void Reposition();

   static FontStruct_t  GetDefaultFontStruct();
   static FontStruct_t  GetHilightFontStruct();
   static const TGGC   &GetDefaultGC();
   static const TGGC   &GetDefaultSelectedGC();
   static const TGGC   &GetDefaultSelectedBackgroundGC();

public:
   TGPopupMenu(const TGWindow *p = 0, UInt_t w = 10, UInt_t h = 10,
               UInt_t options = 0);
   virtual ~TGPopupMenu();

   virtual void AddEntry(TGHotString *s, Int_t id, void *ud = 0,
                         const TGPicture *p = 0, TGMenuEntry *before = 0);
   virtual void AddEntry(const char *s, Int_t id, void *ud = 0,
                         const TGPicture *p = 0, TGMenuEntry *before = 0);
   virtual void AddSeparator(TGMenuEntry *before = 0);
   virtual void AddLabel(TGHotString *s, const TGPicture *p = 0,
                         TGMenuEntry *before = 0);
   virtual void AddLabel(const char *s, const TGPicture *p = 0,
                         TGMenuEntry *before = 0);
   virtual void AddPopup(TGHotString *s, TGPopupMenu *popup,
                         TGMenuEntry *before = 0);
   virtual void AddPopup(const char *s, TGPopupMenu *popup,
                         TGMenuEntry *before = 0);
   virtual void EnableEntry(Int_t id);
   virtual void DisableEntry(Int_t id);
   virtual Bool_t IsEntryEnabled(Int_t id);
   virtual void   HideEntry(Int_t id);
   virtual Bool_t IsEntryHidden(Int_t id);
   virtual void   DefaultEntry(Int_t id);
   virtual void   CheckEntry(Int_t id);
   virtual void   UnCheckEntry(Int_t id);
   virtual Bool_t IsEntryChecked(Int_t id);
   virtual void   RCheckEntry(Int_t id, Int_t IDfirst, Int_t IDlast);
   virtual Bool_t IsEntryRChecked(Int_t id);
   virtual void   PlaceMenu(Int_t x, Int_t y, Bool_t stick_mode,
                            Bool_t grab_pointer);
   virtual Int_t  EndMenu(void *&userData);
   virtual void   DeleteEntry(Int_t id);
   virtual void   DeleteEntry(TGMenuEntry *entry);
   virtual TGMenuEntry *GetEntry(Int_t id);
   virtual TGMenuEntry *GetCurrent() const { return fCurrent; }
   virtual TGMenuEntry *GetEntry(const char *s);
   const TList    *GetListOfEntries() const { return fEntryList; }
   virtual void    DrawBorder();
   virtual Bool_t  HandleButton(Event_t *event);
   virtual Bool_t  HandleMotion(Event_t *event);
   virtual Bool_t  HandleCrossing(Event_t *event);
   virtual Bool_t  HandleTimer(TTimer *t);
   virtual void    Associate(const TGWindow *w) { fMsgWindow = w; }
   virtual void    SetMenuBar(TGMenuBar *bar) { fMenuBar = bar; }
   TGMenuBar  *GetMenuBar() const {  return fMenuBar; }

   virtual void PoppedUp() { Emit("PoppedUp()"); }  //*SIGNAL
   virtual void PoppedDown() { Emit("PoppedDown()"); }  //*SIGNAL
   virtual void Highlighted(Int_t id) { Emit("Highlighted(Int_t)", id); }  //*SIGNAL*
   virtual void Activated(Int_t id) { Emit("Activated(Int_t)", id); }  //*SIGNAL*
   virtual void Activate(Bool_t) { }
   virtual void Activate(TGMenuEntry *entry);
   virtual void SavePrimitive(ofstream &out, Option_t *option);

   ClassDef(TGPopupMenu,0)  // Popup menu
};


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGMenuTitle                                                          //
//                                                                      //
// This class creates a menu title. A menu title is a frame             //
// to which a popup menu can be attached. Menu titles are automatically //
// created when adding a popup menu to a menubar.                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TGMenuTitle : public TGFrame {

protected:
   TGPopupMenu    *fMenu;             // attached popup menu
   TGHotString    *fLabel;            // menu title
   Int_t           fTitleId;          // id of selected menu item
   void           *fTitleData;        // user data associated with selected item
   Bool_t          fState;            // menu title state (active/not active)
   Int_t           fHkeycode;         // hot key code
   FontStruct_t    fFontStruct;       // font
   GContext_t      fNormGC, fSelGC;   // normal and selection graphics contexts

   virtual void DoRedraw();

   static const TGFont *fgDefaultFont;
   static const TGGC   *fgDefaultSelectedGC;
   static const TGGC   *fgDefaultGC;

public:
   static FontStruct_t  GetDefaultFontStruct();
   static const TGGC   &GetDefaultSelectedGC();
   static const TGGC   &GetDefaultGC();

   TGMenuTitle(const TGWindow *p = 0, TGHotString *s = 0, TGPopupMenu *menu = 0,
               GContext_t norm = GetDefaultGC()(),
               FontStruct_t font = GetDefaultFontStruct(),
               UInt_t options = 0);
   virtual ~TGMenuTitle() { if (fLabel) delete fLabel; }

   virtual void SetState(Bool_t state);
   Bool_t       GetState() const { return fState; }
   Int_t        GetHotKeyCode() const { return fHkeycode; }
   TGPopupMenu *GetMenu() const { return fMenu; }
   const char  *GetName() const { return fLabel ? fLabel->GetString() : 0; }
   virtual void DoSendMessage();
   virtual void SavePrimitive(ofstream &out, Option_t *option);

   ClassDef(TGMenuTitle,0)  // Menu title class
};


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGMenuBar                                                            //
//                                                                      //
// This class creates a menu bar.                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TGMenuBar : public TGHorizontalFrame {

friend class TGPopupMenu;

protected:
   TGMenuTitle  *fCurrent;       // current menu title
   TList        *fTitles;        // list of menu titles
   Cursor_t      fDefaultCursor; // right pointing cursor
   Bool_t        fStick;         // stick mode (popup menu stays sticked on screen)
   TList        *fTrash;         // garbage
   Bool_t        fKeyNavigate;   // kTRUE if arrow key navigation is on

   virtual void AddFrameBefore(TGFrame *f, TGLayoutHints *l = 0,
                               TGPopupMenu *before = 0);

   virtual void BindKeys(Bool_t on = kTRUE);
   virtual void BindHotKey(Int_t keycode, Bool_t on = kTRUE);

public:
   TGMenuBar(const TGWindow *p = 0, UInt_t w = 60, UInt_t h = 20,
             UInt_t options = kHorizontalFrame | kRaisedFrame);
   virtual ~TGMenuBar();

   virtual void AddPopup(TGHotString *s, TGPopupMenu *menu, TGLayoutHints *l,
                         TGPopupMenu *before = 0);
   virtual void AddPopup(const char *s, TGPopupMenu *menu, TGLayoutHints *l,
                         TGPopupMenu *before = 0);
   virtual TGPopupMenu *AddPopup(const TString &s, Int_t padleft = 4, Int_t padright = 0,
                                 Int_t padtop = 0, Int_t padbottom = 0);

   virtual TGPopupMenu *GetPopup(const char *s);
   virtual TGPopupMenu *RemovePopup(const char *s);

   virtual Bool_t HandleButton(Event_t *event);
   virtual Bool_t HandleMotion(Event_t *event);
   virtual Bool_t HandleKey(Event_t *event);
   virtual void   SavePrimitive(ofstream &out, Option_t *option);

   ClassDef(TGMenuBar,0)  // Menu bar class
};

#endif
