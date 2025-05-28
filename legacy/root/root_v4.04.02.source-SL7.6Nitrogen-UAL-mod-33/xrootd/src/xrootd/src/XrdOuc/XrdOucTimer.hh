#ifndef __XrdOucTimer__
#define __XrdOucTimer__
/******************************************************************************/
/*                                                                            */
/*                        X r d O u c T i m e r . h h                         */
/*                                                                            */
/* (c) 2004 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*       All Rights Reserved. See XrdInfo.cc for complete License Terms       */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

//         $Id: XrdOucTimer.hh,v 1.2 2004/05/12 06:08:16 abh Exp $

#include <sys/time.h>

/* This include file describes the oo elapsed time interval interface. It is
   used by the oo Real Time Monitor, among others.
*/

class XrdOucTimer {

public:
       struct timeval *Delta_Time(struct timeval &tbeg);

static time_t Midnight(time_t tnow=0);

inline int    TimeLE(time_t tsec) {return StopWatch.tv_sec <= tsec;}

       // The following routines return the current interval added to the
       // passed argument as well as returning the current Unix seconds
       //
       unsigned long Report(double &);
       unsigned long Report(unsigned long &);
       unsigned long Report(unsigned long long &);
       unsigned long Report(struct timeval &);

inline void Reset()   {gettimeofday(&StopWatch, 0);}

inline time_t Seconds() {return StopWatch.tv_sec;}

inline void Set(struct timeval &tod)
                       {StopWatch.tv_sec  = tod.tv_sec;
                        StopWatch.tv_usec = tod.tv_usec;
                       }

static char *s2hms(int sec, char *buff, int blen);

static void Wait(int millisconds);

      XrdOucTimer() {Reset();}

private:
      struct timeval StopWatch;     // Current running clock
      struct timeval LastReport;    // Total time from last report

      unsigned long Report();       // Place interval in Last Report
};
#endif
