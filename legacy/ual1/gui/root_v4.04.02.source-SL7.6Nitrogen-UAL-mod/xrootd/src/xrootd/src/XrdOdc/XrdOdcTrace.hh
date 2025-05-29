#ifndef _XRDODC_TRACE_H
#define _XRDODC_TRACE_H
/******************************************************************************/
/*                                                                            */
/*                        X r d O d c T r a c e . h h                         */
/*                                                                            */
/* (C) 2003 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*                DE-AC03-76-SFO0515 with the Deprtment of Energy             */
/******************************************************************************/

//         $Id: XrdOdcTrace.hh,v 1.2 2004/08/30 07:16:55 elmer Exp $

//#include "XrdOuc/XrdOucTrace.hh"                                       ??????

// Trace flags
//
#define TRACE_ALL       0xffff
#define TRACE_Debug     0x8000
#define TRACE_Redirect  0x0001
#define TRACE_Forward   0x0002

#ifndef NODEBUG

#include <iostream>
#include "XrdOuc/XrdOucTrace.hh"

#define QTRACE(act) OdcTrace.What & TRACE_ ## act

#define TRACE(act, x) \
        if (QTRACE(act)) \
           {OdcTrace.Beg(epname); std::cerr <<x; OdcTrace.End();}

#define DEBUG(y) if (QTRACE(Debug)) \
                    {OdcTrace.Beg(epname); std::cerr <<y; OdcTrace.End();}

#define EPNAME(x) const char *epname = x;

#else

#define QTRACE(x) 0
#define DEBUG(x)
#define TRACE(x, y)
#define EPNAME(x)

#endif
#endif
