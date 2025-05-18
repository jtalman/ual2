// @(#)root/proof:$Name:  $:$Id: TProofServ.h,v 1.25 2005/05/02 11:00:39 rdm Exp $
// Author: Fons Rademakers   16/02/97

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/


#ifndef ROOT_TProofServ
#define ROOT_TProofServ

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TProofServ                                                           //
//                                                                      //
// TProofServ is the PROOF server. It can act either as the master      //
// server or as a slave server, depending on its startup arguments. It  //
// receives and handles message coming from the client or from the      //
// master server.                                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TApplication
#include "TApplication.h"
#endif
#ifndef ROOT_TString
#include "TString.h"
#endif
#ifndef ROOT_Htypes
#include "Htypes.h"
#endif
#ifndef ROOT_TStopwatch
#include "TStopwatch.h"
#endif


class TProof;
class TProofPlayer;
class TSocket;
class TList;
class TDSetElement;


class TProofServ : public TApplication {

private:
   TString       fService;          //service we are running, either "proofserv" or "proofslave"
   TString       fUser;             //user as which we run
   TString       fPasswd;           //encoded passwd info for slaves
   TString       fConfDir;          //directory containing cluster config information
   TString       fConfFile;         //file containing config information
   TString       fWorkDir;          //directory containing all proof related info
   TString       fSessionDir;       //directory containing session dependent files
   TString       fPackageDir;       //directory containing packages and user libs
   TString       fCacheDir;         //directory containing cache of user files
   TString       fPackageLock;      //package dir lock file
   TString       fCacheLock;        //cache dir lock file
   TSocket      *fSocket;           //socket connection to client
   TProof       *fProof;            //PROOF talking to slave servers
   TProofPlayer *fPlayer;           //actual player
   FILE         *fLogFile;          //log file
   TList        *fEnabledPackages;  //list of enabled packages
   Int_t         fPackageLockId;    //file id of package dir lock
   Int_t         fCacheLockId;      //file id of cache dir lock
   Int_t         fProtocol;         //protocol version number
   TString       fOrdinal;          //slave ordinal number
   Int_t         fGroupId;          //slave unique id in the active slave group
   Int_t         fGroupSize;        //size of the active slave group
   Int_t         fLogLevel;         //debug logging level
   Int_t         fNcmd;             //command history number
   Bool_t        fPwHash;           //true if fPasswd is a passwd hash
   Bool_t        fSRPPwd;           //true if fPasswd is a SRP passwd
   Bool_t        fMasterServ;       //true if we are a master server
   Bool_t        fInterrupt;        //if true macro execution will be stopped
   Float_t       fRealTime;         //real time spent executing commands
   Float_t       fCpuTime;          //CPU time spent executing commands
   TStopwatch    fLatency;          //measures latency of packet requests
   TStopwatch    fCompute;          //measures time spend processing a packet

   void        Setup();
   void        RedirectOutput();
   Int_t       CatMotd();
   Int_t       LockDir(const TString &lock);
   Int_t       UnlockDir(const TString &lock);
   Int_t       LockCache() { return LockDir(fCacheLock); }
   Int_t       UnlockCache() { return UnlockDir(fCacheLock); }
   Int_t       LockPackage() { return LockDir(fPackageLock); }
   Int_t       UnlockPackage() { return UnlockDir(fPackageLock); }
   Int_t       UnloadPackage(const char* package);
   Int_t       UnloadPackages();
   void        HandleSocketInputDuringProcess();

public:
   TProofServ(int *argc, char **argv);
   virtual ~TProofServ();

   TProof        *GetProof() const { return fProof; }
   const char    *GetService() const { return fService; }
   const char    *GetConfDir() const { return fConfDir; }
   const char    *GetConfFile() const { return fConfFile; }
   const char    *GetUser() const { return fUser; }
   const char    *GetWorkDir() const { return fWorkDir; }
   const char    *GetSessionDir() const { return fSessionDir; }
   Int_t          GetProtocol() const { return fProtocol; }
   const char    *GetOrdinal() const { return fOrdinal; }
   Int_t          GetGroupId() const { return fGroupId; }
   Int_t          GetGroupSize() const { return fGroupSize; }
   Int_t          GetLogLevel() const { return fLogLevel; }
   TSocket       *GetSocket() const { return fSocket; }
   Float_t        GetRealTime() const { return fRealTime; }
   Float_t        GetCpuTime() const { return fCpuTime; }
   void           GetOptions(int *argc, char **argv);

   void           HandleSocketInput();
   void           HandleUrgentData();
   void           HandleSigPipe();
   void           Interrupt() { fInterrupt = kTRUE; }
   Bool_t         IsMaster() const { return fMasterServ; }
   Bool_t         IsParallel() const;
   Bool_t         IsTopMaster() const { return fOrdinal == "0"; }

   void           Run(Bool_t retrn = kFALSE);

   void           Print(Option_t *option="") const;

   TObject       *Get(const char *namecycle);
   TDSetElement  *GetNextPacket();
   void           Reset(const char *dir);
   Int_t          ReceiveFile(const char *file, Bool_t bin, Long_t size);
   void           SendLogFile(Int_t status = 0);
   void           SendStatistics();
   void           SendParallel();

   void           Terminate(Int_t status);

   static Bool_t      IsActive();
   static TProofServ *This();

   ClassDef(TProofServ,0)  //PROOF Server Application Interface
};

R__EXTERN TProofServ *gProofServ;

#endif
