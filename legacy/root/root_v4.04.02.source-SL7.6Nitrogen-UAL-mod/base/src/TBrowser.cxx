// @(#)root/base:$Name:  $:$Id: TBrowser.cxx,v 1.11 2004/01/10 10:52:29 brun Exp $
// Author: Fons Rademakers   25/10/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Using a TBrowser one can browse all ROOT objects. It shows in a list //
// on the left side of the window all browsable ROOT classes. Selecting //
// one of the classes displays, in the iconbox on the right side, all   //
// objects in the class. Selecting one of the objects in the iconbox,   //
// will place all browsable objects in a new list and draws the         //
// contents of the selected class in the iconbox. And so on....         //
//                                                                      //
//Begin_Html <img src="gif/browser.gif"> End_Html                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TBrowser.h"
#include "TGuiFactory.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TTimer.h"
#include "TContextMenu.h"
#include "TInterpreter.h"

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TBrowserTimer                                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TBrowserTimer : public TTimer {

protected:
   TBrowser *fBrowser;
   Bool_t    fActivate;

public:
   TBrowserTimer(TBrowser *b, Long_t ms = 1000)
      : TTimer(ms, kTRUE), fBrowser(b), fActivate(kFALSE) { }
   Bool_t Notify();
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TBrowserObject                                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TBrowserObject : public TNamed 
{
   // This class is designed to wrap a Foreign object in order to 
   // inject it into the Browse sub-system.

public: 

   TBrowserObject(void *obj, TClass *cl, const char *brname);
  ~TBrowserObject(){;}

   void    Browse(TBrowser* b);
   Bool_t  IsFolder() const;
   TClass *IsA() const { return fClass; }

private:
   void     *fObj;   //! pointer to the foreign object
   TClass   *fClass; //! pointer to class of the foreign object

};  


ClassImp(TBrowser)

//______________________________________________________________________________
TBrowser::TBrowser(const char *name, const char *title)
   : TNamed(name, title), fLastSelectedObject(0), fTimer(0),
     fContextMenu(0), fNeedRefresh(kFALSE)
{
   // Create a new browser with a name, title. Width and height are by
   // default set to 640x400 and (optionally) adjusted by the screen factor
   // (depending on Rint.Canvas.UseScreenFactor to be true or false, default
   // is true).

   if (TClass::IsCallingNew()) {
      fImp = 0;
   } else {
      Float_t cx = gStyle->GetScreenFactor();
      UInt_t w = UInt_t(cx*640);
      UInt_t h = UInt_t(cx*400);

      fImp = gGuiFactory->CreateBrowserImp(this, title, w, h);
      Create();
   }
}

//______________________________________________________________________________
TBrowser::TBrowser(const char *name, const char *title, 
                   UInt_t width, UInt_t height)
   : TNamed(name, title), fLastSelectedObject(0), fTimer(0), fContextMenu(0),
     fNeedRefresh(kFALSE)
{
   // Create a new browser with a name, title, width and height.

   fImp = gGuiFactory->CreateBrowserImp(this, title, width, height);
   Create();
}

//______________________________________________________________________________
TBrowser::TBrowser(const char *name, const char *title, 
                   Int_t x, Int_t y, 
                   UInt_t width, UInt_t height)
   : TNamed(name, title), fLastSelectedObject(0), fTimer(0), fContextMenu(0),
     fNeedRefresh(kFALSE)
{
   // Create a new browser with a name, title, position, width and height.

   fImp = gGuiFactory->CreateBrowserImp(this, title, x, y, width, height);
   Create();
}

//______________________________________________________________________________
TBrowser::TBrowser(const char *name, TObject *obj, const char *title)
   : TNamed(name, title), fLastSelectedObject(0), fTimer(0), fContextMenu(0),
     fNeedRefresh(kFALSE)
{
   // Create a new browser with a name, title, width and height for TObject *obj.

   Float_t cx = gStyle->GetScreenFactor();
   UInt_t w = UInt_t(cx*640);
   UInt_t h = UInt_t(cx*400);

   fImp = gGuiFactory->CreateBrowserImp(this, title, w, h);
   Create(obj);
}

//______________________________________________________________________________
TBrowser::TBrowser(const char *name, TObject *obj, const char *title, 
                   UInt_t width, UInt_t height)
   : TNamed(name, title), fLastSelectedObject(0), fTimer(0), fContextMenu(0),
     fNeedRefresh(kFALSE)
{
   // Create a new browser with a name, title, width and height for TObject *obj.

   fImp = gGuiFactory->CreateBrowserImp(this, title, width, height);
   Create(obj);
}

//______________________________________________________________________________
TBrowser::TBrowser(const char *name, TObject *obj, const char *title, 
                   Int_t x, Int_t y, 
                   UInt_t width, UInt_t height)
   : TNamed(name, title), fLastSelectedObject(0), fTimer(0), fContextMenu(0),
     fNeedRefresh(kFALSE)
{
   // Create a new browser with a name, title, width and height for TObject *obj.

   fImp = gGuiFactory->CreateBrowserImp(this, title, x, y, width, height);
   Create(obj);
}

//______________________________________________________________________________
TBrowser::TBrowser(const char *name, void *obj, TClass *cl, 
                   const char *objname, const char *title)
   : TNamed(name, title), fLastSelectedObject(0), fTimer(0), fContextMenu(0),
     fNeedRefresh(kFALSE)
{
   // Create a new browser with a name, title, width and height for TObject *obj.

   Float_t cx = gStyle->GetScreenFactor();
   UInt_t w = UInt_t(cx*640);
   UInt_t h = UInt_t(cx*400);

   fImp = gGuiFactory->CreateBrowserImp(this, title, w, h);

   Create(new TBrowserObject(obj,cl,objname));
}

//______________________________________________________________________________
TBrowser::TBrowser(const char *name, void *obj, TClass *cl, 
                   const char *objname, const char *title, 
                   UInt_t width, UInt_t height)
   : TNamed(name, title), fLastSelectedObject(0), fTimer(0), fContextMenu(0),
     fNeedRefresh(kFALSE)
{
   // Create a new browser with a name, title, width and height for TObject *obj.

   fImp = gGuiFactory->CreateBrowserImp(this, title, width, height);
   Create(new TBrowserObject(obj,cl,objname));
}

//______________________________________________________________________________
TBrowser::TBrowser(const char *name,void *obj,  TClass *cl, 
                   const char *objname, const char *title, 
                   Int_t x, Int_t y, 
                   UInt_t width, UInt_t height)
   : TNamed(name, title), fLastSelectedObject(0), fTimer(0), fContextMenu(0),
     fNeedRefresh(kFALSE)
{
   // Create a new browser with a name, title, width and height for TObject *obj.

   fImp = gGuiFactory->CreateBrowserImp(this, title, x, y, width, height);
   Create(new TBrowserObject(obj,cl,objname));
}

//______________________________________________________________________________
TBrowser::~TBrowser()
{
   // Delete the browser.

   gROOT->GetListOfBrowsers()->Remove(this);
   delete fContextMenu;
   delete fTimer;
   delete fImp;
}

//______________________________________________________________________________
void TBrowser::Add(TObject *obj, const char *name)
{
   // Add object with name to browser. If name not set the objects GetName()
   // is used.

   if (obj && fImp) {
      fImp->Add(obj, name);
      obj->SetBit(kMustCleanup);
   }
}

//______________________________________________________________________________
void TBrowser::Add(void *obj, TClass *cl, const char *name)
{
   // Add foreign object with name to browser. 
   // 'cl' is the type use to store the value of obj.
   // So literally the following pseudo code should be correct:
   //    `cl->GetName()` * ptr = (`cl->GetName()`*) obj;
   // and the value of obj is not necessarily the start of the object.

   if (!obj || !cl) return;
   TObject *to; 
   if (cl->IsTObject()) to = (TObject*)cl->DynamicCast(TObject::Class(),obj,kTRUE); 
   else                 to = new TBrowserObject(obj,cl,name);

   if (!to) return;
   Add(to,name);
}

//______________________________________________________________________________
void TBrowser::Create(TObject *obj)
{
   // Create the browser, called by the ctors.

   fNeedRefresh = kFALSE;

   fTimer = new TBrowserTimer(this);
   gSystem->AddTimer(fTimer);

   gROOT->GetListOfBrowsers()->Add(this);

   // Get the list of globals
   gROOT->GetListOfGlobals(kTRUE);
   gROOT->GetListOfGlobalFunctions(kTRUE);

   fContextMenu = new TContextMenu("BrowserContextMenu") ;

   // Fill the first list from the present TObject obj
   if (obj) {
      Add(obj);
      if (fImp) fImp->BrowseObj(obj);
   } else if (fImp) {
      // Fill the first list with all browsable classes from TROOT
      fImp->BrowseObj(gROOT);
   }

   // The first list will be filled by TWin32BrowserImp ctor
   // with all browsable classes from TROOT
}

//______________________________________________________________________________
void TBrowser::ExecuteDefaultAction(TObject *obj)
{
   // Execute default action for selected object (action is specified
   // in the $HOME/.root.mimes or $ROOTSYS/etc/root.mimes file.

   if (obj && fImp)
      fImp->ExecuteDefaultAction(obj);
}

//______________________________________________________________________________
void TBrowser::RecursiveRemove(TObject *obj)
{
   // Recursively remove obj from browser.

   if (fImp && obj) {
      fImp->RecursiveRemove(obj);
      fNeedRefresh = kTRUE;
   }
}

//______________________________________________________________________________
void TBrowser::Refresh()
{
   // Refresh browser contents.

   fNeedRefresh = kTRUE;
   if (fImp) fImp->Refresh();
   fNeedRefresh = kFALSE;
}

//______________________________________________________________________________
void TBrowser::SetSelected(TObject *clickedObject)
{
   // Assign the last selected object.

   fLastSelectedObject = clickedObject;
}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//  TBrowserTimer                                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
Bool_t TBrowserTimer::Notify()
{
   // Called whenever timer times out.

   if (fBrowser) {
      if (fBrowser->GetRefreshFlag()) {
         fBrowser->SetRefreshFlag(kFALSE);
         fActivate = kTRUE;
      } else if (fActivate) {
         fActivate = kFALSE;
         fBrowser->Refresh();
      }
   }
   Reset();

   return kFALSE;
}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//  TBrowserObject                                                      //
//                                                                      //
//  This is a wrapper class to emulate the TObject interface            //
//  around an object of a non-TObject class                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
TBrowserObject::TBrowserObject(void *obj, TClass *cl, const char *brname)
   : TNamed(brname,cl->GetName()), fObj(obj), fClass(cl)
{
   if (cl==0) Fatal("Constructor","Class parameter should not be null");
   SetBit(kCanDelete);
}

//______________________________________________________________________________
Bool_t TBrowserObject::IsFolder() const
{ 
   return fClass->IsFolder(fObj);
}

//______________________________________________________________________________
void TBrowserObject::Browse(TBrowser* b)
{
   fClass->Browse(fObj, b);
}

