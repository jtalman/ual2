/******************************************************************************/
/*                                                                            */
/*                         X r d N e t W o r k . c c                          */
/*                                                                            */
/* (c) 2004 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*       All Rights Reserved. See XrdInfo.cc for complete License Terms       */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/
 
//         $Id: XrdNetWork.cc,v 1.1 2004/09/14 06:19:03 abh Exp $

const char *XrdNetWorkCVSID = "$Id: XrdNetWork.cc,v 1.1 2004/09/14 06:19:03 abh Exp $";

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "XrdNet/XrdNet.hh"
#include "XrdNet/XrdNetLink.hh"
#include "XrdNet/XrdNetOpts.hh"
#include "XrdNet/XrdNetPeer.hh"
#include "XrdNet/XrdNetWork.hh"

#include "XrdOuc/XrdOucError.hh"

/******************************************************************************/
/*                                A c c e p t                                 */
/******************************************************************************/

XrdNetLink *XrdNetWork::Accept(int opts, int timeout)
{
   XrdNetPeer myPeer;
   XrdNetLink    *lp;
   int ismyfd, lnkopts;

// Perform regular accept
//
   if (!XrdNet::Accept(myPeer, opts, timeout)) return (XrdNetLink *)0;
   if ((ismyfd = (myPeer.fd == iofd))) lnkopts = XRDNETLINK_NOCLOSE;
      else lnkopts = 0;

// Return a link object
//
   if (!(lp = XrdNetLink::Alloc(eDest, myPeer, BuffQ, lnkopts)))
      {if (!ismyfd) close(myPeer.fd);
       if (!(opts & XRDNET_NOEMSG))
          eDest->Emsg("Connect",ENOMEM,"accept connection from",myPeer.InetName);
      } else myPeer.InetBuff = 0; // Keep buffer after object goes away
   return lp;
}

/******************************************************************************/
/*                               C o n n e c t                                */
/******************************************************************************/

XrdNetLink *XrdNetWork::Connect(char *host, int port, int opts, int tmo)
{
   XrdNetPeer myPeer;
   XrdNetLink *lp;

// Try to do a connect
//
   if (!XrdNet::Connect(myPeer, host, port, opts, tmo)) return (XrdNetLink *)0;

// Return a link object
//
   if (!(lp = XrdNetLink::Alloc(eDest, myPeer, BuffQ)))
      {close(myPeer.fd);
       if (!(opts & XRDNET_NOEMSG))
          eDest->Emsg("Connect", ENOMEM, "connect to", host);
      }
   return lp;
}

/******************************************************************************/
/*                                 R e l a y                                  */
/******************************************************************************/
  
XrdNetLink *XrdNetWork::Relay(char *dest, int opts)
{
   XrdNetPeer myPeer;
   XrdNetLink    *lp;
   int            lnkopts;

// Create a udp socket
//
   if (!XrdNet::Connect(myPeer, dest, -1, opts | XRDNET_UDPSOCKET))
      return (XrdNetLink *)0;

// Determine set of options
//
   lnkopts = (myPeer.fd == iofd      ? XRDNETLINK_NOCLOSE  : 0)
           | (opts & XRDNET_SENDONLY ? XRDNETLINK_NOSTREAM : 0);

// Associate this socket with a link
//
   if (!(lp = XrdNetLink::Alloc(eDest, myPeer, BuffQ, lnkopts)))
      {close(myPeer.fd);
       if (!(opts & XRDNET_NOEMSG))
          eDest->Emsg("Connect", ENOMEM, "allocate relay to", 
                     (dest ? dest : (char *)"network"));
      }
   return lp;
}
