// @(#)root/base:$Name:  $:$Id: TFriendProxyDescriptor.cxx,v 1.2 2004/08/25 15:37:10 rdm Exp $
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
// TFriendProxyDescriptor                                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TFriendProxyDescriptor.h"
#include "TBranchProxyDescriptor.h"

namespace ROOT {

   TFriendProxyDescriptor::TFriendProxyDescriptor(const char *treename,
                                                  const char *aliasname,
                                                  Int_t index) :
      TNamed(treename,aliasname),
      fDuplicate(kFALSE),
      fIndex(index)
   {
      // Constructor
   }

   Bool_t TFriendProxyDescriptor::IsEquivalent(const TFriendProxyDescriptor *other)
   {
      if ( !other ) return kFALSE;
      if ( strcmp(GetName(),other->GetName()) ) return kFALSE;

      TBranchProxyDescriptor *desc;
      TBranchProxyDescriptor *othdesc;

      if ( fListOfTopProxies.GetSize() != other->fListOfTopProxies.GetSize() ) return kFALSE;
      TIter next(&fListOfTopProxies);
      TIter othnext(&other->fListOfTopProxies);
      while ( (desc=(TBranchProxyDescriptor*)next()) ) {
         othdesc=(TBranchProxyDescriptor*)othnext();
         if (!desc->IsEquivalent(othdesc) ) return kFALSE;
      }
      return kTRUE;
   }

   void TFriendProxyDescriptor::OutputClassDecl(FILE *hf, int offset, UInt_t maxVarname)
   {
      fprintf(hf,"%-*sstruct TFriendPx_%s : public TFriendProxy {\n", offset," ", GetName() );
      fprintf(hf,"%-*s   TFriendPx_%s(TBranchProxyDirector *director,TTree *tree,Int_t index) :\n",
              offset," ", GetName() );
      fprintf(hf,"%-*s      %-*s(director,tree,index)",offset," ",maxVarname,"TFriendProxy");
      TBranchProxyDescriptor *data;
      TIter next = &fListOfTopProxies;
      while ( (data = (TBranchProxyDescriptor*)next()) ) {
         fprintf(hf,",\n%-*s      %-*s(&fDirector,\"%s\")",
                 offset," ",maxVarname, data->GetDataName(), data->GetBranchName());
      }
      fprintf(hf,"\n%-*s   { }\n",offset," ");

      fprintf(hf, "\n%-*s   // Proxy for each of the branches and leaves of the tree\n",offset," ");
      next.Reset();
      while ( (data = (TBranchProxyDescriptor*)next()) ) {
         data->OutputDecl(hf, offset+3, maxVarname);
      }
      fprintf(hf,"%-*s};\n",offset," ");
   }

   void TFriendProxyDescriptor::OutputDecl(FILE *hf, int offset, UInt_t maxVarname)
   {
     TString typeName = "TFriendPx_";
     typeName += GetName();
     fprintf(hf,"%-*s%-*s %s;\n",
             offset," ",maxVarname,typeName.Data(),GetTitle());
   }

}
