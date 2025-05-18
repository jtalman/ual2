// @(#)root/geompainter:$Name:  $:$Id: TGeoPainter.cxx,v 1.63 2005/04/25 07:53:27 brun Exp $
// Author: Andrei Gheata   05/03/02
/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "TROOT.h"
#include "TColor.h"
#include "TPoint.h"
#include "TView.h"
#include "TAttLine.h"
#include "TAttFill.h"
#include "TPad.h"
#include "TH2F.h"

#include "TPolyMarker3D.h"
#include "TVirtualGL.h"

#include "TGeoTube.h"
#include "TGeoCone.h"
#include "TGeoSphere.h"
#include "TGeoPcon.h"
#include "TGeoTorus.h"
#include "TGeoXtru.h"
#include "TGeoVolume.h"
#include "TGeoNode.h"
#include "TGeoManager.h"
#include "TGeoTrack.h"
#include "TGeoOverlap.h"
#include "TGeoChecker.h"
#include "TGeoPhysicalNode.h"
#include "TGeoCompositeShape.h"
#include "TGeoPainter.h"

#include "X3DBuffer.h"

#include "TBuffer3D.h"
#include "TVirtualViewer3D.h"

ClassImp(TGeoPainter)

//______________________________________________________________________________
TGeoPainter::TGeoPainter(TGeoManager *manager) : TVirtualGeoPainter(manager)
{
//*-*-*-*-*-*-*-*-*-*-*Geometry painter default constructor*-*-*-*-*-*-*-*-*
//*-*                  ====================================
   printf("Painter created\n");
   TVirtualGeoPainter::SetPainter(this);
   if (manager) fGeoManager = manager;
   else {
      Error("ctor", "No geometry loaded");
      return;
   }   
   fNsegments = fGeoManager->GetNsegments();
   fNVisNodes = 0;
   fBombX = 1.3;
   fBombY = 1.3;
   fBombZ = 1.3;
   fBombR = 1.3;
   fVisLevel = fGeoManager->GetVisLevel();
   fVisOption = fGeoManager->GetVisOption();
   fExplodedView = fGeoManager->GetBombMode();
   fVisBranch = "";
   fVisLock = kFALSE;
   fIsRaytracing = kFALSE;
   fTopVisible = kFALSE;
   fPaintingOverlaps = kFALSE;
   fVisVolumes = new TObjArray();
   fOverlap = 0;
   fMatrix = 0;
   fClippingShape = 0;
   fLastVolume = 0;
   memset(&fCheckedBox[0], 0, 6*sizeof(Double_t));
   
   fCheckedNode = fGeoManager->GetTopNode();
   fChecker = new TGeoChecker(fGeoManager);
   DefineColors();
 }
//______________________________________________________________________________
TGeoPainter::~TGeoPainter()
{
//*-*-*-*-*-*-*-*-*-*-*Geometry painter default destructor*-*-*-*-*-*-*-*-*
//*-*                  ===================================
   if (fChecker) delete fChecker;
   delete fVisVolumes;
}
//______________________________________________________________________________
void TGeoPainter::AddSize3D(Int_t numpoints, Int_t numsegs, Int_t numpolys)
{
//--- Add numpoints, numsegs, numpolys to the global 3D size.
   gSize3D.numPoints += numpoints;
   gSize3D.numSegs   += numsegs;
   gSize3D.numPolys  += numpolys;
}      
//______________________________________________________________________________
TVirtualGeoTrack *TGeoPainter::AddTrack(Int_t id, Int_t pdgcode, TObject *particle)
{
// Create a primary TGeoTrack.
   return (TVirtualGeoTrack*)(new TGeoTrack(id,pdgcode,0,particle));
}

//______________________________________________________________________________
void TGeoPainter::AddTrackPoint(Double_t *point, Double_t *box, Bool_t reset) 
{
// Average center of view of all painted tracklets and compute view box.
   static Int_t npoints = 0;
   static Double_t xmin[3] = {0,0,0};
   static Double_t xmax[3] = {0,0,0};
   Int_t i;
   if (reset) {
      memset(box, 0, 6*sizeof(Double_t));
      memset(xmin, 0, 3*sizeof(Double_t));
      memset(xmax, 0, 3*sizeof(Double_t));
      npoints = 0;
      return;
   }      
   if (npoints==0) {
      for (i=0; i<3; i++) xmin[i]=xmax[i]=0;
      npoints++;
   }
   npoints++;
   Double_t  ninv = 1./Double_t(npoints); 
   for (i=0; i<3; i++) {
      box[i] += ninv*(point[i]-box[i]);
      if (point[i]<xmin[i]) xmin[i]=point[i];
      if (point[i]>xmax[i]) xmax[i]=point[i];
      box[i+3] = 0.5*(xmax[i]-xmin[i]);
   } 
}
   
//______________________________________________________________________________
void TGeoPainter::BombTranslation(const Double_t *tr, Double_t *bombtr)
{
// get the new 'bombed' translation vector according current exploded view mode
   memcpy(bombtr, tr, 3*sizeof(Double_t));
   switch (fExplodedView) {
      case kGeoNoBomb:
         return;
      case kGeoBombXYZ:
         bombtr[0] *= fBombX;
         bombtr[1] *= fBombY;
         bombtr[2] *= fBombZ;
         return;
      case kGeoBombCyl:
         bombtr[0] *= fBombR;
         bombtr[1] *= fBombR;
         bombtr[2] *= fBombZ;
         return;
      case kGeoBombSph:
         bombtr[0] *= fBombR;
         bombtr[1] *= fBombR;
         bombtr[2] *= fBombR;
         return;
      default:
         return;
   }   
}
//______________________________________________________________________________
void TGeoPainter::CheckGeometry(Int_t nrays, Double_t startx, Double_t starty, Double_t startz) const
{
   fChecker->CheckGeometry(nrays, startx, starty, startz);
}   

//______________________________________________________________________________
void TGeoPainter::CheckOverlaps(const TGeoVolume *vol, Double_t ovlp, Option_t *option) const
{
// Check overlaps for the top volume of the geometry, within a limit OVLP. 
   fChecker->CheckOverlaps(vol, ovlp, option);
}

//______________________________________________________________________________
void TGeoPainter::CheckPoint(Double_t x, Double_t y, Double_t z, Option_t *option)
{
// check current point in the geometry
   fChecker->CheckPoint(x,y,z,option);
}   

//______________________________________________________________________________
void TGeoPainter::ClearVisibleVolumes()
{
   //Clear the list of visible volumes
   //reset the kVisOnScreen bit for volumes previously in the list
   
   if (!fVisVolumes) return;
   TIter next(fVisVolumes);
   TGeoVolume *vol;
   while ((vol = (TGeoVolume*)next())) {
      vol->ResetAttBit(TGeoAtt::kVisOnScreen);
   }
   fVisVolumes->Clear();
}
      
      
//______________________________________________________________________________
void TGeoPainter::DefineColors() const
{
// Define 100 colors with increasing light intensities for each basic color (1-7)
// Register these colors at indexes starting with 300.
   TColor *color = gROOT->GetColor(300);
   if (color) return;
   Int_t i,j;
   Float_t r,g,b,h,l,s;
   
   for (i=1; i<8; i++) {
      color = (TColor*)gROOT->GetListOfColors()->At(i);
      if (!color) {
         Warning("DefineColors", "No colors defined");
         return;
      }	 
      color->GetHLS(h,l,s);
      for (j=0; j<100; j++) {
         l = 0.25+0.5*j/99.;
         TColor::HLS2RGB(h,l,s,r,g,b);
         new TColor(300+(i-1)*100+j, r,g,b);
      }
   }           
}

//______________________________________________________________________________
Int_t TGeoPainter::GetColor(Int_t base, Float_t light) const
{
// Get index of a base color with given light intensity (0,1)
   Int_t color, j;
   Int_t c = base%8;
   if (c==0) return c;
   if (light<0.25) {
      j=0;
   } else {
      if (light>0.8) j=99;
      else j = Int_t(99*(light-0.25)/0.5);
   }   
   color = 300 + (c-1)*100+j;
   return color;
}

//______________________________________________________________________________
TGeoVolume *TGeoPainter::GetDrawnVolume() const
{
// Get currently drawn volume.
   if (!gPad) return 0;
   TList *list = gPad->GetListOfPrimitives();
   Int_t size = list->GetSize();
   TObject *obj;
   for (Int_t i=0; i<size; i++) {
      obj = list->At(i);
      if (obj->InheritsFrom("TGeoVolume")) return ((TGeoVolume*)obj);
   }
   return 0;
}         
 
//______________________________________________________________________________
Int_t TGeoPainter::DistanceToPrimitiveVol(TGeoVolume *vol, Int_t px, Int_t py)
{
// compute the closest distance of approach from point px,py to a volume 
   const Int_t big = 9999;
   const Int_t inaxis = 7;
   const Int_t maxdist = 5;
   
   TGeoBBox *box;

   Int_t puxmin = gPad->XtoAbsPixel(gPad->GetUxmin());
   Int_t puymin = gPad->YtoAbsPixel(gPad->GetUymin());
   Int_t puxmax = gPad->XtoAbsPixel(gPad->GetUxmax());
   Int_t puymax = gPad->YtoAbsPixel(gPad->GetUymax());
   // return if point not in user area
   if (px < puxmin - inaxis) return big;
   if (py > puymin + inaxis) return big;
   if (px > puxmax + inaxis) return big;
   if (py < puymax - inaxis) return big;
   
   TView *view = gPad->GetView();
   if (!view) return big;
   Int_t dist = big;
   Int_t id;
   
   if (fPaintingOverlaps) {
      TGeoVolume *crt;
      if (fOverlap->IsExtrusion()) {
         crt = fOverlap->GetVolume();
         fMatrix = gGeoIdentity;
         dist = crt->GetShape()->DistancetoPrimitive(px,py);
         if (dist<maxdist) {
            gPad->SetSelected(crt);
            box = (TGeoBBox*)crt->GetShape();
            fMatrix->LocalToMaster(box->GetOrigin(), &fCheckedBox[0]);
            fCheckedBox[3] = box->GetDX();
            fCheckedBox[4] = box->GetDY();
            fCheckedBox[5] = box->GetDZ();
            return 0;
         }
      }   
      crt = fOverlap->GetNode(0)->GetVolume();
      fMatrix = fOverlap->GetNode(0)->GetMatrix();
      dist = crt->GetShape()->DistancetoPrimitive(px,py);
      if (dist<maxdist) {
         gPad->SetSelected(crt);
         box = (TGeoBBox*)crt->GetShape();
         fMatrix->LocalToMaster(box->GetOrigin(), &fCheckedBox[0]);
         fCheckedBox[3] = box->GetDX();
         fCheckedBox[4] = box->GetDY();
         fCheckedBox[5] = box->GetDZ();
         return 0;
      }
      if (fOverlap->IsExtrusion()) {
         gPad->SetSelected(view);
         return big;
      }
      crt = fOverlap->GetNode(1)->GetVolume();
      fMatrix = fOverlap->GetNode(1)->GetMatrix();
      dist = crt->GetShape()->DistancetoPrimitive(px,py);
      if (dist<maxdist) {
         gPad->SetSelected(crt);
         box = (TGeoBBox*)crt->GetShape();
         fMatrix->LocalToMaster(box->GetOrigin(), &fCheckedBox[0]);
         fCheckedBox[3] = box->GetDX();
         fCheckedBox[4] = box->GetDY();
         fCheckedBox[5] = box->GetDZ();
         return 0;
      }      
      gPad->SetSelected(view);
      return dist;
   }
   
   if (fGeoManager->GetTopVolume() == vol) fGeoManager->CdTop();
   Int_t level = fGeoManager->GetLevel();
   TGeoNode *current = fGeoManager->GetCurrentNode();
   if (vol != current->GetVolume()) return 9999;
   Bool_t vis=(current->IsVisible() && (level || (!level && fTopVisible)) && fGeoManager->IsInPhiRange())?kTRUE:kFALSE;
   TGeoNode *node = 0;
   Int_t nd = vol->GetNdaughters();
   Bool_t last = kFALSE;
   fCheckedNode = fGeoManager->GetTopNode();
   switch (fVisOption) {
      case kGeoVisDefault:
         if (vis && (level<=fVisLevel)) { 
            dist = vol->GetShape()->DistancetoPrimitive(px,py);
            if (dist<maxdist) {
               gPad->SetSelected(vol);
	             fCheckedNode = current;
	             box = (TGeoBBox*)vol->GetShape();
	             fGeoManager->LocalToMaster(box->GetOrigin(), &fCheckedBox[0]);
	             fCheckedBox[3] = box->GetDX();
	             fCheckedBox[4] = box->GetDY();
	             fCheckedBox[5] = box->GetDZ();
               return 0;
            }
         }
         // check daughters
         if (level<fVisLevel) {
            if ((!nd) || (!current->IsVisDaughters())) return dist;
            for (id=0; id<nd; id++) {
               node = vol->GetNode(id);
               fGeoManager->CdDown(id);
               dist = DistanceToPrimitiveVol(node->GetVolume(),px, py);
               if (dist==0) return 0;
               fGeoManager->CdUp();
            }
         }
         break;
      case kGeoVisLeaves:
         last = ((nd==0) || (level==fVisLevel) || (!current->IsVisDaughters()))?kTRUE:kFALSE;
         if (vis && last) {
            dist = vol->GetShape()->DistancetoPrimitive(px, py);
            if (dist<maxdist) {
               gPad->SetSelected(vol);
	             fCheckedNode = current;
	             box = (TGeoBBox*)vol->GetShape();
	             fGeoManager->LocalToMaster(box->GetOrigin(), &fCheckedBox[0]);
	             fCheckedBox[3] = box->GetDX();
	             fCheckedBox[4] = box->GetDY();
	             fCheckedBox[5] = box->GetDZ();
               return 0;
            }
         }
         if (last) return dist;
         for (id=0; id<nd; id++) {
            node = vol->GetNode(id);
            fGeoManager->CdDown(id);
            dist = DistanceToPrimitiveVol(node->GetVolume(),px,py);
            if (dist==0) return 0;
            fGeoManager->CdUp();
         }
         break;
      case kGeoVisOnly:
         dist = vol->GetShape()->DistancetoPrimitive(px, py);
         if (dist<maxdist) {
            gPad->SetSelected(vol);
            fCheckedNode = current;
	          box = (TGeoBBox*)vol->GetShape();
	          fGeoManager->LocalToMaster(box->GetOrigin(), &fCheckedBox[0]);
	          fCheckedBox[3] = box->GetDX();
	          fCheckedBox[4] = box->GetDY();
	          fCheckedBox[5] = box->GetDZ();
            return 0;
         }
         break;
      case kGeoVisBranch:
         fGeoManager->cd(fVisBranch);
         while (fGeoManager->GetLevel()) {
            if (fGeoManager->GetCurrentVolume()->IsVisible()) {
               dist = fGeoManager->GetCurrentVolume()->GetShape()->DistancetoPrimitive(px, py);
               if (dist<maxdist) {
                  gPad->SetSelected(fGeoManager->GetCurrentVolume());
  	          fCheckedNode = current;
	          box = (TGeoBBox*)fGeoManager->GetCurrentVolume()->GetShape();
	          fGeoManager->LocalToMaster(box->GetOrigin(), &fCheckedBox[0]);
	          fCheckedBox[3] = box->GetDX();
	          fCheckedBox[4] = box->GetDY();
	          fCheckedBox[5] = box->GetDZ();
                  return 0;
               }
            }   
            fGeoManager->CdUp();
         }
         gPad->SetSelected(view);
	 fCheckedNode = fGeoManager->GetTopNode();      
         return big;   
      default:
	 fCheckedNode = fGeoManager->GetTopNode();      
         return big;
   }       
   if ((dist>maxdist) && !fGeoManager->GetLevel()) gPad->SetSelected(view);
   fCheckedNode = fGeoManager->GetTopNode();      
   return dist;
}

//______________________________________________________________________________
void TGeoPainter::DefaultAngles()
{   
// Set default angles for the current view.
   if (gPad) {
      Int_t irep;
      TView *view = gPad->GetView();
      if (!view) return;
      view->SetView(-206,126,75,irep);
      gPad->Modified();
      gPad->Update();
   }
}   

//______________________________________________________________________________
void TGeoPainter::DefaultColors()
{   
// Set default volume colors according to tracking media
   TIter next(fGeoManager->GetListOfVolumes());
   TGeoVolume *vol;
   while ((vol=(TGeoVolume*)next()))
      vol->SetLineColor(vol->GetMaterial()->GetDefaultColor());
   if (gPad) {
      if (gPad->GetView()) {
         gPad->Modified();
         gPad->Update();
      }
   }
}   

//______________________________________________________________________________
Int_t TGeoPainter::CountVisibleNodes()
{
// Count total number of visible nodes.
   Int_t maxnodes = fGeoManager->GetMaxVisNodes(); 
   Int_t vislevel;
   TGeoVolume *top = fGeoManager->GetTopVolume();
   if (maxnodes <= 0) {
      vislevel = fGeoManager->GetVisLevel();
      fNVisNodes = top->CountNodes(vislevel,2);
      SetVisLevel(vislevel);
      return fNVisNodes;
   }   
   //if (the total number of nodes of the top volume is less than maxnodes
   // we can visualize everything.
   //recompute the best visibility level
   fNVisNodes = -1;
   Bool_t again = kFALSE;
   for (Int_t level = 1;level<20;level++) {
      vislevel = level;
      Int_t nnodes = top->CountNodes(level,2);
      if (nnodes > maxnodes) {
         vislevel--;
         break;
      }
      if (nnodes == fNVisNodes) {
         if (again) break;
         again = kTRUE;
      }   
      fNVisNodes = nnodes;
   }
   SetVisLevel(vislevel);
   return fNVisNodes;
}

//______________________________________________________________________________
void TGeoPainter::Draw(Option_t *option)
{
   fLastVolume = 0;
   CountVisibleNodes();         
   TString opt = option;
   opt.ToLower();
   fPaintingOverlaps = kFALSE;
   fOverlap = 0;
   if (fVisOption==kGeoVisOnly) fGeoManager->SetVisOption(kGeoVisDefault);
   
   if (fVisLock) {
      ClearVisibleVolumes();
      fVisLock = kFALSE;
   }   
   Bool_t has_pad = (gPad==0)?kFALSE:kTRUE;
   // Clear pad if option "same" not given
   if (!gPad) {
      if (!gROOT->GetMakeDefCanvas()) return;
      (gROOT->GetMakeDefCanvas())();
   }
   if (!opt.Contains("same")) gPad->Clear();
   // append this volume to pad
   fGeoManager->GetTopVolume()->AppendPad(option);

   // Create a 3-D view
   TView *view = gPad->GetView();
   if (!view) {
      view = new TView(11);
      // Set the view to perform a first autorange (frame) draw. 
      // TViewer3DPad will revert view to normal painting after this
      view->SetAutoRange(kTRUE);
      if (has_pad) gPad->Update();
   }
	
   // If we are drawing into the pad, then the view needs to be
   // set to perspective
   if (!view->IsPerspective()) view->SetPerspective();
   
   fLastVolume = fGeoManager->GetTopVolume();
 
 	// Create a 3D viewer to paint us
   gPad->GetViewer3D(option);
}

//______________________________________________________________________________
void TGeoPainter::DrawOverlap(void *ovlp, Option_t *option)
{
   TString opt = option;
   TGeoOverlap *overlap = (TGeoOverlap*)ovlp;
   if (!overlap) return;
   
   fPaintingOverlaps = kTRUE;
   fOverlap = overlap;
   opt.ToLower();
   if (fVisLock) {
      ClearVisibleVolumes();
      fVisLock = kFALSE;
   }   
   Bool_t has_pad = (gPad==0)?kFALSE:kTRUE;
   // Clear pad if option "same" not given
   if (!gPad) {
      if (!gROOT->GetMakeDefCanvas()) return;
      (gROOT->GetMakeDefCanvas())();
   }
   if (!opt.Contains("same")) gPad->Clear();
   // append this volume to pad
   overlap->AppendPad(option);

   // Create a 3-D view
 	// Create a 3D viewer to paint us
   gPad->GetViewer3D(option);
   TView *view = gPad->GetView();
   if (!view) {
      view = new TView(11);
      // Set the view to perform a first autorange (frame) draw. 
      // TViewer3DPad will revert view to normal painting after this
      view->SetAutoRange(kTRUE);
      PaintOverlap(ovlp, "range");
      overlap->GetPolyMarker()->Draw("SAME");
      if (has_pad) gPad->Update();
   }

   // If we are drawing into the pad, then the view needs to be
   // set to perspective
   if (!view->IsPerspective()) view->SetPerspective();
   fVisLock = kTRUE;
}


//______________________________________________________________________________
void TGeoPainter::DrawOnly(Option_t *option)
{
   TString opt = option;
   opt.ToLower();
   if (fVisLock) {
      ClearVisibleVolumes();
      fVisLock = kFALSE;
   }   
   fPaintingOverlaps = kFALSE;
   Bool_t has_pad = (gPad==0)?kFALSE:kTRUE;
   // Clear pad if option "same" not given
   if (!gPad) {
      if (!gROOT->GetMakeDefCanvas()) return;
      (gROOT->GetMakeDefCanvas())();
   }
   if (!opt.Contains("same")) gPad->Clear();
   // append this volume to pad
   fGeoManager->GetCurrentVolume()->AppendPad(option);

   // Create a 3-D view
   TView *view = gPad->GetView();
   if (!view) {
      view = new TView(11);
      // Set the view to perform a first autorange (frame) draw. 
      // TViewer3DPad will revert view to normal painting after this
      view->SetAutoRange(kTRUE);
      fVisOption = kGeoVisOnly;
      if (has_pad) gPad->Update();
   }

   // If we are drawing into the pad, then the view needs to be
   // set to perspective
   if (!view->IsPerspective()) view->SetPerspective();
   fVisLock = kTRUE;
}

//______________________________________________________________________________
void TGeoPainter::DrawCurrentPoint(Int_t color)
{
// Draw current point in the same view.
   if (!gPad) return;
   if (!gPad->GetView()) return;
   TPolyMarker3D *pm = new TPolyMarker3D();
   pm->SetMarkerColor(color);
   Double_t *point = fGeoManager->GetCurrentPoint();
   pm->SetNextPoint(point[0], point[1], point[2]);
   pm->SetMarkerStyle(8);
   pm->SetMarkerSize(0.5);
   pm->Draw("SAME");
}

//______________________________________________________________________________
void TGeoPainter::DrawPanel()
{
}

//______________________________________________________________________________
void TGeoPainter::DrawPath(const char *path)
{
// Draw all volumes for a given path.
   fVisOption=kGeoVisBranch;
   fVisBranch=path; 
   fGeoManager->GetTopVolume()->Draw();   
}

//______________________________________________________________________________
void TGeoPainter::EstimateCameraMove(Double_t tmin, Double_t tmax, Double_t *start, Double_t *end)
{
// Estimate camera movement between tmin and tmax for best track display
   if (!gPad) return;
   TIter next(gPad->GetListOfPrimitives());
   TVirtualGeoTrack *track;
   TObject *obj;
   Int_t ntracks = 0;
   Double_t *point = 0;
   AddTrackPoint(point, start, kTRUE);
   while ((obj=next())) {
      if (strcmp(obj->ClassName(), "TGeoTrack")) continue;
      track = (TVirtualGeoTrack*)obj;
      if (!track) continue;
      ntracks++;
      track->PaintCollect(tmin, start);
   }
   
   if (!ntracks) return;
   next.Reset();
   AddTrackPoint(point, end, kTRUE);
   while ((obj=next())) {
      if (strcmp(obj->ClassName(), "TGeoTrack")) continue;
      track = (TVirtualGeoTrack*)obj;
      if (!track) continue;
      track->PaintCollect(tmax, end);
   }   
}

//______________________________________________________________________________
void TGeoPainter::ExecuteVolumeEvent(TGeoVolume *volume, Int_t event, Int_t /*px*/, Int_t /*py*/)
{
// Execute mouse actions on a given volume.
   if (!gPad) return;
   gPad->SetCursor(kHand);
   if (fIsRaytracing) return;
   static Int_t width, color;
   switch (event) {
   case kMouseEnter:
      width = volume->GetLineWidth();
      color = volume->GetLineColor();
      volume->SetLineWidth(3);
      volume->SetLineColor(2);
      gPad->Modified();
      gPad->Update();
      break;
   
   case kMouseLeave:
      volume->SetLineWidth(width);
      volume->SetLineColor(color);
      gPad->Modified();
      gPad->Update();
      break;
   
   case kButton1Double:
      gPad->SetCursor(kWatch);
      GrabFocus();
      break;
   }
}

//______________________________________________________________________________
char *TGeoPainter::GetVolumeInfo(const TGeoVolume *volume, Int_t /*px*/, Int_t /*py*/) const
{
   const char *snull = "";
   if (!gPad) return (char*)snull;
   static char info[128];
   if (fPaintingOverlaps) {
      if (!fOverlap) {
         sprintf(info, "wrong overlapping flag");
         return info;
      }   
      TString ovtype, name;
      if (fOverlap->IsExtrusion()) {
         ovtype="EXTRUSION";
         if (volume==fOverlap->GetVolume()) name=volume->GetName();
         else name=fOverlap->GetNode(0)->GetName();
      } else {
         ovtype = "OVERLAP";
         if (volume==fOverlap->GetNode(0)->GetVolume()) name=fOverlap->GetNode(0)->GetName();
         else name=fOverlap->GetNode(1)->GetName();
      }   
      sprintf(info, "%s: %s of %g", name.Data(), ovtype.Data(), fOverlap->GetOverlap());
      return info;
   }   
   else sprintf(info,"%s, shape=%s", fGeoManager->GetPath(), volume->GetShape()->ClassName());
   return info;
}

//______________________________________________________________________________
TGeoChecker *TGeoPainter::GetChecker()
{
// Create/return geometry checker.
   if (!fChecker) fChecker = new TGeoChecker(fGeoManager);
   return fChecker;
}
 
//______________________________________________________________________________
void TGeoPainter::GetViewAngles(Double_t &longitude, Double_t &latitude, Double_t &psi) 
{
   if (!gPad) return;
   TView *view = gPad->GetView();
   if (!view) return;
   longitude = view->GetLongitude();
   latitude = view->GetLatitude();
   psi = view->GetPsi();
}   

//______________________________________________________________________________
void TGeoPainter::GrabFocus(Int_t nfr, Double_t dlong, Double_t dlat, Double_t dpsi)
{
// Move focus to current volume
   if (!gPad) return;
   TView *view = gPad->GetView();
   if (!view) return;
   if (!fCheckedNode && !fPaintingOverlaps) {
      printf("Woops!!!\n");
      TGeoBBox *box = (TGeoBBox*)fGeoManager->GetTopVolume()->GetShape();
      memcpy(&fCheckedBox[0], box->GetOrigin(), 3*sizeof(Double_t));
      fCheckedBox[3] = box->GetDX();
      fCheckedBox[4] = box->GetDY();
      fCheckedBox[5] = box->GetDZ();
   }      
   view->SetPerspective();
   Int_t nvols = fVisVolumes->GetEntriesFast();
   Int_t nframes = nfr;
   if (nfr==0) {
      nframes = 1;
      if (nvols<1500) nframes=10;
      if (nvols<1000) nframes=20;
      if (nvols<200) nframes = 50;
      if (nvols<100) nframes = 100;
   }   
   view->MoveFocus(&fCheckedBox[0], fCheckedBox[3], fCheckedBox[4], fCheckedBox[5], nframes, dlong, dlat, dpsi);
}

//______________________________________________________________________________
TH2F *TGeoPainter::LegoPlot(Int_t ntheta, Double_t themin, Double_t themax,
                            Int_t nphi,   Double_t phimin, Double_t phimax,
                            Double_t rmin, Double_t rmax, Option_t *option)
{
// Generate a lego plot fot the top volume, according to option.
   return fChecker->LegoPlot(ntheta, themin, themax, nphi, phimin, phimax, rmin, rmax, option);   
}
//______________________________________________________________________________
void TGeoPainter::LocalToMasterVect(const Double_t *local, Double_t *master) const
{
// Convert a local vector according view rotation matrix
   for (Int_t i=0; i<3; i++)
      master[i] = -local[0]*fMat[i]-local[1]*fMat[i+3]-local[2]*fMat[i+6];
}

//______________________________________________________________________________
void TGeoPainter::ModifiedPad() const
{
// Check if a pad and view are present and send signal "Modified" to pad.
   if (!gPad) return;
   TView *view = gPad->GetView();
   if (!view) return;
   view->SetViewChanged();
   gPad->Modified();
   gPad->Update();
}   

//______________________________________________________________________________
void TGeoPainter::Paint(Option_t *option)
{
// Paint current geometry according to option.
   if (!fGeoManager) return;

   if (strlen(option) || !fIsRaytracing) {
      if (fVisOption==kGeoVisOnly) {
         fGeoManager->GetCurrentNode()->Paint(option);
         return;
      }
      fGeoManager->CdTop();
      TGeoNode *top = fGeoManager->GetTopNode();
      PaintNode(top,option);
      if (fGeoManager->IsDrawingExtra()) {
         // loop the list of physical volumes
         TObjArray *nodeList = fGeoManager->GetListOfPhysicalNodes();
         Int_t nnodes = nodeList->GetEntriesFast();
         Int_t inode;
         TGeoPhysicalNode *node;
         for (inode=0; inode<nnodes; inode++) {
            node = (TGeoPhysicalNode*)nodeList->UncheckedAt(inode);
            PaintPhysicalNode(node, option);
         }
      }      
      
      // If we are drawing into the pad, then the view needs to be
      // set to perspective
      TView *view = gPad->GetView();
      if (view) {
         if (!view->IsPerspective()) view->SetPerspective();
      }
      
      fVisLock = kTRUE;
   } 
   // Check if we have to raytrace (only in pad)  
   if (!strlen(option) && fIsRaytracing) Raytrace();
}

//______________________________________________________________________________
void TGeoPainter::PaintOverlap(void *ovlp, Option_t *option)
{
// Paint an overlap.
   if (!fGeoManager) return;
   TGeoOverlap *overlap = (TGeoOverlap *)ovlp;
   if (!overlap) return;
   Int_t color, transparency;
   if (fOverlap != overlap) fOverlap = overlap;
   TGeoHMatrix *hmat = fGeoManager->GetGLMatrix();
   TGeoVolume *vol = overlap->GetVolume();
   TGeoNode *node1=0, *node2=0;
   fGeoManager->SetMatrixTransform(kTRUE);
   if (fOverlap->IsExtrusion()) {
      if (!fVisLock) fVisVolumes->Add(vol);
      *hmat = gGeoIdentity;
      fGeoManager->SetMatrixReflection(kFALSE);
      fGeoManager->SetPaintVolume(vol);
      color = vol->GetLineColor();
      transparency = vol->GetTransparency();
      vol->SetLineColor(3);
      vol->SetTransparency(49);
      if (!strstr(option,"range")) ((TAttLine*)vol)->Modify();
      PaintShape(*(vol->GetShape()),option);
      vol->SetLineColor(color);
      vol->SetTransparency(transparency);
      node1 = overlap->GetNode(0);
      fGeoManager->SetMatrixReflection(node1->GetMatrix()->IsReflection());
      *hmat = node1->GetMatrix();
      vol = node1->GetVolume();
      if (!fVisLock) fVisVolumes->Add(vol);
      fGeoManager->SetPaintVolume(vol);
      color = vol->GetLineColor();
      vol->SetLineColor(4);
      if (!strstr(option,"range")) ((TAttLine*)vol)->Modify();
      PaintShape(*(vol->GetShape()),option);
      vol->SetLineColor(color);
   } else {
      node1 = overlap->GetNode(0);
      vol = node1->GetVolume();
      *hmat = node1->GetMatrix();
      fGeoManager->SetMatrixReflection(node1->GetMatrix()->IsReflection());
      if (!fVisLock) fVisVolumes->Add(vol);
      fGeoManager->SetPaintVolume(vol);
      color = vol->GetLineColor();
      transparency = vol->GetTransparency();
      vol->SetLineColor(3);
      vol->SetTransparency(40);
      if (!strstr(option,"range")) ((TAttLine*)vol)->Modify();
      PaintShape(*(vol->GetShape()),option);
      vol->SetLineColor(color);
      vol->SetTransparency(transparency);
      node2 = overlap->GetNode(1);
      fGeoManager->SetMatrixReflection(node2->GetMatrix()->IsReflection());
      vol = node2->GetVolume();
      *hmat = node2->GetMatrix();
      if (!fVisLock) fVisVolumes->Add(vol);
      fGeoManager->SetPaintVolume(vol);
      color = vol->GetLineColor();
      transparency = vol->GetTransparency();
      vol->SetLineColor(4);
      vol->SetTransparency(40);
      if (!strstr(option,"range")) ((TAttLine*)vol)->Modify();
      PaintShape(*(vol->GetShape()),option);
      vol->SetLineColor(color);
      vol->SetTransparency(transparency);
   }     
   fGeoManager->SetMatrixTransform(kFALSE);
   fVisLock = kTRUE;
}

//______________________________________________________________________________
void TGeoPainter::PaintNode(TGeoNode *node, Option_t *option)
{
// paint recursively a node and its content accordind to visualization options
   TGeoNode *daughter = 0;
   TGeoVolume *vol = node->GetVolume();
   fGeoManager->SetPaintVolume(vol);
   TGeoHMatrix *currentMatrix = fGeoManager->GetCurrentMatrix();
   fGeoManager->SetMatrixReflection(currentMatrix->IsReflection());
   if (vol->GetShape()->IsComposite()) {
      TGeoHMatrix *glmat = fGeoManager->GetGLMatrix();
      *glmat = currentMatrix;
      fGeoManager->SetMatrixTransform(kTRUE);
   } else {
//      if (fGeoManager->GetCurrentMatrix()->IsReflection()) printf("matrix for node %s is reflection\n", node->GetName());
      fGeoManager->SetMatrixTransform(kFALSE);
   }   
// Temporary solution must go in TGeovolume ...
// TODO: Still needed?
   if (!strstr(option,"range")) {
      ((TAttLine*)vol)->Modify();  //Change line attributes only if necessary
      ((TAttFill*)vol)->Modify();  //Change fill area attributes only if necessary
   }   
//////
   Int_t nd = node->GetNdaughters();
   Bool_t last = kFALSE;
   Int_t level = fGeoManager->GetLevel();
   Bool_t vis=(node->IsVisible() && (level || (!level && fTopVisible)) && fGeoManager->IsInPhiRange())?kTRUE:kFALSE;
   Int_t id;
   Bool_t drawDaughters = kTRUE;

   switch (fVisOption) {
      case kGeoVisDefault:
         if (vis && (level<=fVisLevel)) {
            drawDaughters = PaintShape(*(vol->GetShape()),option);
            if (!fVisLock && !node->IsOnScreen()) {
               fVisVolumes->Add(vol);
               vol->SetAttBit(TGeoAtt::kVisOnScreen);
            }   
         }   
            // draw daughters
         if (drawDaughters && level<fVisLevel) {
            if ((!nd) || (!node->IsVisDaughters())) return;
            for (id=0; id<nd; id++) {
               daughter = node->GetDaughter(id);
               fGeoManager->CdDown(id);
               PaintNode(daughter, option);
               fGeoManager->CdUp();
            }
         }
         break;
      case kGeoVisLeaves:
         if (level>fVisLevel) return;
         last = ((nd==0) || (level==fVisLevel) || (!node->IsVisDaughters()))?kTRUE:kFALSE;
         if (vis && last) {
            drawDaughters = PaintShape(*(vol->GetShape()),option);
            if (!fVisLock && !node->IsOnScreen()) {
               fVisVolumes->Add(vol);
               vol->SetAttBit(TGeoAtt::kVisOnScreen);
            }   
         }            
         if (last || !drawDaughters) return;
         for (id=0; id<nd; id++) {
            daughter = node->GetDaughter(id);
            fGeoManager->CdDown(id);
            PaintNode(daughter, option);
            fGeoManager->CdUp();
         }
         break;
      case kGeoVisOnly:
         drawDaughters = PaintShape(*(vol->GetShape()),option);
         if (!fVisLock && !node->IsOnScreen()) {
            fVisVolumes->Add(vol);
            vol->SetAttBit(TGeoAtt::kVisOnScreen);
         }   
         break;
      case kGeoVisBranch:
         fGeoManager->cd(fVisBranch);
         vol = fGeoManager->GetCurrentVolume();
         while (fGeoManager->GetLevel()) {
            if (vol->IsVisible()) {
               drawDaughters = PaintShape(*(vol->GetShape()),option);
               if (!fVisLock && !fGeoManager->GetCurrentNode()->IsOnScreen()) {
                  fVisVolumes->Add(fGeoManager->GetCurrentVolume());
                  vol->SetAttBit(TGeoAtt::kVisOnScreen);
               }   
            }   
            fGeoManager->CdUp();
         }
         break;
      default:
         return;
   }
} 

//______________________________________________________________________________
Bool_t TGeoPainter::PaintShape(const TGeoShape & shape, Option_t *  option ) const
{
	// Paint the supplied shape into the current 3D viewer
   Bool_t addDaughters = kTRUE;

   TVirtualViewer3D * viewer = gPad->GetViewer3D();

   if (!viewer) {
      return addDaughters;
   }

   // For non-composite shapes we are the main paint method & perform the negotation 
   // with the viewer here
   if (shape.IsA() != TGeoCompositeShape::Class()) {
      // Does viewer prefer local frame positions?
      Bool_t localFrame = viewer->PreferLocalFrame();

      // Perform first fetch of buffer from the shape and try adding it
      // to the viewer
      const TBuffer3D & buffer = 
         shape.GetBuffer3D(TBuffer3D::kCore|TBuffer3D::kBoundingBox|TBuffer3D::kShapeSpecific, localFrame);
      Int_t reqSections = viewer->AddObject(buffer, &addDaughters);

      // If the viewer requires additional sections fetch from the shape (if possible)
      // and add again
      if (reqSections != TBuffer3D::kNone) {
         shape.GetBuffer3D(reqSections, localFrame);
         viewer->AddObject(buffer);
      }
   }
   // Composite shapes have their own internal hierarchy of shapes, each
   // of which generate a filled TBuffer3D. Therefore we can't pass up a 
   // single buffer to here. So as a special case the TGeoCompositeShape
   // performs it's own painting & negotiation with the viewer.
   else {
      const TGeoCompositeShape * composite = dynamic_cast<const TGeoCompositeShape *>(&shape);

      // We need the addDaughters flag returned from the viewer from paint
      // so can't use the normal TObject::Paint()
      addDaughters = composite->PaintComposite(option);
   }

   return addDaughters;
}

//______________________________________________________________________________
void TGeoPainter::PaintPhysicalNode(TGeoPhysicalNode *node, Option_t *option)
{
// Paints a physical node associated with a path.
   if (!node->IsVisible()) return;
   Int_t level = node->GetLevel();
   Int_t i, col, wid, sty;
   TGeoShape *shape;
   TGeoHMatrix *matrix = fGeoManager->GetGLMatrix();
   TGeoVolume *vcrt;
   fGeoManager->SetMatrixTransform(kTRUE);
   if (!node->IsVisibleFull()) {
      // Paint only last node in the branch
      vcrt  = node->GetVolume();
      shape = vcrt->GetShape();
      *matrix = node->GetMatrix();
      fGeoManager->SetMatrixReflection(matrix->IsReflection());
      fGeoManager->SetPaintVolume(vcrt);
      if (!node->IsVolAttributes() && !strstr(option,"range")) {
         col = vcrt->GetLineColor();
         wid = vcrt->GetLineWidth();
         sty = vcrt->GetLineStyle();
         vcrt->SetLineColor(node->GetLineColor());
         vcrt->SetLineWidth(node->GetLineWidth());
         vcrt->SetLineStyle(node->GetLineStyle());
         ((TAttLine*)vcrt)->Modify(); 
         shape->Paint(option);
         vcrt->SetLineColor(col);
         vcrt->SetLineWidth(wid);
         vcrt->SetLineStyle(sty);
      } else {    
         shape->Paint(option);
      }
   } else {
      // Paint full branch, except top node
      for (i=1;i<=level; i++) {
         vcrt  = node->GetVolume(i);
         shape = vcrt->GetShape();
         *matrix = node->GetMatrix(i);
         fGeoManager->SetMatrixReflection(matrix->IsReflection());
         fGeoManager->SetPaintVolume(vcrt);
         if (!node->IsVolAttributes() && !strstr(option,"range")) {
            col = vcrt->GetLineColor();
            wid = vcrt->GetLineWidth();
            sty = vcrt->GetLineStyle();
            vcrt->SetLineColor(node->GetLineColor());
            vcrt->SetLineWidth(node->GetLineWidth());
            vcrt->SetLineStyle(node->GetLineStyle());
            ((TAttLine*)vcrt)->Modify();
            shape->Paint(option);
            vcrt->SetLineColor(col);
            vcrt->SetLineWidth(wid);
            vcrt->SetLineStyle(sty);
         } else {     
            shape->Paint(option);
         }   
      }
   }      
   fGeoManager->SetMatrixTransform(kFALSE);
}   

//______________________________________________________________________________
void TGeoPainter::PrintOverlaps() const
{
   fChecker->PrintOverlaps();
}
   
//______________________________________________________________________________
void TGeoPainter::RandomPoints(const TGeoVolume *vol, Int_t npoints, Option_t *option)
{
// Draw random points in the bounding box of a volume.
   fChecker->RandomPoints((TGeoVolume*)vol, npoints, option);
}   

//______________________________________________________________________________
void TGeoPainter::RandomRays(Int_t nrays, Double_t startx, Double_t starty, Double_t startz)
{
// Shoot nrays in the current drawn geometry
   fChecker->RandomRays(nrays, startx, starty, startz);
}   

//______________________________________________________________________________
void TGeoPainter::Raytrace(Option_t * /*option*/)
{
// Raytrace current drawn geometry
   if (!gPad || gPad->IsBatch()) return;
   TView *view = gPad->GetView();
   if (!view || ! view->IsPerspective()) return;
   gVirtualX->SetMarkerSize(1);
   gVirtualX->SetMarkerStyle(1);
   Int_t i;
   Bool_t inclipst=kFALSE, inclip=kFALSE;
   Double_t krad = TMath::DegToRad();
   Double_t lat = view->GetLatitude();
   Double_t longit = view->GetLongitude();
   Double_t psi = view->GetPsi();
   Double_t c1 = TMath::Cos(psi*krad);
   Double_t s1 = TMath::Sin(psi*krad);
   Double_t c2 = TMath::Cos(lat*krad);
   Double_t s2 = TMath::Sin(lat*krad);
   Double_t s3 = TMath::Cos(longit*krad);
   Double_t c3 = -TMath::Sin(longit*krad);
   fMat[0] =  c1*c3 - s1*c2*s3;
   fMat[1] =  c1*s3 + s1*c2*c3;
   fMat[2] =  s1*s2;
      
   fMat[3] =  -s1*c3 - c1*c2*s3;
   fMat[4] = -s1*s3 + c1*c2*c3;
   fMat[5] =  c1*s2;
   
   fMat[6] =  s2*s3;
   fMat[7] =  -s2*c3;
   fMat[8] = c2; 
   Double_t u0, v0, du, dv;
   view->GetWindow(u0,v0,du,dv);
   Double_t dview = view->GetDview();
   Double_t dproj = view->GetDproj();
   Double_t local[3] = {0,0,1};
   Double_t dir[3], normal[3];   
   LocalToMasterVect(local,dir);
   Double_t min[3], max[3];
   view->GetRange(min, max);
   Double_t cov[3];
   for (i=0; i<3; i++) cov[i] = 0.5*(min[i]+max[i]);
   Double_t cop[3]; 
   for (i=0; i<3; i++) cop[i] = cov[i] - dir[i]*dview;
   fGeoManager->InitTrack(cop, dir);
   if (fClippingShape) inclipst = inclip = fClippingShape->Contains(cop);
   Int_t px, py;
   Double_t xloc, yloc, modloc;
   Int_t pxmin,pxmax, pymin,pymax;
   pxmin = gPad->UtoAbsPixel(0);
   pxmax = gPad->UtoAbsPixel(1);
   pymin = gPad->VtoAbsPixel(1);
   pymax = gPad->VtoAbsPixel(0);
   TGeoNode *next, *nextnode;
   Double_t step,steptot;
   Double_t *norm;
   Double_t *point = fGeoManager->GetCurrentPoint();
   Double_t tosource[3];
   Double_t calf;
   Double_t phi = 0*krad;
   tosource[0] = -dir[0]*TMath::Cos(phi)+dir[1]*TMath::Sin(phi);
   tosource[1] = -dir[0]*TMath::Sin(phi)-dir[1]*TMath::Cos(phi);
   tosource[2] = -dir[2];
   
   Bool_t done;
   Int_t istep;
   Int_t base_color, color;
   Double_t light;
   Double_t stemin=0, stemax=TGeoShape::Big();
   TPoint *pxy = new TPoint[1];
   TGeoVolume *nextvol;
   Int_t up;
   for (px=pxmin; px<pxmax; px++) {
      for (py=pymin; py<pymax; py++) {         
         base_color = 1;
         steptot = 0;
         inclip = inclipst;
         xloc = gPad->AbsPixeltoX(pxmin+pxmax-px);
         xloc = xloc*du-u0;
         yloc = gPad->AbsPixeltoY(pymin+pymax-py);
         yloc = yloc*dv-v0;
         modloc = TMath::Sqrt(xloc*xloc+yloc*yloc+dproj*dproj);  
         local[0] = xloc/modloc;
         local[1] = yloc/modloc;
         local[2] = dproj/modloc;
         LocalToMasterVect(local,dir);
         fGeoManager->InitTrack(cop,dir);
         // current ray pointing to pixel (px,py)
         done = kFALSE;
         norm = 0;
         // propagate to the clipping shape if any
         if (fClippingShape) {
            if (inclip) {
               stemin = fClippingShape->DistFromInside(cop,dir,3);
               stemax = TGeoShape::Big();
            } else {
               stemax = fClippingShape->DistFromOutside(cop,dir,3);
               stemin = 0;
            }
         }         
               
         while (!done) {
            if (fClippingShape) {
               if (stemin>1E10) break;
               if (stemin>0) {
                  // we are inside clipping shape
                  fGeoManager->SetStep(stemin);
                  next = fGeoManager->Step();
                  steptot = 0;
                  stemin = 0;
                  up = 0;
                  while (next) {
                     // we found something after clipping region
                     nextvol = next->GetVolume();
                     if (nextvol->TestAttBit(TGeoAtt::kVisOnScreen)) {
                        done = kTRUE;
                        base_color = nextvol->GetLineColor();
                        fClippingShape->ComputeNormal(point, dir, normal);
                        norm = normal;
                        break;
                     }
                     up++;
                     next = fGeoManager->GetMother(up);
                  }
                  if (done) break;
                  inclip = fClippingShape->Contains(point);
                  fGeoManager->SetStep(1E-3);
                  while (inclip) {
                     fGeoManager->Step();
                     inclip = fClippingShape->Contains(point);
                  }   
                  stemax = fClippingShape->DistFromOutside(point,dir,3);
               }
            }              
            nextnode = fGeoManager->FindNextBoundary();
            step = fGeoManager->GetStep();
            if (!nextnode || step>1E10) break;
            steptot += step;
            next = fGeoManager->Step();
            // Check the step
            if (fClippingShape) {
               if (steptot>stemax) {
                  steptot = 0;
                  inclip = fClippingShape->Contains(point);
                  if (inclip) {
                     stemin = fClippingShape->DistFromInside(point,dir,3);
                     stemax = TGeoShape::Big();
                     continue;
                  } else {
                     stemin = 0;
                     stemax = fClippingShape->DistFromOutside(point,dir,3);  
                  }
               }
            }      
            // Check if next node is visible
            nextvol = nextnode->GetVolume();
            if (nextvol->TestAttBit(TGeoAtt::kVisOnScreen)) {
               done = kTRUE;
               base_color = nextvol->GetLineColor();
               next = nextnode;
               break;
            }
            // Propagate and recheck the point            
            istep = 0;
            if (!fGeoManager->IsEntering()) {
               if (fGeoManager->IsOutside()) break;
               fGeoManager->SetStep(1E-3);
               printf("EXTRA STEPS\n");
            }   
            while (!fGeoManager->IsEntering()) {
               istep++;
               if (istep>1E2) break;
               steptot += 1E-3+1E-6;
               next = fGeoManager->Step();
            }
            if (istep>1E2) {
               printf("WOOPS\n");
               break; 
            }   
            if (fClippingShape) {
               if (steptot>stemax) {
                  steptot = 0;
                  inclip = fClippingShape->Contains(point);
                  if (inclip) {
                     stemin = fClippingShape->DistFromInside(point,dir,3);
                     stemax = TGeoShape::Big();
                     continue;
                  } else {
                     stemin = 0;
                     stemax = fClippingShape->DistFromOutside(point,dir,3);  
                  }
               }
            }      
            if (next) {
               nextvol = next->GetVolume();
               if (nextvol->TestAttBit(TGeoAtt::kVisOnScreen)) {
                  done = kTRUE;
                  base_color = nextvol->GetLineColor();
                  break;
               }
            }
         }
         if (!done) continue;
         // current ray intersect a visible volume having color=base_color
         if (!norm) norm = fGeoManager->FindNormal(kFALSE);
         if (!norm) continue;
         calf = norm[0]*tosource[0]+norm[1]*tosource[1]+norm[2]*tosource[2];
         light = 0.25+0.5*TMath::Abs(calf);
         color = GetColor(base_color, light);
         // Now we know the color of the pixel, just draw it
         gVirtualX->SetMarkerColor(color);         
         pxy[0].fX = px;
         pxy[0].fY = py;
         gVirtualX->DrawPolyMarker(1,pxy);
      }
   } 
   delete [] pxy;      
}

//______________________________________________________________________________
TGeoNode *TGeoPainter::SamplePoints(Int_t npoints, Double_t &dist, Double_t epsil,
                                    const char* g3path)
{
// shoot npoints randomly in a box of 1E-5 arround current point.
// return minimum distance to points outside
   return fChecker->SamplePoints(npoints, dist, epsil, g3path);
}

//______________________________________________________________________________
void TGeoPainter::SetBombFactors(Double_t bombx, Double_t bomby, Double_t bombz, Double_t bombr)
{
//--- Set cartesian and radial bomb factors for translations
   fBombX = bombx;
   fBombY = bomby;
   fBombZ = bombz;
   fBombR = bombr;
   if (IsExplodedView()) {
      if (gPad) {
         gPad->Modified();
         gPad->Update();
      }
   }
}          

//______________________________________________________________________________
void TGeoPainter::SetExplodedView(Int_t ibomb)    
{
   // set type of exploding view
   if ((ibomb<0) || (ibomb>3)) {
      Warning("SetExplodedView", "exploded view can be 0-3");
      return;
   }
   if ((Int_t)ibomb==fExplodedView) return;   
   Bool_t change = (gPad==0)?kFALSE:kTRUE;

   if (ibomb==kGeoNoBomb) {
      change &= ((fExplodedView==kGeoNoBomb)?kFALSE:kTRUE);
   }
   if (ibomb==kGeoBombXYZ) {
      change &= ((fExplodedView==kGeoBombXYZ)?kFALSE:kTRUE);
   }
   if (ibomb==kGeoBombCyl) {
      change &= ((fExplodedView==kGeoBombCyl)?kFALSE:kTRUE);
   }
   if (ibomb==kGeoBombSph) {
      change &= ((fExplodedView==kGeoBombSph)?kFALSE:kTRUE);
   }
   fExplodedView = ibomb;
   if (change && gPad->GetView()) {
      gPad->Modified();
      gPad->Update();
   }   
}

//______________________________________________________________________________
void TGeoPainter::SetNsegments(Int_t nseg)    
{
// Set number of segments to approximate circles
   if (nseg<3) {
      Warning("SetNsegments", "number of segments should be > 2");
      return;
   }
   if (fNsegments==nseg) return;
   fNsegments = nseg;
   if (!gPad) return;
   if (gPad->GetView()) {    
      gPad->Modified();
      gPad->Update();
   }
}

//______________________________________________________________________________
void TGeoPainter::SetVisLevel(Int_t level) {
// Set default level down to which visualization is performed
   if (level==fVisLevel && fLastVolume==fGeoManager->GetTopVolume()) return;
   fVisLevel=level;
   if (fVisLock) {
      ClearVisibleVolumes();
      fVisLock = kFALSE;
   }   
   if (!fLastVolume) {
      printf("--- Drawing   %6d nodes with %d visible levels\n",fNVisNodes,fVisLevel);
      return;
   }   
   if (!gPad) return;
   if (gPad->GetView()) {
      printf("--- Drawing   %6d nodes with %d visible levels\n",fNVisNodes,fVisLevel);
      gPad->Modified();
      gPad->Update();
   }
}

//______________________________________________________________________________
void TGeoPainter::SetTopVisible(Bool_t vis)
{
   if (fTopVisible==vis) return;
   fTopVisible = vis;
   if (!gPad) return;
   if (gPad->GetView()) {
      gPad->Modified();
      gPad->Update();
   }
}
   
//-----------------------------------------------------------------------------
void TGeoPainter::SetVisOption(Int_t option) {
// set drawing mode :
// option=0 (default) all nodes drawn down to vislevel
// option=1           leaves and nodes at vislevel drawn
// option=2           path is drawn
   if ((fVisOption<0) || (fVisOption>3)) {
      Warning("SetVisOption", "wrong visualization option");
      return;
   }
   if (fVisOption==option) return;   
   fVisOption=option;
   if (fVisLock) {
      ClearVisibleVolumes();
      fVisLock = kFALSE;
   }   
   if (!gPad) return;
   if (gPad->GetView()) {
      gPad->Modified();
      gPad->Update();
   }
}

//______________________________________________________________________________
Int_t TGeoPainter::ShapeDistancetoPrimitive(const TGeoShape *shape, Int_t numpoints, Int_t px, Int_t py) const   
{   
//  Returns distance between point px,py on the pad an a shape.
  Int_t dist = 9999;
   TView *view = gPad->GetView();
   if (!(numpoints && view)) return dist;
   Float_t *points = new Float_t[3*numpoints];
   shape->SetPoints(points);
   Float_t dpoint2, x1, y1, xndc[3];
   Double_t dlocal[3], dmaster[3];
   for (Int_t i=0; i<numpoints; i++) {
      dlocal[0]=points[3*i]; dlocal[1]=points[3*i+1]; dlocal[2]=points[3*i+2];
      if (fPaintingOverlaps) {
         fMatrix->LocalToMaster(&dlocal[0], &dmaster[0]);
      } else if (IsExplodedView())
         fGeoManager->LocalToMasterBomb(&dlocal[0], &dmaster[0]);
      else   
         fGeoManager->LocalToMaster(&dlocal[0], &dmaster[0]);
      points[3*i]=dmaster[0]; points[3*i+1]=dmaster[1]; points[3*i+2]=dmaster[2];
      view->WCtoNDC(&points[3*i], xndc);
      x1 = gPad->XtoAbsPixel(xndc[0]);
      y1 = gPad->YtoAbsPixel(xndc[1]);
      dpoint2 = (px-x1)*(px-x1) + (py-y1)*(py-y1);
      if (dpoint2 < dist) dist=(Int_t)dpoint2;
   }
   delete [] points;
   return Int_t(TMath::Sqrt(Double_t(dist)));
}

//______________________________________________________________________________
void TGeoPainter::Test(Int_t npoints, Option_t *option)
{
// Check time of finding "Where am I" for n points.
   fChecker->Test(npoints, option);
}   

//______________________________________________________________________________
void TGeoPainter::TestOverlaps(const char* path)
{
//--- Geometry overlap checker based on sampling. 
  fChecker->TestOverlaps(path);
}   

//______________________________________________________________________________
Bool_t TGeoPainter::TestVoxels(TGeoVolume *vol)
{
// Check voxels efficiency per volume.
   return fChecker->TestVoxels(vol);
}   

//______________________________________________________________________________
void TGeoPainter::UnbombTranslation(const Double_t *tr, Double_t *bombtr)
{
// get the new 'unbombed' translation vector according current exploded view mode
   memcpy(bombtr, tr, 3*sizeof(Double_t));
   switch (fExplodedView) {
      case kGeoNoBomb:
         return;
      case kGeoBombXYZ:
         bombtr[0] /= fBombX;
         bombtr[1] /= fBombY;
         bombtr[2] /= fBombZ;
         return;
      case kGeoBombCyl:
         bombtr[0] /= fBombR;
         bombtr[1] /= fBombR;
         bombtr[2] /= fBombZ;
         return;
      case kGeoBombSph:
         bombtr[0] /= fBombR;
         bombtr[1] /= fBombR;
         bombtr[2] /= fBombR;
         return;
      default:
         return;
   }   
}
   
//______________________________________________________________________________
Double_t TGeoPainter::Weight(Double_t precision, Option_t *option)
{
   return fChecker->Weight(precision, option);
}
   
   
