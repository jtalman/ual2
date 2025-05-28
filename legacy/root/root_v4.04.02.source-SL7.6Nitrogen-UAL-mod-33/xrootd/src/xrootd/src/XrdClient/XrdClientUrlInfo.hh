//////////////////////////////////////////////////////////////////////////
//                                                                      //
// XrdClientUrlInfo                                                     // 
//                                                                      //
// Author: Fabrizio Furano (INFN Padova, 2004)                          //
// Adapted from TXNetFile (root.cern.ch) originally done by             //
//  Alvise Dorigo, Fabrizio Furano                                      //
//          INFN Padova, 2003                                           //
//                                                                      //
// Class handling information about an url                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//       $Id: XrdClientUrlInfo.hh,v 1.6 2004/11/09 21:32:11 elmer Exp $

#ifndef _XRC_URLINFO_H
#define _XRC_URLINFO_H

#include "XrdClient/XrdClientString.hh"

// The information an url may contain
// Plus utilities for parsing and rebuilding an url
class XrdClientUrlInfo {
 public:
   XrdClientString Proto;
   XrdClientString Passwd;
   XrdClientString User;
   XrdClientString Host;
   int Port;
   XrdClientString HostAddr;
   XrdClientString HostWPort;
   XrdClientString File;

   void Clear();
   void TakeUrl(XrdClientString url);
   XrdClientString GetUrl();

   XrdClientUrlInfo(XrdClientString url);
   XrdClientUrlInfo();

   void SetAddrFromHost();

   inline bool IsValid() {
      return (Port >= 0);
   }

};


#endif
