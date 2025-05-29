// @(#)root/matrix:$Name:  $:$Id: TMatrixFSym.cxx,v 1.20 2005/01/06 06:37:14 brun Exp $
// Authors: Fons Rademakers, Eddy Offermann  Nov 2003

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMatrixFSym                                                          //
//                                                                      //
// Implementation of a symmetric matrix in the linear algebra package   //
//                                                                      //
// Note that in this implementation both matrix element m[i][j] and     //
// m[j][i] are updated and stored in memory . However, when making the  //
// object persistent only the upper right triangle is stored .          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TMatrixFSym.h"
#include "TMatrixF.h"
#include "TMatrixDSym.h"
#include "TMatrixFLazy.h"
#include "TMatrixFSymCramerInv.h"
#include "TDecompLU.h"
#include "TDecompBK.h"
#include "TMatrixDSymEigen.h"
#include "TVectorF.h"

ClassImp(TMatrixFSym)

//______________________________________________________________________________
TMatrixFSym::TMatrixFSym(Int_t no_rows)
{
  Allocate(no_rows,no_rows,0,0,1);
}

//______________________________________________________________________________
TMatrixFSym::TMatrixFSym(Int_t row_lwb,Int_t row_upb)
{
  const Int_t no_rows = row_upb-row_lwb+1;
  Allocate(no_rows,no_rows,row_lwb,row_lwb,1);
}

//______________________________________________________________________________
TMatrixFSym::TMatrixFSym(Int_t no_rows,const Float_t *elements,Option_t *option)
{
  // option="F": array elements contains the matrix stored column-wise
  //             like in Fortran, so a[i,j] = elements[i+no_rows*j],
  // else        it is supposed that array elements are stored row-wise
  //             a[i,j] = elements[i*no_cols+j]
  //
  // array elements are copied

  Allocate(no_rows,no_rows);
  SetMatrixArray(elements,option);
  if (!IsSymmetric()) {
    Error("TMatrixFSym(Int_t,Float_t*,Option_t*)","matrix not symmetric");
    Invalidate();
  }
}

//______________________________________________________________________________
TMatrixFSym::TMatrixFSym(Int_t row_lwb,Int_t row_upb,const Float_t *elements,Option_t *option)
{
  // array elements are copied

  const Int_t no_rows = row_upb-row_lwb+1;
  Allocate(no_rows,no_rows,row_lwb,row_lwb);
  SetMatrixArray(elements,option);
  if (!IsSymmetric()) {
    Error("TMatrixFSym(Int_t,Int_t,Float_t*,Option_t*)","matrix not symmetric");
    Invalidate();
  }
}

//______________________________________________________________________________
TMatrixFSym::TMatrixFSym(const TMatrixFSym &another) : TMatrixFBase(another)
{
  Assert(another.IsValid());
  Allocate(another.GetNrows(),another.GetNcols(),another.GetRowLwb(),another.GetColLwb());
  *this = another;
}

//______________________________________________________________________________
TMatrixFSym::TMatrixFSym(const TMatrixDSym &another)
{
  Assert(another.IsValid());
  Allocate(another.GetNrows(),another.GetNcols(),another.GetRowLwb(),another.GetColLwb());
  *this = another;
}

//______________________________________________________________________________
TMatrixFSym::TMatrixFSym(EMatrixCreatorsOp1 op,const TMatrixFSym &prototype)
{
  // Create a matrix applying a specific operation to the prototype.
  // Example: TMatrixFSym a(10,12); ...; TMatrixFSym b(TMatrixFBase::kTransposed, a);
  // Supported operations are: kZero, kUnit, and kTransposed .

  Assert(this != &prototype);
  Invalidate();
                   
  Assert(prototype.IsValid());
  
  switch(op) {
    case kZero:
      Allocate(prototype.GetNrows(),prototype.GetNcols(),
               prototype.GetRowLwb(),prototype.GetColLwb(),1);
      break;

    case kUnit:
      Allocate(prototype.GetNrows(),prototype.GetNcols(),
               prototype.GetRowLwb(),prototype.GetColLwb(),1);
      UnitMatrix();
      break;

    case kTransposed:
      Allocate(prototype.GetNcols(), prototype.GetNrows(),
               prototype.GetColLwb(),prototype.GetRowLwb());
      Transpose(prototype);
      break;

    case kAtA:
      AtMultA(prototype);
      break;

    default:
      Error("TMatrixFSym(EMatrixCreatorOp1,const TMatrixFSym)",
             "operation %d not yet implemented", op);
  }
}

//______________________________________________________________________________
TMatrixFSym::TMatrixFSym(EMatrixCreatorsOp1 op,const TMatrixF &prototype)
{
  Assert(dynamic_cast<TMatrixF *>(this) != &prototype);
  Invalidate();
                   
  Assert(prototype.IsValid());

  switch(op) {
    case kAtA:
      AtMultA(prototype);
      break;

    default:
      Error("TMatrixFSym(EMatrixCreatorOp1,const TMatrixF)",
             "operation %d not yet implemented", op);
  }
}

//______________________________________________________________________________
TMatrixFSym::TMatrixFSym(const TMatrixFSym &a,EMatrixCreatorsOp2 op,const TMatrixFSym &b)
{    
  Invalidate();
   
  Assert(a.IsValid());
  Assert(b.IsValid());
   
  switch(op) {
    case kPlus:
    {
      Allocate(a.GetNrows(),a.GetRowLwb(),1);
      *this = a;
      *this += b;
      break;
    }

    case kMinus:
    {
      Allocate(a.GetNrows(),a.GetRowLwb(),1);
      *this = a;
      *this -= b;
      break;
    }

    default:
      Error("TMatrixFSym(EMatrixCreatorOp2)", "operation %d not yet implemented", op);
  }
}

//______________________________________________________________________________
TMatrixFSym::TMatrixFSym(const TMatrixFSymLazy &lazy_constructor)
{
  Allocate(lazy_constructor.GetRowUpb()-lazy_constructor.GetRowLwb()+1,
           lazy_constructor.GetRowUpb()-lazy_constructor.GetRowLwb()+1,
           lazy_constructor.GetRowLwb(),lazy_constructor.GetRowLwb(),1);
  lazy_constructor.FillIn(*this);
  if (!IsSymmetric()) {
    Error("TMatrixFSym(TMatrixFSymLazy)","matrix not symmetric");
    Invalidate();
  }
}

//______________________________________________________________________________
void TMatrixFSym::Delete_m(Int_t size,Float_t *&m)
{ 
  // delete data pointer m, if it was assigned on the heap
  
  if (m) {
    if (size > kSizeMax)
      delete [] m;
    m = 0;
  }       
}     
             
//______________________________________________________________________________
Float_t* TMatrixFSym::New_m(Int_t size)
{
  // return data pointer . if requested size <= kSizeMax, assign pointer
  // to the stack space

  if (size == 0) return 0;
  else {
    if ( size <= kSizeMax )
      return fDataStack;
    else {
      Float_t *heap = new Float_t[size];
      return heap;
    }
  }
}

//______________________________________________________________________________
Int_t TMatrixFSym::Memcpy_m(Float_t *newp,const Float_t *oldp,Int_t copySize,
                            Int_t newSize,Int_t oldSize)
{
  // copy copySize doubles from *oldp to *newp . However take care of the
  // situation where both pointers are assigned to the same stack space

  if (copySize == 0 || oldp == newp)
    return 0;
  else {
    if ( newSize <= kSizeMax && oldSize <= kSizeMax ) {
      // both pointers are inside fDataStack, be careful with copy direction !
      if (newp > oldp) {
        for (Int_t i = copySize-1; i >= 0; i--)
          newp[i] = oldp[i];
      } else {
        for (Int_t i = 0; i < copySize; i++)
          newp[i] = oldp[i];
      }
    }
    else
      memcpy(newp,oldp,copySize*sizeof(Float_t));
  }
  return 0;
}

//______________________________________________________________________________
void TMatrixFSym::Allocate(Int_t no_rows,Int_t no_cols,Int_t row_lwb,Int_t col_lwb,
                           Int_t init,Int_t /*nr_nonzeros*/)
{
  // Allocate new matrix. Arguments are number of rows, columns, row
  // lowerbound (0 default) and column lowerbound (0 default).

  if (no_rows < 0 || no_cols < 0)
  {
    Error("Allocate","no_rows=%d no_cols=%d",no_rows,no_cols);
    Invalidate();
    return;
  }

  MakeValid();
  fNrows   = no_rows;
  fNcols   = no_cols;
  fRowLwb  = row_lwb;
  fColLwb  = col_lwb;
  fNelems  = fNrows*fNcols;
  fIsOwner = kTRUE;
  fTol     = DBL_EPSILON;

  if (fNelems > 0) {
    fElements = New_m(fNelems);
    if (init)
      memset(fElements,0,fNelems*sizeof(Float_t));
  } else
    fElements = 0;
}

//______________________________________________________________________________
void TMatrixFSym::AtMultA(const TMatrixF &a,Int_t constr)
{
  // Create a matrix C such that C = A' * A. In other words,
  // c[i,j] = SUM{ a[k,i] * a[k,j] }. Note, matrix C is allocated for constr=1.

  Assert(a.IsValid());

  if (constr)
    Allocate(a.GetNcols(),a.GetNcols(),a.GetColLwb(),a.GetColLwb(),1);

#ifdef CBLAS
  const Float_t *ap = a.GetMatrixArray();
        Float_t *cp = this->GetMatrixArray();
  cblas_sgemm (CblasRowMajor,CblasTrans,CblasNoTrans,fNrows,fNcols,a.GetNrows(),
               1.0,ap,a.GetNcols(),ap,a.GetNcols(),1.0,cp,fNcols);
#else
  const Int_t nb     = a.GetNoElements();
  const Int_t ncolsa = a.GetNcols();
  const Int_t ncolsb = ncolsa;
  const Float_t * const ap = a.GetMatrixArray();
  const Float_t * const bp = ap;
        Float_t *       cp = this->GetMatrixArray();

  const Float_t *acp0 = ap;           // Pointer to  A[i,0];
  while (acp0 < ap+a.GetNcols()) {
    for (const Float_t *bcp = bp; bcp < bp+ncolsb; ) { // Pointer to the j-th column of A, Start bcp = A[0,0]
      const Float_t *acp = acp0;                       // Pointer to the i-th column of A, reset to A[0,i]
      Float_t cij = 0;
      while (bcp < bp+nb) {           // Scan the i-th column of A and
        cij += *acp * *bcp;           // the j-th col of A
        acp += ncolsa;
        bcp += ncolsb;
      }
      *cp++ = cij;
      bcp -= nb-1;                    // Set bcp to the (j+1)-th col
    }
    acp0++;                           // Set acp0 to the (i+1)-th col
  }

  Assert(cp == this->GetMatrixArray()+fNelems && acp0 == ap+ncolsa);
#endif
}

//______________________________________________________________________________
void TMatrixFSym::AtMultA(const TMatrixFSym &a,Int_t constr)
{
  // Matrix multiplication, with A symmetric
  // Create a matrix C such that C = A' * A = A * A = A * A'
  // Note, matrix C is allocated for constr=1.

  Assert(a.IsValid());

  if (constr)
    Allocate(a.GetNcols(),a.GetNcols(),a.GetColLwb(),a.GetColLwb(),1);

  const Float_t *ap1 = a.GetMatrixArray();
  const Float_t *bp1 = ap1;
        Float_t *cp1 = this->GetMatrixArray();

#ifdef CBLAS
  cblas_ssymm (CblasRowMajor,CblasLeft,CblasUpper,fNrows,fNcols,1.0,
               ap1,a.GetNcols(),bp1,a.GetNcols(),0.0,cp1,fNcols);
#else
  const Float_t *ap2 = a.GetMatrixArray();
  const Float_t *bp2 = ap2;
        Float_t *cp2 = this->GetMatrixArray();
  for (Int_t i = 0; i < fNrows; i++) {
    for (Int_t j = 0; j < fNcols; j++) {
      const Float_t b_ij = *bp1++;
      *cp1 += b_ij*(*ap1);
      Float_t tmp = 0.0;
      ap2 = ap1+1;
      for (Int_t k = i+1; k < fNrows; k++) {
        const Int_t index_kj = k*fNcols+j;
        const Float_t a_ik = *ap2++;
        const Float_t b_kj = bp2[index_kj];
        cp2[index_kj] += a_ik*b_ij;
        tmp += a_ik*b_kj;
      }
      *cp1++ += tmp;
    }
    ap1 += fNrows+1;
  }
#endif
}

//______________________________________________________________________________ 
TMatrixFSym &TMatrixFSym::Use(Int_t row_lwb,Int_t row_upb,Float_t *data)
{
  if (row_upb < row_lwb)
  {
    Error("Use","row_upb=%d < row_lwb=%d",row_upb,row_lwb);
    Invalidate();
    return *this;
  }

  Clear();
  fNrows    = row_upb-row_lwb+1;
  fNcols    = fNrows;
  fRowLwb   = row_lwb;
  fColLwb   = row_lwb;
  fNelems   = fNrows*fNcols;
  fElements = data;
  fIsOwner  = kFALSE;

  return *this;
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::GetSub(Int_t row_lwb,Int_t row_upb,TMatrixFSym &target,Option_t *option) const
{
  // Get submatrix [row_lwb..row_upb][row_lwb..row_upb]; The indexing range of the
  // returned matrix depends on the argument option:
  //
  // option == "S" : return [0..row_upb-row_lwb+1][0..row_upb-row_lwb+1] (default)
  // else          : return [row_lwb..row_upb][row_lwb..row_upb]

  Assert(IsValid());

  if (row_lwb < fRowLwb || row_lwb > fRowLwb+fNrows-1) {
    Error("GetSub","row_lwb out of bounds");
    target.Invalidate();
    return target;
  }
  if (row_upb < fRowLwb || row_upb > fRowLwb+fNrows-1) {
    Error("GetSub","row_upb out of bounds");
    target.Invalidate();
    return target;
  }
  if (row_upb < row_lwb) {
    Error("GetSub","row_upb < row_lwb");
    target.Invalidate();
    return target;
  }

  TString opt(option);
  opt.ToUpper();
  const Int_t shift = (opt.Contains("S")) ? 1 : 0;

  Int_t row_lwb_sub;
  Int_t row_upb_sub;
  if (shift) {
    row_lwb_sub = 0;
    row_upb_sub = row_upb-row_lwb;
  } else {
    row_lwb_sub = row_lwb;
    row_upb_sub = row_upb;
  }

  target.ResizeTo(row_lwb_sub,row_upb_sub,row_lwb_sub,row_upb_sub);
  const Int_t nrows_sub = row_upb_sub-row_lwb_sub+1;

  if (target.GetRowIndexArray() && target.GetColIndexArray()) {
    for (Int_t irow = 0; irow < nrows_sub; irow++) {
      for (Int_t icol = 0; icol < nrows_sub; icol++) {
        target(irow+row_lwb_sub,icol+row_lwb_sub) = (*this)(row_lwb+irow,row_lwb+icol);
      }
    }
  } else {
    const Float_t *ap = this->GetMatrixArray()+(row_lwb-fRowLwb)*fNrows+(row_lwb-fRowLwb);
          Float_t *bp = target.GetMatrixArray();

    for (Int_t irow = 0; irow < nrows_sub; irow++) {
      const Float_t *ap_sub = ap;
      for (Int_t icol = 0; icol < nrows_sub; icol++) {
        *bp++ = *ap_sub++;
      }
      ap += fNrows;
    }
  }

  return target;
}

//______________________________________________________________________________
TMatrixFBase &TMatrixFSym::GetSub(Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb,
                                  TMatrixFBase &target,Option_t *option) const
{
  // Get submatrix [row_lwb..row_upb][col_lwb..col_upb]; The indexing range of the
  // returned matrix depends on the argument option:
  //
  // option == "S" : return [0..row_upb-row_lwb+1][0..col_upb-col_lwb+1] (default)
  // else          : return [row_lwb..row_upb][col_lwb..col_upb]

  Assert(IsValid());
  if (row_lwb < fRowLwb || row_lwb > fRowLwb+fNrows-1) {
    Error("GetSub","row_lwb out of bounds");
    target.Invalidate();
    return target;
  }
  if (col_lwb < fColLwb || col_lwb > fColLwb+fNcols-1) {
    Error("GetSub","col_lwb out of bounds");
    target.Invalidate();
    return target;
  }
  if (row_upb < fRowLwb || row_upb > fRowLwb+fNrows-1) {
    Error("GetSub","row_upb out of bounds");
    target.Invalidate();
    return target;
  }
  if (col_upb < fColLwb || col_upb > fColLwb+fNcols-1) {
    Error("GetSub","col_upb out of bounds");
    target.Invalidate();
    return target;
  }
  if (row_upb < row_lwb || col_upb < col_lwb) {
    Error("GetSub","row_upb < row_lwb || col_upb < col_lwb");
    target.Invalidate();
    return target;
  }

  TString opt(option);
  opt.ToUpper();
  const Int_t shift = (opt.Contains("S")) ? 1 : 0;

  const Int_t row_lwb_sub = (shift) ? 0               : row_lwb;
  const Int_t row_upb_sub = (shift) ? row_upb-row_lwb : row_upb;
  const Int_t col_lwb_sub = (shift) ? 0               : col_lwb;
  const Int_t col_upb_sub = (shift) ? col_upb-col_lwb : col_upb;

  target.ResizeTo(row_lwb_sub,row_upb_sub,col_lwb_sub,col_upb_sub);
  const Int_t nrows_sub = row_upb_sub-row_lwb_sub+1;
  const Int_t ncols_sub = col_upb_sub-col_lwb_sub+1;

  if (target.GetRowIndexArray() && target.GetColIndexArray()) {
    for (Int_t irow = 0; irow < nrows_sub; irow++) {
      for (Int_t icol = 0; icol < ncols_sub; icol++) {
        target(irow+row_lwb_sub,icol+col_lwb_sub) = (*this)(row_lwb+irow,col_lwb+icol);
      }
    }
  } else {
    const Float_t *ap = this->GetMatrixArray()+(row_lwb-fRowLwb)*fNcols+(col_lwb-fColLwb);
          Float_t *bp = target.GetMatrixArray();

    for (Int_t irow = 0; irow < nrows_sub; irow++) {
      const Float_t *ap_sub = ap;
      for (Int_t icol = 0; icol < ncols_sub; icol++) {
        *bp++ = *ap_sub++;
      }
      ap += fNcols;
    }
  }

  return target;
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::SetSub(Int_t row_lwb,const TMatrixFBase &source)
{ 
  // Insert matrix source starting at [row_lwb][row_lwb], thereby overwriting the part
  // [row_lwb..row_lwb+nrows_source][row_lwb..row_lwb+nrows_source];
    
  Assert(IsValid());
  Assert(source.IsValid());
    
  if (!source.IsSymmetric()) {
    Error("SetSub","source matrix is not symmetric");
    Invalidate();
    return *this;
  }
  if (row_lwb < fRowLwb || row_lwb > fRowLwb+fNrows-1) {
    Error("SetSub","row_lwb outof bounds");
    Invalidate();
    return *this;
  }
  const Int_t nRows_source = source.GetNrows();
  if (row_lwb+nRows_source > fRowLwb+fNrows) {
    Error("SetSub","source matrix too large");
    Invalidate();
    return *this;
  }
  
  if (source.GetRowIndexArray() && source.GetColIndexArray()) {
    const Int_t rowlwb_s = source.GetRowLwb();
    for (Int_t irow = 0; irow < nRows_source; irow++) {
      for (Int_t icol = 0; icol < nRows_source; icol++) {
        (*this)(row_lwb+irow,row_lwb+icol) = source(rowlwb_s+irow,rowlwb_s+icol);
      }
    }
  } else {
    const Float_t *bp = source.GetMatrixArray();
          Float_t *ap = this->GetMatrixArray()+(row_lwb-fRowLwb)*fNrows+(row_lwb-fRowLwb);
      
    for (Int_t irow = 0; irow < nRows_source; irow++) {
      Float_t *ap_sub = ap;
      for (Int_t icol = 0; icol < nRows_source; icol++) {
        *ap_sub++ = *bp++;
      }
      ap += fNrows;
    }
  }

  return *this;
}

//______________________________________________________________________________
TMatrixFBase &TMatrixFSym::SetSub(Int_t row_lwb,Int_t col_lwb,const TMatrixFBase &source)
{
  // Insert matrix source starting at [row_lwb][col_lwb] in a symmetric fashion, thereby overwriting the part
  // [row_lwb..row_lwb+nrows_source][row_lwb..row_lwb+nrows_source];

  Assert(IsValid());
  Assert(source.IsValid());

  if (row_lwb < fRowLwb || row_lwb > fRowLwb+fNrows-1) {
    Error("SetSub","row_lwb out of bounds");
    Invalidate();
    return *this;
  }
  if (col_lwb < fColLwb || col_lwb > fColLwb+fNcols-1) {
    Error("SetSub","col_lwb out of bounds");
    Invalidate();
    return *this;
  }
  const Int_t nRows_source = source.GetNrows();
  const Int_t nCols_source = source.GetNcols();

  if (row_lwb+nRows_source > fRowLwb+fNrows || col_lwb+nCols_source > fRowLwb+fNrows) {
    Error("SetSub","source matrix too large");
    Invalidate();
    return *this;
  }
  if (col_lwb+nCols_source > fRowLwb+fNrows || row_lwb+nRows_source > fRowLwb+fNrows) {
    Error("SetSub","source matrix too large");
    Invalidate();
    return *this;
  }
  
  const Int_t rowlwb_s = source.GetRowLwb();
  const Int_t collwb_s = source.GetColLwb();
  if (row_lwb >= col_lwb) {
    // lower triangle
    Int_t irow;
    for (irow = 0; irow < nRows_source; irow++) {
      for (Int_t icol = 0; col_lwb+icol <= row_lwb+irow &&
                             icol < nCols_source; icol++) {
        (*this)(row_lwb+irow,col_lwb+icol) = source(irow+rowlwb_s,icol+collwb_s);
      }
    }

    // upper triangle
    for (irow = 0; irow < nCols_source; irow++) {
      for (Int_t icol = nRows_source-1; row_lwb+icol > irow+col_lwb &&
                              icol >= 0; icol--) {
        (*this)(col_lwb+irow,row_lwb+icol) = source(icol+rowlwb_s,irow+collwb_s);
      }
    }
  } else {

  }

  return *this;
}

//______________________________________________________________________________
TMatrixFBase &TMatrixFSym::SetMatrixArray(const Float_t *data,Option_t *option)
{
  TMatrixFBase::SetMatrixArray(data,option);
  if (!this->IsSymmetric()) {
    Error("SetMatrixArray","Matrix is not symmetric after Set");
    Invalidate();
  }

  return *this;
}

//______________________________________________________________________________
TMatrixFBase &TMatrixFSym::Shift(Int_t row_shift,Int_t col_shift)
{
  if (row_shift != col_shift) {
    Error("Shift","row_shift != col_shift");
    Invalidate();
  }
  return TMatrixFBase::Shift(row_shift,col_shift);
}

//______________________________________________________________________________
TMatrixFBase &TMatrixFSym::ResizeTo(Int_t nrows,Int_t ncols,Int_t /*nr_nonzeros*/)
{
  // Set size of the matrix to nrows x ncols
  // New dynamic elements are created, the overlapping part of the old ones are
  // copied to the new structures, then the old elements are deleted.
  
  Assert(IsValid());
  if (!fIsOwner) {
    Error("ResizeTo(Int_t,Int_t)","Not owner of data array,cannot resize");
    Invalidate();
    return *this;
  }

  if (nrows != ncols) {
    Error("ResizeTo(Int_t,Int_t)","nrows != ncols");
    Invalidate();
    return *this;
  }
  
  if (fNelems > 0) {
    if (fNrows == nrows && fNcols == ncols)
      return *this;
    else if (nrows == 0 || ncols == 0) {
      fNrows = nrows; fNcols = ncols;
      Clear();
      return *this;
    }
    
    Float_t     *elements_old = GetMatrixArray();
    const Int_t  nelems_old   = fNelems;
    const Int_t  nrows_old    = fNrows;
    const Int_t  ncols_old    = fNcols;
    
    Allocate(nrows,ncols); 
    Assert(IsValid());
      
    Float_t  *elements_new = GetMatrixArray();
    // new memory should be initialized but be careful ot to wipe out the stack
    // storage. Initialize all when old or new storage was on the heap
    if (fNelems > kSizeMax || nelems_old > kSizeMax)
      memset(elements_new,0,fNelems*sizeof(Float_t));
    else if (fNelems > nelems_old)
      memset(elements_new+nelems_old,0,(fNelems-nelems_old)*sizeof(Float_t));

    // Copy overlap
    const Int_t ncols_copy = TMath::Min(fNcols,ncols_old);
    const Int_t nrows_copy = TMath::Min(fNrows,nrows_old);

    const Int_t nelems_new = fNelems;
    if (ncols_old < fNcols) {
      for (Int_t i = nrows_copy-1; i >= 0; i--)
        Memcpy_m(elements_new+i*fNcols,elements_old+i*ncols_old,ncols_copy,
                 nelems_new,nelems_old);
    } else {
      for (Int_t i = 0; i < nrows_copy; i++)
        Memcpy_m(elements_new+i*fNcols,elements_old+i*ncols_old,ncols_copy,
                 nelems_new,nelems_old);
    }

    Delete_m(nelems_old,elements_old);
  } else {
    Allocate(nrows,ncols,0,0,1);
  }

  return *this;
}

//______________________________________________________________________________
TMatrixFBase &TMatrixFSym::ResizeTo(Int_t row_lwb,Int_t row_upb,Int_t col_lwb,Int_t col_upb,
                                    Int_t /*nr_nonzeros*/)
{
  // Set size of the matrix to [row_lwb:row_upb] x [col_lwb:col_upb]
  // New dynamic elemenst are created, the overlapping part of the old ones are
  // copied to the new structures, then the old elements are deleted.

  Assert(IsValid());
  if (!fIsOwner) {
    Error("ResizeTo(Int_t,Int_t,Int_t,Int_t)","Not owner of data array,cannot resize");
    Invalidate();
    return *this;
  }

  if (row_lwb != col_lwb) {
    Error("ResizeTo","row_lwb != col_lwb");
    Invalidate();
    return *this;
  }
  if (row_upb != col_upb) {
    Error("ResizeTo","row_upb != col_upb");
    Invalidate();
    return *this;
  }

  const Int_t new_nrows = row_upb-row_lwb+1;
  const Int_t new_ncols = col_upb-col_lwb+1;
    
  if (fNelems > 0) {
    
    if (fNrows  == new_nrows  && fNcols  == new_ncols &&
        fRowLwb == row_lwb    && fColLwb == col_lwb)
       return *this;
    else if (new_nrows == 0 || new_ncols == 0) {
      fNrows = new_nrows; fNcols = new_ncols;
      fRowLwb = row_lwb; fColLwb = col_lwb;
      Clear();
      return *this;
    }
    
    Float_t     *elements_old = GetMatrixArray();
    const Int_t  nelems_old   = fNelems;
    const Int_t  nrows_old    = fNrows;
    const Int_t  ncols_old    = fNcols; 
    const Int_t  rowLwb_old   = fRowLwb;
    const Int_t  colLwb_old   = fColLwb;
    
    Allocate(new_nrows,new_ncols,row_lwb,col_lwb);
    Assert(IsValid());
                 
    Float_t *elements_new = GetMatrixArray();
    // new memory should be initialized but be careful ot to wipe out the stack
    // storage. Initialize all when old or new storag ewas on the heap
    if (fNelems > kSizeMax || nelems_old > kSizeMax)
      memset(elements_new,0,fNelems*sizeof(Float_t));
    else if (fNelems > nelems_old)
      memset(elements_new+nelems_old,0,(fNelems-nelems_old)*sizeof(Float_t));
  
    // Copy overlap
    const Int_t rowLwb_copy = TMath::Max(fRowLwb,rowLwb_old);
    const Int_t colLwb_copy = TMath::Max(fColLwb,colLwb_old);
    const Int_t rowUpb_copy = TMath::Min(fRowLwb+fNrows-1,rowLwb_old+nrows_old-1);
    const Int_t colUpb_copy = TMath::Min(fColLwb+fNcols-1,colLwb_old+ncols_old-1);

    const Int_t nrows_copy = rowUpb_copy-rowLwb_copy+1;
    const Int_t ncols_copy = colUpb_copy-colLwb_copy+1;

    if (nrows_copy > 0 && ncols_copy > 0) {
      const Int_t colOldOff = colLwb_copy-colLwb_old;
      const Int_t colNewOff = colLwb_copy-fColLwb;
      if (ncols_old < fNcols) {
        for (Int_t i = nrows_copy-1; i >= 0; i--) {
          const Int_t iRowOld = rowLwb_copy+i-rowLwb_old;
          const Int_t iRowNew = rowLwb_copy+i-fRowLwb;
          Memcpy_m(elements_new+iRowNew*fNcols+colNewOff,
                   elements_old+iRowOld*ncols_old+colOldOff,ncols_copy,fNelems,nelems_old);
        }
      } else {
        for (Int_t i = 0; i < nrows_copy; i++) {
          const Int_t iRowOld = rowLwb_copy+i-rowLwb_old;
          const Int_t iRowNew = rowLwb_copy+i-fRowLwb;
          Memcpy_m(elements_new+iRowNew*fNcols+colNewOff,
                   elements_old+iRowOld*ncols_old+colOldOff,ncols_copy,fNelems,nelems_old);
        }
      }
    }

    Delete_m(nelems_old,elements_old);
  } else {
    Allocate(new_nrows,new_ncols,row_lwb,col_lwb,1);
  }

  return *this;
}

//______________________________________________________________________________
Double_t TMatrixFSym::Determinant() const
{
  const TMatrixD tmp = TMatrixDSym(*this);
  TDecompLU lu(tmp,fTol);
  Double_t d1,d2;
  lu.Det(d1,d2);
  return d1*TMath::Power(2.0,d2);
}

//______________________________________________________________________________
void TMatrixFSym::Determinant(Double_t &d1,Double_t &d2) const
{
  const TMatrixD tmp = TMatrixDSym(*this);
  TDecompLU lu(tmp,fTol);
  lu.Det(d1,d2);
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::Invert(Double_t *det)
{     
  // Invert the matrix and calculate its determinant
    
  if (det)
    *det = this->Determinant();
  TMatrixDSym tmp = *this;
  TDecompBK bk(tmp,fTol);
  bk.Invert(tmp);
  *this = tmp;

  return *this;
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::InvertFast(Double_t *det)
{
  // Invert the matrix and calculate its determinant

  Assert(IsValid());

  const Char_t nRows = Char_t(GetNrows());
  switch (nRows) {
    case 1:
    {
      Float_t *pM = this->GetMatrixArray();
      if (*pM == 0.) Invalidate();
      else           *pM = 1.0/(*pM);
      return *this;
    }
    case 2:
    {
      TMatrixFSymCramerInv::Inv2x2(*this,det);
      return *this;
    }
    case 3:
    {
      TMatrixFSymCramerInv::Inv3x3(*this,det);
      return *this;
    }
    case 4:
    {
      TMatrixFSymCramerInv::Inv4x4(*this,det);
      return *this;
    }
    case 5:
    {
      TMatrixFSymCramerInv::Inv5x5(*this,det);
      return *this;
    }
    case 6:
    {
      TMatrixFSymCramerInv::Inv6x6(*this,det);
      return *this;
    }

    default:
    {
      if (det)
        *det = this->Determinant();
      TMatrixDSym tmp = *this;
      TDecompBK bk(tmp,fTol);
      bk.Invert(tmp);
      *this = tmp;
      return *this;
    }
  }
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::Transpose(const TMatrixFSym &source)
{
  // Transpose a matrix.

  Assert(IsValid());
  Assert(source.IsValid());

  if (fNrows != source.GetNcols() || fRowLwb != source.GetColLwb())
  {
    Error("Transpose","matrix has wrong shape");
    Invalidate();
    return *this;
  }

  *this = source;
  return *this;
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::Rank1Update(const TVectorF &v,Float_t alpha)
{
  // Perform a rank 1 operation on the matrix:                          
  //     A += alpha * v * v^T

  Assert(IsValid()); 
  Assert(v.IsValid());                                                  

  if (v.GetNoElements() < fNrows) {
    Error("Rank1Update","vector too short");
    Invalidate();
    return *this;
  }

  const Float_t * const pv = v.GetMatrixArray();
        Float_t *trp = this->GetMatrixArray(); // pointer to UR part and diagonal, traverse row-wise
        Float_t *tcp = trp;                    // pointer to LL part,              traverse col-wise
  for (Int_t i = 0; i < fNrows; i++) {
    trp += i;         // point to [i,i]
    tcp += i*fNcols;  // point to [i,i]
    const Float_t tmp = alpha*pv[i];
    for (Int_t j = i; j < fNcols; j++) {
      if (j > i) *tcp += tmp*pv[j];
      *trp++ += tmp*pv[j];
      tcp += fNcols;
    }
    tcp -= fNelems-1; // point to [0,i]
  }

  return *this;
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::Similarity(const TMatrixF &b)
{
// Calculate B * (*this) * B^T , final matrix will be (nrowsb x nrowsb)
// This is a similarity transform when B is orthogonal . It is more
// efficient than applying the actual multiplication because this
// routine realizes that  the final matrix is symmetric . 

  const TMatrixF ba(b,TMatrixF::kMult,*this);

  const Int_t nrowsb  = b.GetNrows();
  if (nrowsb != fNrows)
    this->ResizeTo(nrowsb,nrowsb);

#ifdef CBLAS
  const Float_t *bap = ba.GetMatrixArray();
  const Float_t *bp  = b.GetMatrixArray();
        Float_t *cp  = this->GetMatrixArray();
  cblas_sgemm (CblasRowMajor,CblasNoTrans,CblasTrans,fNrows,fNcols,ba.GetNcols(),
               1.0,bap,ba.GetNcols(),bp,b.GetNcols(),1.0,cp,fNcols);
#else
  const Int_t nba     = ba.GetNoElements();
  const Int_t nb      = b.GetNoElements();
  const Int_t ncolsba = ba.GetNcols();
  const Int_t ncolsb  = b.GetNcols();
  const Float_t * const bap  = ba.GetMatrixArray();
  const Float_t * const bp   = b.GetMatrixArray();
  const Float_t *       bi1p = bp;
        Float_t *       cp   = this->GetMatrixArray();
        Float_t * const cp0  = cp;

  Int_t ishift = 0;
  const Float_t *barp0 = bap;
  while (barp0 < bap+nba) {
    const Float_t *brp0 = bi1p;
    while (brp0 < bp+nb) {
      const Float_t *barp = barp0;
      const Float_t *brp  = brp0;
      Float_t cij = 0;
      while (brp < brp0+ncolsb)
        cij += *barp++ * *brp++;
      *cp++ = cij;
      brp0 += ncolsb;
    }
    barp0 += ncolsba;
    bi1p += ncolsb;
    cp += ++ishift;
  }

  Assert(cp == cp0+fNelems+ishift && barp0 == bap+nba);

  cp = cp0;
  for (Int_t irow = 0; irow < fNrows; irow++) {
    const Int_t rowOff1 = irow*fNrows;
    for (Int_t icol = 0; icol < irow; icol++) {
      const Int_t rowOff2 = icol*fNrows;
      cp[rowOff1+icol] = cp[rowOff2+irow];
    }
  }
#endif

  return *this;
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::Similarity(const TMatrixFSym &b)
{
// Calculate B * (*this) * B^T , final matrix will be (nrowsb x nrowsb)
// This is a similarity transform when B is orthogonal . It is more
// efficient than applying the actual multiplication because this
// routine realizes that  the final matrix is symmetric .

#ifdef CBLAS
  const TMatrixF abt(*this,TMatrixF::kMultTranspose,b);

  const Float_t *abtp = abt.GetMatrixArray();
  const Float_t *bp   = b.GetMatrixArray();
        Float_t *cp   = this->GetMatrixArray();
  cblas_ssymm (CblasRowMajor,CblasLeft,CblasUpper,fNrows,fNcols,1.0,
               bp,b.GetNcols(),abtp,abt.GetNcols(),0.0,cp,fNcols);
#else
  const TMatrixF ba(b,TMatrixF::kMult,*this);

  const Int_t nba     = ba.GetNoElements();
  const Int_t nb      = b.GetNoElements();
  const Int_t ncolsba = ba.GetNcols();
  const Int_t ncolsb  = b.GetNcols();
  const Float_t * const bap  = ba.GetMatrixArray();
  const Float_t * const bp   = b.GetMatrixArray();
  const Float_t *       bi1p = bp;
        Float_t *       cp   = this->GetMatrixArray();
        Float_t * const cp0  = cp;

  Int_t ishift = 0;
  const Float_t *barp0 = bap;
  while (barp0 < bap+nba) {
    const Float_t *brp0 = bi1p;
    while (brp0 < bp+nb) {
      const Float_t *barp = barp0;
      const Float_t *brp  = brp0;
      Float_t cij = 0;
      while (brp < brp0+ncolsb)
        cij += *barp++ * *brp++;
      *cp++ = cij;
      brp0 += ncolsb;
    }
    barp0 += ncolsba;
    bi1p += ncolsb;
    cp += ++ishift;
  }

  Assert(cp == cp0+fNelems+ishift && barp0 == bap+nba);

  cp = cp0;
  for (Int_t irow = 0; irow < fNrows; irow++) {
    const Int_t rowOff1 = irow*fNrows;
    for (Int_t icol = 0; icol < irow; icol++) {
      const Int_t rowOff2 = icol*fNrows;
      cp[rowOff1+icol] = cp[rowOff2+irow];
    }
  }
#endif

  return *this;
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::SimilarityT(const TMatrixF &b)
{
// Calculate B^T * (*this) * B , final matrix will be (ncolsb x ncolsb)
// It is more efficient than applying the actual multiplication because this
// routine realizes that  the final matrix is symmetric .

  const TMatrixF bta(b,TMatrixF::kTransposeMult,*this);

  const Int_t ncolsb = b.GetNcols();
  if (ncolsb != fNcols)
    this->ResizeTo(ncolsb,ncolsb);

#ifdef CBLAS
  const Float_t *btap = bta.GetMatrixArray();
  const Float_t *bp   = b.GetMatrixArray();
        Float_t *cp   = this->GetMatrixArray();
  cblas_sgemm (CblasRowMajor,CblasNoTrans,CblasNoTrans,fNrows,fNcols,bta.GetNcols(),
               1.0,btap,bta.GetNcols(),bp,b.GetNcols(),1.0,cp,fNcols);
#else
  const Int_t nbta     = bta.GetNoElements();
  const Int_t nb       = b.GetNoElements();
  const Int_t ncolsbta = bta.GetNcols();
  const Float_t * const btap = bta.GetMatrixArray();
  const Float_t * const bp   = b.GetMatrixArray();
        Float_t *       cp   = this->GetMatrixArray();
        Float_t * const cp0  = cp;

  Int_t ishift = 0;
  const Float_t *btarp0 = btap;                     // Pointer to  A[i,0];
  const Float_t *bcp0   = bp;
  while (btarp0 < btap+nbta) {
    for (const Float_t *bcp = bcp0; bcp < bp+ncolsb; ) { // Pointer to the j-th column of B, Start bcp = B[0,0]
      const Float_t *btarp = btarp0;                   // Pointer to the i-th row of A, reset to A[i,0]
      Float_t cij = 0;
      while (bcp < bp+nb) {                         // Scan the i-th row of A and
        cij += *btarp++ * *bcp;                     // the j-th col of B
        bcp += ncolsb;
      }
      *cp++ = cij;
      bcp -= nb-1;                                  // Set bcp to the (j+1)-th col
    }
    btarp0 += ncolsbta;                             // Set ap to the (i+1)-th row
    bcp0++;
    cp += ++ishift;
  }

  Assert(cp == cp0+fNelems+ishift && btarp0 == btap+nbta);

  cp = cp0;
  for (Int_t irow = 0; irow < fNrows; irow++) {
    const Int_t rowOff1 = irow*fNrows;
    for (Int_t icol = 0; icol < irow; icol++) {
      const Int_t rowOff2 = icol*fNrows;
      cp[rowOff1+icol] = cp[rowOff2+irow];
    }
  }
#endif

  return *this;
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::operator=(const TMatrixFSym &source)
{
  if (!AreCompatible(*this,source)) {
    Error("operator=(const TMatrixFSym &","matrices not compatible");
    Invalidate();
    return *this;
  }

  if (this != &source) {
    TObject::operator=(source);
    memcpy(this->GetMatrixArray(),source.fElements,fNelems*sizeof(Float_t));
    fTol = source.GetTol();
  }
  return *this;
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::operator=(const TMatrixDSym &source)
{ 
  if (!AreCompatible(*this,source)) {
    Error("operator=(const TMatrixDSym &","matrices not compatible");
    Invalidate();
    return *this;
  }

  if (dynamic_cast<TMatrixDSym *>(this) != &source) {
    TObject::operator=(source);
    const Double_t * const ps = source.GetMatrixArray();
          Float_t  * const pt = GetMatrixArray();
    for (Int_t i = 0; i < fNelems; i++)
      pt[i] = (Float_t) ps[i];
    fTol = source.GetTol();
  }
  return *this;
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::operator=(const TMatrixFSymLazy &lazy_constructor)
{
  Assert(IsValid());

  if (lazy_constructor.fRowUpb != GetRowUpb() ||
      lazy_constructor.fRowLwb != GetRowLwb()) {
     Error("operator=(const TMatrixFSymLazy&)", "matrix is incompatible with "
           "the assigned Lazy matrix");
    Invalidate();
    return *this;
  }

  lazy_constructor.FillIn(*this);
  return *this;
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::operator=(Float_t val)
{
  // Assign val to every element of the matrix.

  Assert(IsValid());

  Float_t *ep = fElements;
  const Float_t * const ep_last = ep+fNelems;
  while (ep < ep_last)
    *ep++ = val;

  return *this;
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::operator+=(Float_t val)
{
  // Add val to every element of the matrix.

  Assert(IsValid());

  Float_t *ep = fElements;
  const Float_t * const ep_last = ep+fNelems;
  while (ep < ep_last)
    *ep++ += val;

  return *this;
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::operator-=(Float_t val)
{
  // Subtract val from every element of the matrix.

  Assert(IsValid());

  Float_t *ep = fElements;
  const Float_t * const ep_last = ep+fNelems;
  while (ep < ep_last)
    *ep++ -= val;

  return *this;
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::operator*=(Float_t val)
{
  // Multiply every element of the matrix with val.

  Assert(IsValid());

  Float_t *ep = fElements;
  const Float_t * const ep_last = ep+fNelems;
  while (ep < ep_last)
    *ep++ *= val;

  return *this;
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::operator+=(const TMatrixFSym &source)
{
  // Add the source matrix.

  if (!AreCompatible(*this,source)) {
    Error("operator+=","matrices not compatible");
    Invalidate();
    return *this;
  }

  const Float_t *sp  = source.GetMatrixArray();
        Float_t *trp = this->GetMatrixArray(); // pointer to UR part and diagonal, traverse row-wise
        Float_t *tcp = trp;                 // pointer to LL part,              traverse col-wise
  for (Int_t i = 0; i < fNrows; i++) {
    sp  += i;
    trp += i;         // point to [i,i]
    tcp += i*fNcols;  // point to [i,i]
    for (Int_t j = i; j < fNcols; j++) {
      if (j > i) *tcp += *sp;
      *trp++ += *sp++;
      tcp += fNcols;
    }
    tcp -= fNelems-1; // point to [0,i]
  }

  return *this;
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::operator-=(const TMatrixFSym &source)
{
  // Subtract the source matrix.

  if (!AreCompatible(*this,source)) {
    Error("operator-=","matrices not compatible");
    Invalidate();
    return *this;
  }

  const Float_t *sp  = source.GetMatrixArray();
        Float_t *trp = this->GetMatrixArray(); // pointer to UR part and diagonal, traverse row-wise
        Float_t *tcp = trp;                 // pointer to LL part,              traverse col-wise
  for (Int_t i = 0; i < fNrows; i++) {
    sp  += i;
    trp += i;         // point to [i,i]
    tcp += i*fNcols;  // point to [i,i]
    for (Int_t j = i; j < fNcols; j++) {
      if (j > i) *tcp -= *sp;
      *trp++ -= *sp++;
      tcp += fNcols;
    }
    tcp -= fNelems-1; // point to [0,i]
  }

  return *this;
}

//______________________________________________________________________________
TMatrixFBase &TMatrixFSym::Apply(const TElementActionF &action)
{ 
  Assert(IsValid());
  
  Float_t val = 0;
  Float_t *trp = this->GetMatrixArray(); // pointer to UR part and diagonal, traverse row-wise
  Float_t *tcp = trp;                 // pointer to LL part,              traverse col-wise
  for (Int_t i = 0; i < fNrows; i++) {
    trp += i;         // point to [i,i]
    tcp += i*fNcols;  // point to [i,i]
    for (Int_t j = i; j < fNcols; j++) {
      action.Operation(val);
      if (j > i) *tcp = val;
      *trp++ = val;
      tcp += fNcols;
    }
    tcp -= fNelems-1; // point to [0,i]
  }

  return *this;
}

//______________________________________________________________________________
TMatrixFBase &TMatrixFSym::Apply(const TElementPosActionF &action)
{ 
  // Apply action to each element of the matrix. To action the location
  // of the current element is passed.
  
  Assert(IsValid());

  Float_t val = 0;
  Float_t *trp = this->GetMatrixArray(); // pointer to UR part and diagonal, traverse row-wise
  Float_t *tcp = trp;                 // pointer to LL part,              traverse col-wise
  for (Int_t i = 0; i < fNrows; i++) {
    action.fI = i+fRowLwb;
    trp += i;         // point to [i,i]
    tcp += i*fNcols;  // point to [i,i]
    for (Int_t j = i; j < fNcols; j++) {
      action.fJ = j+fColLwb;
      action.Operation(val);
      if (j > i) *tcp = val;
      *trp++ = val;
      tcp += fNcols;
    }
    tcp -= fNelems-1; // point to [0,i]
  }

  return *this;
}

//______________________________________________________________________________
TMatrixFBase &TMatrixFSym::Randomize(Float_t alpha,Float_t beta,Double_t &seed)
{
  // randomize matrix element values but keep matrix symmetric

  Assert(IsValid());
    
  if (fNrows != fNcols || fRowLwb != fColLwb) {
    Error("Randomize(Float_t,Float_t,Double_t &","matrix should be square");
    Invalidate();
    return *this;
  }
  
  const Float_t scale = beta-alpha;
  const Float_t shift = alpha/scale;

  Float_t *ep = GetMatrixArray();
  for (Int_t i = 0; i < fNrows; i++) {
    const Int_t off = i*fNcols;
    for (Int_t j = 0; j <= i; j++) {
      ep[off+j] = scale*(Drand(seed)+shift);
      if (i != j) {
        ep[j*fNcols+i] = ep[off+j];
      }
    }
  }

  return *this;
}

//______________________________________________________________________________
TMatrixFSym &TMatrixFSym::RandomizePD(Float_t alpha,Float_t beta,Double_t &seed)
{
  // randomize matrix element values but keep matrix symmetric positive definite

  Assert(IsValid());

  if (fNrows != fNcols || fRowLwb != fColLwb) {
    Error("RandomizeSym(Float_t,Float_t,Double_t &","matrix should be square");
    Invalidate();
    return *this;
  }

  const Float_t scale = beta-alpha;
  const Float_t shift = alpha/scale;

  Float_t *ep = GetMatrixArray();
  Int_t i;
  for (i = 0; i < fNrows; i++) {
    const Int_t off = i*fNcols;
    for (Int_t j = 0; j <= i; j++)
      ep[off+j] = scale*(Drand(seed)+shift);
  }

  for (i = fNrows-1; i >= 0; i--) {
    const Int_t off1 = i*fNcols;
    for (Int_t j = i; j >= 0; j--) {
      const Int_t off2 = j*fNcols;
      ep[off1+j] *= ep[off2+j];
      for (Int_t k = j-1; k >= 0; k--) {
        ep[off1+j] += ep[off1+k]*ep[off2+k];
      }
      if (i != j)
        ep[off2+i] = ep[off1+j];
    }
  }

  return *this;
}

//______________________________________________________________________________
const TMatrixF TMatrixFSym::EigenVectors(TVectorF &eigenValues) const
{
  // Return a matrix containing the eigen-vectors ordered by descending eigen-values.
  // For full functionality use TMatrixDSymEigen .

  TMatrixDSymEigen eigen(*this);
  eigenValues.ResizeTo(fNrows);
  eigenValues = eigen.GetEigenValues();
  return eigen.GetEigenVectors();
}

//______________________________________________________________________________
//Bool_t operator==(const TMatrixFSym &m1,const TMatrixFSym &m2)
//{
//  // Check to see if two matrices are identical.
//
//  if (!AreCompatible(m1,m2)) return kFALSE;
//  return (memcmp(m1.GetMatrixArray(),m2.GetMatrixArray(),               
//                 m1.GetNoElements()*sizeof(Float_t)) == 0);
//}

//______________________________________________________________________________
TMatrixFSym operator+(const TMatrixFSym &source1,const TMatrixFSym &source2)
{
  TMatrixFSym target(source1);
  target += source2;
  return target;
}

//______________________________________________________________________________
TMatrixFSym operator+(const TMatrixFSym &source1,Float_t val)
{
  TMatrixFSym target(source1);
  target += val;
  return target;
}

//______________________________________________________________________________
TMatrixFSym operator+(Float_t val,const TMatrixFSym &source1)
{
  return operator+(source1,val);
}

//______________________________________________________________________________
TMatrixFSym operator-(const TMatrixFSym &source1,const TMatrixFSym &source2)
{
  TMatrixFSym target(source1);
  target -= source2;
  return target;
}

//______________________________________________________________________________
TMatrixFSym operator-(const TMatrixFSym &source1,Float_t val)
{
  TMatrixFSym target(source1);
  target -= val;
  return target;
}

//______________________________________________________________________________
TMatrixFSym operator-(Float_t val,const TMatrixFSym &source1)
{
  return operator*(operator-(source1,val),Float_t(-1.));
}

//______________________________________________________________________________
TMatrixFSym operator*(const TMatrixFSym &source1,Float_t val)
{
  TMatrixFSym target(source1);
  target *= val;
  return target;
}

//______________________________________________________________________________
TMatrixFSym operator*(Float_t val,const TMatrixFSym &source1)
{
  return operator*(source1,val);
}

//______________________________________________________________________________
TMatrixFSym operator&&(const TMatrixFSym &source1,const TMatrixFSym &source2)
{
  // Logical AND

  TMatrixFSym target;

  if (!AreCompatible(source1,source2)) {
    Error("operator&&(const TMatrixFSym&,const TMatrixFSym&)","matrices not compatible");
    target.Invalidate();
    return target;
  }

  target.ResizeTo(source1);

  const Int_t nelems = target.GetNoElements();
  const Int_t nrows  = target.GetNrows();
  const Int_t ncols  = target.GetNcols();

  const Float_t *srp1 = source1.GetMatrixArray();
  const Float_t *scp1 = srp1;
  const Float_t *sp2  = source2.GetMatrixArray();
        Float_t *trp  = target.GetMatrixArray(); // pointer to UR part and diagonal, traverse row-wise
        Float_t *tcp  = trp;                     // pointer to LL part,              traverse col-wise
  for (Int_t i = 0; i < nrows; i++) {
    sp2 += i;
    srp1 += i; trp += i;              // point to [i,i]
    scp1 += i*ncols; tcp += i*ncols;  // point to [i,i]
    for (Int_t j = i; j < ncols; j++) {
      if (j > i) *tcp = (*scp1 != 0.0) & (*sp2 != 0.0);
      *trp++ = (*srp1++ != 0.0 && *sp2++ != 0.0);
      scp1 += ncols;
      tcp  += ncols;
    }
    scp1 -= nelems-1; // point to [0,i]
    tcp  -= nelems-1; // point to [0,i]
  }

  return target;
}

//______________________________________________________________________________
TMatrixFSym operator||(const TMatrixFSym &source1,const TMatrixFSym &source2)
{
  // Logical Or

  TMatrixFSym target;

  if (!AreCompatible(source1,source2)) {
    Error("operator||(const TMatrixFSym&,const TMatrixFSym&)","matrices not compatible");
    target.Invalidate();
    return target;
  }

  target.ResizeTo(source1);

  const Int_t nelems = target.GetNoElements();
  const Int_t nrows  = target.GetNrows();
  const Int_t ncols  = target.GetNcols();

  const Float_t *srp1 = source1.GetMatrixArray();
  const Float_t *scp1 = srp1;
  const Float_t *sp2  = source2.GetMatrixArray();
        Float_t *trp  = target.GetMatrixArray(); // pointer to UR part and diagonal, traverse row-wise
        Float_t *tcp  = trp;                     // pointer to LL part,              traverse col-wise
  for (Int_t i = 0; i < nrows; i++) {
    sp2 += i;
    srp1 += i; trp += i;              // point to [i,i]
    scp1 += i*ncols; tcp += i*ncols;  // point to [i,i]
    for (Int_t j = i; j < ncols; j++) {
      if (j > i) *tcp = (*scp1 != 0.0) & (*sp2 != 0.0);
      *trp++ = (*srp1++ != 0.0 || *sp2++ != 0.0);
      scp1 += ncols;
      tcp  += ncols;
    }
    scp1 -= nelems-1; // point to [0,i]
    tcp  -= nelems-1; // point to [0,i]
  }

  return target;
}

//______________________________________________________________________________
TMatrixFSym operator>(const TMatrixFSym &source1,const TMatrixFSym &source2)
{
  // source1 > source2

  TMatrixFSym target;

  if (!AreCompatible(source1,source2)) {
    Error("operator>(const TMatrixFSym&,const TMatrixFSym&)","matrices not compatible");
    target.Invalidate();
    return target;
  }

  target.ResizeTo(source1);

  const Int_t nelems = target.GetNoElements();
  const Int_t nrows  = target.GetNrows();
  const Int_t ncols  = target.GetNcols();

  const Float_t *srp1 = source1.GetMatrixArray();
  const Float_t *scp1 = srp1;
  const Float_t *sp2  = source2.GetMatrixArray();
        Float_t *trp  = target.GetMatrixArray(); // pointer to UR part and diagonal, traverse row-wise
        Float_t *tcp  = trp;                     // pointer to LL part,              traverse col-wise
  for (Int_t i = 0; i < nrows; i++) {
    sp2 += i;
    srp1 += i; trp += i;              // point to [i,i]
    scp1 += i*ncols; tcp += i*ncols;  // point to [i,i]
    for (Int_t j = i; j < ncols; j++) {
      if (j > i) *tcp = *scp1 > *sp2;
      *trp++ = *srp1++ > *sp2++;
      scp1 += ncols;
      tcp  += ncols;
    }
    scp1 -= nelems-1; // point to [0,i]
    tcp  -= nelems-1; // point to [0,i]
  }

  return target;
}

//______________________________________________________________________________
TMatrixFSym operator>=(const TMatrixFSym &source1,const TMatrixFSym &source2)
{
  // source1 >= source2

  TMatrixFSym target;

  if (!AreCompatible(source1,source2)) {
    Error("operator>=(const TMatrixFSym&,const TMatrixFSym&)","matrices not compatible");
    target.Invalidate();
    return target;
  }

  target.ResizeTo(source1);

  const Int_t nelems = target.GetNoElements();
  const Int_t nrows  = target.GetNrows();
  const Int_t ncols  = target.GetNcols();

  const Float_t *srp1 = source1.GetMatrixArray();
  const Float_t *scp1 = srp1;
  const Float_t *sp2  = source2.GetMatrixArray();
        Float_t *trp  = target.GetMatrixArray(); // pointer to UR part and diagonal, traverse row-wise
        Float_t *tcp  = trp;                     // pointer to LL part,              traverse col-wise
  for (Int_t i = 0; i < nrows; i++) {
    sp2 += i;
    srp1 += i; trp += i;              // point to [i,i]
    scp1 += i*ncols; tcp += i*ncols;  // point to [i,i]
    for (Int_t j = i; j < ncols; j++) {
      if (j > i) *tcp = *scp1 >= *sp2;
      *trp++ = *srp1++ >= *sp2++;
      scp1 += ncols;
      tcp  += ncols;
    }
    scp1 -= nelems-1; // point to [0,i]
    tcp  -= nelems-1; // point to [0,i]
  }

  return target;
}

//______________________________________________________________________________
TMatrixFSym operator<=(const TMatrixFSym &source1,const TMatrixFSym &source2)
{
  // source1 <= source2

  TMatrixFSym target;

  if (!AreCompatible(source1,source2)) {
    Error("operator<=(const TMatrixFSym&,const TMatrixFSym&)","matrices not compatible");
    target.Invalidate();
    return target;
  }

  target.ResizeTo(source1);

  const Int_t nelems = target.GetNoElements();
  const Int_t nrows  = target.GetNrows();
  const Int_t ncols  = target.GetNcols();

  const Float_t *srp1 = source1.GetMatrixArray();
  const Float_t *scp1 = srp1;
  const Float_t *sp2  = source2.GetMatrixArray();
        Float_t *trp  = target.GetMatrixArray(); // pointer to UR part and diagonal, traverse row-wise
        Float_t *tcp  = trp;                     // pointer to LL part,              traverse col-wise
  for (Int_t i = 0; i < nrows; i++) {
    sp2 += i;
    srp1 += i; trp += i;              // point to [i,i]
    scp1 += i*ncols; tcp += i*ncols;  // point to [i,i]
    for (Int_t j = i; j < ncols; j++) {
      if (j > i) *tcp = *scp1 <= *sp2;
      *trp++ = *srp1++ <= *sp2++;
      scp1 += ncols;
      tcp  += ncols;
    }
    scp1 -= nelems-1; // point to [0,i]
    tcp  -= nelems-1; // point to [0,i]
  }

  return target;
}

//______________________________________________________________________________
TMatrixFSym operator<(const TMatrixFSym &source1,const TMatrixFSym &source2)
{
  // source1 < source2

  TMatrixFSym target;

  if (!AreCompatible(source1,source2)) {
    Error("operator<(const TMatrixFSym&,const TMatrixFSym&)","matrices not compatible");
    target.Invalidate();
    return target;
  }

  target.ResizeTo(source1);

  const Int_t nelems = target.GetNoElements();
  const Int_t nrows  = target.GetNrows();
  const Int_t ncols  = target.GetNcols();

  const Float_t *srp1 = source1.GetMatrixArray();
  const Float_t *scp1 = srp1;
  const Float_t *sp2  = source2.GetMatrixArray();
        Float_t *trp  = target.GetMatrixArray(); // pointer to UR part and diagonal, traverse row-wise
        Float_t *tcp  = trp;                     // pointer to LL part,              traverse col-wise
  for (Int_t i = 0; i < nrows; i++) {
    sp2 += i;
    srp1 += i; trp += i;              // point to [i,i]
    scp1 += i*ncols; tcp += i*ncols;  // point to [i,i]
    for (Int_t j = i; j < ncols; j++) {
      if (j > i) *tcp = *scp1 < *sp2;
      *trp++ = *srp1++ < *sp2++;
      scp1 += ncols;
      tcp  += ncols;
    }
    scp1 -= nelems-1; // point to [0,i]
    tcp  -= nelems-1; // point to [0,i]
  }

  return target;
}

//______________________________________________________________________________
TMatrixFSym &Add(TMatrixFSym &target,Float_t scalar,const TMatrixFSym &source)
{
  // Modify addition: target += scalar * source.

  if (!AreCompatible(target,source)) {
    ::Error("Add","matrices not compatible");
    target.Invalidate();
    return target;
  }

  const Int_t nrows   = target.GetNrows();
  const Int_t ncols   = target.GetNcols();
  const Int_t nelems  = target.GetNoElements();
  const Float_t *sp  = source.GetMatrixArray();
        Float_t *trp = target.GetMatrixArray(); // pointer to UR part and diagonal, traverse row-wise
        Float_t *tcp = target.GetMatrixArray(); // pointer to LL part,              traverse col-wise
  for (Int_t i = 0; i < nrows; i++) {
    sp  += i;
    trp += i;        // point to [i,i]
    tcp += i*ncols;  // point to [i,i]
    for (Int_t j = i; j < ncols; j++) {
      const Float_t tmp = scalar * *sp++;
      if (j > i) *tcp += tmp;
      *trp++ += tmp;
      tcp += ncols;
    }
    tcp -= nelems-1; // point to [0,i]
  }

  return target;
}

//______________________________________________________________________________
TMatrixFSym &ElementMult(TMatrixFSym &target,const TMatrixFSym &source)
{
  // Multiply target by the source, element-by-element.

  if (!AreCompatible(target,source)) {
    ::Error("ElementMult","matrices not compatible");
    target.Invalidate();
    return target;
  }

  const Int_t nrows   = target.GetNrows();
  const Int_t ncols   = target.GetNcols();
  const Int_t nelems  = target.GetNoElements();
  const Float_t *sp  = source.GetMatrixArray();
        Float_t *trp = target.GetMatrixArray(); // pointer to UR part and diagonal, traverse row-wise
        Float_t *tcp = target.GetMatrixArray(); // pointer to LL part,              traverse col-wise
  for (Int_t i = 0; i < nrows; i++) {
    sp  += i;
    trp += i;        // point to [i,i]
    tcp += i*ncols;  // point to [i,i]
    for (Int_t j = i; j < ncols; j++) {
      if (j > i) *tcp *= *sp;
      *trp++ *= *sp++;
      tcp += ncols;
    }
    tcp -= nelems-1; // point to [0,i]
  }

  return target;
}

//______________________________________________________________________________
TMatrixFSym &ElementDiv(TMatrixFSym &target,const TMatrixFSym &source)
{
  // Multiply target by the source, element-by-element.

  if (!AreCompatible(target,source)) {
    ::Error("ElementDiv","matrices not compatible");
    target.Invalidate();
    return target;
  }

  const Int_t nrows   = target.GetNrows();
  const Int_t ncols   = target.GetNcols();
  const Int_t nelems  = target.GetNoElements();
  const Float_t *sp  = source.GetMatrixArray();
        Float_t *trp = target.GetMatrixArray(); // pointer to UR part and diagonal, traverse row-wise
        Float_t *tcp = target.GetMatrixArray(); // pointer to LL part,              traverse col-wise
  for (Int_t i = 0; i < nrows; i++) {
    sp  += i;
    trp += i;        // point to [i,i]
    tcp += i*ncols;  // point to [i,i]
    for (Int_t j = i; j < ncols; j++) {
      Assert(*sp != 0.0);
      if (j > i) *tcp /= *sp;
      *trp++ /= *sp++;
      tcp += ncols;
    }
    tcp -= nelems-1; // point to [0,i]
  }

  return target;
}

//______________________________________________________________________________
void TMatrixFSym::Streamer(TBuffer &R__b)
{
  // Stream an object of class TMatrixFSym.

  if (R__b.IsReading()) {
    UInt_t R__s, R__c;
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c);
    Clear();
    TMatrixFBase::Class()->ReadBuffer(R__b,this,R__v,R__s,R__c);
    fElements = new Float_t[fNelems];
    Int_t i;
    for (i = 0; i < fNrows; i++) {
      R__b.ReadFastArray(fElements+i*fNcols+i,fNcols-i);
    }
    // copy to Lower left triangle
    for (i = 0; i < fNrows; i++) {
      for (Int_t j = 0; j < i; j++) {
        fElements[i*fNcols+j] = fElements[j*fNrows+i];
      }
    }
    if (fNelems <= kSizeMax) {
      memcpy(fDataStack,fElements,fNelems*sizeof(Float_t));
      delete [] fElements;
      fElements = fDataStack;
    }
  } else {
    TMatrixFBase::Class()->WriteBuffer(R__b,this);
    // Only write the Upper right triangle
    for (Int_t i = 0; i < fNrows; i++) {
      R__b.WriteFastArray(fElements+i*fNcols+i,fNcols-i);
    }
  }
}
