// @(#)root/thread:$Name:  $:$Id: TSemaphore.h,v 1.2 2004/12/10 12:13:33 rdm Exp $
// Author: Fons Rademakers   02/07/97

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TSemaphore
#define ROOT_TSemaphore


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSemaphore                                                           //
//                                                                      //
// This class implements a counting semaphore. Use a semaphore          //
// to synchronize threads.                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TObject
#include "TObject.h"
#endif
#ifndef ROOT_TMutex
#include "TMutex.h"
#endif
#ifndef ROOT_TCondition
#include "TCondition.h"
#endif


class TSemaphore : public TObject {

private:
   TMutex       fMutex;   // semaphare mutex
   TCondition   fCond;    // semaphore condition variable
   Int_t        fValue;   // counter

public:
   TSemaphore(UInt_t initial = 1);
   virtual ~TSemaphore() { }

   Int_t  Wait();
   Int_t  TryWait();
   Int_t  Post();

   ClassDef(TSemaphore,0)  // Counting semaphore class
};

#endif
