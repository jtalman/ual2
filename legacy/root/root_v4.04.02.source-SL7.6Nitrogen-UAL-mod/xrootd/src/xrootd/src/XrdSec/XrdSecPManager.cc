/******************************************************************************/
/*                                                                            */
/*                     X r d S e c P M a n a g e r . c c                      */
/*                                                                            */
/* (c) 2003 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

//       $Id: XrdSecPManager.cc,v 1.7 2005/03/11 12:00:13 ganis Exp $

const char *XrdSecPManagerCVSID = "$Id: XrdSecPManager.cc,v 1.7 2005/03/11 12:00:13 ganis Exp $";

// Bypass Solaris ELF madness
//
#if (defined(SUNCC) || defined(SUN)) 
#include <sys/isa_defs.h>
#if defined(_ILP32) && (_FILE_OFFSET_BITS != 32)
#undef  _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 32
#undef  _LARGEFILE_SOURCE
#endif
#endif

#include <dlfcn.h>
#if !defined(__macos__) && !defined(__CYGWIN__)
#include <link.h>
#endif
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <iostream>
  
#include "XrdSec/XrdSecInterface.hh"
#include "XrdSec/XrdSecPManager.hh"
#include "XrdSec/XrdSecProtocolhost.hh"
#include "XrdOuc/XrdOucErrInfo.hh"

/******************************************************************************/
/*                 M i s c e l l a n e o u s   D e f i n e s                  */
/******************************************************************************/

#define DEBUG(x) {if (DebugON) std::cerr <<"sec_PM: " <<x <<std::endl;}
  
/******************************************************************************/
/*                         L o c a l   C l a s s e s                          */
/******************************************************************************/
  
class XrdSecProtList
{
public:

XrdSecPMask_t    protnum;
char             protid[XrdSecPROTOIDSIZE+1];
char            *protargs;
XrdSecProtocol  *(*ep)(PROTPARMS);
XrdSecProtList  *Next;

                XrdSecProtList(const char *pid, const char *parg)
                      {strncpy(protid, pid, sizeof(protid)-1);
                       protid[XrdSecPROTOIDSIZE] = '\0'; ep = 0; Next = 0;
                       protargs = (parg ? strdup(parg): (char *)"");
                      }
               ~XrdSecProtList() {} // ProtList objects never get freed!
};

/******************************************************************************/
/*                X r d S e c P M a n a g e r   M e t h o d s                 */
/******************************************************************************/
/******************************************************************************/
/*                                  F i n d                                   */
/******************************************************************************/
  
XrdSecPMask_t XrdSecPManager::Find(const char *pid, char **parg)
{
   XrdSecProtList *plp;

   if ((plp = Lookup(pid)))
      {if (parg) *parg = plp->protargs;
       return plp->protnum;
      }
   return 0;
}

/******************************************************************************/
/*                                   G e t                                    */
/******************************************************************************/

XrdSecProtocol *XrdSecPManager::Get(const char     *hname,
                                    const sockaddr &netaddr,
                                    const char     *pname,
                                    XrdOucErrInfo  *erp)
{
   XrdSecProtList *pl;
   char *msgv[2];

// Find the protocol and get an instance of the protocol object
//
   if ((pl = Lookup(pname)))
      {DEBUG("Using " <<pname <<" protocol, args='"
              <<(pl->protargs ? pl->protargs : "") <<"'");
       return pl->ep('s', hname, netaddr, 0, erp);
      }

// Protocol is not supported
//
   msgv[0] = (char *)pname;
   msgv[1] = (char *)" security protocol is not supported.";
   erp->setErrInfo(EPROTONOSUPPORT, msgv, 2);
   return 0;
}

XrdSecProtocol *XrdSecPManager::Get(const char     *hname,
                                    const sockaddr &netaddr,
                                    char           *sectoken)
{
   char *nscan, *pname, *pargs, *bp = sectoken;
   XrdSecProtList *pl;
   XrdSecProtocol *pp;
   XrdOucErrInfo   erp;

// Find a protocol marker in the info block and check if acceptable
//
   while(*bp)
        {if (*bp != '&') {bp++; continue;}
            else if (!*(++bp) || *bp != 'P' || !*(++bp) || *bp != '=') continue;
         bp++; pname = bp; pargs = 0;
         while(*bp && *bp != ',' && *bp != '&') bp++;
         if (!*bp) nscan = 0;
            else {if (*bp == '&') {*bp = '\0'; pargs = 0; nscan = bp;}
                     else {*bp = '\0'; pargs = ++bp;
                           while (*bp && *bp != '&') bp++;
                           if (*bp) {*bp ='\0'; nscan = bp;}
                              else nscan = 0;
                          }
                  }
         if ((pl = Lookup(pname)) || (pl = ldPO(&erp, 'c', pname)))
            {DEBUG("Using " <<pname <<" protocol, args='"
                   <<(pargs ? pargs : "") <<"'");
             if ((pp = pl->ep('c', hname, netaddr, pargs, &erp))) return pp;
            }
         if (erp.getErrInfo() != ENOENT)
            std::cerr <<erp.getErrText() <<std::endl;
         if (!nscan) break;
         *nscan = '&'; bp = nscan;
         }
    return (XrdSecProtocol *)0;
}
 
/******************************************************************************/
/*                       P r i v a t e   M e t h o d s                        */
/******************************************************************************/
/******************************************************************************/
/*                                   A d d                                    */
/******************************************************************************/
  
XrdSecProtList *XrdSecPManager::Add(XrdOucErrInfo  *eMsg, const char *pid,
                                    XrdSecProtocol *(*ep)(PROTPARMS),
                                    const char *parg)
{
   XrdSecProtList *plp;

// Make sure we did not overflow the protocol stack
//
   if (!protnum)
      {eMsg->setErrInfo(-1, "XrdSec: Too many protocols defined.");
       return 0;
      }

// Add this protocol to our protocol stack
//
   plp = new XrdSecProtList((char *)pid, parg);
   plp->ep = ep;
   myMutex.Lock();
   if (Last) {Last->Next = plp; Last = plp;}
      else First = Last = plp;
   plp->protnum = protnum; 
   if (protnum & 0x40000000) protnum = 0;
      else protnum = protnum<<1;
   myMutex.UnLock();

// All went well
//
   return plp;
}

/******************************************************************************/
/*                                  l d P O                                   */
/******************************************************************************/

#define INITPARMS const char, const char *, XrdOucErrInfo *
  
XrdSecProtList *XrdSecPManager::ldPO(XrdOucErrInfo *eMsg,  // In
                                     const char     pmode, // In 'c' | 's'
                                     const char    *pid,   // In
                                     const char    *parg,  // In
                                     const char    *spath) // In
{
   extern XrdSecProtocol *XrdSecProtocolhostObject(PROTPARMS);
   void *libhandle;
   XrdSecProtocol *(*ep)(PROTPARMS);
   char           *(*ip)(INITPARMS);
   char *tlist[8], poname[80], libfn[80], libpath[2048], *libloc, *newargs;
   int i, k = 1;

// The "host" protocol is builtin.
//
   if (!strcmp(pid, "host")) return Add(eMsg,pid,XrdSecProtocolhostObject,0);
   tlist[0] = (char *)"XrdSec: ";

// Form library name
//
   snprintf(libfn, sizeof(libfn)-1, "libXrdSec%s.so", pid);
   libfn[sizeof(libfn)-1] = '\0';

// Determine path
//
   if (!spath || (i = strlen(spath)) < 2) libloc = libfn;
      else {char *sep = (spath[i-1] == '/' ? (char *)"" : (char *)"/");
            snprintf(libpath, sizeof(libpath)-1, "%s%s%s", spath, sep, libfn);
            libpath[sizeof(libpath)-1] = '\0';
            libloc = libpath;
           }
   DEBUG("Loading " <<pid <<" protocol object from " <<libloc);

// For clients, verify if the library exists (don't complain, if not)
//
   if (pmode == 'c')
      {struct stat buf;
       if (!stat((const char *)libloc, &buf) && errno == ENOENT)
          {eMsg->setErrInfo(ENOENT, ""); return 0;}
      }

// Open the security library
//
   if (!(libhandle = dlopen(libloc, RTLD_NOW)))
      {tlist[k++] = (char *)dlerror();
       tlist[k++] = (char *)" opening shared library ";
       tlist[k++] = libloc;
       eMsg->setErrInfo(-1, tlist, k);
       return 0;
      }

// Get the protocol object creator
//
   sprintf(poname, "XrdSecProtocol%sObject", pid);
   if (!(ep = (XrdSecProtocol *(*)(PROTPARMS))dlsym(libhandle, poname)))
      {tlist[k++] = (char *)dlerror();
       tlist[k++] = (char *)" finding ";
       tlist[k++] = poname;
       tlist[k++] = (char *)" in ";
       tlist[k++] = libloc;
       eMsg->setErrInfo(-1, tlist, k);
       return 0;
      }

// Get the protocol initializer
//
   sprintf(poname, "XrdSecProtocol%sInit", pid);
   if (!(ip = (char *(*)(INITPARMS))dlsym(libhandle, poname)))
      {tlist[k++] = (char *)dlerror();
       tlist[k++] = (char *)" finding ";
       tlist[k++] = poname;
       tlist[k++] = (char *)" in ";
       tlist[k++] = libloc;
       eMsg->setErrInfo(-1, tlist, k);
       return 0;
      }

// Invoke the one-time initialization
//
   if (!(newargs = ip(pmode, (pmode == 'c' ? 0 : parg), eMsg))) return 0;

// Add this protocol to our protocol stack
//
   return Add(eMsg, pid, ep, newargs);
}
 
/******************************************************************************/
/*                                L o o k u p                                 */
/******************************************************************************/
  
XrdSecProtList *XrdSecPManager::Lookup(const char *pid)   // In
{
   XrdSecProtList *plp;

// Since we only add protocols and never remove them, we need only to lock
// the protocol list to get the first item.
//
   myMutex.Lock();
   plp = First;
   myMutex.UnLock();

// Now we can go and find a matching protocol
//
   while(plp && strcmp(plp->protid, pid)) plp = plp->Next;

   return plp;
}
