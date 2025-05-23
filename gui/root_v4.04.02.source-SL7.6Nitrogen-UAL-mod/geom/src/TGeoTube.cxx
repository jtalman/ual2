// @(#)root/geom:$Name:  $:$Id: TGeoTube.cxx,v 1.60 2005/04/25 21:12:08 rdm Exp $
// Author: Andrei Gheata   24/10/01
// TGeoTube::Contains() and DistFromInside/In() implemented by Mihaela Gheata

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//_____________________________________________________________________________
// TGeoTube - cylindrical tube class. It takes 3 parameters :
//            inner radius, outer radius and half-length dz.
//
//_____________________________________________________________________________
//Begin_Html
/*
<img src="gif/t_tube.gif">
*/
//End_Html
//Begin_Html
/*
<img src="gif/t_tubedivR.gif">
*/
//End_Html
//Begin_Html
/*
<img src="gif/t_tubedivstepR.gif">
*/
//End_Html
//Begin_Html
/*
<img src="gif/t_tubedivPHI.gif">
*/
//End_Html
//Begin_Html
/*
<img src="gif/t_tubedivstepPHI.gif">
*/
//End_Html
//Begin_Html
/*
<img src="gif/t_tubedivZ.gif">
*/
//End_Html
//Begin_Html
/*
<img src="gif/t_tubedivstepZ.gif">
*/
//End_Html
//_____________________________________________________________________________
// TGeoTubeSeg - a phi segment of a tube. Has 5 parameters :
//            - the same 3 as a tube;
//            - first phi limit (in degrees)
//            - second phi limit
//
//_____________________________________________________________________________
//Begin_Html
/*
<img src="gif/t_tubseg.gif">
*/
//End_Html
//Begin_Html
/*
<img src="gif/t_tubsegdivstepR.gif">
*/
//End_Html
//Begin_Html
/*
<img src="gif/t_tubsegdivPHI.gif">
*/
//End_Html
//Begin_Html
/*
<img src="gif/t_tubsegdivZ.gif">
*/
//End_Html
//_____________________________________________________________________________
// TGeoCtub - a tube segment cut with 2 planes. Has 11 parameters :
//            - the same 5 as a tube segment;
//            - x, y, z components of the normal to the -dZ cut plane in
//              point (0, 0, -dZ);
//            - x, y, z components of the normal to the +dZ cut plane in
//              point (0, 0, dZ);
//
//_____________________________________________________________________________
//Begin_Html
/*
<img src="gif/t_ctub.gif">
*/
//End_Html

#include "Riostream.h"
#include "TROOT.h"

#include "TGeoManager.h"
#include "TGeoVolume.h"
#include "TVirtualGeoPainter.h"
#include "TGeoTube.h"
#include "TVirtualPad.h"
#include "TBuffer3D.h"
#include "TBuffer3DTypes.h"

ClassImp(TGeoTube)

//_____________________________________________________________________________
TGeoTube::TGeoTube()
{
// Default constructor
   SetShapeBit(TGeoShape::kGeoTube);
   fRmin = 0.0;
   fRmax = 0.0;
   fDz   = 0.0;
}


//_____________________________________________________________________________
TGeoTube::TGeoTube(Double_t rmin, Double_t rmax, Double_t dz)
           :TGeoBBox(0, 0, 0)
{
// Default constructor specifying minimum and maximum radius
   SetShapeBit(TGeoShape::kGeoTube);
   SetTubeDimensions(rmin, rmax, dz);
   if ((fDz<0) || (fRmin<0) || (fRmax<0)) {
      SetShapeBit(kGeoRunTimeShape);
//      if (fRmax<=fRmin) SetShapeBit(kGeoInvalidShape);
//      printf("tube : dz=%f rmin=%f rmax=%f\n", dz, rmin, rmax);
   }
   ComputeBBox();
}
//_____________________________________________________________________________
TGeoTube::TGeoTube(const char *name, Double_t rmin, Double_t rmax, Double_t dz)
           :TGeoBBox(name, 0, 0, 0)
{
// Default constructor specifying minimum and maximum radius
   SetShapeBit(TGeoShape::kGeoTube);
   SetTubeDimensions(rmin, rmax, dz);
   if ((fDz<0) || (fRmin<0) || (fRmax<0)) {
      SetShapeBit(kGeoRunTimeShape);
//      if (fRmax<=fRmin) SetShapeBit(kGeoInvalidShape);
//      printf("tube : dz=%f rmin=%f rmax=%f\n", dz, rmin, rmax);
   }
   ComputeBBox();
}

//_____________________________________________________________________________
TGeoTube::TGeoTube(Double_t *param)
         :TGeoBBox(0, 0, 0)
{
// Default constructor specifying minimum and maximum radius
// param[0] = Rmin
// param[1] = Rmax
// param[2] = dz
   SetShapeBit(TGeoShape::kGeoTube);
   SetDimensions(param);
   if ((fDz<0) || (fRmin<0) || (fRmax<0)) SetShapeBit(kGeoRunTimeShape);
   ComputeBBox();
}

//_____________________________________________________________________________
TGeoTube::~TGeoTube()
{
// destructor
}

//_____________________________________________________________________________
void TGeoTube::ComputeBBox()
{
// compute bounding box of the tube
   fDX = fDY = fRmax;
   fDZ = fDz;
}

//_____________________________________________________________________________
void TGeoTube::ComputeNormal(Double_t *point, Double_t *dir, Double_t *norm)
{
// Compute normal to closest surface from POINT.
   Double_t saf[3];
   Double_t rsq = point[0]*point[0]+point[1]*point[1];
   Double_t r = TMath::Sqrt(rsq);
   saf[0] = TMath::Abs(fDz-TMath::Abs(point[2]));
   saf[1] = (fRmin>1E-10)?TMath::Abs(r-fRmin):TGeoShape::Big();
   saf[2] = TMath::Abs(fRmax-r);
   Int_t i = TMath::LocMin(3,saf);
   if (i==0) {
      norm[0] = norm[1] = 0.;
      norm[2] = TMath::Sign(1.,dir[2]);
      return;
   }
   norm[2] = 0;
   Double_t phi = TMath::ATan2(point[1], point[0]);
   norm[0] = TMath::Cos(phi);
   norm[1] = TMath::Sin(phi);
   if (norm[0]*dir[0]+norm[1]*dir[1]<0) {
      norm[0] = -norm[0];
      norm[1] = -norm[1];
   }
}

//_____________________________________________________________________________
void TGeoTube::ComputeNormalS(Double_t *point, Double_t *dir, Double_t *norm,
                              Double_t /*rmin*/, Double_t /*rmax*/, Double_t /*dz*/)
{
// Compute normal to closest surface from POINT.
   norm[2] = 0;
   Double_t phi = TMath::ATan2(point[1], point[0]);
   norm[0] = TMath::Cos(phi);
   norm[1] = TMath::Sin(phi);
   if (norm[0]*dir[0]+norm[1]*dir[1]<0) {
      norm[0] = -norm[0];
      norm[1] = -norm[1];
   }
}

//_____________________________________________________________________________
Bool_t TGeoTube::Contains(Double_t *point) const
{
// test if point is inside this tube
   if (TMath::Abs(point[2]) > fDz) return kFALSE;
   Double_t r2 = point[0]*point[0]+point[1]*point[1];
   if ((r2<fRmin*fRmin) || (r2>fRmax*fRmax)) return kFALSE;
   return kTRUE;
}

//_____________________________________________________________________________
Int_t TGeoTube::DistancetoPrimitive(Int_t px, Int_t py)
{
// compute closest distance from point px,py to each corner
   Int_t n = gGeoManager->GetNsegments();
   const Int_t numPoints = 4*n;
   return ShapeDistancetoPrimitive(numPoints, px, py);
}

//_____________________________________________________________________________
Double_t TGeoTube::DistFromInsideS(Double_t *point, Double_t *dir, Double_t rmin, Double_t rmax, Double_t dz)
{
// Compute distance from inside point to surface of the tube (static)
// Boundary safe algorithm.
   // compute distance to surface
   // Do Z
   Double_t sz = TGeoShape::Big();
   if (dir[2]) {
      sz = (TMath::Sign(dz, dir[2])-point[2])/dir[2];
      if (sz<=0) return 0.0;
   }
   // Do R
   Double_t nsq=dir[0]*dir[0]+dir[1]*dir[1];
   if (TMath::Abs(nsq)<TGeoShape::Tolerance()) return sz;
   Double_t rsq=point[0]*point[0]+point[1]*point[1];
   Double_t rdotn=point[0]*dir[0]+point[1]*dir[1];
   Double_t b,d;
   Double_t sr = TGeoShape::Big();
   // inner cylinder
   if (rmin>0) {
      // Protection in case point is actually outside the tube
      if (rsq <= rmin*rmin+TGeoShape::Tolerance()) {
         if (rdotn<0) return 0.0;
      } else {
         if (rdotn<0) {
            DistToTube(rsq,nsq,rdotn,rmin,b,d);
            if (d>0) {
               sr=-b-d;
               if (sr>0) return TMath::Min(sz,sr);
            }
         }
      }
   }
   // outer cylinder
   if (rsq >= rmax*rmax-TGeoShape::Tolerance()) {
      if (rdotn>=0) return 0.0;
   }
   DistToTube(rsq,nsq,rdotn,rmax,b,d);
   if (d>0) {
      sr=-b+d;
      if (sr>0) return TMath::Min(sz,sr);
   }
   return 0.;
}

//_____________________________________________________________________________
Double_t TGeoTube::DistFromInside(Double_t *point, Double_t *dir, Int_t iact, Double_t step, Double_t *safe) const
{
// Compute distance from inside point to surface of the tube
// Boundary safe algorithm.
   if (iact<3 && safe) {
      *safe = Safety(point, kTRUE);
      if (iact==0) return TGeoShape::Big();
      if ((iact==1) && (*safe>step)) return TGeoShape::Big();
   }
   // compute distance to surface
   return DistFromInsideS(point, dir, fRmin, fRmax, fDz);
}

//_____________________________________________________________________________
Double_t TGeoTube::DistFromOutsideS(Double_t *point, Double_t *dir, Double_t rmin, Double_t rmax, Double_t dz)
{
// Static method to compute distance from outside point to a tube with given parameters
// Boundary safe algorithm.
   // check Z planes
   Double_t xi,yi,zi;
   Double_t rmaxsq = rmax*rmax;
   Double_t rminsq = rmin*rmin;
   zi = dz - TMath::Abs(point[2]);
   Double_t s = TGeoShape::Big();
   Bool_t in = kFALSE;
   Bool_t inz = (zi<0)?kFALSE:kTRUE;
   if (!inz) {
      if (point[2]*dir[2]>=0) return TGeoShape::Big();
      s  = -zi/TMath::Abs(dir[2]);
      xi = point[0]+s*dir[0];
      yi = point[1]+s*dir[1];
      Double_t r2=xi*xi+yi*yi;
      if ((rminsq<=r2) && (r2<=rmaxsq)) return s;
   }

   Double_t rsq = point[0]*point[0]+point[1]*point[1];
   // check outer cyl. surface
   Double_t nsq=dir[0]*dir[0]+dir[1]*dir[1];
   Double_t rdotn=point[0]*dir[0]+point[1]*dir[1];
   Double_t b,d;
   Bool_t inrmax = kFALSE;
   Bool_t inrmin = kFALSE;
   if (rsq<=rmaxsq+TGeoShape::Tolerance()) inrmax = kTRUE;
   if (rsq>=rminsq-TGeoShape::Tolerance()) inrmin = kTRUE;
   in = inz & inrmin & inrmax;
   // If inside, we are most likely on a boundary within machine precision.
   if (in) {
      Bool_t checkout = kFALSE;
      Double_t r = TMath::Sqrt(rsq);
      if (zi<rmax-r) {
         if ((rmin==0) || (zi<r-rmin)) {
            if (point[2]*dir[2]<0) return 0.0;
            return TGeoShape::Big();
         }
      }
      if ((rmaxsq-rsq) < (rsq-rminsq)) checkout = kTRUE;
      if (checkout) {
         if (rdotn>=0) return TGeoShape::Big();
         return 0.0;
      }
      if (rmin==0) return 0.0;
      if (rdotn>=0) return 0.0;
      // Ray exiting rmin -> check (+) solution for inner tube
      if (TMath::Abs(nsq)<TGeoShape::Tolerance()) return TGeoShape::Big();
      DistToTube(rsq, nsq, rdotn, rmin, b, d);
      if (d>0) {
         s=-b+d;
         if (s>0) {
            zi=point[2]+s*dir[2];
            if (TMath::Abs(zi)<=dz) return s;
         }
      }
      return TGeoShape::Big();
   }
   // Check outer cylinder (only r>rmax has to be considered)
   if (TMath::Abs(nsq)<TGeoShape::Tolerance()) return TGeoShape::Big();
   if (!inrmax) {
      DistToTube(rsq, nsq, rdotn, rmax, b, d);
      if (d>0) {
         s=-b-d;
         if (s>0) {
            zi=point[2]+s*dir[2];
            if (TMath::Abs(zi)<=dz) return s;
         }
      }
   }
   // check inner cylinder
   if (rmin>0) {
      DistToTube(rsq, nsq, rdotn, rmin, b, d);
      if (d>0) {
         s=-b+d;
         if (s>0) {
            zi=point[2]+s*dir[2];
            if (TMath::Abs(zi)<=dz) return s;
         }
      }
   }
   return TGeoShape::Big();
}

//_____________________________________________________________________________
Double_t TGeoTube::DistFromOutside(Double_t *point, Double_t *dir, Int_t iact, Double_t step, Double_t *safe) const
{
// Compute distance from outside point to surface of the tube and safe distance
// Boundary safe algorithm.
   // fist localize point w.r.t tube
   if (iact<3 && safe) {
      *safe = Safety(point, kFALSE);
      if (iact==0) return TGeoShape::Big();
      if ((iact==1) && (step<=*safe)) return TGeoShape::Big();
   }
   // find distance to shape
   return DistFromOutsideS(point, dir, fRmin, fRmax, fDz);
}

//_____________________________________________________________________________
void TGeoTube::DistToTube(Double_t rsq, Double_t nsq, Double_t rdotn, Double_t radius, Double_t &b, Double_t &delta)
{
// Static method computing the distance to a tube with given radius, starting from
// POINT along DIR director cosines. The distance is computed as :
//    RSQ   = point[0]*point[0]+point[1]*point[1]
//    NSQ   = dir[0]*dir[0]+dir[1]*dir[1]  ---> should NOT be 0 !!!
//    RDOTN = point[0]*dir[0]+point[1]*dir[1]
// The distance can be computed as :
//    D = -B +/- DELTA
// where DELTA.GT.0 and D.GT.0

   Double_t t1 = 1./nsq;
   Double_t t3=rsq-(radius*radius);
   b          = t1*rdotn;
   Double_t c =t1*t3;
   delta = b*b-c;
   if (delta>0) {
      delta=TMath::Sqrt(delta);
   } else {
      delta = -1;
   }
}

//_____________________________________________________________________________
TGeoVolume *TGeoTube::Divide(TGeoVolume *voldiv, const char *divname, Int_t iaxis, Int_t ndiv,
                             Double_t start, Double_t step)
{
//--- Divide this tube shape belonging to volume "voldiv" into ndiv volumes
// called divname, from start position with the given step. Returns pointer
// to created division cell volume in case of Z divisions. For radial division
// creates all volumes with different shapes and returns pointer to volume that
// was divided. In case a wrong division axis is supplied, returns pointer to
// volume that was divided.
   TGeoShape *shape;           //--- shape to be created
   TGeoVolume *vol;            //--- division volume to be created
   TGeoVolumeMulti *vmulti;    //--- generic divided volume
   TGeoPatternFinder *finder;  //--- finder to be attached
   TString opt = "";           //--- option to be attached
   Int_t id;
   Double_t end = start+ndiv*step;
   switch (iaxis) {
      case 1:  //---                R division
         finder = new TGeoPatternCylR(voldiv, ndiv, start, end);
         vmulti = gGeoManager->MakeVolumeMulti(divname, voldiv->GetMedium());
         voldiv->SetFinder(finder);
         finder->SetDivIndex(voldiv->GetNdaughters());
         for (id=0; id<ndiv; id++) {
            shape = new TGeoTube(start+id*step, start+(id+1)*step, fDz);
            vol = new TGeoVolume(divname, shape, voldiv->GetMedium());
            vmulti->AddVolume(vol);
            opt = "R";
            voldiv->AddNodeOffset(vol, id, 0, opt.Data());
            ((TGeoNodeOffset*)voldiv->GetNodes()->At(voldiv->GetNdaughters()-1))->SetFinder(finder);
         }
         return vmulti;
      case 2:  //---                Phi division
         finder = new TGeoPatternCylPhi(voldiv, ndiv, start, end);
         voldiv->SetFinder(finder);
         finder->SetDivIndex(voldiv->GetNdaughters());
         shape = new TGeoTubeSeg(fRmin, fRmax, fDz, -step/2, step/2);
         vol = new TGeoVolume(divname, shape, voldiv->GetMedium());
         vmulti = gGeoManager->MakeVolumeMulti(divname, voldiv->GetMedium());
         vmulti->AddVolume(vol);
         opt = "Phi";
         for (id=0; id<ndiv; id++) {
            voldiv->AddNodeOffset(vol, id, start+id*step+step/2, opt.Data());
            ((TGeoNodeOffset*)voldiv->GetNodes()->At(voldiv->GetNdaughters()-1))->SetFinder(finder);
         }
         return vmulti;
      case 3: //---                  Z division
         finder = new TGeoPatternZ(voldiv, ndiv, start, start+ndiv*step);
         voldiv->SetFinder(finder);
         finder->SetDivIndex(voldiv->GetNdaughters());
         shape = new TGeoTube(fRmin, fRmax, step/2);
         vol = new TGeoVolume(divname, shape, voldiv->GetMedium());
         vmulti = gGeoManager->MakeVolumeMulti(divname, voldiv->GetMedium());
         vmulti->AddVolume(vol);
         opt = "Z";
         for (id=0; id<ndiv; id++) {
            voldiv->AddNodeOffset(vol, id, start+step/2+id*step, opt.Data());
            ((TGeoNodeOffset*)voldiv->GetNodes()->At(voldiv->GetNdaughters()-1))->SetFinder(finder);
         }
         return vmulti;
      default:
         Error("Divide", "In shape %s wrong axis type for division", GetName());
         return 0;
   }
}

//_____________________________________________________________________________
const char *TGeoTube::GetAxisName(Int_t iaxis) const
{
// Returns name of axis IAXIS.
   switch (iaxis) {
      case 1:
         return "R";
      case 2:
         return "PHI";
      case 3:
         return "Z";
      default:
         return "UNDEFINED";
   }
}

//_____________________________________________________________________________
Double_t TGeoTube::GetAxisRange(Int_t iaxis, Double_t &xlo, Double_t &xhi) const
{
// Get range of shape for a given axis.
   xlo = 0;
   xhi = 0;
   Double_t dx = 0;
   switch (iaxis) {
      case 1:
         xlo = fRmin;
         xhi = fRmax;
         dx = xhi-xlo;
         return dx;
      case 2:
         xlo = 0;
         xhi = 360;
         dx = 360;
         return dx;
      case 3:
         xlo = -fDz;
         xhi = fDz;
         dx = xhi-xlo;
         return dx;
   }
   return dx;
}

//_____________________________________________________________________________
void TGeoTube::GetBoundingCylinder(Double_t *param) const
{
//--- Fill vector param[4] with the bounding cylinder parameters. The order
// is the following : Rmin, Rmax, Phi1, Phi2, dZ
   param[0] = fRmin; // Rmin
   param[0] *= param[0];
   param[1] = fRmax; // Rmax
   param[1] *= param[1];
   param[2] = 0.;    // Phi1
   param[3] = 360.;  // Phi1
}

//_____________________________________________________________________________
TGeoShape *TGeoTube::GetMakeRuntimeShape(TGeoShape *mother, TGeoMatrix * /*mat*/) const
{
// in case shape has some negative parameters, these has to be computed
// in order to fit the mother
   if (!TestShapeBit(kGeoRunTimeShape)) return 0;
   Double_t rmin, rmax, dz;
   Double_t xmin,xmax;
   rmin = fRmin;
   rmax = fRmax;
   dz = fDz;
   if (fDz<0) {
      mother->GetAxisRange(3,xmin,xmax);
      if (xmax<0) return 0;
      dz=xmax;
   }
   mother->GetAxisRange(1,xmin,xmax);
   if (fRmin<0) {
      if (xmin<0) return 0;
      rmin = xmin;
   }
   if (fRmax<0) {
      if (xmax<=0) return 0;
      rmax = xmax;
   }

   return (new TGeoTube(GetName(), rmin, rmax, dz));
}

//_____________________________________________________________________________
void TGeoTube::InspectShape() const
{
// print shape parameters
   printf("*** Shape %s: TGeoTube ***\n", GetName());
   printf("    Rmin = %11.5f\n", fRmin);
   printf("    Rmax = %11.5f\n", fRmax);
   printf("    dz   = %11.5f\n", fDz);
   printf(" Bounding box:\n");
   TGeoBBox::InspectShape();
}

//_____________________________________________________________________________
TBuffer3D *TGeoTube::MakeBuffer3D() const
{
   // Creates a TBuffer3D describing *this* shape.
   // Coordinates are in local reference frame.

   Int_t n = gGeoManager->GetNsegments();
   Int_t NbPnts = 4*n;
   Int_t NbSegs = 8*n;
   Int_t NbPols = 4*n;
   TBuffer3D* buff = new TBuffer3D(TBuffer3DTypes::kGeneric,
                                   NbPnts, 3*NbPnts, NbSegs, 3*NbSegs, NbPols, 6*NbPols);
   if (buff)
   {
      SetPoints(buff->fPnts);
      SetSegsAndPols(*buff);
   }

   return buff;
}

//_____________________________________________________________________________
void TGeoTube::SetSegsAndPols(TBuffer3D &buffer) const
{
// Fill TBuffer3D structure for segments and polygons.
   Int_t i, j;
   Int_t n = gGeoManager->GetNsegments();
   Int_t c = (((buffer.fColor) %8) -1) * 4;
   if (c < 0) c = 0;

   for (i = 0; i < 4; i++) {
      for (j = 0; j < n; j++) {
         buffer.fSegs[(i*n+j)*3  ] = c;
         buffer.fSegs[(i*n+j)*3+1] = i*n+j;
         buffer.fSegs[(i*n+j)*3+2] = i*n+j+1;
      }
      buffer.fSegs[(i*n+j-1)*3+2] = i*n;
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

   Int_t indx = 0;
   i=0;
   for (j = 0; j < n; j++) {
      indx = 6*(i*n+j);
      buffer.fPols[indx  ] = c;
      buffer.fPols[indx+1] = 4;
      buffer.fPols[indx+5] = i*n+j;
      buffer.fPols[indx+4] = (4+i)*n+j;
      buffer.fPols[indx+3] = (2+i)*n+j;
      buffer.fPols[indx+2] = (4+i)*n+j+1;
   }
   buffer.fPols[indx+2] = (4+i)*n;
   i=1;
   for (j = 0; j < n; j++) {
      indx = 6*(i*n+j);
      buffer.fPols[indx  ] = c;
      buffer.fPols[indx+1] = 4;
      buffer.fPols[indx+2] = i*n+j;
      buffer.fPols[indx+3] = (4+i)*n+j;
      buffer.fPols[indx+4] = (2+i)*n+j;
      buffer.fPols[indx+5] = (4+i)*n+j+1;
   }
   buffer.fPols[indx+5] = (4+i)*n;
   i=2;
   for (j = 0; j < n; j++) {
      indx = 6*(i*n+j);
      buffer.fPols[indx  ] = c+i;
      buffer.fPols[indx+1] = 4;
      buffer.fPols[indx+2] = (i-2)*2*n+j;
      buffer.fPols[indx+3] = (4+i)*n+j;
      buffer.fPols[indx+4] = ((i-2)*2+1)*n+j;
      buffer.fPols[indx+5] = (4+i)*n+j+1;
   }
   buffer.fPols[indx+5] = (4+i)*n;
   i=3;
   for (j = 0; j < n; j++) {
      indx = 6*(i*n+j);
      buffer.fPols[indx  ] = c+i;
      buffer.fPols[indx+1] = 4;
      buffer.fPols[indx+5] = (i-2)*2*n+j;
      buffer.fPols[indx+4] = (4+i)*n+j;
      buffer.fPols[indx+3] = ((i-2)*2+1)*n+j;
      buffer.fPols[indx+2] = (4+i)*n+j+1;
   }
   buffer.fPols[indx+2] = (4+i)*n;
}

//_____________________________________________________________________________
Double_t TGeoTube::Safety(Double_t *point, Bool_t in) const
{
// computes the closest distance from given point to this shape, according
// to option. The matching point on the shape is stored in spoint.
#ifndef NEVER
   Double_t r = TMath::Sqrt(point[0]*point[0]+point[1]*point[1]);
   Double_t safe, safrmin, safrmax;
   if (in) {
      safe    = fDz-TMath::Abs(point[2]); // positive if inside
      if (fRmin>1E-10) {
         safrmin = r-fRmin;
         if (safrmin < safe) safe = safrmin;
      }
      safrmax = fRmax-r;
      if (safrmax < safe) safe = safrmax;
   } else {
      safe    = -fDz+TMath::Abs(point[2]);
      if (fRmin>1E-10) {
         safrmin = -r+fRmin;
         if (safrmin > safe) safe = safrmin;
      }
      safrmax = -fRmax+r;
      if (safrmax > safe) safe = safrmax;
   }
   return safe;
#else
   Double_t saf[3];
   Double_t rsq = point[0]*point[0]+point[1]*point[1];
   Double_t r = TMath::Sqrt(rsq);
   saf[0] = fDz-TMath::Abs(point[2]); // positive if inside
   saf[1] = (fRmin>1E-10)?(r-fRmin):TGeoShape::Big();
   saf[2] = fRmax-r;
   if (in) return saf[TMath::LocMin(3,saf)];
   for (Int_t i=0; i<3; i++) saf[i]=-saf[i];
   return saf[TMath::LocMax(3,saf)];
#endif
}

//_____________________________________________________________________________
Double_t TGeoTube::SafetyS(Double_t *point, Bool_t in, Double_t rmin, Double_t rmax, Double_t dz, Int_t skipz)
{
// computes the closest distance from given point to this shape, according
// to option. The matching point on the shape is stored in spoint.
   Double_t saf[3];
   Double_t rsq = point[0]*point[0]+point[1]*point[1];
   Double_t r = TMath::Sqrt(rsq);
   switch (skipz) {
      case 1: // skip lower Z plane
         saf[0] = dz - point[2];
         break;
      case 2: // skip upper Z plane
         saf[0] = dz + point[2];
         break;
      case 3: // skip both
         saf[0] = TGeoShape::Big();
         break;
      default:
         saf[0] = dz-TMath::Abs(point[2]);
   }
   saf[1] = (rmin>1E-10)?(r-rmin):TGeoShape::Big();
   saf[2] = rmax-r;
//   printf("saf0=%g saf1=%g saf2=%g in=%d skipz=%d\n", saf[0],saf[1],saf[2],in,skipz);
   if (in) return saf[TMath::LocMin(3,saf)];
   for (Int_t i=0; i<3; i++) saf[i]=-saf[i];
   return saf[TMath::LocMax(3,saf)];
}

//_____________________________________________________________________________
void TGeoTube::SavePrimitive(ofstream &out, Option_t * /*option*/)
{
// Save a primitive as a C++ statement(s) on output stream "out".
   if (TObject::TestBit(kGeoSavePrimitive)) return;
   out << "   // Shape: " << GetName() << " type: " << ClassName() << endl;
   out << "   rmin = " << fRmin << ";" << endl;
   out << "   rmax = " << fRmax << ";" << endl;
   out << "   dz   = " << fDz << ";" << endl;
   out << "   pShape = new TGeoTube(\"" << GetName() << "\",rmin,rmax,dz);" << endl;
   TObject::SetBit(TGeoShape::kGeoSavePrimitive);
}

//_____________________________________________________________________________
void TGeoTube::SetTubeDimensions(Double_t rmin, Double_t rmax, Double_t dz)
{
   fRmin = rmin;
   fRmax = rmax;
   fDz   = dz;
   if (fRmin>0 && fRmax>0 && fRmin>=fRmax)
      Error("SetTubeDimensions", "In shape %s wrong rmin=%g rmax=%g", GetName(), rmin,rmax);
}

//_____________________________________________________________________________
void TGeoTube::SetDimensions(Double_t *param)
{
   Double_t rmin = param[0];
   Double_t rmax = param[1];
   Double_t dz   = param[2];
   SetTubeDimensions(rmin, rmax, dz);
}

//_____________________________________________________________________________
void TGeoTube::SetPoints(Double_t *points) const
{
// create tube mesh points
    Double_t dz;
    Int_t j, n;

    n = gGeoManager->GetNsegments();
    Double_t dphi = 360./n;
    Double_t phi = 0;
    dz = fDz;

    Int_t indx = 0;


    if (points) {

        for (j = 0; j < n; j++) {
            phi = j*dphi*TMath::DegToRad();
            points[indx+6*n] = points[indx] = fRmin * TMath::Cos(phi);
            indx++;
            points[indx+6*n] = points[indx] = fRmin * TMath::Sin(phi);
            indx++;
            points[indx+6*n] = dz;
            points[indx]     =-dz;
            indx++;
        }
        for (j = 0; j < n; j++) {
            phi = j*dphi*TMath::DegToRad();
            points[indx+6*n] = points[indx] = fRmax * TMath::Cos(phi);
            indx++;
            points[indx+6*n] = points[indx] = fRmax * TMath::Sin(phi);
            indx++;
            points[indx+6*n]= dz;
            points[indx]    =-dz;
            indx++;
        }
    }
}

//_____________________________________________________________________________
void TGeoTube::SetPoints(Float_t *points) const
{
// create tube mesh points
    Double_t dz;
    Int_t j, n;

    n = gGeoManager->GetNsegments();
    Double_t dphi = 360./n;
    Double_t phi = 0;
    dz = fDz;

    Int_t indx = 0;

    if (points) {

        for (j = 0; j < n; j++) {
            phi = j*dphi*TMath::DegToRad();
            points[indx+6*n] = points[indx] = fRmin * TMath::Cos(phi);
            indx++;
            points[indx+6*n] = points[indx] = fRmin * TMath::Sin(phi);
            indx++;
            points[indx+6*n] = dz;
            points[indx]     =-dz;
            indx++;
        }
        for (j = 0; j < n; j++) {
            phi = j*dphi*TMath::DegToRad();
            points[indx+6*n] = points[indx] = fRmax * TMath::Cos(phi);
            indx++;
            points[indx+6*n] = points[indx] = fRmax * TMath::Sin(phi);
            indx++;
            points[indx+6*n]= dz;
            points[indx]    =-dz;
            indx++;
        }
    }
}

//_____________________________________________________________________________
Int_t TGeoTube::GetNmeshVertices() const
{
// Return number of vertices of the mesh representation
   Int_t n = gGeoManager->GetNsegments();
   Int_t numPoints = n*4;
   return numPoints;
}

//_____________________________________________________________________________
void TGeoTube::Sizeof3D() const
{
///// fill size of this 3-D object
///    TVirtualGeoPainter *painter = gGeoManager->GetGeomPainter();
///    if (!painter) return;
///    Int_t n = gGeoManager->GetNsegments();
///    Int_t numPoints = n*4;
///    Int_t numSegs   = n*8;
///    Int_t numPolys  = n*4;
///    painter->AddSize3D(numPoints, numSegs, numPolys);
}

//_____________________________________________________________________________
const TBuffer3D & TGeoTube::GetBuffer3D(Int_t reqSections, Bool_t localFrame) const
{
   static TBuffer3DTube buffer;
   TGeoBBox::FillBuffer3D(buffer, reqSections, localFrame);

   if (reqSections & TBuffer3D::kShapeSpecific) {
      buffer.fRadiusInner  = fRmin;
      buffer.fRadiusOuter  = fRmax;
      buffer.fHalfLength   = fDz;

      // TODO: HACK
      // At present OGL viewer wants local -> master matrix for tubes
      // even though rest works in master frame. As no one else uses
      // buffer.fLocalMaster we can overwrite it here
      // Once OGL goes to local frame this can all be removed
      // (and subclasses)
      TGeoMatrix * localMasterMat = 0;
      if (gGeoManager->IsMatrixTransform()) {
         localMasterMat = gGeoManager->GetGLMatrix();
      }
      else {
         localMasterMat = gGeoManager->GetCurrentMatrix();
      }
      localMasterMat->GetHomogenousMatrix(buffer.fLocalMaster);
      // End of HACK

      buffer.SetSectionsValid(TBuffer3D::kShapeSpecific);
   }
   if (reqSections & TBuffer3D::kRawSizes) {
      Int_t n = gGeoManager->GetNsegments();
      Int_t NbPnts = 4*n;
      Int_t NbSegs = 8*n;
      Int_t NbPols = 4*n;
      if (buffer.SetRawSizes(NbPnts, 3*NbPnts, NbSegs, 3*NbSegs, NbPols, 6*NbPols)) {
         buffer.SetSectionsValid(TBuffer3D::kRawSizes);
      }
   }
   if ((reqSections & TBuffer3D::kRaw) && buffer.SectionsValid(TBuffer3D::kRawSizes)) {
      SetPoints(buffer.fPnts);
      if (!buffer.fLocalFrame) {
         TransformPoints(buffer.fPnts, buffer.NbPnts());
      }
      SetSegsAndPols(buffer);
      buffer.SetSectionsValid(TBuffer3D::kRaw);
   }

   return buffer;
}

ClassImp(TGeoTubeSeg)

//_____________________________________________________________________________
TGeoTubeSeg::TGeoTubeSeg()
{
// Default constructor
   SetShapeBit(TGeoShape::kGeoTubeSeg);
   fPhi1 = fPhi2 = 0.0;
}

//_____________________________________________________________________________
TGeoTubeSeg::TGeoTubeSeg(Double_t rmin, Double_t rmax, Double_t dz,
                          Double_t phi1, Double_t phi2)
            :TGeoTube(rmin, rmax, dz)
{
// Default constructor specifying minimum and maximum radius
   SetShapeBit(TGeoShape::kGeoTubeSeg);
   SetTubsDimensions(rmin, rmax, dz, phi1, phi2);
   ComputeBBox();
}

//_____________________________________________________________________________
TGeoTubeSeg::TGeoTubeSeg(const char *name, Double_t rmin, Double_t rmax, Double_t dz,
                          Double_t phi1, Double_t phi2)
            :TGeoTube(name, rmin, rmax, dz)
{
// Default constructor specifying minimum and maximum radius
   SetShapeBit(TGeoShape::kGeoTubeSeg);
   SetTubsDimensions(rmin, rmax, dz, phi1, phi2);
   ComputeBBox();
}

//_____________________________________________________________________________
TGeoTubeSeg::TGeoTubeSeg(Double_t *param)
            :TGeoTube(0, 0, 0)
{
// Default constructor specifying minimum and maximum radius
// param[0] = Rmin
// param[1] = Rmax
// param[2] = dz
// param[3] = phi1
// param[4] = phi2
   SetShapeBit(TGeoShape::kGeoTubeSeg);
   SetDimensions(param);
   ComputeBBox();
}

//_____________________________________________________________________________
TGeoTubeSeg::~TGeoTubeSeg()
{
// destructor
}

//_____________________________________________________________________________
void TGeoTubeSeg::ComputeBBox()
{
// compute bounding box of the tube segment
   Double_t xc[4];
   Double_t yc[4];
   xc[0] = fRmax*TMath::Cos(fPhi1*TMath::DegToRad());
   yc[0] = fRmax*TMath::Sin(fPhi1*TMath::DegToRad());
   xc[1] = fRmax*TMath::Cos(fPhi2*TMath::DegToRad());
   yc[1] = fRmax*TMath::Sin(fPhi2*TMath::DegToRad());
   xc[2] = fRmin*TMath::Cos(fPhi1*TMath::DegToRad());
   yc[2] = fRmin*TMath::Sin(fPhi1*TMath::DegToRad());
   xc[3] = fRmin*TMath::Cos(fPhi2*TMath::DegToRad());
   yc[3] = fRmin*TMath::Sin(fPhi2*TMath::DegToRad());

   Double_t xmin = xc[TMath::LocMin(4, &xc[0])];
   Double_t xmax = xc[TMath::LocMax(4, &xc[0])];
   Double_t ymin = yc[TMath::LocMin(4, &yc[0])];
   Double_t ymax = yc[TMath::LocMax(4, &yc[0])];

   Double_t dp = fPhi2-fPhi1;
   if (dp<0) dp+=360;
   Double_t ddp = -fPhi1;
   if (ddp<0) ddp+= 360;
   if (ddp>360) ddp-=360;
   if (ddp<=dp) xmax = fRmax;
   ddp = 90-fPhi1;
   if (ddp<0) ddp+= 360;
   if (ddp>360) ddp-=360;
   if (ddp<=dp) ymax = fRmax;
   ddp = 180-fPhi1;
   if (ddp<0) ddp+= 360;
   if (ddp>360) ddp-=360;
   if (ddp<=dp) xmin = -fRmax;
   ddp = 270-fPhi1;
   if (ddp<0) ddp+= 360;
   if (ddp>360) ddp-=360;
   if (ddp<=dp) ymin = -fRmax;
   fOrigin[0] = (xmax+xmin)/2;
   fOrigin[1] = (ymax+ymin)/2;
   fOrigin[2] = 0;
   fDX = (xmax-xmin)/2;
   fDY = (ymax-ymin)/2;
   fDZ = fDz;
}

//_____________________________________________________________________________
void TGeoTubeSeg::ComputeNormal(Double_t *point, Double_t *dir, Double_t *norm)
{
// Compute normal to closest surface from POINT.
   Double_t saf[3];
   Double_t rsq = point[0]*point[0]+point[1]*point[1];
   Double_t r = TMath::Sqrt(rsq);
   Double_t c1 = TMath::Cos(fPhi1*TMath::DegToRad());
   Double_t s1 = TMath::Sin(fPhi1*TMath::DegToRad());
   Double_t c2 = TMath::Cos(fPhi2*TMath::DegToRad());
   Double_t s2 = TMath::Sin(fPhi2*TMath::DegToRad());
   saf[0] = TMath::Abs(fDz-TMath::Abs(point[2]));
   saf[1] = (fRmin>1E-10)?TMath::Abs(r-fRmin):TGeoShape::Big();
   saf[2] = TMath::Abs(fRmax-r);
   Int_t i = TMath::LocMin(3,saf);
   if (TGeoShape::IsCloseToPhi(saf[i], point,c1,s1,c2,s2)) {
      TGeoShape::NormalPhi(point,dir,norm,c1,s1,c2,s2);
      return;
   }
   if (i==0) {
      norm[0] = norm[1] = 0.;
      norm[2] = TMath::Sign(1.,dir[2]);
      return;
   }
   norm[2] = 0;
   Double_t phi = TMath::ATan2(point[1], point[0]);
   norm[0] = TMath::Cos(phi);
   norm[1] = TMath::Sin(phi);
   if (norm[0]*dir[0]+norm[1]*dir[1]<0) {
      norm[0] = -norm[0];
      norm[1] = -norm[1];
   }
}

//_____________________________________________________________________________
void TGeoTubeSeg::ComputeNormalS(Double_t *point, Double_t *dir, Double_t *norm,
                                 Double_t rmin, Double_t rmax, Double_t /*dz*/,
                                 Double_t c1, Double_t s1, Double_t c2, Double_t s2)
{
// Compute normal to closest surface from POINT.
   Double_t saf[2];
   Double_t rsq = point[0]*point[0]+point[1]*point[1];
   Double_t r = TMath::Sqrt(rsq);
   saf[0] = (rmin>1E-10)?TMath::Abs(r-rmin):TGeoShape::Big();
   saf[1] = TMath::Abs(rmax-r);
   Int_t i = TMath::LocMin(2,saf);
   if (TGeoShape::IsCloseToPhi(saf[i], point,c1,s1,c2,s2)) {
      TGeoShape::NormalPhi(point,dir,norm,c1,s1,c2,s2);
      return;
   }
   norm[2] = 0;
   Double_t phi = TMath::ATan2(point[1], point[0]);
   norm[0] = TMath::Cos(phi);
   norm[1] = TMath::Sin(phi);
   if (norm[0]*dir[0]+norm[1]*dir[1]<0) {
      norm[0] = -norm[0];
      norm[1] = -norm[1];
   }
}

//_____________________________________________________________________________
Bool_t TGeoTubeSeg::Contains(Double_t *point) const
{
// test if point is inside this tube segment
   // first check if point is inside the tube
   if (!TGeoTube::Contains(point)) return kFALSE;
   return IsInPhiRange(point, fPhi1, fPhi2);
}

//_____________________________________________________________________________
Int_t TGeoTubeSeg::DistancetoPrimitive(Int_t px, Int_t py)
{
// compute closest distance from point px,py to each corner
   Int_t n = gGeoManager->GetNsegments()+1;
   const Int_t numPoints = 4*n;
   return ShapeDistancetoPrimitive(numPoints, px, py);
}

//_____________________________________________________________________________
Double_t TGeoTubeSeg::DistFromInsideS(Double_t *point, Double_t *dir, Double_t rmin, Double_t rmax, Double_t dz,
                                 Double_t c1, Double_t s1, Double_t c2, Double_t s2, Double_t cm, Double_t sm, Double_t cdfi)
{
// Compute distance from inside point to surface of the tube segment (static)
// Boundary safe algorithm.
   // Do Z
   Double_t stube = TGeoTube::DistFromInsideS(point,dir,rmin,rmax,dz);
   if (stube<=0) return 0.0;
   Double_t sfmin = TGeoShape::Big();
   Double_t rsq = point[0]*point[0]+point[1]*point[1];
   Double_t r = TMath::Sqrt(rsq);
   Double_t cpsi=point[0]*cm+point[1]*sm;
   if (cpsi>r*cdfi+TGeoShape::Tolerance())  {
      sfmin = TGeoShape::DistToPhiMin(point, dir, s1, c1, s2, c2, sm, cm);
      return TMath::Min(stube,sfmin);
   }
   // Point on the phi boundary or outside
   // which one: phi1 or phi2
   Double_t ddotn, xi, yi;
   if (TMath::Abs(point[1]-s1*r) < TMath::Abs(point[1]-s2*r)) {
      ddotn = s1*dir[0]-c1*dir[1];
      if (ddotn>=0) return 0.0;
      ddotn = -s2*dir[0]+c2*dir[1];
      if (ddotn<=0) return stube;
      sfmin = s2*point[0]-c2*point[1];
      if (sfmin<=0) return stube;
      sfmin /= ddotn;
      if (sfmin >= stube) return stube;
      xi = point[0]+sfmin*dir[0];
      yi = point[1]+sfmin*dir[1];
      if (yi*cm-xi*sm<0) return stube;
      return sfmin;
   }
   ddotn = -s2*dir[0]+c2*dir[1];
   if (ddotn>=0) return 0.0;
   ddotn = s1*dir[0]-c1*dir[1];
   if (ddotn<=0) return stube;
   sfmin = -s1*point[0]+c1*point[1];
   if (sfmin<=0) return stube;
   sfmin /= ddotn;
   if (sfmin >= stube) return stube;
   xi = point[0]+sfmin*dir[0];
   yi = point[1]+sfmin*dir[1];
   if (yi*cm-xi*sm>0) return stube;
   return sfmin;
}

//_____________________________________________________________________________
Double_t TGeoTubeSeg::DistFromInside(Double_t *point, Double_t *dir, Int_t iact, Double_t step, Double_t *safe) const
{
// Compute distance from inside point to surface of the tube segment
// Boundary safe algorithm.
   if (iact<3 && safe) {
      *safe = SafetyS(point, kTRUE, fRmin, fRmax, fDz, fPhi1, fPhi2);
      if (iact==0) return TGeoShape::Big();
      if ((iact==1) && (*safe>step)) return TGeoShape::Big();
   }
   Double_t phi1 = fPhi1*TMath::DegToRad();
   Double_t phi2 = fPhi2*TMath::DegToRad();
   Double_t c1 = TMath::Cos(phi1);
   Double_t c2 = TMath::Cos(phi2);
   Double_t s1 = TMath::Sin(phi1);
   Double_t s2 = TMath::Sin(phi2);
   Double_t phim = 0.5*(phi1+phi2);
   Double_t cm = TMath::Cos(phim);
   Double_t sm = TMath::Sin(phim);
   Double_t dfi = 0.5*(phi2-phi1);
   Double_t cdfi = TMath::Cos(dfi);

   // compute distance to surface
   return TGeoTubeSeg::DistFromInsideS(point,dir,fRmin,fRmax,fDz,c1,s1,c2,s2,cm,sm,cdfi);
}

//_____________________________________________________________________________
Double_t TGeoTubeSeg::DistFromOutsideS(Double_t *point, Double_t *dir, Double_t rmin, Double_t rmax,
                                Double_t dz, Double_t c1, Double_t s1, Double_t c2, Double_t s2,
                                Double_t cm, Double_t sm, Double_t cdfi)
{
// Static method to compute distance to arbitrary tube segment from outside point
// Boundary safe algorithm.
   Double_t r2, cpsi;
   // check Z planes
   Double_t xi, yi, zi;
   zi = dz - TMath::Abs(point[2]);
   Double_t rmaxsq = rmax*rmax;
   Double_t rminsq = rmin*rmin;
   Double_t s = TGeoShape::Big();
   Double_t snxt=TGeoShape::Big();
   Bool_t in = kFALSE;
   Bool_t inz = (zi<0)?kFALSE:kTRUE;
   if (!inz) {
      if (point[2]*dir[2]>=0) return TGeoShape::Big();
      s = -zi/TMath::Abs(dir[2]);
      xi = point[0]+s*dir[0];
      yi = point[1]+s*dir[1];
      r2=xi*xi+yi*yi;
      if ((rminsq<=r2) && (r2<=rmaxsq)) {
         cpsi=(xi*cm+yi*sm)/TMath::Sqrt(r2);
         if (cpsi>=cdfi) return s;
      }
   }

   // check outer cyl. surface
   Double_t rsq = point[0]*point[0]+point[1]*point[1];
   Double_t r = TMath::Sqrt(rsq);
   Double_t nsq=dir[0]*dir[0]+dir[1]*dir[1];
   Double_t rdotn=point[0]*dir[0]+point[1]*dir[1];
   Double_t b,d;
   Bool_t inrmax = kFALSE;
   Bool_t inrmin = kFALSE;
   Bool_t inphi  = kFALSE;
   if (rsq<=rmaxsq+TGeoShape::Tolerance()) inrmax = kTRUE;
   if (rsq>=rminsq-TGeoShape::Tolerance()) inrmin = kTRUE;
   cpsi=point[0]*cm+point[1]*sm;
   if (cpsi>r*cdfi-TGeoShape::Tolerance())  inphi = kTRUE;
   in = inz & inrmin & inrmax & inphi;
   // If inside, we are most likely on a boundary within machine precision.
   if (in) {
      Bool_t checkout = kFALSE;
      Double_t safphi = (cpsi-r*cdfi)*TMath::Sqrt(1.-cdfi*cdfi);
//      Double_t sch, cch;
      // check if on Z boundaries
      if (zi<rmax-r) {
         if ((rmin==0) || (zi<r-rmin)) {
            if (zi<safphi) {
               if (point[2]*dir[2]<0) return 0.0;
               return TGeoShape::Big();
            }
         }
      }
      if ((rmaxsq-rsq) < (rsq-rminsq)) checkout = kTRUE;
      // check if on Rmax boundary
      if (checkout && (rmax-r<safphi)) {
         if (rdotn>=0) return TGeoShape::Big();
         return 0.0;
      }
      if (TMath::Abs(nsq)<TGeoShape::Tolerance()) return TGeoShape::Big();
      // check if on phi boundary
      if ((rmin==0) || (safphi<r-rmin)) {
         // We may cross again a phi of rmin boundary
         // check first if we are on phi1 or phi2
         Double_t un;
         if (TMath::Abs(point[1]-s1*r) < TMath::Abs(point[1]-s2*r)) {
            un = dir[0]*s1-dir[1]*c1;
            if (un < 0) return 0.0;
            if (cdfi>=0) return TGeoShape::Big();
            un = -dir[0]*s2+dir[1]*c2;
            if (un<0) {
               s = -point[0]*s2+point[1]*c2;
               if (s>0) {
                  s /= (-un);
                  zi = point[2]+s*dir[2];
                  if (TMath::Abs(zi)<=dz) {
                     xi = point[0]+s*dir[0];
                     yi = point[1]+s*dir[1];
                     r2=xi*xi+yi*yi;
                     if ((rminsq<=r2) && (r2<=rmaxsq)) {
                        if ((yi*cm-xi*sm)>0) return s;
                     }
                  }
               }
            }
         } else {
            un = -dir[0]*s2+dir[1]*c2;
            if (un < 0) return 0.0;
            if (cdfi>=0) return TGeoShape::Big();
            un = dir[0]*s1-dir[1]*c1;
            if (un<0) {
               s = point[0]*s1-point[1]*c1;
               if (s>0) {
                  s /= (-un);
                  zi = point[2]+s*dir[2];
                  if (TMath::Abs(zi)<=dz) {
                     xi = point[0]+s*dir[0];
                     yi = point[1]+s*dir[1];
                     r2=xi*xi+yi*yi;
                     if ((rminsq<=r2) && (r2<=rmaxsq)) {
                        if ((yi*cm-xi*sm)<0) return s;
                     }
                  }
               }
            }
         }
         // We may also cross rmin, (+) solution
         if (rdotn>=0) return TGeoShape::Big();
         if (cdfi>=0) return TGeoShape::Big();
         DistToTube(rsq, nsq, rdotn, rmin, b, d);
         if (d>0) {
            s=-b+d;
            if (s>0) {
               zi=point[2]+s*dir[2];
               if (TMath::Abs(zi)<=dz) {
                  xi=point[0]+s*dir[0];
                  yi=point[1]+s*dir[1];
                  if ((xi*cm+yi*sm) >= rmin*cdfi) return s;
               }
            }
         }
         return TGeoShape::Big();
      }
      // we are on rmin boundary: we may cross again rmin or a phi facette
      if (rdotn>=0) return 0.0;
      DistToTube(rsq, nsq, rdotn, rmin, b, d);
      if (d>0) {
         s=-b+d;
         if (s>0) {
            zi=point[2]+s*dir[2];
            if (TMath::Abs(zi)<=dz) {
               // now check phi range
               xi=point[0]+s*dir[0];
               yi=point[1]+s*dir[1];
               r2=xi*xi+yi*yi;
               if ((xi*cm+yi*sm) >= rmin*cdfi) return s;
               // now we really have to check any phi crossing
               Double_t un=-dir[0]*s1+dir[1]*c1;
               if (un > 0) {
                  s=point[0]*s1-point[1]*c1;
                  if (s>=0) {
                     s /= un;
                     zi=point[2]+s*dir[2];
                     if (TMath::Abs(zi)<=dz) {
                        xi=point[0]+s*dir[0];
                        yi=point[1]+s*dir[1];
                        r2=xi*xi+yi*yi;
                        if ((rminsq<=r2) && (r2<=rmaxsq)) {
                           if ((yi*cm-xi*sm)<=0) {
                              if (s<snxt) snxt=s;
                           }
                        }
                     }
                  }
               }
               un=dir[0]*s2-dir[1]*c2;
               if (un > 0) {
                  s=(point[1]*c2-point[0]*s2)/un;
                  if (s>=0 && s<snxt) {
                     zi=point[2]+s*dir[2];
                     if (TMath::Abs(zi)<=dz) {
                        xi=point[0]+s*dir[0];
                        yi=point[1]+s*dir[1];
                        r2=xi*xi+yi*yi;
                        if ((rminsq<=r2) && (r2<=rmaxsq)) {
                           if ((yi*cm-xi*sm)>=0) {
                              return s;
                           }
                        }
                     }
                  }
               }
               return snxt;
            }
         }
      }
      return TGeoShape::Big();
   }
   // only r>rmax has to be considered
   if (TMath::Abs(nsq)<TGeoShape::Tolerance()) return TGeoShape::Big();
   if (rsq>=rmax*rmax) {
      if (rdotn>=0) return TGeoShape::Big();
      TGeoTube::DistToTube(rsq, nsq, rdotn, rmax, b, d);
      if (d>0) {
         s=-b-d;
         if (s>0) {
            zi=point[2]+s*dir[2];
            if (TMath::Abs(zi)<=dz) {
               xi=point[0]+s*dir[0];
               yi=point[1]+s*dir[1];
               cpsi = xi*cm+yi*sm;
               if (cpsi>=rmax*cdfi) return s;
            }
         }
      }
   }
   // check inner cylinder
   if (rmin>0) {
      TGeoTube::DistToTube(rsq, nsq, rdotn, rmin, b, d);
      if (d>0) {
         s=-b+d;
         if (s>0) {
            zi=point[2]+s*dir[2];
            if (TMath::Abs(zi)<=dz) {
               xi=point[0]+s*dir[0];
               yi=point[1]+s*dir[1];
               cpsi = xi*cm+yi*sm;
               if (cpsi>=rmin*cdfi) snxt=s;
            }
         }
      }
   }
   // check phi planes
   Double_t un=-dir[0]*s1+dir[1]*c1;
   if (un > 0) {
      s=point[0]*s1-point[1]*c1;
      if (s>=0) {
         s /= un;
         zi=point[2]+s*dir[2];
         if (TMath::Abs(zi)<=dz) {
            xi=point[0]+s*dir[0];
            yi=point[1]+s*dir[1];
            r2=xi*xi+yi*yi;
            if ((rminsq<=r2) && (r2<=rmaxsq)) {
               if ((yi*cm-xi*sm)<=0) {
                  if (s<snxt) snxt=s;
               }
            }
         }
      }
   }
   un=dir[0]*s2-dir[1]*c2;
   if (un > 0) {
      s=point[1]*c2-point[0]*s2;
      if (s>=0) {
         s /= un;
         zi=point[2]+s*dir[2];
         if (TMath::Abs(zi)<=dz) {
            xi=point[0]+s*dir[0];
            yi=point[1]+s*dir[1];
            r2=xi*xi+yi*yi;
            if ((rminsq<=r2) && (r2<=rmaxsq)) {
               if ((yi*cm-xi*sm)>=0) {
                  if (s<snxt) snxt=s;
               }
            }
         }
      }
   }
   return snxt;
}

//_____________________________________________________________________________
Double_t TGeoTubeSeg::DistFromOutside(Double_t *point, Double_t *dir, Int_t iact, Double_t step, Double_t *safe) const
{
// compute distance from outside point to surface of the tube segment
   // fist localize point w.r.t tube
   if (iact<3 && safe) {
      *safe = SafetyS(point, kFALSE, fRmin, fRmax, fDz, fPhi1, fPhi2);
      if (iact==0) return TGeoShape::Big();
      if ((iact==1) && (step<=*safe)) return TGeoShape::Big();
   }
   Double_t phi1 = fPhi1*TMath::DegToRad();
   Double_t phi2 = fPhi2*TMath::DegToRad();
   Double_t c1 = TMath::Cos(phi1);
   Double_t s1 = TMath::Sin(phi1);
   Double_t c2 = TMath::Cos(phi2);
   Double_t s2 = TMath::Sin(phi2);
   Double_t fio = 0.5*(phi1+phi2);
   Double_t cm = TMath::Cos(fio);
   Double_t sm = TMath::Sin(fio);
   Double_t dfi = 0.5*(phi2-phi1);
   Double_t cdfi = TMath::Cos(dfi);

   // find distance to shape
   return TGeoTubeSeg::DistFromOutsideS(point, dir, fRmin, fRmax, fDz, c1, s1, c2, s2, cm, sm, cdfi);
}

//_____________________________________________________________________________
TGeoVolume *TGeoTubeSeg::Divide(TGeoVolume *voldiv, const char *divname, Int_t iaxis, Int_t ndiv,
                             Double_t start, Double_t step)
{
//--- Divide this tube segment shape belonging to volume "voldiv" into ndiv volumes
// called divname, from start position with the given step. Returns pointer
// to created division cell volume in case of Z divisions. For radialdivision
// creates all volumes with different shapes and returns pointer to volume that
// was divided. In case a wrong division axis is supplied, returns pointer to
// volume that was divided.
   TGeoShape *shape;           //--- shape to be created
   TGeoVolume *vol;            //--- division volume to be created
   TGeoVolumeMulti *vmulti;    //--- generic divided volume
   TGeoPatternFinder *finder;  //--- finder to be attached
   TString opt = "";           //--- option to be attached
   Double_t dphi;
   Int_t id;
   Double_t end = start+ndiv*step;
   switch (iaxis) {
      case 1:  //---                 R division
         finder = new TGeoPatternCylR(voldiv, ndiv, start, end);
         vmulti = gGeoManager->MakeVolumeMulti(divname, voldiv->GetMedium());
         voldiv->SetFinder(finder);
         finder->SetDivIndex(voldiv->GetNdaughters());
         for (id=0; id<ndiv; id++) {
            shape = new TGeoTubeSeg(start+id*step, start+(id+1)*step, fDz, fPhi1, fPhi2);
            vol = new TGeoVolume(divname, shape, voldiv->GetMedium());
            vmulti->AddVolume(vol);
            opt = "R";
            voldiv->AddNodeOffset(vol, id, 0, opt.Data());
            ((TGeoNodeOffset*)voldiv->GetNodes()->At(voldiv->GetNdaughters()-1))->SetFinder(finder);
         }
         return vmulti;
      case 2:  //---                 Phi division
         dphi = fPhi2-fPhi1;
         if (dphi<0) dphi+=360.;
         if (step<=0) {step=dphi/ndiv; start=fPhi1; end=fPhi2;}
         finder = new TGeoPatternCylPhi(voldiv, ndiv, start, end);
         voldiv->SetFinder(finder);
         finder->SetDivIndex(voldiv->GetNdaughters());
         shape = new TGeoTubeSeg(fRmin, fRmax, fDz, -step/2, step/2);
         vol = new TGeoVolume(divname, shape, voldiv->GetMedium());
         vmulti = gGeoManager->MakeVolumeMulti(divname, voldiv->GetMedium());
         vmulti->AddVolume(vol);
         opt = "Phi";
         for (id=0; id<ndiv; id++) {
            voldiv->AddNodeOffset(vol, id, start+id*step+step/2, opt.Data());
            ((TGeoNodeOffset*)voldiv->GetNodes()->At(voldiv->GetNdaughters()-1))->SetFinder(finder);
         }
         return vmulti;
      case 3: //---                  Z division
         finder = new TGeoPatternZ(voldiv, ndiv, start, end);
         voldiv->SetFinder(finder);
         finder->SetDivIndex(voldiv->GetNdaughters());
         shape = new TGeoTubeSeg(fRmin, fRmax, step/2, fPhi1, fPhi2);
         vol = new TGeoVolume(divname, shape, voldiv->GetMedium());
         vmulti = gGeoManager->MakeVolumeMulti(divname, voldiv->GetMedium());
         vmulti->AddVolume(vol);
         opt = "Z";
         for (id=0; id<ndiv; id++) {
            voldiv->AddNodeOffset(vol, id, start+step/2+id*step, opt.Data());
            ((TGeoNodeOffset*)voldiv->GetNodes()->At(voldiv->GetNdaughters()-1))->SetFinder(finder);
         }
         return vmulti;
      default:
         Error("Divide", "In shape %s wrong axis type for division", GetName());
         return 0;
   }
}

//_____________________________________________________________________________
Double_t TGeoTubeSeg::GetAxisRange(Int_t iaxis, Double_t &xlo, Double_t &xhi) const
{
// Get range of shape for a given axis.
   xlo = 0;
   xhi = 0;
   Double_t dx = 0;
   switch (iaxis) {
      case 1:
         xlo = fRmin;
         xhi = fRmax;
         dx = xhi-xlo;
         return dx;
      case 2:
         xlo = fPhi1;
         xhi = fPhi2;
         dx = xhi-xlo;
         return dx;
      case 3:
         xlo = -fDz;
         xhi = fDz;
         dx = xhi-xlo;
         return dx;
   }
   return dx;
}

//_____________________________________________________________________________
void TGeoTubeSeg::GetBoundingCylinder(Double_t *param) const
{
//--- Fill vector param[4] with the bounding cylinder parameters. The order
// is the following : Rmin, Rmax, Phi1, Phi2
   param[0] = fRmin;
   param[0] *= param[0];
   param[1] = fRmax;
   param[1] *= param[1];
   param[2] = fPhi1;
   param[3] = fPhi2;
}

//_____________________________________________________________________________
TGeoShape *TGeoTubeSeg::GetMakeRuntimeShape(TGeoShape *mother, TGeoMatrix * /*mat*/) const
{
// in case shape has some negative parameters, these has to be computed
// in order to fit the mother
   if (!TestShapeBit(kGeoRunTimeShape)) return 0;
   if (!mother->TestShapeBit(kGeoTube)) {
      Error("GetMakeRuntimeShape", "Invalid mother for shape %s", GetName());
      return 0;
   }
   Double_t rmin, rmax, dz;
   rmin = fRmin;
   rmax = fRmax;
   dz = fDz;
   if (fDz<0) dz=((TGeoTube*)mother)->GetDz();
   if (fRmin<0)
      rmin = ((TGeoTube*)mother)->GetRmin();
   if ((fRmax<0) || (fRmax<=fRmin))
      rmax = ((TGeoTube*)mother)->GetRmax();

   return (new TGeoTubeSeg(GetName(),rmin, rmax, dz, fPhi1, fPhi2));
}

//_____________________________________________________________________________
void TGeoTubeSeg::InspectShape() const
{
// print shape parameters
   printf("*** Shape %s: TGeoTubeSeg ***\n", GetName());
   printf("    Rmin = %11.5f\n", fRmin);
   printf("    Rmax = %11.5f\n", fRmax);
   printf("    dz   = %11.5f\n", fDz);
   printf("    phi1 = %11.5f\n", fPhi1);
   printf("    phi2 = %11.5f\n", fPhi2);
   printf(" Bounding box:\n");
   TGeoBBox::InspectShape();
}

//_____________________________________________________________________________
TBuffer3D *TGeoTubeSeg::MakeBuffer3D() const
{
   // Creates a TBuffer3D describing *this* shape.
   // Coordinates are in local reference frame.

   Int_t n = gGeoManager->GetNsegments()+1;
   Int_t NbPnts = 4*n;
   Int_t NbSegs = 2*NbPnts;
   Int_t NbPols = NbPnts-2;

   TBuffer3D* buff = new TBuffer3D(TBuffer3DTypes::kGeneric,
                                   NbPnts, 3*NbPnts, NbSegs, 3*NbSegs, NbPols, 6*NbPols);
   if (buff)
   {
      SetPoints(buff->fPnts);
      SetSegsAndPols(*buff);
   }

   return buff;
}

//_____________________________________________________________________________
void TGeoTubeSeg::SetSegsAndPols(TBuffer3D &buff) const
{
// Fill TBuffer3D structure for segments and polygons.
   Int_t i, j;
   Int_t n = gGeoManager->GetNsegments()+1;
   Int_t c = GetBasicColor();

   memset(buff.fSegs, 0, buff.NbSegs()*3*sizeof(Int_t));
   for (i = 0; i < 4; i++) {
      for (j = 1; j < n; j++) {
         buff.fSegs[(i*n+j-1)*3  ] = c;
         buff.fSegs[(i*n+j-1)*3+1] = i*n+j-1;
         buff.fSegs[(i*n+j-1)*3+2] = i*n+j;
      }
   }
   for (i = 4; i < 6; i++) {
      for (j = 0; j < n; j++) {
         buff.fSegs[(i*n+j)*3  ] = c+1;
         buff.fSegs[(i*n+j)*3+1] = (i-4)*n+j;
         buff.fSegs[(i*n+j)*3+2] = (i-2)*n+j;
      }
   }
   for (i = 6; i < 8; i++) {
      for (j = 0; j < n; j++) {
         buff.fSegs[(i*n+j)*3  ] = c;
         buff.fSegs[(i*n+j)*3+1] = 2*(i-6)*n+j;
         buff.fSegs[(i*n+j)*3+2] = (2*(i-6)+1)*n+j;
      }
   }

   Int_t indx = 0;
   memset(buff.fPols, 0, buff.NbPols()*6*sizeof(Int_t));
   i = 0;
   for (j = 0; j < n-1; j++) {
      buff.fPols[indx++] = c;
      buff.fPols[indx++] = 4;
      buff.fPols[indx++] = (4+i)*n+j+1;
      buff.fPols[indx++] = (2+i)*n+j;
      buff.fPols[indx++] = (4+i)*n+j;
      buff.fPols[indx++] = i*n+j;
   }
   i = 1;
   for (j = 0; j < n-1; j++) {
      buff.fPols[indx++] = c;
      buff.fPols[indx++] = 4;
      buff.fPols[indx++] = i*n+j;
      buff.fPols[indx++] = (4+i)*n+j;
      buff.fPols[indx++] = (2+i)*n+j;
      buff.fPols[indx++] = (4+i)*n+j+1;
   }
   i = 2;
   for (j = 0; j < n-1; j++) {
      buff.fPols[indx++] = c+i;
      buff.fPols[indx++] = 4;
      buff.fPols[indx++] = (i-2)*2*n+j;
      buff.fPols[indx++] = (4+i)*n+j;
      buff.fPols[indx++] = ((i-2)*2+1)*n+j;
      buff.fPols[indx++] = (4+i)*n+j+1;
   }
   i = 3;
   for (j = 0; j < n-1; j++) {
      buff.fPols[indx++] = c+i;
      buff.fPols[indx++] = 4;
      buff.fPols[indx++] = (4+i)*n+j+1;
      buff.fPols[indx++] = ((i-2)*2+1)*n+j;
      buff.fPols[indx++] = (4+i)*n+j;
      buff.fPols[indx++] = (i-2)*2*n+j;
   }
   buff.fPols[indx++] = c+2;
   buff.fPols[indx++] = 4;
   buff.fPols[indx++] = 6*n;
   buff.fPols[indx++] = 4*n;
   buff.fPols[indx++] = 7*n;
   buff.fPols[indx++] = 5*n;
   buff.fPols[indx++] = c+2;
   buff.fPols[indx++] = 4;
   buff.fPols[indx++] = 6*n-1;
   buff.fPols[indx++] = 8*n-1;
   buff.fPols[indx++] = 5*n-1;
   buff.fPols[indx++] = 7*n-1;
}

//_____________________________________________________________________________
Double_t TGeoTubeSeg::Safety(Double_t *point, Bool_t in) const
{
// computes the closest distance from given point to this shape, according
// to option. The matching point on the shape is stored in spoint.
   Double_t saf[3];
   Double_t rsq = point[0]*point[0]+point[1]*point[1];
   Double_t r = TMath::Sqrt(rsq);

   Double_t safe = TGeoShape::Big();
   if (in) {
      saf[0] = fDz-TMath::Abs(point[2]);
      saf[1] = r-fRmin;
      saf[2] = fRmax-r;
      safe   = saf[TMath::LocMin(3,saf)];
   } else {
   // at least one positive
      saf[0] = TMath::Abs(point[2])-fDz;
      saf[1] = fRmin-r;
      saf[2] = r-fRmax;
      safe   = saf[TMath::LocMax(3,saf)];
   }
   Double_t safphi = TGeoShape::SafetyPhi(point,in,fPhi1,fPhi2);

   if (in) return TMath::Min(safe, safphi);
   return TMath::Max(safe, safphi);
}

//_____________________________________________________________________________
Double_t TGeoTubeSeg::SafetyS(Double_t *point, Bool_t in, Double_t rmin, Double_t rmax, Double_t dz,
                              Double_t phi1, Double_t phi2, Int_t skipz)
{
// Static method to compute the closest distance from given point to this shape.
   Double_t saf[3];
   Double_t rsq = point[0]*point[0]+point[1]*point[1];
   Double_t r = TMath::Sqrt(rsq);

   switch (skipz) {
      case 1: // skip lower Z plane
         saf[0] = dz - point[2];
         break;
      case 2: // skip upper Z plane
         saf[0] = dz + point[2];
         break;
      case 3: // skip both
         saf[0] = TGeoShape::Big();
         break;
      default:
         saf[0] = dz-TMath::Abs(point[2]);
   }
   saf[1] = r-rmin;
   saf[2] = rmax-r;
   Double_t safphi = TGeoShape::SafetyPhi(point,in,phi1,phi2);
   Double_t safe = TGeoShape::Big();

   if (in)  {
      safe = saf[TMath::LocMin(3,saf)];
      return TMath::Min(safe, safphi);
   }

   for (Int_t i=0; i<3; i++) saf[i]=-saf[i];
   safe = saf[TMath::LocMax(3,saf)];
   return TMath::Max(safe, safphi);
}

//_____________________________________________________________________________
void TGeoTubeSeg::SavePrimitive(ofstream &out, Option_t * /*option*/)
{
// Save a primitive as a C++ statement(s) on output stream "out".
   if (TObject::TestBit(kGeoSavePrimitive)) return;
   out << "   // Shape: " << GetName() << " type: " << ClassName() << endl;
   out << "   rmin = " << fRmin << ";" << endl;
   out << "   rmax = " << fRmax << ";" << endl;
   out << "   dz   = " << fDz << ";" << endl;
   out << "   phi1 = " << fPhi1 << ";" << endl;
   out << "   phi2 = " << fPhi2 << ";" << endl;
   out << "   pShape = new TGeoTubeSeg(\"" << GetName() << "\",rmin,rmax,dz,phi1,phi2);" << endl;
   TObject::SetBit(TGeoShape::kGeoSavePrimitive);
}

//_____________________________________________________________________________
void TGeoTubeSeg::SetTubsDimensions(Double_t rmin, Double_t rmax, Double_t dz,
                          Double_t phi1, Double_t phi2)
{
   fRmin = rmin;
   fRmax = rmax;
   fDz   = dz;
   fPhi1 = phi1;
   if (fPhi1 < 0) fPhi1+=360.;
   fPhi2 = phi2;
   while (fPhi2<=fPhi1) fPhi2+=360.;
   if (fPhi1==fPhi2) Error("SetTubsDimensions", "In shape %s invalid phi1=%g, phi2=%g\n", GetName(), fPhi1, fPhi2);
}

//_____________________________________________________________________________
void TGeoTubeSeg::SetDimensions(Double_t *param)
{
   Double_t rmin = param[0];
   Double_t rmax = param[1];
   Double_t dz   = param[2];
   Double_t phi1 = param[3];
   Double_t phi2 = param[4];
   SetTubsDimensions(rmin, rmax, dz, phi1, phi2);
}

//_____________________________________________________________________________
void TGeoTubeSeg::SetPoints(Double_t *points) const
{
// create sphere mesh points
    Double_t dz;
    Int_t j, n;
    Double_t phi, phi1, phi2, dphi;
    phi1 = fPhi1;
    phi2 = fPhi2;
    if (phi2<phi1) phi2+=360.;
    n = gGeoManager->GetNsegments()+1;

    dphi = (phi2-phi1)/(n-1);
    dz   = fDz;

    if (points) {
        Int_t indx = 0;

        for (j = 0; j < n; j++) {
            phi = (phi1+j*dphi)*TMath::DegToRad();
            points[indx+6*n] = points[indx] = fRmin * TMath::Cos(phi);
            indx++;
            points[indx+6*n] = points[indx] = fRmin * TMath::Sin(phi);
            indx++;
            points[indx+6*n] = dz;
            points[indx]     =-dz;
            indx++;
        }
        for (j = 0; j < n; j++) {
            phi = (phi1+j*dphi)*TMath::DegToRad();
            points[indx+6*n] = points[indx] = fRmax * TMath::Cos(phi);
            indx++;
            points[indx+6*n] = points[indx] = fRmax * TMath::Sin(phi);
            indx++;
            points[indx+6*n]= dz;
            points[indx]    =-dz;
            indx++;
        }
    }
}

//_____________________________________________________________________________
void TGeoTubeSeg::SetPoints(Float_t *points) const
{
// create sphere mesh points
    Double_t dz;
    Int_t j, n;
    Double_t phi, phi1, phi2, dphi;
    phi1 = fPhi1;
    phi2 = fPhi2;
    if (phi2<phi1) phi2+=360.;
    n = gGeoManager->GetNsegments()+1;

    dphi = (phi2-phi1)/(n-1);
    dz   = fDz;

    if (points) {
        Int_t indx = 0;

        for (j = 0; j < n; j++) {
            phi = (phi1+j*dphi)*TMath::DegToRad();
            points[indx+6*n] = points[indx] = fRmin * TMath::Cos(phi);
            indx++;
            points[indx+6*n] = points[indx] = fRmin * TMath::Sin(phi);
            indx++;
            points[indx+6*n] = dz;
            points[indx]     =-dz;
            indx++;
        }
        for (j = 0; j < n; j++) {
            phi = (phi1+j*dphi)*TMath::DegToRad();
            points[indx+6*n] = points[indx] = fRmax * TMath::Cos(phi);
            indx++;
            points[indx+6*n] = points[indx] = fRmax * TMath::Sin(phi);
            indx++;
            points[indx+6*n]= dz;
            points[indx]    =-dz;
            indx++;
        }
    }
}

//_____________________________________________________________________________
Int_t TGeoTubeSeg::GetNmeshVertices() const
{
// Return number of vertices of the mesh representation
   Int_t n = gGeoManager->GetNsegments()+1;
   Int_t numPoints = n*4;
   return numPoints;
}

//_____________________________________________________________________________
void TGeoTubeSeg::Sizeof3D() const
{
///// fill size of this 3-D object
///    TVirtualGeoPainter *painter = gGeoManager->GetGeomPainter();
///    if (!painter) return;
///
///    Int_t n = gGeoManager->GetNsegments()+1;
///    Int_t numPoints = n*4;
///    Int_t numSegs   = n*8;
///    Int_t numPolys  = n*4-2;
///
///    painter->AddSize3D(numPoints, numSegs, numPolys);
}

//_____________________________________________________________________________
const TBuffer3D & TGeoTubeSeg::GetBuffer3D(Int_t reqSections, Bool_t localFrame) const
{
   static TBuffer3DTubeSeg buffer;
   TGeoBBox::FillBuffer3D(buffer, reqSections, localFrame);

   if (reqSections & TBuffer3D::kShapeSpecific) {
      // These from TBuffer3DTube / TGeoTube
      buffer.fRadiusInner  = fRmin;
      buffer.fRadiusOuter  = fRmax;
      buffer.fHalfLength   = fDz;

      buffer.fPhiMin       = fPhi1;
      buffer.fPhiMax       = fPhi2;

      // TODO: HACK
      // At present OGL viewer wants local -> master matrix for tubes
      // even though rest works in master frame. As no one else uses
      // buffer.fLocalMaster we can overwrite it here
      // Once OGL goes to local frame this can all be removed
      // (and subclasses)
      TGeoMatrix * localMasterMat = 0;
      if (gGeoManager->IsMatrixTransform()) {
         localMasterMat = gGeoManager->GetGLMatrix();
      }
      else {
         localMasterMat = gGeoManager->GetCurrentMatrix();
      }
      localMasterMat->GetHomogenousMatrix(buffer.fLocalMaster);
      // End of HACK

      buffer.SetSectionsValid(TBuffer3D::kShapeSpecific);
   }
   if (reqSections & TBuffer3D::kRawSizes) {
      Int_t n = gGeoManager->GetNsegments()+1;
      Int_t NbPnts = 4*n;
      Int_t NbSegs = 2*NbPnts;
      Int_t NbPols = NbPnts-2;
      if (buffer.SetRawSizes(NbPnts, 3*NbPnts, NbSegs, 3*NbSegs, NbPols, 6*NbPols)) {
         buffer.SetSectionsValid(TBuffer3D::kRawSizes);
      }
   }
   if ((reqSections & TBuffer3D::kRaw) && buffer.SectionsValid(TBuffer3D::kRawSizes)) {
      SetPoints(buffer.fPnts);
      if (!buffer.fLocalFrame) {
         TransformPoints(buffer.fPnts, buffer.NbPnts());
      }
      SetSegsAndPols(buffer);
      buffer.SetSectionsValid(TBuffer3D::kRaw);
   }

   return buffer;
}

ClassImp(TGeoCtub)

TGeoCtub::TGeoCtub()
{
// default ctor
   fNlow[0] = fNlow[1] = fNhigh[0] = fNhigh[1] = 0.;
   fNlow[2] = -1;
   fNhigh[2] = 1;
}

//_____________________________________________________________________________
TGeoCtub::TGeoCtub(Double_t rmin, Double_t rmax, Double_t dz, Double_t phi1, Double_t phi2,
                   Double_t lx, Double_t ly, Double_t lz, Double_t tx, Double_t ty, Double_t tz)
         :TGeoTubeSeg(rmin, rmax, dz, phi1, phi2)
{
// ctor
   fNlow[0] = lx;
   fNlow[1] = ly;
   fNlow[2] = lz;
   fNhigh[0] = tx;
   fNhigh[1] = ty;
   fNhigh[2] = tz;
   SetShapeBit(kGeoCtub);
   ComputeBBox();
}

//_____________________________________________________________________________
TGeoCtub::TGeoCtub(const char *name, Double_t rmin, Double_t rmax, Double_t dz, Double_t phi1, Double_t phi2,
                   Double_t lx, Double_t ly, Double_t lz, Double_t tx, Double_t ty, Double_t tz)
         :TGeoTubeSeg(name, rmin, rmax, dz, phi1, phi2)
{
// ctor
   fNlow[0] = lx;
   fNlow[1] = ly;
   fNlow[2] = lz;
   fNhigh[0] = tx;
   fNhigh[1] = ty;
   fNhigh[2] = tz;
   SetShapeBit(kGeoCtub);
   ComputeBBox();
}

//_____________________________________________________________________________
TGeoCtub::TGeoCtub(Double_t *params)
         :TGeoTubeSeg(0,0,0,0,0)
{
// ctor with parameters
   SetCtubDimensions(params[0], params[1], params[2], params[3], params[4], params[5],
                     params[6], params[7], params[8], params[9], params[10]);
   SetShapeBit(kGeoCtub);
}

//_____________________________________________________________________________
TGeoCtub::~TGeoCtub()
{
// dtor
}

//_____________________________________________________________________________
void TGeoCtub::ComputeBBox()
{
// compute minimum bounding box of the ctub
   TGeoTubeSeg::ComputeBBox();
   if ((fNlow[2]>-(1E-10)) || (fNhigh[2]<1E-10)) {
      Error("ComputeBBox", "In shape %s wrong definition of cut planes", GetName());
      return;
   }
   Double_t xc=0, yc=0;
   Double_t zmin=0, zmax=0;
   Double_t z1;
   Double_t z[8];
   // check if nxy is in the phi range
   Double_t phi_low = TMath::ATan2(fNlow[1], fNlow[0]) *TMath::RadToDeg();
   Double_t phi_hi = TMath::ATan2(fNhigh[1], fNhigh[0]) *TMath::RadToDeg();
   Bool_t in_range_low = kFALSE;
   Bool_t in_range_hi = kFALSE;

   Int_t i;
   for (i=0; i<2; i++) {
      if (phi_low<0) phi_low+=360.;
      Double_t dphi = fPhi2 -fPhi1;
      if (dphi < 0) dphi+=360.;
      Double_t ddp = phi_low-fPhi1;
      if (ddp<0) ddp += 360.;
      if (ddp <= dphi) {
         xc = fRmin*TMath::Cos(phi_low*TMath::DegToRad());
         yc = fRmin*TMath::Sin(phi_low*TMath::DegToRad());
         z1 = GetZcoord(xc, yc, -fDz);
         xc = fRmax*TMath::Cos(phi_low*TMath::DegToRad());
         yc = fRmax*TMath::Sin(phi_low*TMath::DegToRad());
         z1 = TMath::Min(z1, GetZcoord(xc, yc, -fDz));
         if (in_range_low)
            zmin = TMath::Min(zmin, z1);
         else
            zmin = z1;
         in_range_low = kTRUE;
      }
      phi_low += 180;
      if (phi_low>360) phi_low-=360.;
   }

   for (i=0; i<2; i++) {
      if (phi_hi<0) phi_hi+=360.;
      Double_t dphi = fPhi2 -fPhi1;
      if (dphi < 0) dphi+=360.;
      Double_t ddp = phi_hi-fPhi1;
      if (ddp<0) ddp += 360.;
      if (ddp <= dphi) {
         xc = fRmin*TMath::Cos(phi_hi*TMath::DegToRad());
         yc = fRmin*TMath::Sin(phi_hi*TMath::DegToRad());
         z1 = GetZcoord(xc, yc, fDz);
         xc = fRmax*TMath::Cos(phi_hi*TMath::DegToRad());
         yc = fRmax*TMath::Sin(phi_hi*TMath::DegToRad());
         z1 = TMath::Max(z1, GetZcoord(xc, yc, fDz));
         if (in_range_hi)
            zmax = TMath::Max(zmax, z1);
         else
            zmax = z1;
         in_range_hi = kTRUE;
      }
      phi_hi += 180;
      if (phi_hi>360) phi_hi-=360.;
   }


   xc = fRmin*TMath::Cos(fPhi1*TMath::DegToRad());
   yc = fRmin*TMath::Sin(fPhi1*TMath::DegToRad());
   z[0] = GetZcoord(xc, yc, -fDz);
   z[4] = GetZcoord(xc, yc, fDz);

   xc = fRmin*TMath::Cos(fPhi2*TMath::DegToRad());
   yc = fRmin*TMath::Sin(fPhi2*TMath::DegToRad());
   z[1] = GetZcoord(xc, yc, -fDz);
   z[5] = GetZcoord(xc, yc, fDz);

   xc = fRmax*TMath::Cos(fPhi1*TMath::DegToRad());
   yc = fRmax*TMath::Sin(fPhi1*TMath::DegToRad());
   z[2] = GetZcoord(xc, yc, -fDz);
   z[6] = GetZcoord(xc, yc, fDz);

   xc = fRmax*TMath::Cos(fPhi2*TMath::DegToRad());
   yc = fRmax*TMath::Sin(fPhi2*TMath::DegToRad());
   z[3] = GetZcoord(xc, yc, -fDz);
   z[7] = GetZcoord(xc, yc, fDz);

   z1 = z[TMath::LocMin(4, &z[0])];
   if (in_range_low)
      zmin = TMath::Min(zmin, z1);
   else
      zmin = z1;

   z1 = z[TMath::LocMax(4, &z[4])+4];
   if (in_range_hi)
      zmax = TMath::Max(zmax, z1);
   else
      zmax = z1;

   fDZ = 0.5*(zmax-zmin);
   fOrigin[2] = 0.5*(zmax+zmin);
}

//_____________________________________________________________________________
void TGeoCtub::ComputeNormal(Double_t *point, Double_t *dir, Double_t *norm)
{
// Compute normal to closest surface from POINT.
   Double_t saf[4];
   Bool_t isseg = kTRUE;
   if (TMath::Abs(fPhi2-fPhi1-360.)<1E-8) isseg=kFALSE;
   Double_t rsq = point[0]*point[0]+point[1]*point[1];
   Double_t r = TMath::Sqrt(rsq);

   saf[0] = TMath::Abs(point[0]*fNlow[0] + point[1]*fNlow[1] + (fDz+point[2])*fNlow[2]);
   saf[1] = TMath::Abs(point[0]*fNhigh[0] + point[1]*fNhigh[1] - (fDz-point[2])*fNhigh[2]);
   saf[2] = (fRmin>1E-10)?TMath::Abs(r-fRmin):TGeoShape::Big();
   saf[3] = TMath::Abs(fRmax-r);
   Int_t i = TMath::LocMin(4,saf);
   if (isseg) {
      Double_t c1 = TMath::Cos(fPhi1*TMath::DegToRad());
      Double_t s1 = TMath::Sin(fPhi1*TMath::DegToRad());
      Double_t c2 = TMath::Cos(fPhi2*TMath::DegToRad());
      Double_t s2 = TMath::Sin(fPhi2*TMath::DegToRad());
      if (TGeoShape::IsCloseToPhi(saf[i], point,c1,s1,c2,s2)) {
         TGeoShape::NormalPhi(point,dir,norm,c1,s1,c2,s2);
         return;
      }
   }
   if (i==0) {
      memcpy(norm, fNlow, 3*sizeof(Double_t));
      if (norm[0]*dir[0]+norm[1]*dir[1]+norm[2]*dir[2]<0) {
         norm[0] = -norm[0];
         norm[1] = -norm[1];
         norm[2] = -norm[2];
      }
      return;
   }
   if (i==1) {
      memcpy(norm, fNhigh, 3*sizeof(Double_t));
      if (norm[0]*dir[0]+norm[1]*dir[1]+norm[2]*dir[2]<0) {
         norm[0] = -norm[0];
         norm[1] = -norm[1];
         norm[2] = -norm[2];
      }
      return;
   }

   norm[2] = 0;
   Double_t phi = TMath::ATan2(point[1], point[0]);
   norm[0] = TMath::Cos(phi);
   norm[1] = TMath::Sin(phi);
   if (norm[0]*dir[0]+norm[1]*dir[1]<0) {
      norm[0] = -norm[0];
      norm[1] = -norm[1];
   }
}

//_____________________________________________________________________________
Bool_t TGeoCtub::Contains(Double_t *point) const
{
// check if point is contained in the cut tube
   // check the lower cut plane
   Double_t zin = point[0]*fNlow[0]+point[1]*fNlow[1]+(point[2]+fDz)*fNlow[2];
   if (zin>0) return kFALSE;
   // check the higher cut plane
   zin = point[0]*fNhigh[0]+point[1]*fNhigh[1]+(point[2]-fDz)*fNhigh[2];
   if (zin>0) return kFALSE;
   // check radius
   Double_t r2 = point[0]*point[0]+point[1]*point[1];
   if ((r2<fRmin*fRmin) || (r2>fRmax*fRmax)) return kFALSE;
   // check phi
   Double_t phi = TMath::ATan2(point[1], point[0]) * TMath::RadToDeg();
   if (phi < 0 ) phi+=360.;
   Double_t dphi = fPhi2 -fPhi1;
   Double_t ddp = phi-fPhi1;
   if (ddp<0) ddp += 360.;
//   if (ddp>360) ddp-=360;
   if (ddp > dphi) return kFALSE;
   return kTRUE;
}

//_____________________________________________________________________________
Double_t TGeoCtub::GetAxisRange(Int_t iaxis, Double_t &xlo, Double_t &xhi) const
{
// Get range of shape for a given axis.
   xlo = 0;
   xhi = 0;
   Double_t dx = 0;
   switch (iaxis) {
      case 1:
         xlo = fRmin;
         xhi = fRmax;
         dx = xhi-xlo;
         return dx;
      case 2:
         xlo = fPhi1;
         xhi = fPhi2;
         dx = xhi-xlo;
         return dx;
   }
   return dx;
}

//_____________________________________________________________________________
Double_t TGeoCtub::GetZcoord(Double_t xc, Double_t yc, Double_t zc) const
{
// compute real Z coordinate of a point belonging to either lower or
// higher caps (z should be either +fDz or -fDz)
   Double_t newz = 0;
   if (zc<0) newz =  -fDz-(xc*fNlow[0]+yc*fNlow[1])/fNlow[2];
   else      newz = fDz-(xc*fNhigh[0]+yc*fNhigh[1])/fNhigh[2];
   return newz;
}

//_____________________________________________________________________________
Double_t TGeoCtub::DistFromOutside(Double_t *point, Double_t *dir, Int_t iact, Double_t step, Double_t *safe) const
{
// compute distance from outside point to surface of the cut tube
   if (iact<3 && safe) {
      *safe = Safety(point, kFALSE);
      if (iact==0) return TGeoShape::Big();
      if ((iact==1) && (step<=*safe)) return TGeoShape::Big();
   }
   Double_t saf[2];
   saf[0] = point[0]*fNlow[0] + point[1]*fNlow[1] + (fDz+point[2])*fNlow[2];
   saf[1] = point[0]*fNhigh[0] + point[1]*fNhigh[1] + (point[2]-fDz)*fNhigh[2];
   Double_t rsq = point[0]*point[0]+point[1]*point[1];
   Double_t r = TMath::Sqrt(rsq);
   Double_t c1=0,s1=0,c2=0,s2=0;
   Double_t fio=0, cfio=0, sfio=0, dfi=0, cdfi=0, cpsi=0;
   Double_t phi1 = fPhi1*TMath::DegToRad();
   Double_t phi2 = fPhi2*TMath::DegToRad();
   Bool_t tub = kFALSE;
   if (TMath::Abs(fPhi2-fPhi1-360.)<1E-8) tub = kTRUE;
   if (!tub) {
      c1   = TMath::Cos(phi1);
      c2   = TMath::Cos(phi2);
      s1   = TMath::Sin(phi1);
      s2   = TMath::Sin(phi2);
      fio  = 0.5*(phi1+phi2);
      cfio = TMath::Cos(fio);
      sfio = TMath::Sin(fio);
      dfi  = 0.5*(phi2-phi1);
      cdfi = TMath::Cos(dfi);
   }

   // find distance to shape
   Double_t r2;
   Double_t calf = dir[0]*fNlow[0]+dir[1]*fNlow[1]+dir[2]*fNlow[2];
   // check Z planes
   Double_t xi, yi, zi;
   Double_t s = TGeoShape::Big();
   if (saf[0]>0) {
      if (calf<0) {
         s = -saf[0]/calf;
         xi = point[0]+s*dir[0];
         yi = point[1]+s*dir[1];
         r2=xi*xi+yi*yi;
         if (((fRmin*fRmin)<=r2) && (r2<=(fRmax*fRmax))) {
            if (tub) return s;
            cpsi=(xi*cfio+yi*sfio)/TMath::Sqrt(r2);
            if (cpsi>=cdfi) return s;
         }
      }
   }
   calf = dir[0]*fNhigh[0]+dir[1]*fNhigh[1]+dir[2]*fNhigh[2];
   if (saf[1]>0) {
      if (calf<0) {
         s = -saf[1]/calf;
         xi = point[0]+s*dir[0];
         yi = point[1]+s*dir[1];
         r2=xi*xi+yi*yi;
         if (((fRmin*fRmin)<=r2) && (r2<=(fRmax*fRmax))) {
            if (tub) return s;
            cpsi=(xi*cfio+yi*sfio)/TMath::Sqrt(r2);
            if (cpsi>=cdfi) return s;
         }
      }
   }

   // check outer cyl. surface
   Double_t nsq=dir[0]*dir[0]+dir[1]*dir[1];
   if (TMath::Abs(nsq)<1E-10) return TGeoShape::Big();
   Double_t rdotn=point[0]*dir[0]+point[1]*dir[1];
   Double_t b,d;
   // only r>fRmax has to be considered
   if (r>fRmax) {
      TGeoTube::DistToTube(rsq, nsq, rdotn, fRmax, b, d);
      if (d>0) {
         s=-b-d;
         if (s>0) {
            xi=point[0]+s*dir[0];
            yi=point[1]+s*dir[1];
            zi=point[2]+s*dir[2];
            if ((-xi*fNlow[0]-yi*fNlow[1]-(zi+fDz)*fNlow[2])>0) {
               if ((-xi*fNhigh[0]-yi*fNhigh[1]+(fDz-zi)*fNhigh[2])>0) {
                  if (tub) return s;
                  cpsi=(xi*cfio+yi*sfio)/fRmax;
                  if (cpsi>=cdfi) return s;
               }
            }
         }
      }
   }
   // check inner cylinder
   Double_t snxt=TGeoShape::Big();
   if (fRmin>0) {
      TGeoTube::DistToTube(rsq, nsq, rdotn, fRmin, b, d);
      if (d>0) {
         s=-b+d;
         if (s>0) {
            xi=point[0]+s*dir[0];
            yi=point[1]+s*dir[1];
            zi=point[2]+s*dir[2];
            if ((-xi*fNlow[0]-yi*fNlow[1]-(zi+fDz)*fNlow[2])>0) {
               if ((-xi*fNhigh[0]-yi*fNhigh[1]+(fDz-zi)*fNhigh[2])>0) {
                  if (tub) return s;
                  cpsi=(xi*cfio+yi*sfio)/fRmin;
                  if (cpsi>=cdfi) snxt=s;
               }
            }
         }
      }
   }
   // check phi planes
   if (tub) return snxt;
   Double_t un=dir[0]*s1-dir[1]*c1;
   if (un != 0) {
      s=(point[1]*c1-point[0]*s1)/un;
      if (s>=0) {
         xi=point[0]+s*dir[0];
         yi=point[1]+s*dir[1];
         zi=point[2]+s*dir[2];
         if ((-xi*fNlow[0]-yi*fNlow[1]-(zi+fDz)*fNlow[2])>0) {
            if ((-xi*fNhigh[0]-yi*fNhigh[1]+(fDz-zi)*fNhigh[2])>0) {
               r2=xi*xi+yi*yi;
               if ((fRmin*fRmin<=r2) && (r2<=fRmax*fRmax)) {
                  if ((yi*cfio-xi*sfio)<=0) {
                     if (s<snxt) snxt=s;
                  }
               }
            }
         }
      }
   }
   un=dir[0]*s2-dir[1]*c2;
   if (un != 0) {
      s=(point[1]*c2-point[0]*s2)/un;
      if (s>=0) {
         xi=point[0]+s*dir[0];
         yi=point[1]+s*dir[1];
         zi=point[2]+s*dir[2];
         if ((-xi*fNlow[0]-yi*fNlow[1]-(zi+fDz)*fNlow[2])>0) {
            if ((-xi*fNhigh[0]-yi*fNhigh[1]+(fDz-zi)*fNhigh[2])>0) {
               r2=xi*xi+yi*yi;
               if ((fRmin*fRmin<=r2) && (r2<=fRmax*fRmax)) {
                  if ((yi*cfio-xi*sfio)>=0) {
                     if (s<snxt) snxt=s;
                  }
               }
            }
         }
      }
   }
   return snxt;
}

//_____________________________________________________________________________
Double_t TGeoCtub::DistFromInside(Double_t *point, Double_t *dir, Int_t iact, Double_t step, Double_t *safe) const
{
// compute distance from inside point to surface of the cut tube
   if (iact<3 && safe) *safe = Safety(point, kTRUE);
   if (iact==0) return TGeoShape::Big();
   if ((iact==1) && (*safe>step)) return TGeoShape::Big();
   Double_t rsq = point[0]*point[0]+point[1]*point[1];
   Double_t c1=0,s1=0,c2=0,s2=0,cm=0,sm=0,phim=0;
   Double_t phi1 = fPhi1*TMath::DegToRad();
   Double_t phi2 = fPhi2*TMath::DegToRad();
   Bool_t tub = kFALSE;
   if (TMath::Abs(fPhi2-fPhi1-360.)<1E-8) tub = kTRUE;
   if (!tub) {
      if (phi2<phi1) phi2+=2.*TMath::Pi();
      phim = 0.5*(phi1+phi2);
      c1 = TMath::Cos(phi1);
      c2 = TMath::Cos(phi2);
      s1 = TMath::Sin(phi1);
      s2 = TMath::Sin(phi2);
      cm = TMath::Cos(phim);
      sm = TMath::Sin(phim);
   }
   // compute distance to surface
   // Do Z
   Double_t sz = TGeoShape::Big();
   Double_t saf[2];
   saf[0] = -point[0]*fNlow[0] - point[1]*fNlow[1] - (fDz+point[2])*fNlow[2];
   saf[1] = -point[0]*fNhigh[0] - point[1]*fNhigh[1] + (fDz-point[2])*fNhigh[2];
   Double_t calf = dir[0]*fNlow[0]+dir[1]*fNlow[1]+dir[2]*fNlow[2];
   if (calf>0) sz = saf[0]/calf;

   Double_t sz1=TGeoShape::Big();
   calf = dir[0]*fNhigh[0]+dir[1]*fNhigh[1]+dir[2]*fNhigh[2];
   if (calf>0) {
      sz1 = saf[1]/calf;
      if (sz1<sz) sz = sz1;
   }

   // Do R
   Double_t nsq=dir[0]*dir[0]+dir[1]*dir[1];
   // track parallel to Z
   if (TMath::Abs(nsq)<1E-10) return sz;
   Double_t rdotn=point[0]*dir[0]+point[1]*dir[1];
   Double_t sr=TGeoShape::Big();
   Double_t b, d;
   Bool_t skip_outer = kFALSE;
   // inner cylinder
   if (fRmin>1E-10) {
      TGeoTube::DistToTube(rsq, nsq, rdotn, fRmin, b, d);
      if (d>0) {
         sr=-b-d;
         if (sr>0) skip_outer = kTRUE;
      }
   }
   // outer cylinder
   if (!skip_outer) {
      TGeoTube::DistToTube(rsq, nsq, rdotn, fRmax, b, d);
      if (d>0) {
         sr=-b+d;
         if (sr<0) sr=TGeoShape::Big();
      } else {
         Error("DistFromInside", "In shape %s cannot get outside !", GetName());
      }
   }
   // phi planes
   Double_t sfmin = TGeoShape::Big();
   if (!tub) sfmin=TGeoShape::DistToPhiMin(point, dir, s1, c1, s2, c2, sm, cm);
   return TMath::Min(TMath::Min(sz,sr), sfmin);
}

//_____________________________________________________________________________
TGeoVolume *TGeoCtub::Divide(TGeoVolume * /*voldiv*/, const char * /*divname*/, Int_t /*iaxis*/, Int_t /*ndiv*/,
                             Double_t /*start*/, Double_t /*step*/)
{
   Warning("Divide", "In shape %s division of a cut tube not implemented", GetName());
   return 0;
}

//_____________________________________________________________________________
TGeoShape *TGeoCtub::GetMakeRuntimeShape(TGeoShape *mother, TGeoMatrix * /*mat*/) const
{
// in case shape has some negative parameters, these has to be computed
// in order to fit the mother
   if (!TestShapeBit(kGeoRunTimeShape)) return 0;
   if (!mother->TestShapeBit(kGeoTube)) {
      Error("GetMakeRuntimeShape", "Invalid mother for shape %s", GetName());
      return 0;
   }
   Double_t rmin, rmax, dz;
   rmin = fRmin;
   rmax = fRmax;
   dz = fDz;
   if (fDz<0) dz=((TGeoTube*)mother)->GetDz();
   if (fRmin<0)
      rmin = ((TGeoTube*)mother)->GetRmin();
   if ((fRmax<0) || (fRmax<=fRmin))
      rmax = ((TGeoTube*)mother)->GetRmax();

   return (new TGeoCtub(rmin, rmax, dz, fPhi1, fPhi2, fNlow[0], fNlow[1], fNlow[2],
                        fNhigh[0], fNhigh[1], fNhigh[2]));
}

//_____________________________________________________________________________
void TGeoCtub::InspectShape() const
{
// print shape parameters
   printf("*** Shape %s: TGeoCtub ***\n", GetName());
   printf("    lx = %11.5f\n", fNlow[0]);
   printf("    ly = %11.5f\n", fNlow[1]);
   printf("    lz = %11.5f\n", fNlow[2]);
   printf("    tx = %11.5f\n", fNhigh[0]);
   printf("    ty = %11.5f\n", fNhigh[1]);
   printf("    tz = %11.5f\n", fNhigh[2]);
   TGeoTubeSeg::InspectShape();
}

//_____________________________________________________________________________
Double_t TGeoCtub::Safety(Double_t *point, Bool_t in) const
{
// computes the closest distance from given point to this shape, according
// to option. The matching point on the shape is stored in spoint.
   Double_t saf[4];
   Double_t rsq = point[0]*point[0]+point[1]*point[1];
   Double_t r = TMath::Sqrt(rsq);
   Bool_t isseg = kTRUE;
   if (TMath::Abs(fPhi2-fPhi1-360.)<1E-8) isseg=kFALSE;

   saf[0] = -point[0]*fNlow[0] - point[1]*fNlow[1] - (fDz+point[2])*fNlow[2];
   saf[1] = -point[0]*fNhigh[0] - point[1]*fNhigh[1] + (fDz-point[2])*fNhigh[2];
   saf[2] = (fRmin<1E-10 && !isseg)?TGeoShape::Big():(r-fRmin);
   saf[3] = fRmax-r;
   Double_t safphi = TGeoShape::Big();
   Double_t safe = TGeoShape::Big();
   if (isseg) safphi =  TGeoShape::SafetyPhi(point, in, fPhi1, fPhi2);

   if (in) {
      safe = saf[TMath::LocMin(4,saf)];
      return TMath::Min(safe, safphi);
   }
   for (Int_t i=0; i<4; i++) saf[i]=-saf[i];
   safe = saf[TMath::LocMax(5,saf)];
   if (isseg) return TMath::Max(safe, safphi);
   return safe;
}

//_____________________________________________________________________________
void TGeoCtub::SetCtubDimensions(Double_t rmin, Double_t rmax, Double_t dz, Double_t phi1, Double_t phi2,
                   Double_t lx, Double_t ly, Double_t lz, Double_t tx, Double_t ty, Double_t tz)
{
// set dimensions of a cut tube
   SetTubsDimensions(rmin, rmax, dz, phi1, phi2);
   fNlow[0] = lx;
   fNlow[1] = ly;
   fNlow[2] = lz;
   fNhigh[0] = tx;
   fNhigh[1] = ty;
   fNhigh[2] = tz;
   ComputeBBox();
}

//_____________________________________________________________________________
void TGeoCtub::SavePrimitive(ofstream &out, Option_t * /*option*/)
{
// Save a primitive as a C++ statement(s) on output stream "out".
   if (TObject::TestBit(kGeoSavePrimitive)) return;
   out << "   // Shape: " << GetName() << " type: " << ClassName() << endl;
   out << "   rmin = " << fRmin << ";" << endl;
   out << "   rmax = " << fRmax << ";" << endl;
   out << "   dz   = " << fDz << ";" << endl;
   out << "   phi1 = " << fPhi1 << ";" << endl;
   out << "   phi2 = " << fPhi2 << ";" << endl;
   out << "   lx   = " << fNlow[0] << ";" << endl;
   out << "   ly   = " << fNlow[1] << ";" << endl;
   out << "   lz   = " << fNlow[2] << ";" << endl;
   out << "   tx   = " << fNhigh[0] << ";" << endl;
   out << "   ty   = " << fNhigh[1] << ";" << endl;
   out << "   tz   = " << fNhigh[2] << ";" << endl;
   out << "   pShape = new TGeoCtub(\"" << GetName() << "\",rmin,rmax,dz,phi1,phi2,lx,ly,lz,tx,ty,tz);" << endl;   TObject::SetBit(TGeoShape::kGeoSavePrimitive);
}

//_____________________________________________________________________________
void TGeoCtub::SetDimensions(Double_t *param)
{
   SetCtubDimensions(param[0], param[1], param[2], param[3], param[4], param[5],
                     param[6], param[7], param[8], param[9], param[10]);
   ComputeBBox();
}

//_____________________________________________________________________________
void TGeoCtub::SetPoints(Double_t *points) const
{
// create sphere mesh points
    Double_t dz;
    Int_t j, n;
    Double_t phi, phi1, phi2, dphi;
    phi1 = fPhi1;
    phi2 = fPhi2;
    if (phi2<phi1) phi2+=360.;
    n = gGeoManager->GetNsegments()+1;

    dphi = (phi2-phi1)/(n-1);
    dz   = fDz;

    if (points) {
        Int_t indx = 0;

        for (j = 0; j < n; j++) {
            phi = (phi1+j*dphi)*TMath::DegToRad();
            points[indx+6*n] = points[indx] = fRmin * TMath::Cos(phi);
            indx++;
            points[indx+6*n] = points[indx] = fRmin * TMath::Sin(phi);
            indx++;
            points[indx+6*n] = GetZcoord(points[indx-2], points[indx-1], dz);
            points[indx]     = GetZcoord(points[indx-2], points[indx-1], -dz);
            indx++;
        }
        for (j = 0; j < n; j++) {
            phi = (phi1+j*dphi)*TMath::DegToRad();
            points[indx+6*n] = points[indx] = fRmax * TMath::Cos(phi);
            indx++;
            points[indx+6*n] = points[indx] = fRmax * TMath::Sin(phi);
            indx++;
            points[indx+6*n]= GetZcoord(points[indx-2], points[indx-1], dz);
            points[indx]    = GetZcoord(points[indx-2], points[indx-1], -dz);
            indx++;
        }
    }
}

//_____________________________________________________________________________
void TGeoCtub::SetPoints(Float_t *points) const
{
// create sphere mesh points
    Double_t dz;
    Int_t j, n;
    Double_t phi, phi1, phi2, dphi;
    phi1 = fPhi1;
    phi2 = fPhi2;
    if (phi2<phi1) phi2+=360.;
    n = gGeoManager->GetNsegments()+1;

    dphi = (phi2-phi1)/(n-1);
    dz   = fDz;

    if (points) {
        Int_t indx = 0;

        for (j = 0; j < n; j++) {
            phi = (phi1+j*dphi)*TMath::DegToRad();
            points[indx+6*n] = points[indx] = fRmin * TMath::Cos(phi);
            indx++;
            points[indx+6*n] = points[indx] = fRmin * TMath::Sin(phi);
            indx++;
            points[indx+6*n] = GetZcoord(points[indx-2], points[indx-1], dz);
            points[indx]     = GetZcoord(points[indx-2], points[indx-1], -dz);
            indx++;
        }
        for (j = 0; j < n; j++) {
            phi = (phi1+j*dphi)*TMath::DegToRad();
            points[indx+6*n] = points[indx] = fRmax * TMath::Cos(phi);
            indx++;
            points[indx+6*n] = points[indx] = fRmax * TMath::Sin(phi);
            indx++;
            points[indx+6*n]= GetZcoord(points[indx-2], points[indx-1], dz);
            points[indx]    = GetZcoord(points[indx-2], points[indx-1], -dz);
            indx++;
        }
    }
}

//_____________________________________________________________________________
Int_t TGeoCtub::GetNmeshVertices() const
{
// Return number of vertices of the mesh representation
   Int_t n = gGeoManager->GetNsegments()+1;
   Int_t numPoints = n*4;
   return numPoints;
}

//_____________________________________________________________________________
const TBuffer3D & TGeoCtub::GetBuffer3D(Int_t reqSections, Bool_t localFrame) const
{
   static TBuffer3DCutTube buffer;

   TGeoBBox::FillBuffer3D(buffer, reqSections, localFrame);

   if (reqSections & TBuffer3D::kShapeSpecific) {
      // These from TBuffer3DCutTube / TGeoCtub
      buffer.fRadiusInner  = fRmin;
      buffer.fRadiusOuter  = fRmax;
      buffer.fHalfLength   = fDz;
      buffer.fPhiMin       = fPhi1;
      buffer.fPhiMax       = fPhi2;

      for (UInt_t i = 0; i < 3; i++ ) {
         buffer.fLowPlaneNorm[i] = fNlow[i];
         buffer.fHighPlaneNorm[i] = fNhigh[i];
      }

      // TODO: HACK
      // At present OGL viewer wants local -> master matrix for tubes
      // even though rest works in master frame. As no one else uses
      // buffer.fLocalMaster we can overwrite it here
      // Once OGL goes to local frame this can all be removed
      // (and subclasses)
      TGeoMatrix * localMasterMat = 0;
      if (gGeoManager->IsMatrixTransform()) {
         localMasterMat = gGeoManager->GetGLMatrix();
      }
      else {
         localMasterMat = gGeoManager->GetCurrentMatrix();
      }
      localMasterMat->GetHomogenousMatrix(buffer.fLocalMaster);
      // End of HACK
   }
   if (reqSections & TBuffer3D::kRawSizes) {
      Int_t n = gGeoManager->GetNsegments()+1;
      Int_t NbPnts = 4*n;
      Int_t NbSegs = 2*NbPnts;
      Int_t NbPols = NbPnts-2;
      if (buffer.SetRawSizes(NbPnts, 3*NbPnts, NbSegs, 3*NbSegs, NbPols, 6*NbPols)) {
         buffer.SetSectionsValid(TBuffer3D::kRawSizes);
      }
   }
   if ((reqSections & TBuffer3D::kRaw) && buffer.SectionsValid(TBuffer3D::kRawSizes)) {
      SetPoints(buffer.fPnts);
      if (!buffer.fLocalFrame) {
         TransformPoints(buffer.fPnts, buffer.NbPnts());
      }
      SetSegsAndPols(buffer);
      buffer.SetSectionsValid(TBuffer3D::kRaw);
   }

   return buffer;
}
