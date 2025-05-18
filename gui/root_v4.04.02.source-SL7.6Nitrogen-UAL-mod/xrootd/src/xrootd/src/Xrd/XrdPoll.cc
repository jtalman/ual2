/******************************************************************************/
/*                                                                            */
/*                            X r d P o l l . c c                             */
/*                                                                            */
/* (c) 2004 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*       All Rights Reserved. See XrdInfo.cc for complete License Terms       */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

//           $Id: XrdPoll.cc,v 1.7 2004/11/20 03:47:18 abh Exp $

const char *XrdPollCVSID = "$Id: XrdPoll.cc,v 1.7 2004/11/20 03:47:18 abh Exp $";

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
  
#include "XrdOuc/XrdOucError.hh"
#include "XrdOuc/XrdOucPlatform.hh"
#include "XrdOuc/XrdOucPthread.hh"
#include "Xrd/XrdLink.hh"
#define  TRACELINK lp
#include "Xrd/XrdTrace.hh"

#ifdef _DEVPOLL
#include "Xrd/XrdPollDev.hh"
#else
#include "Xrd/XrdPollPoll.hh"
#endif

/******************************************************************************/
/*                           G l o b a l   D a t a                            */
/******************************************************************************/
  
       XrdPoll   *XrdPoll::Pollers[XRD_NUMPOLLERS] = {0, 0, 0};

       XrdOucMutex  XrdPoll::doingAttach;

       const char *XrdPoll::TraceID = "Poll";

extern XrdOucError  XrdLog;

extern XrdOucTrace  XrdTrace;

/******************************************************************************/
/*              T h r e a d   S t a r t u p   I n t e r f a c e               */
/******************************************************************************/

struct XrdPollArg
       {XrdPoll      *Poller;
        int            retcode;
        XrdOucSemaphore PollSync;

        XrdPollArg() : PollSync(0, "poll sync") {}
       ~XrdPollArg()               {}
       };

  
void *XrdStartPolling(void *parg)
{
     struct XrdPollArg *PArg = (struct XrdPollArg *)parg;
     PArg->Poller->Start(&(PArg->PollSync), PArg->retcode);
     return (void *)0;
}
 
/******************************************************************************/
/*                           C o n s t r u c t o r                            */
/******************************************************************************/
  
XrdPoll::XrdPoll()
{
   int fildes[2];

   TID=0;
   numAttached=numEnabled=numEvents=numInterrupts=0;

   if (pipe(fildes) == 0)
      {CmdFD = fildes[1]; fcntl(CmdFD, F_SETFD, FD_CLOEXEC);
       ReqFD = fildes[0]; fcntl(ReqFD, F_SETFD, FD_CLOEXEC);
      } else {
       CmdFD = ReqFD = -1;
       XrdLog.Emsg("Poll", errno, "create poll pipe");
      }
   PipeBuff        = 0;
   PipeBlen        = 0;
   PipePoll.fd     = ReqFD;
   PipePoll.events = POLLIN | POLLRDNORM;
}

/******************************************************************************/
/*                                A t t a c h                                 */
/******************************************************************************/
  
int XrdPoll::Attach(XrdLink *lp)
{
   int i;
   XrdPoll *pp;

// We allow only one attach at a time to simplify the processing
//
   doingAttach.Lock();

// Find a poller with the smallest number of entries
//
   pp = Pollers[0];
   for (i = 1; i < XRD_NUMPOLLERS; i++)
       if (pp->numAttached > Pollers[i]->numAttached) pp = Pollers[i];

// Include this FD into the poll set of the poller
//
   if (!pp->Include(lp)) {doingAttach.UnLock(); return 0;}

// Complete the link setup
//
   lp->Poller = pp;
   pp->numAttached++;
   doingAttach.UnLock();
   TRACEI(POLL, "FD " <<lp->FD <<" attached to poller " <<pp->PID <<"; num=" <<pp->numAttached);
   return 1;
}

/******************************************************************************/
/*                                D e t a c h                                 */
/******************************************************************************/
  
void XrdPoll::Detach(XrdLink *lp)
{
   XrdPoll *pp;

// If link is not attached, simply return
//
   if (!(pp = lp->Poller)) return;

// Exclude this link from the associated poll set
//
   pp->Exclude(lp);

// Make sure we are consistent
//
   doingAttach.Lock();
   if (!pp->numAttached)
      {XrdLog.Emsg("Poll","Underflow detaching", lp->ID); abort();}
   pp->numAttached--;
   doingAttach.UnLock();
   TRACEI(POLL, "FD " <<lp->FD <<" detached from poller " <<pp->PID <<"; num=" <<pp->numAttached);
}

/******************************************************************************/
/*                            g e t R e q u e s t                             */
/******************************************************************************/

// Warning: This method runs unlocked. The caller must have exclusive use of
//          the ReqBuff otherwise unpredictable results will occur.

int XrdPoll::getRequest()
{
   ssize_t rlen;
   int rc;

// See if we are to resume a read or start a fresh one
//
   if (!PipeBlen) 
      {PipeBuff = (char *)&ReqBuff; PipeBlen = sizeof(ReqBuff);}

// Wait for the next request. Some OS's (like Linux) don't support non-blocking
// pipes. So, we must front the read with a poll.
//
   do {rc = poll(&PipePoll, 1, 0);}
      while(rc < 0 && (errno == EAGAIN || errno == EINTR));
   if (rc < 1) return 0;

// Now we can put up a read without a delay. Normally a full command will be
// present. Under some heavy conditions, this may not be the case.
//
   do {rlen = read(ReqFD, PipeBuff, PipeBlen);} 
      while(rlen < 0 && errno == EINTR);
   if (rlen <= 0)
      {if (rlen) XrdLog.Emsg("Poll", errno, "read from request pipe");
       return 0;
      }

// Check if all the data has arrived. If not all the data is present, defer
// this request until more data arrives.
//
   if (!(PipeBlen -= rlen)) return 1;
   PipeBuff += rlen;
   TRACE(POLL, "Poller " <<PID <<" still needs " <<PipeBlen <<" req pipe bytes");
   return 0;
}

/******************************************************************************/
/*                             P o l l 2 T e x t                              */
/******************************************************************************/
  
char *XrdPoll::Poll2Text(short events)
{
   if (events & POLLERR) return strdup("socket error");

   if (events & POLLHUP) return strdup("client disconnected");

   if (events & POLLNVAL) return strdup("client closed socket");

  {char buff[64];
   sprintf(buff, "unusual event (%.4x)", events);
   return strdup(buff);
  }
  return (char *)0;
}

/******************************************************************************/
/*                                 S e t u p                                  */
/******************************************************************************/
  
int XrdPoll::Setup(int numfd)
{
   pthread_t tid;
   int maxfd, retc, i;
   struct XrdPollArg PArg;

// Calculate the number of table entries per poller
//
   maxfd  = (numfd / XRD_NUMPOLLERS) + 16;

// Verify that we initialized the poller table
//
   for (i = 0; i < XRD_NUMPOLLERS; i++)
       {if (!(Pollers[i] = newPoller(i, maxfd))) return 0;
        Pollers[i]->PID = i;

   // Now start a thread to handle this poller object
   //
        PArg.Poller = Pollers[i];
        PArg.retcode= 0;
        TRACE(POLL, "Starting poller " <<i);
        if ((retc = XrdOucThread::Run(&tid,XrdStartPolling,(void *)&PArg,
                                      XRDOUCTHREAD_BIND, "Poller")))
           {XrdLog.Emsg("Poll", retc, "create poller thread"); return 0;}
        Pollers[i]->TID = tid;
        PArg.PollSync.Wait();
        if (PArg.retcode)
           {XrdLog.Emsg("Poll", PArg.retcode, "start poller");
            return 0;
           }
       }

// All done
//
   return 1;
}

/******************************************************************************/
/*                                 S t a t s                                  */
/******************************************************************************/
  
int XrdPoll::Stats(char *buff, int blen, int do_sync)
{
   static const char statfmt[] = "<stats id=\"poll\"><att>%d</att>"
   "<en>%d</en><ev>%d</ev><int>%d</int></stats>";
   int i, numatt = 0, numen = 0, numev = 0, numint = 0;
   XrdPoll *pp;

// Return number of bytes if so wanted
//
   if (!buff) return (sizeof(statfmt)+(4*16))*XRD_NUMPOLLERS;

// Get statistics. While we wish we could honor do_sync, doing so would be
// costly and hardly worth it. So, we do not include code such as:
//    x = pp->y; if (do_sync) while(x != pp->y) x = pp->y; tot += x;
//
   for (i = 0; i < XRD_NUMPOLLERS; i++)
       {pp = Pollers[i];
        numatt += pp->numAttached; 
        numen  += pp->numEnabled;
        numev  += pp->numEvents;
        numint += pp->numInterrupts;
       }

// Format and return
//
   return snprintf(buff, blen, statfmt, numatt, numen, numev, numint);
}
  
/******************************************************************************/
/*              I m p l e m e n t a t i o n   S p e c i f i c s               */
/******************************************************************************/
  
#ifdef _DEVPOLL
#include "Xrd/XrdPollDev.icc"
#else
#include "Xrd/XrdPollPoll.icc"
#endif
