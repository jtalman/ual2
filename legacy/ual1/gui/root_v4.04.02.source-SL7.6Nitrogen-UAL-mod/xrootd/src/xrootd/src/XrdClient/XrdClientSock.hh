//////////////////////////////////////////////////////////////////////////
//                                                                      //
// XrdClientSock                                                        //
//                                                                      //
// Author: Fabrizio Furano (INFN Padova, 2004)                          //
// Adapted from TXNetFile (root.cern.ch) originally done by             //
//  Alvise Dorigo, Fabrizio Furano                                      //
//          INFN Padova, 2003                                           //
//                                                                      //
// Client Socket with timeout features                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//       $Id: XrdClientSock.hh,v 1.7 2004/11/09 21:32:11 elmer Exp $

#ifndef XRD_CLIENTSOCK_H
#define XRD_CLIENTSOCK_H


#include "XrdClient/XrdClientUrlInfo.hh"
#include "XrdClient/XrdClientConst.hh"


struct XrdClientSockConnectParms {
   XrdClientUrlInfo TcpHost;
   int TcpWindowSize;
};

class XrdClientSock {

private:

   XrdClientSockConnectParms fHost;

   bool fConnected;

   int fSocket;

public:
   XrdClientSock(XrdClientUrlInfo Host);
   ~XrdClientSock();

   void           Create(XrdClientString, int, int);

   int            RecvRaw(void* buffer, int length);
   int            SendRaw(const void* buffer, int length);

   void           TryConnect();

   void           Disconnect();
  

   bool           IsConnected() { return fConnected; };
};

#endif
