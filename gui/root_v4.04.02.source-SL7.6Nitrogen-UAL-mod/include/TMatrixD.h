// @(#)root/matrix:$Name:  $:$Id: TMatrixD.h,v 1.43 2005/01/06 06:37:14 brun Exp $
// Authors: Fons Rademakers, Eddy Offermann   Nov 2003

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TMatrixD
#define ROOT_TMatrixD

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMatrixD                                                             //
//                                                                      //
// Implementation of a general matrix in the linear algebra package     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TMatrixDBase
#include "TMatrixDBase.h"
#endif
#ifndef ROOT_TMatrixDUtils
#include "TMatrixDUtils.h"
#endif

#ifdef CBLAS
#include <vecLib/vBLAS.h>
//#include <cblas.h>
#endif

class TMatrixF;
class TMatrixDSym;
class TMatrixDSparse;
class TMatrixDLazy;

class TMatrixD : public TMatrixDBase {

protected:

  Double_t  fDataStack[kSizeMax]; //! data container
  Double_t *fElements;            //[fNelems] elements themselves

          Double_t *New_m   (Int_t size);
          void      Delete_m(Int_t size,Double_t*&);
          Int_t     Memcpy_m(Double_t *newp,const Double_t *oldp,Int_t copySize,
                              Int_t newSize,Int_t oldSize);
  virtual void      Allocate(Int_t nrows,Int_t ncols,Int_t row_lwb = 0,Int_t col_lwb = 0,Int_t init = 0,
                             Int_t nr_nonzeros = -1);

  // Elementary constructors
  void AMultB (const TMatrixD    &a,const TMatrixD    &b,Int_t constr=1);
  void AMultB (const TMatrixD    &a,const TMatrixDSym &b,Int_t constr=1);
  void AMultB (const TMatrixDSym &a,const TMatrixD    &b,Int_t constr=1);
  void AMultB (const TMatrixDSym &a,const TMatrixDSym &b,Int_t constr=1);

  void AtMultB(const TMatrixD    &a,const TMatrixD    &b,Int_t constr=1);
  void AtMultB(const TMatrixD    &a,const TMatrixDSym &b,Int_t constr=1);
  void AtMultB(const TMatrixDSym &a,const TMatrixD    &b,Int_t constr=1) { AMultB(a,b,constr); }
  void AtMultB(const TMatrixDSym &a,const TMatrixDSym &b,Int_t constr=1) { AMultB(a,b,constr); }

  void AMultBt(const TMatrixD    &a,const TMatrixD    &b,Int_t constr=1);
  void AMultBt(const TMatrixD    &a,const TMatrixDSym &b,Int_t constr=1) { AMultB(a,b,constr); }
  void AMultBt(const TMatrixDSym &a,const TMatrixD    &b,Int_t constr=1);
  void AMultBt(const TMatrixDSym &a,const TMatrixDSym &b,Int_t constr=1) { AMultB(a,b,constr); }

public:

  TMatrixD() { fElements = 0; }
  TMatrixD(Int_t nrows,Int_t ncols);
  TMatrixD(Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb);
  TMatrixD(Int_t nrows,Int_t ncols,const Double_t *data,Option_t *option="");
  TMatrixD(Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb,const Double_t *data,Option_t *option="");
  TMatrixD(const TMatrixD       &another);
  TMatrixD(const TMatrixF       &another);
  TMatrixD(const TMatrixDSym    &another);
  TMatrixD(const TMatrixDSparse &another);

  TMatrixD(EMatrixCreatorsOp1 op,const TMatrixD &prototype);
  TMatrixD(const TMatrixD     &a,EMatrixCreatorsOp2 op,const TMatrixD    &b);
  TMatrixD(const TMatrixD     &a,EMatrixCreatorsOp2 op,const TMatrixDSym &b);
  TMatrixD(const TMatrixDSym  &a,EMatrixCreatorsOp2 op,const TMatrixD    &b);
  TMatrixD(const TMatrixDSym  &a,EMatrixCreatorsOp2 op,const TMatrixDSym &b);
  TMatrixD(const TMatrixDLazy &lazy_constructor);

  virtual ~TMatrixD() { Clear(); }

  virtual const Double_t *GetMatrixArray  () const;
  virtual       Double_t *GetMatrixArray  ();
  virtual const Int_t    *GetRowIndexArray() const { return 0; }
  virtual       Int_t    *GetRowIndexArray()       { return 0; }
  virtual const Int_t    *GetColIndexArray() const { return 0; }
  virtual       Int_t    *GetColIndexArray()       { return 0; }

  virtual       TMatrixDBase &SetRowIndexArray(Int_t * /*data*/) { MayNotUse("SetRowIndexArray(Int_t *)"); return *this; }
  virtual       TMatrixDBase &SetColIndexArray(Int_t * /*data*/) { MayNotUse("SetColIndexArray(Int_t *)"); return *this; }

  virtual void Clear(Option_t * /*option*/ ="") { if (fIsOwner) Delete_m(fNelems,fElements);
                                                    else fElements = 0;  fNelems = 0; }

          TMatrixD     &Use     (Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb,Double_t *data);
          TMatrixD     &Use     (Int_t nrows,Int_t ncols,Double_t *data);
          TMatrixD     &Use     (TMatrixD &a);

  virtual TMatrixDBase &GetSub  (Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb,
                                 TMatrixDBase &target,Option_t *option="S") const;
          TMatrixD      GetSub  (Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb,Option_t *option="S") const;
  virtual TMatrixDBase &SetSub  (Int_t row_lwb,Int_t col_lwb,const TMatrixDBase &source);

  virtual TMatrixDBase &ResizeTo(Int_t nrows,Int_t ncols,Int_t nr_nonzeros=-1);
  virtual TMatrixDBase &ResizeTo(Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb,Int_t nr_nonzeros=-1);
  inline  TMatrixDBase &ResizeTo(const TMatrixD &m) {
                                  return ResizeTo(m.GetRowLwb(),m.GetRowUpb(),m.GetColLwb(),m.GetColUpb());
                                }

  virtual Double_t Determinant  () const;
  virtual void     Determinant  (Double_t &d1,Double_t &d2) const;

          TMatrixD &Invert      (Double_t *det=0);
          TMatrixD &InvertFast  (Double_t *det=0);
          TMatrixD &Transpose   (const TMatrixD &source);
  inline  TMatrixD &T           () { return this->Transpose(*this); }
          TMatrixD &Rank1Update (const TVectorD &v,Double_t alpha=1.0);
          TMatrixD &Rank1Update (const TVectorD &v1,const TVectorD &v2,Double_t alpha=1.0);

  TMatrixD &NormByColumn(const TVectorD &v,Option_t *option="D");
  TMatrixD &NormByRow   (const TVectorD &v,Option_t *option="D");

  inline void Mult(const TMatrixD    &a,const TMatrixD    &b) { AMultB(a,b,0); }
  inline void Mult(const TMatrixD    &a,const TMatrixDSym &b) { AMultB(a,b,0); }
  inline void Mult(const TMatrixDSym &a,const TMatrixD    &b) { AMultB(a,b,0); }

  // Either access a_ij as a(i,j)
  inline       Double_t           operator()(Int_t rown,Int_t coln) const;
  inline       Double_t          &operator()(Int_t rown,Int_t coln);

  // or as a[i][j]
  inline const TMatrixDRow_const  operator[](Int_t rown) const { return TMatrixDRow_const(*this,rown); }
  inline       TMatrixDRow        operator[](Int_t rown)       { return TMatrixDRow      (*this,rown); }

  TMatrixD &operator= (const TMatrixD       &source);
  TMatrixD &operator= (const TMatrixF       &source);
  TMatrixD &operator= (const TMatrixDSym    &source);
  TMatrixD &operator= (const TMatrixDSparse &source);
  TMatrixD &operator= (const TMatrixDLazy   &source);

  TMatrixD &operator= (Double_t val);
  TMatrixD &operator-=(Double_t val);
  TMatrixD &operator+=(Double_t val);
  TMatrixD &operator*=(Double_t val);

  TMatrixD &operator+=(const TMatrixD    &source);
  TMatrixD &operator+=(const TMatrixDSym &source);
  TMatrixD &operator-=(const TMatrixD    &source);
  TMatrixD &operator-=(const TMatrixDSym &source);

  TMatrixD &operator*=(const TMatrixD             &source);
  TMatrixD &operator*=(const TMatrixDSym          &source);
  TMatrixD &operator*=(const TMatrixDDiag_const   &diag);
  TMatrixD &operator/=(const TMatrixDDiag_const   &diag);
  TMatrixD &operator*=(const TMatrixDRow_const    &row);
  TMatrixD &operator/=(const TMatrixDRow_const    &row);
  TMatrixD &operator*=(const TMatrixDColumn_const &col);
  TMatrixD &operator/=(const TMatrixDColumn_const &col);

  const TMatrixD EigenVectors(TVectorD &eigenValues) const;

  ClassDef(TMatrixD,3) // Matrix class (double precision)
};

inline const Double_t *TMatrixD::GetMatrixArray() const { return fElements; }
inline       Double_t *TMatrixD::GetMatrixArray()       { return fElements; }
inline       TMatrixD &TMatrixD::Use           (Int_t nrows,Int_t ncols,Double_t *data)
                                                        { return Use(0,nrows-1,0,ncols-1,data); }
inline       TMatrixD &TMatrixD::Use           (TMatrixD &a)
                                                        {
                                                          Assert(a.IsValid());
                                                          return Use(a.GetRowLwb(),a.GetRowUpb(),
                                                                     a.GetColLwb(),a.GetColUpb(),a.GetMatrixArray());
                                                        }
inline       TMatrixD  TMatrixD::GetSub        (Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb,
                                                Option_t *option) const
                                                        {
                                                          TMatrixD tmp;
                                                          this->GetSub(row_lwb,row_upb,col_lwb,col_upb,tmp,option);
                                                          return tmp;
                                                        }

inline Double_t TMatrixD::operator()(Int_t rown,Int_t coln) const
{
  Assert(IsValid());
  const Int_t arown = rown-fRowLwb;
  const Int_t acoln = coln-fColLwb;
  Assert(arown < fNrows && arown >= 0);
  Assert(acoln < fNcols && acoln >= 0);
  return (fElements[arown*fNcols+acoln]);
}

inline Double_t &TMatrixD::operator()(Int_t rown,Int_t coln)
{
  Assert(IsValid());
  const Int_t arown = rown-fRowLwb;
  const Int_t acoln = coln-fColLwb;
  Assert(arown < fNrows && arown >= 0);
  Assert(acoln < fNcols && acoln >= 0);
  return (fElements[arown*fNcols+acoln]);
}

TMatrixD  operator+  (const TMatrixD    &source1,const TMatrixD    &source2);
TMatrixD  operator+  (const TMatrixD    &source1,const TMatrixDSym &source2);
TMatrixD  operator+  (const TMatrixDSym &source1,const TMatrixD    &source2);
TMatrixD  operator+  (const TMatrixD    &source ,      Double_t     val    );
TMatrixD  operator+  (      Double_t     val    ,const TMatrixD    &source );
TMatrixD  operator-  (const TMatrixD    &source1,const TMatrixD    &source2);
TMatrixD  operator-  (const TMatrixD    &source1,const TMatrixDSym &source2);
TMatrixD  operator-  (const TMatrixDSym &source1,const TMatrixD    &source2);
TMatrixD  operator-  (const TMatrixD    &source ,      Double_t     val    );
TMatrixD  operator-  (      Double_t     val    ,const TMatrixD    &source );
TMatrixD  operator*  (      Double_t     val    ,const TMatrixD    &source );
TMatrixD  operator*  (const TMatrixD    &source ,       Double_t    val    );
TMatrixD  operator*  (const TMatrixD    &source1,const TMatrixD    &source2);
TMatrixD  operator*  (const TMatrixD    &source1,const TMatrixDSym &source2);
TMatrixD  operator*  (const TMatrixDSym &source1,const TMatrixD    &source2);
TMatrixD  operator*  (const TMatrixDSym &source1,const TMatrixDSym &source2);
TMatrixD  operator&& (const TMatrixD    &source1,const TMatrixD    &source2);
TMatrixD  operator&& (const TMatrixD    &source1,const TMatrixDSym &source2);
TMatrixD  operator&& (const TMatrixDSym &source1,const TMatrixD    &source2);
TMatrixD  operator|| (const TMatrixD    &source1,const TMatrixD    &source2);
TMatrixD  operator|| (const TMatrixD    &source1,const TMatrixDSym &source2);
TMatrixD  operator|| (const TMatrixDSym &source1,const TMatrixD    &source2);
TMatrixD  operator>  (const TMatrixD    &source1,const TMatrixD    &source2);
TMatrixD  operator>  (const TMatrixD    &source1,const TMatrixDSym &source2);
TMatrixD  operator>  (const TMatrixDSym &source1,const TMatrixD    &source2);
TMatrixD  operator>= (const TMatrixD    &source1,const TMatrixD    &source2);
TMatrixD  operator>= (const TMatrixD    &source1,const TMatrixDSym &source2);
TMatrixD  operator>= (const TMatrixDSym &source1,const TMatrixD    &source2);
TMatrixD  operator<= (const TMatrixD    &source1,const TMatrixD    &source2);
TMatrixD  operator<= (const TMatrixD    &source1,const TMatrixDSym &source2);
TMatrixD  operator<= (const TMatrixDSym &source1,const TMatrixD    &source2);
TMatrixD  operator<  (const TMatrixD    &source1,const TMatrixD    &source2);
TMatrixD  operator<  (const TMatrixD    &source1,const TMatrixDSym &source2);
TMatrixD  operator<  (const TMatrixDSym &source1,const TMatrixD    &source2);
TMatrixD  operator!= (const TMatrixD    &source1,const TMatrixD    &source2);
TMatrixD  operator!= (const TMatrixD    &source1,const TMatrixDSym &source2);
TMatrixD  operator!= (const TMatrixDSym &source1,const TMatrixD    &source2);

TMatrixD &Add        (TMatrixD &target,       Double_t    scalar,const TMatrixD    &source);
TMatrixD &Add        (TMatrixD &target,       Double_t    scalar,const TMatrixDSym &source);
TMatrixD &ElementMult(TMatrixD &target,const TMatrixD    &source);
TMatrixD &ElementMult(TMatrixD &target,const TMatrixDSym &source);
TMatrixD &ElementDiv (TMatrixD &target,const TMatrixD    &source);
TMatrixD &ElementDiv (TMatrixD &target,const TMatrixDSym &source);

#endif
