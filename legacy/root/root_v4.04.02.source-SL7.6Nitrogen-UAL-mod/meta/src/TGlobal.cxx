// @(#)root/meta:$Name:  $:$Id: TGlobal.cxx,v 1.3 2002/11/26 10:24:09 brun Exp $
// Author: Rene Brun   13/11/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Global variables class (global variables are obtained from CINT).    //
// This class describes the attributes of a global variable.            //
// The TROOT class contains a list of all currently defined global      //
// variables (accessible via TROOT::GetListOfGlobals()).                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TGlobal.h"
#include "TString.h"
#include "TInterpreter.h"
#include "Api.h"


ClassImp(TGlobal)

//______________________________________________________________________________
TGlobal::TGlobal(G__DataMemberInfo *info) : TDictionary()
{
   // Default TGlobal ctor. TGlobals are constructed in TROOT via
   // a call to TCint::UpdateListOfGlobals().

   fInfo = info;
   if (fInfo) {
      SetName(fInfo->Name());
      SetTitle(fInfo->Title());
   }
}

//______________________________________________________________________________
TGlobal::~TGlobal()
{
   // TGlobal dtor deletes adopted G__DataMemberInfo object.

   delete fInfo;
}

//______________________________________________________________________________
void *TGlobal::GetAddress() const
{
   // Return address of global.

   return (void *)fInfo->Offset();
}

//______________________________________________________________________________
Int_t TGlobal::GetArrayDim() const
{
   // Return number of array dimensions.

   return fInfo->ArrayDim();
}

//______________________________________________________________________________
Int_t TGlobal::GetMaxIndex(Int_t dim) const
{
   // Return maximum index for array dimension "dim".

   return fInfo->MaxIndex(dim);
}

//______________________________________________________________________________
const char *TGlobal::GetTypeName() const
{
   // Get type of global variable, e,g.: "class TDirectory*" -> "TDirectory".
   // Result needs to be used or copied immediately.

   return gInterpreter->TypeName(fInfo->Type()->Name());
}

//______________________________________________________________________________
const char *TGlobal::GetFullTypeName() const
{
   // Get full type description of global variable, e,g.: "class TDirectory*".

   return fInfo->Type()->Name();
}

//______________________________________________________________________________
Long_t TGlobal::Property() const
{
   // Get property description word. For meaning of bits see EProperty.

   return fInfo->Property();
}
