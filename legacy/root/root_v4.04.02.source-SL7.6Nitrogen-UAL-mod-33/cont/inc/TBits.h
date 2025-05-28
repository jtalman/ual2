// @(#)root/base:$Name:  $:$Id: TBits.h,v 1.9 2004/01/26 11:10:07 brun Exp $
// Author: Philippe Canal 05/02/01

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TBits
#define ROOT_TBits

//------------------------------------------------------------------------------
// Copyright(c) 2001,.Canal (FNAL)
//
// Permission to use, copy, modify and distribute this software and its
// documentation for non-commercial purposes is hereby granted without fee,
// provided that the above copyright notice appears in all copies and
// that both the copyright notice and this permission notice appear in
// the supporting documentation. The authors make no claims about the
// suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.
//------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TBits                                                                //
//                                                                      //
// Container of bits.                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TObject
#include "TObject.h"
#endif

class TBits : public TObject {

protected:

   UInt_t   fNbits;         // Number of bits (around fNbytes*8)
   UInt_t   fNbytes;        // Number of UChars in fAllBits
   UChar_t *fAllBits;       //[fNbytes] array of UChars 
   
   void ReserveBytes(UInt_t nbytes);
   
public:
   TBits(UInt_t nbits = 8);
   TBits(const TBits&);
   TBits& operator=(const TBits&);
   virtual ~TBits();

   //----- bit manipulation
   //----- (note the difference with TObject's bit manipulations)
   void   ResetAllBits(Bool_t value=kFALSE);  // if value=1 set all bits to 1
   void   ResetBitNumber(UInt_t bitnumber);
   void   SetBitNumber(UInt_t bitnumber, Bool_t value = kTRUE);
   Bool_t TestBitNumber(UInt_t bitnumber) const;

   //----- Optimized setters
   // Each of these will replace the contents of the receiver with the bitvector
   // in the parameter array.  The number of bits is changed to nbits.  If nbits
   // is smaller than fNbits, the receiver will NOT be compacted.
   void   Set(UInt_t nbits, const Char_t *array);
   void   Set(UInt_t nbits, const UChar_t *array) { Set(nbits, (const Char_t*)array); }
   void   Set(UInt_t nbits, const Short_t *array);
   void   Set(UInt_t nbits, const UShort_t *array) { Set(nbits, (const Short_t*)array); }
   void   Set(UInt_t nbits, const Int_t *array);
   void   Set(UInt_t nbits, const UInt_t *array) { Set(nbits, (const Int_t*)array); }
   void   Set(UInt_t nbits, const Long64_t *array);
   void   Set(UInt_t nbits, const ULong64_t *array) { Set(nbits, (const Long64_t*)array); }
   
   //----- Optimized getters
   // Each of these will replace the contents of the parameter array with the
   // bits in the receiver.  The parameter array must be large enough to hold
   // all of the bits in the receiver.
   // Note on semantics: any bits in the parameter array that go beyond the
   // number of the bits in the receiver will have an unspecified value.  For
   // example, if you call Get(Int*) with an array of one integer and the TBits
   // object has less than 32 bits, then the remaining bits in the integer will
   // have an unspecified value.
   void   Get(Char_t *array) const;
   void   Get(UChar_t *array) const { Get((Char_t*)array); }
   void   Get(Short_t *array) const;
   void   Get(UShort_t *array) const { Get((Short_t*)array); }
   void   Get(Int_t *array) const;
   void   Get(UInt_t *array) const { Get((Int_t*)array); }
   void   Get(Long64_t *array) const;
   void   Get(ULong64_t *array) const { Get((Long64_t*)array); }
   
   //----- Utilities
   void    Clear(Option_t *option="");
   void    Compact();               // Reduce the space used.
   UInt_t  CountBits(UInt_t startBit=0)     const ;  // return number of bits set to 1
   UInt_t  FirstNullBit(UInt_t startBit=0)  const;
   UInt_t  FirstSetBit(UInt_t startBit=0)   const;
   UInt_t  GetNbits()      const { return fNbits; }
   UInt_t  GetNbytes()     const { return fNbytes; }
   
   Bool_t  operator==(const TBits &other) const;
   
   void   Paint(Option_t *option="");        // to visualize the bits array as an histogram, etc
   void   Print(Option_t *option="") const;  // to show the list of active bits
   
   ClassDef(TBits,1)        // Bit container
};

// inline functions...

inline void TBits::SetBitNumber(UInt_t bitnumber, Bool_t value)
{
   // Set bit number 'bitnumber' to be value

   if (bitnumber >= fNbits) {
      UInt_t new_size = (bitnumber/8) + 1;
      if (new_size > fNbytes) {
         UChar_t *old_location = fAllBits;
         fAllBits = new UChar_t[new_size];
         memcpy(fAllBits,old_location,fNbytes);
         memset(fAllBits+fNbytes ,0, new_size-fNbytes);
         fNbytes = new_size;
         delete [] old_location;
      }
      fNbits = bitnumber+1;
   }
   UInt_t  loc = bitnumber/8;
   UChar_t bit = bitnumber%8;
   if (value)
      fAllBits[loc] |= (1<<bit);
   else
      fAllBits[loc] &= (0xFF ^ (1<<bit));
}

inline Bool_t TBits::TestBitNumber(UInt_t bitnumber) const
{
   // Return the current value of the bit

   if (bitnumber >= fNbits) return kFALSE;
   UInt_t  loc = bitnumber/8;
   UChar_t value = fAllBits[loc];
   UChar_t bit = bitnumber%8;
   Bool_t  result = (value & (1<<bit)) != 0;
   return result;
   // short: return 0 != (fAllBits[bitnumber/8] & (1<< (bitnumber%8)));
}

inline void TBits::ResetBitNumber(UInt_t bitnumber) 
{
   SetBitNumber(bitnumber,kFALSE);
}

#endif


