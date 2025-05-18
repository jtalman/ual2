/******************************************************************************/
/*                                                                            */
/*                       X r d O s s R e n a m e . c c                        */
/*                                                                            */
/* (c) 2003 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

//         $Id: XrdOssRename.cc,v 1.8 2004/12/15 07:04:48 abh Exp $

const char *XrdOssRenameCVSID = "$Id: XrdOssRename.cc,v 1.8 2004/12/15 07:04:48 abh Exp $";

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
/*                                R e n a m e                                 */
/******************************************************************************/

/*
  Function: Renames a file with name 'old_name' to 'new_name'.

  Input:    old_name  - Is the fully qualified name of the file to be renamed.
            new_name  - Is the fully qualified name that the file is to have.

  Output:   Returns XrdOssOK upon success and -errno upon failure.
*/
int XrdOssSys::Rename(const char *oldname, const char *newname)
{
    EPNAME("Rename")
    int i, retc2, remotefs_Old, remotefs_New, remotefs, ismig, retc = XrdOssOK;
    int old_popts, new_popts;
    XrdOssLock old_file, new_file;
    struct stat statbuff;
    char  local_path_Old[XrdOssMAX_PATH_LEN+1+8], *lpo;
    char  local_path_New[XrdOssMAX_PATH_LEN+1+8], *lpn;
    char remote_path_Old[XrdOssMAX_PATH_LEN+1];
    char remote_path_New[XrdOssMAX_PATH_LEN+1];

// Determine whether we can actually rename a file on this server.
//
   remotefs_Old = Check_RO(Rename, old_popts, (char *)oldname, "renaming ");
   remotefs_New = Check_RO(Rename, new_popts, (char *)newname, "renaming to ");

// Make sure we are renaming within compatible file systems
//
   if (remotefs_Old ^ remotefs_New
   || ((old_popts & XrdOssMIG) ^ (new_popts & XrdOssMIG)))
      {char buff[PATH_MAX+128];
       snprintf(buff, sizeof(buff), "rename %s to ", oldname);
       return OssEroute.Emsg("XrdOssRename",-XRDOSS_E8011,buff,(char *)newname);
      }
   remotefs = remotefs_Old | remotefs_New;
   ismig    = remotefs | (XrdOssMIG & (old_popts | new_popts));

// Construct the filename that we will be dealing with.
//
   if ( (retc = GenLocalPath( oldname, local_path_Old))
     || (retc = GenLocalPath( newname, local_path_New)) ) return retc;
   if (remotefs
     && (retc = GenRemotePath(oldname, remote_path_Old))
     || (retc = GenRemotePath(newname, remote_path_New)) ) return retc;

// Lock the target directory if this is a remote backed filesystem
//
   if (remotefs &&
       (retc2 = new_file.Serialize(local_path_New, XrdOssDIR|XrdOssEXC)) < 0)
      return retc2;

// Make sure that the target file does not exist
//
   retc2 = lstat(local_path_New, &statbuff);
   if (remotefs) new_file.UnSerialize(0);
   if (!retc2) return -EEXIST;

// Serialize access to the source directory.
//
     if (remotefs &&
         (retc = old_file.Serialize(local_path_Old, XrdOssDIR|XrdOssEXC)) < 0)
       return retc;

// Check if this path is really a symbolic link elsewhere
//
    if (lstat(local_path_Old, &statbuff)) retc = (errno == ENOENT ? 0 : -errno);
       else if ((statbuff.st_mode & S_IFMT) == S_IFLNK)
               retc = RenameLink(local_path_Old, local_path_New);
               else if (rename(local_path_Old, local_path_New)) retc = -errno;
    DEBUG("lcl rc=" <<retc <<" op=" <<local_path_Old <<" np=" <<local_path_New);

// For migratable spave, rename all suffix variations of the base file
//
   if (ismig)
      {if ((!retc || retc == -ENOENT))
          {i = strlen(local_path_Old); lpo = &local_path_Old[i];
           i = strlen(local_path_New); lpn = &local_path_New[i];
           for (i = 0; sfx[i]; i++)
               {strcpy(lpo, sfx[i]); strcpy(lpn, sfx[i]);
                if (rename(local_path_Old,local_path_New) && ENOENT != errno)
                   DEBUG("sfx retc=" <<errno <<" op=" <<local_path_Old);
               }
          }
      }

// Now rename the data file in the remote system if the local rename "worked".
//
   if (remotefs)
      {if (remotefs && (!retc || retc == -ENOENT))
          {if ( (retc2 = MSS_Rename(remote_path_Old, remote_path_New))
              != -ENOENT) retc = retc2;
           DEBUG("rmt rc=" <<retc2 <<" op=" <<remote_path_Old <<" np=" <<remote_path_New);
          }

      // All done.
      //
         old_file.UnSerialize(0);
      }

// All done.
//
   return retc;
}
 
/******************************************************************************/
/*                       p r i v a t e   m e t h o d s                        */
/******************************************************************************/
/******************************************************************************/
/*                            R e n a m e L i n k                             */
/******************************************************************************/

int XrdOssSys::RenameLink(char *old_path, char *new_path)
{
    struct stat statbuff;
    char oldlnk[PATH_MAX+1], newlnk[PATH_MAX+1], *nl, *np;
    int i, lnklen, rc = 0;

// Read the contents of the link
//
   if ((lnklen = readlink(old_path, oldlnk, sizeof(oldlnk)-1)) < 0)
      return -errno;
   oldlnk[lnklen] = '\0';

// Copy all the char up to the first % into the new link buffer
//
   for (i = 0; oldlnk[i] && oldlnk[i] != XrdOssTPC; i++) newlnk[i] = oldlnk[i];

// Now make sure the resulting name will fit
//
   if ((i + strlen(new_path)) >= sizeof(newlnk))
      {OssEroute.Emsg("XrdOssRenameLink",-ENAMETOOLONG,"convert",oldlnk);
       return -ENAMETOOLONG;
      }

// Append the new name, appropriately translated
//
   np = new_path; nl = &newlnk[i];
   XrdOssTAMP(nl, np);

// Make sure that the target name does not exist
//
   if (!lstat(newlnk, &statbuff))
      {OssEroute.Emsg("XrdOssRenameLink",-EEXIST,"check new target", newlnk);
       return -EEXIST;
      }

// Insert a new link in the target cache
//
   if (symlink((const char *)newlnk, (const char *)new_path))
      {rc = errno;
       OssEroute.Emsg("XrdOssRenameLink", rc," symlink to", newlnk);
       return -rc;
      }

// Rename the actual target file
//
   if (rename((const char *)oldlnk, (const char *)newlnk))
      {rc = errno;
       OssEroute.Emsg("XrdOssRenameLink", rc," rename", oldlnk);
       unlink(new_path);
       return -rc;
      }

// Now, unlink the source path
//
   if (unlink((const char *)old_path))
      OssEroute.Emsg("XrdOssRenameLink", rc," unlink", old_path);

// All done
//
   return 0;
}
