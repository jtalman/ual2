#ifndef __OOUC_A2X__
#define __OOUC_A2X__
/******************************************************************************/
/*                                                                            */
/*                          X r d O u c a 2 x . h h                           */
/*                                                                            */
/* (c) 2004 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*       All Rights Reserved. See XrdInfo.cc for complete License Terms       */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/
  
//          $Id: XrdOuca2x.hh,v 1.3 2004/09/14 06:24:16 abh Exp $

#include "XrdOuc/XrdOucError.hh"

// This class is a holding area for various conversion utility routines
//

class XrdOuca2x
{
public:
static int         a2i( XrdOucError &, const char *emsg, char *item, int *val, int minv=-1, int maxv=-1);
static long long   a2ll(XrdOucError &, const char *emsg, char *item, long long *val, long long minv=-1, long long maxv=-1);
static int         a2fm(XrdOucError &, const char *emsg, char *item, int *val, int minv);
static int         a2fm(XrdOucError &, const char *emsg, char *item, int *val, int minv, int maxv);
static long long   a2sz(XrdOucError &, const char *emsg, char *item, long long *val, long long minv=-1, long long maxv=-1);
static int         a2tm(XrdOucError &, const char *emsg, char *item, int *val, int minv=-1, int maxv=-1);
static int         a2vp(XrdOucError &, const char *emsg, char *item, int *val, int minv=-1, int maxv=-1);

private:
static int       Emsg(XrdOucError &Eroute, const char *etxt1, char *item,
                                           const char *etxt2, int       val);
static long long Emsg(XrdOucError &Eroute, const char *etxt1, char *item,
                                           const char *etxt2, long long val);
};

#endif
