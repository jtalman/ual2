#ifndef _ACC_CONFIG_H
#define _ACC_CONFIG_H
/******************************************************************************/
/*                                                                            */
/*                       X r d A c c C o n f i g . h h                        */
/*                                                                            */
/* (C) 2003 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*                DE-AC03-76-SFO0515 with the Deprtment of Energy             */
/******************************************************************************/

//         $Id: XrdAccConfig.hh,v 1.1 2004/05/11 06:23:41 abh Exp $

#include <sys/types.h>

#include "XrdOuc/XrdOuca2x.hh"
#include "XrdOuc/XrdOucError.hh"
#include "XrdOuc/XrdOucHash.hh"
#include "XrdOuc/XrdOucPthread.hh"
#include "XrdOuc/XrdOucStream.hh"
#include "XrdAcc/XrdAccAccess.hh"
#include "XrdAcc/XrdAccAuthDB.hh"
#include "XrdAcc/XrdAccCapability.hh"
#include "XrdAcc/XrdAccGroups.hh"

/******************************************************************************/
/*                           X r d A c c G l i s t                            */
/******************************************************************************/
  
struct XrdAccGlist 
{
       struct XrdAccGlist *next;     /* Null if this is the last one */
       char               *name;     /* -> null terminated name */

       XrdAccGlist(const char *Name, struct XrdAccGlist *Next=0)
                  {name = strdup(Name); next = Next;}
      ~XrdAccGlist()
                  {if (name) free(name);}
};

/******************************************************************************/
/*                          X r d A c c C o n f i g                           */
/******************************************************************************/
  
class XrdAccConfig
{
public:

// Configure() is called during initialization.
//
int           Configure(XrdOucError &Eroute, const char *cfn);

// ConfigDB() simply refreshes the in-core authorization database. When the 
// Warm is true, a check is made whether the database actually changed and the
// refresh is skipped if it has not changed.
//
int           ConfigDB(int Warm, XrdOucError &Eroute);

XrdAccAccess *Authorization;
XrdAccGroups  GroupMaster;

int           AuthRT;

              XrdAccConfig();
             ~XrdAccConfig() {}    // Configuration is never destroyed!

private:

struct XrdAccGlist *addGlist(gid_t Gid, const char *Gname, 
                             struct XrdAccGlist *Gnext);
int                 ConfigDBrec(XrdOucError &Eroute,
                                struct XrdAccAccess_Tables &tabs);
void                ConfigDefaults(void);
int                 ConfigFile(XrdOucError &Eroute, const char *cfn);
int                 ConfigXeq(char *, XrdOucStream &, XrdOucError &);
int                 PrivsConvert(char *privs, XrdAccPrivCaps &ctab);
int                 xaud(XrdOucStream &Config, XrdOucError &Eroute);
int                 xart(XrdOucStream &Config, XrdOucError &Eroute);
int                 xdbp(XrdOucStream &Config, XrdOucError &Eroute);
int                 xglt(XrdOucStream &Config, XrdOucError &Eroute);
int                 xgrt(XrdOucStream &Config, XrdOucError &Eroute);
int                 xnis(XrdOucStream &Cofig, XrdOucError &Eroute);

XrdAccAuthDB        *Database;
char                *dbpath;

XrdOucMutex          Config_Context;
XrdOucThread         Config_Refresh;

int                  options;
};
#endif
