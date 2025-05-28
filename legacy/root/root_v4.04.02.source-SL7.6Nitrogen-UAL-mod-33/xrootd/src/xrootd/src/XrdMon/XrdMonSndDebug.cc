/*****************************************************************************/
/*                                                                           */
/*                            XrdMonSndDebug.cc                              */
/*                                                                           */
/* (c) 2005 by the Board of Trustees of the Leland Stanford, Jr., University */
/*                            All Rights Reserved                            */
/*       Produced by Jacek Becla for Stanford University under contract      */
/*              DE-AC02-76SF00515 with the Department of Energy              */
/*****************************************************************************/

// $Id: XrdMonSndDebug.cc,v 1.1.1.1 2005/01/25 06:12:25 becla Exp $

#include "XrdMon/XrdMonSndDebug.hh"
#include "XrdMon/XrdMonCommon.hh"
#include <stdlib.h> /* getenv */
#include <string.h>

XrdMonSndDebug::Verbosity XrdMonSndDebug::_verbose = XrdMonSndDebug::Generator;

void
XrdMonSndDebug::initialize()
{
    const char* env = getenv("DEBUG");
    if ( 0 == env ) {
        return;
    }
    if ( 0 == strcmp(env, "all") ) {
        _verbose = XrdMonSndDebug::All;
        return;
    }
    _verbose = (XrdMonSndDebug::Verbosity) atoi(env);
}
