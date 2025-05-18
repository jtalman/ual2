/******************************************************************************/
/*                                                                            */
/*                  X r d X r o o t d R e s p o n s e . c c                   */
/*                                                                            */
/* (c) 2004 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*       All Rights Reserved. See XrdInfo.cc for complete License Terms       */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

//      $Id: XrdXrootdResponse.cc,v 1.7 2005/02/01 00:29:20 becla Exp $

const char *XrdXrootdResponseCVSID = "$Id: XrdXrootdResponse.cc,v 1.7 2005/02/01 00:29:20 becla Exp $";
 
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <string.h>

#include "Xrd/XrdLink.hh"
#include "XrdXrootd/XrdXrootdResponse.hh"
#include "XrdXrootd/XrdXrootdTrace.hh"
  
/******************************************************************************/
/*                               G l o b a l s                                */
/******************************************************************************/
  
extern XrdOucTrace *XrdXrootdTrace;

const char *XrdXrootdResponse::TraceID = "Response";

/******************************************************************************/
/*                         L o c a l   D e f i n e s                          */
/******************************************************************************/

#define TRACELINK Link
  
/******************************************************************************/
/*                                  P u s h                                   */
/******************************************************************************/

int XrdXrootdResponse::Push(void *data, int dlen)
{
    kXR_int32 DLen = static_cast<kXR_int32>(htonl(dlen));
    RespIO[1].iov_base = (caddr_t)&DLen;
    RespIO[1].iov_len  = sizeof(dlen);
    RespIO[2].iov_base = (caddr_t)data;
    RespIO[2].iov_len  = dlen;

    TRACES(RSP, "pushing " <<dlen <<" data bytes");

    if (Link->Send(&RespIO[1], 2, sizeof(kXR_int32) + dlen) < 0)
       return Link->setEtext("send failure");
    return 0;
}

int XrdXrootdResponse::Push()
{
    static int null = 0;
    TRACES(RSP, "pushing " <<sizeof(kXR_int32) <<" data bytes");
    if (Link->Send((char *)&null, sizeof(kXR_int32)) < 0)
       return Link->setEtext("send failure");
    return 0;
}

/******************************************************************************/
/*                                  S e n d                                   */
/******************************************************************************/

int XrdXrootdResponse::Send()
{

    Resp.status = static_cast<kXR_unt16>(htons(kXR_ok));
    Resp.dlen   = 0;
    TRACES(RSP, "sending OK");

    if (Link->Send((char *)&Resp, sizeof(Resp)) < 0)
       return Link->setEtext("send failure");
    return 0;
}

/******************************************************************************/

int XrdXrootdResponse::Send(const char *msg)
{

    Resp.status        = static_cast<kXR_unt16>(htons(kXR_ok));
    RespIO[1].iov_base = (caddr_t)msg;
    RespIO[1].iov_len  = strlen(msg)+1;
    Resp.dlen          = static_cast<kXR_int32>(htonl(RespIO[1].iov_len));
    TRACES(RSP, "sending OK: " <<msg);

    if (Link->Send(RespIO, 2, sizeof(Resp) + RespIO[1].iov_len) < 0)
       return Link->setEtext("send failure");
    return 0;
}

/******************************************************************************/

int XrdXrootdResponse::Send(XResponseType rcode, void *data, int dlen)
{

    Resp.status        = static_cast<kXR_unt16>(htons(rcode));
    RespIO[1].iov_base = (caddr_t)data;
    RespIO[1].iov_len  = dlen;
    Resp.dlen          = static_cast<kXR_int32>(htonl(dlen));
    TRACES(RSP, "sending " <<dlen <<" data bytes; status=" <<rcode);

    if (Link->Send(RespIO, 2, sizeof(Resp) + dlen) < 0)
       return Link->setEtext("send failure");
    return 0;
}

/******************************************************************************/

int XrdXrootdResponse::Send(XResponseType rcode, int info, char *data)
{
    kXR_int32 xbuf = static_cast<kXR_int32>(htonl(info));
    int dlen;

    Resp.status        = static_cast<kXR_unt16>(htons(rcode));
    RespIO[1].iov_base = (caddr_t)(&xbuf);
    RespIO[1].iov_len  = sizeof(xbuf);
    RespIO[2].iov_base = (caddr_t)data;
    RespIO[2].iov_len  = dlen = strlen(data);
    Resp.dlen          = static_cast<kXR_int32>(htonl((dlen+sizeof(xbuf))));

    TRACES(RSP, "sending " <<dlen <<" data bytes; status=" <<rcode);

    if (Link->Send(RespIO, 3, sizeof(Resp) + dlen) < 0)
       return Link->setEtext("send failure");
    return 0;
}

/******************************************************************************/

int XrdXrootdResponse::Send(void *data, int dlen)
{

    Resp.status        = static_cast<kXR_unt16>(htons(kXR_ok));
    RespIO[1].iov_base = (caddr_t)data;
    RespIO[1].iov_len  = dlen;
    Resp.dlen          = static_cast<kXR_int32>(htonl(dlen));
    TRACES(RSP, "sending " <<dlen <<" data bytes; status=0");

    if (Link->Send(RespIO, 2, sizeof(Resp) + dlen) < 0)
       return Link->setEtext("send failure");
    return 0;
}

/******************************************************************************/

int XrdXrootdResponse::Send(struct iovec *IOResp, int iornum, int iolen)
{
    int i, dlen = 0;

    if (iolen < 0) for (i = 1; i < iornum; i++) dlen += IOResp[i].iov_len;
       else dlen = iolen;

    Resp.status        = static_cast<kXR_unt16>(htons(kXR_ok));
    IOResp[0].iov_base = RespIO[0].iov_base;
    IOResp[0].iov_len  = RespIO[0].iov_len;
    Resp.dlen          = static_cast<kXR_int32>(htonl(dlen));
    TRACES(RSP, "sending " <<dlen <<" data bytes; status=0");

    if (Link->Send(IOResp, iornum, sizeof(Resp) + dlen) < 0)
       return Link->setEtext("send failure");
    return 0;
}

/******************************************************************************/

int XrdXrootdResponse::Send(XErrorCode ecode, const char *msg)
{
    int dlen;
    kXR_int32 erc = static_cast<kXR_int32>(htonl(ecode));

    Resp.status        = static_cast<kXR_unt16>(htons(kXR_error));
    RespIO[1].iov_base = (char *)&erc;
    RespIO[1].iov_len  = sizeof(erc);
    RespIO[2].iov_base = (caddr_t)msg;
    RespIO[2].iov_len  = strlen(msg)+1;
                dlen   = sizeof(erc) + RespIO[2].iov_len;
    Resp.dlen          = static_cast<kXR_int32>(htonl(dlen));
    TRACES(EMSG, "sending err " <<ecode <<": " <<msg);

    if (Link->Send(RespIO, 3, sizeof(Resp) + dlen) < 0)
       return Link->setEtext("send failure");
    return 0;
}
 

/******************************************************************************/
/*                                   S e t                                    */
/******************************************************************************/

void XrdXrootdResponse::Set(unsigned char *stream)
{
   static char hv[] = "0123456789abcdef";
   char *outbuff;
   int i;

   Resp.streamid[0] = stream[0];
   Resp.streamid[1] = stream[1];

   if (TRACING(TRACE_REQ|TRACE_RSP))
      {outbuff = trsid;
       for (i = 0; i < (int)sizeof(Resp.streamid); i++)
           {*outbuff++ = hv[(stream[i] >> 4) & 0x0f];
            *outbuff++ = hv[ stream[i]       & 0x0f];
            }
       *outbuff++ = ' '; *outbuff = '\0';
      }
}
