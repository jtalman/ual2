// @(#)root/base:$Name:  $:$Id: TMemberStreamer.h,v 1.1 2004/01/10 10:52:29 brun Exp $
// Author: Victor Perev and Philippe Canal   08/05/02

/*************************************************************************
 * Copyright (C) 1995-2003, Rene Brun, Fons Rademakers and al.           *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TMemberStreamer
#define ROOT_TMemberStreamer

#include "Rtypes.h"

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMemberStreamer is used to stream a data member.                     //
//                                                                      //
// The address passed to operator() will be the address of the data     //
// member.                                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TMemberStreamer {
protected:
   TMemberStreamer() : fStreamer(0) {};
   
public:
   TMemberStreamer(MemberStreamerFunc_t pointer) : fStreamer(pointer) {};
   TMemberStreamer(const TMemberStreamer &rhs) : fStreamer(rhs.fStreamer) {};

   virtual  ~TMemberStreamer(){};
   
   virtual void operator()(TBuffer &b, void *pmember, Int_t size=0)
   {
      // The address passed to operator() will be the address of the data member.
      // If the data member is a variable size array, 'size' is the number of elements
      // to read/write
      
      (*fStreamer)(b,pmember,size);
   }
   
private:
   MemberStreamerFunc_t fStreamer; 
};

#endif
