// @(#)root/gui:$Name:  $:$Id: TRootContextMenu.cxx,v 1.12 2005/03/31 12:51:43 brun Exp $
// Author: Fons Rademakers   12/02/98

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TRootContextMenu                                                     //
//                                                                      //
// This class provides an interface to context sensitive popup menus.   //
// These menus pop up when the user hits the right mouse button, and    //
// are destroyed when the menu pops downs.                              //
// The picture below shows a canvas with a pop-up menu.                 //
//                                                                      //
//Begin_Html <img src="gif/hsumMenu.gif"> End_Html                      //
//                                                                      //
// The picture below shows a canvas with a pop-up menu and a dialog box.//
//                                                                      //
//Begin_Html <img src="gif/hsumDialog.gif"> End_Html                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TRootContextMenu.h"
#include "TROOT.h"
#include "TGClient.h"
#include "TList.h"
#include "TContextMenu.h"
#include "TMethod.h"
#include "TMethodArg.h"
#include "TClass.h"
#include "TVirtualX.h"
#include "TCanvas.h"
#include "TDataMember.h"
#include "TToggle.h"
#include "TRootDialog.h"
#include "TDataType.h"
#include "TCanvas.h"
#include "TBrowser.h"
#include "TRootCanvas.h"
#include "TRootBrowser.h"
#include "TClassMenuItem.h"


enum {
   kToggleStart       = 1000, // first id of toggle menu items
   kToggleListStart   = 2000, // first id of toggle list menu items
   kUserFunctionStart = 3000  // first id of user added functions/methods, etc...
};


ClassImp(TRootContextMenu)

//______________________________________________________________________________
TRootContextMenu::TRootContextMenu(TContextMenu *c, const char *)
    : TGPopupMenu(gClient->GetDefaultRoot()), TContextMenuImp(c)
{
   // Create context menu.

   fDialog  = 0;
   fTrash = new TList;

   // Context menu handles its own messages
   Associate(this);
}

//______________________________________________________________________________
TRootContextMenu::~TRootContextMenu()
{
   // Delete a context menu.

   delete fDialog;
   if (fTrash) fTrash->Delete();
   delete fTrash;
}

//______________________________________________________________________________
void TRootContextMenu::DisplayPopup(Int_t x, Int_t y)
{
   // Display context popup menu for currently selected object.

   // delete menu items releated to previous object and reset menu size
   if (fEntryList) fEntryList->Delete();
   if (fTrash)   fTrash->Delete();
   fMenuHeight = 6;
   fMenuWidth  = 8;

   // delete previous dialog
   if (fDialog) {
      delete fDialog;
      fDialog = 0;
   }

   // add menu items to popup menu
   CreateMenu(fContextMenu->GetSelectedObject());

   int    xx, yy, topx = 0, topy = 0;
   UInt_t w, h;

   if (fContextMenu->GetSelectedCanvas())
      gVirtualX->GetGeometry(fContextMenu->GetSelectedCanvas()->GetCanvasID(),
                        topx, topy, w, h);

   xx = topx + x + 1;
   yy = topy + y + 1;

   PlaceMenu(xx, yy, kFALSE, kTRUE);
}

//______________________________________________________________________________
void TRootContextMenu::CreateMenu(TObject *object)
{
   // Create the context menu depending on the selected object.

   int entry = 0, toggle = kToggleStart, togglelist = kToggleListStart;
   int userfunction = kUserFunctionStart;

   // Add a title
   AddLabel(fContextMenu->CreatePopupTitle(object));
   AddSeparator();

   // Get list of menu items from the selected object's class
   TList *menuItemList = object->IsA()->GetMenuList();

   TClassMenuItem *menuItem;
   TIter nextItem(menuItemList);

   while ((menuItem = (TClassMenuItem*) nextItem())) {
      switch (menuItem->GetType()) {
         case TClassMenuItem::kPopupSeparator:
            AddSeparator();
            break;
         case TClassMenuItem::kPopupStandardList:
            {
               // Standard list of class methods. Rebuild from scratch.
               // Get linked list of objects menu items (i.e. member functions
               // with the token *MENU in their comment fields.
               TList *methodList = new TList;
               object->IsA()->GetMenuItems(methodList);

               TMethod *method;
               TClass  *classPtr = 0;
               TIter next(methodList);

               while ((method = (TMethod*) next())) {
                  if (classPtr != method->GetClass()) {
                     AddSeparator();
                     classPtr = method->GetClass();
                  }

                  TDataMember *m;
                  EMenuItemKind menuKind = method->IsMenuItem();
                  switch (menuKind) {
                     case kMenuDialog:
                        AddEntry(method->GetName(), entry++, method);
                        break;
                     case kMenuSubMenu:
                        if ((m = method->FindDataMember())) {
                           if (m->GetterMethod()) {
                              TGPopupMenu *r = new TGPopupMenu(gClient->GetDefaultRoot());
                              AddPopup(method->GetName(), r);
                              fTrash->Add(r);
                              TIter nxt(m->GetOptions());
                              TOptionListItem *it;
                              while ((it = (TOptionListItem*) nxt())) {
                                 char  *name  = it->fOptName;
                                 Long_t val   = it->fValue;

                                 TToggle *t = new TToggle;
                                 t->SetToggledObject(object, method);
                                 t->SetOnValue(val);
                                 fTrash->Add(t);

                                 r->AddSeparator();
                                 r->AddEntry(name, togglelist++, t);
                                 if (t->GetState()) r->CheckEntry(togglelist-1);

                              }
                           } else {
                              AddEntry(method->GetName(), entry++, method);
                           }
                        }
                        break;

                     case kMenuToggle:
                        {
                           TToggle *t = new TToggle;
                           t->SetToggledObject(object, method);
                           t->SetOnValue(1);
                           fTrash->Add(t);

                           AddEntry(method->GetName(), toggle++, t);
                           if (t->GetState()) CheckEntry(toggle-1);
                        }
                        break;

                     default:
                        break;
                  }
               }
               delete methodList;
            }
            break;
         case TClassMenuItem::kPopupUserFunction:
            {
               if (menuItem->IsToggle()) {
                  if (object) {
                     TMethod* method =
                           object->IsA()->GetMethodWithPrototype(menuItem->GetFunctionName(),menuItem->GetArgs());
                     TToggle *t = new TToggle;
                     t->SetToggledObject(object, method);
                     t->SetOnValue(1);
                     fTrash->Add(t);

                     AddEntry(method->GetName(), toggle++, t);
                     if (t->GetState()) CheckEntry(toggle-1);
                  } else {
                     Warning("Dialog","Cannot use toggle for a global function");
                  }
               } else {
                  const char* menuItemTitle = menuItem->GetTitle();
                  if (strlen(menuItemTitle)==0) menuItemTitle = menuItem->GetFunctionName();
                  AddEntry(menuItemTitle,userfunction++,menuItem);
               }
            }
            break;
         default:
            break;
      }
   }
}

//______________________________________________________________________________
void TRootContextMenu::Dialog(TObject *object, TMethod *method)
{
   // Create dialog object with OK and Cancel buttons. This dialog
   // prompts for the arguments of "method".

   Dialog(object,(TFunction*)method);
}

//______________________________________________________________________________
void TRootContextMenu::Dialog(TObject *object, TFunction *function)
{
   // Create dialog object with OK and Cancel buttons. This dialog
   // prompts for the arguments of "function".
   // function may be a global function or a method

   Int_t selfobjpos;

   if (!function) return;

   // Position, if it exists, of the argument that correspond to the object itself
   if (fContextMenu->GetSelectedMenuItem())
      selfobjpos =  fContextMenu->GetSelectedMenuItem()->GetSelfObjectPos();
   else selfobjpos = -1;

   const TGWindow *w;
   if (fContextMenu->GetSelectedCanvas()) {
      TCanvas *c = (TCanvas *) fContextMenu->GetSelectedCanvas();
      // Embedded canvas has no canvasimp that is a TGFrame
      if (c->GetCanvasImp()->IsA()->InheritsFrom(TGFrame::Class())) {
         w = fClient->GetWindowById(gVirtualX->GetWindowID(c->GetCanvasID()));
         if (!w) w = (TRootCanvas *) c->GetCanvasImp();
      } else {
         w = gClient->GetDefaultRoot();
      }
   } else if (fContextMenu->GetBrowser()) {
      TBrowser *b = (TBrowser *) fContextMenu->GetBrowser();
      w = (TRootBrowser *) b->GetBrowserImp();
   } else {
      w = gClient->GetDefaultRoot();
   }
   fDialog = new TRootDialog(this, w, fContextMenu->CreateDialogTitle(object, function));

   // iterate through all arguments and create apropriate input-data objects:
   // inputlines, option menus...
   TMethodArg *argument = 0;

   TIter next(function->GetListOfMethodArgs());
   Int_t argpos = 0;

   while ((argument = (TMethodArg *) next())) {
      // Do not input argument for self object
      if (selfobjpos != argpos) {
         Text_t       *argname    = fContextMenu->CreateArgumentTitle(argument);
         const Text_t *type       = argument->GetTypeName();
         TDataType    *datatype   = gROOT->GetType(type);
         const Text_t *charstar   = "char*";
         Text_t        basictype[32];

         if (datatype) {
            strcpy(basictype, datatype->GetTypeName());
         } else {
            TClass *cl = gROOT->GetClass(type);
            if (strncmp(type, "enum", 4) && (cl && !(cl->Property() & kIsEnum)))
               Warning("Dialog", "data type is not basic type, assuming (int)");
            strcpy(basictype, "int");
         }

         if (strchr(argname, '*')) {
            strcat(basictype, "*");
            type = charstar;
         }

         TDataMember *m = argument->GetDataMember();
         if (m && m->GetterMethod(object->IsA())) {

            // Get the current value and form it as a text:

            Text_t val[256];

            if (!strncmp(basictype, "char*", 5)) {
               Text_t *tdefval;
               m->GetterMethod()->Execute(object, "", &tdefval);
               strncpy(val, tdefval, 255);
            } else if (!strncmp(basictype, "float", 5) ||
                       !strncmp(basictype, "double", 6)) {
               Double_t ddefval;
               m->GetterMethod()->Execute(object, "", ddefval);
               sprintf(val, "%g", ddefval);
            } else if (!strncmp(basictype, "char", 4) ||
                       !strncmp(basictype, "bool", 4) ||
                       !strncmp(basictype, "int", 3)  ||
                       !strncmp(basictype, "long", 4) ||
                       !strncmp(basictype, "short", 5)) {
               Long_t ldefval;
               m->GetterMethod()->Execute(object, "", ldefval);
               sprintf(val, "%li", ldefval);
            }

            // Find out whether we have options ...

            TList *opt;
            if ((opt = m->GetOptions())) {
               Warning("Dialog", "option menu not yet implemented", opt);
#if 0
               TMotifOptionMenu *o= new TMotifOptionMenu(argname);
               TIter nextopt(opt);
               TOptionListItem *it = 0;
               while ((it = (TOptionListItem*) nextopt())) {
                  Text_t *name  = it->fOptName;
                  Text_t *label = it->fOptLabel;
                  Long_t value  = it->fValue;
                  if (value != -9999) {
                     Text_t val[256];
                     sprintf(val, "%li", value);
                     o->AddItem(name, val);
                  }else
                     o->AddItem(name, label);
               }
               o->SetData(val);
               fDialog->Add(o);
#endif
            } else {
               // we haven't got options - textfield ...
               fDialog->Add(argname, val, type);
            }
         } else {    // if m not found ...

            char val[256] = "";
            const char *tval = argument->GetDefault();
            if (tval) strncpy(val, tval, 255);
            fDialog->Add(argname, val, type);
         }
      }
      argpos++;
   }

   fDialog->Popup();
}

//______________________________________________________________________________
Bool_t TRootContextMenu::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
   // Handle context menu messages.
   
   TVirtualPad *savedPad = 0;
   if (GetContextMenu()->GetSelectedPad()) {
      savedPad = (TVirtualPad *) gPad;
      gPad = GetContextMenu()->GetSelectedPad();
   }

   switch (GET_MSG(msg)) {

      case kC_COMMAND:

         switch (GET_SUBMSG(msg)) {

            case kCM_MENU:
   
               if (parm1 < kToggleStart) {
                  TMethod *m = (TMethod *) parm2;
                  GetContextMenu()->Action(m);
               } else if (parm1 >= kToggleStart && parm1 < kToggleListStart) {
                  TToggle *t = (TToggle *) parm2;
                  GetContextMenu()->Action(t);
               } else if (parm1 >= kToggleListStart && parm1<kUserFunctionStart) {
                  TToggle *t = (TToggle *) parm2;
                  if (t->GetState() == 0)
                     t->SetState(1);
               } else {
                  TClassMenuItem *mi = (TClassMenuItem*)parm2;
                  GetContextMenu()->Action(mi);
               }
               break;

            case kCM_BUTTON:
               if (parm1 == 1) {
                  const char *args = fDialog->GetParameters();
                  GetContextMenu()->Execute((char *)args);
                  delete fDialog;
                  fDialog = 0;
               }
               if (parm1 == 2) {
                  const char *args = fDialog->GetParameters();
                  GetContextMenu()->Execute((char *)args);
               }
               if (parm1 == 3) {
                  delete fDialog;
                  fDialog = 0;
               }
               break;

            default:
               break;
         }
         break;

      case kC_TEXTENTRY:

         switch (GET_SUBMSG(msg)) {

            case kTE_ENTER:
               {
                  const char *args = fDialog->GetParameters();
                  GetContextMenu()->Execute((char *)args);
                  delete fDialog;
                  fDialog = 0;
               }
               break;

            default:
               break;
         }
         break;

      default:
         break;
   }

   if (savedPad && savedPad->TestBit(kNotDeleted)) gPad = savedPad;

   return kTRUE;
}
