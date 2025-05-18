/******************************************************************************/
/*                                                                            */
/*                          X r d N e t D N S . c c                           */
/*                                                                            */
/* (c) 2004 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC02-76-SFO0515 with the Department of Energy              */
/******************************************************************************/
  
//         $Id: XrdNetDNS.cc,v 1.5 2005/03/01 21:09:21 abh Exp $

const char *XrdNetDNSCVSID = "$Id: XrdNetDNS.cc,v 1.5 2005/03/01 21:09:21 abh Exp $";

#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

#include "XrdNet/XrdNetDNS.hh"
#include "XrdOuc/XrdOucPlatform.hh"
#include "XrdOuc/XrdOucPthread.hh"
  
/******************************************************************************/
/*                           g e t H o s t A d d r                            */
/******************************************************************************/
  
int XrdNetDNS::getHostAddr(       char     *InetName,
                           struct sockaddr  InetAddr[],
                                  int       maxipa,
                                  char    **errtxt)
{
#ifdef HAS_NAMEINFO
   struct addrinfo   *rp, *np, *pnp=0;
   struct addrinfo    myhints = {AI_CANONNAME};
#else
   unsigned int addr;
   struct hostent hent, *hp;
   char **p, hbuff[1024];
#endif
   struct sockaddr_in *ip;
   int i, rc;

// Make sure we have something to lookup here
//
    if (!InetName || !InetName[0])
       {ip = (struct sockaddr_in *)&InetAddr[0];
        ip->sin_family      = AF_INET;
        ip->sin_port        = 0;
        ip->sin_addr.s_addr = INADDR_ANY;
        memset((void *)ip->sin_zero, 0, sizeof(ip->sin_zero));
        return 1;
       }

// Determine how we will resolve the name
//
   rc = 0;
#ifndef HAS_NAMEINFO
    if (!isdigit((int)*InetName))
#ifdef __sun
       gethostbyname_r((const char *)InetName,
                       &hent, hbuff, sizeof(hbuff),      &rc);
#else
       gethostbyname_r((const char *)InetName,
                       &hent, hbuff, sizeof(hbuff), &hp, &rc);
#endif
       else if ((int)(addr = inet_addr(InetName)) == -1)
               return (errtxt ? setET(errtxt, EINVAL) : 0);
#ifdef __sun
               else gethostbyaddr_r((const char *)&addr,sizeof(addr),
                                    AF_INET, &hent,
                                    hbuff, sizeof(hbuff),      &rc);
#else
               else gethostbyaddr_r((const char *)&addr,sizeof(addr),
                                    AF_INET, &hent,
                                    hbuff, sizeof(hbuff), &hp, &rc);
#endif
    if (rc) return (errtxt ? setET(errtxt, rc) : 0);

// Check if we resolved the name
//
   for (i = 0, p = hent.h_addr_list; *p != 0 && i < maxipa; p++, i++)
       {ip = (struct sockaddr_in *)&InetAddr[i];
        memcpy((void *)&(ip->sin_addr), (const void *)*p, sizeof(ip->sin_addr));
        ip->sin_port   = 0;
        ip->sin_family = hent.h_addrtype;
        memset((void *)ip->sin_zero, 0, sizeof(ip->sin_zero));
       }

#else

// Translate the name to an address list
//
    if (isdigit((int)*InetName)) myhints.ai_flags |= AI_NUMERICHOST;
    rc = getaddrinfo((const char *)InetName,0,(const addrinfo *)&myhints, &rp);
    if (rc || !(np = rp)) return (errtxt ? setETni(errtxt, rc) : 0);

// Return all of the addresses. On some platforms (like linux) this function is
// brain-dead in that it returns all addreses assoacted with all aliases even
// when those addresses are duplicates.
//
   i = 0;
   do {if (!pnp
       ||  memcmp((const void *)pnp->ai_addr, (const void *)np->ai_addr,
                  sizeof(struct sockaddr)))
           memcpy((void *)&InetAddr[i++], (const void *)np->ai_addr,
                  sizeof(struct sockaddr));
       pnp = np; np = np->ai_next;
      } while(i < maxipa && np);
   freeaddrinfo(rp);

#endif

// All done
//
   return i;
}
 
/******************************************************************************/
/*               g e t H o s t N a m e   ( V a r i a n t   1 )                */
/******************************************************************************/

char *XrdNetDNS::getHostName(char *InetName, char **errtxt)
{
   char myname[256], *hp;
   struct sockaddr InetAddr;
  
// Identify ourselves if we don't have a passed hostname
//
   if (InetName) hp = InetName;
      else if (gethostname(myname, sizeof(myname))) 
              {if (errtxt) setET(errtxt, errno); return (char *)0;}
              else hp = myname;

// Get the address
//
   if (!getHostAddr(hp, InetAddr, errtxt)) return 0;

// Convert it to a fully qualified host name and return it
//
   return getHostName(InetAddr, errtxt);
}

/******************************************************************************/
/*               g e t H o s t N a m e   ( V a r i a n t   2 )                */
/******************************************************************************/

char *XrdNetDNS::getHostName(struct sockaddr &InetAddr, char **errtxt)
{
     char *result;
     if (getHostName(InetAddr, &result, 1, errtxt)) return result;
     return(char *)0;
}

/******************************************************************************/
/*               g e t H o s t N a m e   ( V a r i a n t   3 )                */
/******************************************************************************/
  
int XrdNetDNS::getHostName(struct sockaddr &InetAddr,
                                  char     *InetName[],
                                  int       maxipn,
                                  char    **errtxt)
{
   char mybuff[256];
   int i, rc;

// Some platforms have nameinfo but getnameinfo() is broken. If so, we revert
// to using the gethostbyaddr().
//
#if defined(HAS_NAMEINFO) && !defined(__macos__)
    struct addrinfo   *rp, *np;
    struct addrinfo    myhints = {AI_CANONNAME};
#elif defined(HAS_GETHBYXR)
   struct sockaddr_in *ip = (sockaddr_in *)&InetAddr;
   struct hostent hent, *hp;
   char *hname, hbuff[1024];
#else
   static XrdOucMutex getHN;
          XrdOucMutexHelper getHNhelper;
   struct sockaddr_in *ip = (sockaddr_in *)&InetAddr;
   struct hostent *hp;
   unsigned int ipaddr;
   char *hname;
#endif

// Make sure we can return something
//
   if (maxipn < 1) return (errtxt ? setET(errtxt, EINVAL) : 0);

#if !defined(HAS_NAMEINFO) || defined(__macos__)

// Convert it to a host name
//
   rc = 0;
#ifdef HAS_GETHBYXR
#ifdef __sun
   gethostbyaddr_r((const char *)&(ip->sin_addr), sizeof(struct in_addr),
                   AF_INET, &hent, hbuff, sizeof(hbuff),      &rc);
   hp = &hent;
#else
   gethostbyaddr_r((const char *)&(ip->sin_addr), sizeof(struct in_addr),
                   AF_INET, &hent, hbuff, sizeof(hbuff), &hp, &rc);
#endif
#else
   memcpy(&ipaddr, &ip->sin_addr, sizeof(ipaddr));
   getHNhelper.Lock(&getHN);
   if (!(hp=gethostbyaddr((const char *)&ipaddr, sizeof(InetAddr), AF_INET)))
      rc = (h_errno ? h_errno : EINVAL);
#endif

   if (rc)
      {hname = (char *)inet_ntop(ip->sin_family,
                                 (const void *)(&ip->sin_addr),
                                 mybuff, sizeof(mybuff));
       if (!hname) return (errtxt ? setET(errtxt, errno) : 0);
       InetName[0] = strdup(hname);
       return 1;
      }

// Return first result
//
   InetName[0] = LowCase(strdup(hp->h_name));

// Return additional names
//
   hname = *hp->h_aliases;
   for (i = 1; i < maxipn && hname; i++, hname++)
       InetName[i] = LowCase(strdup(hname));
#else

// Do lookup of canonical name. We can't use getaddrinfo() for this on all
// platforms because AI_CANONICAL was never well defined in the spec.
//
   if ((rc = getnameinfo(&InetAddr, sizeof(struct sockaddr),
                        mybuff, sizeof(mybuff), 0, 0, 0)))
      return (errtxt ? setETni(errtxt, rc) : 0);

// Return the result if aliases not wanted
//
   if (maxipn < 2)
      {InetName[0] = LowCase(strdup(mybuff));
       return 1;
      }

// Get the aliases for this name
//
    rc = getaddrinfo((const char *)mybuff,0,(const addrinfo *)&myhints, &rp);
    if (rc || !(np = rp)) return (errtxt ? setETni(errtxt, rc) : 0);

// Return all of the names
//
   for (i = 0; i < maxipn && np; i++)
       {InetName[i] = LowCase(strdup(np->ai_canonname));
        np = np->ai_next;
       }
   freeaddrinfo(rp);

#endif

// All done
//
   return i;
}
 
/******************************************************************************/
/*                               g e t P o r t                                */
/******************************************************************************/
  
int XrdNetDNS::getPort(const char  *servname,
                       const char  *servtype,
                             char **errtxt)
{
   int rc;

#ifdef HAS_NAMEINFO
    struct addrinfo   *rp, *np;
    struct addrinfo   myhints = {0};
    int portnum = 0;
#else
   struct servent sent, *sp;
   char sbuff[1024];
#endif

// Try to find minimum port number
//
#ifndef HAS_NAMEINFO
#ifdef __sun
   if (   !getservbyname_r(servname,servtype,&sent,sbuff,sizeof(sbuff)))
      return (errtxt ? setET(errtxt, errno) : 0);
#else
   if ((rc=getservbyname_r(servname,servtype,&sent,sbuff,sizeof(sbuff),&sp)))
      return (errtxt ? setET(errtxt, rc) : 0);
#endif
   return int(ntohs(sent.s_port));
#else
   if ((rc = getaddrinfo(0,servname,(const struct addrinfo *)&myhints,&rp))
   || !(np = rp)) return (errtxt ? setETni(errtxt, rc) : 0);

   while(np)      if (np->ai_socktype == SOCK_STREAM && *servtype == 't') break;
             else if (np->ai_socktype == SOCK_DGRAM  && *servtype == 'u') break;
             else np = np->ai_next;
   if (np) portnum=int(ntohs(((struct sockaddr_in *)(np->ai_addr))->sin_port));
   freeaddrinfo(rp);
   if (!portnum) return (errtxt ? setET(errtxt, ESRCH) : 0);
   return portnum;
#endif
}
 
/******************************************************************************/
/*                            g e t P r o t o I D                             */
/******************************************************************************/

#define NET_IPPROTO_TCP 6

int XrdNetDNS::getProtoID(const char *pname)
{
#ifdef HAS_PROTOR
    struct protoent pp;
    char buff[1024];
#else
    static XrdOucMutex protomutex;
    struct protoent *pp;
    int    protoid;
#endif

// Note that POSIX did include getprotobyname_r() in the last minute. Many
// platforms do not document this variant but just about all include it.
//
#ifdef __sun
    if (!getprotobyname_r(pname, &pp, buff, sizeof(buff))) 
       return NET_IPPROTO_TCP;
    return pp.p_proto;
#elif !defined(HAS_PROTOR)
    protomutex.Lock();
    if (!(pp = getprotobyname(pname))) protoid = NET_IPPROTO_TCP;
       else protoid = pp->p_proto;
    protomutex.UnLock();
    return protoid;
#else
    struct protoent *ppp;
    if (getprotobyname_r(pname, &pp, buff, sizeof(buff), &ppp))
       return NET_IPPROTO_TCP;
    return pp.p_proto;
#endif
}
  
/******************************************************************************/
/*                             H o s t 2 D e s t                              */
/******************************************************************************/
  
int XrdNetDNS::Host2Dest(char            *hostname,
                         struct sockaddr &DestAddr,
                         char           **errtxt)
{ char *cp;
  int port;
  struct sockaddr_in InetAddr;

// Find the colon in the host name
//
/*
   if (!(cp = index((const char *)hostname, (int)':'))) 
       {if (errtxt) *errtxt = (char *)"port not specified";
        return 0;
       }
*/
   *cp = '\0';

// Convert hostname to an ascii ip address
//
   if (!getHostAddr(hostname, (struct sockaddr &)InetAddr, errtxt))
      {*cp = ':'; return 0;}

// Insert port number in address
//
   if (!(port = atoi((const char *)cp+1)) || port > 0xffff)
      {if (errtxt) *errtxt = (char *)"invalid port number";
       *cp = ':'; 
       return 0;
      }

// Compose the destination address
//
   *cp = ':';
   InetAddr.sin_family = AF_INET;
   InetAddr.sin_port = htons(port);
   memcpy((void *)&DestAddr, (const void *)&InetAddr, sizeof(sockaddr));
   memset((void *)&InetAddr.sin_zero, 0, sizeof(InetAddr.sin_zero));
   return 1;
}

/******************************************************************************/
/*                               H o s t 2 I P                                */
/******************************************************************************/
  
int XrdNetDNS::Host2IP(char *hname, unsigned int *ipaddr)
{
   struct sockaddr_in InetAddr;

// Convert hostname to an ascii ip address
//
   if (!getHostAddr(hname, (struct sockaddr &)InetAddr)) return 0;
   if (ipaddr) memcpy(ipaddr, &InetAddr.sin_addr, sizeof(unsigned int));
   return 1;
}
 
/******************************************************************************/
/*                                I P A d d r                                 */
/******************************************************************************/
  
unsigned int XrdNetDNS::IPAddr(struct sockaddr *InetAddr)
  {return (unsigned int)(((struct sockaddr_in *)InetAddr)->sin_addr.s_addr);}

/******************************************************************************/
/*                            i s L o o p b a c k                             */
/******************************************************************************/
  
int XrdNetDNS::isLoopback(struct sockaddr &InetAddr)
{
   struct sockaddr_in *ip = (struct sockaddr_in *)&InetAddr;
   return ip->sin_addr.s_addr == 0x7f000001;
}

/******************************************************************************/
/*                              P e e r n a m e                               */
/******************************************************************************/
  
char *XrdNetDNS::Peername(int snum, struct sockaddr *sap, char **errtxt)
{
   struct sockaddr addr;
   SOCKLEN_t addrlen = sizeof(addr);

// Get the address on the other side of this socket
//
   if (!sap) sap = &addr;
   if (getpeername(snum, (struct sockaddr *)sap, &addrlen) < 0)
      {if (errtxt) setET(errtxt, errno);
       return (char *)0;
      }

// Convert it to a host name
//
   return getHostName(*sap, errtxt);
}

/******************************************************************************/
/*                               s e t P o r t                                */
/******************************************************************************/
  
void XrdNetDNS::setPort(struct sockaddr &InetAddr, int port, int anyaddr)
{
   unsigned short int sport = static_cast<unsigned short int>(port);
   struct sockaddr_in *ip = (struct sockaddr_in *)&InetAddr;
   ip->sin_port = htons(sport);
   if (anyaddr) {ip->sin_family = AF_INET;
                 ip->sin_addr.s_addr = INADDR_ANY;
                 memset((void *)ip->sin_zero, 0, sizeof(ip->sin_zero));
                }
}

/******************************************************************************/
/*                       P r i v a t e   M e t h o d s                        */
/******************************************************************************/
/******************************************************************************/
/*                               L o w C a s e                                */
/******************************************************************************/
  
char *XrdNetDNS::LowCase(char *str)
{
   char *sp = str;

   while(*sp) {if (isupper((int)*sp)) *sp = (char)tolower((int)*sp); sp++;}

   return str;
}
 
/******************************************************************************/
/*                                 s e t E T                                  */
/******************************************************************************/
  
int XrdNetDNS::setET(char **errtxt, int rc)
{
    if (rc) *errtxt = strerror(rc);
       else *errtxt = (char *)"unexpected error";
    return 0;
}
 
/******************************************************************************/
/*                               s e t E T n i                                */
/******************************************************************************/
  
int XrdNetDNS::setETni(char **errtxt, int rc)
{
#ifndef HAS_NAMEINFO
    return setET(errtxt, rc);
#else
    if (rc) *errtxt = (char *)gai_strerror(rc);
       else *errtxt = (char *)"unexpected error";
    return 0;
#endif
}
