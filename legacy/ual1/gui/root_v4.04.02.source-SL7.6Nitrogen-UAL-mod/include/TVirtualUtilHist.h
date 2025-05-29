// @(#)root/base:$Name:  $:$Id: TVirtualUtilHist.h,v 1.3 2004/01/25 20:33:31 brun Exp $
// Author: Rene Brun   14/09/2002

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/
#ifndef ROOT_TVirtualUtilHist
#define ROOT_TVirtualUtilHist


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TVirtualUtilHist                                                     //
//                                                                      //
// Abstract interface to the histogram  utilities                       //
//                                                                      //
// This class is called via the TPluginManager from classes that        //
// do not require linking with libHist except in some rare cases like   //
// painting matrices and vectors              .                         //
// The concrete implementation TUtilHist is defined in system.rootrc    //
// and can be overridden by a user to extend the functionality.         //
// This abstract interface has three main goals:                        //
//   - it decouples libHist from the calling classes                    //
//   - it does not require the use of magic strings like when using     //
//     gROOT->ProcessLine to instantiate classes in libHist.            //
//   - it allows advanced users to redefine or extend some functions.   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef ROOT_TNamed
#include "TNamed.h"
#endif

class TMatrixFBase;
class TMatrixDBase;
class TVectorF;
class TVectorD;

class TVirtualUtilHist : public TNamed {


public:
   TVirtualUtilHist();
   virtual     ~TVirtualUtilHist();
   virtual void  InitStandardFunctions() = 0;
   virtual void  PaintMatrix(const TMatrixFBase &m, Option_t *option) = 0;
   virtual void  PaintMatrix(const TMatrixDBase &m, Option_t *option) = 0;
   virtual void  PaintVector(const TVectorF     &v, Option_t *option) = 0;
   virtual void  PaintVector(const TVectorD     &v, Option_t *option) = 0;

   ClassDef(TVirtualUtilHist,0)  //Abstract interface to the histogram  utilities 
};

#endif
