/******************************************************************************/
/*                                                                            */
/*                       X r d O s s U n l i n k . c c                        */
/*                                                                            */
/* (c) 2003 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

//         $Id: XrdOssUnlink.cc,v 1.6 2004/12/14 01:16:44 abh Exp $

const char *XrdOssUnlinkCVSID = "$Id: XrdOssUnlink.cc,v 1.6 2004/12/14 01:16:44 abh Exp $";

#include <unistd.h>
#include <errno.h>
#include <strings.h>
#include <iostream>
#include <limits.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "XrdOss/XrdOssApi.hh"
#include "XrdOss/XrdOssError.hh"
#include "XrdOss/XrdOssLock.hh"
#include "XrdOss/XrdOssTrace.hh"

/******************************************************************************/
/*           G l o b a l   E r r o r   R o u t i n g   O b j e c t            */
/******************************************************************************/

extern XrdOucError OssEroute;

extern XrdOucTrace OssTrace;

/******************************************************************************/
/*                 S t o r a g e   S y s t e m   O b j e c t                  */
/******************************************************************************/
  
extern XrdOssSys XrdOssSS;
  
/******************************************************************************/
/*                                U n l i n k                                 */
/******************************************************************************/

/*
  Function: Delete a file from the namespace and release it's data storage.

  Input:    path      - Is the fully qualified name of the file to be removed.

  Output:   Returns XrdOssOK upon success and -errno upon failure.
*/
int XrdOssSys::Unlink(const char *path) 
{
    EPNAME("Unlink")
    int ismig, i, remotefs, retc2, retc = XrdOssOK;
    XrdOssLock un_file;
    struct stat statbuff;
    char *fnp;
    char  local_path[XrdOssMAX_PATH_LEN+1+8];
    char remote_path[XrdOssMAX_PATH_LEN+1];

// Determine whether we can actually unlink a file on this server.
//
   remotefs = Check_RO(Unlink, ismig, (char *)path, "deleting ");
   ismig &= (XrdOssREMOTE | XrdOssMIG);

// Build the right local and remote paths.
//
   if ( (retc = XrdOssSS.GenLocalPath( path,  local_path))
   ||   (retc = XrdOssSS.GenRemotePath(path, remote_path)) ) return retc;

 // Serialize the directory.
 //
    if (remotefs && (retc=un_file.Serialize(local_path,XrdOssDIR|XrdOssEXC)) < 0)
       return retc;

// Check if this path is really a symbolic link elsewhere
//
    if (lstat(local_path, &statbuff)) retc = (errno == ENOENT ? 0 : -errno);
       else if ((statbuff.st_mode & S_IFMT) == S_IFLNK)
               retc = BreakLink(local_path, statbuff);

// Delete the local copy and every valid suffix variation
//
   if (!retc)
      if (unlink(local_path)) retc = -errno;
         else {i = strlen(local_path); fnp = &local_path[i];
               Adjust(statbuff.st_dev, statbuff.st_size);
               if (ismig) for (i = 0; sfx[i]; i++)
                  {strcpy(fnp, sfx[i]);
                   if (unlink(local_path))
                      if (errno == ENOENT) continue;
                         else retc2 = errno;
                      else retc2 = 0;
                   DEBUG("sfx retc=" <<retc2 <<' ' <<local_path);
                  }
              }
      DEBUG("lcl rc=" <<retc <<" path=" <<local_path);

// If local copy effectively deleted. delete the remote copy if need be
//
   if (remotefs && (!retc || retc == -ENOENT))
      {if ((retc2 = MSS_Unlink(remote_path)) != -ENOENT) retc = retc2;
       DEBUG("rmt rc=" <<retc2 <<" path=" <<remote_path);
      }

// Deserialize the directory and return
//
   if (remotefs) un_file.UnSerialize(0);
   return retc;
}

/******************************************************************************/
/*                       P r i v a t e   M e t h o d s                        */
/******************************************************************************/
/******************************************************************************/
/*                             B r e a k L i n k                              */
/******************************************************************************/

int XrdOssSys::BreakLink(const char *local_path, struct stat &statbuff)
{
    EPNAME("BreakLink")
    char lnkbuff[PATH_MAX+1];
    int lnklen, retc = 0;

// Read the contents of the link
//
    if ((lnklen = readlink(local_path, lnkbuff, sizeof(lnkbuff)-1)) < 0)
       return -errno;

// Return the actual stat information on the target (which might not exist
//
   lnkbuff[lnklen] = '\0';
   if (stat(lnkbuff, &statbuff)) 
      {statbuff.st_size = 0;
       if (errno == ENOENT) return 0;
      }

// Now unlink the target
//
   if (unlink(lnkbuff) && errno != ENOENT)
      {retc = -errno;
       OssEroute.Emsg("XrdOssBreakLink",retc,"unlink symlink target",lnkbuff);
      }

// All done
//
   DEBUG("broke link " <<local_path <<"->" <<lnkbuff);
   return retc;
}
