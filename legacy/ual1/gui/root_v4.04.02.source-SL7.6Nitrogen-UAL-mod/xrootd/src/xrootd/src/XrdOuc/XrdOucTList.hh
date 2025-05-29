#ifndef __OUC_TLIST__
#define __OUC_TLIST__
/******************************************************************************/
/*                                                                            */
/*                        X r d O u c T L i s t . h h                         */
/*                                                                            */
/* (c) 2003 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

//          $Id: XrdOucTList.hh,v 1.1 2004/05/11 06:24:46 abh Exp $

#include <stdlib.h>
#include <string.h>
#include <strings.h>
  
class XrdOucTList
{
public:

XrdOucTList *next;
char        *text;
int          val;

             XrdOucTList(const char *tval=0, int num=0, XrdOucTList *np=0)
                        {text = (tval ? strdup(tval) : 0); val=num; next=np;}

            ~XrdOucTList() {if (text) free(text);}
};
#endif
