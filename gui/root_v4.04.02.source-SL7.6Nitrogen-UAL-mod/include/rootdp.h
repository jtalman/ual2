/* @(#)root/rootd:$Name:  $:$Id: rootdp.h,v 1.8 2004/04/20 15:21:50 rdm Exp $ */

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_rootdp
#define ROOT_rootdp


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// rootdp                                                               //
//                                                                      //
// This header file contains private definitions and declarations       //
// used by rootd.                                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_rpdp
#include "rpdp.h"
#endif

void  RootdClose();
int   RootdIsOpen();

#endif
