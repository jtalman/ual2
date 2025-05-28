/*****************************************************************************/
/*                                                                           */
/*                              XrdMonCommon.hh                              */
/*                                                                           */
/* (c) 2005 by the Board of Trustees of the Leland Stanford, Jr., University */
/*                            All Rights Reserved                            */
/*       Produced by Jacek Becla for Stanford University under contract      */
/*              DE-AC02-76SF00515 with the Department of Energy              */
/*****************************************************************************/

// $Id: XrdMonCommon.hh,v 1.9 2005/03/10 06:06:59 becla Exp $

#ifndef XRDMONCOMMON_HH
#define XRDMONCOMMON_HH

#include "XrdMon/XrdMonTypes.hh"

// common settings for UDP transmitter and receiver

const kXR_int32 MAXPACKETSIZE  = 65536; // [bytes], (16 bits for length in hdr)
const kXR_int16 HDRLEN         =     8; // [bytes]
const kXR_int16 TRACEELEMLEN   =    16; // [bytes]

// size for data inside packet. 2*kXR_int16 is used
// by packet type and number of elements
const kXR_int16 TRACELEN       =    16;

extern const char* DEFAULT_HOST;
extern const int   DEFAULT_PORT;

const kXR_char PACKET_TYPE_ADMIN = 'A';
const kXR_char PACKET_TYPE_DICT  = 'd';
const kXR_char PACKET_TYPE_INFO  = 'i';
const kXR_char PACKET_TYPE_TRACE = 't';
const kXR_char PACKET_TYPE_USER  = 'u';

const char XROOTD_MON_RWREQUESTMASK = 0x80;
// why 0x80: anything that is < 0x7f is rwrequest
// 0x7f = 01111111, so !(x & 10000000), 1000 0000=0x80

enum AdminCommand {
    c_shutdown = 1000
};

enum {
    INVALID_SENDER_ID = 65535
};
#endif /* XRDMONCOMMON_HH */
