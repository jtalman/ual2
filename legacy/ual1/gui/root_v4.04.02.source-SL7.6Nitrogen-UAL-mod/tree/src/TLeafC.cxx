// @(#)root/tree:$Name:  $:$Id: TLeafC.cxx,v 1.15 2004/10/18 12:32:12 brun Exp $
// Author: Rene Brun   17/03/97

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// A TLeaf for a variable length string.                                //
//////////////////////////////////////////////////////////////////////////

#include "TLeafC.h"
#include "TBranch.h"

ClassImp(TLeafC)

//______________________________________________________________________________
TLeafC::TLeafC(): TLeaf()
{
//*-*-*-*-*-*Default constructor for LeafC*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*        ============================

   fValue = 0;
   fPointer = 0;
}

//______________________________________________________________________________
TLeafC::TLeafC(const char *name, const char *type)
       :TLeaf(name,type)
{
//*-*-*-*-*-*-*-*-*-*-*-*-*Create a LeafC*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                      ==============
//*-*

   fLenType = 1;
   fMinimum = 0;
   fMaximum = 0;
   fValue   = 0;
   fPointer = 0;
}

//______________________________________________________________________________
TLeafC::~TLeafC()
{
//*-*-*-*-*-*Default destructor for a LeafC*-*-*-*-*-*-*-*-*-*-*-*
//*-*        ===============================

   if (ResetAddress(0,kTRUE)) delete [] fValue;
}


//______________________________________________________________________________
void TLeafC::Export(TClonesArray *list, Int_t n)
{
//*-*-*-*-*-*Export element from local leaf buffer to ClonesArray*-*-*-*-*
//*-*        ====================================================

   Int_t j = 0;
   for (Int_t i=0;i<n;i++) {
      memcpy((char*)list->UncheckedAt(i) + fOffset,&fValue[j], 1);
      j += fLen;
   }
}


//______________________________________________________________________________
void TLeafC::FillBasket(TBuffer &b)
{
//*-*-*-*-*-*-*-*-*-*-*Pack leaf elements in Basket output buffer*-*-*-*-*-*-*
//*-*                  ==========================================

   if (fPointer) fValue = *fPointer;
   Int_t len = strlen(fValue);
   if (len >= fMaximum) fMaximum = len+1;
   if (len >= fLen)     fLen = len+1;
   if (len < 255) {
      b << (UChar_t)len;
   } else {
      b << (UChar_t)255;
      b << len;
   }
   if (len) b.WriteFastArray(fValue,len);
}

//______________________________________________________________________________
const char *TLeafC::GetTypeName() const
{
//*-*-*-*-*-*-*-*Returns name of leaf type*-*-*-*-*-*-*-*-*-*-*-*
//*-*            =========================

   if (fIsUnsigned) return "UChar_t";
   return "Char_t";
}


//______________________________________________________________________________
void TLeafC::Import(TClonesArray *list, Int_t n)
{
//*-*-*-*-*-*Import element from ClonesArray into local leaf buffer*-*-*-*-*
//*-*        ======================================================

   Int_t j = 0;
   for (Int_t i=0;i<n;i++) {
      memcpy(&fValue[j],(char*)list->UncheckedAt(i) + fOffset, 1);
      j += fLen;
   }
}

//______________________________________________________________________________
void TLeafC::PrintValue(Int_t) const
{
// Prints leaf value

   char *value = (char*)GetValuePointer();
   printf("%s",value);
}

//______________________________________________________________________________
void TLeafC::ReadBasket(TBuffer &b)
{
//*-*-*-*-*-*-*-*-*-*-*Read leaf elements from Basket input buffer*-*-*-*-*-*
//*-*                  ===========================================

   Int_t len;
   UChar_t lenchar;
   b >> lenchar;
   if (lenchar < 255) {
      len = lenchar;
   } else {
      b >> len;
   }
   if (len) {
      if (len >= fLen) len = fLen-1;
      b.ReadFastArray(fValue,len);
      fValue[len] = 0;
   } else {
      fValue[0] = 0;
   }
}

//______________________________________________________________________________
void TLeafC::ReadBasketExport(TBuffer &b, TClonesArray *list, Int_t n)
{
//*-*-*-*-*-*-*-*-*-*-*Read leaf elements from Basket input buffer*-*-*-*-*-*
//  and export buffer to TClonesArray objects

   UChar_t len;
   b >> len;
   if (len) {
      if (len >= fLen) len = fLen-1;
      b.ReadFastArray(fValue,len);
      fValue[len] = 0;
   } else {
      fValue[0] = 0;
   }

   Int_t j = 0;
   for (Int_t i=0;i<n;i++) {
      memcpy((char*)list->UncheckedAt(i) + fOffset,&fValue[j], 1);
      j += fLen;
   }
}

//______________________________________________________________________________
void TLeafC::ReadValue(ifstream &s)
{
// read a string from ifstream s and store it into the branch buffer
   char *value = (char*)GetValuePointer();
   s >> value;
}

//______________________________________________________________________________
void TLeafC::SetAddress(void *add)
{
//*-*-*-*-*-*-*-*-*-*-*Set leaf buffer data address*-*-*-*-*-*
//*-*                  ============================

   if (ResetAddress(add)) {
      delete [] fValue;
   }
   if (add) {
      if (TestBit(kIndirectAddress)) {
         fPointer = (char**)add;
         Int_t ncountmax = fLen;
         if (fLeafCount) ncountmax = fLen*(fLeafCount->GetMaximum() + 1);
         if (ncountmax > fNdata || *fPointer == 0) {
            if (*fPointer) delete [] *fPointer;
            if (ncountmax > fNdata) fNdata = ncountmax;
            *fPointer = new char[fNdata];
         }
         fValue = *fPointer;
      } else {
         fValue = (char*)add;
      }
   }
   else {
      fValue = new char[fNdata];
      fValue[0] = 0;
   }
}
