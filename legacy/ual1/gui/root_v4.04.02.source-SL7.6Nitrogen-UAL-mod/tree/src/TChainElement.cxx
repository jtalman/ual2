// @(#)root/tree:$Name:  $:$Id: TChainElement.cxx,v 1.4 2005/01/12 18:02:28 brun Exp $
// Author: Rene Brun   11/02/97

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// A TChainElement describes a component of a TChain.                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TTree.h"
#include "TChainElement.h"

ClassImp(TChainElement)

//______________________________________________________________________________
TChainElement::TChainElement(): TNamed(),fBaddress(0),fBaddressType(0),
     fBaddressIsPtr(kFALSE)
{
//*-*-*-*-*-*Default constructor for a chain element*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*        =======================================

   fNPackets   = 0;
   fPackets    = 0;
   fEntries    = 0;
   fPacketSize = 100;
   fStatus     = -1;
}

//______________________________________________________________________________
TChainElement::TChainElement(const char *name, const char *title)
    :TNamed(name,title),fBaddress(0),fBaddressType(0),
     fBaddressIsPtr(kFALSE)
{
//*-*-*-*-*-*-*-*-*-*-*-*-*Create a chain element*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                      ======================
//

   fNPackets   = 0;
   fPackets    = 0;
   fEntries    = 0;
   fPacketSize = 100;
   fStatus     = -1;
}

//______________________________________________________________________________
TChainElement::~TChainElement()
{
//*-*-*-*-*-*Default destructor for a chain element*-*-*-*-*-*-*-*-*-*-*-*
//*-*        ======================================

   delete [] fPackets;
}

//_______________________________________________________________________
void TChainElement::CreatePackets()
{
//*-*-*-*-*-*-*-*-*Initialize the packet descriptor string*-*-*-*-*-*-*-*-*-*
//*-*              =======================================

   fNPackets = 1 + fEntries/fPacketSize;
   delete [] fPackets;
   fPackets = new char[fNPackets+1];
   for (Int_t i=0;i<fNPackets;i++) fPackets[i] = ' ';
   fPackets[fNPackets] = 0;

}

//_______________________________________________________________________
void TChainElement::ls(Option_t *) const
{
// List files in the chain
//

   printf("%s tree:%s entries=%d\n",GetTitle(),GetName(),fEntries);
}

//_______________________________________________________________________
void TChainElement::SetPacketSize(Int_t size)
{
//*-*-*-*-*-*-*-*-*Set number of entries per packet for parallel root*-*-*-*-*
//*-*              =================================================

   fPacketSize = size;
}
