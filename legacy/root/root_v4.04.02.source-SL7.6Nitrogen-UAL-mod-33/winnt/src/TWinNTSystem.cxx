// @(#)root/winnt:$Name:  $:$Id: TWinNTSystem.cxx,v 1.119 2005/05/03 13:17:55 rdm Exp $
// Author: Fons Rademakers   15/09/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
// TWinNTSystem                                                                 //
//                                                                              //
// Class providing an interface to the Windows NT/Windows 95 Operating Systems. //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////


#ifdef HAVE_CONFIG
#include "config.h"
#endif

#include "Windows4Root.h"
#include "TWinNTSystem.h"
#include "TROOT.h"
#include "TError.h"
#include "TOrdCollection.h"
#include "TRegexp.h"
#include "TException.h"
#include "TEnv.h"
#include "TSocket.h"
#include "TApplication.h"
#include "TWin32SplashThread.h"
#include "Win32Constants.h"
#include "TWin32HookViaThread.h"
#include "TWin32Timer.h"
#include "TGWin32Command.h"
#include "TInterpreter.h"
#include "TObjString.h"

#include <sys/utime.h>
#include <process.h>
#include <io.h>
#include <direct.h>
#include <ctype.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <lm.h>

extern "C" {
   extern int G__get_security_error();
   extern int G__genericerror(const char* msg);
}

const char *kProtocolName   = "tcp";
typedef void (*SigHandler_t)(ESignals);

static HANDLE gConsoleEvent;
static HANDLE gConsoleThreadHandle;
typedef NET_API_STATUS (WINAPI *pfn1)(LPVOID);
typedef NET_API_STATUS (WINAPI *pfn2)(LPCWSTR, LPCWSTR, DWORD, LPBYTE*);
typedef NET_API_STATUS (WINAPI *pfn3)(LPCWSTR, LPCWSTR, DWORD, LPBYTE*,
                                     DWORD, LPDWORD, LPDWORD, PDWORD);
typedef NET_API_STATUS (WINAPI *pfn4)(LPCWSTR, DWORD, LPBYTE*, DWORD, LPDWORD,
                                     LPDWORD, PDWORD);
static pfn1 p2NetApiBufferFree;
static pfn2 p2NetUserGetInfo;
static pfn3 p2NetLocalGroupGetMembers;
static pfn4 p2NetLocalGroupEnum;


static struct signal_map {
   int code;
   SigHandler_t handler;
   char *signame;
} signal_map[kMAXSIGNALS] = {   // the order of the signals should be identical
//   SIGBUS,   0, "bus error",    // to the one in SysEvtHandler.h
   SIGSEGV,  0, "segmentation violation",
//   SIGSYS,   0, "bad argument to system call",
//   SIGPIPE,  0, "write on a pipe with no one to read it",
   SIGILL,   0, "illegal instruction",
//   SIGQUIT,  0, "quit",
   SIGINT,   0, "interrupt",
//   SIGWINCH, 0, "window size change",
//   SIGALRM,  0, "alarm clock",
//   SIGCHLD,  0, "death of a child",
//   SIGURG,   0, "urgent data arrived on an I/O channel",
   SIGFPE,   0, "floating point exception"
//   SIGTERM,  0, "termination signal",
//   SIGUSR1,  0, "user-defined signal 1",
//   SIGUSR2,  0, "user-defined signal 2"
};

//////////////////// Windows TFdSet ////////////////////////////////////////////////
class TFdSet {
private:
   fd_set *fds_bits; // file descriptors (according MSDN maximum is 64)
public:
   TFdSet() { fds_bits = new fd_set; fds_bits->fd_count = 0; }
   virtual ~TFdSet() { delete fds_bits; }
   void  Copy(TFdSet &fd) const { memcpy((void*)fd.fds_bits, fds_bits, sizeof(fd_set)); }
   TFdSet(const TFdSet& fd) { fd.Copy(*this); }
   TFdSet& operator=(const TFdSet& fd)  { fd.Copy(*this); return *this; }
   void  Zero() { fds_bits->fd_count = 0; }
   void  Set(Int_t fd) { fds_bits->fd_array[fds_bits->fd_count++] = (SOCKET)fd; }
   void  Clr(Int_t fd)
   {
      int i;
      for (i=0; i<fds_bits->fd_count; i++) {
         if (fds_bits->fd_array[i]==(SOCKET)fd) {
            while (i<fds_bits->fd_count-1) {
               fds_bits->fd_array[i] = fds_bits->fd_array[i+1];
               i++;
            }
            fds_bits->fd_count--;
            break;
         }
      }
   }
   Int_t IsSet(Int_t fd) { return __WSAFDIsSet((SOCKET)fd, fds_bits); }
   Int_t *GetBits() { return fds_bits && fds_bits->fd_count ? (Int_t*)fds_bits : 0; }
   UInt_t GetCount() { return (UInt_t)fds_bits->fd_count; }
   Int_t GetFd(Int_t i) { return i<fds_bits->fd_count ? fds_bits->fd_array[i] : 0; }
};


////// static functions providing interface to raw WinNT ////////////////////
struct  itimerval {
   struct  timeval it_interval;
   struct  timeval it_value;
};

static UINT   timer_active = 0;
static struct itimerval itv;
static DWORD  start_time;

#define ITIMER_REAL     0
#define ITIMER_VIRTUAL  1
#define ITIMER_PROF     2

//______________________________________________________________________________
static int setitimer(int which, const struct itimerval *value, struct itimerval *oldvalue)
{
   //

   UINT elapse;

   if (which != ITIMER_REAL) {
      return -1;
   }
   // Check if we will wrap
   if (itv.it_value.tv_sec >= (long) (UINT_MAX/1000)) {
      return -1;
   }
   if (timer_active) {
      ::KillTimer(NULL, timer_active);
      timer_active = 0;
   }
   if (oldvalue) {
      *oldvalue = itv;
   }
   if (value == NULL) {
      return -1;
   }
   itv = *value;
   elapse = itv.it_value.tv_sec * 1000 + itv.it_value.tv_usec / 1000;
   if (elapse == 0) {
      if (itv.it_value.tv_usec) {
         elapse = 1;
      } else {
         return 0;
      }
   }
   if (!(timer_active = ::SetTimer(NULL, 1, elapse, NULL))) {
      return -1;
   }
   start_time = ::GetTickCount();
   return 0;
}

#ifndef GDK_WIN32

//______________________________________________________________________________
static int WinNTSetitimer(TTimer *ti)
{
   // Set interval timer to time-out in ms milliseconds.

   return 0;
}

#else // GDK_WIN32

//______________________________________________________________________________
static int WinNTSetitimer(Long_t ms)
{
   // Set interval timer to time-out in ms milliseconds.

   struct itimerval itval;
   itval.it_interval.tv_sec = itval.it_interval.tv_usec = 0;
   itval.it_value.tv_sec = itval.it_value.tv_usec = 0;
   if (ms >= 0) {
      itval.it_value.tv_sec  = ms / 1000;
      itval.it_value.tv_usec = (ms % 1000) * 1000;
   }
   return ::setitimer(ITIMER_REAL, &itval, 0);
}

#endif // GDK_WIN32

//---- RPC -------------------------------------------------------------------
//*-* Error codes set by the Windows Sockets implementation are not made available
//*-* via the errno variable. Additionally, for the getXbyY class of functions,
//*-* error codes are NOT made available via the h_errno variable. Instead, error
//*-* codes are accessed by using the WSAGetLastError . This function is provided
//*-* in Windows Sockets as a precursor (and eventually an alias) for the Win32
//*-* function GetLastError. This is intended to provide a reliable way for a thread
//*-* in a multithreaded process to obtain per-thread error information.

//______________________________________________________________________________
static int WinNTRecv(int socket, void *buffer, int length, int flag)
{
   // Receive exactly length bytes into buffer. Returns number of bytes
   // received. Returns -1 in case of error, -2 in case of MSG_OOB
   // and errno == EWOULDBLOCK, -3 in case of MSG_OOB and errno == EINVAL
   // and -4 in case of kNonBlock and errno == EWOULDBLOCK.
   // Returns -5 if pipe broken or reset by peer (EPIPE || ECONNRESET).

   if (socket == -1) return -1;
   SOCKET sock = socket;

   int once = 0;
   if (flag == -1) {
      flag = 0;
      once = 1;
   }

   int nrecv, n;
   char *buf = (char *)buffer;

   for (n = 0; n < length; n += nrecv) {
      if ((nrecv = ::recv(sock, buf+n, length-n, flag)) <= 0) {
         if (nrecv == 0) {
            break;        // EOF
         }
         if (flag == MSG_OOB) {
            if (::WSAGetLastError() == WSAEWOULDBLOCK) {
               return -2;
            } else if (::WSAGetLastError() == WSAEINVAL) {
               return -3;
            }
         }
         if (::WSAGetLastError() == WSAEWOULDBLOCK) {
            return -4;
         } else {
            if (::WSAGetLastError() != WSAEINTR)
               ::SysError("TWinNTSystem::WinNTRecv", "recv");
            if (::WSAGetLastError() == EPIPE ||
                ::WSAGetLastError() == WSAECONNRESET)
               return -5;
            else
               return -1;
         }
      }
      if (once) {
         return nrecv;
      }
   }
   return n;
}

//______________________________________________________________________________
static int WinNTSend(int socket, const void *buffer, int length, int flag)
{
   // Send exactly length bytes from buffer. Returns -1 in case of error,
   // otherwise number of sent bytes. Returns -4 in case of kNoBlock and
   // errno == EWOULDBLOCK. Returns -5 if pipe broken or reset by peer
   // (EPIPE || ECONNRESET).

   if (socket < 0) return -1;
   SOCKET sock = socket;

   int once = 0;
   if (flag == -1) {
      flag = 0;
      once = 1;
   }

   int nsent, n;
   const char *buf = (const char *)buffer;

   for (n = 0; n < length; n += nsent) {
      if ((nsent = ::send(sock, buf+n, length-n, flag)) <= 0) {
         if (nsent == 0) {
            break;
         }
         if (::WSAGetLastError() == WSAEWOULDBLOCK) {
            return -4;
         } else {
            if (::WSAGetLastError() != WSAEINTR)
               ::SysError("TWinNTSystem::WinNTSend", "send");
            if (::WSAGetLastError() == EPIPE ||
                ::WSAGetLastError() == WSAECONNRESET)
               return -5;
            else
               return -1;
         }
      }
      if (once) {
         return nsent;
      }
   }
   return n;
}

//______________________________________________________________________________
static int WinNTSelect(TFdSet *readready, TFdSet *writeready, Long_t timeout)
{
   // Wait for events on the file descriptors specified in the readready and
   // writeready masks or for timeout (in milliseconds) to occur.

   int retcode;
   fd_set* rbits = readready ? (fd_set*)readready->GetBits() : 0;
   fd_set* wbits = writeready ? (fd_set*)writeready->GetBits() : 0;

   if (timeout >= 0) {
      timeval tv;
      tv.tv_sec  = timeout / 1000;
      tv.tv_usec = (timeout % 1000) * 1000;

      retcode = ::select(0, rbits, wbits, 0, &tv);
   } else {
      retcode = ::select(0, rbits, wbits, 0, 0);
   }

   if (retcode == SOCKET_ERROR) {
      int errcode = ::WSAGetLastError();

      if ( errcode == WSAEINTR) {
         TSystem::ResetErrno();  // errno is not self reseting
         return -2;
      }
      if (errcode == EBADF) {
         return -3;
      }
      return -1;
   }
   return retcode;
}

//______________________________________________________________________________
static const char *DynamicPath(const char *newpath = 0, Bool_t reset = kFALSE)
{
   // Get shared library search path.

   static const char *dynpath = 0;

   if ((reset || newpath) && dynpath) {
      delete [] (char*)dynpath;
      dynpath = 0;
   }
   if (newpath) {

      dynpath = StrDup(newpath);

   } else if (dynpath == 0) {
      dynpath = gEnv->GetValue("Root.DynamicPath", (char*)0);
      if (dynpath == 0) {
         dynpath = StrDup(Form("%s;%s/bin;%s,", gProgPath, gRootDir, gSystem->Getenv("PATH")));
      }
   }
   return dynpath;
}

//______________________________________________________________________________
static void sighandler(int sig)
{
   // Call the signal handler associated with the signal.

   for (int i = 0; i < kMAXSIGNALS; i++) {
      if (signal_map[i].code == sig) {
         (*signal_map[i].handler)((ESignals)i);
         return;
      }
   }
}

//______________________________________________________________________________
static void WinNTSignal(ESignals sig, SigHandler_t handler)
{
   // Set a signal handler for a signal.
}

//______________________________________________________________________________
static char *WinNTSigname(ESignals sig)
{
   // Return the signal name associated with a signal.

   return signal_map[sig].signame;
}

//______________________________________________________________________________
static BOOL ConsoleSigHandler(DWORD sig)
{
   // WinNT signal handler.

   switch (sig) {
   case CTRL_C_EVENT:
   if (!G__get_security_error()) {
      G__genericerror("\n *** Break *** keyboard interrupt");
   } else {
      Break("TInterruptHandler::Notify", "keyboard interrupt");
      if (TROOT::Initialized()) {
         gInterpreter->RewindDictionary();
      }
   }
   return kTRUE;
   case CTRL_BREAK_EVENT:
   case CTRL_LOGOFF_EVENT:
   case CTRL_SHUTDOWN_EVENT:
   case CTRL_CLOSE_EVENT:
   default:
      printf("\n *** Break *** keyboard interrupt - ROOT is terminated\n");
      gSystem->Exit(-1);
      return kTRUE;
   }
}

//______________________________________________________________________________
static void SigHandler(ESignals sig)
{
   if (gSystem) {
      if (TROOT::Initialized()) {
         ::Throw(sig);
      }
      gSystem->Abort(-1);
   }
}


///////////////////////////////////////////////////////////////////////////////
class TTermInputLine :  public  TWin32HookViaThread {

protected:
   void ExecThreadCB(TWin32SendClass *sentclass);
public:
   TTermInputLine::TTermInputLine();
};

//______________________________________________________________________________
TTermInputLine::TTermInputLine()
{
   //

   TWin32SendWaitClass CodeOp(this);
   ExecCommandThread(&CodeOp, kFALSE);
   CodeOp.Wait();
}

//______________________________________________________________________________
void TTermInputLine::ExecThreadCB(TWin32SendClass *code)
{
   // Dispatch a single event.

   gROOT->GetApplication()->HandleTermInput();
   ((TWin32SendWaitClass *)code)->Release();
}


///////////////////////////////////////////////////////////////////////////////
ClassImp(TWinNTSystem)

#ifdef GDK_WIN32
//______________________________________________________________________________
unsigned __stdcall HandleConsoleThread(void *pArg )
{
   //

   while (1) {
      if(gROOT->GetApplication()) {
         if (gConsoleEvent) {
            ::WaitForSingleObject(gConsoleEvent, INFINITE);
         }

         if(!gApplication->HandleTermInput()) break; // no terminal input

         if (gSplash) {    // terminate splash window after first key press
            delete gSplash;
            gSplash = 0;
         }
         ::SetConsoleMode(::GetStdHandle(STD_OUTPUT_HANDLE),
                          ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
         if (gConsoleEvent) {
            ::ResetEvent(gConsoleEvent);
            ::CloseHandle(gConsoleEvent);
            gConsoleEvent = 0;
         }
      } else {
         static int i = 0;
         ::SleepEx(100, 1);
         i++;
         if (i > 20) break; // TApplication object doesn't exist
      }
   }

   ::CloseHandle(gConsoleThreadHandle);
   gConsoleThreadHandle = 0;
   _endthreadex( 0 );
   return 0;
}
#endif

//______________________________________________________________________________
Bool_t TWinNTSystem::HandleConsoleEvent()
{
   //

   TSignalHandler *sh;
   TIter next(fSignalHandler);
   ESignals s;

   while (sh = (TSignalHandler*)next()) {
      s = sh->GetSignal();
      if (s == kSigInterrupt) {
         sh->Notify();
         Throw(SIGINT);
         return kTRUE;
      }
   }
   return kFALSE;
}

//______________________________________________________________________________
TWinNTSystem::TWinNTSystem() : TSystem("WinNT", "WinNT System")
{
   // ctor

   fhProcess = ::GetCurrentProcess();
   fDirNameBuffer = 0;
   fShellName = 0;
   fWin32Timer = 0;
   fhSmallIconList = 0;
   fhNormalIconList = 0;

   WSADATA WSAData;
   int initwinsock = 0;

   if (initwinsock = ::WSAStartup(MAKEWORD(2, 0), &WSAData)) {
      Error("TWinNTSystem()","Starting sockets failed");
   }
}

//______________________________________________________________________________
TWinNTSystem::~TWinNTSystem()
{
   // dtor

   SafeDelete(fWin32Timer);

   // Clean up the WinSocket connectios
   ::WSACleanup();

   if (fDirNameBuffer) {
      delete [] fDirNameBuffer;
      fDirNameBuffer = 0;
   }

   if (fhSmallIconList) {
      ImageList_Destroy(fhSmallIconList);
      fhSmallIconList = 0;
   }

   if (fhNormalIconList) {
      ImageList_Destroy(fhNormalIconList);
      fhNormalIconList = 0;
   }

#ifdef GDK_WIN32
   if (gConsoleThreadHandle) ::CloseHandle(gConsoleThreadHandle);
#else
   ::CloseHandle(fhTermInputEvent);
#endif
}

//______________________________________________________________________________
Bool_t TWinNTSystem::Init()
{
   // Initialize WinNT system interface.

   const char *dir = 0;

   if (TSystem::Init()) {
      return kTRUE;
   }

   fReadmask = new TFdSet;
   fWritemask = new TFdSet;
   fReadready = new TFdSet;
   fWriteready = new TFdSet;
   fSignals = new TFdSet;
   fNfd    = 0;

   //--- install default handlers
   WinNTSignal(kSigChild,                 SigHandler);
   WinNTSignal(kSigBus,                   SigHandler);
   WinNTSignal(kSigSegmentationViolation, SigHandler);
   WinNTSignal(kSigIllegalInstruction,    SigHandler);
   WinNTSignal(kSigSystem,                SigHandler);
   WinNTSignal(kSigPipe,                  SigHandler);
   WinNTSignal(kSigAlarm,                 SigHandler);
   WinNTSignal(kSigFloatingException,     SigHandler);

   fSigcnt = 0;

#ifndef ROOTPREFIX
   gRootDir = Getenv("ROOTSYS");
   if (gRootDir == 0) {
      static char lpFilename[MAX_PATH];
      if (::GetModuleFileName(NULL,               // handle to module to find filename for
                            lpFilename,           // pointer to buffer to receive module path
                            sizeof(lpFilename)))  // size of buffer, in characters
      {
         const char *dirName = DirName(DirName(lpFilename));
         gRootDir = StrDup(dirName);
      } else {
         gRootDir = 0;
      }
   }
#else
   gRootDir= ROOTPREFIX;
#endif

#ifdef GDK_WIN32
   if (!gROOT->IsBatch()) {
      gConsoleEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
      gConsoleThreadHandle = (HANDLE)_beginthreadex(NULL, 0, &HandleConsoleThread,
                                                    0, 0, 0);
   }
#else
   // The the name of the DLL to be used as a stock of the icon
   SetShellName();
   CreateIcons();

   // Create Event HANDLE for stand-alone ROOT-based applications
   fhTermInputEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
#endif

   fGroupsInitDone = kFALSE;

   return kFALSE;
}

//---- Misc --------------------------------------------------------------------

//______________________________________________________________________________
const char *TWinNTSystem::BaseName(const char *name)
{
   // Base name of a file name. Base name of /user/root is root.
   // But the base name of '/' is '/'
   //                      'c:\' is 'c:\'
   // The calling routine should use free() to free memory BaseName allocated
   // for the base name

   if (name) {
      int idx = 0;
      const char *symbol=name;

      // Skip leading blanks
      while ( (*symbol == ' ' || *symbol == '\t') && *symbol) symbol++;

      if (*symbol) {
         if (isalpha(symbol[idx]) && symbol[idx+1] == ':') idx = 2;
         if ( (symbol[idx] == '/'  ||  symbol[idx] == '\\')  &&  symbol[idx+1] == '\0') {
            return StrDup(symbol);
         }
      } else {
         Error("BaseName", "name = 0");
         return 0;
      }
      char *cp;
      char *bslash = (char *)strrchr(&symbol[idx],'\\');
      char *rslash = (char *)strrchr(&symbol[idx],'/');
      if (cp = max(rslash, bslash)) {
         return StrDup(++cp);
      }
      return StrDup(&symbol[idx]);
   }
   Error("BaseName", "name = 0");
   return 0;
}

//______________________________________________________________________________
void TWinNTSystem::CreateIcons()
{
   //

   const char *shellname =  fShellName;

   HINSTANCE hShellInstance = ::LoadLibrary(shellname);
   fhSmallIconList  = 0;
   fhNormalIconList = 0;

   if (hShellInstance) {
      fhSmallIconList = ImageList_Create(::GetSystemMetrics(SM_CXSMICON),
                                         ::GetSystemMetrics(SM_CYSMICON),
                                         ILC_MASK, kTotalNumOfICons, 1);

      fhNormalIconList = ImageList_Create(::GetSystemMetrics(SM_CXICON),
                                          ::GetSystemMetrics(SM_CYICON),
                                          ILC_MASK, kTotalNumOfICons, 1);
      HICON hicon;
      HICON hDummyIcon = ::LoadIcon(NULL, IDI_APPLICATION);

      // Add "ROOT" main icon
      hicon = ::LoadIcon(::GetModuleHandle(NULL), MAKEINTRESOURCE(101));
      if (!hicon) {
         hicon = ::LoadIcon(hShellInstance, MAKEINTRESOURCE(101));
      }
      if (!hicon) hicon = hDummyIcon;
      ImageList_AddIcon(fhSmallIconList, hicon);
      ImageList_AddIcon(fhNormalIconList, hicon);
      if (hicon != hDummyIcon) ::DeleteObject(hicon);

      // Add "Canvas" icon
      hicon = ::LoadIcon(hShellInstance, MAKEINTRESOURCE(16));
      if (!hicon) hicon = hDummyIcon;
      ImageList_AddIcon(fhSmallIconList, hicon);
      ImageList_AddIcon(fhNormalIconList, hicon);
      if (hicon != hDummyIcon) ::DeleteObject(hicon);

      // Add "Browser" icon
      hicon = ::LoadIcon(hShellInstance,MAKEINTRESOURCE(171));
      if (!hicon) hicon = hDummyIcon;
      ImageList_AddIcon(fhSmallIconList, hicon);
      ImageList_AddIcon(fhNormalIconList, hicon);
      if (hicon != hDummyIcon) ::DeleteObject(hicon);

      // Add "Closed Folder" icon
      hicon = ::LoadIcon(hShellInstance, MAKEINTRESOURCE(4));
      if (!hicon) hicon = hDummyIcon;
      ImageList_AddIcon(fhSmallIconList, hicon);
      ImageList_AddIcon(fhNormalIconList, hicon);
      if (hicon != hDummyIcon) ::DeleteObject(hicon);

      //  Add the "Open Folder" icon
      hicon = LoadIcon(hShellInstance, MAKEINTRESOURCE(5));
      if (!hicon) hicon = hDummyIcon;
      ImageList_AddIcon(fhSmallIconList, hicon);
      ImageList_AddIcon(fhNormalIconList, hicon);
      if (hicon != hDummyIcon) ::DeleteObject(hicon);

      // Add the "Document" icon
      hicon = ::LoadIcon(hShellInstance, MAKEINTRESOURCE(152));
      if (!hicon) hicon = hDummyIcon;
      ImageList_AddIcon(fhSmallIconList, hicon);
      ImageList_AddIcon(fhNormalIconList, hicon);
      if (hicon != hDummyIcon) ::DeleteObject(hicon);

      ::FreeLibrary((HMODULE)hShellInstance);
   }
}

//______________________________________________________________________________
void  TWinNTSystem::SetShellName(const char *name)
{
   //

   const char *shellname = "SHELL32.DLL";

   if (name) {
      fShellName = new char[lstrlen(name)+1];
      strcpy((char *)fShellName, name);
   } else {
//*-* use the system "shell32.dll" file as the icons stock.
//*-*  Check the type of the OS
      OSVERSIONINFO OsVersionInfo;

//*-*         Value                      Platform
//*-*  ----------------------------------------------------
//*-*  VER_PLATFORM_WIN32s              Win32s on Windows 3.1
//*-*  VER_PLATFORM_WIN32_WINDOWS       Win32 on Windows 95
//*-*  VER_PLATFORM_WIN32_NT            Windows NT
//*-*
      OsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
      GetVersionEx(&OsVersionInfo);
      if (OsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) {
        fShellName = strcpy(new char[lstrlen(shellname)+1], shellname);
      } else {
         //  for Windows 95 we have to create a local copy this file
         const char *rootdir = gRootDir;
         const char newshellname[] = "bin/RootShell32.dll";
         fShellName = ConcatFileName(gRootDir, newshellname);

         char sysdir[1024];
         ::GetSystemDirectory(sysdir, 1024);
         char *sysfile = (char *) ConcatFileName(sysdir, shellname);
         CopyFile(sysfile, fShellName, TRUE);  // TRUE means "don't overwrite if fShellName is exists
         delete [] sysfile;
      }
   }
}

//______________________________________________________________________________
void TWinNTSystem::SetProgname(const char *name)
{
   // Set the application name (from command line, argv[0]) and copy it in
   // gProgName. Copy the application pathname in gProgPath.

   ULong_t  idot = 0;
   char *dot = 0;
   char *progname;
   char *fullname = 0; // the program name with extension

  // On command prompt the progname can be supplied with no extension (under Windows)
   ULong_t namelen=name ? strlen(name) : 0;
   if (name && namelen > 0) {
      // Check whether the name contains "extention"
      fullname = new char[namelen+5];
      strcpy(fullname, name);
      if ( !strrchr(fullname, '.') )
         strcat(fullname, ".exe");

      progname = StrDup(BaseName(fullname));
      dot = strrchr(progname, '.');
      idot = (ULong_t)(dot - progname);

      char *which = 0;

      if (IsAbsoluteFileName(fullname) && !AccessPathName(fullname)) {
         which = StrDup(fullname);
      } else {
         which = Which(Form("%s;%s", WorkingDirectory(), Getenv("PATH")), progname);
      }

      if (which) {
         const char *dirname;
         char driveletter = DriveName(which);
         const char *d = DirName(which);

         if (driveletter) {
            dirname = Form("%c:%s", driveletter, d);
         } else {
            dirname = Form("%s", d);
         }

         gProgPath = StrDup(dirname);
      } else {
         // Do not issue a warning - ROOT is not using gProgPath anyway.
         // Warning("SetProgname",
         //   "Cannot find this program named \"%s\" (Did you create a TApplication? Is this program in your %%PATH%%?)",
         //   fullname);
         gProgPath = WorkingDirectory();
      }

      // Cut the extension for progname off
      progname[idot] = '\0';
      gProgName = StrDup(progname);
      if (which) delete [] which;
      delete[] fullname;
   }
}

//______________________________________________________________________________
const char *TWinNTSystem::GetError()
{
   // Return system error string.

   Int_t err = GetErrno();
   if (err == 0 && fLastErrorString != "")
      return fLastErrorString;
   if (err < 0 || err >= sys_nerr) {
      return Form("errno out of range %d", err);
   }
   return sys_errlist[err];
}

//______________________________________________________________________________
const char *TWinNTSystem::HostName()
{
   // Return the system's host name.

   if (fHostname == "") {
      char hn[64];
      DWORD il = sizeof(hn);
      ::GetComputerName(hn, &il);
      fHostname = hn;
   }
   return fHostname;
}

//---- EventLoop ---------------------------------------------------------------

//______________________________________________________________________________
void TWinNTSystem::AddFileHandler(TFileHandler *h)
{
   // Add a file handler to the list of system file handlers. Only adds
   // the handler if it is not already in the list of file handlers.

   TSystem::AddFileHandler(h);
   if (h) {
      int fd = h->GetFd();
      if (!fd) return;

      if (h->HasReadInterest()) {
         fReadmask->Set(fd);
      }
      if (h->HasWriteInterest()) {
         fWritemask->Set(fd);
      }
   }
}

//______________________________________________________________________________
TFileHandler *TWinNTSystem::RemoveFileHandler(TFileHandler *h)
{
   // Remove a file handler from the list of file handlers. Returns
   // the handler or 0 if the handler was not in the list of file handlers.

   TFileHandler *oh = TSystem::RemoveFileHandler(h);
   if (oh) {       // found
      TFileHandler *th;
      TIter next(fFileHandler);
      fReadmask->Zero();
      fWritemask->Zero();

      while ((th = (TFileHandler *) next())) {
         int fd = th->GetFd();
         if (!fd) return oh;

         if (th->HasReadInterest()) {
            fReadmask->Set(fd);
         }
         if (th->HasWriteInterest()) {
            fWritemask->Set(fd);
         }
      }
   }
   return oh;
}

//______________________________________________________________________________
void TWinNTSystem::AddSignalHandler(TSignalHandler *h)
{
   // Add a signal handler to list of system signal handlers. Only adds
   // the handler if it is not already in the list of signal handlers.

   TSystem::AddSignalHandler(h);
   ESignals  sig = h->GetSignal();

   // Add a new handler to the list of the console handlers
   if (sig == kSigInterrupt) {
      ::SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleSigHandler, TRUE);
   }
   WinNTSignal(h->GetSignal(), SigHandler);
}

//______________________________________________________________________________
TSignalHandler *TWinNTSystem::RemoveSignalHandler(TSignalHandler *h)
{
   // Remove a signal handler from list of signal handlers. Returns
   // the handler or 0 if the handler was not in the list of signal handlers.

   int sig = h->GetSignal();

   if (sig = kSigInterrupt) {
      // Remove a  handler to the list of the console handlers
      ::SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleSigHandler, FALSE);
   }
   return TSystem::RemoveSignalHandler(h);
}

//______________________________________________________________________________
void TWinNTSystem::ResetSignal(ESignals sig, Bool_t reset)
{
   // If reset is true reset the signal handler for the specified signal
   // to the default handler, else restore previous behaviour.

   //FIXME!
}

//______________________________________________________________________________
void TWinNTSystem::IgnoreSignal(ESignals sig, Bool_t ignore)
{
   // If ignore is true ignore the specified signal, else restore previous
   // behaviour.

   // FIXME!
}

//______________________________________________________________________________
Int_t TWinNTSystem::GetFPEMask()
{
   // Return the bitmap of conditions that trigger a floating point exception.

   Int_t mask = 0;
   UInt_t oldmask = _statusfp( );

   if (oldmask & _EM_INVALID  )   mask |= kInvalid;
   if (oldmask & _EM_ZERODIVIDE)  mask |= kDivByZero;
   if (oldmask & _EM_OVERFLOW )   mask |= kOverflow;
   if (oldmask & _EM_UNDERFLOW)   mask |= kUnderflow;
   if (oldmask & _EM_INEXACT  )   mask |= kInexact;

   return mask;
}

//______________________________________________________________________________
Int_t TWinNTSystem::SetFPEMask(Int_t mask)
{
   // Set which conditions trigger a floating point exception.
   // Return the previous set of conditions.

   Int_t old = GetFPEMask();

   UInt_t newm = 0;
   if (mask & kInvalid  )   newm |= _EM_INVALID;
   if (mask & kDivByZero)   newm |= _EM_ZERODIVIDE;
   if (mask & kOverflow )   newm |= _EM_OVERFLOW;
   if (mask & kUnderflow)   newm |= _EM_UNDERFLOW;
   if (mask & kInexact  )   newm |= _EM_INEXACT;

   UInt_t cm = ::_statusfp();
   cm &= ~newm;
   ::_controlfp(cm , _MCW_EM);

   return old;
}

#ifndef GDK_WIN32

//______________________________________________________________________________
Bool_t TWinNTSystem::ProcessEvents()
{
   // Events are processed by separate thread. Here we just return the
   // interrupt value the might have been set in command thread.

   Bool_t intr = gROOT->IsInterrupted();
   gROOT->SetInterrupt(kFALSE);
   return intr;
}

//______________________________________________________________________________
void TWinNTSystem::DispatchOneEvent(Bool_t)
{
 // Dispatch a single event via Command thread

  if (!gApplication->HandleTermInput()) {
     // wait ExitLoop()
     ::WaitForSingleObject(fhTermInputEvent, INFINITE);
     ::ResetEvent(fhTermInputEvent);
  }
}

#else // GDK_WIN32

//______________________________________________________________________________
Bool_t TWinNTSystem::ProcessEvents()
{
   // process pending events, i.e. DispatchOneEvent(kTRUE)

   return TSystem::ProcessEvents();
}

//______________________________________________________________________________
void TWinNTSystem::DispatchOneEvent(Bool_t pendingOnly)
{
   // Dispatch a single event in TApplication::Run() loop

   if (gConsoleEvent) ::SetEvent(gConsoleEvent);

   Bool_t pollOnce = pendingOnly;

   while (1) {
      if (gROOT->IsLineProcessing() && !gVirtualX->IsCmdThread()) {
         if (!pendingOnly) {
            SleepEx(1, TRUE);
            return;
         }
      }
      // first handle any GUI events
      if (gXDisplay && !gROOT->IsBatch()) {
         if (gXDisplay->Notify()) {
            if (!pendingOnly) {
               return;
            }
         }
      }

      // check for file descriptors ready for reading/writing
      if ((fNfd > 0) && fFileHandler && (fFileHandler->GetSize() > 0)) {
         if (CheckDescriptors()) {
            if (!pendingOnly) {
               return;
            }
         }
      }
      fNfd = 0;
      fReadready->Zero();
      fWriteready->Zero();

      // check synchronous signals
      if (fSigcnt > 0 && fSignalHandler->GetSize() > 0) {
         if (CheckSignals(kTRUE)) {
            if (!pendingOnly) {
               return;
            }
         }
      }
      fSigcnt = 0;
      fSignals->Zero();

      // handle past due timers
      if (fTimers && fTimers->GetSize() > 0) {
         if (DispatchTimers(kTRUE)) {
            // prevent timers from blocking the rest types of events
            Long_t to = NextTimeOut(kTRUE);
            if (to > kItimerResolution || to == -1) {
               return;
            }
         }
      }

      // if in pendingOnly mode poll once file descriptor activity
      Long_t nextto = NextTimeOut(kTRUE);
      if (pendingOnly) {
         if (pollOnce && fFileHandler && fFileHandler->GetSize() > 0) {
            nextto = 0;
            pollOnce = kFALSE;
         } else
            return;
      }

      if (fReadmask && !fReadmask->GetBits() &&
          fWritemask && !fWritemask->GetBits()) {
         ::SleepEx(1, 1);
         return;
      }

      *fReadready  = *fReadmask;
      *fWriteready = *fWritemask;

      fNfd = WinNTSelect(fReadready, fWriteready, nextto);

      // serious error has happened -> reset all file descrptors
      if ((fNfd < 0) && (fNfd != -2)) {
         int fd, rc, i;

         for (i = 0; i < fReadmask->GetCount(); i++) {
            TFdSet t;
            Int_t fd = fReadmask->GetFd(i);
            t.Set(fd);
            if (fReadmask->IsSet(fd)) {
               rc = WinNTSelect(&t, 0, 0);
               if (rc < 0 && rc != -2) {
                  ::SysError("DispatchOneEvent", "select: read error on %d\n", fd);
                  fReadmask->Clr(fd);
               }
            }
         }

         for (i = 0; i < fWritemask->GetCount(); i++) {
            TFdSet t;
            Int_t fd = fWritemask->GetFd(i);
            t.Set(fd);

            if (fWritemask->IsSet(fd)) {
               rc = WinNTSelect(0, &t, 0);
               if (rc < 0 && rc != -2) {
                  ::SysError("DispatchOneEvent", "select: write error on %d\n", fd);
                  fWritemask->Clr(fd);
               }
            }
            t.Clr(fd);
         }
      }
   }
}

#endif  // GDK_WIN32

//______________________________________________________________________________
void TWinNTSystem::ExitLoop()
{
   //

   TSystem::ExitLoop();
#ifndef GDK_WIN32
   // Release Dispatch one event
   if (fhTermInputEvent) ::SetEvent(fhTermInputEvent);
#endif
}

//---- handling of system events -----------------------------------------------
//______________________________________________________________________________
Bool_t TWinNTSystem::CheckSignals(Bool_t sync)
{
   // Check if some signals were raised and call their Notify() member.

   TSignalHandler *sh;
   Int_t sigdone = -1;
   {
      TIter next(fSignalHandler);

      while (sh = (TSignalHandler*)next()) {
         if (sync == sh->IsSync()) {
            ESignals sig = sh->GetSignal();
            if ((fSignals->IsSet(sig) && sigdone == -1) || sigdone == sig) {
               if (sigdone == -1) {
                  fSignals->Clr(sig);
                  sigdone = sig;
                  fSigcnt--;
               }
               sh->Notify();
            }
         }
      }
   }
   if (sigdone != -1) return kTRUE;

   return kFALSE;
}

//______________________________________________________________________________
Bool_t TWinNTSystem::CheckDescriptors()
{
   // Check if there is activity on some file descriptors and call their
   // Notify() member.

   TFileHandler *fh;
   Int_t  fddone = -1;
   Bool_t read   = kFALSE;

   TOrdCollectionIter it((TOrdCollection*)fFileHandler);

   while ((fh = (TFileHandler*) it.Next())) {
      Int_t fd = fh->GetFd();
      if (!fd) continue; // ignore TTermInputHandler

      if ((fReadready->IsSet(fd) && fddone == -1) ||
          (fddone == fd && read)) {
         if (fddone == -1) {
            fReadready->Clr(fd);
            fddone = fd;
            read = kTRUE;
            fNfd--;
         }
         fh->ReadNotify();
      }
      if ((fWriteready->IsSet(fd) && fddone == -1) ||
          (fddone == fd && !read)) {
         if (fddone == -1) {
            fWriteready->Clr(fd);
            fddone = fd;
            read = kFALSE;
            fNfd--;
         }
         fh->WriteNotify();
      }
   }
   if (fddone != -1) return kTRUE;

   return kFALSE;
}

//---- Directories -------------------------------------------------------------

//______________________________________________________________________________
int TWinNTSystem::mkdir(const char *name, Bool_t recursive)
{
   // Make a file system directory. Returns 0 in case of success and
   // -1 if the directory could not be created (either already exists or
   // illegal path name).
   // If 'recursive' is true, makes parent directories as needed.

   if (recursive) {
      TString dirname = DirName(name);
      if (dirname.Length() == 0) {
         // well we should not have to make the root of the file system!
         // (and this avoid infinite recursions!)
         return 0;
      }
      if (IsAbsoluteFileName(name)) {
         // For some good reason DirName strips off the drive letter
         // (if present), we need it to make the directory on the
         // right disk, so let's put it back!
         const char driveletter = DriveName(name);
         if (driveletter) {
            dirname.Prepend(":");
            dirname.Prepend(driveletter);
         }
      }
      if (AccessPathName(dirname, kFileExists)) {
         int res = this->mkdir(dirname, kTRUE);
         if (res) return res;
      }
      if (!AccessPathName(name, kFileExists)) {
         return -1;
      }
   }
   return MakeDirectory(name);
}

//______________________________________________________________________________
int  TWinNTSystem::MakeDirectory(const char *name)
{
   // Make a WinNT file system directory. Returns 0 in case of success and
   // -1 if the directory could not be created (either already exists or
   // illegal path name).

   TSystem *helper = FindHelper(name);
   if (helper) {
      return helper->MakeDirectory(name);
   }

#ifdef WATCOM
   // It must be as follows
   if (!name) return 0;
   return ::mkdir(name);
#else
   // but to be in line with TUnixSystem I did like this
   if (!name) return 0;
   return ::_mkdir(name);
#endif
}

//______________________________________________________________________________
void TWinNTSystem::FreeDirectory(void *dirp)
{
   // Close a WinNT file system directory.

   TSystem *helper = FindHelper(0, dirp);
   if (helper) {
      helper->FreeDirectory(dirp);
      return;
   }

   if (dirp) {
      ::FindClose(dirp);
   }
}

//______________________________________________________________________________
const char *TWinNTSystem::GetDirEntry(void *dirp)
{
   // Returns the next directory entry.

   TSystem *helper = FindHelper(0, dirp);
   if (helper) {
      return helper->GetDirEntry(dirp);
   }

   if (dirp) {
      HANDLE searchFile = (HANDLE)dirp;
      if (::FindNextFile(searchFile, &fFindFileData)) {
         return (const char *)fFindFileData.cFileName;
      }
   }
   return 0;
}

//______________________________________________________________________________
Bool_t TWinNTSystem::ChangeDirectory(const char *path)
{
   // Change directory.

   Bool_t ret = (Bool_t) (::chdir(path) == 0);
   if (fWdpath != "") {
      fWdpath = "";   // invalidate path cache
   }
   return ret;
}

//______________________________________________________________________________
__inline BOOL DBL_BSLASH(LPCTSTR psz)
{
   //
   // Inline function to check for a double-backslash at the
   // beginning of a string
   //
   return (psz[0] == TEXT('\\') && psz[1] == TEXT('\\'));
}

//______________________________________________________________________________
BOOL PathIsUNC(LPCTSTR pszPath)
{
   // Returns TRUE if the given string is a UNC path.
   //
   // TRUE
   //      "\\foo\bar"
   //      "\\foo"         <- careful
   //      "\\"
   // FALSE
   //      "\foo"
   //      "foo"
   //      "c:\foo"
   return DBL_BSLASH(pszPath);
}

#pragma data_seg(".text", "CODE")
const TCHAR c_szColonSlash[] = TEXT(":\\");
#pragma data_seg()

//______________________________________________________________________________
BOOL PathIsRoot(LPCTSTR pPath)
{
   //
   // check if a path is a root
   //
   // returns:
   //  TRUE for "\" "X:\" "\\foo\asdf" "\\foo\"
   //  FALSE for others
   //
   if (!IsDBCSLeadByte(*pPath)) {
      if (!lstrcmpi(pPath + 1, c_szColonSlash))
         // "X:\" case
         return TRUE;
   }
   if ((*pPath == TEXT('\\')) && (*(pPath + 1) == 0))
      // "\" case
      return TRUE;
   if (DBL_BSLASH(pPath)) {
      // smells like UNC name
      LPCTSTR p;
      int cBackslashes = 0;
      for (p = pPath + 2; *p; p = CharNext(p)) {
         if (*p == TEXT('\\') && (++cBackslashes > 1))
            return FALSE;   // not a bare UNC name, therefore not a root dir
      }
      // end of string with only 1 more backslash
      // must be a bare UNC, which looks like a root dir
      return TRUE;
   }
   return FALSE;
}

//______________________________________________________________________________
void *TWinNTSystem::OpenDirectory(const char *dir)
{
   // Open a directory. Returns 0 if directory does not exist.

   TSystem *helper = FindHelper(dir);
   if (helper) {
      return helper->OpenDirectory(dir);
   }

   char *entry = new char[strlen(dir)+3];
   struct _stati64 finfo;

   if(PathIsUNC(dir)) {
      strcpy(entry, dir);
      if ((entry[strlen(dir)-1] == '/') || (entry[strlen(dir)-1] == '\\' )) {
         entry[strlen(dir)-1] = '\0';
      }
      if(PathIsRoot(entry)) {
         strcat(entry,"\\");
      }
      if (_stati64(entry, &finfo) < 0) {
         delete [] entry;
         return 0;
      }
   }
   else {
      strcpy(entry, dir);
      if ((entry[strlen(dir)-1] == '/') || (entry[strlen(dir)-1] == '\\' )) {
         if(!PathIsRoot(entry))
            entry[strlen(dir)-1] = '\0';
      }
      if (_stati64(entry, &finfo) < 0) {
         delete [] entry;
         return 0;
      }
   }

   if (finfo.st_mode & S_IFDIR) {
      strcpy(entry, dir);
      if (!(entry[strlen(dir)-1] == '/' || entry[strlen(dir)-1] == '\\' )) {
         strcat(entry,"\\");
      }
      strcat(entry,"*");

      HANDLE searchFile;
      searchFile = ::FindFirstFile(entry, &fFindFileData);
      if (searchFile == INVALID_HANDLE_VALUE) {
         ((TWinNTSystem *)gSystem)->Error( "Unable to find' for reading:", entry);
         delete [] entry;
         return 0;
      }
      delete [] entry;
      return searchFile;
   } else {
      delete [] entry;
      return 0;
   }
}

//______________________________________________________________________________
const char *TWinNTSystem::WorkingDirectory()
{
   // Return the working directory for the default drive

   return WorkingDirectory('\0');
}

//______________________________________________________________________________
const char *TWinNTSystem::WorkingDirectory(char driveletter)
{
   //  Return working directory for the selected drive
   //  driveletter == 0 means return the working durectory for the default drive

   char *wdpath = 0;
   char drive = driveletter ? toupper( driveletter ) - 'A' + 1 : 0;

   if (fWdpath != "" ) {
      return fWdpath;
   }

   if (!(wdpath = ::_getdcwd( (int)drive, wdpath, kMAXPATHLEN))) {
      free(wdpath);
      Warning("WorkingDirectory", "getcwd() failed");
      return 0;
   }
   fWdpath = wdpath;
   free(wdpath);
   return fWdpath;
}

//______________________________________________________________________________
const char *TWinNTSystem::HomeDirectory(const char *userName)
{
   // Return the user's home directory.

   static char mydir[kMAXPATHLEN] = "./";
   const char *h = 0;
   if (!(h = ::getenv("home"))) h = ::getenv("HOME");

   if (h) {
      strcpy(mydir, h);
   } else {
      // for Windows NT HOME might be defined as either $(HOMESHARE)/$(HOMEPATH)
      //                                         or     $(HOMEDRIVE)/$(HOMEPATH)
      h = ::getenv("HOMESHARE");
      if (!h)  h = ::getenv("HOMEDRIVE");
      if (h) {
         strcpy(mydir, h);
         h = ::getenv("HOMEPATH");
         if(h) strcat(mydir, h);
      }
   }
   return mydir;
}

//______________________________________________________________________________
const char *TWinNTSystem::TempDirectory() const
{
   // Return a user configured or systemwide directory to create
   // temporary files in.

   const char *dir =  gSystem->Getenv("TEMP");
   if (!dir)   dir =  gSystem->Getenv("TEMPDIR");
   if (!dir)   dir =  gSystem->Getenv("TEMP_DIR");
   if (!dir)   dir =  gSystem->Getenv("TMP");
   if (!dir)   dir =  gSystem->Getenv("TMPDIR");
   if (!dir)   dir =  gSystem->Getenv("TMP_DIR");
   if (!dir) dir = "c:\\";

   return dir;
}

//______________________________________________________________________________
FILE *TWinNTSystem::TempFileName(TString &base, const char *dir)
{
   // Create a secure temporary file by appending a unique
   // 6 letter string to base. The file will be created in
   // a standard (system) directory or in the directory
   // provided in dir. The full filename is returned in base
   // and a filepointer is returned for safely writing to the file
   // (this avoids certain security problems). Returns 0 in case
   // of error.

   char tmpName[MAX_PATH];

   ::GetTempFileName(dir ? dir : TempDirectory(), base.Data(), 0, tmpName);
   base = tmpName;
   FILE *fp = fopen(tmpName, "w+");

   if (!fp) ::SysError("TempFileName", "error opening %s", tmpName);

   return fp;
}

//---- Paths & Files -----------------------------------------------------------

//______________________________________________________________________________
const char *TWinNTSystem::DirName(const char *pathname)
{
   // Return the directory name in pathname. DirName of c:/user/root is /user.
   // It creates output with 'new char []' operator. Returned string has to
   // be deleted.

   // Delete old buffer
   if (fDirNameBuffer) {
      // delete [] fDirNameBuffer;
      fDirNameBuffer = 0;
   }

   // Create a buffer to keep the path name
   if (pathname) {
      if (strchr(pathname, '/') || strchr(pathname, '\\')) {
         const char *rslash = strrchr(pathname, '/');
         const char *bslash = strrchr(pathname, '\\');
         const char *r = max(rslash, bslash);
         const char *ptr = pathname;
         while (ptr <= r) {
            if (*ptr == ':') {
               // Windows path may contain a drive letter
               // For NTFS ":" may be a "stream" delimiter as well
               pathname =  ptr + 1;
               break;
            }
            ptr++;
         }
         int len =  r - pathname;
         if (len > 0) {
            fDirNameBuffer = new char[len+1];
            memcpy(fDirNameBuffer, pathname, len);
            fDirNameBuffer[len] = 0;
         }
      }
   }
   if (!fDirNameBuffer) {
      fDirNameBuffer = new char[1];
      *fDirNameBuffer = '\0'; // Set the empty default response
   }
   return fDirNameBuffer;
}

//______________________________________________________________________________
const char TWinNTSystem::DriveName(const char *pathname)
{
   ////////////////////////////////////////////////////////////////////////////
   // Return the drive letter in pathname. DriveName of 'c:/user/root' is 'c'//
   //   Input:                                                               //
   //      pathname - the string containing file name                        //
   //   Return:                                                              //
   //     = Letter presenting the drive letter in the file name              //
   //     = The current drive if the pathname has no drive assigment         //
   //     = 0 if pathname is an empty string  or uses UNC syntax             //
   //   Note:                                                                //
   //      It doesn't chech whether pathname presents the 'real filename     //
   //      This subroutine looks for 'single letter' is follows with a ':'   //
   ////////////////////////////////////////////////////////////////////////////

   if (!pathname)    return 0;
   if (!pathname[0]) return 0;

   const char *lpchar;
   lpchar = pathname;

   // Skip blanks
   while(*lpchar == ' ') lpchar++;

   if (isalpha((int)*lpchar) && *(lpchar+1) == ':') {
      return *lpchar;
   }
   // Test UNC syntax
   if ( (*lpchar == '\\' || *lpchar == '/' ) &&
        (*(lpchar+1) == '\\' || *(lpchar+1) == '/') ) return 0;

   // return the current drive
   return DriveName(WorkingDirectory());
}

//______________________________________________________________________________
Bool_t TWinNTSystem::IsAbsoluteFileName(const char *dir)
{
   // Return true if dir is an absolute pathname.

   if (dir) {
      int idx = 0;
      if (strchr(dir,':')) idx = 2;
      return  (dir[idx] == '/' || dir[idx] == '\\');
   }
   return kFALSE;
}

//______________________________________________________________________________
const char *TWinNTSystem::UnixPathName(const char *name)
{
   // Convert a pathname to a unix pathname. E.g. form \user\root to /user/root.
   // General rules for applications creating names for directories and files or
   // processing names supplied by the user include the following:
   //
   //  �  Use any character in the current code page for a name, but do not use
   //     a path separator, a character in the range 0 through 31, or any character
   //     explicitly disallowed by the file system. A name can contain characters
   //     in the extended character set (128-255).
   //  �  Use the backslash (\), the forward slash (/), or both to separate
   //     components in a path. No other character is acceptable as a path separator.
   //  �  Use a period (.) as a directory component in a path to represent the
   //     current directory.
   //  �  Use two consecutive periods (..) as a directory component in a path to
   //     represent the parent of the current directory.
   //  �  Use a period (.) to separate components in a directory name or filename.
   //  �  Do not use the following characters in directory names or filenames, because
   //     they are reserved for Windows:
   //                      < > : " / \ |
   //  �  Do not use reserved words, such as aux, con, and prn, as filenames or
   //     directory names.
   //  �  Process a path as a null-terminated string. The maximum length for a path
   //     is given by MAX_PATH.
   //  �  Do not assume case sensitivity. Consider names such as OSCAR, Oscar, and
   //     oscar to be the same.

   static char temp[1024];
   strcpy(temp, name);
   char *currentChar = temp;

   while (*currentChar != '\0') {
      if (*currentChar == '\\') *currentChar = '/';
      currentChar++;
   }
   return temp;
}

//______________________________________________________________________________
Bool_t TWinNTSystem::AccessPathName(const char *path, EAccessMode mode)
{
   // Returns FALSE if one can access a file using the specified access mode.
   // Mode is the same as for the WinNT access(2) function.
   // Attention, bizarre convention of return value!!

   TSystem *helper = FindHelper(path);
   if (helper) {
      return helper->AccessPathName(path, mode);
   }
   if (::_access(path, mode) == 0) {
      return kFALSE;
   }
   fLastErrorString = GetError();
   return kTRUE;
}

//______________________________________________________________________________
char *TWinNTSystem::ConcatFileName(const char *dir, const char *name)
{
   // Concatenate a directory and a file name. Returned string must be
   // deleted by user.

   Int_t ldir  = dir  ? strlen(dir) : 0;
   Int_t lname = name ? strlen(name) : 0;

   if (!lname) return StrDup(dir);

   char *buf = new char[ldir+lname+2];

   if (ldir) {
      // Test whether the last symbol of the directory is a separator
      char last = dir[ldir-1];
      if (last == '/' || last == '\\' || last == ':') {
         sprintf(buf, "%s%s", dir, name);
      } else {
         sprintf(buf, "%s\\%s", dir, name);
      }
   } else {
      sprintf(buf, "\\%s", name);
   }
   return buf;
}

//______________________________________________________________________________
int TWinNTSystem::CopyFile(const char *f, const char *t, Bool_t overwrite)
{
   // Copy a file. If overwrite is true and file already exists the
   // file will be overwritten. Returns 0 when successful, -1 in case
   // of failure, -2 in case the file already exists and overwrite was false.

   if (AccessPathName(f, kReadPermission)) return -1;
   if (!AccessPathName(t) && !overwrite) return -2;

   Bool_t ret = ::CopyFileA(f, t, kFALSE);

   if (!ret) return -1;
   return 0;
}

//______________________________________________________________________________
int TWinNTSystem::Rename(const char *f, const char *t)
{
   // Rename a file. Returns 0 when successful, -1 in case of failure.

   int ret = ::rename(f, t);
   fLastErrorString = GetError();
   return ret;
}

//______________________________________________________________________________
int TWinNTSystem::GetPathInfo(const char *path, FileStat_t &buf)
{
   // Get info about a file. Info is returned in the form of a FileStat_t
   // structure (see TSystem.h).
   // The function returns 0 in case of success and 1 if the file could
   // not be stat'ed.

   TSystem *helper = FindHelper(path);
   if (helper)
      return helper->GetPathInfo(path, buf);

   struct _stati64 sbuf;

   // Remove trailing backslashes
   char *newpath = StrDup(path);
   int l = strlen(newpath);
   while (l > 1) {
      if (newpath[--l] != '\\' || newpath[--l] != '/') {
         break;
      }
      newpath[l] = '\0';
   }
   if (newpath && ::_stati64(newpath, &sbuf) >= 0) {

      buf.fDev    = sbuf.st_dev;
      buf.fIno    = sbuf.st_ino;
      buf.fMode   = sbuf.st_mode;
      buf.fUid    = sbuf.st_uid;
      buf.fGid    = sbuf.st_gid;
      buf.fSize   = sbuf.st_size;
      buf.fMtime  = sbuf.st_mtime;
      buf.fIsLink = kFALSE;

      delete [] newpath;
      return 0;
   }
   delete [] newpath;
   return 1;
}

//______________________________________________________________________________
int TWinNTSystem::GetFsInfo(const char *path, Long_t *id, Long_t *bsize,
                            Long_t *blocks, Long_t *bfree)
{
   // Get info about a file system: id, bsize, bfree, blocks.
   // Id      is file system type (machine dependend, see statfs())
   // Bsize   is block size of file system
   // Blocks  is total number of blocks in file system
   // Bfree   is number of free blocks in file system
   // The function returns 0 in case of success and 1 if the file system could
   // not be stat'ed.

   // address of root directory of the file system
   LPCTSTR lpRootPathName = path;

   // address of name of the volume
   LPTSTR  lpVolumeNameBuffer = 0;
   DWORD   nVolumeNameSize = 0;

   DWORD   volumeSerialNumber;     // volume serial number
   DWORD   maximumComponentLength; // system's maximum filename length

   // file system flags
   DWORD fileSystemFlags;

   // address of name of file system
   char  fileSystemNameBuffer[512];
   DWORD nFileSystemNameSize = sizeof(fileSystemNameBuffer);

   if (!::GetVolumeInformation(lpRootPathName,
                               lpVolumeNameBuffer, nVolumeNameSize,
                               &volumeSerialNumber,
                               &maximumComponentLength,
                               &fileSystemFlags,
                               fileSystemNameBuffer, nFileSystemNameSize)) {
      return 1;
   }

   const char *fsNames[] = { "FAT", "NTFS" };
   int i;
   for (i = 0; i < 2; i++) {
      strncmp(fileSystemNameBuffer, fsNames[i], nFileSystemNameSize);
   }
   *id = i;

   DWORD sectorsPerCluster;      // # sectors per cluster
   DWORD bytesPerSector;         // # bytes per sector
   DWORD numberOfFreeClusters;   // # free clusters
   DWORD totalNumberOfClusters;  // # total of clusters

   if (!::GetDiskFreeSpace(lpRootPathName,
                           &sectorsPerCluster,
                           &bytesPerSector,
                           &numberOfFreeClusters,
                           &totalNumberOfClusters)) {
      return 1;
   }

   *bsize  = sectorsPerCluster * bytesPerSector;
   *blocks = totalNumberOfClusters;
   *bfree  = numberOfFreeClusters;

   return 0;
}

//______________________________________________________________________________
int TWinNTSystem::Link(const char *from, const char *to)
{
   // Create a link from file1 to file2.

   return 0;
}

//______________________________________________________________________________
int TWinNTSystem::Unlink(const char *name)
{
   // Unlink, i.e. remove, a file or directory.

   struct _stati64 finfo;

   if (_stati64(name, &finfo) < 0) {
      return -1;
   }

   if (finfo.st_mode & S_IFDIR) {
      return ::_rmdir(name);
   } else {
      return ::_unlink(name);
   }
}

//______________________________________________________________________________
int TWinNTSystem::SetNonBlock(int fd)
{
   // Make descriptor fd non-blocking.

   if (::ioctlsocket(fd, FIONBIO, (u_long *)1) == SOCKET_ERROR) {
      ::SysError("SetNonBlock", "ioctlsocket");
      return -1;
   }
   return 0;
}

// expand the metacharacters as in the shell

static char
   *shellMeta      = "~*[]{}?$%",
   *shellStuff     = "(){}<>\"'",
   shellEscape     = '\\';

//______________________________________________________________________________
Bool_t TWinNTSystem::ExpandPathName(TString &patbuf0)
{
   // Expand a pathname getting rid of special shell characaters like ~.$, etc.

   const char *patbuf = (const char *)patbuf0;
   const char *hd, *p;
   char   *cmd = 0;
   char  *q;
   int    ch, i;

   // skip leading blanks
   while (*patbuf == ' ') {
      patbuf++;
   }

   // skip leading ':'
   while (*patbuf == ':') {
      patbuf++;
   }

   // skip leading ';'
   while (*patbuf == ';') {
      patbuf++;
   }

   // Transform a Unix list of directories into a Windows list
   // by changing the separator from ':' into ';'
   for (q = (char*)patbuf; *q; q++) {
      if ( *q == ':' ) {
         // We are avoiding substitution in the case of
         // ....;c:.... and of ...;root:/... where root can be any url protocol
         if ( (((q-2)>patbuf) && ( (*(q-2)!=';') || !isalpha(*(q-1)) )) &&
              *(q+1)!='/' ) {
            *q=';';
         }
      }
   }
   // any shell meta characters ?
   for (p = patbuf; *p; p++) {
      if (strchr(shellMeta, *p)) {
         goto needshell;
      }
   }
   return kFALSE;

needshell:

   // Because (problably) we built with cygwin, the path name like:
   //     LOCALS~1\\Temp
   // gets extended to
   //     LOCALSc:\\Devel
   // The most likely cause is that '~' is used with Unix semantic of the
   // home directory (and it also cuts the path short after ... who knows why!)
   // So we need to detect this case and prevents its expansion :(.

   char replacement[4];

   // intentionally a non visible, unlikely character
   for (int k = 0; k<3; k++) replacement[k] = 0x1;

   replacement[3] = 0x0;
   Ssiz_t pos = 0;
   TRegexp TildaNum = "~[0-9]";

   while ( (pos = patbuf0.Index(TildaNum,pos)) != kNPOS ) {
      patbuf0.Replace(pos, 1, replacement);
   }

   // escape shell quote characters
   // EscChar(patbuf, stuffedPat, sizeof(stuffedPat), shellStuff, shellEscape);
   patbuf0 = ExpandFileName(patbuf0.Data());
   Int_t lbuf = ::ExpandEnvironmentStrings(
                                 patbuf0.Data(), // pointer to string with environment variables
                                 cmd,            // pointer to string with expanded environment variables
                                 0               // maximum characters in expanded string
                              );
   if (lbuf > 0) {
      cmd = new char[lbuf+1];
      ::ExpandEnvironmentStrings(
                               patbuf0.Data(), // pointer to string with environment variables
                               cmd,            // pointer to string with expanded environment variables
                               lbuf            // maximum characters in expanded string
                               );
      patbuf0 = cmd;
      patbuf0.ReplaceAll(replacement, "~");
      return kFALSE;
   }
   return kTRUE;
}

//______________________________________________________________________________
char *TWinNTSystem::ExpandPathName(const char *path)
{
   // Expand a pathname getting rid of special shell characaters like ~.$, etc.
   // User must delete returned string.

   TString patbuf = path;
   if (ExpandPathName(patbuf)) return 0;

   return StrDup(patbuf.Data());
}

//______________________________________________________________________________
int TWinNTSystem::Chmod(const char *file, UInt_t mode)
{
   // Set the file permission bits. Returns -1 in case or error, 0 otherwise.
   // On windows mode can only be a combination of "user read" (0400),
   // "user write" (0200) or "user read | user write" (0600). Any other value
   // for mode are ignored.

   return ::_chmod(file, mode);
}

//______________________________________________________________________________
int TWinNTSystem::Umask(Int_t mask)
{
   // Set the process file creation mode mask.

   return ::umask(mask);
}

//______________________________________________________________________________
int TWinNTSystem::Utime(const char *file, Long_t modtime, Long_t actime)
{
   // Set a files modification and access times. If actime = 0 it will be
   // set to the modtime. Returns 0 on success and -1 in case of error.

   if (AccessPathName(file, kWritePermission)) {
      Error("Utime", "need write permission for %s to change utime", file);
      return -1;
   }
   if (!actime) actime = modtime;

   struct utimbuf t;
   t.actime  = (time_t)actime;
   t.modtime = (time_t)modtime;
   return ::utime(file, &t);
}

//______________________________________________________________________________
char *TWinNTSystem::Which(const char *search, const char *infile, EAccessMode mode)
{
   // Find location of file in a search path.
   // User must delete returned string. Returns 0 in case file is not found.

   static char name[kMAXPATHLEN];
   char *lpFilePart = 0;
   char *found = 0;

   // Expand parameters

   char *exinfile = gSystem->ExpandPathName(infile);
   // Check whether this infile has the absolute path first
   if (IsAbsoluteFileName(exinfile) ) {
      found = exinfile;
   } else {
      char *tmp = gSystem->ExpandPathName(search);
      TString exsearch(tmp);
      delete [] tmp;

      // Need to use Windows delimiters
      Int_t lastDelim = -1;
      for(int i=0; i<exsearch.Length(); ++i) {
         switch( exsearch[i] ) {
            case ':': if (i-lastDelim!=2) exsearch[i] = ';'; // Replace the ':' unless there are after a disk suffix (aka ;c:\mydirec...)
               lastDelim = i;
               break;
            case ';': lastDelim = i; break;
         }
      }

      // Check access
      struct stat finfo;
      if (::SearchPath(exsearch, exinfile, NULL, kMAXPATHLEN, name, &lpFilePart) &&
          ::access(name, mode) == 0 && stat(name, &finfo) == 0 &&
          finfo.st_mode & S_IFREG) {
         if (gEnv->GetValue("Root.ShowPath", 0)) {
            Printf("Which: %s = %s", infile, name);
         }
         found = StrDup(name);
      }
      delete [] exinfile;
   }

   if (found  && AccessPathName(found, mode)) {
      delete [] found;
      found = 0;
   }
   return found;
}

//---- Users & Groups ----------------------------------------------------------

//______________________________________________________________________________
Bool_t TWinNTSystem::InitUsersGroups()
{
   // Collect local users and groups accounts informations

   // Net* API functions allowed and OS is Windows NT/2000/XP
   if ((gEnv->GetValue("WinNT.UseNetAPI", 0)) && (::GetVersion() < 0x80000000)) {
      fActUser = -1;
      fNbGroups = fNbUsers = 0;
      HINSTANCE netapi = ::LoadLibrary("netapi32.DLL");
      if (!netapi) return kFALSE;

      p2NetApiBufferFree  = (pfn1)::GetProcAddress(netapi, "NetApiBufferFree");
      p2NetUserGetInfo  = (pfn2)::GetProcAddress(netapi, "NetUserGetInfo");
      p2NetLocalGroupGetMembers  = (pfn3)::GetProcAddress(netapi, "NetLocalGroupGetMembers");
      p2NetLocalGroupEnum = (pfn4)::GetProcAddress(netapi, "NetLocalGroupEnum");

      if (!p2NetApiBufferFree || !p2NetUserGetInfo ||
          !p2NetLocalGroupGetMembers || !p2NetLocalGroupEnum) return kFALSE;

      GetNbGroups();

      fGroups = (struct group *)calloc(fNbGroups, sizeof(struct group));
      for(int i=0;i<fNbGroups;i++) {
        fGroups[i].gr_mem = (char **)calloc(fNbUsers, sizeof (char*));
      }
      fPasswords = (struct passwd *)calloc(fNbUsers, sizeof(struct passwd));

      CollectGroups();
      ::FreeLibrary(netapi);
   }
   fGroupsInitDone = kTRUE;
   return kTRUE;
}

//________________________________________________________________________________
Bool_t TWinNTSystem::CountMembers(const char *lpszGroupName)
{
   NET_API_STATUS NetStatus = NERR_Success;
   LPBYTE Data = NULL;
   DWORD Index = 0, ResumeHandle = 0, Total = 0;
   LOCALGROUP_MEMBERS_INFO_1 *MemberInfo;
   WCHAR wszGroupName[256];
   int iRetOp = 0;
   DWORD dwLastError = 0;

   iRetOp = MultiByteToWideChar (
            (UINT)CP_ACP,                // code page
            (DWORD)MB_PRECOMPOSED,       // character-type options
            (LPCSTR)lpszGroupName,       // address of string to map
            (int)-1,                     // number of bytes in string
            (LPWSTR)wszGroupName,        // address of wide-character buffer
            (int)sizeof(wszGroupName) ); // size of buffer

   if (iRetOp == 0) {
      dwLastError = GetLastError();
      if (Data)
         p2NetApiBufferFree(Data);
      return FALSE;
   }

   // The NetLocalGroupGetMembers() API retrieves a list of the members
   // of a particular local group.
   NetStatus = p2NetLocalGroupGetMembers (NULL, wszGroupName, 1,
                            &Data, 8192, &Index, &Total, &ResumeHandle );

   if (NetStatus != NERR_Success || Data == NULL) {
      dwLastError = GetLastError();

      if (dwLastError == ERROR_ENVVAR_NOT_FOUND) {
         // This usually means that the current Group has no members.
         // We call NetLocalGroupGetMembers() again.
         // This time, we set the level to 0.
         // We do this just to confirm that the number of members in
         // this group is zero.
         NetStatus = p2NetLocalGroupGetMembers ( NULL, wszGroupName, 0,
                                  &Data, 8192, &Index, &Total, &ResumeHandle );
      }

      if (Data)
         p2NetApiBufferFree(Data);
      return FALSE;
   }

   fNbUsers += Total;
   MemberInfo = (LOCALGROUP_MEMBERS_INFO_1 *)Data;

   if (Data)
      p2NetApiBufferFree(Data);

   return TRUE;
}

//________________________________________________________________________________
Bool_t TWinNTSystem::GetNbGroups()
{
   NET_API_STATUS NetStatus = NERR_Success;
   LPBYTE Data = NULL;
   DWORD Index = 0, ResumeHandle = 0, Total = 0, i;
   LOCALGROUP_INFO_0 *GroupInfo;
   char szAnsiName[256];
   DWORD dwLastError = 0;
   int  iRetOp = 0;

   NetStatus = p2NetLocalGroupEnum(NULL, 0, &Data, 8192, &Index,
                                    &Total, &ResumeHandle );

   if (NetStatus != NERR_Success || Data == NULL) {
      dwLastError = GetLastError();
      if (Data)
         p2NetApiBufferFree(Data);
      return FALSE;
   }

   fNbGroups = Total;
   GroupInfo = (LOCALGROUP_INFO_0 *)Data;
   for (i=0; i < Total; i++) {
      // Convert group name from UNICODE to ansi.
      iRetOp = WideCharToMultiByte (
               (UINT)CP_ACP,                    // code page
               (DWORD)0,                        // performance and mapping flags
               (LPCWSTR)(GroupInfo->lgrpi0_name), // address of wide-char string
               (int)-1,                        // number of characters in string
               (LPSTR)szAnsiName,            // address of buffer for new string
               (int)(sizeof(szAnsiName)),    // size of buffer
               (LPCSTR)NULL,     // address of default for unmappable characters
               (LPBOOL)NULL );     // address of flag set when default char used.

      // Now lookup all members of this group and record down their names and
      // SIDs into the output file.
      CountMembers((LPCTSTR)szAnsiName);

      GroupInfo++;
   }

   if (Data)
      p2NetApiBufferFree(Data);

   return TRUE;
}

//________________________________________________________________________________
Long_t TWinNTSystem::LookupSID (const char *lpszAccountName, int what,
                                int &groupIdx, int &memberIdx)
{
   //
   // Take the name and look up a SID so that we can get full
   // domain/user information
   //
   BOOL bRetOp = FALSE;
   PSID pSid = NULL;
   DWORD dwSidSize, dwDomainNameSize;
   BYTE bySidBuffer[MAX_SID_SIZE];
   char szDomainName[MAX_NAME_STRING];
   SID_NAME_USE sidType;
   PUCHAR puchar_SubAuthCount = NULL;
   SID_IDENTIFIER_AUTHORITY sid_identifier_authority;
   PSID_IDENTIFIER_AUTHORITY psid_identifier_authority = NULL;
   char szIdentAuthValue[80];
   int i;
   unsigned char j = 0;
   DWORD dwLastError = 0;

   pSid = (PSID)bySidBuffer;
   dwSidSize = sizeof(bySidBuffer);
   dwDomainNameSize = sizeof(szDomainName);

   bRetOp = LookupAccountName (
            (LPCTSTR)NULL,             // address of string for system name
            (LPCTSTR)lpszAccountName,  // address of string for account name
            (PSID)pSid,                // address of security identifier
            (LPDWORD)&dwSidSize,       // address of size of security identifier
            (LPTSTR)szDomainName,      // address of string for referenced domain
            (LPDWORD)&dwDomainNameSize,// address of size of domain string
            (PSID_NAME_USE)&sidType ); // address of SID-type indicator

   if (bRetOp == FALSE) {
      dwLastError = GetLastError();
      return -1;  // Unable to obtain Account SID.
   }

   bRetOp = IsValidSid((PSID)pSid);

   if (bRetOp == FALSE) {
      dwLastError = GetLastError();
      return -2;  // SID returned is invalid.
   }

   // Obtain via APIs the identifier authority value.
   psid_identifier_authority = GetSidIdentifierAuthority ((PSID)pSid);

   // Make a copy of it.
   memcpy (&sid_identifier_authority, psid_identifier_authority,
       sizeof(SID_IDENTIFIER_AUTHORITY));

   // Determine how many sub-authority values there are in the current SID.
   puchar_SubAuthCount = (PUCHAR)GetSidSubAuthorityCount((PSID)pSid);
   // Assign it to a more convenient variable.
   j = (unsigned char)(*puchar_SubAuthCount);
   // Now obtain all the sub-authority values from the current SID.
   DWORD dwSubAuth = 0;
   PDWORD pdwSubAuth = NULL;
   char szSubAuthValue[80];
   // Obtain the current sub-authority DWORD (referenced by a pointer)
   pdwSubAuth = (PDWORD)GetSidSubAuthority (
                (PSID)pSid,  // address of security identifier to query
                (DWORD)j-1); // index of subauthority to retrieve
   dwSubAuth = *pdwSubAuth;
   if(what == SID_MEMBER) {
       fPasswords[memberIdx].pw_uid = dwSubAuth;
       fPasswords[memberIdx].pw_gid = fGroups[groupIdx].gr_gid;
       fPasswords[memberIdx].pw_group = strdup(fGroups[groupIdx].gr_name);
   }
   else if(what == SID_GROUP) {
       fGroups[groupIdx].gr_gid = dwSubAuth;
   }
   return 0;
}

//________________________________________________________________________________
Bool_t TWinNTSystem::CollectMembers(const char *lpszGroupName, int &groupIdx,
                                    int &memberIdx)
{
   //


   NET_API_STATUS NetStatus = NERR_Success;
   LPBYTE Data = NULL;
   DWORD Index = 0, ResumeHandle = 0, Total = 0, i;
   LOCALGROUP_MEMBERS_INFO_1 *MemberInfo;
   char szAnsiMemberName[256];
   char szFullMemberName[256];
   char szMemberHomeDir[256];
   WCHAR wszGroupName[256];
   int iRetOp = 0;
   char  act_name[256];
   DWORD length = sizeof (act_name);
   DWORD dwLastError = 0;
   LPUSER_INFO_11  pUI11Buf = NULL;
   NET_API_STATUS  nStatus;

   iRetOp = MultiByteToWideChar (
            (UINT)CP_ACP,                // code page
            (DWORD)MB_PRECOMPOSED,       // character-type options
            (LPCSTR)lpszGroupName,       // address of string to map
            (int)-1,                     // number of bytes in string
            (LPWSTR)wszGroupName,        // address of wide-character buffer
            (int)sizeof(wszGroupName) ); // size of buffer

   if (iRetOp == 0) {
      dwLastError = GetLastError();
      if (Data)
         p2NetApiBufferFree(Data);
      return FALSE;
   }

   GetUserName (act_name, &length);

   // The NetLocalGroupGetMembers() API retrieves a list of the members
   // of a particular local group.
   NetStatus = p2NetLocalGroupGetMembers (NULL, wszGroupName, 1,
                            &Data, 8192, &Index, &Total, &ResumeHandle );

   if (NetStatus != NERR_Success || Data == NULL) {
      dwLastError = GetLastError();

      if (dwLastError == ERROR_ENVVAR_NOT_FOUND) {
         // This usually means that the current Group has no members.
         // We call NetLocalGroupGetMembers() again.
         // This time, we set the level to 0.
         // We do this just to confirm that the number of members in
         // this group is zero.
         NetStatus = p2NetLocalGroupGetMembers ( NULL, wszGroupName, 0,
                                  &Data, 8192, &Index, &Total, &ResumeHandle );
      }

      if (Data)
         p2NetApiBufferFree(Data);
      return FALSE;
   }

   MemberInfo = (LOCALGROUP_MEMBERS_INFO_1 *)Data;
   for (i=0; i < Total; i++) {
      iRetOp = WideCharToMultiByte (
               (UINT)CP_ACP,                     // code page
               (DWORD)0,                         // performance and mapping flags
               (LPCWSTR)(MemberInfo->lgrmi1_name), // address of wide-char string
               (int)-1,                         // number of characters in string
               (LPSTR)szAnsiMemberName,       // address of buffer for new string
               (int)(sizeof(szAnsiMemberName)), // size of buffer
               (LPCSTR)NULL,      // address of default for unmappable characters
               (LPBOOL)NULL );      // address of flag set when default char used.

      if (iRetOp == 0) {
         dwLastError = GetLastError();
      }

      fPasswords[memberIdx].pw_name = strdup(szAnsiMemberName);
      fPasswords[memberIdx].pw_passwd = strdup("");
      fGroups[groupIdx].gr_mem[i] = strdup(szAnsiMemberName);

      if(fActUser == -1 && !stricmp(fPasswords[memberIdx].pw_name,act_name))
                      fActUser = memberIdx;


      TCHAR szUserName[255]=TEXT("");
      MultiByteToWideChar(CP_ACP, 0, szAnsiMemberName, -1, (LPWSTR)szUserName, 255);
      //
      // Call the NetUserGetInfo function; specify level 10.
      //
      nStatus = p2NetUserGetInfo(NULL, (LPCWSTR)szUserName, 11, (LPBYTE *)&pUI11Buf);
      //
      // If the call succeeds, print the user information.
      //
      if (nStatus == NERR_Success) {
         if (pUI11Buf != NULL) {
            wsprintf(szFullMemberName,"%S",pUI11Buf->usri11_full_name);
            fPasswords[memberIdx].pw_gecos = strdup(szFullMemberName);
            wsprintf(szMemberHomeDir,"%S",pUI11Buf->usri11_home_dir);
            fPasswords[memberIdx].pw_dir = strdup(szMemberHomeDir);
         }
      }
      if((fPasswords[memberIdx].pw_gecos == NULL) || (strlen(fPasswords[memberIdx].pw_gecos) == 0))
         fPasswords[memberIdx].pw_gecos = strdup(fPasswords[memberIdx].pw_name);
      if((fPasswords[memberIdx].pw_dir == NULL) || (strlen(fPasswords[memberIdx].pw_dir) == 0))
         fPasswords[memberIdx].pw_dir = strdup("c:\\");
      //
      // Free the allocated memory.
      //
      if (pUI11Buf != NULL) {
         p2NetApiBufferFree(pUI11Buf);
         pUI11Buf = NULL;
      }

      /* Ensure SHELL is defined. */
      if (getenv ("SHELL") == NULL)
         putenv ((GetVersion () & 0x80000000) ? "SHELL=command" : "SHELL=cmd");

      /* Set dir and shell from environment variables. */
      fPasswords[memberIdx].pw_shell = getenv ("SHELL");

      // Find out the SID of the Member.
      LookupSID ((LPCTSTR)szAnsiMemberName, SID_MEMBER, groupIdx, memberIdx);
      memberIdx++;
      MemberInfo++;
   }
   if(fActUser == -1)  fActUser = 0;

   if (Data)
      p2NetApiBufferFree(Data);

   return TRUE;
}

//________________________________________________________________________________
Bool_t TWinNTSystem::CollectGroups()
{
   //

   NET_API_STATUS NetStatus = NERR_Success;
   LPBYTE Data = NULL;
   DWORD Index = 0, ResumeHandle = 0, Total = 0, i;
   LOCALGROUP_INFO_0 *GroupInfo;
   char szAnsiName[256];
   DWORD dwLastError = 0;
   int  iRetOp = 0, iGroupIdx = 0, iMemberIdx = 0;

   NetStatus = p2NetLocalGroupEnum(NULL, 0, &Data, 8192, &Index,
                                    &Total, &ResumeHandle );

   if (NetStatus != NERR_Success || Data == NULL) {
      dwLastError = GetLastError();
      if (Data)
         p2NetApiBufferFree(Data);
      return FALSE;
   }

   GroupInfo = (LOCALGROUP_INFO_0 *)Data;
   for (i=0; i < Total; i++) {
      // Convert group name from UNICODE to ansi.
      iRetOp = WideCharToMultiByte (
               (UINT)CP_ACP,                    // code page
               (DWORD)0,                        // performance and mapping flags
               (LPCWSTR)(GroupInfo->lgrpi0_name), // address of wide-char string
               (int)-1,                        // number of characters in string
               (LPSTR)szAnsiName,            // address of buffer for new string
               (int)(sizeof(szAnsiName)),    // size of buffer
               (LPCSTR)NULL,     // address of default for unmappable characters
               (LPBOOL)NULL );     // address of flag set when default char used.

      fGroups[iGroupIdx].gr_name = strdup(szAnsiName);
      fGroups[iGroupIdx].gr_passwd = strdup("");

      // Find out the SID of the Group.
      LookupSID ((LPCTSTR)szAnsiName, SID_GROUP, iGroupIdx, iMemberIdx);
      // Now lookup all members of this group and record down their names and
      // SIDs into the output file.
      CollectMembers((LPCTSTR)szAnsiName, iGroupIdx, iMemberIdx);

      iGroupIdx++;
      GroupInfo++;
   }

   if (Data)
      p2NetApiBufferFree(Data);

   return TRUE;
}

//______________________________________________________________________________
Int_t TWinNTSystem::GetUid(const char *user)
{
   // Returns the user's id. If user = 0, returns current user's id.

   if(!fGroupsInitDone)
      InitUsersGroups();

   // Net* API functions not allowed or OS not Windows NT/2000/XP
   if ((!gEnv->GetValue("WinNT.UseNetAPI", 0)) || (::GetVersion() >= 0x80000000)) {
      int   uid;
      char  name[256];
      DWORD length = sizeof (name);
      if (::GetUserName (name, &length)) {
         if (stricmp ("administrator", name) == 0)
            uid = 0;
         else
            uid = 123;
      }
      else {
         uid = 123;
      }
      return uid;
   }
   if (!user || !user[0])
      return fPasswords[fActUser].pw_uid;
   else {
      struct passwd *pwd = 0;
      for(int i=0;i<fNbUsers;i++) {
         if (!stricmp (user, fPasswords[i].pw_name)) {
            pwd = &fPasswords[i];
            break;
         }
      }
      if (pwd)
         return pwd->pw_uid;
   }
   return 0;
}

//______________________________________________________________________________
Int_t TWinNTSystem::GetEffectiveUid()
{
   // Returns the effective user id. The effective id corresponds to the
   // set id bit on the file being executed.

   if(!fGroupsInitDone)
      InitUsersGroups();

   // Net* API functions not allowed or OS not Windows NT/2000/XP
   if ((!gEnv->GetValue("WinNT.UseNetAPI", 0)) || (::GetVersion() >= 0x80000000)) {
      int   uid;
      char  name[256];
      DWORD length = sizeof (name);
      if (::GetUserName (name, &length)) {
         if (stricmp ("administrator", name) == 0)
            uid = 0;
         else
            uid = 123;
      }
      else {
         uid = 123;
      }
      return uid;
   }
   return fPasswords[fActUser].pw_uid;
}

//______________________________________________________________________________
Int_t TWinNTSystem::GetGid(const char *group)
{
   // Returns the group's id. If group = 0, returns current user's group.

   if(!fGroupsInitDone)
      InitUsersGroups();

   // Net* API functions not allowed or OS not Windows NT/2000/XP
   if ((!gEnv->GetValue("WinNT.UseNetAPI", 0)) || (::GetVersion() >= 0x80000000)) {
      int   gid;
      char  name[256];
      DWORD length = sizeof (name);
      if (::GetUserName (name, &length)) {
         if (stricmp ("administrator", name) == 0)
            gid = 0;
         else
            gid = 123;
      }
      else {
         gid = 123;
      }
      return gid;
   }
   if (!group || !group[0])
      return fPasswords[fActUser].pw_gid;
   else {
      struct group *grp = 0;
      for(int i=0;i<fNbGroups;i++) {
         if (!stricmp (group, fGroups[i].gr_name)) {
            grp = &fGroups[i];
            break;
         }
      }
      if (grp)
         return grp->gr_gid;
   }
   return 0;
}

//______________________________________________________________________________
Int_t TWinNTSystem::GetEffectiveGid()
{
   // Returns the effective group id. The effective group id corresponds
   // to the set id bit on the file being executed.

   if(!fGroupsInitDone)
      InitUsersGroups();

   // Net* API functions not allowed or OS not Windows NT/2000/XP
   if ((!gEnv->GetValue("WinNT.UseNetAPI", 0)) || (::GetVersion() >= 0x80000000)) {
      int   gid;
      char  name[256];
      DWORD length = sizeof (name);
      if (::GetUserName (name, &length)) {
         if (stricmp ("administrator", name) == 0)
            gid = 0;
         else
            gid = 123;
      }
      else {
         gid = 123;
      }
      return gid;
   }
   return fPasswords[fActUser].pw_gid;
}

//______________________________________________________________________________
UserGroup_t *TWinNTSystem::GetUserInfo(Int_t uid)
{
   // Returns all user info in the UserGroup_t structure. The returned
   // structure must be deleted by the user. In case of error 0 is returned.

   if(!fGroupsInitDone)
      InitUsersGroups();

   // Net* API functions not allowed or OS not Windows NT/2000/XP
   if ((!gEnv->GetValue("WinNT.UseNetAPI", 0)) || (::GetVersion() >= 0x80000000)) {
      char  name[256];
      DWORD length = sizeof (name);
      UserGroup_t *ug = new UserGroup_t;
      if (::GetUserName (name, &length)) {
         ug->fUser = name;
         if (stricmp ("administrator", name) == 0) {
            ug->fUid = 0;
            ug->fGroup = "administrators";
         }
         else {
            ug->fUid = 123;
            ug->fGroup = "users";
         }
         ug->fGid = ug->fUid;
      }
      else {
         ug->fUser = "unknown";
         ug->fGroup = "unknown";
         ug->fUid = ug->fGid = 123;
      }
      ug->fPasswd = "";
      ug->fRealName = ug->fUser;
      ug->fShell = "command";
      return ug;
   }
   struct passwd *pwd = 0;
   if (uid == 0)
      pwd = &fPasswords[fActUser];
   else {
      for (int i = 0; i < fNbUsers; i++) {
         if (uid == fPasswords[i].pw_uid) {
            pwd = &fPasswords[i];
            break;
         }
      }
   }
   if (pwd) {
      UserGroup_t *ug = new UserGroup_t;
      ug->fUid      = pwd->pw_uid;
      ug->fGid      = pwd->pw_gid;
      ug->fUser     = pwd->pw_name;
      ug->fPasswd   = pwd->pw_passwd;
      ug->fRealName = pwd->pw_gecos;
      ug->fShell    = pwd->pw_shell;
      ug->fGroup    = pwd->pw_group;
      return ug;
   }
   return 0;
}

//______________________________________________________________________________
UserGroup_t *TWinNTSystem::GetUserInfo(const char *user)
{
   // Returns all user info in the UserGroup_t structure. If user = 0, returns
   // current user's id info. The returned structure must be deleted by the
   // user. In case of error 0 is returned.

   return GetUserInfo(GetUid(user));
}

//______________________________________________________________________________
UserGroup_t *TWinNTSystem::GetGroupInfo(Int_t gid)
{
   // Returns all group info in the UserGroup_t structure. The only active
   // fields in the UserGroup_t structure for this call are:
   //    fGid and fGroup
   // The returned structure must be deleted by the user. In case of
   // error 0 is returned.

   if(!fGroupsInitDone)
      InitUsersGroups();

   // Net* API functions not allowed or OS not Windows NT/2000/XP
   if ((!gEnv->GetValue("WinNT.UseNetAPI", 0)) || (::GetVersion() >= 0x80000000)) {
      char  name[256];
      DWORD length = sizeof (name);
      UserGroup_t *gr = new UserGroup_t;
      if (::GetUserName (name, &length)) {
         if (stricmp ("administrator", name) == 0) {
            gr->fGroup = "administrators";
            gr->fGid = 0;
         }
         else {
            gr->fGroup = "users";
            gr->fGid = 123;
         }
      }
      else {
         gr->fGroup = "unknown";
         gr->fGid = 123;
      }
      gr->fUid = 0;
      return gr;
   }
   struct group *grp = 0;
   for(int i=0;i<fNbGroups;i++) {
      if (gid == fGroups[i].gr_gid) {
         grp = &fGroups[i];
         break;
      }
   }
   if (grp) {
      UserGroup_t *gr = new UserGroup_t;
      gr->fUid   = 0;
      gr->fGid   = grp->gr_gid;
      gr->fGroup = grp->gr_name;
      return gr;
   }
   return 0;

}

//______________________________________________________________________________
UserGroup_t *TWinNTSystem::GetGroupInfo(const char *group)
{
   // Returns all group info in the UserGroup_t structure. The only active
   // fields in the UserGroup_t structure for this call are:
   //    fGid and fGroup
   // If group = 0, returns current user's group. The returned structure
   // must be deleted by the user. In case of error 0 is returned.

   return GetGroupInfo(GetGid(group));
}

//---- environment manipulation ------------------------------------------------

//______________________________________________________________________________
void TWinNTSystem::Setenv(const char *name, const char *value)
{
   // Set environment variable.

   ::_putenv(Form("%s=%s", name, value));
}

//______________________________________________________________________________
const char *TWinNTSystem::Getenv(const char *name)
{
   // Get environment variable.

   const char *env = ::getenv(name);
   if (!env) {
      if (::_stricmp(name,"home") == 0 ) {
        env = HomeDirectory();
      } else if (::_stricmp(name, "rootsys") == 0 ) {
        env = gRootDir;
      }
   }
   return env;
}

//---- Processes ---------------------------------------------------------------

//______________________________________________________________________________
int TWinNTSystem::Exec(const char *shellcmd)
{
   // Execute a command.

   return ::system(shellcmd);
}

//______________________________________________________________________________
FILE *TWinNTSystem::OpenPipe(const char *command, const char *mode)
{
   // Open a pipe.

  return ::_popen(command, mode);
}

//______________________________________________________________________________
int TWinNTSystem::ClosePipe(FILE *pipe)
{
   // Close the pipe.

  return ::_pclose(pipe);
}

//______________________________________________________________________________
int TWinNTSystem::GetPid()
{
   // Get process id.

   return ::getpid();
}

//______________________________________________________________________________
HANDLE TWinNTSystem::GetProcess()
{
  // Get current process handle

  return fhProcess;
}

//______________________________________________________________________________
void TWinNTSystem::Exit(int code, Bool_t mode)
{
   // Exit the application.

   gVirtualX->CloseDisplay();

   // Insures that the files and sockets are closed before any library is unloaded!
   if (gROOT) {
      if (gROOT->GetListOfFiles()) gROOT->GetListOfFiles()->Delete("slow");
      if (gROOT->GetListOfSockets()) gROOT->GetListOfSockets()->Delete();
      if (gROOT->GetListOfMappedFiles()) gROOT->GetListOfMappedFiles()->Delete("slow");
   }

   if (mode) {
      ::exit(code);
   } else {
      ::_exit(code);
   }
}

//______________________________________________________________________________
void TWinNTSystem::Abort(int)
{
   // Abort the application.

   ::abort();
}

//---- dynamic loading and linking ---------------------------------------------

//______________________________________________________________________________
const char* TWinNTSystem::GetDynamicPath()
{
   // Return the dynamic path (used to find shared libraries).

   return DynamicPath(0, kFALSE);
}

//______________________________________________________________________________
void TWinNTSystem::SetDynamicPath(const char *path)
{
   // Set the dynamic path to a new value.
   // If the value of 'path' is zero, the dynamic path is reset to its
   // default value.

   if (!path)
      DynamicPath(0, kTRUE);
   else
      DynamicPath(path);
}

//______________________________________________________________________________
char *TWinNTSystem::DynamicPathName(const char *lib, Bool_t quiet)
{
   // Returns the path of a dynamic library (searches for library in the
   // dynamic library search path). If no file name extension is provided
   // it tries .DLL. Returned string must be deleted.

   char *name;

   int len = strlen(lib);
   if (len > 4 && (!stricmp(lib+len-4, ".dll"))) {
      name = gSystem->Which(GetDynamicPath(), lib, kReadPermission);
   } else {
      name = Form("%s.dll", lib);
      name = gSystem->Which(GetDynamicPath(), name, kReadPermission);
   }

   if (!name && !quiet) {
      Error("DynamicPathName",
            "%s does not exist in %s,\nor has wrong file extension (.dll)", lib,
            GetDynamicPath());
   }
   return name;
}

//______________________________________________________________________________
const char *TWinNTSystem::GetLinkedLibraries()
{
   // Get list of shared libraries loaded at the start of the executable.
   // Returns 0 in case list cannot be obtained or in case of error.
   char winPath[256];
   char winDrive[256];
   char winDir[256];
   char winName[256];
   char winExt[256];

   if (!gApplication) return 0;

   static Bool_t once = kFALSE;
   static TString linkedLibs;

   if (!linkedLibs.IsNull())
      return linkedLibs;

   if (once)
      return 0;

   char *exe = gSystem->Which(Getenv("PATH"), gApplication->Argv(0),
                              kExecutePermission);
   if (!exe) {
      once = kTRUE;
      return 0;
   }

   HANDLE hFile, hMapping;
   void *basepointer;

   if((hFile = CreateFile(exe,GENERIC_READ,0,0,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,0))==INVALID_HANDLE_VALUE) {
      return 0;
   }
   if(!(hMapping = CreateFileMapping(hFile,0,PAGE_READONLY|SEC_COMMIT,0,0,0))) {
      CloseHandle(hFile);
      return 0;
   }
   if(!(basepointer = MapViewOfFile(hMapping,FILE_MAP_READ,0,0,0))) {
      CloseHandle(hMapping);
      CloseHandle(hFile);
      return 0;
   }

   int sect;
   IMAGE_DOS_HEADER *dos_head = (IMAGE_DOS_HEADER *)basepointer;
   struct header {
      DWORD signature;
      IMAGE_FILE_HEADER _head;
      IMAGE_OPTIONAL_HEADER opt_head;
      IMAGE_SECTION_HEADER section_header[];  // actual number in NumberOfSections
   };
   struct header *pheader;
   const IMAGE_SECTION_HEADER * section_header;

   if(dos_head->e_magic!='ZM') {
      return 0;
   }  // verify DOS-EXE-Header
   // after end of DOS-EXE-Header: offset to PE-Header
   pheader = (struct header *)((char*)dos_head + dos_head->e_lfanew);

   if(IsBadReadPtr(pheader,sizeof(struct header))) { // start of PE-Header
      return 0;
   }
   if(pheader->signature!=IMAGE_NT_SIGNATURE) {      // verify PE format
      switch((unsigned short)pheader->signature) {
         case IMAGE_DOS_SIGNATURE:
            return 0;
         case IMAGE_OS2_SIGNATURE:
            return 0;
         case IMAGE_OS2_SIGNATURE_LE:
            return 0;
         default: // unknown signature
            return 0;
      }
   }
#define isin(address,start,length) ((address)>=(start) && (address)<(start)+(length))
   TString odump;
   // walk through sections
   for(sect=0,section_header=pheader->section_header;
       sect<pheader->_head.NumberOfSections;sect++,section_header++) {
      int directory;
      const void * const section_data =
            (char*)basepointer + section_header->PointerToRawData;
      for(directory=0;directory<IMAGE_NUMBEROF_DIRECTORY_ENTRIES;directory++) {
         if(isin(pheader->opt_head.DataDirectory[directory].VirtualAddress,
                 section_header->VirtualAddress,
                 section_header->SizeOfRawData)) {
            const IMAGE_IMPORT_DESCRIPTOR *stuff_start =
                 (IMAGE_IMPORT_DESCRIPTOR *)((char*)section_data +
                 (pheader->opt_head.DataDirectory[directory].VirtualAddress -
                  section_header->VirtualAddress));
            // (virtual address of stuff - virtual address of section) =
            // offset of stuff in section
            const unsigned stuff_length =
                  pheader->opt_head.DataDirectory[directory].Size;
            if(directory == IMAGE_DIRECTORY_ENTRY_IMPORT) {
               while(!IsBadReadPtr(stuff_start,sizeof(*stuff_start)) &&
                      stuff_start->Name) {
                  TString dll = (char*)section_data +
                               ((DWORD)(stuff_start->Name)) -
                                section_header->VirtualAddress;
                  if (dll.EndsWith(".dll")) {
                     char *dllPath = DynamicPathName(dll, kTRUE);
                     if (dllPath) {
                        char *winPath = getenv("windir");
                        _splitpath(winPath,winDrive,winDir,winName,winExt);
                        if(!strstr(dllPath, winDir)) {
                           if (!linkedLibs.IsNull())
                              linkedLibs += " ";
                           linkedLibs += dllPath;
                        }
                     }
                     delete [] dllPath;
                  }
                  stuff_start++;
               }
            }
         }
      }
   }

   UnmapViewOfFile(basepointer);
   CloseHandle(hMapping);
   CloseHandle(hFile);

   delete [] exe;

   once = kTRUE;

   if (linkedLibs.IsNull())
      return 0;

   return linkedLibs;
}


//______________________________________________________________________________
const char *TWinNTSystem::GetLibraries(const char *regexp, const char *options,
                                       Bool_t isRegexp)
{
   // Return a space separated list of loaded shared libraries.
   // This list is of a format suitable for a linker, i.e it may contain
   // -Lpathname and/or -lNameOfLib.
   // Option can be any of:
   //   S: shared libraries loaded at the start of the executable, because
   //      they were specified on the link line.
   //   D: shared libraries dynamically loaded after the start of the program.
   //   L: list the .LIB rather than the .DLL (this is intended for linking)
   //      [This options is not the default]

   TString libs(TSystem::GetLibraries(regexp, options, isRegexp));
   TString ntlibs;
   TString opt = options;

   if ( (opt.First('L')!=kNPOS) ) {
      TRegexp separator("[^ \\t\\s]+");
      TRegexp user_dll("\\.dll$");
      TRegexp user_lib("\\.lib$");
      FileStat_t sbuf;
      TString s;
      Ssiz_t start, index, end;
      start = index = end = 0;

      while ((start < libs.Length()) && (index != kNPOS)) {
         index = libs.Index(separator, &end, start);
         if (index >= 0) {
            // Change .dll into .lib and remove the
            // path info if it not accessible.
            s = libs(index, end);
            if (s.Index(user_dll) != kNPOS) {
               s.ReplaceAll(".dll",".lib");
               if ( GetPathInfo( s, sbuf ) != 0 ) {
                  s.Replace( 0, s.Last('/')+1, 0, 0);
                  s.Replace( 0, s.Last('\\')+1, 0, 0);
               }
            } else if (s.Index(user_lib) != kNPOS) {
               if ( GetPathInfo( s, sbuf ) != 0 ) {
                  s.Replace( 0, s.Last('/')+1, 0, 0);
                  s.Replace( 0, s.Last('\\')+1, 0, 0);
               }
            }
            if (!ntlibs.IsNull()) ntlibs.Append(" ");
            ntlibs.Append(s);
         }
         start += end+1;
      }
   } else {
      ntlibs = libs;
   }

   fListLibs = ntlibs;
   fListLibs.ReplaceAll("/","\\");
   return fListLibs;
}

//---- Time & Date -------------------------------------------------------------

//______________________________________________________________________________
void TWinNTSystem::AddTimer(TTimer *ti)
{
   //

#ifndef GDK_WIN32
   if (ti) {
      TSystem::AddTimer(ti);

      if (!fWin32Timer) fWin32Timer = new TWin32Timer;
      fWin32Timer->CreateTimer(ti);
      if (!ti->GetTimerID()) {
         RemoveTimer(ti);
      }
   }
#else
   TSystem::AddTimer(ti);
   if (!fInsideNotify && ti->IsAsync()) {
      WinNTSetitimer(NextTimeOut(kFALSE));
   }
#endif
}

//______________________________________________________________________________
TTimer *TWinNTSystem::RemoveTimer(TTimer *ti)
{
   //

#ifndef GDK_WIN32
   if (ti && fWin32Timer ) {
      fWin32Timer->KillTimer(ti);
      return TSystem::RemoveTimer(ti);
   }
   return 0;
#else
   TTimer *t = TSystem::RemoveTimer(ti);
   if (ti->IsAsync()) {
      WinNTSetitimer(NextTimeOut(kFALSE));
   }
   return t;
#endif
}

//______________________________________________________________________________
Bool_t TWinNTSystem::DispatchTimers(Bool_t mode)
{
   // Handle and dispatch timers. If mode = kTRUE dispatch synchronous
   // timers else a-synchronous timers.

#ifndef GDK_WIN32
   return kFALSE;
#else
   if (!fTimers) return kFALSE;

   fInsideNotify = kTRUE;

   TOrdCollectionIter it((TOrdCollection*)fTimers);
   TTimer *t;
   Bool_t  timedout = kFALSE;

   while ((t = (TTimer *) it.Next())) {
      TTime now = Now();
      now += TTime(kItimerResolution);
      if (mode && t->IsSync()) {
         if (t->CheckTimer(now)) {
            timedout = kTRUE;
         }
      } else if (!mode && t->IsAsync()) {
         if (t->CheckTimer(now)) {
            WinNTSetitimer(NextTimeOut(kFALSE));
            timedout = kTRUE;
         }
      }
   }
   fInsideNotify = kFALSE;
   return timedout;
#endif
}

//______________________________________________________________________________
Bool_t TWinNTSystem::DispatchSynchTimers()
{
   // Handle and dispatch timers. If mode = kTRUE dispatch synchronous
   // timers else a-synchronous timers.

   if (!fTimers) return kFALSE;

   fInsideNotify = kTRUE;

   TOrdCollectionIter it((TOrdCollection*)fTimers);
   TTimer *t;
   Bool_t  timedout = kFALSE;

   while ((t = (TTimer *) it.Next())) {
      if (t->IsSync()) {
         TTime now = Now();
         now += TTime(kItimerResolution);
         if (t->CheckTimer(now)) timedout = kTRUE;
      }
   }
   fInsideNotify = kFALSE;
   return timedout;
}

const Double_t gTicks = 1.0e-7;
//______________________________________________________________________________
Double_t TWinNTSystem::GetRealTime()
{
   //

   union {
      FILETIME ftFileTime;
      __int64  ftInt64;
   } ftRealTime; // time the process has spent in kernel mode

   SYSTEMTIME st;
   ::GetSystemTime(&st);
   ::SystemTimeToFileTime(&st, &ftRealTime.ftFileTime);

   return (Double_t)ftRealTime.ftInt64 * gTicks;
}

//______________________________________________________________________________
Double_t TWinNTSystem::GetCPUTime()
{
   //

   OSVERSIONINFO OsVersionInfo;

//*-*         Value                      Platform
//*-*  ----------------------------------------------------
//*-*  VER_PLATFORM_WIN32s              Win32s on Windows 3.1
//*-*  VER_PLATFORM_WIN32_WINDOWS       Win32 on Windows 95
//*-*  VER_PLATFORM_WIN32_NT            Windows NT
//*-*

   OsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
   GetVersionEx(&OsVersionInfo);
   if (OsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) {
      DWORD       ret;
      FILETIME    ftCreate,       // when the process was created
                  ftExit;         // when the process exited

      union {
         FILETIME ftFileTime;
         __int64  ftInt64;
      } ftKernel; // time the process has spent in kernel mode

      union {
         FILETIME ftFileTime;
         __int64  ftInt64;
      } ftUser;   // time the process has spent in user mode

      HANDLE hProcess = ::GetCurrentProcess();
      ret = ::GetProcessTimes(hProcess, &ftCreate, &ftExit,
                              &ftKernel.ftFileTime, &ftUser.ftFileTime);
      if (ret != TRUE){
         ret = ::GetLastError();
         ::Error("GetCPUTime", " Error on GetProcessTimes 0x%lx", (int)ret);
      }

      // Process times are returned in a 64-bit structure, as the number of
      // 100 nanosecond ticks since 1 January 1601.  User mode and kernel mode
      // times for this process are in separate 64-bit structures.
      // To convert to floating point seconds, we will:
      //          Convert sum of high 32-bit quantities to 64-bit int

       return (Double_t) (ftKernel.ftInt64 + ftUser.ftInt64) * gTicks;
   } else {
      return GetRealTime();
   }
}

//______________________________________________________________________________
TTime TWinNTSystem::Now()
{
   // Return current time.

   return Long_t(GetRealTime()*1000.0);
}

//______________________________________________________________________________
void TWinNTSystem::Sleep(UInt_t milliSec)
{
   // Sleep milliSec milli seconds.
   // The Sleep function suspends the execution of the CURRENT THREAD for
   // a specified interval.

   ::Sleep(milliSec);
}

//______________________________________________________________________________
Int_t TWinNTSystem::Select(TList *act, Long_t to)
{
   // Select on file descriptors. The timeout to is in millisec.
   Int_t rc = -4;

   TFdSet rd, wr;
   Int_t mxfd = -1;
   TIter next(act);
   TFileHandler *h = 0;
   while ((h = (TFileHandler *) next())) {
      Int_t fd = h->GetFd();
      if (h->HasReadInterest())
         rd.Set(fd);
      if (h->HasWriteInterest())
         wr.Set(fd);
      h->ResetReadyMask();
   }
   rc = WinNTSelect(&rd, &wr, to);

   // Set readiness bits
   if (rc > 0) {
      next.Reset();
      while ((h = (TFileHandler *) next())) {
         Int_t fd = h->GetFd();
         if (rd.IsSet(fd))
            h->SetReadReady();
         if (wr.IsSet(fd))
            h->SetWriteReady();
      }
   }

   return rc;
}

//______________________________________________________________________________
Int_t TWinNTSystem::Select(TFileHandler *h, Long_t to)
{
   // Select on the file descriptor related to file handler h.
   // The timeout to is in millisec.
   Int_t rc = -4;

   TFdSet rd, wr;
   Int_t fd = -1;
   if (h) {
      fd = h->GetFd();
      if (h->HasReadInterest())
         rd.Set(fd);
      if (h->HasWriteInterest())
         wr.Set(fd);
      h->ResetReadyMask();
      rc = WinNTSelect(&rd, &wr, to);
   }

   // Fill output lists, if required
   if (rc > 0) {
      if (rd.IsSet(fd))
         h->SetReadReady();
      if (wr.IsSet(fd))
         h->SetWriteReady();
   }

   return rc;
}

//---- RPC ---------------------------------------------------------------------
//______________________________________________________________________________
int TWinNTSystem::GetServiceByName(const char *servicename)
{
   // Get port # of internet service.

   struct servent *sp;

   if ((sp = ::getservbyname(servicename, kProtocolName)) == 0) {
      Error("GetServiceByName", "no service \"%s\" with protocol \"%s\"\n",
             servicename, kProtocolName);
      return -1;
   }
   return ::ntohs(sp->s_port);
}

//______________________________________________________________________________
char *TWinNTSystem::GetServiceByPort(int port)
{

   // Get name of internet service.

   struct servent *sp;

   if ((sp = ::getservbyport(::htons(port), kProtocolName)) == 0) {
      return Form("%d", port);
   }
   return sp->s_name;
}

//______________________________________________________________________________
TInetAddress TWinNTSystem::GetHostByName(const char *hostname)
{
   // Get Internet Protocol (IP) address of host.

   struct hostent *host_ptr;
   struct in_addr  ad;
   const char     *host;
   int             type;
   UInt_t          addr;    // good for 4 byte addresses

   if ((addr = ::inet_addr(hostname)) != INADDR_NONE) {
      type = AF_INET;
      if ((host_ptr = ::gethostbyaddr((const char *)&addr,
                                      sizeof(addr), AF_INET))) {
         host = host_ptr->h_name;
         TInetAddress a(host, ntohl(addr), type);
         UInt_t addr2;
         Int_t  i;
         for (i = 1; host_ptr->h_addr_list[i]; i++) {
            memcpy(&addr2, host_ptr->h_addr_list[i], host_ptr->h_length);
            a.AddAddress(ntohl(addr2));
         }
         for (i = 0; host_ptr->h_aliases[i]; i++)
            a.AddAlias(host_ptr->h_aliases[i]);
         return a;
      } else {
         host = "UnNamedHost";
      }
   } else if ((host_ptr = ::gethostbyname(hostname))) {
      // Check the address type for an internet host
      if (host_ptr->h_addrtype != AF_INET) {
         Error("GetHostByName", "%s is not an internet host\n", hostname);
         return TInetAddress();
      }
      memcpy(&addr, host_ptr->h_addr, host_ptr->h_length);
      host = host_ptr->h_name;
      type = host_ptr->h_addrtype;
      TInetAddress a(host, ntohl(addr), type);
      UInt_t addr2;
      Int_t  i;
      for (i = 1; host_ptr->h_addr_list[i]; i++) {
         memcpy(&addr2, host_ptr->h_addr_list[i], host_ptr->h_length);
         a.AddAddress(ntohl(addr2));
      }
      for (i = 0; host_ptr->h_aliases[i]; i++)
         a.AddAlias(host_ptr->h_aliases[i]);
      return a;
   } else {
      if (gDebug > 0) Error("GetHostByName", "unknown host %s", hostname);
      return TInetAddress(hostname, 0, -1);
   }

   return TInetAddress(host, ::ntohl(addr), type);
}

//______________________________________________________________________________
TInetAddress TWinNTSystem::GetPeerName(int socket)
{
   // Get Internet Protocol (IP) address of remote host and port #.

   SOCKET sock = socket;
   struct sockaddr_in addr;
   int len = sizeof(addr);

   if (::getpeername(sock, (struct sockaddr *)&addr, &len) == SOCKET_ERROR) {
      ::SysError("GetPeerName", "getpeername");
      return TInetAddress();
   }

   struct hostent *host_ptr;
   const char *hostname;
   int         family;
   UInt_t      iaddr;

   if ((host_ptr = ::gethostbyaddr((const char *)&addr.sin_addr,
                                   sizeof(addr.sin_addr), AF_INET))) {
      memcpy(&iaddr, host_ptr->h_addr, host_ptr->h_length);
      hostname = host_ptr->h_name;
      family   = host_ptr->h_addrtype;
   } else {
      memcpy(&iaddr, &addr.sin_addr, sizeof(addr.sin_addr));
      hostname = "????";
      family   = AF_INET;
   }

   return TInetAddress(hostname, ::ntohl(iaddr), family, ::ntohs(addr.sin_port));
}

//______________________________________________________________________________
TInetAddress TWinNTSystem::GetSockName(int socket)
{
   // Get Internet Protocol (IP) address of host and port #.

   SOCKET sock = socket;
   struct sockaddr_in addr;
   int len = sizeof(addr);

   if (::getsockname(sock, (struct sockaddr *)&addr, &len) == SOCKET_ERROR) {
      ::SysError("GetSockName", "getsockname");
      return TInetAddress();
   }

   struct hostent *host_ptr;
   const char *hostname;
   int         family;
   UInt_t      iaddr;

   if ((host_ptr = ::gethostbyaddr((const char *)&addr.sin_addr,
                                   sizeof(addr.sin_addr), AF_INET))) {
      memcpy(&iaddr, host_ptr->h_addr, host_ptr->h_length);
      hostname = host_ptr->h_name;
      family   = host_ptr->h_addrtype;
   } else {
      memcpy(&iaddr, &addr.sin_addr, sizeof(addr.sin_addr));
      hostname = "????";
      family   = AF_INET;
   }

   return TInetAddress(hostname, ::ntohl(iaddr), family, ::ntohs(addr.sin_port));
}

//______________________________________________________________________________
int TWinNTSystem::AnnounceUnixService(int port, int backlog)
{
   // Announce unix domain service.

   SOCKET sock;

   // Create socket
   if ((sock = ::socket(AF_UNIX, SOCK_STREAM, 0)) == INVALID_SOCKET) {
      ::SysError("TWinNTSystem::AnnounceUnixService", "socket");
      return -1;
   }

   // Start accepting connections
   if (::listen(sock, backlog)) {
      ::SysError("TWinNTSystem::AnnounceUnixService", "listen");
      return -1;
   }
   return (int)sock;
}

//______________________________________________________________________________
void TWinNTSystem::CloseConnection(int socket, Bool_t force)
{
   // Close socket.

   if (socket == -1) return;
   SOCKET sock = socket;

   if (force) {
      ::shutdown(sock, 2);
   }
   while (::closesocket(sock) == SOCKET_ERROR && WSAGetLastError() == WSAEINTR) {
      TSystem::ResetErrno();
   }
}

//______________________________________________________________________________
int TWinNTSystem::RecvBuf(int sock, void *buf, int length)
{
   // Receive a buffer headed by a length indicator. Lenght is the size of
   // the buffer. Returns the number of bytes received in buf or -1 in
   // case of error.

   Int_t header;

   if (WinNTRecv(sock, &header, sizeof(header), 0) > 0) {
      int count = ::ntohl(header);

      if (count > length) {
         Error("RecvBuf", "record header exceeds buffer size");
         return -1;
      } else if (count > 0) {
         if (WinNTRecv(sock, buf, count, 0) < 0) {
            Error("RecvBuf", "cannot receive buffer");
            return -1;
         }
      }
      return count;
   }
   return -1;
}

//______________________________________________________________________________
int TWinNTSystem::SendBuf(int sock, const void *buf, int length)
{
   // Send a buffer headed by a length indicator. Returns length of sent buffer
   // or -1 in case of error.

   Int_t header = ::htonl(length);

   if (WinNTSend(sock, &header, sizeof(header), 0) < 0) {
      Error("SendBuf", "cannot send header");
      return -1;
   }
   if (length > 0) {
      if (WinNTSend(sock, buf, length, 0) < 0) {
         Error("SendBuf", "cannot send buffer");
         return -1;
      }
   }
   return length;
}

//______________________________________________________________________________
int TWinNTSystem::RecvRaw(int sock, void *buf, int length, int opt)
{
   // Receive exactly length bytes into buffer. Use opt to receive out-of-band
   // data or to have a peek at what is in the buffer (see TSocket). Buffer
   // must be able to store at least lenght bytes. Returns the number of
   // bytes received (can be 0 if other side of connection was closed) or -1
   // in case of error, -2 in case of MSG_OOB and errno == EWOULDBLOCK, -3
   // in case of MSG_OOB and errno == EINVAL and -4 in case of kNoBlock and
   // errno == EWOULDBLOCK. Returns -5 if pipe broken or reset by peer
   // (EPIPE || ECONNRESET).

   int flag;

   switch (opt) {
   case kDefault:
      flag = 0;
      break;
   case kOob:
      flag = MSG_OOB;
      break;
   case kPeek:
      flag = MSG_PEEK;
      break;
   case kDontBlock:
      flag = -1;
      break;
   default:
      flag = 0;
      break;
   }

   int n;
   if ((n = WinNTRecv(sock, buf, length, flag)) <= 0) {
      if (n == -1) {
         Error("RecvRaw", "cannot receive buffer");
      }
      return n;
   }
   return n;
}

//______________________________________________________________________________
int TWinNTSystem::SendRaw(int sock, const void *buf, int length, int opt)
{
   // Send exactly length bytes from buffer. Use opt to send out-of-band
   // data (see TSocket). Returns the number of bytes sent or -1 in case of
   // error. Returns -4 in case of kNoBlock and errno == EWOULDBLOCK.
   // Returns -5 if pipe broken or reset by peer (EPIPE || ECONNRESET).

   int flag;

   switch (opt) {
   case kDefault:
      flag = 0;
      break;
   case kOob:
      flag = MSG_OOB;
      break;
   case kDontBlock:
      flag = -1;
      break;
   case kPeek:            // receive only option (see RecvRaw)
   default:
      flag = 0;
      break;
   }

   int n;
   if ((n = WinNTSend(sock, buf, length, flag)) <= 0) {
      if (n == -1 && GetErrno() != EINTR) {
         Error("SendRaw", "cannot send buffer");
      }
      return n;
   }
   return n;
}

//______________________________________________________________________________
int  TWinNTSystem::SetSockOpt(int socket, int opt, int value)
{
   // Set socket option.

   u_long val = value;
   if (socket == -1) return -1;
   SOCKET sock = socket;

   switch (opt) {
   case kSendBuffer:
      if (::setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&val, sizeof(val)) == SOCKET_ERROR) {
         ::SysError("SetSockOpt", "setsockopt(SO_SNDBUF)");
         return -1;
      }
      break;
   case kRecvBuffer:
      if (::setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&val, sizeof(val)) == SOCKET_ERROR) {
         ::SysError("SetSockOpt", "setsockopt(SO_RCVBUF)");
         return -1;
      }
      break;
   case kOobInline:
      if (::setsockopt(sock, SOL_SOCKET, SO_OOBINLINE, (char*)&val, sizeof(val)) == SOCKET_ERROR) {
         SysError("SetSockOpt", "setsockopt(SO_OOBINLINE)");
         return -1;
      }
      break;
   case kKeepAlive:
      if (::setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&val, sizeof(val)) == SOCKET_ERROR) {
         ::SysError("SetSockOpt", "setsockopt(SO_KEEPALIVE)");
         return -1;
      }
      break;
   case kReuseAddr:
      if (::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&val, sizeof(val)) == SOCKET_ERROR) {
         ::SysError("SetSockOpt", "setsockopt(SO_REUSEADDR)");
         return -1;
      }
      break;
   case kNoDelay:
      if (::setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&val, sizeof(val)) == SOCKET_ERROR) {
         ::SysError("SetSockOpt", "setsockopt(TCP_NODELAY)");
         return -1;
      }
      break;
   case kNoBlock:
      if (::ioctlsocket(sock, FIONBIO, &val) == SOCKET_ERROR) {
         ::SysError("SetSockOpt", "ioctl(FIONBIO)");
         return -1;
      }
      break;
#if 0
   case kProcessGroup:
      if (::ioctl(sock, SIOCSPGRP, &val) == -1) {
         ::SysError("SetSockOpt", "ioctl(SIOCSPGRP)");
         return -1;
      }
      break;
#endif
   kAtMark:       // read-only option (see GetSockOpt)
   kBytesToRead:  // read-only option
   default:
      Error("SetSockOpt", "illegal option (%d)", opt);
      return -1;
      break;
   }
   return 0;
}

//______________________________________________________________________________
int TWinNTSystem::GetSockOpt(int socket, int opt, int *val)
{
   // Get socket option.

   if (socket == -1) return -1;
   SOCKET sock = socket;

   int optlen = sizeof(*val);

   switch (opt) {
   case kSendBuffer:
      if (::getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)val, &optlen) == SOCKET_ERROR) {
         ::SysError("GetSockOpt", "getsockopt(SO_SNDBUF)");
         return -1;
      }
      break;
   case kRecvBuffer:
      if (::getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)val, &optlen) == SOCKET_ERROR) {
         ::SysError("GetSockOpt", "getsockopt(SO_RCVBUF)");
         return -1;
      }
      break;
   case kOobInline:
      if (::getsockopt(sock, SOL_SOCKET, SO_OOBINLINE, (char*)val, &optlen) == SOCKET_ERROR) {
         ::SysError("GetSockOpt", "getsockopt(SO_OOBINLINE)");
         return -1;
      }
      break;
   case kKeepAlive:
      if (::getsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char*)val, &optlen) == SOCKET_ERROR) {
         ::SysError("GetSockOpt", "getsockopt(SO_KEEPALIVE)");
         return -1;
      }
      break;
   case kReuseAddr:
      if (::getsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)val, &optlen) == SOCKET_ERROR) {
         ::SysError("GetSockOpt", "getsockopt(SO_REUSEADDR)");
         return -1;
      }
      break;
   case kNoDelay:
      if (::getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)val, &optlen) == SOCKET_ERROR) {
         ::SysError("GetSockOpt", "getsockopt(TCP_NODELAY)");
         return -1;
      }
      break;
   case kNoBlock:
      {
         int flg = 0;
         if (sock == INVALID_SOCKET) {
            ::SysError("GetSockOpt", "INVALID_SOCKET");
         }
         return -1;
         *val = flg; //  & O_NDELAY;  It is not been defined for WIN32
      }
      break;
#if 0
   case kProcessGroup:
      if (::ioctlsocket(sock, SIOCGPGRP, (u_long*)val) == SOCKET_ERROR) {
         ::SysError("GetSockOpt", "ioctl(SIOCGPGRP)");
         return -1;
      }
      break;
#endif
   case kAtMark:
      if (::ioctlsocket(sock, SIOCATMARK, (u_long*)val) == SOCKET_ERROR) {
         ::SysError("GetSockOpt", "ioctl(SIOCATMARK)");
         return -1;
      }
      break;
   case kBytesToRead:
      if (::ioctlsocket(sock, FIONREAD, (u_long*)val) == SOCKET_ERROR) {
         ::SysError("GetSockOpt", "ioctl(FIONREAD)");
         return -1;
      }
      break;
   default:
      Error("GetSockOpt", "illegal option (%d)", opt);
      *val = 0;
      return -1;
      break;
   }
   return 0;
}

//______________________________________________________________________________
int TWinNTSystem::ConnectService(const char *servername, int port,
                                 int tcpwindowsize)
{
   // Connect to service servicename on server servername.

   short  sport;
   struct servent *sp;

   if (!strcmp(servername, "unix")) {
      printf(" Error don't know how to do UnixUnixConnect under WIN32 \n");
      return -1;
   }

   if ((sp = ::getservbyport(::htons(port), kProtocolName))) {
      sport = sp->s_port;
   } else {
      sport = ::htons(port);
   }

   TInetAddress addr = gSystem->GetHostByName(servername);
   if (!addr.IsValid()) return -1;
   UInt_t adr = ::htonl(addr.GetAddress());

   struct sockaddr_in server;
   memset(&server, 0, sizeof(server));
   memcpy(&server.sin_addr, &adr, sizeof(adr));
   server.sin_family = addr.GetFamily();
   server.sin_port   = sport;

   // Create socket
   SOCKET sock;
   if ((sock = ::socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
      ::SysError("TWinNTSystem::WinNTConnectTcp", "socket");
      return -1;
   }

   if (tcpwindowsize > 0) {
      gSystem->SetSockOpt((int)sock, kRecvBuffer, tcpwindowsize);
      gSystem->SetSockOpt((int)sock, kSendBuffer, tcpwindowsize);
   }

   if (::connect(sock, (struct sockaddr*) &server, sizeof(server)) == INVALID_SOCKET) {
      //::SysError("TWinNTSystem::UnixConnectTcp", "connect");
      ::closesocket(sock);
      return -1;
   }
   return (int) sock;
}

//______________________________________________________________________________
int TWinNTSystem::OpenConnection(const char *server, int port, int tcpwindowsize)
{
   // Open a connection to a service on a server. Returns -1 in case
   // connection cannot be opened.
   // Use tcpwindowsize to specify the size of the receive buffer, it has
   // to be specified here to make sure the window scale option is set (for
   // tcpwindowsize > 65KB and for platforms supporting window scaling).
   // Is called via the TSocket constructor.

   return ConnectService(server, port, tcpwindowsize);
}

//______________________________________________________________________________
int TWinNTSystem::AnnounceTcpService(int port, Bool_t reuse, int backlog,
                                     int tcpwindowsize)
{
   // Announce TCP/IP service.
   // Open a socket, bind to it and start listening for TCP/IP connections
   // on the port. If reuse is true reuse the address, backlog specifies
   // how many sockets can be waiting to be accepted.
   // Use tcpwindowsize to specify the size of the receive buffer, it has
   // to be specified here to make sure the window scale option is set (for
   // tcpwindowsize > 65KB and for platforms supporting window scaling).
   // Returns socket fd or -1 if socket() failed, -2 if bind() failed
   // or -3 if listen() failed.

   short  sport;
   struct servent *sp;
   const short kSOCKET_MINPORT = 5000, kSOCKET_MAXPORT = 15000;
   short tryport = kSOCKET_MINPORT;

   if ((sp = ::getservbyport(::htons(port), kProtocolName))) {
      sport = sp->s_port;
   } else {
      sport = ::htons(port);
   }

   if (port == 0 && reuse) {
      ::Error("TWinNTSystem::WinNTTcpService", "cannot do a port scan while reuse is true");
      return -1;
   }

   if ((sp = ::getservbyport(::htons(port), kProtocolName))) {
      sport = sp->s_port;
   } else {
      sport = ::htons(port);
   }

   // Create tcp socket
   SOCKET sock;
   if ((sock = ::socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      ::SysError("TWinNTSystem::WinNTTcpService", "socket");
      return -1;
   }

   if (reuse) {
      gSystem->SetSockOpt((int)sock, kReuseAddr, 1);
   }

   if (tcpwindowsize > 0) {
      gSystem->SetSockOpt((int)sock, kRecvBuffer, tcpwindowsize);
      gSystem->SetSockOpt((int)sock, kSendBuffer, tcpwindowsize);
   }

   struct sockaddr_in inserver;
   memset(&inserver, 0, sizeof(inserver));
   inserver.sin_family = AF_INET;
   inserver.sin_addr.s_addr = ::htonl(INADDR_ANY);
   inserver.sin_port = sport;

   // Bind socket
   if (port > 0) {
      if (::bind(sock, (struct sockaddr*) &inserver, sizeof(inserver)) == SOCKET_ERROR) {
         ::SysError("TWinNTSystem::WinNTTcpService", "bind");
         return -2;
      }
   } else {
      int bret;
      do {
         inserver.sin_port = ::htons(tryport++);
         bret = ::bind(sock, (struct sockaddr*) &inserver, sizeof(inserver));
      } while (bret == SOCKET_ERROR && WSAGetLastError() == WSAEADDRINUSE &&
               tryport < kSOCKET_MAXPORT);
      if (bret == SOCKET_ERROR) {
         ::SysError("TWinNTSystem::WinNTTcpService", "bind (port scan)");
         return -2;
      }
   }

   // Start accepting connections
   if (::listen(sock, backlog) == SOCKET_ERROR) {
      ::SysError("TWinNTSystem::WinNTTcpService", "listen");
      return -3;
   }
   return (int)sock;
}

//______________________________________________________________________________
int TWinNTSystem::AcceptConnection(int socket)
{
   // Accept a connection. In case of an error return -1. In case
   // non-blocking I/O is enabled and no connections are available
   // return -2.

   int soc = -1;
   SOCKET sock = socket;

   while ((soc = ::accept(sock, 0, 0)) == INVALID_SOCKET &&
          (::WSAGetLastError() == WSAEINTR)) {
      TSystem::ResetErrno();
   }

   if (soc == -1) {
      if (::WSAGetLastError() == WSAEWOULDBLOCK) {
         return -2;
      } else {
         ::SysError("AcceptConnection", "accept");
         return -1;
      }
   }
   return soc;
}
