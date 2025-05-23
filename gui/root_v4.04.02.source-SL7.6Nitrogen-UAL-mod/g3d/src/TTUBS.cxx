// @(#)root/g3d:$Name:  $:$Id: TTUBS.cxx,v 1.7 2005/03/09 18:19:26 brun Exp $
// Author: Nenad Buncic   18/09/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "TTUBS.h"
#include "TNode.h"
#include "TVirtualPad.h"
#include "TBuffer3D.h"
#include "TBuffer3DTypes.h"
#include "TGeometry.h"

ClassImp(TTUBS)


//______________________________________________________________________________
// Begin_Html <P ALIGN=CENTER> <IMG SRC="gif/tubs.gif"> </P> End_Html
// TUBS is a segment of a tube. It has 8 parameters:
//
//     - name       name of the shape
//     - title      shape's title
//     - material  (see TMaterial)
//     - rmin       inside radius
//     - rmax       outside radius
//     - dz         half length in z
//     - phi1       starting angle of the segment
//     - phi2       ending angle of the segment
//
//
// NOTE: phi1 should be smaller than phi2. If this is not the case,
//       the system adds 360 degrees to phi2.


//______________________________________________________________________________
TTUBS::TTUBS()
{
   // TUBS shape default constructor
}


//______________________________________________________________________________
TTUBS::TTUBS(const char *name, const char *title, const char *material, Float_t rmin,
             Float_t rmax, Float_t dz, Float_t phi1, Float_t phi2)
      : TTUBE(name,title,material,rmin,rmax,dz)
{
   // TUBS shape normal constructor

   fPhi1 = phi1;
   fPhi2 = phi2;
   MakeTableOfCoSin();
}


//______________________________________________________________________________
TTUBS::TTUBS(const char *name, const char *title, const char *material, Float_t rmax, Float_t dz,
               Float_t phi1, Float_t phi2)
      : TTUBE(name,title,material,rmax,dz)
{
   // TUBS shape "simplified" constructor

   fPhi1 = phi1;
   fPhi2 = phi2;
   MakeTableOfCoSin();
}
//______________________________________________________________________________
void TTUBS::MakeTableOfCoSin() const
{
   const Double_t PI  = TMath::ATan(1) * 4.0;
   const Double_t TWOPI  =2*PI;
   const Double_t ragrad  = PI/180.0;

   Int_t j;
   Int_t n = GetNumberOfDivisions () + 1;

   if (fCoTab)
      delete [] fCoTab; // Delete the old tab if any
      fCoTab = new Double_t [n];
   if (!fCoTab ) return;

   if (fSiTab)
      delete [] fSiTab; // Delete the old tab if any
   fSiTab = new Double_t [n];
   if (!fSiTab ) return;

   Double_t phi1    = Double_t(fPhi1  * ragrad);
   Double_t phi2    = Double_t(fPhi2  * ragrad);

   if (phi1 > phi2 ) phi2 += TWOPI;

   Double_t range = phi2- phi1;

   Double_t angstep = range/(n-1);

   Double_t ph = phi1;
   for (j = 0; j < n; j++) {
      ph = phi1 + j*angstep;
      fCoTab[j] = TMath::Cos(ph);
      fSiTab[j] = TMath::Sin(ph);
   }
}


//______________________________________________________________________________
TTUBS::~TTUBS()
{
   // TUBS shape default destructor
}


//______________________________________________________________________________
Int_t TTUBS::DistancetoPrimitive(Int_t px, Int_t py)
{
   // Compute distance from point px,py to a TUBE
   //
   // Compute the closest distance of approach from point px,py to each
   // computed outline point of the TUBE.

   Int_t n = GetNumberOfDivisions()+1;
   Int_t numPoints = n*4;
   return ShapeDistancetoPrimitive(numPoints,px,py);
}


//______________________________________________________________________________
void TTUBS::SetPoints(Double_t *points) const
{
   // Create TUBS points

   Int_t j, n;
   Int_t indx = 0;
   Float_t dz = TTUBE::fDz;

   n = GetNumberOfDivisions()+1;

   if (points) {
      if (!fCoTab)   MakeTableOfCoSin();
      for (j = 0; j < n; j++) {
         points[indx+6*n] = points[indx] = fRmin * fCoTab[j];
         indx++;
         points[indx+6*n] = points[indx] = fAspectRatio*fRmin * fSiTab[j];
         indx++;
         points[indx+6*n] = dz;
         points[indx]     =-dz;
         indx++;
      }
      for (j = 0; j < n; j++) {
         points[indx+6*n] = points[indx] = fRmax * fCoTab[j];
         indx++;
         points[indx+6*n] = points[indx] = fAspectRatio*fRmax * fSiTab[j];
         indx++;
         points[indx+6*n]= dz;
         points[indx]    =-dz;
         indx++;
      }
   }
}


//______________________________________________________________________________
void TTUBS::Sizeof3D() const
{
   // Return total X3D needed by TNode::ls (when called with option "x")

   Int_t n = GetNumberOfDivisions()+1;

   gSize3D.numPoints += n*4;
   gSize3D.numSegs   += n*8;
   gSize3D.numPolys  += n*4-2;  
}


//_______________________________________________________________________
const TBuffer3D & TTUBS::GetBuffer3D(Int_t reqSections) const
{
   static TBuffer3D buffer(TBuffer3DTypes::kGeneric);

   TShape::FillBuffer3D(buffer, reqSections);

   // TODO: Although we now have a TBuffer3DTubeSeg class for
   // tube segment shapes, we do not use it for old geometry shapes, as 
   // OGL viewer needs various rotation matrix info we can't easily
   // pass yet. To be revisited.

   // We also don't provide a bounding box - as fiddly to calculate
   // leave to viewer to work it out from points

   if (reqSections & TBuffer3D::kRawSizes) {
      const Int_t n = GetNumberOfDivisions()+1;
      Int_t NbPnts = 4*n;
      Int_t NbSegs = 2*NbPnts;
      Int_t NbPols = NbPnts-2;

      if (buffer.SetRawSizes(NbPnts, 3*NbPnts, NbSegs, 3*NbSegs, NbPols, 6*NbPols)) {
         buffer.SetSectionsValid(TBuffer3D::kRawSizes);
      }
   }
   if (reqSections & TBuffer3D::kRaw) {
      // Points
      SetPoints(buffer.fPnts);
      if (!buffer.fLocalFrame) {
         TransformPoints(buffer.fPnts, buffer.NbPnts());
      }

      const Int_t n = GetNumberOfDivisions()+1;
      Int_t i,j;
      Int_t c = GetBasicColor();

      // Segments
      memset(buffer.fSegs, 0, buffer.NbSegs()*3*sizeof(Int_t));
      for (i = 0; i < 4; i++) {
         for (j = 1; j < n; j++) {
            buffer.fSegs[(i*n+j-1)*3  ] = c;
            buffer.fSegs[(i*n+j-1)*3+1] = i*n+j-1;
            buffer.fSegs[(i*n+j-1)*3+2] = i*n+j;
         }
      }
      for (i = 4; i < 6; i++) {
         for (j = 0; j < n; j++) {
            buffer.fSegs[(i*n+j)*3  ] = c+1;
            buffer.fSegs[(i*n+j)*3+1] = (i-4)*n+j;
            buffer.fSegs[(i*n+j)*3+2] = (i-2)*n+j;
         }
      }
      for (i = 6; i < 8; i++) {
         for (j = 0; j < n; j++) {
            buffer.fSegs[(i*n+j)*3  ] = c;
            buffer.fSegs[(i*n+j)*3+1] = 2*(i-6)*n+j;
            buffer.fSegs[(i*n+j)*3+2] = (2*(i-6)+1)*n+j;
         }
      }

      // Polygons
      Int_t indx = 0;
      memset(buffer.fPols, 0, buffer.NbPols()*6*sizeof(Int_t));
      i = 0;
      for (j = 0; j < n-1; j++) {
         buffer.fPols[indx++] = c;
         buffer.fPols[indx++] = 4;
         buffer.fPols[indx++] = (4+i)*n+j+1;
         buffer.fPols[indx++] = (2+i)*n+j;
         buffer.fPols[indx++] = (4+i)*n+j;
         buffer.fPols[indx++] = i*n+j;
      }
      i = 1;
      for (j = 0; j < n-1; j++) {
         buffer.fPols[indx++] = c;
         buffer.fPols[indx++] = 4;
         buffer.fPols[indx++] = i*n+j;
         buffer.fPols[indx++] = (4+i)*n+j;
         buffer.fPols[indx++] = (2+i)*n+j;
         buffer.fPols[indx++] = (4+i)*n+j+1;
      }
      i = 2;
      for (j = 0; j < n-1; j++) {
         buffer.fPols[indx++] = c+i;
         buffer.fPols[indx++] = 4;
         buffer.fPols[indx++] = (i-2)*2*n+j;
         buffer.fPols[indx++] = (4+i)*n+j;
         buffer.fPols[indx++] = ((i-2)*2+1)*n+j;
         buffer.fPols[indx++] = (4+i)*n+j+1;
      }
      i = 3;
      for (j = 0; j < n-1; j++) {
         buffer.fPols[indx++] = c+i;
         buffer.fPols[indx++] = 4;
         buffer.fPols[indx++] = (4+i)*n+j+1;
         buffer.fPols[indx++] = ((i-2)*2+1)*n+j;
         buffer.fPols[indx++] = (4+i)*n+j;
         buffer.fPols[indx++] = (i-2)*2*n+j;
      }
      buffer.fPols[indx++] = c+2;
      buffer.fPols[indx++] = 4;
      buffer.fPols[indx++] = 6*n;
      buffer.fPols[indx++] = 4*n;
      buffer.fPols[indx++] = 7*n;
      buffer.fPols[indx++] = 5*n;
      buffer.fPols[indx++] = c+2;
      buffer.fPols[indx++] = 4;
      buffer.fPols[indx++] = 6*n-1;
      buffer.fPols[indx++] = 8*n-1;
      buffer.fPols[indx++] = 5*n-1;
      buffer.fPols[indx++] = 7*n-1;

      buffer.SetSectionsValid(TBuffer3D::kRaw);
   }
   return buffer;
}
