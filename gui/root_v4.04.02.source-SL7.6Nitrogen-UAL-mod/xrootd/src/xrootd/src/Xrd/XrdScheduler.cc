/******************************************************************************/
/*                                                                            */
/*                       X r d S c h e d u l e r . h h                        */
/*                                                                            */
/* (c) 2004 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*       All Rights Reserved. See XrdInfo.cc for complete License Terms       */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

//        $Id: XrdScheduler.cc,v 1.9 2005/01/21 06:50:27 abh Exp $

const char *XrdSchedulerCVSID = "$Id: XrdScheduler.cc,v 1.9 2005/01/21 06:50:27 abh Exp $";

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Xrd/XrdJob.hh"
#include "Xrd/XrdScheduler.hh"
#include "Xrd/XrdTrace.hh"
#include "XrdOuc/XrdOucError.hh"

/******************************************************************************/
/*                        G l o b a l   O b j e c t s                         */
/******************************************************************************/

extern XrdOucError   XrdLog;
  
#ifndef NODEBUG
extern XrdOucTrace   XrdTrace;
#endif

       const char   *XrdScheduler::TraceID = "Sched";

/******************************************************************************/
/*                         L o c a l   C l a s s e s                          */
/******************************************************************************/

class XrdFireWorker : public XrdJob
{
public:

void DoIt() {delete this;
             pthread_exit((void *)0);
            }

     XrdFireWorker() : XrdJob("underused worker termination"){}
    ~XrdFireWorker() {}
};

class XrdSchedulerPID
     {public:
      XrdSchedulerPID *next;
      pid_t            pid;

      XrdSchedulerPID(pid_t newpid, XrdSchedulerPID *prev) 
                        {next = prev; pid = newpid;}
     ~XrdSchedulerPID() {}
     };
  
/******************************************************************************/
/*            E x t e r n a l   T h r e a d   I n t e r f a c e s             */
/******************************************************************************/
  
void *XrdStartReaper(void *carg)
      {XrdScheduler *sp = (XrdScheduler *)carg;
       sp->Reaper();
       return (void *)0;
      }

void *XrdStartTSched(void *carg)
      {XrdScheduler *sp = (XrdScheduler *)carg;
       sp->TimeSched();
       return (void *)0;
      }

void *XrdStartWorking(void *carg)
      {XrdScheduler *sp = (XrdScheduler *)carg;
       sp->Run();
       return (void *)0;
      }

/******************************************************************************/
/*                           C o n s t r u c t o r                            */
/******************************************************************************/
  
XrdScheduler::XrdScheduler(int minw, int maxw, int avlw, int maxi)
              : XrdJob("underused thread monitor"),
                WorkAvail(0, "sched work")
{
    min_Workers =  minw;
    max_Workers =  maxw;
    max_Workidl =  maxi;
    num_Workers =  0;
    num_JobsinQ =  0;
    avl_Workers =  avlw;
    stk_Workers =  maxw - avlw;
    idl_Workers =  0;
    num_Jobs    =  0;
    max_QLength =  0;
    num_TCreate =  0;
    num_TDestroy=  0;
    num_Limited =  0;
    firstPID    =  0;
    WorkFirst = WorkLast = TimerQueue = 0;
}
 
/******************************************************************************/
/*                            D e s t r u c t o r                             */
/******************************************************************************/

XrdScheduler::~XrdScheduler()  // The scheduler is never deleted!
{
}
 
/******************************************************************************/
/*                                C a n c e l                                 */
/******************************************************************************/

void XrdScheduler::Cancel(XrdJob *jp)
{
   XrdJob *p, *pp = 0;

// Lock the queue
//
   TimerMutex.Lock();

// Find the matching job, if any
//
   p = TimerQueue;
   while(p && p != jp) {pp = p; p = p->NextJob;}

// Delete the job element
//
   if (p)
      {if (pp) pp->NextJob = p->NextJob;
          else  TimerQueue = p->NextJob;
       TRACE(SCHED, "time event " <<jp->Comment <<" cancelled");
      }

// All done
//
   TimerMutex.UnLock();
}
  
/******************************************************************************/
/*                                  D o I t                                   */
/******************************************************************************/

void XrdScheduler::DoIt()
{
   XrdFireWorker *fwp;
   int num_kill, num_idle;

// Now check if there are too many idle threads (kill them if there are)
//
   if (!num_JobsinQ)
      {DispatchMutex.Lock(); num_idle = idl_Workers; DispatchMutex.UnLock();
       num_kill = num_idle - min_Workers;
       TRACE(SCHED, num_Workers <<" threads; " <<num_idle <<" idle");
       if (num_kill > 0)
          {if (num_kill > 1) num_kill = num_kill/2;
           SchedMutex.Lock();
           num_TDestroy += num_kill;
           num_Workers  -= num_kill;
           SchedMutex.UnLock();
           while(num_kill--)
                if ((fwp = new XrdFireWorker())) Schedule((XrdJob *)fwp);
          }
      }

// Check if we should reschedule ourselves
//
   if (max_Workidl > 0) Schedule((XrdJob *)this, max_Workidl+time(0));
}

/******************************************************************************/
/*                                  F o r k                                   */
/******************************************************************************/
  
// This entry exists solely so that we can start a reaper thread for processes
//
pid_t XrdScheduler::Fork(const char *id)
{
   static int  retc, ReaperStarted = 0;
   pthread_t tid;
   pid_t pid;

// Fork
//
   if ((pid = fork()) < 0)
      {XrdLog.Emsg("Scheduler",errno,(char *)"fork to handle",(char *)id);
       return pid;
      }
   if (!pid) return pid;

// Obtain the status of the reaper thread.
//
   ReaperMutex.Lock();
   firstPID = new XrdSchedulerPID(pid, firstPID);
   retc = ReaperStarted;
   ReaperStarted = 1;
   ReaperMutex.UnLock();

// Start the reaper thread if it has not started.
//
   if (!retc)
      if ((retc = XrdOucThread::Run(&tid, XrdStartReaper, (void *)this,
                                    0, "Process reaper")))
         {XrdLog.Emsg("Scheduler", retc, "create reaper thread");
          ReaperStarted = 0;
         }

   return pid;
}

/******************************************************************************/
/*                                R e a p e r                                 */
/******************************************************************************/
  
void *XrdScheduler::Reaper()
{
   sigset_t Sset;
   int signum, status;
   pid_t pid;
   XrdSchedulerPID *tp, *ptp, *xtp;

// Set up for signal handling. Note: main() must block this signal at start)
//
   sigemptyset(&Sset);
   sigaddset(&Sset, SIGCHLD);

// Wait for all outstanding children
//
   do {ReaperMutex.Lock();
       tp = firstPID; ptp = 0;
       while(tp)
            {do {pid = waitpid(tp->pid, &status, WNOHANG);}
                while (pid < 0 && errno == EINTR);
             if (pid > 0)
                {if (TRACING(TRACE_SCHED)) traceExit(pid, status);
                 xtp = tp; tp = tp->next;
                 if (ptp) ptp->next = tp;
                    else   firstPID = tp;
                 delete xtp;
                } else {ptp = tp; tp = tp->next;}
             }
       ReaperMutex.UnLock();
      } while(sigwait(&Sset, &signum) >= 0);
   return (void *)0;
}

/******************************************************************************/
/*                                   R u n                                    */
/******************************************************************************/
  
void XrdScheduler::Run()
{
   int waiting;
   XrdJob *jp;

// Wait for work then do it (an endless task for a worker thread)
//
   do {DispatchMutex.Lock(); idl_Workers++; DispatchMutex.UnLock();
       do {WorkAvail.Wait();
           SchedMutex.Lock();
           if ((jp = WorkFirst))
              {if (!(WorkFirst = jp->NextJob)) WorkLast = 0;
               if (num_JobsinQ) num_JobsinQ--;
                  else {XrdLog.Emsg("Scheduler","Job queue count underflow!");
                        num_JobsinQ = 0;
                       }
               DispatchMutex.Lock(); 
               waiting = --idl_Workers;
               DispatchMutex.UnLock();
               if (!waiting)
                  if (num_Workers < max_Workers) hireWorker();
                     else {num_Limited++;
                           if ((num_Limited & 4095) == 1)
                              XrdLog.Emsg("Scheduler",
                                           "Thread limit has been reached!");
                          }
              } else if (num_JobsinQ)
                        {XrdLog.Emsg("Scheduler","More jobs scheduled than could be run");
                         num_JobsinQ = 0;
                        }
           SchedMutex.UnLock();
          } while(!jp);
       TRACE(SCHED, "running " <<jp->Comment <<" inq=" <<num_JobsinQ);
       jp->DoIt();
      } while(1);
}
 
/******************************************************************************/
/*                              S c h e d u l e                               */
/******************************************************************************/
  
void XrdScheduler::Schedule(XrdJob *jp)
{
// Lock down our data area
//
   SchedMutex.Lock();

// Place the request on the queue and broadcast it
//
   jp->NextJob  = 0;
   if (WorkFirst)
      {WorkLast->NextJob = jp;
       WorkLast = jp;
      } else {
       WorkFirst = jp;
       WorkLast  = jp;
      }
   WorkAvail.Post();

// Calculate statistics
//
   num_Jobs++;
   num_JobsinQ++;
   if (num_JobsinQ > max_QLength) max_QLength = num_JobsinQ;

// Unlock the data area and return
//
   SchedMutex.UnLock();
}

/******************************************************************************/
  
void XrdScheduler::Schedule(int numjobs, XrdJob *jfirst, XrdJob *jlast)
{

// Lock down our data area
//
   SchedMutex.Lock();

// Place the request list on the queue
//
   jlast->NextJob = 0;
   if (WorkFirst)
      {WorkLast->NextJob = jfirst;
       WorkLast = jlast;
      } else {
       WorkFirst = jfirst;
       WorkLast  = jlast;
      }

// Calculate statistics
//
   num_Jobs    += numjobs;
   num_JobsinQ += numjobs;
   if (num_JobsinQ > max_QLength) max_QLength = num_JobsinQ;

// Indicate number of jobs to work on
//
   while(numjobs--) WorkAvail.Post();

// Unlock the data area and return
//
   SchedMutex.UnLock();
}

/******************************************************************************/

void XrdScheduler::Schedule(XrdJob *jp, time_t atime)
{
   XrdJob *pp = 0, *p;

// Cancel this event, if scheduled
//
   Cancel(jp);

// Lock the queue
//
   TRACE(SCHED, "scheduling " <<jp->Comment <<" in " <<atime-time(0) <<" seconds");
   jp->SchedTime = atime;
   TimerMutex.Lock();

// Find the insertion point for the work element
//
   p = TimerQueue;
   while(p && p->SchedTime <= atime) {pp = p; p = p->NextJob;}

// Insert the job element
//
   jp->NextJob = p;
   if (pp)  pp->NextJob = jp;
      else {TimerQueue = jp; TimerRings.Signal();}

// All done
//
   TimerMutex.UnLock();
}

/******************************************************************************/
/*                              s e t P a r m s                               */
/******************************************************************************/
  
void XrdScheduler::setParms(int minw, int maxw, int avlw, int maxi)
{

// Lock the data area
//
   SchedMutex.Lock();

// Make sure we have consistent minw and maxw
//
        if (minw <= 0)   minw = maxw;
   else if (maxw <= 0)   maxw = minw;
   else if (minw > maxw) minw = maxw = -1;
        if (avlw > maxw) avlw = maxw;

// Set the values
//
   if (minw > 0)  min_Workers = minw;
   if (maxw > 0)  max_Workers = maxw;
   if (maxi >=0)  max_Workidl = maxi;
   if (avlw > 0) {stk_Workers = max_Workers - avlw;
                  avl_Workers = avlw;
                 }

// Unlock the data area
//
   SchedMutex.UnLock();

// If we have an idle interval, schedule the idle check
//
   if (maxi > 0) Schedule((XrdJob *)this, (time_t)maxi+time(0));

// Debug the info
//
   TRACE(SCHED,"Set min_Workers=" <<min_Workers <<" max_Workers=" <<max_Workers);
   TRACE(SCHED,"Set stk_Workers=" <<stk_Workers <<" max_Workidl=" <<max_Workidl);
}

/******************************************************************************/
/*                                 S t a r t                                  */
/******************************************************************************/
  
void XrdScheduler::Start(int numw)
{
    int retc;
    pthread_t tid;

// Start a time based scheduler
//
   if ((retc = XrdOucThread::Run(&tid, XrdStartTSched, (void *)this,
                                 XRDOUCTHREAD_BIND, "Time scheduler")))
      XrdLog.Emsg("Scheduler", retc, "create time scheduler thread");

// If we an idle interval, schedule the idle check
//
   if (max_Workidl > 0) Schedule((XrdJob *)this, (time_t)max_Workidl+time(0));

// Lock the data area
//
   SchedMutex.Lock();

// If we are in agressive mode scheduling, start the minimum threads now
//
   if (numw <= 1) hireWorker();
      else {if (numw > min_Workers) numw = min_Workers;
            while(numw--) hireWorker();
           }

// Unlock the data area
//
   SchedMutex.UnLock();
}

/******************************************************************************/
/*                                 S t a t s                                  */
/******************************************************************************/
  
int XrdScheduler::Stats(char *buff, int blen, int do_sync)
{
    int cnt_Jobs, cnt_JobsinQ, xam_QLength, cnt_Workers, cnt_idl;
    int cnt_TCreate, cnt_TDestroy, cnt_Limited;
    static char statfmt[] = "<stats id=\"sched\"><jobs>%d</jobs>"
                "<inq>%d</inq><maxinq>%d</maxinq>"
                "<threads>%d</threads><idle>%d</idle>"
                "<tcr>%d</tcr><tde>%d</tde>"
                "<tlimr>%d</tlimr></stats>";

// If only length wanted, do so
//
   if (!buff) return sizeof(statfmt) + 16*8;

// Get values protected by the Dispatch lock (avoid lock if no sync needed)
//
   if (do_sync) DispatchMutex.Lock();
   cnt_idl = idl_Workers;
   if (do_sync) DispatchMutex.UnLock();

// Get values protected by the Scheduler lock (avoid lock if no sync needed)
//
   if (do_sync) SchedMutex.Lock();
   cnt_Workers = num_Workers;
   cnt_Jobs    = num_Jobs;
   cnt_JobsinQ = num_JobsinQ;
   xam_QLength = max_QLength;
   cnt_TCreate = num_TCreate;
   cnt_TDestroy= num_TDestroy;
   cnt_Limited = num_Limited;
   if (do_sync) SchedMutex.UnLock();

// Format the stats and return them
//
   return snprintf(buff, blen, statfmt, cnt_Jobs, cnt_JobsinQ, xam_QLength,
                   cnt_Workers, cnt_idl, cnt_TCreate, cnt_TDestroy,
                   cnt_Limited);
}

/******************************************************************************/
/*                             T i m e S c h e d                              */
/******************************************************************************/
  
void XrdScheduler::TimeSched()
{
   XrdJob *jp;
   int wtime;

// Continuous loop until we find some work here
//
   do {TimerMutex.Lock();
       if (TimerQueue) wtime = TimerQueue->SchedTime-time(0);
          else wtime = 60*60;
       if (wtime > 0)
          {TimerMutex.UnLock();
           TimerRings.Wait(wtime);
          } else {
           jp = TimerQueue;
           TimerQueue = jp->NextJob;
           Schedule(jp);
           TimerMutex.UnLock();
          }
       } while(1);
}

/******************************************************************************/
/*                       P r i v a t e   M e t h o d s                        */
/******************************************************************************/
/******************************************************************************/
/*                           h i r e   W o r k e r                            */
/******************************************************************************/
  
void XrdScheduler::hireWorker()  // Called with SchedMutex locked!
{
   pthread_t tid;
   int retc;

// Start a new thread
//
   if ((retc = XrdOucThread::Run(&tid, XrdStartWorking, (void *)this,
                                 0, "Worker")))
      XrdLog.Emsg("Scheduler", retc, "create worker thread");
      else {num_Workers++;
            num_TCreate++;
            TRACE(SCHED, "Now have " <<num_Workers <<" workers" );
           }
}
 
/******************************************************************************/
/*                             t r a c e E x i t                              */
/******************************************************************************/
  
void XrdScheduler::traceExit(pid_t pid, int status)
{  char *why;
   int   retc;

   if (WIFEXITED(status))
      {retc = WEXITSTATUS(status);
       why = (char *)" exited with rc=";
      } else if (WIFSIGNALED(status))
                {retc = WTERMSIG(status);
                 why = (char *)" killed with signal ";
                } else {retc = 0;
                        why = (char *)" changed state ";
                       }
   TRACE(SCHED, "Process " <<pid <<why <<retc);
}
