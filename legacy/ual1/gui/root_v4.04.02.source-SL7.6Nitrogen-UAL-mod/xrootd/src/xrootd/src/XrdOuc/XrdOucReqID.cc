/******************************************************************************/
/*                                                                            */
/*                        X r d O u c R e q I D . c c                         */
/*                                                                            */
/* (c) 2004 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*       All Rights Reserved. See XrdInfo.cc for complete License Terms       */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

//         $Id: XrdOucReqID.cc,v 1.5 2005/01/03 07:50:50 abh Exp $

const char *XrdOucReqIDCVSID = "$Id: XrdOucReqID.cc,v 1.5 2005/01/03 07:50:50 abh Exp $";

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <sys/types.h>
  
#include "XrdOucReqID.hh"

/******************************************************************************/
/*                      S t a t i c   V a r i a b l e s                       */
/******************************************************************************/
  
XrdOucMutex  XrdOucReqID::myMutex;
char        *XrdOucReqID::reqFMT;
char        *XrdOucReqID::reqPFX;
int          XrdOucReqID::reqPFXlen = 0;
int          XrdOucReqID::reqNum = 0;

/******************************************************************************/
/*                           C o n s t r u c t o r                            */
/******************************************************************************/
  
XrdOucReqID::XrdOucReqID(int inst, char *myHost, unsigned int myIP)
{
   time_t eNow = time(0);
   char xbuff[256];

   snprintf(xbuff, sizeof(xbuff)-1, "%08x:%04x.%08x:%%d", myIP, inst,
                                    static_cast<unsigned int>(eNow));
   reqFMT = strdup(xbuff);
   reqPFXlen = 13;
   xbuff[reqPFXlen] = '\0';
   reqPFX = strdup(xbuff);
}
 
/******************************************************************************/
/*                                i s M i n e                                 */
/******************************************************************************/
 
int XrdOucReqID::isMine(char *reqid, int &hport, char *hname, int hlen)
{
   unsigned int ipaddr, ipport;
   char *cp, *ep, *ip;

// Determine whether this is our host
//
   if (isMine(reqid)) return 1;

// Not ours, try to tell the caller who it is
//
   if (!hlen) return 0;

// Get the IP address of his id
//
   hport = 0;
// if (!(cp = index((const char *)reqid, int(':'))) || cp-reqid != 8) return 0;
   if (!(ipaddr = strtol((const char *)reqid, &ep, 16)) || ep != cp)  return 0;

// Get the port number
//
   ep++;
// if (!(cp = index((const char *)ep, int('.')))     || cp-ep != 4) return 0;
   if (!(ipport = strtol((const char *)ep, &cp, 16)) || ep != cp)   return 0;

// Format the address and return the port
//
   ip = (char *)&ipaddr;
   snprintf(hname, hlen-1, "%d.%d.%d.%d",
                   (int)ip[0], (int)ip[1], (int)ip[2], (int)ip[3]);
   hname[hlen-1] = '\0';
   hport = (int)ipport;
   return 0;
}
  
/******************************************************************************/
/*                                    I D                                     */
/******************************************************************************/
  
char *XrdOucReqID::ID(char *buff, int blen)
{
   int myNum;

// Get a new sequence number
//
   myMutex.Lock();
   myNum = (reqNum += 1);
   myMutex.UnLock();

// Generate the request id and return it
//
   snprintf(buff, blen-1, reqFMT, myNum);
   return buff;
}

/******************************************************************************/
/*                                 I n d e x                                  */
/******************************************************************************/
  
int XrdOucReqID::Index(int KeyMax, const char *KeyVal, int KeyLen)
{  int j;
   unsigned int *ip, iword, ival = 0;
   const int il = sizeof(ival);

// Get actual key length
//
   if (KeyLen == 0) KeyLen = strlen(KeyVal);

// If name is shorter than the hash length, use the key.
//
   if (KeyLen <= il)
      {memcpy(&ival, KeyVal, (size_t)KeyLen);
       return (int)((ival & INT_MAX) % KeyMax);
      }

// Compute the length of the name and develop starting hash.
//
   ival = KeyLen;
   j = KeyLen % il; KeyLen /= il;
   if (j) 
      {memcpy(&iword, KeyVal, (size_t)il);
       ival ^= iword;
      }
   ip = (unsigned int *)&KeyVal[j];

// Compute and return the index.
//
   while(KeyLen--)
        {memcpy(&iword, ip++, (size_t)il);
         ival ^= iword;
        }
   return (int)((ival & INT_MAX) % KeyMax);
}
