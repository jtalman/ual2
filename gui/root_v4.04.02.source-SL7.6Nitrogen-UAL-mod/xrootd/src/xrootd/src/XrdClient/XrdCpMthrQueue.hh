//////////////////////////////////////////////////////////////////////////
//                                                                      //
// XrdCpMthrQueue                                                       //
//                                                                      //
// Author: Fabrizio Furano (INFN Padova, 2004)                          //
//                                                                      //
// A thread safe queue to be used for multithreaded producers-consumers //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//       $Id: XrdCpMthrQueue.hh,v 1.4 2004/11/09 21:32:11 elmer Exp $

#include <pthread.h>
#include "XrdClient/XrdClientVector.hh"
#include "XrdOuc/XrdOucPthread.hh"
using namespace std;

struct XrdCpMessage {
   void *buf;
   int len;
};

// The max allowed size for this queue
// If this value is reached, then the writer has to wait...
#define CPMTQ_BUFFSIZE            2000000

class XrdCpMthrQueue {
 private:
   long                           fTotSize;
   XrdClientVector<XrdCpMessage*>            fMsgQue;      // queue for incoming messages
   int                                       fMsgIter;     // an iterator on it

   pthread_mutex_t                    fMutex;       // mutex to protect data structures

   XrdOucCondVar                      fCnd;         // variable to make the reader wait
                                                    // until some data is available
   XrdOucCondVar                      fWriteCnd;    // variable to make the writer wait
                                                    // if the queue is full
 public:

   XrdCpMthrQueue();
   ~XrdCpMthrQueue();

   int PutBuffer(void *buf, int len);
   int GetBuffer(void **buf, int &len);
   void Clear();
};
   
