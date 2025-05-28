//////////////////////////////////////////////////////////////////////////
//                                                                      //
// XrdClientReadCache                                                   //
//                                                                      //
// Author: Fabrizio Furano (INFN Padova, 2004)                          //
// Adapted from TXNetFile (root.cern.ch) originally done by             //
//  Alvise Dorigo, Fabrizio Furano                                      //
//          INFN Padova, 2003                                           //
//                                                                      //
// Classes to handle cache reading                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//       $Id: XrdClientReadCache.hh,v 1.9 2005/03/03 18:14:56 furano Exp $

#ifndef XRD_READCACHE_H
#define XRD_READCACHE_H

#include <pthread.h>
#include <iostream>

#include "XrdClient/XrdClientInputBuffer.hh"
#include "XrdClient/XrdClientMessage.hh"
#include "XrdClient/XrdClientVector.hh"

#define xrdmin(a, b) (a < b ? a : b)

//
// XrdClientReadCacheItem
//
// An item is nothing more than an interval of bytes taken from a file.
// Extremes are included.
// Since a cache object is to be associated to a single instance
// of TXNetFile, we do not have to keep here any filehandle
//

class XrdClientReadCacheItem {
 private:
   long long        fBeginOffset;    // Offset of the first byte of data
   void             *fData;
   long long        fEndOffset;      // Offset of the last byte of data
   long             fTimestampTicks; // timestamp updated each time it's referenced

 public:
   XrdClientReadCacheItem(const void *buffer, long long begin_offs, 
			  long long end_offs, long long ticksnow);
   ~XrdClientReadCacheItem();

   // Is this obj contained in the given interval (which is going to be inserted) ?
   inline bool   ContainedInInterval(long long begin_offs, long long end_offs) {
      return ( (end_offs > begin_offs) &&
               (fBeginOffset >= begin_offs) &&
               (fEndOffset <= end_offs) );
   }

   // Does this obj contain the given interval (which is going to be requested) ?
   inline bool   ContainsInterval(long long begin_offs, long long end_offs) {
      return ( (end_offs > begin_offs) &&
               (fBeginOffset <= begin_offs) && (fEndOffset >= end_offs) );
   }

   // Get the requested interval, if possible
   inline bool   GetInterval(const void *buffer, long long begin_offs, 
			     long long end_offs) {
      if (!ContainsInterval(begin_offs, end_offs))
         return FALSE;
      memcpy((void *)buffer, ((char *)fData)+(begin_offs - fBeginOffset),
	     end_offs - begin_offs + 1);
      return TRUE;
   }

   // Get as many bytes as possible, starting from the beginning of the given
   // interval
   inline long   GetPartialInterval(const void *buffer, long long begin_offs,
				    long long end_offs) {

      long long b = -1, e, l;

      if (!fData || (begin_offs >= end_offs)) return 0;

      // Try to set the starting point, if contained in the given interval
      if ( (begin_offs >= fBeginOffset) &&
	   (begin_offs < fEndOffset) )
	 b = begin_offs;

      if (b < 0) return 0;

      // The starting point is in the interval. Let's get the minimum endpoint
      e = xrdmin(end_offs, fEndOffset);

      l = e - b + 1;

      memcpy((void *)buffer, ((char *)fData)+(b - fBeginOffset), l);

      return l;
   }

   inline long long GetTimestampTicks() { return(fTimestampTicks); }
   long Size() { return (fEndOffset - fBeginOffset - 1); }
   inline void     Touch(long long ticksnow) { fTimestampTicks = ticksnow; }
};

//
// XrdClientReadCache
//
// The content of the cache. Not cache blocks, but
// variable length Items
//
typedef XrdClientVector<XrdClientReadCacheItem *> ItemVect;

class XrdClientReadCache {
 private:

   long long      fBytesHit;         // Total number of bytes read with a cache hit
   long long      fBytesSubmitted;   // Total number of bytes inserted
   float         fBytesUsefulness;
   ItemVect      fItems;
   long long      fMaxCacheSize;
   long long      fMissCount;        // Counter of the cache misses
   float         fMissRate;            // Miss rate
   pthread_mutex_t  fMutex;
   long long      fReadsCounter;     // Counter of all the attempted reads (hit or miss)
   long long      fTimestampTickCounter;        // Aging mechanism yuk!
   long long      fTotalByteCount;

   long long      GetTimestampTick();
   bool        MakeFreeSpace(long long bytes);
   bool        RemoveLRUItem();
   inline void   UpdatePerfCounters() {
      if (fReadsCounter > 0)
         fMissRate = (float)fMissCount / fReadsCounter;
      if (fBytesSubmitted > 0)
         fBytesUsefulness = (float)fBytesHit / fBytesSubmitted;
   }

 public:
   XrdClientReadCache();
   ~XrdClientReadCache();
  
   bool          GetDataIfPresent(const void *buffer, long long begin_offs,
                                  long long end_offs, bool PerfCalc);
   inline long long GetTotalByteCount() { return fTotalByteCount; }
   inline void     PrintPerfCounters() {
      cout << "Caching info: MissRate=" << fMissRate << " MissCount=" << 
	 fMissCount << " ReadsCounter=" << fReadsCounter << endl;
      cout << "Caching info: BytesUsefulness=" << fBytesUsefulness <<
	 " BytesSubmitted=" << fBytesSubmitted << " BytesHit=" << 
	 fBytesHit << endl;
   }

   void            SubmitXMessage(XrdClientMessage *xmsg, long long begin_offs,
				  long long end_offs);
   void            RemoveItems();
   void            RemoveItems(long long begin_offs, long long end_offs);
   // To check if a block dimension will fit into the cache
   inline bool   WillFit(long long bc) { return (bc < fMaxCacheSize); }

};

#endif
