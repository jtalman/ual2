// @(#)root/matrix:$Name:  $:$Id: TMatrixF.h,v 1.17 2005/01/06 06:37:14 brun Exp $
// Authors: Fons Rademakers, Eddy Offermann   Nov 2003

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TMatrixF
#define ROOT_TMatrixF

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMatrixF                                                             //
//                                                                      //
// Implementation of a general matrix in the linear algebra package     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TMatrixFBase
#include "TMatrixFBase.h"
#endif
#ifndef ROOT_TMatrixFUtils
#include "TMatrixFUtils.h"
#endif

#ifdef CBLAS
#include <vecLib/vBLAS.h>
//#include <cblas.h>
#endif

class TMatrixD;
class TMatrixFSym;
class TMatrixFLazy;

class TMatrixF : public TMatrixFBase {

protected:

  Float_t  fDataStack[kSizeMax]; //! data container
  Float_t *fElements;            //[fNelems] elements themselves

          Float_t *New_m   (Int_t size);
          void     Delete_m(Int_t size,Float_t*&);
          Int_t    Memcpy_m(Float_t *newp,const Float_t *oldp,Int_t copySize,
                             Int_t newSize,Int_t oldSize);
  virtual void     Allocate(Int_t nrows,Int_t ncols,Int_t row_lwb = 0,Int_t col_lwb = 0,Int_t init = 0,
                             Int_t nr_nonzeros = -1);

  // Elementary constructors
  void AMultB (const TMatrixF     &a,const TMatrixF    &b,Int_t constr=1);
  void AMultB (const TMatrixF     &a,const TMatrixFSym &b,Int_t constr=1);
  void AMultB (const TMatrixFSym  &a,const TMatrixF    &b,Int_t constr=1);
  void AMultB (const TMatrixFSym  &a,const TMatrixFSym &b,Int_t constr=1);

  void AtMultB(const TMatrixF     &a,const TMatrixF    &b,Int_t constr=1);
  void AtMultB(const TMatrixF     &a,const TMatrixFSym &b,Int_t constr=1);
  void AtMultB(const TMatrixFSym  &a,const TMatrixF    &b,Int_t constr=1) { AMultB(a,b,constr); }
  void AtMultB(const TMatrixFSym  &a,const TMatrixFSym &b,Int_t constr=1) { AMultB(a,b,constr); }

  void AMultBt(const TMatrixF    &a,const TMatrixF    &b,Int_t constr=1);
  void AMultBt(const TMatrixF    &a,const TMatrixFSym &b,Int_t constr=1) { AMultB(a,b,constr); }
  void AMultBt(const TMatrixFSym &a,const TMatrixF    &b,Int_t constr=1);
  void AMultBt(const TMatrixFSym &a,const TMatrixFSym &b,Int_t constr=1) { AMultB(a,b,constr); }

public:

  TMatrixF() { fElements = 0; }
  TMatrixF(Int_t nrows,Int_t ncols);
  TMatrixF(Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb);
  TMatrixF(Int_t nrows,Int_t ncols,const Float_t *data,Option_t *option="");
  TMatrixF(Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb,const Float_t *data,Option_t *option="");
  TMatrixF(const TMatrixF &another);
  TMatrixF(const TMatrixD &another);
  TMatrixF(const TMatrixFSym  &another);

  TMatrixF(EMatrixCreatorsOp1 op,const TMatrixF &prototype);
  TMatrixF(const TMatrixF     &a,EMatrixCreatorsOp2 op,const TMatrixF &b);
  TMatrixF(const TMatrixF     &a,EMatrixCreatorsOp2 op,const TMatrixFSym  &b);
  TMatrixF(const TMatrixFSym  &a,EMatrixCreatorsOp2 op,const TMatrixF &b);
  TMatrixF(const TMatrixFSym  &a,EMatrixCreatorsOp2 op,const TMatrixFSym  &b);
  TMatrixF(const TMatrixFLazy &lazy_constructor);

  virtual ~TMatrixF() { Clear(); }

  virtual const Float_t *GetMatrixArray  () const;
  virtual       Float_t *GetMatrixArray  ();
  virtual const Int_t   *GetRowIndexArray() const { return 0; }
  virtual       Int_t   *GetRowIndexArray()       { return 0; }
  virtual const Int_t   *GetColIndexArray() const { return 0; }
  virtual       Int_t   *GetColIndexArray()       { return 0; }

  virtual       TMatrixFBase &SetRowIndexArray(Int_t * /*data*/) { MayNotUse("SetRowIndexArray(Int_t *)"); return *this; }
  virtual       TMatrixFBase &SetColIndexArray(Int_t * /*data*/) { MayNotUse("SetColIndexArray(Int_t *)"); return *this; }

  virtual void Clear(Option_t * /*option*/ ="") { if (fIsOwner) Delete_m(fNelems,fElements);
                                                  else fElements = 0;  fNelems = 0; }

          TMatrixF     &Use   (Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb,Float_t *data);
          TMatrixF     &Use   (Int_t nrows,Int_t ncols,Float_t *data);
          TMatrixF     &Use   (TMatrixF &a);

  virtual TMatrixFBase &GetSub(Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb,
                               TMatrixFBase &target,Option_t *option="S") const;
          TMatrixF      GetSub(Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb,Option_t *option="S") const;
  virtual TMatrixFBase &SetSub(Int_t row_lwb,Int_t col_lwb,const TMatrixFBase &source);

  virtual TMatrixFBase &ResizeTo(Int_t nrows,Int_t ncols,Int_t nr_nonzeros=-1);
  virtual TMatrixFBase &ResizeTo(Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb,Int_t nr_nonzeros=-1);
  inline  TMatrixFBase &ResizeTo(const TMatrixF &m) {
                                  return ResizeTo(m.GetRowLwb(),m.GetRowUpb(),m.GetColLwb(),m.GetColUpb());
                                }

  virtual Double_t Determinant  () const;
  virtual void     Determinant  (Double_t &d1,Double_t &d2) const;

          TMatrixF &Invert      (Double_t *det=0);
          TMatrixF &InvertFast  (Double_t *det=0);
          TMatrixF &Transpose   (const TMatrixF &source);
  inline  TMatrixF &T           () { return this->Transpose(*this); }
          TMatrixF &Rank1Update (const TVectorF &v,Float_t alpha=1.0);
          TMatrixF &Rank1Update (const TVectorF &v1,const TVectorF &v2,Float_t alpha=1.0);

  TMatrixF &NormByColumn(const TVectorF &v,Option_t *option="D");
  TMatrixF &NormByRow   (const TVectorF &v,Option_t *option="D");

  inline void Mult(const TMatrixF    &a,const TMatrixF    &b) { AMultB(a,b,0); }
  inline void Mult(const TMatrixF    &a,const TMatrixFSym &b) { AMultB(a,b,0); }
  inline void Mult(const TMatrixFSym &a,const TMatrixF    &b) { AMultB(a,b,0); }

  // Either access a_ij as a(i,j)
  inline Float_t            operator()(Int_t rown,Int_t coln) const;
  inline Float_t           &operator()(Int_t rown,Int_t coln);

  // or as a[i][j]
  inline const TMatrixFRow_const  operator[](Int_t rown) const { return TMatrixFRow_const(*this,rown); }
  inline       TMatrixFRow        operator[](Int_t rown)       { return TMatrixFRow      (*this,rown); }

  TMatrixF &operator= (const TMatrixF     &source);
  TMatrixF &operator= (const TMatrixD     &source);
  TMatrixF &operator= (const TMatrixFSym  &source);
  TMatrixF &operator= (const TMatrixFLazy &source);

  TMatrixF &operator= (Float_t val);
  TMatrixF &operator-=(Float_t val);
  TMatrixF &operator+=(Float_t val);
  TMatrixF &operator*=(Float_t val);

  TMatrixF &operator+=(const TMatrixF    &source);
  TMatrixF &operator+=(const TMatrixFSym &source);
  TMatrixF &operator-=(const TMatrixF    &source);
  TMatrixF &operator-=(const TMatrixFSym &source);

  TMatrixF &operator*=(const TMatrixF             &source);
  TMatrixF &operator*=(const TMatrixFSym          &source);
  TMatrixF &operator*=(const TMatrixFDiag_const   &diag);
  TMatrixF &operator/=(const TMatrixFDiag_const   &diag);
  TMatrixF &operator*=(const TMatrixFRow_const    &row);
  TMatrixF &operator/=(const TMatrixFRow_const    &row);
  TMatrixF &operator*=(const TMatrixFColumn_const &col);
  TMatrixF &operator/=(const TMatrixFColumn_const &col);

  const TMatrixF EigenVectors(TVectorF &eigenValues) const;

  ClassDef(TMatrixF,3) // Matrix class (single precision)
};

class TMatrix : public TMatrixF {
public :
  TMatrix() {}
  TMatrix(Int_t nrows,Int_t ncols) : TMatrixF(nrows,ncols) {}
  TMatrix(Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb) :
    TMatrixF(row_lwb,row_upb,col_lwb,col_upb) {}
  TMatrix(Int_t nrows,Int_t ncols,const Float_t *data,Option_t *option="") :
    TMatrixF(nrows,ncols,data,option) {}
  TMatrix(Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb,const Float_t *data,Option_t *option="") :
    TMatrixF(row_lwb,row_upb,col_lwb,col_upb,data,option) {}
  TMatrix(const TMatrixF     &another) : TMatrixF(another) {}
  TMatrix(const TMatrixD     &another) : TMatrixF(another) {}
  TMatrix(const TMatrixFSym  &another) : TMatrixF(another) {}

  TMatrix(EMatrixCreatorsOp1 op,const TMatrixF &prototype)                  : TMatrixF(op,prototype) {}
  TMatrix(const TMatrixF    &a,EMatrixCreatorsOp2 op,const TMatrixF &b)     : TMatrixF(a,op,b) {}
  TMatrix(const TMatrixF    &a,EMatrixCreatorsOp2 op,const TMatrixFSym  &b) : TMatrixF(a,op,b) {}
  TMatrix(const TMatrixFSym &a,EMatrixCreatorsOp2 op,const TMatrixF &b)     : TMatrixF(a,op,b) {}
  TMatrix(const TMatrixFSym &a,EMatrixCreatorsOp2 op,const TMatrixFSym  &b) : TMatrixF(a,op,b) {}
  TMatrix(const TMatrixFLazy &lazy_constructor)                             : TMatrixF(lazy_constructor) {}

  virtual ~TMatrix() {}
  ClassDef(TMatrix,3)  // Matrix class (single precision)
};

inline const Float_t  *TMatrixF::GetMatrixArray() const { return fElements; }
inline       Float_t  *TMatrixF::GetMatrixArray()       { return fElements; }
inline       TMatrixF &TMatrixF::Use           (Int_t nrows,Int_t ncols,Float_t *data)
                                                        { return Use(0,nrows-1,0,ncols-1,data); }
inline       TMatrixF &TMatrixF::Use           (TMatrixF &a)
                                                        {
                                                          Assert(a.IsValid());
                                                          return Use(a.GetRowLwb(),a.GetRowUpb(),
                                                                     a.GetColLwb(),a.GetColUpb(),a.GetMatrixArray());
                                                        }
inline       TMatrixF  TMatrixF::GetSub        (Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb,
                                                Option_t *option) const
                                                        {
                                                          TMatrixF tmp;
                                                          this->GetSub(row_lwb,row_upb,col_lwb,col_upb,tmp,option);
                                                          return tmp;
                                                        }

inline Float_t TMatrixF::operator()(Int_t rown,Int_t coln) const
{
  Assert(IsValid());
  const Int_t arown = rown-fRowLwb;
  const Int_t acoln = coln-fColLwb;
  Assert(arown < fNrows && arown >= 0);
  Assert(acoln < fNcols && acoln >= 0);
  return (fElements[arown*fNcols+acoln]);
}

inline Float_t &TMatrixF::operator()(Int_t rown,Int_t coln)
{
  Assert(IsValid());
  const Int_t arown = rown-fRowLwb;
  const Int_t acoln = coln-fColLwb;
  Assert(arown < fNrows && arown >= 0);
  Assert(acoln < fNcols && acoln >= 0);
  return (fElements[arown*fNcols+acoln]);
}

TMatrixF  operator+  (const TMatrixF    &source1,const TMatrixF    &source2);
TMatrixF  operator+  (const TMatrixF    &source1,const TMatrixFSym &source2);
TMatrixF  operator+  (const TMatrixFSym &source1,const TMatrixF    &source2);
TMatrixF  operator+  (const TMatrixF    &source ,      Float_t      val    );
TMatrixF  operator+  (      Float_t      val    ,const TMatrixF    &source );
TMatrixF  operator-  (const TMatrixF    &source1,const TMatrixF    &source2);
TMatrixF  operator-  (const TMatrixF    &source1,const TMatrixFSym &source2);
TMatrixF  operator-  (const TMatrixFSym &source1,const TMatrixF    &source2);
TMatrixF  operator-  (const TMatrixF    &source ,      Float_t      val    );
TMatrixF  operator-  (      Float_t      val    ,const TMatrixF    &source );
TMatrixF  operator*  (      Float_t      val    ,const TMatrixF    &source );
TMatrixF  operator*  (const TMatrixF    &source ,       Float_t     val    );
TMatrixF  operator*  (const TMatrixF    &source1,const TMatrixF    &source2);
TMatrixF  operator*  (const TMatrixF    &source1,const TMatrixFSym &source2);
TMatrixF  operator*  (const TMatrixFSym &source1,const TMatrixF    &source2);
TMatrixF  operator*  (const TMatrixFSym &source1,const TMatrixFSym &source2);
TMatrixF  operator&& (const TMatrixF    &source1,const TMatrixF    &source2);
TMatrixF  operator&& (const TMatrixF    &source1,const TMatrixFSym &source2);
TMatrixF  operator&& (const TMatrixFSym &source1,const TMatrixF    &source2);
TMatrixF  operator|| (const TMatrixF    &source1,const TMatrixF    &source2);
TMatrixF  operator|| (const TMatrixF    &source1,const TMatrixFSym &source2);
TMatrixF  operator|| (const TMatrixFSym &source1,const TMatrixF    &source2);
TMatrixF  operator>  (const TMatrixF    &source1,const TMatrixF    &source2);
TMatrixF  operator>  (const TMatrixF    &source1,const TMatrixFSym &source2);
TMatrixF  operator>  (const TMatrixFSym &source1,const TMatrixF    &source2);
TMatrixF  operator>= (const TMatrixF    &source1,const TMatrixF    &source2);
TMatrixF  operator>= (const TMatrixF    &source1,const TMatrixFSym &source2);
TMatrixF  operator>= (const TMatrixFSym &source1,const TMatrixF    &source2);
TMatrixF  operator<= (const TMatrixF    &source1,const TMatrixF    &source2);
TMatrixF  operator<= (const TMatrixF    &source1,const TMatrixFSym &source2);
TMatrixF  operator<= (const TMatrixFSym &source1,const TMatrixF    &source2);
TMatrixF  operator<  (const TMatrixF    &source1,const TMatrixF    &source2);
TMatrixF  operator<  (const TMatrixF    &source1,const TMatrixFSym &source2);
TMatrixF  operator<  (const TMatrixFSym &source1,const TMatrixF    &source2);
TMatrixF  operator!= (const TMatrixF    &source1,const TMatrixF    &source2);
TMatrixF  operator!= (const TMatrixF    &source1,const TMatrixFSym &source2);
TMatrixF  operator!= (const TMatrixFSym &source1,const TMatrixF    &source2);

TMatrixF &Add        (TMatrixF &target,       Float_t     scalar,const TMatrixF    &source);
TMatrixF &Add        (TMatrixF &target,       Float_t     scalar,const TMatrixFSym &source);
TMatrixF &ElementMult(TMatrixF &target,const TMatrixF    &source);
TMatrixF &ElementMult(TMatrixF &target,const TMatrixFSym &source);
TMatrixF &ElementDiv (TMatrixF &target,const TMatrixF    &source);
TMatrixF &ElementDiv (TMatrixF &target,const TMatrixFSym &source);

#endif
