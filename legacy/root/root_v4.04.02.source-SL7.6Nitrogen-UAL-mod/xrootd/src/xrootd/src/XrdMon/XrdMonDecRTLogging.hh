/*****************************************************************************/
/*                                                                           */
/*                           XrdMonDecRTLogging.hh                           */
/*                                                                           */
/* (c) 2005 by the Board of Trustees of the Leland Stanford, Jr., University */
/*                            All Rights Reserved                            */
/*       Produced by Jacek Becla for Stanford University under contract      */
/*              DE-AC02-76SF00515 with the Department of Energy              */
/*****************************************************************************/

// $Id: XrdMonDecRTLogging.hh,v 1.5 2005/03/15 22:47:29 becla Exp $

#ifndef XRDMONDECRTLOGGING_HH
#define XRDMONDECRTLOGGING_HH

#include "XrdMon/XrdMonDecDictInfo.hh"
#include "XrdMon/XrdMonDecUserInfo.hh"
#include "XrdOuc/XrdOucPthread.hh"
#include <string>
using std::string;

class XrdMonDecRTLogging {
public:
    XrdMonDecRTLogging(const char* dir, int rtBufSize);
    ~XrdMonDecRTLogging();

    void add(XrdMonDecUserInfo::TYPE t, XrdMonDecUserInfo* x);
    void add(XrdMonDecDictInfo::TYPE t, XrdMonDecDictInfo* x);

    void flush(bool lockMutex=true);
    
private:
    char*       _rtLog;
    char*       _rtLogLock;

    char*       _buf;
    const int   _bufSize; // flush when buffer is full, or when
                          // triggered by external thread
    XrdOucMutex _mutex;
};

#endif /* XRDMONDECRTLOGGING_HH */
