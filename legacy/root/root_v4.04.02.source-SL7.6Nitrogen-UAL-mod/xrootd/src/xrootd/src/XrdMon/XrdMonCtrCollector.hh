/*****************************************************************************/
/*                                                                           */
/*                       XrdMonCtrCollectoriver.hh                           */
/*                                                                           */
/* (c) 2005 by the Board of Trustees of the Leland Stanford, Jr., University */
/*                            All Rights Reserved                            */
/*       Produced by Jacek Becla for Stanford University under contract      */
/*              DE-AC02-76SF00515 with the Department of Energy              */
/*****************************************************************************/

// $Id: XrdMonCtrCollector.hh,v 1.3 2005/03/10 06:07:05 becla Exp $

#ifndef XRDMONCTRCOLLECTOR_HH
#define XRDMONCTRCOLLECTOR_HH

// core function for receiving packets, runs in a dedicated thread

extern "C" void* receivePackets(void*);

namespace XrdMonCtrCollector {
    extern int port;
};

#endif /* XRDMONCTRCOLLECTOR_HH */
