/*****************************************************************************/
/*                                                                           */
/*                          XrdMonDecTraceInfo.cc                            */
/*                                                                           */
/* (c) 2004 by the Board of Trustees of the Leland Stanford, Jr., University */
/*                            All Rights Reserved                            */
/*       Produced by Jacek Becla for Stanford University under contract      */
/*              DE-AC02-76SF00515 with the Department of Energy              */
/*****************************************************************************/

// $Id: XrdMonDecTraceInfo.cc,v 1.5 2005/03/10 21:32:30 becla Exp $

#include "XrdMon/XrdMonUtils.hh"
#include "XrdMon/XrdMonDecTraceInfo.hh"
#include <stdio.h>
#include <iomanip>

kXR_int32 XrdMonDecTraceInfo::_lastT(0);
string XrdMonDecTraceInfo::_lastS;

void
XrdMonDecTraceInfo::convertToString(char s[256])
{
    if ( _timestamp != _lastT ) {
        XrdMonDecTraceInfo::_lastT = _timestamp;
        XrdMonDecTraceInfo::_lastS = timestamp2string(_timestamp);
    }
    sprintf(s, "%lld\t%d\t%c\t%s\t%d\n", (long long)_offset, 
            _length, _rwReq, _lastS.c_str(), _uniqueId);
}

ostream& operator<<(ostream& o, const XrdMonDecTraceInfo& ti) {
    if ( ti._timestamp != XrdMonDecTraceInfo::_lastT ) {
        XrdMonDecTraceInfo::_lastT = ti._timestamp;
        XrdMonDecTraceInfo::_lastS = timestamp2string(ti._timestamp);
    }

    o << ti._offset << '\t'
      << ti._length << '\t'
      << ti._rwReq  << '\t' 
      << XrdMonDecTraceInfo::_lastS << '\t'
      << ti._uniqueId;

    return o;
}
