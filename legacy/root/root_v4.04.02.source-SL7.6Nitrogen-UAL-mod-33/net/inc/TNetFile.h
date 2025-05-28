// @(#)root/net:$Name:  $:$Id: TNetFile.h,v 1.18 2004/12/15 17:48:03 rdm Exp $
// Author: Fons Rademakers   14/08/97

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TNetFile
#define ROOT_TNetFile


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TNetFile                                                             //
//                                                                      //
// A TNetFile is like a normal TFile except that it reads and writes    //
// its data via a rootd server.                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TFile
#include "TFile.h"
#endif
#ifndef ROOT_TUrl
#include "TUrl.h"
#endif
#ifndef ROOT_TFTP
#include "TFTP.h"
#endif
#ifndef ROOT_TSystem
#include "TSystem.h"
#endif
#ifndef ROOT_MessageTypes
#include "MessageTypes.h"
#endif

class TSocket;


class TNetFile : public TFile {

protected:
   TUrl      fUrl;        //URL of file
   TString   fUser;       //remote user name
   TSocket  *fSocket;     //connection to rootd server
   Int_t     fProtocol;   //rootd protocol level
   Int_t     fErrorCode;  //error code returned by rootd (matching gRootdErrStr)

   TNetFile(const char *url, const char *ftitle, Int_t comp, Bool_t);
   virtual void ConnectServer(Int_t *stat, EMessageTypes *kind, Int_t netopt,
                              Int_t tcpwindowsize, Bool_t forceOpen,
                              Bool_t forceRead);
   virtual void Create(const char *url, Option_t *option, Int_t netopt);
   virtual void Create(TSocket *s, Option_t *option, Int_t netopt);
   void         Init(Bool_t create);
   void         Print(Option_t *option) const;
   void         PrintError(const char *where, Int_t err);
   Int_t        Recv(Int_t &status, EMessageTypes &kind);
   Int_t        SysOpen(const char *pathname, Int_t flags, UInt_t mode);
   Int_t        SysClose(Int_t fd);
   Int_t        SysStat(Int_t fd, Long_t *id, Long64_t *size, Long_t *flags, Long_t *modtime);

public:
   TNetFile(const char *url, Option_t *option = "", const char *ftitle = "",
            Int_t compress = 1, Int_t netopt = 0);
   TNetFile() : fUrl("dummy") { fSocket = 0; }
   virtual ~TNetFile();

   void    Close(Option_t *option="");  // *MENU*
   void    Flush();
   Int_t   GetErrorCode() const { return fErrorCode; }
   Bool_t  IsOpen() const;
   Int_t   ReOpen(Option_t *mode);
   Bool_t  ReadBuffer(char *buf, Int_t len);
   Bool_t  WriteBuffer(const char *buf, Int_t len);
   void    Seek(Long64_t offset, ERelativeTo pos = kBeg);

   ClassDef(TNetFile,1)  //A ROOT file that reads/writes via a rootd server
};


class TNetSystem : public TSystem {

private:
   Bool_t      fDir;         // true if a directory is open remotely
   void       *fDirp;        // directory handler
   TFTP       *fFTP;         // Connection to rootd
   TString     fHost;        // Remote host
   TString     fUser;        // Remote user

   void       *GetDirPtr() const { return fDirp; }

public:
   TNetSystem();
   TNetSystem(const char *url);
   virtual ~TNetSystem();

   Bool_t      ConsistentWith(const char *path, void *dirptr);
   Int_t       MakeDirectory(const char *name);
   void       *OpenDirectory(const char *name);
   void        FreeDirectory(void *dirp = 0);
   const char *GetDirEntry(void *dirp = 0);
   Int_t       GetPathInfo(const char *path, FileStat_t &buf);
   Bool_t      AccessPathName(const char *path, EAccessMode mode);

   ClassDef(TNetSystem,0)  // Directory handler for NetFile
};

#endif
