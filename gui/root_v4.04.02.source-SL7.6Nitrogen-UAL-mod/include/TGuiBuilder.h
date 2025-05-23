// @(#)root/gui:$Name:  $:$Id: TGuiBuilder.h,v 1.1 2004/10/15 15:34:52 rdm Exp $
// Author: Valeriy Onuchin   12/08/04

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGuiBuilder
#define ROOT_TGuiBuilder


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGuiBuilder                                                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef ROOT_TNamed
#include "TNamed.h"
#endif

enum EGuiBldAction { kGuiBldNone, kGuiBldCtor,  kGuiBldProj,
                     kGuiBldMacro, kGuiBldFunc };

class TGFrame;
class TGLayoutHints;
//////////////////////////////////////////////////////////////////////////
class TGuiBldAction : public TNamed {

public:
   Int_t          fType;   // type of action
   TString        fAct;    // action, after action execution new frame is created
   const char    *fPic;    // picture name
   TGLayoutHints *fHints;  // layout hints for frame created by action

   TGuiBldAction(const char *name = 0, const char *title = 0,
                 Int_t type = kGuiBldCtor, TGLayoutHints *hints = 0);
   virtual ~TGuiBldAction();

   ClassDef(TGuiBldAction,0)  // gui builder action
};


//////////////////////////////////////////////////////////////////////////
class TGuiBuilder {

protected:
   TGuiBldAction *fAction;   // current action

public:
   TGuiBuilder();
   virtual ~TGuiBuilder();

   virtual void      AddAction(TGuiBldAction *, const char * /*section*/) {}
   virtual void      AddSection(const char * /*section*/) {}
   virtual TGFrame  *ExecuteAction() { return 0; }
   virtual void      SetAction(TGuiBldAction *act) { fAction = act; }
   TGuiBldAction    *GetAction() const { return fAction; }
   virtual Bool_t    IsExecutalble() const  { return fAction && !fAction->fAct.IsNull(); }
   virtual void      Show() {}
   virtual void      Hide() {}

   static  TGuiBuilder  *Instance();

   ClassDef(TGuiBuilder,0)  // ABC for gui builder
};

R__EXTERN TGuiBuilder *gGuiBuilder; // global gui builder

#endif
