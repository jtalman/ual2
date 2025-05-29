/******************************************************************************/
/*                                                                            */
/*                       X r d S e c C l i e n t . c c                        */
/*                                                                            */
/* (c) 2003 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

//       $Id: XrdSecClient.cc,v 1.5 2005/03/01 05:58:41 abh Exp $

const char *XrdSecClientCVSID = "$Id: XrdSecClient.cc,v 1.5 2005/03/01 05:58:41 abh Exp $";

#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "XrdOuc/XrdOucErrInfo.hh"
#include "XrdOuc/XrdOucPthread.hh"
#include "XrdSec/XrdSecPManager.hh"
#include "XrdSec/XrdSecInterface.hh"

/******************************************************************************/
/*                 M i s c e l l a n e o u s   D e f i n e s                  */
/******************************************************************************/

#define DEBUG(x) {if (DebugON) std::cerr <<"sec_Client: " <<x <<std::endl;}

/******************************************************************************/
/*                         L o c a l   C l a s s e s                          */
/******************************************************************************/
  
class XrdSecProtNone : public XrdSecProtocol
{
public:
int                Authenticate  (XrdSecCredentials  *cred,
                                  XrdSecParameters  **parms,
                                  XrdOucErrInfo      *einfo=0) 
                                 {return 0;}

XrdSecCredentials *getCredentials(XrdSecParameters  *parm=0,       // In
                                  XrdOucErrInfo     *einfo=0)
                                 {return new XrdSecCredentials();}

void               Delete() {}  // Never deleted because it's static!

              XrdSecProtNone() {}
             ~XrdSecProtNone() {}
};
  
/******************************************************************************/
/*                     X r d S e c G e t P r o t o c o l                      */
/******************************************************************************/

// This function is only invoked by the client. It exists in the top level
// shared library that interposes between all other protocol shared libraries.
//
extern "C"
{
XrdSecProtocol *XrdSecGetProtocol(const char             *hostname,
                                  const struct sockaddr  &netaddr,
                                  const XrdSecParameters &parms,
                                        XrdOucErrInfo    *einfo)
{
   static int DebugON = (getenv("XrdSecDEBUG") ? 1 : 0);
   static XrdSecProtNone ProtNone;
   static XrdSecPManager PManager(DebugON);
   const char *noperr = "XrdSec: No authentication protocols are available.";

   char sectoken[4096];
   int i;
   XrdSecProtocol *protp;

// Perform any required debugging
//
   DEBUG("protocol request for host " <<hostname <<" token='"
         <<(parms.size ? parms.buffer : "") <<"'");

// Check if the server wants no security.
//
   if (!parms.size || !parms.buffer[0]) return (XrdSecProtocol *)&ProtNone;

// Copy the string into a local buffer so that we can simplify some comparisons
// and isolate ourselves from server protocol errors.
//
   if (parms.size < (int)sizeof(sectoken)) i = parms.size;
      else i = sizeof(sectoken)-1;
   strncpy(sectoken, parms.buffer, i);
   sectoken[i] = '\0';

// Find a supported protocol.
//
   if (!(protp = PManager.Get(hostname, netaddr, sectoken)))
      if (einfo) einfo->setErrInfo(ENOPROTOOPT, noperr);
         else std::cerr <<noperr <<std::endl;

// All done
//
   return protp;
}
}
