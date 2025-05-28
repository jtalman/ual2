/*****************************************************************************/
/*                                                                           */
/*                            XrdMonCtrDebug.hh                              */
/*                                                                           */
/* (c) 2005 by the Board of Trustees of the Leland Stanford, Jr., University */
/*                            All Rights Reserved                            */
/*       Produced by Jacek Becla for Stanford University under contract      */
/*              DE-AC02-76SF00515 with the Department of Energy              */
/*****************************************************************************/

// $Id: XrdMonCtrDebug.hh,v 1.1.1.1 2005/01/25 06:12:20 becla Exp $

#ifndef XRDMONCTRDEBUG_HH
#define XRDMONCTRDEBUG_HH

#include "XrdOuc/XrdOucPthread.hh"

// class responsible for handling log/debug/error messages

class XrdMonCtrDebug {
public:
    enum Verbosity {
        Quiet     = 0x0000, // No printing

        Receiving = 0x0001, // related to receiving data from xrootd
        PathDec   = 0x0002, // for path decoder, use with PathCod to compare
        DataDec   = 0x0004, // for data decoder, use with DataCod to compare
        IdDec     = 0x0008, // for id decoder, use with IdCod to compare
        Sink      = 0x0010, // related to sink
        // =======
        All       = 0xFFFF  // Everything
    };

    static void initialize();
    
    inline static bool verbose(Verbosity val) {
        return _verbose & val;
    }

    static XrdOucMutex _mutex;

private:
    static Verbosity _verbose;
};

#endif /* XRDMONCTRDEBUG_HH */
