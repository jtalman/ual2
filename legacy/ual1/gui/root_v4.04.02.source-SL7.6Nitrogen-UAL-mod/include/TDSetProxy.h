// @(#)root/proof:$Name:  $:$Id: TDSetProxy.h,v 1.1 2002/03/13 01:52:20 rdm Exp $
// Author: Maarten Ballintijn  12/03/02

/*************************************************************************
 * Copyright (C) 1995-2001, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TDSetProxy
#define ROOT_TDSetProxy


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TDSetProxy                                                           //
//                                                                      //
// TDSet proxy for use on slaves.                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TDSet
#include "TDSet.h"
#endif

class TProofServ;


class TDSetProxy : public TDSet {

   // skeleton implementation need to properly deal with all TDSet methods ...

private:
   TProofServ     *fServ;     //!

public:
   TDSetProxy();
   TDSetProxy(const char *type, const char *objname = "*", const char *dir = "/");

   void           Reset();
   TDSetElement  *Next();

   void  SetProofServ(TProofServ *serv);

   ClassDef(TDSetProxy,1)  // TDSet proxy for use on slaves
};

#endif
