// @(#)root/thread:$Name:  $:$Id: TPosixThreadFactory.h,v 1.1.1.1 2000/05/16 17:00:48 rdm Exp $
// Author: Fons Rademakers   01/07/97

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/


#ifndef ROOT_TPosixThreadFactory
#define ROOT_TPosixThreadFactory

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TPosixThreadFactory                                                  //
//                                                                      //
// This is a factory for Posix thread components.                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TThreadFactory
#include "TThreadFactory.h"
#endif

class TMutexImp;
class TConditionImp;
class TThreadImp;


class TPosixThreadFactory : public TThreadFactory {

public:
   TPosixThreadFactory(const char *name = "Posix", const char *title = "Posix Thread Factory");
   virtual ~TPosixThreadFactory() { }

   virtual TMutexImp      *CreateMutexImp();
   virtual TConditionImp  *CreateConditionImp(TMutexImp *m);
   virtual TThreadImp     *CreateThreadImp();

   ClassDef(TPosixThreadFactory,0)  // Posix thread factory
};

#endif
