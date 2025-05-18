// @(#)root/netx:$Name:  $:$Id: TXSocket.h,v 1.5 2004/12/16 19:23:18 rdm Exp $
// Author: Alvise Dorigo, Fabrizio Furano

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TXSocket
#define ROOT_TXSocket


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TXSocket                                                             //
//                                                                      //
// Authors: Alvise Dorigo, Fabrizio Furano                              //
//          INFN Padova, 2003                                           //
//                                                                      //
// Extension of TSocket to handle read/write and connection timeouts    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef ROOT_TSocket
#include "TSocket.h"
#endif
#ifndef ROOT_TSystem
#include "TSystem.h"
#endif
#ifndef ROOT_TString
#include "TString.h"
#endif
#ifndef __CINT__
#include <poll.h>
#endif

#define DFLT_CONNECTTIMEOUT    10
#define DFLT_CONNECTTIMEOUTWAN 90
#define DFLT_REQUESTTIMEOUT    60

#define TXSOCK_ERR_TIMEOUT	-1
#define TXSOCK_ERR		-2

// Used to enable the asynchronous working mode
#define DFLT_GOASYNC 0

struct TXSocketConnectParms {
  TString TcpAddress;
  Int_t TcpPort;
  Int_t TcpWindowSize;
};

class TXSocket : public TSocket {

friend class TXPhyConnection;

private:
   Bool_t               fASYNC;
   TXSocketConnectParms fHost2contact;  // status connection thread
   Int_t                fRequestTimeout;

protected:
   TSocket *ExtractSocket(); // Extract TSocket part

public:
   TXSocket(TString host, Int_t port, Int_t tcpwindowsize = -1);
   virtual ~TXSocket();

   static void    CatchTimeOut();

   void           Create(TString, Int_t, Int_t);
   virtual Int_t  RecvRaw(void* buffer, Int_t length,
                                        ESendRecvOptions opt = kDefault);
   virtual Int_t  SendRaw(const void* buffer, Int_t length,
                                        ESendRecvOptions opt = kDefault);
   void           TryConnect();

   ClassDef(TXSocket, 0); // An extension of TSocket with read/write/connect
                          // timeouts and threads
};

#endif
