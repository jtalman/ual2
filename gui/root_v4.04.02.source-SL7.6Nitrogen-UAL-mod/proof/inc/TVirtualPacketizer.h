// @(#)root/proof:$Name:  $:$Id: TVirtualPacketizer.h,v 1.4 2003/05/01 17:51:42 rdm Exp $
// Author: Maarten Ballintijn    9/7/2002

/*************************************************************************
 * Copyright (C) 1995-2002, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TVirtualPacketizer
#define ROOT_TVirtualPacketizer

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TVirtualPacketizer                                                   //
//                                                                      //
// XXX update Comment XXX                                               //
// This class generates packets to be processed on PROOF slave servers. //
// A packet is an event range (begin entry and number of entries) or    //
// object range (first object and number of objects) in a TTree         //
// (entries) or a directory (objects) in a file.                        //
// Packets are generated taking into account the performance of the     //
// remote machine, the time it took to process a previous packet on     //
// the remote machine, the locality of the database files, etc.         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TObject
#include "TObject.h"
#endif


class TDSet;
class TDSetElement;
class TSlave;
class TMessage;



class TVirtualPacketizer : public TObject {

private:
   TVirtualPacketizer(const TVirtualPacketizer &);  // no implementation, will generate
   void operator=(const TVirtualPacketizer &);      // error on accidental usage

protected:
   Bool_t   fValid;           // Constructed properly?
   Bool_t   fStop;            // Termination of Process() requested?

   TVirtualPacketizer();
   Long64_t GetEntries(Bool_t tree, TDSetElement *e); // Num of entries or objects

public:
   virtual ~TVirtualPacketizer();

   Bool_t                  IsValid() const { return fValid; }
   virtual Long64_t        GetEntriesProcessed() const;
   virtual Long64_t        GetEntriesProcessed(TSlave *sl) const;
   virtual TDSetElement   *GetNextPacket(TSlave *sl, TMessage *r);
   virtual void            StopProcess(Bool_t abort);

   ClassDef(TVirtualPacketizer,0)  //Generate work packets for parallel processing
};

#endif
