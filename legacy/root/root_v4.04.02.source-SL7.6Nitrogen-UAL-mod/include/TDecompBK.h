// @(#)root/matrix:$Name:  $:$Id: TDecompBK.h,v 1.1 2004/10/16 18:09:16 brun Exp $
// Authors: Fons Rademakers, Eddy Offermann   Sep 2004

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TDecompBK
#define ROOT_TDecompBK

///////////////////////////////////////////////////////////////////////////
//                                                                       //
// Bunch-Kaufman Decomposition class                                     //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TDecompBase
#include "TDecompBase.h"
#endif

class TDecompBK : public TDecompBase
{
protected :

  Int_t     fNIpiv;    // size of row permutation index
  Int_t    *fIpiv;     //[fNIpiv] row permutation index
  TMatrixD  fU;        // decomposed matrix so that a = u d u^T

  virtual const TMatrixDBase &GetDecompMatrix() const { return fU; }

public :

  TDecompBK();
  explicit TDecompBK(Int_t nrows);
  TDecompBK(Int_t row_lwb,Int_t row_upb);
  TDecompBK(const TMatrixDSym &m,Double_t tol = 0.0);
  TDecompBK(const TDecompBK &another);
  virtual ~TDecompBK() {if (fIpiv) delete [] fIpiv; fIpiv = 0; }

  virtual       Int_t     GetNrows  () const { return fU.GetNrows(); }
  virtual       Int_t     GetNcols  () const { return fU.GetNcols(); }
          const TMatrixD &GetU      ()       { if ( !TestBit(kDecomposed) ) Decompose();
                                                  return fU; }

  virtual       void      SetMatrix (const TMatrixDSym &a);

  virtual Bool_t   Decompose  ();    
  virtual Bool_t   Solve      (      TVectorD &b);
  virtual TVectorD Solve      (const TVectorD& b,Bool_t &ok) { TVectorD x = b; ok = Solve(x); return x; }
  virtual Bool_t   Solve      (      TMatrixDColumn &b);     
  virtual Bool_t   TransSolve (      TVectorD &b)            { return Solve(b); } 
  virtual TVectorD TransSolve (const TVectorD& b,Bool_t &ok) { TVectorD x = b; ok = Solve(x); return x; }
  virtual Bool_t   TransSolve (      TMatrixDColumn &b)      { return Solve(b); }
  virtual void     Det        (Double_t &/*d1*/,Double_t &/*d2*/)
                               { MayNotUse("Det(Double_t&,Double_t&)"); }

          void        Invert  (TMatrixDSym &inv);
          TMatrixDSym Invert  ();

  void Print(Option_t *opt ="") const; // *MENU*

  TDecompBK &operator= (const TDecompBK &source);

  ClassDef(TDecompBK,1) // Matrix Decomposition Bunch-Kaufman
};

#endif
