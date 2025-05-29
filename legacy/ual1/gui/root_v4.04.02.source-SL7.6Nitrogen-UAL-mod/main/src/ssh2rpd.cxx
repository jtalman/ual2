// @(#)root/main:$Name:  $:$Id: ssh2rpd.cxx,v 1.7 2004/04/20 22:12:42 rdm Exp $
// Author: Gerardo Ganis    1/7/2003

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// ssh2rpd                                                              //
//                                                                      //
// Small program to communicate successful result of sshd auth to the   //
// relevant rootd and proofd daemons                                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <errno.h>
#include <pwd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include "Varargs.h"
#include "snprintf.h"
#include <string.h>
#ifdef R__GLOBALSTL
namespace std { using ::string; }
#endif


#define kMAXPATHLEN 2048

//______________________________________________________________________________
void Info(const char *va_(fmt), ...)
{
   // Write info message to syslog.

   char    buf[kMAXPATHLEN];
   va_list ap;

   va_start(ap,va_(fmt));
   vsnprintf(buf, sizeof(buf), fmt, ap);
   va_end(ap);

#if 0
// syslog(LOG_INFO, buf);
#else
   fprintf(stderr, "%s\n",buf);
#endif

}

//______________________________________________________________________________
int main(int argc, char **argv)
{
   // Small program to communicate successful result of sshd auth to the
   // relevant root server daemons.

   int   gDebug = 0;
   char *PipeId = 0;
   char *TmpDir = 0;

   if (argc < 3) {
      Info("ssh2rpd: argc=%d"
           ": at least one additional argument required - exit",
           argc);
      exit(1);
   }

   // Parse Arguments
   gDebug = atoi(argv[1]);
   if (argc > 2)
      PipeId   = strdup(argv[2]);
   if (argc > 3)
      TmpDir = strdup(argv[3]);

   if (gDebug > 0) {
      std::string tmp = std::string("ssh2rpd: forked with args:");
      int i = 1;
      for( ; i < argc; i++) {
         tmp.append(" ");
         tmp.append(argv[i]);
      }
      Info("%s",tmp.c_str());
   }

   // Get logged username
   struct passwd *pw = getpwuid(getuid());

   char PipeFile[kMAXPATHLEN];
   if (!TmpDir)
      snprintf(PipeFile,kMAXPATHLEN, "%s/RootSshPipe.%s", pw->pw_dir, PipeId);
   else
      snprintf(PipeFile,kMAXPATHLEN,"%s/RootSshPipe.%s", TmpDir, PipeId);

   FILE *fpipe = fopen(PipeFile, "r");
   char Pipe[kMAXPATHLEN];
   if (fpipe) {
      while (fgets(Pipe, sizeof(Pipe), fpipe)) {
         if (Pipe[strlen(Pipe)-1] == '\n')
            Pipe[strlen(Pipe)-1] = 0;
      }
      fclose(fpipe);
      // Remove the temporary file
      unlink(PipeFile);
   } else {
      Info("ssh2rpd: cannot open file with pipe info (%s): exiting"
           " (errno= %d)",PipeFile,errno);
      exit(1);
   }

   // Preparing socket connection
   struct sockaddr_un servAddr;
   servAddr.sun_family = AF_UNIX;
   strcpy(servAddr.sun_path,Pipe);
   int sd;
   if ((sd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
      Info("ssh2rpd: cannot open socket: exiting ");
      exit(1);
   }
   // Connecting to socket
   int rc;
   if ((rc = connect(sd, (struct sockaddr *) &servAddr, sizeof(servAddr))) < 0) {
      Info("ssh2rpd: cannot connect socket: exiting (errno= %d)",errno);
      exit(1);
   }

   // Sending 'OK <username>'
   char okbuf[256];
   sprintf(okbuf,"OK %s",pw->pw_name);
   int lbuf = strlen(okbuf);
   lbuf = htonl(lbuf);
   rc = send(sd, &lbuf, sizeof(lbuf), 0);
   if (rc != (int)sizeof(lbuf)) {
      Info("ssh2rpd: sending might have been unsuccessful (bytes send: %d)",rc);
   }
   rc = send(sd, okbuf, strlen(okbuf), 0);
   if (rc != (int)strlen(okbuf)) {
      Info("ssh2rpd: sending might have been unsuccessful (bytes send: %d)",rc);
   }

   if (TmpDir) free(TmpDir);

   exit(0);
}
