/******************************************************************************/
/*                                                                            */
/*                          X r d O u c E n v . c c                           */
/*                                                                            */
/* (c) 2003 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/
  
//         $Id: XrdOucEnv.cc,v 1.4 2005/01/16 04:34:27 abh Exp $

const char *XrdOucEnvCVSID = "$Id: XrdOucEnv.cc,v 1.4 2005/01/16 04:34:27 abh Exp $";

#include "string.h"
#include "stdio.h"

#include "XrdOuc/XrdOucEnv.hh"
  
/******************************************************************************/
/*                           C o n s t r u c t o r                            */
/******************************************************************************/
  
XrdOucEnv::XrdOucEnv(const char *vardata, int varlen) : env_Hash(8,13)
{
   char *vdp, varsave, *varname, *varvalu;

   if (!vardata) {global_env = 0; return;}

// Copy the the global information (don't rely on its being correct)
//
   if (!varlen) varlen = strlen(vardata);
   global_env = (char *)malloc(varlen+1);
   memcpy((void *)global_env, (const void *)vardata, (size_t)varlen);
   global_env[varlen+1] = '\0';
   vdp = global_env;

// scan through the string looking for '&'
//
   if (vdp) while(*vdp)
        {if (*vdp != '&') {vdp++; continue;}    // &....
         varname = ++vdp;

         while(*vdp && *vdp != '=') vdp++;  // &....=
         if (!*vdp) break;
         *vdp = '\0';
         varvalu = ++vdp;

         while(*vdp && *vdp != '&') vdp++;  // &....=....&
         varsave = *vdp; *vdp = '\0';

         if (*varname && *varvalu)
            env_Hash.Rep(strdup(varname), strdup(varvalu), 0, Hash_dofree);

         *vdp = varsave; *(--varvalu) = '=';
        }
   return;
}

/******************************************************************************/
/*                               D e l i m i t                                */
/******************************************************************************/

char *XrdOucEnv::Delimit(char *value)
{
     while(*value) if (*value == ',') {*value = '\0'; return ++value;}
                      else value++;
     return (char *)0;
}

/******************************************************************************/
/*                                G e t I n t                                 */
/******************************************************************************/

long XrdOucEnv::GetInt(const char *varname) 
{
// Retrieve a char* value from the Hash table and convert it into a long.
// Return -999999999 if the varname does not exist
//
  if (env_Hash.Find(varname) == NULL) {
    return -999999999;
  } else {
    return atol(env_Hash.Find(varname));
  }
}


/******************************************************************************/
/*                                P u t I n t                                 */
/******************************************************************************/

void XrdOucEnv::PutInt(const char *varname, long value) 
{
// Convert the long into a char* and the put it into the hash table
//
  char *stringValue = (char*) malloc(20);
  sprintf(stringValue, "%ld", value);
  env_Hash.Rep(varname, strdup(stringValue), 0, Hash_dofree);
  free(stringValue);
}
