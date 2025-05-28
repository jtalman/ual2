#ifndef __OUC_XSLOCK_HH__
#define __OUC_XSLOCK_HH__
/******************************************************************************/
/*                                                                            */
/*                       X r d O u c X S L o c k . h h                        */
/*                                                                            */
/* (c) 2003 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

//          $Id: XrdOucXSLock.hh,v 1.1 2004/05/11 06:24:50 abh Exp $

#include <errno.h>
#include "XrdOuc/XrdOucPthread.hh"

// These are valid usage options
//
enum XrdOucXS_Type {xs_None = 0, xs_Shared = 1, xs_Exclusive = 2};

// This class implements the shared lock. Any number of readers are allowed
// by requesting a shared lock. Only one exclusive writer is allowed by
// requesting an exclusive lock. Up/downgrading is not supported.
//
class XrdOucXSLock
{
public:

void        Lock(const XrdOucXS_Type usage);

void      UnLock(const XrdOucXS_Type usage=xs_None);

          XrdOucXSLock()
               {cur_usage = xs_None; cur_count = 0;
                exc_wait = 0; shr_wait = 0; toggle = 0;}

         ~XrdOucXSLock();

private:

XrdOucXS_Type cur_usage;
int           cur_count;
int           exc_wait;
int           shr_wait;
int           toggle;

XrdOucMutex       LockContext;
XrdOucSemaphore   WantShr;
XrdOucSemaphore   WantExc;
};
#endif
