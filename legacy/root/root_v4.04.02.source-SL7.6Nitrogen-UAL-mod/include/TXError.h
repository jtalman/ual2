// @(#)root/netx:$Name:  $:$Id: TXError.h,v 1.2 2004/08/20 22:16:33 rdm Exp $
// Author: Alvise Dorigo, Fabrizio Furano

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TXError
#define ROOT_TXError

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TXError                                                              //
//                                                                      //
// Authors: Alvise Dorigo, Fabrizio Furano                              //
//          INFN Padova, 2003                                           //
//                                                                      //
// Error handler function for TXNetFile classes                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif
#ifndef ROOT_Varargs
#include "Varargs.h"
#endif

extern void TXNErrorHandler(int level, Bool_t abort, const char *location,
                                const char *msg);

#endif
