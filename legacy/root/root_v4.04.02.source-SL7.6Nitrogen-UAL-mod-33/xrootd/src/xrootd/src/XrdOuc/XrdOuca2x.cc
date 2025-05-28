/******************************************************************************/
/*                                                                            */
/*                          X r d O u c a 2 x . c c                           */
/*                                                                            */
/* (c) 2004 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*       All Rights Reserved. See XrdInfo.cc for complete License Terms       */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

//           $Id: XrdOuca2x.cc,v 1.5 2004/09/14 06:24:16 abh Exp $

const char *XrdOuca2xCVSID = "$Id: XrdOuca2x.cc,v 1.5 2004/09/14 06:24:16 abh Exp $";

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>

#include "XrdOuc/XrdOuca2x.hh"

/******************************************************************************/
/*                                   a 2 i                                    */
/******************************************************************************/

int XrdOuca2x::a2i(XrdOucError &Eroute, const char *emsg, char *item,
                                             int *val, int minv, int maxv)
{
    if (!item || !*item)
       {Eroute.Emsg("a2x", emsg, (char *)"value not specified"); return -1;}

    errno = 0;
    *val  = strtol(item, (char **)NULL, 10);
    if (errno)
       {Eroute.Emsg("a2x", emsg, item, (char *)"is not a number");
        return -1;
       }
    if (*val < minv) 
       return Emsg(Eroute, emsg, item, "may not be less than %d", minv);
    if (maxv >= 0 && *val > maxv)
       return Emsg(Eroute, emsg, item, "may not be greater than %d", maxv);
    return 0;
}
 
/******************************************************************************/
/*                                  a 2 l l                                   */
/******************************************************************************/

long long XrdOuca2x::a2ll(XrdOucError &Eroute, const char *emsg, char *item,
                                long long *val, long long minv, long long maxv)
{
    if (!item || !*item)
       {Eroute.Emsg("a2x", emsg, (char *)"value not specified"); return -1;}

    errno = 0;
    *val  = strtoll(item, (char **)NULL, 10);
    if (errno)
       {Eroute.Emsg("a2x", emsg, item, (char *)"is not a number");
        return -1;
       }
    if (*val < minv) 
       return Emsg(Eroute, emsg, item, "may not be less than %lld", minv);
    if (maxv >= 0 && *val > maxv)
       return Emsg(Eroute, emsg, item, "may not be greater than %lld", maxv);
    return 0;
}

/******************************************************************************/
/*                                  a 2 f m                                   */
/******************************************************************************/

int XrdOuca2x::a2fm(XrdOucError &Eroute, const char *emsg, char *item,
                                              int *val, int minv, int maxv)
{  int rc, num;
   if ((rc = a2fm(Eroute, emsg, item, &num, minv))) return rc;
   if ((*val | maxv) != maxv) 
      {Eroute.Emsg("a2fm", emsg, item, (char *)"is too inclusive.");
       return -1;
      }

   *val = 0;
   if (num & 0100) *val |= S_IXUSR; // execute permission: owner
   if (num & 0200) *val |= S_IWUSR; // write permission:   owner
   if (num & 0400) *val |= S_IRUSR; // read permission:    owner
   if (num & 0010) *val |= S_IXGRP; // execute permission: group
   if (num & 0020) *val |= S_IWGRP; // write permission:   group
   if (num & 0040) *val |= S_IRGRP; // read permission:    group
   if (num & 0001) *val |= S_IXOTH; // execute permission: other
   if (num & 0002) *val |= S_IWOTH; // write permission:   other
   if (num & 0004) *val |= S_IROTH; // read permission:    other
   return 0;
}

int XrdOuca2x::a2fm(XrdOucError &Eroute, const char *emsg, char *item,
                                              int *val, int minv)
{
    if (!item || !*item)
       {Eroute.Emsg("a2x", emsg, (char *)"value not specified"); return -1;}

    errno = 0;
    *val  = strtol(item, (char **)NULL, 8);
    if (errno)
       {Eroute.Emsg("a2x", emsg, item, (char *)"is not an octal number");
        return -1;
       }
    if (!(*val & minv))
       {Eroute.Emsg("a2x", emsg, item, (char *)"is too exclusive");;
        return -1;
       }
    return 0;
}
 
/******************************************************************************/
/*                                  a 2 s z                                   */
/******************************************************************************/

long long XrdOuca2x::a2sz(XrdOucError &Eroute, const char *emsg, char *item,
                                long long *val, long long minv, long long maxv)
{   int i = strlen(item)-1;
    long long qmult = 1;

    if (!item || !*item)
       {Eroute.Emsg("a2x", emsg, (char *)"value not specified"); return -1;}

    errno = 0;
    if (item[i] == 'k' || item[i] == 'K') qmult = 1024;
    if (item[i] == 'm' || item[i] == 'M') qmult = 1024*1024;
    if (item[i] == 'g' || item[i] == 'g') qmult = 1024*1024*1024;
    if (qmult > 1) item[i] = '\0';
    *val  = strtoll(item, (char **)NULL, 10) * qmult;
    if (errno)
       {Eroute.Emsg("a2x", emsg, item, (char *)"is not a number");
        return -1;
       }
    if (*val < minv) 
       return Emsg(Eroute, emsg, item, "may not be less than %lld", minv);
    if (maxv >= 0 && *val > maxv)
       return Emsg(Eroute, emsg, item, "may not be greater than %lld", maxv);
    return 0;
}
 
/******************************************************************************/
/*                                  a 2 t m                                   */
/******************************************************************************/

int XrdOuca2x::a2tm(XrdOucError &Eroute, const char *emsg, char *item, int *val,
                          int minv, int maxv)
{   int i = strlen(item)-1;
    int qmult = 0;

    if (!item || !*item)
       {Eroute.Emsg("a2x", emsg, (char *)"value not specified"); return -1;}

    errno = 0;
    if (item[i] == 's' || item[i] == 's') qmult = 1;
    if (item[i] == 'm' || item[i] == 'M') qmult = 60;
    if (item[i] == 'h' || item[i] == 'H') qmult = 60*60;
    if (item[i] == 'd' || item[i] == 'D') qmult = 60*60*24;
    if (qmult > 0) item[i] = '\0';
       else qmult = 1;
    *val  = strtoll(item, (char **)NULL, 10) * qmult;
    if (errno)
       {Eroute.Emsg("a2x", emsg, item, (char *)"is not a number");
        return -1;
       }
    if (*val < minv) 
       return Emsg(Eroute, emsg, item, "may not be less than %d", minv);
    if (maxv >= 0 && *val > maxv)
       return Emsg(Eroute, emsg, item, "may not be greater than %d", maxv);
    return 0;
}

/******************************************************************************/
/*                                  a 2 v p                                   */
/******************************************************************************/

int XrdOuca2x::a2vp(XrdOucError &Eroute, const char *emsg, char *item,
                                             int *val, int minv, int maxv)
{
    char *pp;

    if (!item || !*item)
       {Eroute.Emsg("a2x", emsg, (char *)"value not specified"); return -1;}

    errno = 0;
    *val  = strtol(item, &pp, 10);

    if (!errno && *pp == '%')
       {if (*val < 0)
           {Eroute.Emsg("a2x", emsg, item, (char *)"may not be negative.");
            return -1;
           }
        if (*val > 100)
           {Eroute.Emsg("a2x", emsg, item, (char *)"may not be greater than 100%.");
            return -1;
           }
           else {*val = -*val; return 0;}
       }

    if (*val < minv) 
       return Emsg(Eroute, emsg, item, "may not be less than %d", minv);
    if (maxv >= 0 && *val > maxv)
       return Emsg(Eroute, emsg, item, "may not be greater than %d", maxv);
    return 0;
}

/******************************************************************************/
/*                       P r i v a t e   M e t h o d s                        */
/******************************************************************************/
  
int XrdOuca2x::Emsg(XrdOucError &Eroute, const char *etxt1, char *item,
                                         const char *etxt2, int val)
{char buff[256];
 sprintf(buff, etxt2, val);
 Eroute.Emsg("a2x", etxt1, item, buff);
 return -1;
}

long long XrdOuca2x::Emsg(XrdOucError &Eroute, const char *etxt1, char *item,
                                               const char *etxt2, long long val)
{char buff[256];
 sprintf(buff, etxt2, val);
 Eroute.Emsg("a2x", etxt1, item, buff);
 return -1;
}
