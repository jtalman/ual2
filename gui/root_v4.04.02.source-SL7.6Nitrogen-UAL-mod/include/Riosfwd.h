// @(#)root/base:$Name:  $:$Id: Riosfwd.h,v 1.3 2004/08/02 08:52:53 rdm Exp $
// Author: Fons Rademakers   23/1/02

/*************************************************************************
 * Copyright (C) 1995-2002, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_Riosfwd
#define ROOT_Riosfwd

#ifndef ROOT_RConfig
#include "RConfig.h"
#endif

#if defined(R__ANSISTREAM)
#   if defined(R__TMPLTSTREAM)
#      include <iostream>
#   else
#      include <iosfwd>
#   endif
using std::istream;
using std::ostream;
using std::fstream;
using std::ifstream;
using std::ofstream;
#else
class istream;
class ostream;
class fstream;
class ifstream;
class ofstream;
#endif

#endif
