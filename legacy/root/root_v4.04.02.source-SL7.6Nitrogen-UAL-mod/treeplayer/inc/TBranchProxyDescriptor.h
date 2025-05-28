// @(#)root/treeplayer:$Name:  $:$Id: TBranchProxyDescriptor.h,v 1.5 2004/07/27 12:21:31 brun Exp $
// Author: Philippe Canal 06/06/2004

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers and al.        *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TBranchProxyDescriptor
#define ROOT_TBranchProxyDescriptor

#ifndef ROOT_TNamed
#include "TNamed.h"
#endif


namespace ROOT {

   class TBranchProxyDescriptor : public TNamed {
      TString fDataName;
      TString fBranchName;
      Bool_t fIsSplit;

   public:
      TBranchProxyDescriptor(const char *dataname, const char *type,
                             const char *branchname, Bool_t split = true);
      const char *GetDataName();
      const char *GetTypeName();
      const char *GetBranchName();

      Bool_t IsEquivalent(const TBranchProxyDescriptor *other, Bool_t inClass = kFALSE);
      Bool_t IsSplit() const;

      void OutputDecl(FILE *hf, int offset, UInt_t maxVarname);
      void OutputInit(FILE *hf, int offset, UInt_t maxVarname,
                      const char *prefix);

      ClassDef(TBranchProxyDescriptor,0);
   };
}

#endif
