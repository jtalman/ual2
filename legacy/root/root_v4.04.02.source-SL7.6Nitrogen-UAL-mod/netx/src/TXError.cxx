// @(#)root/netx:$Name:  $:$Id: TXError.cxx,v 1.4 2004/09/16 20:50:33 rdm Exp $
// Author: Alvise Dorigo, Fabrizio Furano

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TXError                                                              //
//                                                                      //
// Authors: Alvise Dorigo, Fabrizio Furano                              //
//          INFN Padova, 2003                                           //
//                                                                      //
// Error handler function for TXNetFile classes                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TError.h"
#include "TSystem.h"
#include "TString.h"
#include "TDatime.h"
#include "TXError.h"


//_________________________________________________________________________
void TXNErrorHandler(int level, Bool_t abort, const char *location, const char *msg)
{
   // A modified error handler function. As in the DefaultErrorHandler
   // it prints the message on stderr and if abort is set it aborts the
   // application, however this one also prints time stamps before the
   // messages.

   if (level < gErrorIgnoreLevel)
      return;

   const char *type = 0;

   if (level >= kInfo)
      type = "Info";
   if (level >= kWarning)
      type = "Warn";
   if (level >= kError)
      type = "Err ";
   if (level >= kSysError)
      type = "SysError";
   if (level >= kFatal)
      type = "Fatal";

   if (!location || strlen(location) == 0)
      fprintf(stderr, "%s: %s\n", type, msg);
   else
      fprintf(stderr, "%s %d %s: %-30s - %s\n", TDatime().AsSQLString(), getpid(),
              type, location, msg);
   fflush(stderr);
   if (abort) {
      fprintf(stderr, "aborting\n");
      fflush(stderr);
      if (gSystem) {
         gSystem->StackTrace();
         gSystem->Abort();
      } else
         ::abort();
   }
}
