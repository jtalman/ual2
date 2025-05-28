// @(#)root/base:$Name:  $:$Id: TBrowser.h,v 1.11 2004/06/22 15:36:42 brun Exp $
// Author: Fons Rademakers   25/10/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/


#ifndef ROOT_TBrowser
#define ROOT_TBrowser

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TBrowser                                                             //
//                                                                      //
// Using a TBrowser on can browse all ROOT objects. It shows in a list  //
// on the left side of the window all browsable ROOT classes. Selecting //
// one of the classes displays, in the iconbox on the right side, all   //
// objects in the class. Selecting one of the objects in the iconbox,   //
// will place all browsable objects in a new list and draws the         //
// contents of the selected class in the iconbox. And so on....         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TNamed
#include "TNamed.h"
#endif
#ifndef ROOT_TBrowserImp
#include "TBrowserImp.h"
#endif


class TContextMenu;
class TBrowserTimer;


class TBrowser : public TNamed {

private:
   TObject       *fLastSelectedObject; //!The last TObject selected by user

protected:
   TBrowserImp   *fImp;                //!Window system specific browser implementation
   TBrowserTimer *fTimer;              //!Browser's timer
   TContextMenu  *fContextMenu;        //!Context menu pointer
   Bool_t         fNeedRefresh;        //True if the browser needs refresh

public:
   enum {
      kNoHidden     = BIT(9)   // don't show '.' files and directories
   };

   TBrowser(const char *name="Browser", const char *title="ROOT Object Browser");
   TBrowser(const char *name, const char *title, UInt_t width, UInt_t height);
   TBrowser(const char *name, const char *title, Int_t x, Int_t y, UInt_t width, UInt_t height);

   TBrowser(const char *name, TObject *obj, const char *title="ROOT Object Browser");
   TBrowser(const char *name, TObject *obj, const char *title, UInt_t width, UInt_t height);
   TBrowser(const char *name, TObject *obj, const char *title, Int_t x, Int_t y, UInt_t width, UInt_t height);

   TBrowser(const char *name, void *obj, TClass *cl, const char *objname="", const char *title="ROOT Foreign Browser");
   TBrowser(const char *name, void *obj, TClass *cl, const char *objname, const char *title, UInt_t width, UInt_t height);
   TBrowser(const char *name, void *obj, TClass *cl, const char *objname, const char *title, Int_t x, Int_t y, UInt_t width, UInt_t height);

   // In a world with only standard C++ compliant compilers, we could also add:
   // template <class T>  TBrowser(const char *name, T *obj, const char *objname="", const char *title="ROOT Foreign Browser") :
   //       : TNamed(name, title), fLastSelectedObject(0), fTimer(0), fContextMenu(0),
   //            fNeedRefresh(kFALSE)
   // {
   //    Create a new browser with a name, title, width and height for TObject *obj.
   //
   //    fImp = gGuiFactory->CreateBrowserImp(this, title, width, height);
   //    Create(new TBrowserObject(obj,gROOT->GetClass(typeid(T)),objname));
   // }

   virtual ~TBrowser();

   void          Add(TObject *obj,             const char *name = 0);
   void          Add(void    *obj, TClass *cl, const char *name = 0);
   virtual void  Create(TObject *obj = 0);      // Create this Browser
   void          ExecuteDefaultAction(TObject *obj);
   TBrowserImp  *GetBrowserImp() const         { return fImp; }
   TContextMenu *GetContextMenu() const        { return fContextMenu; }
   Bool_t        GetRefreshFlag() const        { return fNeedRefresh; }
   TObject      *GetSelected() const           { return fLastSelectedObject; }
   void          SetRefreshFlag(Bool_t flag)   { fNeedRefresh = flag; }
   void          Iconify()                     { fImp->Iconify(); }
   virtual void  RecursiveRemove(TObject *obj);
   void          Refresh();
   void          SetSelected(TObject *clickedObject);
   void          Show()                        { fImp->Show(); }
   void          SetDrawOption(Option_t *option="") { fImp->SetDrawOption(option); }
   Option_t     *GetDrawOption() const { return  fImp->GetDrawOption(); }

   ClassDef(TBrowser,0)  //ROOT Object Browser
};

#endif
