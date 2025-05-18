//////////////////////////////////////////////////////////////////////////
//                                                                      //
// XrdClientPhyConnection                                               //
// Author: Fabrizio Furano (INFN Padova, 2004)                          //
// Adapted from TXNetFile (root.cern.ch) originally done by             //
//  Alvise Dorigo, Fabrizio Furano                                      //
//          INFN Padova, 2003                                           //
//                                                                      //
// Class handling physical connections to xrootd servers                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//       $Id: XrdClientPhyConnection.hh,v 1.8 2004/11/09 21:32:11 elmer Exp $

#ifndef _XrdClientPhyConnection
#define _XrdClientPhyConnection

#include "XrdClient/XrdClientSock.hh"
#include "XrdClient/XrdClientMessage.hh"
#include "XrdClient/XrdClientUnsolMsg.hh"
#include "XrdClient/XrdClientInputBuffer.hh"
#include "XrdClient/XrdClientUrlInfo.hh"
#include "XrdOuc/XrdOucPthread.hh"

#include <time.h> // for time_t data type
#include <pthread.h>

enum ELoginState {
   kNo      = 0,
   kYes     = 1, 
   kPending = 2
};
enum ERemoteServer {
   kBase    = 100, 
   kData    = 101, 
   kUnknown = 102
};

class XrdClientPhyConnection: public XrdClientUnsolMsgSender {
private:
   time_t              fLastUseTimestamp;
   enum ELoginState    fLogged;       // only 1 login/auth is needed for physical  
   XrdClientInputBuffer      fMsgQ;         // The queue used to hold incoming messages
   int                 fRequestTimeout;
  
   pthread_mutex_t     fRwMutex;     // Lock before using the physical channel 
                                      // (for reading and/or writing)

   pthread_mutex_t     fMutex;

   pthread_t           fReaderthreadhandler; // The thread which is going to pump
                                             // out the data from the socket
                                             // in the async mode
   bool                fReaderthreadrunning;

   XrdClientUrlInfo          fServer;

   XrdClientSock       *fSocket;

   void HandleUnsolicited(XrdClientMessage *m);

   XrdOucCondVar       fReaderCV;

public:
   ERemoteServer       fServerType;
   long                fTTLsec;

   XrdClientPhyConnection(XrdClientAbsUnsolMsgHandler *h);
   ~XrdClientPhyConnection();

   XrdClientMessage     *BuildMessage(bool IgnoreTimeouts, bool Enqueue);
   void                  CheckAutoTerm();

   bool           Connect(XrdClientUrlInfo RemoteHost);
   void           Disconnect();
   bool           ExpiredTTL();
   long           GetTTL() { return fTTLsec; }

   void           StartedReader();

   bool           IsAddress(XrdClientString &addr) {
      return ( (fServer.Host == addr) ||
	       (fServer.HostAddr == addr) );
   }

   ELoginState    IsLogged() const { return fLogged; }
   bool           IsPort(int port) { return (fServer.Port == port); };
   bool           IsValid() const { return (fSocket && fSocket->IsConnected());}
   void           LockChannel();
   int            ReadRaw(void *buffer, int BufferLength);
   XrdClientMessage     *ReadMessage(int streamid);
   bool           ReConnect(XrdClientUrlInfo RemoteHost);
   void           SetLogged(ELoginState status) { fLogged = status; }
   inline void    SetTTL(long ttl) { fTTLsec = ttl; }
   void           StartReader();
   void           Touch();
   void           UnlockChannel();
   int            WriteRaw(const void *buffer, int BufferLength);

};




//
// Class implementing a trick to automatically unlock an XrdClientPhyConnection
//
class XrdClientPhyConnLocker {
private:
   XrdClientPhyConnection *phyconn;

public:
   XrdClientPhyConnLocker(XrdClientPhyConnection *phyc) {
      // Constructor
      phyconn = phyc;
      phyconn->LockChannel();
   }

   ~XrdClientPhyConnLocker(){
      // Destructor. 
      phyconn->UnlockChannel();
   }

};

#endif
