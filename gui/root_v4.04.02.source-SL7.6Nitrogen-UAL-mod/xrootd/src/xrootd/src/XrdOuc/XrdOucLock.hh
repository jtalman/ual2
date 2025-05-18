#ifndef __OUC_LOCK_HH__
#define __OUC_LOCK_HH__
/******************************************************************************/
/*                                                                            */
/*                         X r d O u c L o c k . h h                          */
/*                                                                            */
/* (c) 2003 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/
  
//         $Id: XrdOucLock.hh,v 1.1 2004/05/11 06:24:37 abh Exp $

#include "XrdOuc/XrdOucPthread.hh"

class XrdOucLock
{
public:
       XrdOucLock(XrdOucMutex *mp)
                    {Mutex = mp; Mutex->Lock(); isLocked = 1;}
      ~XrdOucLock() {if (isLocked) Mutex->UnLock();}

private:

XrdOucMutex *Mutex;
int          isLocked;
};
#endif
