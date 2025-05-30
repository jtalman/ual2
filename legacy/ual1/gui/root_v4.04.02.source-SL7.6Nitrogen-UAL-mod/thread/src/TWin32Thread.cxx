// @(#)root/thread:$Name:  $:$Id: TWin32Thread.cxx,v 1.4 2004/12/15 10:09:04 rdm Exp $
// Author: Bertrand Bellenot  20/10/2004

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TWin32Thread                                                         //
//                                                                      //
// This class provides an interface to the win32 thread routines.       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TWin32Thread.h"
#include <process.h>

ClassImp(TWin32Thread)

//______________________________________________________________________________
Int_t TWin32Thread::Run(TThread *th)
{
   // Win32 threads -- spawn new thread (like pthread_create).
   // Win32 has a thread handle in addition to the thread ID.

   DWORD  dwThreadId;
   HANDLE hHandle = CreateThread(0, 0,
                                 (LPTHREAD_START_ROUTINE)&TThread::Function,
                                 th, 0, (DWORD*)&dwThreadId);
   if (th->fDetached) {
      ::CloseHandle(hHandle);
      th->fHandle = 0L;
   } else
      th->fHandle = (Long_t)hHandle;

   th->fId = dwThreadId;

   return hHandle ? 0 : EINVAL;
}

//______________________________________________________________________________
Int_t TWin32Thread::Join(TThread *th, void **ret)
{
   // Wait for specified thread execution (if any) to complete
   // (like pthread_join).

   DWORD R = WaitForSingleObject((HANDLE)th->fHandle, INFINITE);

   if ( (R == WAIT_OBJECT_0) || (R == WAIT_ABANDONED) ) {
      //::CloseHandle((HANDLE)th->fHandle);
      return 0;
   }
   if ( R == WAIT_TIMEOUT )
      return EAGAIN;
   return EINVAL;
}

//______________________________________________________________________________
Int_t TWin32Thread::Exit(void *ret)
{
   // Exit the thread.

   ExitThread(0);
   return 0;
}

//______________________________________________________________________________
Int_t TWin32Thread::Kill(TThread *th)
{
   // This is a somewhat dangerous function; it's not
   // suggested to Stop() threads a lot.

   return TerminateThread((HANDLE)th->fHandle,0) ? 0 : EINVAL;
}

//______________________________________________________________________________
Int_t TWin32Thread::CleanUpPush(void **main, void *free,void *arg)
{
   if (!free) fprintf(stderr, "CleanUpPush ***ERROR*** Routine=0\n");
   new TWin32ThreadCleanUp(main,free,arg);
   return 0;
}

//______________________________________________________________________________
Int_t TWin32Thread::CleanUpPop(void **main,Int_t exe)
{
   if (!*main) return 1;
   TWin32ThreadCleanUp *l = (TWin32ThreadCleanUp*)(*main);
   if (!l->fRoutine) fprintf(stderr,"CleanUpPop ***ERROR*** Routine=0\n");
   if (exe && l->fRoutine) ((void (*)(void*))(l->fRoutine))(l->fArgument);
   *main = l->fNext;  delete l;
   return 0;
}

//______________________________________________________________________________
Int_t TWin32Thread::CleanUp(void **main)
{
   fprintf(stderr," CleanUp %lx\n",(ULong_t)*main);
   while(!CleanUpPop(main,1)) { }
   return 0;
}

//______________________________________________________________________________
Long_t TWin32Thread::SelfId()
{
   // Return the current thread's ID.

   return (Long_t)::GetCurrentThreadId();
}

//______________________________________________________________________________
Int_t TWin32Thread::SetCancelOff()
{
   if (gDebug)
      Warning("SetCancelOff", "Not implemented on Win32");
   return 0;
}

//______________________________________________________________________________
Int_t TWin32Thread::SetCancelOn()
{
   if (gDebug)
      Warning("SetCancelOn", "Not implemented on Win32");
   return 0;
}

//______________________________________________________________________________
Int_t TWin32Thread::SetCancelAsynchronous()
{
   if (gDebug)
      Warning("SetCancelAsynchronous", "Not implemented on Win32");
   return 0;
}

//______________________________________________________________________________
Int_t TWin32Thread::SetCancelDeferred()
{
   if (gDebug)
      Warning("SetCancelDeferred", "Not implemented on Win32");
   return 0;
}

//______________________________________________________________________________
Int_t TWin32Thread::CancelPoint()
{
   if (gDebug)
      Warning("CancelPoint", "Not implemented on Win32");
   return 0;
}

//   Clean Up section. PTHREAD implementations of cleanup after cancel are
//   too different and often too bad. Temporary I invent my own bicycle.
//                                                              V.Perev.

//______________________________________________________________________________
TWin32ThreadCleanUp::TWin32ThreadCleanUp(void **main, void *routine, void *arg)
{
   fNext = (TWin32ThreadCleanUp*)*main;
   fRoutine = routine; fArgument = arg;
   *main  = this;
}
