//////////////////////////////////////////////////////////////////////////
//                                                                      //
// XrdClientUnsolMsg                                                          //
//                                                                      //
// Author: Fabrizio Furano (INFN Padova, 2004)                          //
// Adapted from TXNetFile (root.cern.ch) originally done by             //
//  Alvise Dorigo, Fabrizio Furano                                      //
//          INFN Padova, 2003                                           //
//                                                                      //
// Base classes for unsolicited msg senders/receivers                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//       $Id: XrdClientUnsolMsg.hh,v 1.4 2004/11/09 21:32:11 elmer Exp $

#ifndef XRC_UNSOLMSG_H
#define XRC_UNSOLMSG_H

#include "XrdClient/XrdClientMessage.hh"

class XrdClientUnsolMsgSender;

// Handler

class XrdClientAbsUnsolMsgHandler {
public:
  
  virtual ~XrdClientAbsUnsolMsgHandler() { }
  // To be called when an unsolicited response arrives from the lower layers
  virtual bool ProcessUnsolicitedMsg(XrdClientUnsolMsgSender *sender, 
                                       XrdClientMessage *unsolmsg) = 0;

};

// Sender

class XrdClientUnsolMsgSender {
public:
  virtual ~XrdClientUnsolMsgSender() { }
   // The upper level handler for unsolicited responses
  XrdClientAbsUnsolMsgHandler *UnsolicitedMsgHandler;

  inline void SendUnsolicitedMsg(XrdClientUnsolMsgSender *sender, XrdClientMessage *unsolmsg) {
    // We simply send the event
    if (UnsolicitedMsgHandler)
      UnsolicitedMsgHandler->ProcessUnsolicitedMsg(sender, unsolmsg);
  }

  inline XrdClientUnsolMsgSender() { UnsolicitedMsgHandler = 0; }
};

#endif
