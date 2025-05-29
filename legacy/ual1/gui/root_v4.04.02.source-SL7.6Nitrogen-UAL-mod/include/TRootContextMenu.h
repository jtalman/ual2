// @(#)root/gui:$Name:  $:$Id: TRootContextMenu.h,v 1.4 2004/09/08 08:13:11 brun Exp $
// Author: Fons Rademakers   12/02/98

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TRootContextMenu
#define ROOT_TRootContextMenu


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TRootContextMenu                                                     //
//                                                                      //
// This class provides an interface to context sensitive popup menus.   //
// These menus pop up when the user hits the right mouse button, and    //
// are destroyed when the menu pops downs.                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TContextMenuImp
#include "TContextMenuImp.h"
#endif
#ifndef ROOT_TGMenu
#include "TGMenu.h"
#endif

class TRootDialog;


class TRootContextMenu : public TGPopupMenu, public TContextMenuImp {

private:
   TRootDialog *fDialog;    // dialog prompting for command line arguments
   TList       *fTrash;     // list of objects to be deleted before refilling menu

   void CreateMenu(TObject *object);

public:
   TRootContextMenu(TContextMenu *c = 0, const char *name = "ROOT Context Menu");
   virtual ~TRootContextMenu();

   virtual void DisplayPopup(Int_t x, Int_t y);
   virtual void Dialog(TObject *object, TMethod *method);
   virtual void Dialog(TObject *object, TFunction *function);
   TRootDialog *GetDialog() const { return fDialog; };

   Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

   ClassDef(TRootContextMenu,0)  //ROOT native GUI context sensitive popup menu
};

#endif
