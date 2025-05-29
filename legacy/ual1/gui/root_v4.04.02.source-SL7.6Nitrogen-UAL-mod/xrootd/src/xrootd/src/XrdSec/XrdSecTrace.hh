#ifndef ___SEC_TRACE_H___
#define ___SEC_TRACE_H___
/******************************************************************************/
/*                                                                            */
/*                        X r d S e c T r a c e . h h                         */
/*                                                                            */
/* (C) 2003 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*               DE-AC03-76-SFO0515 with the Deprtment of Energy              */
/******************************************************************************/
  
//         $Id: XrdSecTrace.hh,v 1.2 2004/08/30 07:17:04 elmer Exp $

#include "XrdOuc/XrdOucTrace.hh"

#ifndef NODEBUG

#include <iostream>

#define QTRACE(act) SecTrace->What & TRACE_ ## act

#define TRACE(act, x) \
        if (QTRACE(act)) \
           {SecTrace->Beg(epname,tident); std::cerr <<x; SecTrace->End();}

#define DEBUG(y) if (QTRACE(Debug)) \
                    {SecTrace->Beg(epname); std::cerr <<y; SecTrace->End();}
#define EPNAME(x) const char *epname = x;

#else

#define  TRACE(x, y)
#define QTRACE(x)
#define DEBUG(x)
#define EPNAME(x)

#endif

// Trace flags
//
#define TRACE_ALL      0x000f
#define TRACE_Authenxx 0x0007
#define TRACE_Authen   0x0004
#define TRACE_Debug    0x0001

#endif
