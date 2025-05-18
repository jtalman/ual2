// @(#)root/winnt:$Name:  $:$Id: TWin32Application.cxx,v 1.2 2002/09/12 18:59:53 brun Exp $
// Author: Valery Fine   10/01/96

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//______________________________________________________________________________
//*-*-*-*-*-*-*-*-*The  W I N 3 2 A p p l i c a t i o n class-*-*-*-*-*-*-*
//*-*              ==========================================
//*-*
//*-*  Basic interface to the WIN32 window system
//*-*
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

#include <process.h>

#include "TWin32Application.h"
#include "TGWin32Command.h"
#include "TApplication.h"
#include "TROOT.h"

#include "TError.h"

#include "TWin32HookViaThread.h"

#include <windows.h>
#undef GetWindow

//______________________________________________________________________________
unsigned int _stdcall ROOT_CmdLoop(HANDLE ThrSem)
//*-*-*-*-*-*-*-*-*-*-*-*-* ROOT_CmdLoop*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                       ============
//*-*  Launch a separate thread to handle the ROOT command  messages
//*-*
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 {
   MSG msg;
   int erret;  // GetMessage result

   ReleaseSemaphore(ThrSem, 1, NULL);
   Bool_t EventLoopStop = kFALSE;
   while(!EventLoopStop)
   {
       if (EventLoopStop = (!(erret=GetMessage(&msg,NULL,0,0)) || erret == -1))
       {
           int err = GetLastError();
           if (err) printf( "ROOT_CmdLoop: GetMessage Error %d Last error was %d\n", erret, err);
           continue;
       }

//*-*
//*-*   GetMessage( ... ):
//*-* If the function retrieves a message other than WM_QUIT,
//*-*     the return value is TRUE.
//*-* If the function retrieves the WM_QUIT,
//*-*     the return value is FALSE.
//*-* If there is an error,
//*-*     the return value is -1.
//*-*

       if ((msg.hwnd == NULL) && (msg.message == ROOT_CMD || msg.message == ROOT_SYNCH_CMD)) {

           if (TWin32HookViaThread::ExecuteEvent(&msg, msg.message==ROOT_SYNCH_CMD)) continue;
       }
       TranslateMessage(&msg);
       DispatchMessage(&msg);
   }

   // Delete GUI thread first
   if (gVirtualX != gGXBatch) {
     delete gVirtualX;
   }

   fprintf(stderr," Leaving thread \n");
   if (erret == -1)
   {
       erret = GetLastError();
       Error("CmdLoop", "Error in GetMessage");
       Printf(" %d \n", erret);
   }
//  _endthreadex(0);
   return 0;
} /* ROOT_CmdLoop */


//______________________________________________________________________________
unsigned int ROOT_DlgLoop(HANDLE ThrSem)
//*-*-*-*-*-*-*-*-*-*-*-*-* ROOT_DlgLoop*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                       ============
//*-*  Launch a separate thread to handle the ROOT command  messages
//*-*
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 {
   MSG msg;
   int erret;  // GetMessage result

   ReleaseSemaphore(ThrSem, 1, NULL);
   Bool_t EventLoopStop = kFALSE;
   while(!EventLoopStop)
   {
     if (EventLoopStop = (!(erret=GetMessage(&msg,NULL,0,0)) || erret == -1))
                                                                   continue;
//*-*
//*-*   GetMessage( ... ):
//*-* If the function retrieves a message other than WM_QUIT,
//*-*     the return value is TRUE.
//*-* If the function retrieves the WM_QUIT,
//*-*     the return value is FALSE.
//*-* If there is an error,
//*-*     the return value is -1.
//*-*

       if (msg.hwnd == NULL) ;
       else {
          if (msg.message != ROOT_CMD && msg.message != ROOT_SYNCH_CMD)
                           TranslateMessage(&msg);
          DispatchMessage(&msg);
       }
    }
    if (erret == -1)
    {
        erret = GetLastError();
        Error("CmdLoop", "Error in GetMessage");
        Printf( "%d \n", erret);
    }
    _endthreadex(0);
    return 0;
} /* ROOT_DlgLoop */



// ClassImp(TWin32Application)

//______________________________________________________________________________
TWin32Application::TWin32Application(const char *appClassName, int *argc,
                                     char **argv)
                  : fIDCmdThread(NULL)
{
   fApplicationName = appClassName;
   SetConsoleTitle(appClassName);
   CreateCmdThread();
}
//______________________________________________________________________________
   TWin32Application::~TWin32Application() {

    if (fIDCmdThread) {
        PostThreadMessage(fIDCmdThread,WM_QUIT,0,0);
        if (WaitForSingleObject(fhdCmdThread,10000)==WAIT_FAILED)
                               TerminateThread(fhdCmdThread, -1);
        CloseHandle(fhdCmdThread);
    }
}

//______________________________________________________________________________
Int_t TWin32Application::CreateCmdThread()
{
  HANDLE ThrSem;

  //
  //  Create thread to do the cmd loop
  //

  ThrSem = CreateSemaphore(NULL, 0, 1, NULL);



#ifdef _SC_
  if ((Int_t)(fhdCmdThread = (HANDLE)_beginthreadex(NULL,0, (void *) ROOT_CmdLoop,
                    (LPVOID) ThrSem, 0, (void *)&fIDCmdThread)) == -1){
#else
  if ((Int_t)(fhdCmdThread = (HANDLE)_beginthreadex(NULL,0,  ROOT_CmdLoop,
                    (LPVOID) ThrSem, 0, (unsigned int *)&fIDCmdThread)) == -1){
#endif

      int  erret = GetLastError();
      Error("CreatCmdThread", "Thread was not created");
      Printf(" %d \n", erret);
  }

  WaitForSingleObject(ThrSem, INFINITE);
  CloseHandle(ThrSem);

  return 0;
}

//______________________________________________________________________________
BOOL  TWin32Application::ExecCommand(TGWin32Command *command,Bool_t synch)
{
// To exec a command coming from the other threads

 BOOL postresult;
 ERoot_Msgs cmd = ROOT_CMD;
 if (fIDCmdThread == GetCurrentThreadId())
         Warning("ExecCommand","The dead lock danger");

 if (synch) cmd =  ROOT_SYNCH_CMD;
 while (!(postresult = PostThreadMessage(fIDCmdThread,
                             cmd,
                             (WPARAM)command->GetCOP(),
                             (LPARAM)command))
       ){ ; }
 return postresult;
}

//______________________________________________________________________________
void    TWin32Application::Show(){; }
//______________________________________________________________________________
void    TWin32Application::Hide(){; }
//______________________________________________________________________________
void    TWin32Application::Iconify(){; }
//______________________________________________________________________________
void    TWin32Application::Init(){ ; }
//______________________________________________________________________________
Bool_t  TWin32Application::IsCmdThread()
{
   return (GetCurrentThreadId() == fIDCmdThread) ? kTRUE : kFALSE;
}
//______________________________________________________________________________
   void    TWin32Application::Open(){; }
//______________________________________________________________________________
   void    TWin32Application::Raise(){; }
//______________________________________________________________________________
   void    TWin32Application::Lower(){; }
