// @(#)root/base:$Name:  $:$Id: TError.cxx,v 1.10 2005/04/28 16:14:27 rdm Exp $
// Author: Fons Rademakers   29/07/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Error handling routines.                                             //
//                                                                      //
// This file defines a number of global error handling routines:        //
// Warning(), Error(), SysError() and Fatal(). They all take a          //
// location string (where the error happened) and a printf style format //
// string plus vararg's. In the end these functions call an             //
// errorhanlder function. By default DefaultErrorHandler() is used.     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include "snprintf.h"

#include "TError.h"
#include "TSystem.h"
#include "TString.h"
#include "TVirtualMutex.h"

int gErrorIgnoreLevel = 0;
int gErrorAbortLevel  = kSysError+1;

const char *kAssertMsg = "%s violated at line %d of `%s'";
const char *kCheckMsg  = "%s not true at line %d of `%s'";

static ErrorHandlerFunc_t gErrorHandler = DefaultErrorHandler;


//______________________________________________________________________________
static void DebugPrint(const char *fmt, ...)
{
   // Print debugging message to stderr and, on Windows, to the system debugger.

   static Int_t buf_size = 2048;
   static char *buf = 0;

   va_list arg_ptr;
   va_start(arg_ptr, fmt);

   R__LOCKGUARD(gErrPrintMutex);

again:
   if (!buf)
      buf = new char[buf_size];

   int n = vsnprintf(buf, buf_size, fmt, arg_ptr);

   if (n == -1 || n >= buf_size) {
      buf_size *= 2;
      delete [] buf;
      buf = 0;
      goto again;
   }
   va_end(arg_ptr);

   fprintf(stderr, "%s", buf);

#ifdef WIN32
   ::OutputDebugString(buf);
#endif
}


//______________________________________________________________________________
ErrorHandlerFunc_t SetErrorHandler(ErrorHandlerFunc_t newhandler)
{
   // Set an errorhandler function. Returns the old handler.

   ErrorHandlerFunc_t oldhandler = gErrorHandler;
   gErrorHandler = newhandler;
   return oldhandler;
}

//______________________________________________________________________________
ErrorHandlerFunc_t GetErrorHandler()
{
   // Returns the current error handler function.

   return gErrorHandler;
}

//______________________________________________________________________________
void DefaultErrorHandler(int level, Bool_t abort, const char *location, const char *msg)
{
   // The default error handler function. It prints the message on stderr and
   // if abort is set it aborts the application.

   if (level < gErrorIgnoreLevel)
      return;

   const char *type = 0;

   if (level >= kInfo)
      type = "Info";
   if (level >= kWarning)
      type = "Warning";
   if (level >= kError)
      type = "Error";
   if (level >= kBreak)
      type = "\n *** Break ***";
   if (level >= kSysError)
      type = "SysError";
   if (level >= kFatal)
      type = "Fatal";

   if (level >= kBreak && level < kSysError)
      DebugPrint("%s %s\n", type, msg);
   else if (!location || strlen(location) == 0)
      DebugPrint("%s: %s\n", type, msg);
   else
      DebugPrint("%s in <%s>: %s\n", type, location, msg);

   fflush(stderr);
   if (abort) {
      DebugPrint("aborting\n");
      fflush(stderr);
      if (gSystem) {
         gSystem->StackTrace();
         gSystem->Abort();
      } else
         ::abort();
   }
}

//______________________________________________________________________________
void ErrorHandler(int level, const char *location, const char *fmt, va_list ap)
{
   // General error handler function. It calls the user set error handler.

   static Int_t buf_size = 2048;
   static char *buf = 0;

   char *bp;

again:
   if (!buf)
      buf = new char[buf_size];

   int n = vsnprintf(buf, buf_size, fmt, ap);
   // old vsnprintf's return -1 if string is truncated new ones return
   // total number of characters that would have been written
   if (n == -1 || n >= buf_size) {
      buf_size *= 2;
      delete [] buf;
      buf = 0;
      goto again;
   }
   if (level >= kSysError && level < kFatal)
      bp = Form("%s (%s)", buf, gSystem->GetError());
   else
      bp = buf;

   if (level != kFatal)
      gErrorHandler(level, level >= gErrorAbortLevel, location, bp);
   else
      gErrorHandler(level, kTRUE, location, bp);
}

//______________________________________________________________________________
void AbstractMethod(const char *method)
{
   // This function can be used in abstract base classes in case one does
   // not want to make the class a "real" (in C++ sense) ABC. If this
   // function is called it will warn the user that the function should
   // have been overridden.

   Warning(method, "this method must be overridden!");
}

//______________________________________________________________________________
void MayNotUse(const char *method)
{
   // This function can be used in classes that should override a certain
   // function, but in the inherited class the function makes no sense.

   Warning(method, "may not use this method");
}

//______________________________________________________________________________
void Error(const char *location, const char *va_(fmt), ...)
{
   // Use this function in case an error occured.

   va_list ap;
   va_start(ap,va_(fmt));
   ErrorHandler(kError, location, va_(fmt), ap);
   va_end(ap);
}

//______________________________________________________________________________
void SysError(const char *location, const char *va_(fmt), ...)
{
   // Use this function in case a system (OS or GUI) related error occured.

   va_list ap;
   va_start(ap, va_(fmt));
   ErrorHandler(kSysError, location, va_(fmt), ap);
   va_end(ap);
}

//______________________________________________________________________________
void Break(const char *location, const char *va_(fmt), ...)
{
   // Use this function in case an error occured.

   va_list ap;
   va_start(ap,va_(fmt));
   ErrorHandler(kBreak, location, va_(fmt), ap);
   va_end(ap);
}

//______________________________________________________________________________
void Info(const char *location, const char *va_(fmt), ...)
{
   // Use this function for informational messages.

   va_list ap;
   va_start(ap,va_(fmt));
   ErrorHandler(kInfo, location, va_(fmt), ap);
   va_end(ap);
}

//______________________________________________________________________________
void Warning(const char *location, const char *va_(fmt), ...)
{
   // Use this function in warning situations.

   va_list ap;
   va_start(ap,va_(fmt));
   ErrorHandler(kWarning, location, va_(fmt), ap);
   va_end(ap);
}

//______________________________________________________________________________
void Fatal(const char *location, const char *va_(fmt), ...)
{
   // Use this function in case of a fatal error. It will abort the program.

   va_list ap;
   va_start(ap,va_(fmt));
   ErrorHandler(kFatal, location, va_(fmt), ap);
   va_end(ap);
}
