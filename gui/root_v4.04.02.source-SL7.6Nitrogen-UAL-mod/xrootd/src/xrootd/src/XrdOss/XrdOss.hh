#ifndef _XRDOSS_H
#define _XRDOSS_H
/******************************************************************************/
/*                                                                            */
/*                     X r d O s s   &   X r d O s s D F                      */
/*                                                                            */
/* (c) 2003 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

//         $Id: XrdOss.hh,v 1.7 2005/02/25 05:35:55 abh Exp $

#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

/******************************************************************************/
/*                                X r d O s s                                 */
/******************************************************************************/

class XrdOucEnv;
class XrdOucLogger;
class XrdSfsAio;
  
class XrdOss
{
public:
virtual int     Create(const char *, mode_t, XrdOucEnv &, int mkpath=0)=0;
virtual int     Init(XrdOucLogger *, const char *)=0;
virtual int     Mkdir(const char *, mode_t mode, int mkpath=0)=0;
virtual int     Remdir(const char *)=0;
virtual int     Rename(const char *, const char *)=0;
virtual int     Stat(const char *, struct stat *, int resonly=0)=0;
virtual int     Unlink(const char *)=0;

                XrdOss() {}
virtual        ~XrdOss() {}
};

/******************************************************************************/
/*                              X r d O s s D F                               */
/******************************************************************************/
  
class XrdOssDF
{
public:
                // Directory oriented methods
virtual int     Opendir(const char *)                        {return -ENOTDIR;}
virtual int     Readdir(char *buff, int blen)                {return -ENOTDIR;}

                // File oriented methods
virtual int     Fstat(struct stat *)                         {return -EISDIR;}
virtual int     Fsync()                                      {return -EISDIR;}
virtual int     Fsync(XrdSfsAio *aiop)                       {return -EISDIR;}
virtual int     Ftruncate(unsigned long long)                {return -EISDIR;}
virtual size_t  getMmap(void **addr)                         {return 0;}
virtual int     isCompressed(char *cxidp=0)                  {return -EISDIR;}
virtual int     Open(const char *, int, mode_t, XrdOucEnv &) {return -EISDIR;}
virtual ssize_t Read(off_t, size_t)                          {return (ssize_t)-EISDIR;}
virtual ssize_t Read(void *, off_t, size_t)                  {return (ssize_t)-EISDIR;}
virtual int     Read(XrdSfsAio *aoip)                        {return (ssize_t)-EISDIR;}
virtual ssize_t ReadRaw(    void *, off_t, size_t)           {return (ssize_t)-EISDIR;}
virtual ssize_t Write(const void *, off_t, size_t)           {return (ssize_t)-EISDIR;}
virtual int     Write(XrdSfsAio *aiop)                       {return (ssize_t)-EISDIR;}

                // Methods common to both
virtual int     Close()=0;
inline  int     Handle() {return fd;}

                XrdOssDF() {fd = -1;}
virtual        ~XrdOssDF() {}

protected:

int     fd;      // The associated file descriptor.
};

#ifndef XrdOssOK
#define XrdOssOK 0
#endif
#endif
