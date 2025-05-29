#ifndef __XRDOUC_PLATFORM_H__
#define __XRDOUC_PLATFORM_H__
/******************************************************************************/
/*                                                                            */
/*                     X r d O u c P l a t f o r m . h h                      */
/*                                                                            */
/* (c) 2004 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*       All Rights Reserved. See XrdInfo.cc for complete License Terms       */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/
  
//        $Id: XrdOucPlatform.hh,v 1.13 2005/03/11 11:58:36 ganis Exp $

#ifdef __linux__
#include <memory.h>
#include <string.h>
#include <sys/types.h>
#include <asm/param.h>
#include <byteswap.h>
#endif
#ifdef __macos__
#include <sys/types.h>
#endif

#if defined(__linux__) || defined(__macos__)

#define S_IAMB      0x1FF   /* access mode bits */

#define F_DUP2FD F_DUPFD

#define STATFS      statfs
#define STATFS_BUFF struct statfs

#define FS_BLKFACT  4

#define FLOCK_t struct flock

typedef off_t offset_t;

#define GTZ_NULL (struct timezone *)0

#else

#define STATFS      statvfs
#define STATFS_BUFF struct statvfs

#define FS_BLKFACT  1

#define SHMDT_t char *

#define FLOCK_t flock_t

#define GTZ_NULL (void *)0

#define GETHOSTBYNAME(hname, rbuff, cbuff, cblen, rpnt, pretc) \
(rpnt=gethostbyname_r(hname, rbuff, cbuff, cblen,       pretc))


#define GETHOSTBYADDR(haddr, hlen, htype, rbuff, cbuff, cblen, rpnt, pretc) \
(rpnt=gethostbyaddr_r(haddr, hlen, htype, rbuff, cbuff, cblen,       pretc))

#define GETSERVBYNAME(name, stype, psrv, buff, blen, rpnt) \
(rpnt=getservbyname_r(name, stype, psrv, buff, blen))

#endif

#ifdef __linux__

#define SHMDT_t const void *

#define GETHOSTBYNAME(hname, rbuff, cbuff, cblen,  rpnt, pretc) \
     (gethostbyname_r(hname, rbuff, cbuff, cblen, &rpnt, pretc) == 0)

#define GETHOSTBYADDR(haddr,hlen,htype,rbuff,cbuff,cblen, rpnt,pretc) \
     (gethostbyaddr_r(haddr,hlen,htype,rbuff,cbuff,cblen,&rpnt,pretc) == 0)

#define GETSERVBYNAME(name, stype, psrv, buff, blen,  rpnt) \
     (getservbyname_r(name, stype, psrv, buff, blen, &rpnt) == 0)

#endif

// For alternative platforms
//
#ifdef __macos__
#include <AvailabilityMacros.h>
#if !defined(MAC_OS_X_VERSION_10_4)
#define POLLRDNORM  0
#define POLLRDBAND  0
#define POLLWRNORM  0
#endif
#define O_LARGEFILE 0
#define memalign(pgsz,amt) valloc(amt)
#define SHMDT_t void *
#endif

// Only sparc platforms have structure alignment problems w/ optimization
// so the h2xxx() variants are used when converting network streams.

#if defined(_BIG_ENDIAN) || defined(__IEEE_BIG_ENDIAN) || \
   (defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN)
#ifndef htonll
#define htonll(_x_)  _x_
#endif
#ifndef h2nll
#define h2nll(_x_, _y_) memcpy((void *)&_y_,(const void *)&_x_,sizeof(long long))
#endif
#ifndef ntohll
#define ntohll(_x_)  _x_
#endif
#ifndef n2hll
#define n2hll(_x_, _y_) memcpy((void *)&_y_,(const void *)&_x_,sizeof(long long))
#endif

#elif defined(_LITTLE_ENDIAN) || defined(__IEEE_LITTLE_ENDIAN) || \
     (defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)
// Use GNU's bswap routines if compiling using g++ o/w use our own.
#if !defined(__GNUC__)
#ifndef __bswap_64
extern unsigned long long Swap_n2hll(unsigned long long x);
#define __bswap_64(x) Swap_n2hll(x)
#endif
#endif

// When we ever return a long long we will have to fix this as well, perhaps.
#ifndef htonll
#define htonll(_x_) __bswap_64(_x_)
#endif
#ifndef h2nll
#define h2nll(_x_, _y_) memcpy((void *)&_y_,(const void *)&_x_,sizeof(long long));\
                        _y_ = htonll(_y_)
#endif
#ifndef ntohll
#define ntohll(_x_) __bswap_64(_x_)
#endif
#ifndef n2hll
#define n2hll(_x_, _y_) memcpy((void *)&_y_,(const void *)&_x_,sizeof(long long));\
                        _y_ = ntohll(_y_)
#endif

#else
#error Unable to determine target architecture endianness!
#endif

#ifndef HAS_STRLCPY
extern "C"
{extern size_t strlcpy(char *dst, const char *src, size_t size);}
#endif

//
// To make socklen_t portable use SOCKLEN_t
//
#if defined(__sun) && !defined(__linux)
#   if __GNUC__ >= 3 || __GNUC_MINOR__ >= 90
#      define XR__SUNGCC3
#   endif
#endif
#if defined(__linux)
#   include <features.h>
#   if __GNU_LIBRARY__ == 6
#      ifndef XR__GLIBC
#         define XR__GLIBC
#      endif
#   endif
#endif
#if defined(__MACH__) && defined(__i386__)
#   define R__GLIBC
#endif
#if defined(_AIX) || \
   (defined(__FreeBSD__) && (!defined(__alpha) || defined(__linux))) || \
   (defined(XR__SUNGCC3) && !defined(__arch64__))
#   define SOCKLEN_t size_t
#elif defined(XR__GLIBC) || \
   (defined(__FreeBSD__) && (defined(__alpha) && !defined(__linux))) || \
   (defined(XR__SUNGCC3) && defined(__arch64__)) || defined(__macos__)
#   ifndef SOCKLEN_t
#      define SOCKLEN_t socklen_t
#   endif
#elif !defined(SOCKLEN_t)
#   define SOCKLEN_t unsigned int
#endif

#ifdef _LP64
#define PTR2INT(x) static_cast<int>((long long)x)
#else
#define PTR2INT(x) int(x)
#endif

#endif  // __XRDOUC_PLATFORM_H__
