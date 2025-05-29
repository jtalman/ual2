/*****************************************************************************/
/*                                                                           */
/*                            XrdMonSndPacket.hh                             */
/*                                                                           */
/* (c) 2005 by the Board of Trustees of the Leland Stanford, Jr., University */
/*                            All Rights Reserved                            */
/*       Produced by Jacek Becla for Stanford University under contract      */
/*              DE-AC02-76SF00515 with the Department of Energy              */
/*****************************************************************************/

// $Id: XrdMonSndPacket.hh,v 1.2 2005/01/29 05:43:47 becla Exp $

#ifndef XRDMONSNDPACKET_HH
#define XRDMONSNDPACKET_HH

#include "XrdMon/XrdMonCommon.hh"
#include "XrdMon/XrdMonTypes.hh"

// an instance of the class describes a packet

class XrdMonSndPacket {
public:
    XrdMonSndPacket();
    XrdMonSndPacket(const XrdMonSndPacket& p);
    ~XrdMonSndPacket();
    
    kXR_int32 size() const { return _size; }
    const char* data() const { return _data; }

    char* offset(kXR_int32 x) { return _data+x; }

    int init(packetlen_t newSize);
    void reset();

private:
    kXR_int32 _size;
    char*   _data;
};
    
#endif /* XRDMONSNDPACKET_HH */
