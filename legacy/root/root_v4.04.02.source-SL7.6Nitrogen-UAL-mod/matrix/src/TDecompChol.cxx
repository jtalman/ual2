// @(#)root/matrix:$Name:  $:$Id: TDecompChol.cxx,v 1.15 2005/02/15 16:17:09 brun Exp $
// Authors: Fons Rademakers, Eddy Offermann  Dec 2003

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

///////////////////////////////////////////////////////////////////////////
//                                                                       //
// Cholesky Decomposition class                                          //
//                                                                       //
// Decompose a symmetric, positive definite matrix A = U^T * U           //
//                                                                       //
// where U is a upper triangular matrix                                  //
//                                                                       //
// The decomposition fails if a diagonal element of fU is <= 0, the      //
// matrix is not positive negative . The matrix fU is made invalid .     // 
//                                                                       //
// fU has the same index range as A .                                    //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include "TDecompChol.h"

ClassImp(TDecompChol)

//______________________________________________________________________________
TDecompChol::TDecompChol(Int_t nrows)
{
  fU.ResizeTo(nrows,nrows);
}

//______________________________________________________________________________
TDecompChol::TDecompChol(Int_t row_lwb,Int_t row_upb)
{
  const Int_t nrows = row_upb-row_lwb+1;
  fRowLwb = row_lwb;
  fColLwb = row_lwb;
  fU.ResizeTo(row_lwb,row_lwb+nrows-1,row_lwb,row_lwb+nrows-1);
}

//______________________________________________________________________________
TDecompChol::TDecompChol(const TMatrixDSym &a,Double_t tol)
{
  Assert(a.IsValid());

  SetBit(kMatrixSet);
  fCondition = a.Norm1();
  fTol = a.GetTol();
  if (tol > 0)
    fTol = tol;

  fRowLwb = a.GetRowLwb();
  fColLwb = a.GetColLwb();
  fU.ResizeTo(a);
  fU = a;
}

//______________________________________________________________________________
TDecompChol::TDecompChol(const TMatrixD &a,Double_t tol)
{
  Assert(a.IsValid());

  if (a.GetNrows() != a.GetNcols() || a.GetRowLwb() != a.GetColLwb()) {
    Error("TDecompChol(const TMatrixD &","matrix should be square");
    return;
  }

  SetBit(kMatrixSet);
  fCondition = a.Norm1();
  fTol = a.GetTol();
  if (tol > 0)
    fTol = tol;

  fRowLwb = a.GetRowLwb();
  fColLwb = a.GetColLwb();
  fU.ResizeTo(a);
  fU = a;
}

//______________________________________________________________________________
TDecompChol::TDecompChol(const TDecompChol &another) : TDecompBase(another)
{
  *this = another;
}

//______________________________________________________________________________
Bool_t TDecompChol::Decompose()
{
  if ( !TestBit(kMatrixSet) )
    return kFALSE;

  Int_t i,j,icol,irow;
  const Int_t     n  = fU.GetNrows();
        Double_t *pU = fU.GetMatrixArray();
  for (icol = 0; icol < n; icol++) {
    const Int_t rowOff = icol*n;
   
    //Compute fU(j,j) and test for non-positive-definiteness.
    Double_t ujj = pU[rowOff+icol];
    for (irow = 0; irow < icol; irow++) {
      const Int_t pos_ij = irow*n+icol;
      ujj -= pU[pos_ij]*pU[pos_ij];
    }
    if (ujj <= 0) {
      Error("Decompose()","matrix not positive definite");
      return kFALSE;
    }
    ujj = TMath::Sqrt(ujj);
    pU[rowOff+icol] = ujj;

    if (icol < n-1) {
      for (j = icol+1; j < n; j++) {
        for (i = 0; i < icol; i++) {
          const Int_t rowOff2 = i*n;
          pU[rowOff+j] -= pU[rowOff2+j]*pU[rowOff2+icol];
        }
      }
      for (j = icol+1; j < n; j++)
        pU[rowOff+j] /= ujj;
    }
  }

  for (irow = 0; irow < n; irow++) {
    const Int_t rowOff = irow*n;
    for (icol = 0; icol < irow; icol++)
      pU[rowOff+icol] = 0.;
  }

  SetBit(kDecomposed);

  return kTRUE;
}

//______________________________________________________________________________
const TMatrixDSym TDecompChol::GetMatrix()
{
// Reconstruct the original matrix using the decomposition parts

  if (TestBit(kSingular)) {
    TMatrixDSym tmp; tmp.Invalidate();
    return tmp;
  }
  if ( !TestBit(kDecomposed) ) {
    if (!Decompose()) {
      TMatrixDSym tmp; tmp.Invalidate();
      return tmp;
    }
  }

  return TMatrixDSym(TMatrixDSym::kAtA,fU);
}

//______________________________________________________________________________
void TDecompChol::SetMatrix(const TMatrixDSym &a)
{
  Assert(a.IsValid());
  
  ResetStatus();
  if (a.GetNrows() != a.GetNcols() || a.GetRowLwb() != a.GetColLwb()) {
    Error("SetMatrix(const TMatrixDSym &","matrix should be square");
    return;
  } 
  
  SetBit(kMatrixSet);
  fCondition = -1.0;
    
  fRowLwb = a.GetRowLwb();
  fColLwb = a.GetColLwb();
  fU.ResizeTo(a);
  fU = a;
}

//______________________________________________________________________________
Bool_t TDecompChol::Solve(TVectorD &b)
{
// Solve equations Ax=b assuming A has been factored by Cholesky. The factor U is
// assumed to be in upper triang of fU. fTol is used to determine if diagonal
// element is zero. The solution is returned in b.

  Assert(b.IsValid());
  if (TestBit(kSingular)) {
    b.Invalidate();
    return kFALSE;
  }
  if ( !TestBit(kDecomposed) ) {
    if (!Decompose()) {
      b.Invalidate();
      return kFALSE;
    }
  }

  if (fU.GetNrows() != b.GetNrows() || fU.GetRowLwb() != b.GetLwb()) {
    Error("Solve(TVectorD &","vector and matrix incompatible");
    b.Invalidate();
    return kFALSE;
  }

  const Int_t n = fU.GetNrows();

  const Double_t *pU = fU.GetMatrixArray();
        Double_t *pb = b.GetMatrixArray();

  Int_t i;
  // step 1: Forward substitution on U^T
  for (i = 0; i < n; i++) {
    const Int_t off_i = i*n;
    if (pU[off_i+i] < fTol)
    {
      Error("Solve(TVectorD &b)","u[%d,%d]=%.4e < %.4e",i,i,pU[off_i+i],fTol);
      b.Invalidate();
      return kFALSE;
    }
    Double_t r = pb[i];
    for (Int_t j = 0; j < i; j++) {
      const Int_t off_j = j*n;
      r -= pU[off_j+i]*pb[j];
    }
    pb[i] = r/pU[off_i+i];
  }

 // step 2: Backward substitution on U
  for (i = n-1; i >= 0; i--) {
    const Int_t off_i = i*n;
    Double_t r = pb[i];
    for (Int_t j = i+1; j < n; j++)
      r -= pU[off_i+j]*pb[j];
    pb[i] = r/pU[off_i+i];
  }

  return kTRUE;
}

//______________________________________________________________________________
Bool_t TDecompChol::Solve(TMatrixDColumn &cb)
{ 
  TMatrixDBase *b = const_cast<TMatrixDBase *>(cb.GetMatrix());
  Assert(b->IsValid());
  if (TestBit(kSingular)) {
    b->Invalidate();
    return kFALSE;
  }
  if ( !TestBit(kDecomposed) ) {
    if (!Decompose()) {
      b->Invalidate();
      return kFALSE;
    }
  }

  if (fU.GetNrows() != b->GetNrows() || fU.GetRowLwb() != b->GetRowLwb())
  { 
    Error("Solve(TMatrixDColumn &cb","vector and matrix incompatible");
    b->Invalidate();
    return kFALSE; 
  }
      
  const Int_t n = fU.GetNrows();
    
  const Double_t *pU  = fU.GetMatrixArray();
        Double_t *pcb = cb.GetPtr();
  const Int_t     inc = cb.GetInc(); 
  
  Int_t i;
  // step 1: Forward substitution U^T
  for (i = 0; i < n; i++) { 
    const Int_t off_i  = i*n;
    const Int_t off_i2 = i*inc;
    if (pU[off_i+i] < fTol)
    {
      Error("Solve(TMatrixDColumn &cb)","u[%d,%d]=%.4e < %.4e",i,i,pU[off_i+i],fTol);
      b->Invalidate();
      return kFALSE;
    }
    Double_t r = pcb[off_i2];
    for (Int_t j = 0; j < i; j++) {
      const Int_t off_j = j*n;
      r -= pU[off_j+i]*pcb[j*inc];
    }
    pcb[off_i2] = r/pU[off_i+i];
  }

  // step 2: Backward substitution U
  for (i = n-1; i >= 0; i--) {
    const Int_t off_i  = i*n;
    const Int_t off_i2 = i*inc;
    Double_t r = pcb[off_i2];
    for (Int_t j = i+1; j < n; j++)
      r -= pU[off_i+j]*pcb[j*inc];
    pcb[off_i2] = r/pU[off_i+i];
  }

  return kTRUE;
}

//______________________________________________________________________________
void TDecompChol::Det(Double_t &d1,Double_t &d2)
{
  // determinant is square of diagProd of cholesky factor

  if ( !TestBit(kDetermined) ) {
    if ( !TestBit(kDecomposed) )
      Decompose();
    TDecompBase::Det(d1,d2);
    // square det as calculated by above
    fDet1 *= fDet1;
    fDet2 += fDet2;
    SetBit(kDetermined);
  }
  d1 = fDet1;
  d2 = fDet2;
}

//______________________________________________________________________________
void TDecompChol::Invert(TMatrixDSym &inv)
{
  // For a symmetric matrix A(m,m), its inverse A_inv(m,m) is returned .

  if (inv.GetNrows() != GetNrows() || inv.GetRowLwb() != GetRowLwb()) {
    Error("Invert(TMatrixDSym &","Input matrix has wrong shape");
    inv.Invalidate();
    return;
  }

  inv.UnitMatrix();

  const Int_t colLwb = inv.GetColLwb();
  const Int_t colUpb = inv.GetColUpb();
  Bool_t status = kTRUE;
  for (Int_t icol = colLwb; icol <= colUpb && status; icol++) {
    TMatrixDColumn b(inv,icol);
    status &= Solve(b);
  }
  
  if (!status)
    inv.Invalidate();
}

//______________________________________________________________________________
TMatrixDSym TDecompChol::Invert()
{ 
  // For a symmetric matrix A(m,m), its inverse A_inv(m,m) is returned .

  const Int_t rowLwb = GetRowLwb();
  const Int_t rowUpb = rowLwb+GetNrows()-1;

  TMatrixDSym inv(rowLwb,rowUpb);
  inv.UnitMatrix();
  Invert(inv);

  return inv;
}

//______________________________________________________________________________
void TDecompChol::Print(Option_t *opt) const
{
  TDecompBase::Print(opt);
  fU.Print("fU");
}

//______________________________________________________________________________
TDecompChol &TDecompChol::operator=(const TDecompChol &source)
{ 
  if (this != &source) {
    TDecompBase::operator=(source);
    fU.ResizeTo(source.fU);
    fU = source.fU;
  }
  return *this;
}     

//______________________________________________________________________________
TVectorD NormalEqn(const TMatrixD &A,const TVectorD &b)
{
  // Solve min {(A . x - b)^T (A . x - b)} for vector x where
  //   A : (m x n) matrix, m >= n
  //   b : (m)     vector
  //   x : (n)     vector

  TDecompChol ch(TMatrixDSym(TMatrixDBase::kAtA,A));
  Bool_t ok;
  return ch.Solve(TMatrixD(TMatrixDBase::kTransposed,A)*b,ok);
}

//______________________________________________________________________________
TVectorD NormalEqn(const TMatrixD &A,const TVectorD &b,const TVectorD &std)
{
  // Solve min {(A . x - b)^T W (A . x - b)} for vector x where
  //   A : (m x n) matrix, m >= n
  //   b : (m)     vector
  //   x : (n)     vector
  //   W : (m x m) weight matrix with W(i,j) = 1/std(i)^2  for i == j
  //                                         = 0           fir i != j

  if (!AreCompatible(b,std)) {
    ::Error("NormalEqn","vectors b and std are not compatible");
    TVectorD tmp; tmp.Invalidate();
    return tmp;
  }

  TMatrixD Aw = A;
  TVectorD bw = b;
  for (Int_t irow = 0; irow < A.GetNrows(); irow++) {
    TMatrixDRow(Aw,irow) *= 1/std(irow);
    bw(irow) /= std(irow);
  }
  TDecompChol ch(TMatrixDSym(TMatrixDBase::kAtA,Aw));
  Bool_t ok;
  return ch.Solve(TMatrixD(TMatrixDBase::kTransposed,Aw)*bw,ok);
}

//______________________________________________________________________________
TMatrixD NormalEqn(const TMatrixD &A,const TMatrixD &B)
{
  // Solve min {(A . X_j - B_j)^T (A . X_j - B_j)} for each column j in
  // B and X
  //   A : (m x n ) matrix, m >= n
  //   B : (m x nb) matrix, nb >= 1
  //   X : (n x nb) matrix

  TDecompChol ch(TMatrixDSym(TMatrixDBase::kAtA,A));
  TMatrixD X(A,TMatrixDBase::kTransposeMult,B);
  ch.MultiSolve(X);
  return X;
}

//______________________________________________________________________________
TMatrixD NormalEqn(const TMatrixD &A,const TMatrixD &B,const TVectorD &std)
{
  // Solve min {(A . X_j - B_j)^T W (A . X_j - B_j)} for each column j in
  // B and X
  //   A : (m x n ) matrix, m >= n
  //   B : (m x nb) matrix, nb >= 1
  //   X : (n x nb) matrix
  //   W : (m x m) weight matrix with W(i,j) = 1/std(i)^2  for i == j
  //                                         = 0           fir i != j

  TMatrixD Aw = A;
  TMatrixD Bw = B;
  for (Int_t irow = 0; irow < A.GetNrows(); irow++) {
    TMatrixDRow(Aw,irow) *= 1/std(irow);
    TMatrixDRow(Bw,irow) *= 1/std(irow);
  }

  TDecompChol ch(TMatrixDSym(TMatrixDBase::kAtA,Aw));
  TMatrixD X(Aw,TMatrixDBase::kTransposeMult,Bw);
  ch.MultiSolve(X);
  return X;
}
