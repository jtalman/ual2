// @(#)root/base:$Name:  $:$Id: TRealData.cxx,v 1.4 2004/01/10 10:52:29 brun Exp $
// Author: Rene Brun   05/03/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "TRealData.h"
#include "TDataMember.h"
#include "TClass.h"
#include "TStreamer.h"

ClassImp(TRealData)

//______________________________________________________________________________
//
//  The TRealData class manages the effective list of all data members
//  for a given class. For example for an object of class TLine that inherits
//  from TObject and TAttLine, the TRealData object for a line contains the
//  complete list of all data members of the 3 classes.
//
//  The list of TRealData members in TClass is built when functions like
//  object.Inspect or object.DrawClass are called.

//______________________________________________________________________________
TRealData::TRealData() : TObject(), fStreamer(0)
{
//*-*-*-*-*-*-*-*-*-*-*RealData default constructor*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ============================

   fIsObject = kFALSE;
}


//______________________________________________________________________________
TRealData::TRealData(const char *name, Int_t offset, TDataMember *datamember)
   : TObject(),fStreamer(0)
{
//*-*-*-*-*-*-*-*-*-*Constructor to define one persistent data member*-*-*-*-*
//*-*                ================================================
//*-* datamember is the pointer to the data member descriptor.
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
   fDataMember = datamember;
   fThisOffset = offset;
   fName = name;
   fIsObject = kFALSE;
}

//______________________________________________________________________________
TRealData::~TRealData()
{
//*-*-*-*-*-*-*-*-*-*-*RealData default destructor*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  =============================

   delete fStreamer;
}

//______________________________________________________________________________
void TRealData::WriteRealData(void *, char *&)
{
//*-*-*-*-*Write one persistent data member on output buffer*-*-*-*-*-*-*-*
//*-*      =================================================
//*-* pointer points to the current persistent data member
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*
}

//______________________________________________________________________________
void TRealData::AdoptStreamer(TMemberStreamer *str)
{
   //fDataMember->SetStreamer(str);
   //delete fStreamer; 
   fStreamer = str;
}

//______________________________________________________________________________
TMemberStreamer *TRealData::GetStreamer() const
{
   return fStreamer; // return fDataMember->GetStreamer();
}

