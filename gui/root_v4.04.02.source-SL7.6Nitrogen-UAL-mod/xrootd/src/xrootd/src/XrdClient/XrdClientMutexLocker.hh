//////////////////////////////////////////////////////////////////////////
//                                                                      //
// XrdClientMutexLocker                                                 //
//                                                                      //
// Author: Fabrizio Furano (INFN Padova, 2004)                          //
// Adapted from TXNetFile (root.cern.ch) originally done by             //
//  Alvise Dorigo, Fabrizio Furano                                      //
//          INFN Padova, 2003                                           //
//                                                                      //
// A simple class useful to associate a mutex lock/unlock               //
//  to a syntactical block enclosed in {}                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//       $Id: XrdClientMutexLocker.hh,v 1.3 2004/09/16 20:02:19 elmer Exp $

#ifndef XRC_MUTEXLOCKER_H
#define XRC_MUTEXLOCKER_H

#include <pthread.h>

class XrdClientMutexLocker {

private:
   pthread_mutex_t fMtx;

public:
 
   inline XrdClientMutexLocker(pthread_mutex_t mutex) { 
      fMtx = mutex;
      pthread_mutex_lock(&fMtx);
   }

   inline ~XrdClientMutexLocker() { pthread_mutex_unlock(&fMtx); }
};




#endif
