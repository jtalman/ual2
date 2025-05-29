// @(#)root/base:$Name:  $:$Id: TFriendProxy.cxx,v 1.2 2005/02/07 18:02:37 rdm Exp $
// Author: Philippe Canal  13/05/2003

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun, Fons Rademakers and al.           *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TFriendProxy                                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TFriendProxy.h"
#include "TTree.h"
#include "TFriendElement.h"

namespace ROOT {

   //------------------------------------------------------------------------------------
   TFriendProxy::TFriendProxy() : fDirector(0,-1), fIndex(-1)
   {
   }

   //------------------------------------------------------------------------------------
   TFriendProxy::TFriendProxy(TBranchProxyDirector *director, TTree *main, Int_t index) :
      fDirector(0,-1), fIndex(index)
   {
      // Constructor.

      if (main) {
         TObject *obj = main->GetListOfFriends()->At(fIndex);
         TFriendElement *element = dynamic_cast<TFriendElement*>( obj );
         fDirector.SetTree(element->GetTree());
      }
      director->Attach(this);
   }

   //------------------------------------------------------------------------------------
   Long64_t TFriendProxy::GetReadEntry() const
   {
      return fDirector.GetReadEntry();
   }

   //------------------------------------------------------------------------------------
   void TFriendProxy::ResetReadEntry()
   {
      // Refresh the cached read entry number from the original tree.

      if (fDirector.GetTree()) fDirector.SetReadEntry(fDirector.GetTree()->GetReadEntry());
   }

  //------------------------------------------------------------------------------------
   void TFriendProxy::Update(TTree *newmain)
   {
      // Update the address of the underlying tree.

      if (newmain) {
         TObject *obj = newmain->GetListOfFriends()->At(fIndex);
         TFriendElement *element = dynamic_cast<TFriendElement*>( obj );
         fDirector.SetTree(element->GetTree());
      } else {
         fDirector.SetTree(0);
      }
   }
}
