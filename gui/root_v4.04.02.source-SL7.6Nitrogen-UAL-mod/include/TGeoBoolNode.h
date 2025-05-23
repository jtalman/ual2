// @(#):$Name:  $:$Id: TGeoBoolNode.h,v 1.11 2005/04/05 10:17:00 brun Exp $
// Author: Andrei Gheata   30/05/02

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGeoBoolNode
#define ROOT_TGeoBoolNode

//#ifndef ROOT_TGeoShape
//#include "TGeoShape.h"
//#endif

// forward declarations
class TGeoShape;
class TGeoMatrix;
class TGeoHMatrix;

/*************************************************************************
 * TGeoBoolNode - Base class for boolean nodes. A boolean node has pointers
 *  to two shapes having two transformations with respect to the mother
 *  composite shape they belong to. It represents the boolean operation
 *  between the two component shapes. 
 *
 *************************************************************************/

class TGeoBoolNode : public TObject
{
public:
enum EGeoBoolType {
   kGeoUnion,
   kGeoIntersection,
   kGeoSubtraction
};

protected:
   Int_t             fSelected;       // ! selected branch
   TGeoShape        *fLeft;           // shape on the left branch
   TGeoShape        *fRight;          // shape on the right branch
   TGeoMatrix       *fLeftMat;        // transformation that applies to the left branch
   TGeoMatrix       *fRightMat;       // transformation that applies to the right branch
// methods
   Bool_t            MakeBranch(const char *expr, Bool_t left);
public:
   // constructors
   TGeoBoolNode();
   TGeoBoolNode(const char *expr1, const char *expr2);
   TGeoBoolNode(TGeoShape *left, TGeoShape *right, TGeoMatrix *lmat=0, TGeoMatrix *rmat=0);

   // destructor
   virtual ~TGeoBoolNode();
   // methods
   virtual void      ComputeBBox(Double_t &dx, Double_t &dy, Double_t &dz, Double_t *origin) = 0;
   virtual void      ComputeNormal(Double_t *point, Double_t *dir, Double_t *norm) = 0;
   virtual Bool_t    Contains(Double_t *point) const         = 0;
   virtual Int_t     DistanceToPrimitive(Int_t px, Int_t py) = 0;
   virtual Double_t  DistFromInside(Double_t *point, Double_t *dir, Int_t iact=1,
                               Double_t step=0, Double_t *safe=0) const = 0;
   virtual Double_t  DistFromOutside(Double_t *point, Double_t *dir, Int_t iact=1,
                               Double_t step=0, Double_t *safe=0) const = 0;
   virtual EGeoBoolType GetBooleanOperator() const = 0;
   virtual Int_t     GetNpoints() const = 0;
   TGeoMatrix       *GetLeftMatrix() const {return fLeftMat;}
   TGeoMatrix       *GetRightMatrix() const {return fRightMat;}
   TGeoShape        *GetLeftShape() const {return fLeft;}
   TGeoShape        *GetRightShape() const {return fRight;}
   virtual void      Paint(Option_t *option);
   virtual Double_t  Safety(Double_t *point, Bool_t in=kTRUE) const = 0;
   virtual void      SetPoints(Double_t *points) const = 0;
   virtual void      SetPoints(Float_t *points)  const = 0;
   void              SetSelected(Int_t sel) {fSelected = sel;}
   virtual void      Sizeof3D() const;

  ClassDef(TGeoBoolNode, 1)              // a boolean node
};

/*************************************************************************
 * TGeoUnion - Boolean node representing a union between two components.
 *  
 *  
 *************************************************************************/

class TGeoUnion : public TGeoBoolNode
{
public:
   // constructors
   TGeoUnion();
   TGeoUnion(const char *expr1, const char *expr2);
   TGeoUnion(TGeoShape *left, TGeoShape *right, TGeoMatrix *lmat=0, TGeoMatrix *rmat=0);

   // destructor
   virtual ~TGeoUnion();
   // methods
   virtual void      ComputeBBox(Double_t &dx, Double_t &dy, Double_t &dz, Double_t *origin);
   virtual void      ComputeNormal(Double_t *point, Double_t *dir, Double_t *norm);
   virtual Bool_t    Contains(Double_t *point) const;
   virtual Int_t     DistanceToPrimitive(Int_t px, Int_t py);
   virtual Double_t  DistFromInside(Double_t *point, Double_t *dir, Int_t iact=1,
                               Double_t step=0, Double_t *safe=0) const;
   virtual Double_t  DistFromOutside(Double_t *point, Double_t *dir, Int_t iact=1,
                               Double_t step=0, Double_t *safe=0) const;
   virtual EGeoBoolType GetBooleanOperator() const {return kGeoUnion;}
   virtual Int_t     GetNpoints() const;
   virtual Double_t  Safety(Double_t *point, Bool_t in=kTRUE) const;
   virtual void      SetPoints(Double_t *points) const;
   virtual void      SetPoints(Float_t *points)  const;
   virtual void      Sizeof3D() const;

   //CS specific
   virtual void      Paint(Option_t *option);
   ///////////////////////////////////////////

  ClassDef(TGeoUnion, 1)              // union node
};

/*************************************************************************
 * TGeoIntersection - Boolean node representing an intersection between two 
 * components.
 *  
 *************************************************************************/

class TGeoIntersection : public TGeoBoolNode
{
public:
   // constructors
   TGeoIntersection();
   TGeoIntersection(const char *expr1, const char *expr2);
   TGeoIntersection(TGeoShape *left, TGeoShape *right, TGeoMatrix *lmat=0, TGeoMatrix *rmat=0);

   // destructor
   virtual ~TGeoIntersection();
   // methods
   virtual void      ComputeBBox(Double_t &dx, Double_t &dy, Double_t &dz, Double_t *origin);
   virtual void      ComputeNormal(Double_t *point, Double_t *dir, Double_t *norm);
   virtual Bool_t    Contains(Double_t *point) const;
   virtual Int_t     DistanceToPrimitive(Int_t px, Int_t py);
   virtual Double_t  DistFromInside(Double_t *point, Double_t *dir, Int_t iact=1,
                               Double_t step=0, Double_t *safe=0) const;
   virtual Double_t  DistFromOutside(Double_t *point, Double_t *dir, Int_t iact=1,
                               Double_t step=0, Double_t *safe=0) const;
   virtual EGeoBoolType GetBooleanOperator() const {return kGeoIntersection;}
   virtual Int_t     GetNpoints() const;
   virtual Double_t  Safety(Double_t *point, Bool_t in=kTRUE) const;
   virtual void      SetPoints(Double_t *points) const;
   virtual void      SetPoints(Float_t *points)  const;
   virtual void      Sizeof3D() const;

   //CS specific
   virtual void      Paint(Option_t *option);
   ///////////////////////////////////////////


  ClassDef(TGeoIntersection, 1)              // intersection node
};

/*************************************************************************
 * TGeoSubtraction - Boolean node representing a subtraction.
 * 
 *  
 *************************************************************************/

class TGeoSubtraction : public TGeoBoolNode
{
public:
   // constructors
   TGeoSubtraction();
   TGeoSubtraction(const char *expr1, const char *expr2);
   TGeoSubtraction(TGeoShape *left, TGeoShape *right, TGeoMatrix *lmat=0, TGeoMatrix *rmat=0);

   // destructor
   virtual ~TGeoSubtraction();
   // methods
   virtual void      ComputeBBox(Double_t &dx, Double_t &dy, Double_t &dz, Double_t *origin);
   virtual void      ComputeNormal(Double_t *point, Double_t *dir, Double_t *norm);
   virtual Bool_t    Contains(Double_t *point) const;
   virtual Int_t     DistanceToPrimitive(Int_t px, Int_t py);
   virtual Double_t  DistFromInside(Double_t *point, Double_t *dir, Int_t iact=1,
                               Double_t step=0, Double_t *safe=0) const;
   virtual Double_t  DistFromOutside(Double_t *point, Double_t *dir, Int_t iact=1,
                               Double_t step=0, Double_t *safe=0) const;
   virtual EGeoBoolType GetBooleanOperator() const {return kGeoSubtraction;}
   virtual Int_t     GetNpoints() const;
   virtual Double_t  Safety(Double_t *point, Bool_t in=kTRUE) const;
   virtual void      SetPoints(Double_t *points) const;
   virtual void      SetPoints(Float_t *points)  const;
   virtual void      Sizeof3D() const;

   //CS specific
   virtual void      Paint(Option_t *option);
   ///////////////////////////////////////////


  ClassDef(TGeoSubtraction, 1)              // subtraction node
};
#endif

