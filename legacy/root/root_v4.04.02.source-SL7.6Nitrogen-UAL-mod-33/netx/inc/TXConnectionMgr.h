// @(#)root/netx:$Name:  $:$Id: TXConnectionMgr.h,v 1.5 2004/12/16 19:23:18 rdm Exp $
// Author: Alvise Dorigo, Fabrizio Furano

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TXConnectionMgr
#define ROOT_TXConnectionMgr

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TXConnectionMgr                                                      //
//                                                                      //
// Authors: Alvise Dorigo, Fabrizio Furano                              //
//          INFN Padova, 2003                                           //
//                                                                      //
// The Connection Manager handles socket communications for TXNetFile   //
// action: connect, disconnect, read, write. It is a static object of   //
// the TXNetFile class such that within a single application multiple   //
// TXNetFile objects share the same connection manager.                 //
// The connection manager maps multiple logical connections on a single //
// physical connection.                                                 //
// There is one and only one logical connection per client (XNTetFile   //
// object), and one and only one physical connection per server:port.   //
// Thus multiple TXNetFile objects withing a given application share    //
// the same physical TCP channel to communicate with the server.        //
// This reduces the time overhead for socket creation and reduces also  //
// the server load due to handling many sockets.                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <vector>

#ifndef ROOT_TString
#include "TString.h"
#endif
#ifndef ROOT_TXUnsolicitedMsg
#include "TXUnsolicitedMsg.h"
#endif
#ifndef ROOT_TXLogConnection
#include "TXLogConnection.h"
#endif
#ifndef ROOT_TXMessage
#include "TXMessage.h"
#endif
#ifndef ROOT_TMutex
#include "TMutex.h"
#endif

#ifdef __CINT__
class TXConnectionMgr;
#endif

// Maybe we don't want to start the garbage collector
// But the default must be to start it
#define DFLT_STARTGARBAGECOLLECTORTHREAD	0

// Ugly prototype to avoid warnings under solaris
extern "C" void * GarbageCollectorThread(void * arg);

class TXConnectionMgr: public TXAbsUnsolicitedMsgHandler,
                       TXUnsolicitedMsgSender {
friend class TXNetConn;
private:
   Bool_t                    fInitialized;
   vector <TXLogConnection*> fLogVec;
   vector <TXPhyConnection*> fPhyVec;
   TMutex                    *fMutex; // mutex used to protect local variables
                                      // of this and TXLogConnection, TXPhyConnection
                                      // classes; not used to protect i/o streams

   TThread                   *fThreadHandler;
   Bool_t                     fThreadKilled;

   friend void * GarbageCollectorThread(void *);
   Bool_t        ProcessUnsolicitedMsg(TXUnsolicitedMsgSender *sender,
                                       TXMessage *unsolmsg);
   void          RemoveLogConn(TXLogConnection *);
   void          RemovePhyConn(TXPhyConnection *);

protected:
   void          GarbageCollect();

public:
   TXConnectionMgr();
   virtual ~TXConnectionMgr();

   Short_t       Connect(TString RemoteAddress, Int_t TcpPort,
                         Int_t TcpWindowSize);
   void          Disconnect(Short_t LogConnectionID, Bool_t ForcePhysicalDisc);
   TXLogConnection *GetConnection(Short_t LogConnectionID);
   short int     GetPhyConnectionRefCount(TXPhyConnection *PhyConn);
   void          Init();
   TXMessage*    ReadMsg(Short_t LogConnectionID,
                         ESendRecvOptions opt = kDefault);
   Int_t         ReadRaw(Short_t LogConnectionID, void *buffer,
                         Int_t BufferLength, ESendRecvOptions opt = kDefault);
   Bool_t        ThreadKilled() const { return fThreadKilled; }
   Int_t         WriteRaw(Short_t LogConnectionID, const void *buffer,
                          Int_t BufferLength, ESendRecvOptions opt = kDefault);

   ClassDef(TXConnectionMgr, 0); // The Connection Manager
};


#endif
