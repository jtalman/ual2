#ifndef __SFS_INTERFACE_H__
#define __SFS_INTERFACE_H__
/******************************************************************************/
/*                                                                            */
/*                    X r d S f s I n t e r f a c e . h h                     */
/*                                                                            */
/* (c) 2004 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*       All Rights Reserved. See XrdInfo.cc for complete License Terms       */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

//        $Id: XrdSfsInterface.hh,v 1.10 2005/03/01 05:59:00 abh Exp $

#include <string.h>      // For strlcpy()
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>  // for sockaddr

#include "XrdOuc/XrdOucErrInfo.hh"

/******************************************************************************/
/*                            O p e n   M o d e s                             */
/******************************************************************************/

#define SFS_O_RDONLY           0         // open read/only
#define SFS_O_WRONLY           1         // open write/only
#define SFS_O_RDWR             2         // open read/write
#define SFS_O_CREAT        0x100         // used for file creation
#define SFS_O_TRUNC        0x200         // used for file truncation
#define SFS_O_RAWIO   0x02000000         // allow client-side decompression
#define SFS_O_RESET   0x04000000         // Reset any cached information

// The following flag may be set in the access mode arg for open() & mkdir()
//
#define SFS_O_MKPTH   0x08000000         // Make directory path if missing

/******************************************************************************/
/*                               D e f i n e s                                */
/******************************************************************************/

// Longest filename returned by interface (including trailing null)
//
#define SFS_MAX_FILE_NAME_LEN (1024+1)

// Maximum number of bytes of opaque information that can be set
//
#define SFS_MAX_ERROR_LEN 1280

// Return Values for Integer Returning XrdSfs Interface
//
#define SFS_STALL         1 // Return code -> Seconds to stall client
#define SFS_OK            0 // Return code -> All is well
#define SFS_ERROR        -1 // Return code -> Error occurred
#define SFS_REDIRECT   -256 // Return code -> Port number to redirect to

/******************************************************************************/
/*                 S t r u c t u r e s   &   T y p e d e f s                  */
/******************************************************************************/

typedef long long     XrdSfsFileOffset;
typedef int           XrdSfsFileOpenMode;
typedef int           XrdSfsMode;
typedef int           XrdSfsXferSize;

enum XrdSfsFileExistence 
{
     XrdSfsFileExistNo,
     XrdSfsFileExistIsFile,
     XrdSfsFileExistIsDirectory
};
//------------------------------------------------

#define Prep_PRTY0 0
#define Prep_PRTY1 1
#define Prep_PRTY2 2
#define Prep_PRTY3 3
#define Prep_PMASK 3
#define Prep_SENDAOK 4
#define Prep_SENDERR 8
#define Prep_SENDACK 12
#define Prep_WMODE   16

class XrdOucTList;

struct XrdSfsPrep  // Prepare parameters
{
       char            *reqid;     // Request ID
       char            *notify;    // Notification path or 0
       int              opts;      // Prep_xxx
       XrdOucTList     *paths;
};

/******************************************************************************/
/*                      A b s t r a c t   C l a s s e s                       */
/******************************************************************************/
/******************************************************************************/
/*                     s f s F i l e S y s t e m D e s c                      */
/******************************************************************************/

class  XrdOucTList;
class  XrdSfsFile;
class  XrdSfsDirectory;
class  XrdSecEntity;

class XrdSfsFileSystem
{
public:

// The following two methods allocate a directory or file object
//
virtual XrdSfsDirectory *newDir(char *user=0)  = 0;

virtual XrdSfsFile      *newFile(char *user=0) = 0;

// The following are filesystem related methods
//
virtual int            chmod(const char             *Name,
                                   XrdSfsMode        Mode,
                                   XrdOucErrInfo    &out_error,
                             const XrdSecEntity     *client = 0) = 0;

virtual int            fsctl(const int               cmd,
                             const char             *args,
                                   XrdOucErrInfo    &out_error,
                             const XrdSecEntity     *client = 0) = 0;

virtual int            getStats(char *buff, int blen) = 0;

virtual const char    *getVersion() = 0;

virtual int            exists(const char                *fileName,
                                    XrdSfsFileExistence &exists_flag,
                                    XrdOucErrInfo       &out_error,
                              const XrdSecEntity        *client = 0) = 0;

virtual int            mkdir(const char             *dirName,
                                   XrdSfsMode        Mode,
                                   XrdOucErrInfo    &out_error,
                             const XrdSecEntity     *client = 0) = 0;

virtual int            prepare(      XrdSfsPrep       &pargs,
                                     XrdOucErrInfo    &out_error,
                               const XrdSecEntity     *client = 0) = 0;

virtual int            rem(const char             *path,
                                 XrdOucErrInfo    &out_error,
                           const XrdSecEntity     *client = 0) = 0;

virtual int            remdir(const char             *dirName,
                                    XrdOucErrInfo    &out_error,
                              const XrdSecEntity     *client = 0) = 0;

virtual int            rename(const char             *oldFileName,
                              const char             *newFileName,
                                    XrdOucErrInfo    &out_error,
                              const XrdSecEntity     *client = 0) = 0;

virtual int            stat(const char             *Name,
                                  struct stat      *buf,
                                  XrdOucErrInfo    &out_error,
                            const XrdSecEntity     *client = 0) = 0;

virtual int            stat(const char             *Name,
                                  mode_t           &mode,
                                  XrdOucErrInfo    &out_error,
                            const XrdSecEntity     *client = 0) = 0;

                       XrdSfsFileSystem() {}
virtual               ~XrdSfsFileSystem() {}
};

/******************************************************************************/
/*              F i l e   S y s t e m   I n s t a n t i a t o r               */
/******************************************************************************/

class XrdOucLogger;

extern "C"
{
XrdSfsFileSystem *XrdSfsGetFileSystem(XrdSfsFileSystem *native_fs,
                                      XrdOucLogger     *Logger,
                                      const char       *config_fn);
}

/******************************************************************************/
/*                               s f s F i l e                                */
/******************************************************************************/

class XrdSfsAio;
  
class XrdSfsFile
{
public:
        XrdOucErrInfo  error;

virtual int            open(const char                *fileName,
                                  XrdSfsFileOpenMode   openMode,
                                  mode_t               createMode,
                            const XrdSecEntity        *client = 0,
                            const char                *opaque = 0) = 0;

virtual int            close() = 0;

virtual const char    *FName() = 0;

virtual int            getMmap(void **Addr, size_t &Size) = 0;

virtual int            read(XrdSfsFileOffset   fileOffset,
                          XrdSfsXferSize       preread_sz) = 0;

virtual XrdSfsXferSize read(XrdSfsFileOffset   fileOffset,
                          char                *buffer,
                          XrdSfsXferSize       buffer_size) = 0;

virtual int            read(XrdSfsAio *aioparm) = 0;

virtual XrdSfsXferSize write(XrdSfsFileOffset  fileOffset,
                           const char         *buffer,
                           XrdSfsXferSize      buffer_size) = 0;

virtual int            write(XrdSfsAio *aioparm) = 0;

virtual int            stat(struct stat *buf) = 0;

virtual int            sync() = 0;

virtual int            sync(XrdSfsAio *aiop) = 0;

virtual int            truncate(XrdSfsFileOffset fileOffset) = 0;

virtual int            getCXinfo(char cxtype[4], int &cxrsz) = 0;

                       XrdSfsFile(char *user=0) {error.setErrUser(user);}
virtual               ~XrdSfsFile() {}

}; // class XrdSfsFile

/******************************************************************************/
/*                          s f s D i r e c t o r y                           */
/******************************************************************************/
  
class XrdSfsDirectory
{
public:
        XrdOucErrInfo error;

virtual int         open(const char              *dirName,
                         const XrdSecEntity      *client = 0) = 0;

virtual const char *nextEntry() = 0;

virtual int         close() = 0;

virtual const char *FName() = 0;

                    XrdSfsDirectory(char *user=0) {error.setErrUser(user);}
virtual            ~XrdSfsDirectory() {}

}; // class XrdSfsDirectory
#endif
