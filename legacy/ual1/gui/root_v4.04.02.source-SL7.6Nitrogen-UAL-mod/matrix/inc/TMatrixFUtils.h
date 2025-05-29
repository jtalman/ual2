// @(#)root/matrix:$Name:  $:$Id: TMatrixFUtils.h,v 1.11 2005/04/07 14:43:35 rdm Exp $
// Authors: Fons Rademakers, Eddy Offermann   Nov 2003

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TMatrixFUtils
#define ROOT_TMatrixFUtils

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Matrix utility classes.                                              //
//                                                                      //
// This file defines utility classes for the Linear Algebra Package.    //
// The following classes are defined here:                              //
//                                                                      //
// Different matrix views without copying data elements :               //
//   TMatrixFRow_const        TMatrixFRow                               //
//   TMatrixFColumn_const     TMatrixFColumn                            //
//   TMatrixFDiag_const       TMatrixFDiag                              //
//   TMatrixFFlat_const       TMatrixFFlat                              //
//   TMatrixFSub_const        TMatrixFSub                               //
//                                                                      //
//   TElementActionF                                                    //
//   TElementPosActionF                                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TMatrixFBase
#include "TMatrixFBase.h"
#endif

class TVectorF;
class TMatrixFBase;
class TMatrixF;
class TMatrixFSym;

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TElementActionF                                                      //
//                                                                      //
// A class to do a specific operation on every vector or matrix element //
// (regardless of it position) as the object is being traversed.        //
// This is an abstract class. Derived classes need to implement the     //
// action function Operation().                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TElementActionF {

friend class TMatrixFBase;
friend class TMatrixF;
friend class TMatrixFSym;
friend class TVectorF;

protected:
  virtual ~TElementActionF() { }
  virtual void Operation(Float_t  &element) const = 0;

private:
  void operator=(const TElementActionF &) { }
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TElementPosActionF                                                   //
//                                                                      //
// A class to do a specific operation on every vector or matrix element //
// as the object is being traversed. This is an abstract class.         //
// Derived classes need to implement the action function Operation().   //
// In the action function the location of the current element is        //
// known (fI=row, fJ=columns).                                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TElementPosActionF {

friend class TMatrixFBase;
friend class TMatrixF;
friend class TMatrixFSym;
friend class TVectorF;

protected:
  mutable Int_t fI; // i position of element being passed to Operation()
  mutable Int_t fJ; // j position of element being passed to Operation()
  virtual ~TElementPosActionF() { }
  virtual void Operation(Float_t  &element) const = 0;

private:
  void operator=(const TElementPosActionF &) { }
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMatrixFRow_const                                                    //
//                                                                      //
// Class represents a row of a TMatrixFBase                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TMatrixFRow_const {

protected:
  const TMatrixFBase *fMatrix;  //  the matrix I am a row of
        Int_t         fRowInd;  //  effective row index
        Int_t         fInc;     //  if ptr = @a[row,i], then ptr+inc = @a[row,i+1]
  const Float_t      *fPtr;     //  pointer to the a[row,0]

public:
  TMatrixFRow_const() { fMatrix = 0; fInc = 0; fPtr = 0; }
  TMatrixFRow_const(const TMatrixF    &matrix,Int_t row);
  TMatrixFRow_const(const TMatrixFSym &matrix,Int_t row);
  virtual ~TMatrixFRow_const() { }

  inline const TMatrixFBase *GetMatrix  () const { return fMatrix; }
  inline       Int_t         GetRowIndex() const { return fRowInd; }
  inline       Int_t         GetInc     () const { return fInc; }
  inline const Float_t      *GetPtr     () const { return fPtr; }
  inline const Float_t      &operator   ()(Int_t i) const { Assert(fMatrix->IsValid());
                                                            const Int_t acoln = i-fMatrix->GetColLwb();
                                                            Assert(acoln < fMatrix->GetNcols() && acoln >= 0);
                                                            return fPtr[acoln]; }
  inline const Float_t      &operator [](Int_t i) const { return (*(const TMatrixFRow_const *)this)(i); }

  ClassDef(TMatrixFRow_const,0)  // One row of a dense matrix (single precision)
};

class TMatrixFRow : public TMatrixFRow_const {

public:
  TMatrixFRow() {}
  TMatrixFRow(TMatrixF    &matrix,Int_t row);
  TMatrixFRow(TMatrixFSym &matrix,Int_t row);
  TMatrixFRow(const TMatrixFRow &mr);

  inline Float_t  *GetPtr() const { return const_cast<Float_t  *>(fPtr); }

  inline const Float_t  &operator()(Int_t i) const { Assert(fMatrix->IsValid());
                                                     const Int_t acoln = i-fMatrix->GetColLwb();
                                                     Assert(acoln < fMatrix->GetNcols() && acoln >= 0);
                                                     return fPtr[acoln]; }
  inline       Float_t  &operator()(Int_t i)       { Assert(fMatrix->IsValid());
                                                     const Int_t acoln = i-fMatrix->GetColLwb();
                                                     Assert(acoln < fMatrix->GetNcols() && acoln >= 0);
                                                     return (const_cast<Float_t  *>(fPtr))[acoln]; }
  inline const Float_t  &operator[](Int_t i) const { return (*(const TMatrixFRow *)this)(i); }
  inline       Float_t  &operator[](Int_t i)       { return (*(      TMatrixFRow *)this)(i); }

  void operator= (Float_t  val);
  void operator+=(Float_t  val);
  void operator*=(Float_t  val);

  void operator=(const TMatrixFRow_const &r);
  void operator=(const TMatrixFRow       &r) { operator=((TMatrixFRow_const &)r); }
  void operator=(const TVectorF          &vec);

  void operator+=(const TMatrixFRow_const &r);
  void operator*=(const TMatrixFRow_const &r);

  ClassDef(TMatrixFRow,0)  // One row of a dense matrix (single precision)
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMatrixFColumn_const                                                 //
//                                                                      //
// Class represents a column of a TMatrixFBase                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TMatrixFColumn_const {

protected:
  const TMatrixFBase *fMatrix;  //  the matrix I am a column of
        Int_t         fColInd;  //  effective column index
        Int_t         fInc;     //  if ptr = @a[i,col], then ptr+inc = @a[i+1,col]
  const Float_t      *fPtr;     //  pointer to the a[0,col] column

public:
  TMatrixFColumn_const() { fMatrix = 0; fInc = 0; fPtr = 0; }
  TMatrixFColumn_const(const TMatrixF    &matrix,Int_t col);
  TMatrixFColumn_const(const TMatrixFSym &matrix,Int_t col);
  virtual ~TMatrixFColumn_const() { }

  inline const TMatrixFBase *GetMatrix  () const { return fMatrix; }
  inline       Int_t         GetColIndex() const { return fColInd; }
  inline       Int_t         GetInc     () const { return fInc; }
  inline const Float_t      *GetPtr     () const { return fPtr; }
  inline const Float_t      &operator   ()(Int_t i) const { Assert(fMatrix->IsValid());
                                                            const Int_t arown = i-fMatrix->GetRowLwb();
                                                            Assert(arown < fMatrix->GetNrows() && arown >= 0);
                                                            return fPtr[arown*fInc]; }
  inline const Float_t      &operator [](Int_t i) const { return (*(const TMatrixFColumn_const *)this)(i); }

  ClassDef(TMatrixFColumn_const,0)  // One column of a dense matrix (single precision)
};

class TMatrixFColumn : public TMatrixFColumn_const {

public:
  TMatrixFColumn() {}
  TMatrixFColumn(TMatrixF    &matrix,Int_t col);
  TMatrixFColumn(TMatrixFSym &matrix,Int_t col);
  TMatrixFColumn(const TMatrixFColumn &mc);

  inline Float_t  *GetPtr() const { return const_cast<Float_t  *>(fPtr); }

  inline const Float_t  &operator()(Int_t i) const { Assert(fMatrix->IsValid());
                                                     const Int_t arown = i-fMatrix->GetRowLwb();
                                                     Assert(arown < fMatrix->GetNrows() && arown >= 0);
                                                     return fPtr[arown]; }
  inline       Float_t  &operator()(Int_t i)       { Assert(fMatrix->IsValid());
                                                     const Int_t arown = i-fMatrix->GetRowLwb();
                                                     Assert(arown < fMatrix->GetNrows() && arown >= 0);
                                                     return (const_cast<Float_t  *>(fPtr))[arown*fInc]; }
  inline const Float_t  &operator[](Int_t i) const { return (*(const TMatrixFColumn *)this)(i); }
  inline       Float_t  &operator[](Int_t i)       { return (*(      TMatrixFColumn *)this)(i); }

  void operator= (Float_t  val);
  void operator+=(Float_t  val);
  void operator*=(Float_t  val);

  void operator=(const TMatrixFColumn_const &c);
  void operator=(const TMatrixFColumn       &c) { operator=((TMatrixFColumn_const &)c); }
  void operator=(const TVectorF             &vec);

  void operator+=(const TMatrixFColumn_const &c);
  void operator*=(const TMatrixFColumn_const &c);

  ClassDef(TMatrixFColumn,0)  // One column of a dense matrix (single precision)
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMatrixFDiag_const                                                   //
//                                                                      //
// Class represents the diagonal of a matrix (for easy manipulation).   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TMatrixFDiag_const {

protected:
  const TMatrixFBase *fMatrix;  //  the matrix I am the diagonal of
        Int_t         fInc;     //  if ptr=@a[i,i], then ptr+inc = @a[i+1,i+1]
        Int_t         fNdiag;   //  number of diag elems, min(nrows,ncols)
  const Float_t      *fPtr;     //  pointer to the a[0,0]

public:
  TMatrixFDiag_const() { fMatrix = 0; fInc = 0; fNdiag = 0; fPtr = 0; }
  TMatrixFDiag_const(const TMatrixF    &matrix);
  TMatrixFDiag_const(const TMatrixFSym &matrix);
  virtual ~TMatrixFDiag_const() { }

  inline const TMatrixFBase *GetMatrix() const { return fMatrix; }
  inline const Float_t      *GetPtr   () const { return fPtr; }
  inline       Int_t         GetInc   () const { return fInc; }
  inline const Float_t      &operator ()(Int_t i) const { Assert(fMatrix->IsValid());
                                                          Assert(i < fNdiag && i >= 0); return fPtr[i*fInc]; }
  inline const Float_t      &operator [](Int_t i) const { return (*(const TMatrixFDiag_const *)this)(i); }

  Int_t GetNdiags() const { return fNdiag; }

  ClassDef(TMatrixFDiag_const,0)  // Diagonal of a dense matrix (single  precision)
};

class TMatrixFDiag : public TMatrixFDiag_const {

public:
  TMatrixFDiag() {}
  TMatrixFDiag(TMatrixF    &matrix);
  TMatrixFDiag(TMatrixFSym &matrix);
  TMatrixFDiag(const TMatrixFDiag &md);

  inline Float_t  *GetPtr() const { return const_cast<Float_t  *>(fPtr); }

  inline const Float_t  &operator()(Int_t i) const { Assert(fMatrix->IsValid());
                                                     Assert(i < fNdiag && i >= 0); return fPtr[i*fInc]; }
  inline       Float_t  &operator()(Int_t i)       { Assert(fMatrix->IsValid());
                                                     Assert(i < fNdiag && i >= 0);
                                                     return (const_cast<Float_t  *>(fPtr))[i*fInc]; }
  inline const Float_t  &operator[](Int_t i) const { return (*(const TMatrixFDiag *)this)(i); }
  inline       Float_t  &operator[](Int_t i)       { return (*(      TMatrixFDiag *)this)(i); }

  void operator= (Float_t  val);
  void operator+=(Float_t  val);
  void operator*=(Float_t  val);

  void operator=(const TMatrixFDiag_const &d);
  void operator=(const TMatrixFDiag       &d) { operator=((TMatrixFDiag_const &)d); }
  void operator=(const TVectorF           &vec);

  void operator+=(const TMatrixFDiag_const &d);
  void operator*=(const TMatrixFDiag_const &d);

  ClassDef(TMatrixFDiag,0)  // Diagonal of a dense matrix (single  precision)
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMatrixFFlat_const                                                   //
//                                                                      //
// Class represents a flat matrix (for easy manipulation).              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TMatrixFFlat_const {

protected:
  const TMatrixFBase *fMatrix;  //  the matrix I am the diagonal of
        Int_t         fNelems;  //
  const Float_t      *fPtr;     //  pointer to the a[0,0]

public:
  TMatrixFFlat_const() { fMatrix = 0; fNelems = 0; fPtr = 0; }
  TMatrixFFlat_const(const TMatrixF    &matrix);
  TMatrixFFlat_const(const TMatrixFSym &matrix);
  virtual ~TMatrixFFlat_const() { }

  inline const TMatrixFBase *GetMatrix() const { return fMatrix; }
  inline const Float_t      *GetPtr   () const { return fPtr; }
  inline const Float_t      &operator ()(Int_t i) const { Assert(fMatrix->IsValid());
                                                          Assert(i >=0 && i < fNelems); return fPtr[i]; }
  inline const Float_t      &operator [](Int_t i) const { return (*(const TMatrixFFlat_const *)this)(i); }

  ClassDef(TMatrixFFlat_const,0)  // Flat representation of a dense matrix
};

class TMatrixFFlat : public TMatrixFFlat_const {

public:
  TMatrixFFlat() {}
  TMatrixFFlat(TMatrixF    &matrix);
  TMatrixFFlat(TMatrixFSym &matrix);
  TMatrixFFlat(const TMatrixFFlat &mf);

  inline Float_t  *GetPtr() const { return const_cast<Float_t  *>(fPtr); }

  inline const Float_t  &operator()(Int_t i) const { Assert(fMatrix->IsValid());
                                                     Assert(i >=0 && i < fNelems); return fPtr[i]; }
  inline       Float_t  &operator()(Int_t i)       { Assert(fMatrix->IsValid());
                                                     Assert(i >=0 && i < fNelems);
                                                     return (const_cast<Float_t  *>(fPtr))[i]; }
  inline const Float_t  &operator[](Int_t i) const { return (*(const TMatrixFFlat *)this)(i); }
  inline       Float_t  &operator[](Int_t i)       { return (*(      TMatrixFFlat *)this)(i); }

  void operator= (Float_t  val);
  void operator+=(Float_t  val);
  void operator*=(Float_t  val);

  void operator=(const TMatrixFFlat_const &f);
  void operator=(const TMatrixFFlat       &f) { operator=((TMatrixFFlat_const &)f); }
  void operator=(const TVectorF           &vec);

  void operator+=(const TMatrixFFlat_const &f);
  void operator*=(const TMatrixFFlat_const &f);

  ClassDef(TMatrixFFlat,0)  // Flat representation of a dense matrix
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMatrixFSub_const                                                    //
//                                                                      //
// Class represents a sub matrix of a TMatrixFBase                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TMatrixFSub_const {

protected:
  const TMatrixFBase *fMatrix;    //  the matrix I am a submatrix of
        Int_t         fRowOff;    //
        Int_t         fColOff;    //
        Int_t         fNrowsSub;  //
        Int_t         fNcolsSub;  //

  enum {kWorkMax = 100};

public:
  TMatrixFSub_const() { fRowOff = fColOff = fNrowsSub = fNcolsSub = 0; fMatrix = 0; }
  TMatrixFSub_const(const TMatrixF    &matrix,Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb);
  TMatrixFSub_const(const TMatrixFSym &matrix,Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb);
  virtual ~TMatrixFSub_const() { }

  inline const TMatrixFBase *GetMatrix() const { return fMatrix; }
  inline       Int_t         GetRowOff() const { return fRowOff; }
  inline       Int_t         GetColOff() const { return fColOff; }
  inline       Int_t         GetNrows () const { return fNrowsSub; }
  inline       Int_t         GetNcols () const { return fNcolsSub; }
  inline const Float_t      &operator ()(Int_t rown,Int_t coln) const
                                                    { Assert(fMatrix->IsValid());
                                                      Assert(rown < fNrowsSub && rown >= 0);
                                                      Assert(coln < fNcolsSub && coln >= 0);
                                                      const Int_t index = (rown+fRowOff)*fMatrix->GetNcols()+coln+fRowOff;
                                                      const Float_t  *ptr = fMatrix->GetMatrixArray();
                                                      return ptr[index]; }

  ClassDef(TMatrixFSub_const,0)  // sub matrix (single precision)
};

class TMatrixFSub : public TMatrixFSub_const {

public:
  TMatrixFSub() {}
  TMatrixFSub(TMatrixF    &matrix,Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb);
  TMatrixFSub(TMatrixFSym &matrix,Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb);
  TMatrixFSub(const TMatrixFSub &ms);

  inline       Float_t  &operator()(Int_t rown,Int_t coln)
                                                    { Assert(fMatrix->IsValid());
                                                      Assert(rown < fNrowsSub && rown >= 0);
                                                      Assert(coln < fNcolsSub && coln >= 0);
                                                      const Int_t index = (rown+fRowOff)*fMatrix->GetNcols()+coln+fRowOff;
                                                      const Float_t  *ptr = fMatrix->GetMatrixArray();
                                                      return (const_cast<Float_t  *>(ptr))[index]; }

  void Rank1Update(const TVectorF &vec,Float_t alpha=1.0);

  void operator= (Float_t  val);
  void operator+=(Float_t  val);
  void operator*=(Float_t  val);

  void operator=(const TMatrixFSub_const &s);
  void operator=(const TMatrixFSub       &s) { operator=((TMatrixFSub_const &)s); }
  void operator=(const TMatrixFBase      &m);

  void operator+=(const TMatrixFSub_const &s);
  void operator*=(const TMatrixFSub_const &s);
  void operator+=(const TMatrixFBase      &m);
  void operator*=(const TMatrixF          &m);
  void operator*=(const TMatrixFSym       &m);

  ClassDef(TMatrixFSub,0)  // sub matrix (single precision)
};

class TMatrixRow : public TMatrixFRow {
public :
  TMatrixRow() {}
  TMatrixRow(TMatrixF    &matrix,Int_t row) : TMatrixFRow(matrix,row) {}
  TMatrixRow(TMatrixFSym &matrix,Int_t row) : TMatrixFRow(matrix,row) {}
  TMatrixRow(const TMatrixFRow &mr) : TMatrixFRow(mr) {}
  virtual ~TMatrixRow() {}
  ClassDef(TMatrixRow,0)  // One row of a matrix (single precision)
};

class TMatrixColumn : public TMatrixFColumn {
public :
  TMatrixColumn() {}
  TMatrixColumn(TMatrixF    &matrix,Int_t column) : TMatrixFColumn(matrix,column) {}
  TMatrixColumn(TMatrixFSym &matrix,Int_t column) : TMatrixFColumn(matrix,column) {}
  TMatrixColumn(const TMatrixFColumn &mc) : TMatrixFColumn(mc) {}

  virtual ~TMatrixColumn() {}
  ClassDef(TMatrixColumn,0)  // One column of a matrix (single precision)
};

class TMatrixDiag : public TMatrixFDiag {
public :
  TMatrixDiag() {}
  TMatrixDiag(TMatrixF    &matrix) : TMatrixFDiag(matrix) {}
  TMatrixDiag(TMatrixFSym &matrix) : TMatrixFDiag(matrix) {}
  TMatrixDiag(const TMatrixFDiag &md) : TMatrixFDiag(md) {}
  virtual ~TMatrixDiag() {}
  ClassDef(TMatrixDiag,0)  // Diagonal of a matrix (single precision)
};

class TMatrixFlat : public TMatrixFFlat {
public :
  TMatrixFlat() {}
  TMatrixFlat(TMatrixF    &matrix) : TMatrixFFlat(matrix) {}
  TMatrixFlat(TMatrixFSym &matrix) : TMatrixFFlat(matrix) {}
  TMatrixFlat(const TMatrixFFlat &mf) : TMatrixFFlat(mf) {}
  virtual ~TMatrixFlat() {}
  ClassDef(TMatrixFlat,0)  // Flat representation of a matrix
};

Float_t Frand(Double_t &ix);
#endif
