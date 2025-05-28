// @(#)root/thread:$Name:  $:$Id: TMutex.cxx,v 1.4 2004/12/10 22:27:21 rdm Exp $
// Author: Fons Rademakers   26/06/97

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMutex                                                               //
//                                                                      //
// This class implements mutex locks. A mutex is a mutual exclusive     //
// lock. The actual work is done via the TMutexImp class (either        //
// TPosixMutex or TWin32Mutex).                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TMutex.h"
#include "TThreadFactory.h"
#include <errno.h>


ClassImp(TMutex)

//______________________________________________________________________________
TMutex::TMutex(Bool_t recursive)
{
   // Create a mutex lock. The actual mutex implementation will be
   // provided via the TThreadFactory.

   fMutexImp = gThreadFactory->CreateMutexImp();
   fId  = -1;
   fRef = recursive ? 0 : -1;

   if (!fMutexImp)
      Error("TMutex", "could not create TMutexImp");
}

//______________________________________________________________________________
Int_t TMutex::Lock()
{
   // Lock the mutex. Returns 0 when no error, EDEADLK when mutex was already
   // locked by this thread and this mutex is not reentrant.

   Long_t id = TThread::SelfId();
   if (id == fId) {
      // we hold the mutex
      if (fRef < 0) {
         Error("Lock", "mutex is already locked by this thread %ld", fId);
         return EDEADLK;
      } else {
         fRef++;
         return 0;
      }
   }

   // we do not hold the mutex yet
   Int_t iret = fMutexImp->Lock();
   fId = id;
   return iret;
}

//______________________________________________________________________________
Int_t TMutex::TryLock()
{
   // Try to lock mutex. Returns 0 when no error, EDEADLK when mutex was
   // already locked by this thread and this mutex is not reentrant.

   Long_t id = TThread::SelfId();
   if (id == fId) {
      // we hold the mutex
      if (fRef < 0) {
         Error("TryLock", "mutex is already locked by this thread %ld", fId);
         return EDEADLK;
      } else {
         fRef++;
         return 0;
      }
   }

   // we do not hold the mutex yet
   Int_t iret = fMutexImp->TryLock();
   if (iret == 0) fId = id;
   return iret;
}

//______________________________________________________________________________
Int_t TMutex::UnLock()
{
   // Unlock the mutex. Returns 0 when no error, EPERM when mutex was already
   // unlocked by this thread.

   Long_t id = fId;
   Long_t myid = TThread::SelfId();

   if (id != myid) {
      // we do not own the mutex, figure out what happened
      if (id == -1) {
         Error("UnLock", "thread %ld tries to unlock unlocked mutex", myid);
      } else {
         Error("UnLock", "thread %ld tries to unlock mutex locked by thread %ld", myid, id);
      }
      return EPERM;
   }

   // we own the mutex
   if (fRef > 0) {
      fRef--;
      return 0;
   }
   fId = -1;
   return fMutexImp->UnLock();
}

//______________________________________________________________________________
Int_t TMutex::CleanUp()
{
   // Clean up of mutex if it belongs to thread.

   if (TThread::SelfId() != fId) return 0;
   if (fRef > 0) fRef = 0;
   return UnLock();
}
