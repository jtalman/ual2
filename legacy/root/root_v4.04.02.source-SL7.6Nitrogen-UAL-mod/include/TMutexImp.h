// @(#)root/thread:$Name:  $:$Id: TMutexImp.h,v 1.2 2004/12/10 12:13:33 rdm Exp $
// Author: Fons Rademakers   01/07/97

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TMutexImp
#define ROOT_TMutexImp


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMutexImp                                                            //
//                                                                      //
// This class provides an abstract interface to the OS dependent mutex  //
// classes (TPosixMutex and TWin32Mutex).                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif
#ifndef ROOT_TObject
#include "TObject.h"
#endif


class TMutexImp : public TObject {

public:
   TMutexImp() { }
   virtual ~TMutexImp() { }

   virtual Int_t  Lock() = 0;
   virtual Int_t  TryLock() = 0;
   virtual Int_t  UnLock() = 0;

   ClassDef(TMutexImp,0)  // Mutex lock implementation ABC
};

#endif
