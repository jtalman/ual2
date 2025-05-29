/******************************************************************************/
/*                                                                            */
/*                     X r d O u c P l a t f o r m . c c                      */
/*                                                                            */
/* (c) 2004 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*       All Rights Reserved. See XrdInfo.cc for complete License Terms       */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/
  
//        $Id: XrdOucPlatform.cc,v 1.6 2005/01/03 07:50:49 abh Exp $

const char *XrdOucPlatformCVSID = "$Id: XrdOucPlatform.cc,v 1.6 2005/01/03 07:50:49 abh Exp $";

#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>

#if defined(_LITTLE_ENDIAN) || (defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)
#if !defined(__GNUC__)
unsigned long long Swap_n2hll(unsigned long long x)
{
 unsigned long long ret_val;
#ifdef _LP64
    *( (unsigned int  *)(&ret_val) + 1) = ntohl(*( (unsigned int  *)(&x)));
    *(((unsigned int  *)(&ret_val)))    = ntohl(*(((unsigned int  *)(&x))+1));
#else
    *( (unsigned long *)(&ret_val) + 1) = ntohl(*( (unsigned long *)(&x)));
    *(((unsigned long *)(&ret_val)))    = ntohl(*(((unsigned long *)(&x))+1));
#endif
    return ret_val;
}
#endif

#endif

#ifndef HAS_STRLCPY
extern "C"
{
size_t strlcpy(char *dst, const char *src, size_t sz)
{
    size_t slen = strlen(src);
    size_t tlen =sz-1;

    if (slen <= tlen) strcpy(dst, src);
       else if (tlen > 0) {strncpy(dst, src, tlen); dst[tlen] = '\0';}
               else if (tlen == 0) dst[0] = '\0';

    return slen;
}
}
#endif

