#ifndef __XRDNETDNS__
#define __XRDNETDNS__
/******************************************************************************/
/*                                                                            */
/*                          X r d N e t D N S . h h                           */
/*                                                                            */
/* (c) 2004 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC02-76-SFO0515 with the Department of Energy              */
/******************************************************************************/
  
//         $Id: XrdNetDNS.hh,v 1.3 2005/01/03 07:48:51 abh Exp $

class XrdNetDNS
{
public:

// Note: Most methods allow the reason for failure to be returned via an errtxt
//       argument. The string returned in errtxt is stiatic and must neither be
//       modified not freed.

// getHostAddr() translates an host name or an ascii host ip address to the
//               binary address suitable for use in network system calls. The
//               host name or address must be registered in the DNS for the
//               translation to be successful. Upon success the either the
//               primary address (1st form) or a list of addresses (2nd form)
//               up to maxipa is returned. The return values are:
//                 0 -> Host name could not be translated, the error text
//                      is placed in errtxt, if an address is supplied.
//               > 0 -> The number of addresses returned.
//
static int getHostAddr(       char     *InetName,
                       struct sockaddr &InetAddr,
                              char    **errtxt=0)
                      {return getHostAddr(InetName, &InetAddr, 1, errtxt);}

static int getHostAddr(       char     *InetName,
                       struct sockaddr  InetAddr[],
                              int       maxipa=1,
                              char    **errtxt=0);

// getHostName() returns the fully qualified name of a host. If no partial
//               host name is specified (or specifiied as 0), the fully
//               qualified name of this host is returned. The name is returned
//               as an strdup'd string which must be released using free().
//               Upon failure, zero is returned and the error text is placed
//               in errtxt if an address is supplied.
//
static char *getHostName(char *InetName=0,
                         char **errtxt=0);

// getHostName() returns the primary name of the host associated with the IP
//               address InetAddr. If a translation is successful, the address
//               of an strdup'd null terminated name is returned (it must be
//               released using free()). Otherwise, 0 id returned and the
//               errnor text is placed in errtxt if an address is supplied.
//
static char *getHostName(struct sockaddr &InetAddr,
                                char    **errtxt=0);

// getHostName() returns the names of the host associated with the IP address
//               InetAddr. The first name is the primary name of the host.
//               Upon success, the address of each null terminated name is
//               placed in InetName[i]. Up to maxipn names are returned. The
//               array must be large enough to hold maxipn entries, Each
//               name is returned as an strdup'd string, which must be 
//               released using free().  Return values are:
//                0 -> No names could be returned; the error text is placed
//                     in errtxt if an address is supplied.
//               >0 -> Number of names returned.
//
static int getHostName(struct sockaddr &InetAddr, 
                              char     *InetName[],
                              int       maxipn,
                              char    **errtxt=0);

// getPort()  returns the port number of the service corresponding to the
//            supplied name and service type (i.e., "tcp" or "udp"). If the port
//            cannot be found, zero is returned and the error text is placed
//            in errtxt if an address is supplied.
//
static int getPort(const char  *servname,
                   const char  *servtype,
                         char **errtxt=0);

// getProtoID() returns the protocol number associated with the protocol name
//              passed as a parameter. No failures can occur since TCP is
//              returned if the protocol cannot be found.
//
static int getProtoID(const char *pname);

// Host2Dest() returns a sockaddr structure suitable for socket operations
//             built from the "host:port" specified in InetName. It returns
//             1 upon success and 0 upon failure with the reason placed in
//             errtxt, if as address is supplied.
//
static int Host2Dest(char            *InetName,
                     struct sockaddr &DestAddr,
                     char           **errtxt=0);

// Host2IP() converts a host name passed in InetName to an IPV4 address,
//           returned in ipaddr (unless it is zero, in which only a conversion
//           check is performed). 1 is returned upon success, 0 upon failure.
//
static int Host2IP(char         *InetName,
                   unsigned int *ipaddr=0);

// IPAddr() returns the IPV4 version of the address in the address argument
//
static unsigned int IPAddr(struct sockaddr *InetAddr);

// isLoopback() returns true if the address in InetAddr is the loopback address.
//              This test is used to discover IP address spoofing in UDP packets.
//
static int isLoopback(struct sockaddr &InetAddr);

// Peername() returns the strdupp'd string name (and optionally the address) of 
//            the host associated with the socket passed as the first parameter. 
//            The string must be released using free(). If the host cannot be
//            determined, 0 is returned and the error text is placed in errtxt
//            if an address is supplied.
//
static char *Peername(       int       snum,
                      struct sockaddr *sap=0,
                             char    **errtxt=0);

// setPort() sets the port number InetAddr. If anyaddr is true,, InetAddr is
//           initialized to the network defined "any" IP address.
//
static void setPort(struct sockaddr &InetAddr, int port, int anyaddr=0);

              XrdNetDNS() {}
             ~XrdNetDNS() {}
 
private:
 
static char *LowCase(char *str);
static int   setET(char **errtxt, int rc);
static int   setETni(char **errtxt, int rc);
};
#endif
