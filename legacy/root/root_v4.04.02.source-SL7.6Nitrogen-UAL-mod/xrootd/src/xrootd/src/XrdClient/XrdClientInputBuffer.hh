//////////////////////////////////////////////////////////////////////////
//                                                                      //
// XrdClientInputBuffer                                                 //
//                                                                      //
// Author: Fabrizio Furano (INFN Padova, 2004)                          //
// Adapted from TXNetFile (root.cern.ch) originally done by             //
//  Alvise Dorigo, Fabrizio Furano                                      //
//          INFN Padova, 2003                                           //
//                                                                      //
// Buffer for incoming messages (responses)                             //
//  Handles the waiting (with timeout) for a message to come            //
//   belonging to a logical streamid                                    //
//  Multithread friendly                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//       $Id: XrdClientInputBuffer.hh,v 1.7 2004/11/09 21:32:11 elmer Exp $

#ifndef XRC_INPUTBUFFER_H
#define XRC_INPUTBUFFER_H

#include <pthread.h>
#include "XrdClient/XrdClientMessage.hh"
#include "XrdClient/XrdClientMutexLocker.hh"

#include "XrdClient/XrdClientVector.hh"
#include "XrdOuc/XrdOucHash.hh"
#include "XrdClient/XrdClientVector.hh"

using namespace std;

class XrdClientInputBuffer {

private:

   XrdClientVector<XrdClientMessage*> fMsgQue;      // queue for incoming messages
   int                                fMsgIter;     // an iterator on it

   pthread_mutex_t             fMutex;       // mutex to protect data structures
   pthread_mutex_t             fCndMutex;    // mutex to protect the condition variables

   XrdOucHash<pthread_cond_t>  fSyncobjRepo;
                                             // each streamid counts on a condition
                                             // variable to make the caller wait
                                             // until some data is available


   pthread_cond_t  *GetSyncObjOrMakeOne(int streamid);
   int             MsgForStreamidCnt(int streamid);

public:
   XrdClientInputBuffer();
  ~XrdClientInputBuffer();

   inline bool     IsMexEmpty() { return (MexSize() == 0); }
   inline bool     IsSemEmpty() { return (SemSize() == 0); }
   inline int      MexSize() { 
                       XrdClientMutexLocker mtx(fMutex);
                       return fMsgQue.GetSize();
                       }
   int             PutMsg(XrdClientMessage *msg);
   inline int      SemSize() {
                       XrdClientMutexLocker mtx(fMutex);
                       return fSyncobjRepo.Num();
                       }

   XrdClientMessage      *GetMsg(int streamid, int secstimeout);
};



#endif
