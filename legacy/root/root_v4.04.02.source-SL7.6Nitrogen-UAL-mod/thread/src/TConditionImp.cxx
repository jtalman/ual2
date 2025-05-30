// @(#)root/thread:$Name:  $:$Id: TConditionImp.cxx,v 1.2 2004/12/10 12:13:33 rdm Exp $
// Author: Fons Rademakers   01/07/97

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TConditionImp                                                        //
//                                                                      //
// This class provides an abstract interface to the OS dependent        //
// condition classes (TPosixCondition and TWin32Condition).             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TConditionImp.h"

ClassImp(TConditionImp)
