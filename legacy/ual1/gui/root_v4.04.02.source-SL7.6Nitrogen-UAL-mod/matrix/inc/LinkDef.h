/* @(#)root/matrix:$Name:  $:$Id: LinkDef.h,v 1.20 2005/04/22 15:32:38 brun Exp $ */

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class TVectorF-;
#pragma link C++ class TMatrixFBase-;
#pragma link C++ class TMatrixF-;
#pragma link C++ class TMatrixFSym-;

#pragma link C++ class TMatrixFLazy+;
#pragma link C++ class TMatrixFSymLazy+;
#pragma link C++ class THaarMatrixF+;
#pragma link C++ class THilbertMatrixF+;
#pragma link C++ class THilbertMatrixFSym+;

#pragma link C++ class TMatrixFRow;
#pragma link C++ class TMatrixFColumn;
#pragma link C++ class TMatrixFDiag;
#pragma link C++ class TMatrixFFlat;
#pragma link C++ class TMatrixFSub;
#pragma link C++ class TMatrixFRow_const;
#pragma link C++ class TMatrixFColumn_const;
#pragma link C++ class TMatrixFDiag_const;
#pragma link C++ class TMatrixFFlat_const;
#pragma link C++ class TMatrixFSub_const;

#pragma link C++ class TVector-;
#pragma link C++ class TMatrix-;
#pragma link C++ class TMatrixLazy+;
#pragma link C++ class TMatrixRow;
#pragma link C++ class TMatrixColumn;
#pragma link C++ class TMatrixDiag;
#pragma link C++ class TMatrixFlat;

#pragma link C++ function operator==   (const TVectorF       &,const TVectorF &);
#pragma link C++ function operator+    (const TVectorF       &,const TVectorF &);
#pragma link C++ function operator-    (const TVectorF       &,const TVectorF &);
#pragma link C++ function operator*    (const TVectorF       &,const TVectorF &);
#pragma link C++ function operator*    (const TMatrixF       &,const TVectorF &);
#pragma link C++ function operator*    (const TMatrixFSym    &,const TVectorF &);
#pragma link C++ function operator*    (const TMatrixDSparse &,const TVectorD &);
#pragma link C++ function operator*    (      Float_t         ,const TVectorF &);
#pragma link C++ function Add          (      TVectorF       &,      Float_t   ,const TVectorF &);
#pragma link C++ function AddElemMult  (      TVectorF       &,      Float_t   ,const TVectorF &,const TVectorF &);
#pragma link C++ function AddElemMult  (      TVectorF       &,      Float_t   ,const TVectorF &,const TVectorF &,const TVectorF &);
#pragma link C++ function AddElemDiv   (      TVectorF       &,      Float_t   ,const TVectorF &,const TVectorF &);
#pragma link C++ function AddElemDiv   (      TVectorF       &,      Float_t   ,const TVectorF &,const TVectorF &,const TVectorF &);
#pragma link C++ function ElementMult  (      TVectorF       &,const TVectorF &);
#pragma link C++ function ElementMult  (      TVectorF       &,const TVectorF &,const TVectorF &);
#pragma link C++ function ElementDiv   (      TVectorF       &,const TVectorF &);
#pragma link C++ function ElementDiv   (      TVectorF       &,const TVectorF &,const TVectorF &);
#pragma link C++ function AreCompatible(const TVectorF       &,const TVectorF &,      Int_t);
#pragma link C++ function AreCompatible(const TVectorF       &,const TVectorD &,      Int_t);
#pragma link C++ function Compare      (const TVectorF       &,const TVectorF &);

#pragma link C++ function AreCompatible(const TMatrixFBase &,const TMatrixFBase &,Int_t);
#pragma link C++ function AreCompatible(const TMatrixFBase &,const TMatrixDBase &,Int_t);
#pragma link C++ function Compare      (const TMatrixFBase &,const TMatrixFBase &);
#pragma link C++ function E2Norm       (const TMatrixFBase &,const TMatrixFBase &);
#pragma link C++ function operator==   (const TMatrixFBase &,const TMatrixFBase &);

#pragma link C++ function operator+    (const TMatrixF     &,const TMatrixF     &);
#pragma link C++ function operator+    (const TMatrixF     &,const TMatrixFSym  &);
#pragma link C++ function operator+    (const TMatrixFSym  &,const TMatrixF     &);
#pragma link C++ function operator+    (const TMatrixF     &,      Float_t       );
#pragma link C++ function operator+    (      Float_t       ,const TMatrixF     &);
#pragma link C++ function operator-    (const TMatrixF     &,const TMatrixF     &);
#pragma link C++ function operator-    (const TMatrixF     &,const TMatrixFSym  &);
#pragma link C++ function operator-    (const TMatrixFSym  &,const TMatrixF     &);
#pragma link C++ function operator-    (const TMatrixF     &,      Float_t       );
#pragma link C++ function operator-    (      Float_t       ,const TMatrixF     &);
#pragma link C++ function operator*    (      Float_t       ,const TMatrixF     &);
#pragma link C++ function operator*    (const TMatrixF     &,      Float_t       );
#pragma link C++ function operator*    (const TMatrixF      ,const TMatrixF     &);
#pragma link C++ function operator*    (const TMatrixF     &,const TMatrixFSym  &);
#pragma link C++ function operator*    (const TMatrixFSym  &,const TMatrixF     &);
#pragma link C++ function operator*    (const TMatrixFSym  &,const TMatrixFSym  &);

#pragma link C++ function Add          (      TMatrixF     &,      Float_t ,     const TMatrixF    &);
#pragma link C++ function Add          (      TMatrixF     &,      Float_t ,     const TMatrixFSym &);
#pragma link C++ function ElementMult  (      TMatrixF     &,const TMatrixF     &);
#pragma link C++ function ElementMult  (      TMatrixF     &,const TMatrixFSym  &);
#pragma link C++ function ElementDiv   (      TMatrixF     &,const TMatrixF     &);
#pragma link C++ function ElementDiv   (      TMatrixF     &,const TMatrixFSym  &);

#pragma link C++ function operator+    (const TMatrixFSym  &,const TMatrixFSym  &);
#pragma link C++ function operator-    (const TMatrixFSym  &,const TMatrixFSym  &);
#pragma link C++ function operator*    (      Float_t       ,const TMatrixFSym  &);
#pragma link C++ function operator*    (const TMatrixFSym  &,         Float_t    );

#pragma link C++ function Add          (      TMatrixFSym  &,         Float_t ,  const TMatrixFSym &);
#pragma link C++ function ElementMult  (      TMatrixFSym  &,const TMatrixFSym  &);
#pragma link C++ function ElementDiv   (      TMatrixFSym  &,const TMatrixFSym  &);

#pragma link C++ function VerifyVectorValue   (const TVectorF     &,Float_t             ,Int_t,Float_t );
#pragma link C++ function VerifyVectorIdentity(const TVectorF     &,const TVectorF     &,Int_t,Float_t );
#pragma link C++ function VerifyMatrixValue   (const TMatrixFBase &,Float_t             ,Int_t,Float_t );
#pragma link C++ function VerifyMatrixIdentity(const TMatrixFBase &,const TMatrixFBase &,Int_t,Float_t );

#pragma link C++ class TVectorD-;
#pragma link C++ class TMatrixDBase-;
#pragma link C++ class TMatrixD-;
#pragma link C++ class TMatrixDSym-;
#pragma link C++ class TMatrixDSparse+;

#pragma link C++ class TMatrixDLazy+;
#pragma link C++ class TMatrixDSymLazy+;
#pragma link C++ class THaarMatrixD+;
#pragma link C++ class THilbertMatrixD+;
#pragma link C++ class THilbertMatrixDSym+;

#pragma link C++ class TMatrixDRow;
#pragma link C++ class TMatrixDColumn;
#pragma link C++ class TMatrixDDiag;
#pragma link C++ class TMatrixDFlat;
#pragma link C++ class TMatrixDSub;
#pragma link C++ class TMatrixDRow_const;
#pragma link C++ class TMatrixDColumn_const;
#pragma link C++ class TMatrixDDiag_const;
#pragma link C++ class TMatrixDFlat_const;
#pragma link C++ class TMatrixDSub_const;

#pragma link C++ class TMatrixDSparseRow_const;
#pragma link C++ class TMatrixDSparseRow;
#pragma link C++ class TMatrixDSparseDiag_const;
#pragma link C++ class TMatrixDSparseDiag;

#pragma link C++ class TMatrixDEigen+;
#pragma link C++ class TMatrixDSymEigen+;

#pragma link C++ class TDecompBase+;
#pragma link C++ class TDecompBK+;
#pragma link C++ class TDecompChol+;
#pragma link C++ class TDecompLU+;
#pragma link C++ class TDecompQRH+;
#pragma link C++ class TDecompSVD+;
#pragma link C++ class TDecompSparse+;

Bool_t    operator==    (const TVectorD       &source1,const TVectorD &source2);
TVectorD  operator+     (const TVectorD       &source1,const TVectorD &source2);
TVectorD  operator-     (const TVectorD       &source1,const TVectorD &source2);
Double_t  operator*     (const TVectorD       &source1,const TVectorD &source2);

TVectorD  operator*     (const TMatrixD       &a,      const TVectorD &source);
TVectorD  operator*     (const TMatrixDSym    &a,      const TVectorD &source);
TVectorD  operator*     (const TMatrixDSparse &a,      const TVectorD &source);

TVectorD  operator*     (      Double_t        val,    const TVectorD &source);
TVectorD  &Add          (      TVectorD       &target,       Double_t  scalar,const TVectorD &source);

#pragma link C++ function operator==   (const TVectorD       &,const TVectorD &);
#pragma link C++ function operator+    (const TVectorD       &,const TVectorD &);
#pragma link C++ function operator-    (const TVectorD       &,const TVectorD &);
#pragma link C++ function operator*    (const TVectorD       &,const TVectorD &);
#pragma link C++ function operator*    (const TMatrixD       &,const TVectorD &);
#pragma link C++ function operator*    (const TMatrixDSym    &,const TVectorD &);
#pragma link C++ function operator*    (const TMatrixDSparse &,const TVectorD &);
#pragma link C++ function operator*    (      Double_t        ,const TVectorD &);
#pragma link C++ function Add          (      TVectorD       &,      Double_t  ,const TVectorD &);
#pragma link C++ function AddElemMult  (      TVectorD       &,      Double_t  ,const TVectorD &,const TVectorD &);
#pragma link C++ function AddElemMult  (      TVectorD       &,      Double_t  ,const TVectorD &,const TVectorD &,const TVectorD &);
#pragma link C++ function AddElemDiv   (      TVectorD       &,      Double_t  ,const TVectorD &,const TVectorD &);
#pragma link C++ function AddElemDiv   (      TVectorD       &,      Double_t  ,const TVectorD &,const TVectorD &,const TVectorD &);
#pragma link C++ function ElementMult  (      TVectorD       &,const TVectorD &);
#pragma link C++ function ElementMult  (      TVectorD       &,const TVectorD &,const TVectorD &);
#pragma link C++ function ElementDiv   (      TVectorD       &,const TVectorD &);
#pragma link C++ function ElementDiv   (      TVectorD       &,const TVectorD &,const TVectorD &);
#pragma link C++ function AreCompatible(const TVectorD       &,const TVectorD &,      Int_t);
#pragma link C++ function AreCompatible(const TVectorD       &,const TVectorF &,      Int_t);
#pragma link C++ function Compare      (const TVectorD       &,const TVectorD &);

#pragma link C++ function AreCompatible(const TMatrixDBase &,const TMatrixDBase &,Int_t);
#pragma link C++ function AreCompatible(const TMatrixDBase &,const TMatrixFBase &,Int_t);
#pragma link C++ function Compare      (const TMatrixDBase &,const TMatrixDBase &);
#pragma link C++ function E2Norm       (const TMatrixDBase &,const TMatrixDBase &);
#pragma link C++ function operator==   (const TMatrixDBase &,const TMatrixDBase &);

#pragma link C++ function operator+    (const TMatrixD     &,const TMatrixD     &);
#pragma link C++ function operator+    (const TMatrixD     &,const TMatrixDSym  &);
#pragma link C++ function operator+    (const TMatrixDSym  &,const TMatrixD     &);
#pragma link C++ function operator+    (const TMatrixD     &,      Double_t      );
#pragma link C++ function operator+    (      Double_t      ,const TMatrixD     &);
#pragma link C++ function operator-    (const TMatrixD     &,const TMatrixD     &);
#pragma link C++ function operator-    (const TMatrixD     &,const TMatrixDSym  &);
#pragma link C++ function operator-    (const TMatrixDSym  &,const TMatrixD     &);
#pragma link C++ function operator-    (const TMatrixD     &,      Double_t      );
#pragma link C++ function operator-    (      Double_t      ,const TMatrixD     &);
#pragma link C++ function operator*    (      Double_t      ,const TMatrixD     &);
#pragma link C++ function operator*    (const TMatrixD     &,      Double_t      );
#pragma link C++ function operator*    (const TMatrixD      ,const TMatrixD     &);
#pragma link C++ function operator*    (const TMatrixD     &,const TMatrixDSym  &);
#pragma link C++ function operator*    (const TMatrixDSym  &,const TMatrixD     &);
#pragma link C++ function operator*    (const TMatrixDSym  &,const TMatrixDSym  &);

#pragma link C++ function Add          (      TMatrixD     &,      Double_t,     const TMatrixD    &);
#pragma link C++ function Add          (      TMatrixD     &,      Double_t,     const TMatrixDSym &);
#pragma link C++ function ElementMult  (      TMatrixD     &,const TMatrixD     &);
#pragma link C++ function ElementMult  (      TMatrixD     &,const TMatrixDSym  &);
#pragma link C++ function ElementDiv   (      TMatrixD     &,const TMatrixD     &);
#pragma link C++ function ElementDiv   (      TMatrixD     &,const TMatrixDSym  &);

#pragma link C++ function operator+    (const TMatrixDSym  &,const TMatrixDSym  &);
#pragma link C++ function operator-    (const TMatrixDSym  &,const TMatrixDSym  &);
#pragma link C++ function operator*    (      Double_t      ,const TMatrixDSym  &);
#pragma link C++ function operator*    (const TMatrixDSym  &,         Double_t   );

#pragma link C++ function Add          (      TMatrixDSym  &,         Double_t,  const TMatrixDSym &);
#pragma link C++ function ElementMult  (      TMatrixDSym  &,const TMatrixDSym  &);
#pragma link C++ function ElementDiv   (      TMatrixDSym  &,const TMatrixDSym  &);

#pragma link C++ function VerifyVectorValue   (const TVectorD     &,Double_t            ,Int_t,Double_t);
#pragma link C++ function VerifyVectorIdentity(const TVectorD     &,const TVectorD     &,Int_t,Double_t);
#pragma link C++ function VerifyMatrixValue   (const TMatrixDBase &,Double_t            ,Int_t,Double_t);
#pragma link C++ function VerifyMatrixIdentity(const TMatrixDBase &,const TMatrixDBase &,Int_t,Double_t);

#pragma link C++ function NormalEqn (const TMatrixD &,const TVectorD &                 );
#pragma link C++ function NormalEqn (const TMatrixD &,const TVectorD &,const TVectorD &);
#pragma link C++ function NormalEqn (const TMatrixD &,const TMatrixD &                 );
#pragma link C++ function NormalEqn (const TMatrixD &,const TMatrixD &,const TVectorD &);

#pragma link C++ function AreCompatible(const TMatrixDSparse &,const TMatrixDSparse &,Int_t);

#pragma link C++ function operator+    (const TMatrixDSparse &,const TMatrixDSparse &);
#pragma link C++ function operator+    (const TMatrixDSparse &,const TMatrixD       &);
#pragma link C++ function operator+    (const TMatrixD       &,const TMatrixDSparse &);
#pragma link C++ function operator+    (const TMatrixDSparse &,      Double_t        );
#pragma link C++ function operator+    (      Double_t        ,const TMatrixDSparse &);
#pragma link C++ function operator-    (const TMatrixDSparse &,const TMatrixDSparse &);
#pragma link C++ function operator-    (const TMatrixDSparse &,const TMatrixD       &);
#pragma link C++ function operator-    (const TMatrixD       &,const TMatrixDSparse &);
#pragma link C++ function operator-    (const TMatrixDSparse &,      Double_t        );
#pragma link C++ function operator-    (      Double_t        ,const TMatrixDSparse &);
#pragma link C++ function operator*    (const TMatrixDSparse &,const TMatrixDSparse &);
#pragma link C++ function operator*    (const TMatrixDSparse &,const TMatrixD       &);
#pragma link C++ function operator*    (const TMatrixD       &,const TMatrixDSparse &);
#pragma link C++ function operator*    (      Double_t        ,const TMatrixDSparse &);
#pragma link C++ function operator*    (const TMatrixDSparse &,      Double_t        );

#pragma link C++ function Add          (TMatrixDSparse &,      Double_t        ,const TMatrixDSparse &);
#pragma link C++ function ElementMult  (TMatrixDSparse &,const TMatrixDSparse &);
#pragma link C++ function ElementDiv   (TMatrixDSparse &,const TMatrixDSparse &);

#endif
