// @(#)root/geom:$Name:  $:$Id: TGeoPolygon.h,v 1.2 2004/01/29 11:59:10 brun Exp $
// Author: Mihaela Gheata   05/01/04

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGeoPolygon
#define ROOT_TGeoPolygon

#ifndef ROOT_TObject
#include "TObject.h"
#endif

#ifndef ROOT_TObjArray
#include "TObjArray.h"
#endif

/*************************************************************************
 * TGeoPolygon - An arbitrary polygon defined by vertices. The vertices
 *   have to be defined CLOCKWISE in the XY plane, making either a convex or
 *   concave polygon. No test for malformed polygons is performed.
 *
 *************************************************************************/

class TGeoPolygon : public TObject
{
public:
  enum {
      kGeoConvex        = BIT(9),
      kGeoFinishPolygon = BIT(10)
   };
protected :
// data members
   Int_t               fNvert;            // number of vertices (must be defined clockwise in XY plane)
   Int_t               fNconvex;          // number of points of the outscribed convex polygon 
   Int_t              *fInd;              //[fNvert] list of vertex indices
   Int_t              *fIndc;             //[fNconvex] indices of vertices of the outscribed convex polygon
   Double_t           *fX;                // pointer to list of current X coordinates of vertices
   Double_t           *fY;                // pointer to list of current Y coordinates of vertices
   TObjArray          *fDaughters;        // list of concave daughters
private:
   void                ConvexCheck(); // force convexity checking
   Bool_t              IsSegConvex(Int_t i1, Int_t i2=-1) const;
   Bool_t              IsRightSided(Double_t *point, Int_t ind1, Int_t ind2) const;
   void                OutscribedConvex();
public:
   // constructors
   TGeoPolygon();
   TGeoPolygon(Int_t nvert);
   // destructor
   virtual ~TGeoPolygon();
   // methods
   Bool_t              Contains(Double_t *point) const;
   void                FinishPolygon();
   Int_t               GetNvert() const {return fNvert;}
   Double_t           *GetX() {return fX;}
   Double_t           *GetY() {return fY;}
   Bool_t              IsConvex() const {return TObject::TestBit(kGeoConvex);}
   Bool_t              IsFinished() const {return TObject::TestBit(kGeoFinishPolygon);}
   Double_t            Safety(Double_t *point, Int_t &isegment) const;
   void                SetConvex(Bool_t flag=kTRUE) {TObject::SetBit(kGeoConvex,flag);}
   void                SetXY(Double_t *x, Double_t *y) {fX=x; fY=y;}   
   void                SetNextIndex(Int_t index=-1);

  ClassDef(TGeoPolygon, 1)         // class for handling arbitrary polygons
};

#endif

