// @(#)root/graf:$Name:  $:$Id: TEllipse.cxx,v 1.19 2005/01/13 21:39:40 brun Exp $
// Author: Rene Brun   16/10/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include <stdlib.h>

#include "Riostream.h"
#include "TROOT.h"
#include "TEllipse.h"
#include "TVirtualPad.h"
#include "TMath.h"

const Double_t kPI = 3.14159265358979323846;

ClassImp(TEllipse)

//______________________________________________________________________________
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*Ellipse class*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                          =============
//  Ellipse is a general ellipse that can be truncated and rotated.
//  An Ellipse is defined by its center (x1,y1) and two radius r1 and r2.
//  A minimum and maximum angle may be specified (phimin, phimax).
//  The Ellipse may be rotated with an angle theta.
//  The attributes of the outline line are given via TAttLine.
//  The attributes of the fill area are given via TAttFill.
//  The picture below illustrates different types of ellipses.
//
//  When an ellipse sector only is drawn, the lines connecting the center
//  of the ellipse to the edges are drawn by default. One can specify
//  the drawing option "only" to not draw these lines or alternatively
//  call the function SetNoEdges().
//
//Begin_Html
/*
<img src="gif/ellipse.gif">
*/
//End_Html
//

//______________________________________________________________________________
TEllipse::TEllipse(): TObject(), TAttLine(), TAttFill()
{
//*-*-*-*-*-*-*-*-*-*-*Ellipse default constructor*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ===========================
   fX1 = 0;
   fY1 = 0;
   fR1 = 1;
   fR2 = 1;
   fPhimin = 0;
   fPhimax = 360;
   fTheta  = 0;
}
//______________________________________________________________________________
TEllipse::TEllipse(Double_t x1, Double_t y1,Double_t r1,Double_t r2,Double_t phimin,Double_t phimax,Double_t theta)
      :TObject(), TAttLine(), TAttFill(0,1001)
{
//*-*-*-*-*-*-*-*-*-*-*Ellipse normal constructor*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ==========================
   fX1     = x1;
   fY1     = y1;
   fR1     = r1;
   fR2     = r2;
   fPhimin = phimin;
   fPhimax = phimax;
   fTheta  = theta;
   if (r2 <= 0) fR2 = fR1;
}

//______________________________________________________________________________
TEllipse::~TEllipse()
{
//*-*-*-*-*-*-*-*-*-*-*Ellipse default destructor*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ==========================
}

//______________________________________________________________________________
TEllipse::TEllipse(const TEllipse &ellipse) : TObject(ellipse), TAttLine(ellipse), TAttFill(ellipse)
{
   ((TEllipse&)ellipse).Copy(*this);
}

//______________________________________________________________________________
void TEllipse::Copy(TObject &obj) const
{
//*-*-*-*-*-*-*-*-*-*-*Copy this ellipse to ellipse*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ============================

   TObject::Copy(obj);
   TAttLine::Copy(((TEllipse&)obj));
   TAttFill::Copy(((TEllipse&)obj));
   ((TEllipse&)obj).fX1 = fX1;
   ((TEllipse&)obj).fY1 = fY1;
   ((TEllipse&)obj).fR1 = fR1;
   ((TEllipse&)obj).fR2 = fR2;
   ((TEllipse&)obj).fPhimin = fPhimin;
   ((TEllipse&)obj).fPhimax = fPhimax;
   ((TEllipse&)obj).fTheta  = fTheta;
}

//______________________________________________________________________________
Int_t TEllipse::DistancetoPrimitive(Int_t px, Int_t py)
{
//*-*-*-*-*-*-*-*-*-*-*Compute distance from point px,py to an ellipse*-*-*-*
//*-*                  ===========================================
//  Compute the closest distance of approach from point px,py to this ellipse.
//  The distance is computed in pixels units.
//

    Double_t x = gPad->PadtoX(gPad->AbsPixeltoX(px));
    Double_t y = gPad->PadtoY(gPad->AbsPixeltoY(py));

    Double_t dxnr = x - fX1;
    Double_t dynr = y - fY1;

    Double_t ct = TMath::Cos(kPI*GetTheta()/180.0);
    Double_t st = TMath::Sin(kPI*GetTheta()/180.0);

    Double_t dx =  dxnr*ct + dynr*st;
    Double_t dy = -dxnr*st + dynr*ct;

    Double_t r1 = fR1;
    Double_t r2 = fR2;

    if (dx == 0 || r1 == 0 || r2 == 0) return 9999;
    Double_t distp = TMath::Sqrt(dx*dx + dy*dy);

    Double_t tana = dy/dx;
    tana *= tana;
    Double_t distr = TMath::Sqrt((1+tana)/(1.0/(r1*r1) + tana/(r2*r2)));
    Int_t dist = 9999;
    if (GetFillColor() && GetFillStyle()) {
       if (distr > distp) dist = 0;
    } else {
       if (TMath::Abs(distr-distp)/(r1+r2) < 0.01) dist = 0;
    }
    return dist;
}

//______________________________________________________________________________
void TEllipse::Draw(Option_t *option)
{
//*-*-*-*-*-*-*-*-*-*-*Draw this ellipse with its current attributes*-*-*-*-*
//*-*                  ==========================================

   AppendPad(option);

}

//______________________________________________________________________________
void TEllipse::DrawEllipse(Double_t x1, Double_t y1,Double_t r1,Double_t r2,Double_t phimin,Double_t phimax,Double_t theta,Option_t *option)
{
//*-*-*-*-*-*-*-*-*-*-*Draw this ellipse with new coordinates*-*-*-*-*-*-*-*-*
//*-*                  ======================================
   TEllipse *newellipse = new TEllipse(x1, y1, r1, r2, phimin, phimax,theta);
   TAttLine::Copy(*newellipse);
   TAttFill::Copy(*newellipse);
   newellipse->SetBit(kCanDelete);
   newellipse->AppendPad(option);
   if (TestBit(kNoEdges)) newellipse->SetBit(kNoEdges);
}

//______________________________________________________________________________
void TEllipse::ExecuteEvent(Int_t event, Int_t px, Int_t py)
{
//*-*-*-*-*-*-*-*-*-*-*Execute action corresponding to one event*-*-*-*
//*-*                  =========================================
//  This member function is called when a line is clicked with the locator
//
//  If Left button clicked on one of the line end points, this point
//     follows the cursor until button is released.
//
//  if Middle button clicked, the line is moved parallel to itself
//     until the button is released.
//
//  NOTE that support for log scale is not implemented

   Int_t kMaxDiff = 10;
   const Int_t kMinSize = 25;
   const Int_t np = 40;
   static Int_t x[np+2], y[np+2];
   static Int_t px1,py1,npe,R1,R2,sav1,sav2;
   static Int_t pxold, pyold;
   static Int_t sig,impair;
   Int_t i, dpx, dpy;
   Double_t angle,dx,dy,dphi,ct,st,fTy,fBy,fLx,fRx;
   static Bool_t T, L, R, B, INSIDE;
   static Int_t Tx,Ty,Lx,Ly,Rx,Ry,Bx,By;

   if (!gPad->IsEditable()) return;

   switch (event) {

   case kButton1Down:
      gVirtualX->SetLineColor(-1);
      TAttLine::Modify();
      dphi = (fPhimax-fPhimin)*kPI/(180*np);
      ct   = TMath::Cos(kPI*fTheta/180);
      st   = TMath::Sin(kPI*fTheta/180);
      for (i=0;i<np;i++) {
         angle = fPhimin*kPI/180 + Double_t(i)*dphi;
         dx    = fR1*TMath::Cos(angle);
         dy    = fR2*TMath::Sin(angle);
         x[i]  = gPad->XtoAbsPixel(fX1 + dx*ct - dy*st);
         y[i]  = gPad->YtoAbsPixel(fY1 + dx*st + dy*ct);
      }
      if (fPhimax-fPhimin >= 360 ) {
         x[np] = x[0];
         y[np] = y[0];
         npe = np;
      } else {
         x[np]   = gPad->XtoAbsPixel(fX1);
         y[np]   = gPad->YtoAbsPixel(fY1);
         x[np+1] = x[0];
         y[np+1] = y[0];
         npe = np + 1;
      }
      impair = 0;
      px1 = gPad->XtoAbsPixel(fX1);
      py1 = gPad->YtoAbsPixel(fY1);
      Tx = Bx = px1;
      Ly = Ry = py1;
      Ty = gPad->YtoAbsPixel(fR2+fY1);
      By = gPad->YtoAbsPixel(-fR2+fY1);
      Lx = gPad->XtoAbsPixel(-fR1+fX1);
      Rx = gPad->XtoAbsPixel(fR1+fX1);
      R2 = (By-Ty)/2;
      R1 = (Rx-Lx)/2;
      gVirtualX->DrawLine(Rx+4, py1+4, Rx-4, py1+4);
      gVirtualX->DrawLine(Rx-4, py1+4, Rx-4, py1-4);
      gVirtualX->DrawLine(Rx-4, py1-4, Rx+4, py1-4);
      gVirtualX->DrawLine(Rx+4, py1-4, Rx+4, py1+4);
      gVirtualX->DrawLine(Lx+4, py1+4, Lx-4, py1+4);
      gVirtualX->DrawLine(Lx-4, py1+4, Lx-4, py1-4);
      gVirtualX->DrawLine(Lx-4, py1-4, Lx+4, py1-4);
      gVirtualX->DrawLine(Lx+4, py1-4, Lx+4, py1+4);
      gVirtualX->DrawLine(px1+4, By+4, px1-4, By+4);
      gVirtualX->DrawLine(px1-4, By+4, px1-4, By-4);
      gVirtualX->DrawLine(px1-4, By-4, px1+4, By-4);
      gVirtualX->DrawLine(px1+4, By-4, px1+4, By+4);
      gVirtualX->DrawLine(px1+4, Ty+4, px1-4, Ty+4);
      gVirtualX->DrawLine(px1-4, Ty+4, px1-4, Ty-4);
      gVirtualX->DrawLine(px1-4, Ty-4, px1+4, Ty-4);
      gVirtualX->DrawLine(px1+4, Ty-4, px1+4, Ty+4);
      // No break !!!

   case kMouseMotion:
      px1 = gPad->XtoAbsPixel(fX1);
      py1 = gPad->YtoAbsPixel(fY1);
      Tx = Bx = px1;
      Ly = Ry = py1;
      Ty = gPad->YtoAbsPixel(fR2+fY1);
      By = gPad->YtoAbsPixel(-fR2+fY1);
      Lx = gPad->XtoAbsPixel(-fR1+fX1);
      Rx = gPad->XtoAbsPixel(fR1+fX1);
      T = L = R = B = INSIDE = kFALSE;
      if ((TMath::Abs(px - Tx) < kMaxDiff) &&
          (TMath::Abs(py - Ty) < kMaxDiff)) {             // top edge
         T = kTRUE;
         gPad->SetCursor(kTopSide);
      }
      else
      if ((TMath::Abs(px - Bx) < kMaxDiff) &&
          (TMath::Abs(py - By) < kMaxDiff)) {             // bottom edge
         B = kTRUE;
         gPad->SetCursor(kBottomSide);
      }
      else
      if ((TMath::Abs(py - Ly) < kMaxDiff) &&
          (TMath::Abs(px - Lx) < kMaxDiff)) {             // left edge
         L = kTRUE;
         gPad->SetCursor(kLeftSide);
      }
      else
      if ((TMath::Abs(py - Ry) < kMaxDiff) &&
          (TMath::Abs(px - Rx) < kMaxDiff)) {             // right edge
         R = kTRUE;
         gPad->SetCursor(kRightSide);
      }
      else {INSIDE= kTRUE; gPad->SetCursor(kMove); }
      pxold = px;  pyold = py;

      break;

   case kButton1Motion:
      gVirtualX->DrawLine(Rx+4, py1+4, Rx-4, py1+4);
      gVirtualX->DrawLine(Rx-4, py1+4, Rx-4, py1-4);
      gVirtualX->DrawLine(Rx-4, py1-4, Rx+4, py1-4);
      gVirtualX->DrawLine(Rx+4, py1-4, Rx+4, py1+4);
      gVirtualX->DrawLine(Lx+4, py1+4, Lx-4, py1+4);
      gVirtualX->DrawLine(Lx-4, py1+4, Lx-4, py1-4);
      gVirtualX->DrawLine(Lx-4, py1-4, Lx+4, py1-4);
      gVirtualX->DrawLine(Lx+4, py1-4, Lx+4, py1+4);
      gVirtualX->DrawLine(px1+4, By+4, px1-4, By+4);
      gVirtualX->DrawLine(px1-4, By+4, px1-4, By-4);
      gVirtualX->DrawLine(px1-4, By-4, px1+4, By-4);
      gVirtualX->DrawLine(px1+4, By-4, px1+4, By+4);
      gVirtualX->DrawLine(px1+4, Ty+4, px1-4, Ty+4);
      gVirtualX->DrawLine(px1-4, Ty+4, px1-4, Ty-4);
      gVirtualX->DrawLine(px1-4, Ty-4, px1+4, Ty-4);
      gVirtualX->DrawLine(px1+4, Ty-4, px1+4, Ty+4);
      for (i=0;i<npe;i++) gVirtualX->DrawLine(x[i], y[i], x[i+1], y[i+1]);
      if (T) {
         sav1 = py1;
         sav2 = R2;
         py1 += (py - pyold)/2;
         R2 -= (py - pyold)/2;
         if (TMath::Abs(pyold-py)%2==1) impair++;
         if (py-pyold>0) sig=+1;
         else sig=-1;
         if (impair==2) { impair = 0; py1 += sig; R2 -= sig;}
         if (py1 > By-kMinSize) {py1 = sav1; R2 = sav2; py = pyold;}
      }
      if (B) {
         sav1 = py1;
         sav2 = R2;
         py1 += (py - pyold)/2;
         R2 += (py - pyold)/2;
         if (TMath::Abs(pyold-py)%2==1) impair++;
         if (py-pyold>0) sig=+1;
         else sig=-1;
         if (impair==2) { impair = 0; py1 += sig; R2 += sig;}
         if (py1 < Ty+kMinSize) {py1 = sav1; R2 = sav2; py = pyold;}
      }
      if (L) {
         sav1 = px1;
         sav2 = R1;
         px1 += (px - pxold)/2;
         R1 -= (px - pxold)/2;
         if (TMath::Abs(pxold-px)%2==1) impair++;
         if (px-pxold>0) sig=+1;
         else sig=-1;
         if (impair==2) { impair = 0; px1 += sig; R1 -= sig;}
         if (px1 > Rx-kMinSize) {px1 = sav1; R1 = sav2; px = pxold;}
      }
      if (R) {
         sav1 = px1;
         sav2 = R1;
         px1 += (px - pxold)/2;
         R1 += (px - pxold)/2;
         if (TMath::Abs(pxold-px)%2==1) impair++;
         if (px-pxold>0) sig=+1;
         else sig=-1;
         if (impair==2) { impair = 0; px1 += sig; R1 += sig;}
         if (px1 < Lx+kMinSize) {px1 = sav1; R1 = sav2; px = pxold;}
      }
      if (T || B || L || R) {
         gVirtualX->SetLineColor(-1);
         TAttLine::Modify();
         dphi = (fPhimax-fPhimin)*kPI/(180*np);
         ct   = TMath::Cos(kPI*fTheta/180);
         st   = TMath::Sin(kPI*fTheta/180);
         for (i=0;i<np;i++) {
            angle = fPhimin*kPI/180 + Double_t(i)*dphi;
            dx    = R1*TMath::Cos(angle);
            dy    = R2*TMath::Sin(angle);
            x[i]  = px1 + Int_t(dx*ct - dy*st);
            y[i]  = py1 + Int_t(dx*st + dy*ct);
         }
         if (fPhimax-fPhimin >= 360 ) {
            x[np] = x[0];
            y[np] = y[0];
            npe = np;
         } else {
            x[np]   = px1;
            y[np]   = py1;
            x[np+1] = x[0];
            y[np+1] = y[0];
            npe = np + 1;
         }
         for (i=0;i<npe;i++) gVirtualX->DrawLine(x[i], y[i], x[i+1], y[i+1]);
      }
      if (INSIDE) {
          dpx  = px-pxold;  dpy = py-pyold;
          px1 += dpx; py1 += dpy;
          for (i=0;i<=npe;i++) { x[i] += dpx; y[i] += dpy;}
          for (i=0;i<npe;i++) gVirtualX->DrawLine(x[i], y[i], x[i+1], y[i+1]);
      }
      Tx = Bx = px1;
      Rx = px1+R1;
      Lx = px1-R1;
      Ry = Ly = py1;
      Ty = py1-R2;
      By = py1+R2;
      gVirtualX->DrawLine(Rx+4, py1+4, Rx-4, py1+4);
      gVirtualX->DrawLine(Rx-4, py1+4, Rx-4, py1-4);
      gVirtualX->DrawLine(Rx-4, py1-4, Rx+4, py1-4);
      gVirtualX->DrawLine(Rx+4, py1-4, Rx+4, py1+4);
      gVirtualX->DrawLine(Lx+4, py1+4, Lx-4, py1+4);
      gVirtualX->DrawLine(Lx-4, py1+4, Lx-4, py1-4);
      gVirtualX->DrawLine(Lx-4, py1-4, Lx+4, py1-4);
      gVirtualX->DrawLine(Lx+4, py1-4, Lx+4, py1+4);
      gVirtualX->DrawLine(px1+4, By+4, px1-4, By+4);
      gVirtualX->DrawLine(px1-4, By+4, px1-4, By-4);
      gVirtualX->DrawLine(px1-4, By-4, px1+4, By-4);
      gVirtualX->DrawLine(px1+4, By-4, px1+4, By+4);
      gVirtualX->DrawLine(px1+4, Ty+4, px1-4, Ty+4);
      gVirtualX->DrawLine(px1-4, Ty+4, px1-4, Ty-4);
      gVirtualX->DrawLine(px1-4, Ty-4, px1+4, Ty-4);
      gVirtualX->DrawLine(px1+4, Ty-4, px1+4, Ty+4);
      pxold = px;
      pyold = py;
      break;

   case kButton1Up:
      fX1 = gPad->AbsPixeltoX(px1);
      fY1 = gPad->AbsPixeltoY(py1);
      fBy = gPad->AbsPixeltoY(py1+R2);
      fTy = gPad->AbsPixeltoY(py1-R2);
      fLx = gPad->AbsPixeltoX(px1+R1);
      fRx = gPad->AbsPixeltoX(px1-R1);
      fR1 = TMath::Abs(fRx-fLx)/2;
      fR2 = TMath::Abs(fTy-fBy)/2;
      gPad->Modified(kTRUE);
      gVirtualX->SetLineColor(-1);
   }
}

//______________________________________________________________________________
void TEllipse::ls(Option_t *) const
{
//*-*-*-*-*-*-*-*-*-*-*-*List this ellipse with its attributes*-*-*-*-*-*-*-*
//*-*                    =====================================
   TROOT::IndentLevel();
   printf("%s:  X1= %f Y1=%f R1=%f R2=%f\n",GetName(),fX1,fY1,fR1,fR2);
}

//______________________________________________________________________________
void TEllipse::Paint(Option_t *option)
{
//*-*-*-*-*-*-*-*-*-*-*Paint this ellipse with its current attributes*-*-*-*-*
//*-*                  ==============================================
   PaintEllipse(fX1,fY1,fR1,fR2,fPhimin,fPhimax,fTheta,option);
}

//______________________________________________________________________________
void TEllipse::PaintEllipse(Double_t, Double_t, Double_t, Double_t, Double_t phimin,Double_t phimax, Double_t theta,Option_t *option)
{
//*-*-*-*-*-*-*-*-*-*-*Draw this ellipse with new coordinates*-*-*-*-*-*-*-*-*
//*-*                  ======================================

   const Int_t np = 200;
   static Double_t x[np+3], y[np+3];
   TAttLine::Modify();  //Change line attributes only if necessary
   TAttFill::Modify();  //Change fill attributes only if necessary

   //set number of points approximatively proportional to the ellipse circumference
   Double_t circ = kPI*(fR1+fR2)*(phimax-phimin)/360;
   Int_t n = (Int_t)(np*circ/((gPad->GetX2()-gPad->GetX1())+(gPad->GetY2()-gPad->GetY1())));
   if (n < 8) n= 8;
   if (n > np) n = np;
   Double_t angle,dx,dy;
   Double_t dphi = (phimax-phimin)*kPI/(180*n);
   Double_t ct   = TMath::Cos(kPI*theta/180);
   Double_t st   = TMath::Sin(kPI*theta/180);
   for (Int_t i=0;i<=n;i++) {
      angle = phimin*kPI/180 + Double_t(i)*dphi;
      dx    = fR1*TMath::Cos(angle);
      dy    = fR2*TMath::Sin(angle);
      x[i]  = gPad->XtoPad(fX1 + dx*ct - dy*st);
      y[i]  = gPad->YtoPad(fY1 + dx*st + dy*ct);
   }
   TString opt = option;
   opt.ToLower();
   if (phimax-phimin >= 360 ) {
      if (GetFillColor()) gPad->PaintFillArea(n,x,y);
      if (GetLineStyle()) gPad->PaintPolyLine(n+1,x,y);
   } else {
      x[n+1] = gPad->XtoPad(fX1);
      y[n+1] = gPad->YtoPad(fY1);
      x[n+2] = x[0];
      y[n+2] = y[0];
      if (GetFillColor()) gPad->PaintFillArea(n+2,x,y);
      if (GetLineStyle()) {
         if (TestBit(kNoEdges) || opt.Contains("only")) gPad->PaintPolyLine(n+1,x,y);
         else                                           gPad->PaintPolyLine(n+3,x,y);
      }
   }
}


//______________________________________________________________________________
void TEllipse::Print(Option_t *) const
{
//*-*-*-*-*-*-*-*-*-*-*Dump this ellipse with its attributes*-*-*-*-*-*-*-*-*
//*-*                  =====================================

   printf("Ellipse:  X1=%f Y1=%f R1=%f R2=%f",fX1,fY1,fR1,fR2);
   if (GetLineColor() != 1) printf(" Color=%d",GetLineColor());
   if (GetLineStyle() != 1) printf(" Style=%d",GetLineStyle());
   if (GetLineWidth() != 1) printf(" Width=%d",GetLineWidth());
   printf("\n");
}

//______________________________________________________________________________
void TEllipse::SavePrimitive(ofstream &out, Option_t *)
{
    // Save primitive as a C++ statement(s) on output stream out

   out<<"   "<<endl;
   if (gROOT->ClassSaved(TEllipse::Class())) {
       out<<"   ";
   } else {
       out<<"   TEllipse *";
   }
   out<<"ellipse = new TEllipse("<<fX1<<","<<fY1<<","<<fR1<<","<<fR2
      <<","<<fPhimin<<","<<fPhimax<<","<<fTheta<<");"<<endl;

   SaveFillAttributes(out,"ellipse",0,1001);
   SaveLineAttributes(out,"ellipse",1,1,1);

   if (GetNoEdges()) out<<"   ellipse->SetNoEdges();"<<endl;

   out<<"   ellipse->Draw();"<<endl;
}

//______________________________________________________________________________
Bool_t TEllipse::GetNoEdges() const
{
   // Return kTRUE if kNoEdges bit is set, kFALSE otherwise.

   return TestBit(kNoEdges) ? kTRUE : kFALSE;
}

//______________________________________________________________________________
void TEllipse::SetNoEdges(Bool_t noEdges)
{
//   if  noEdges = kTRUE the lines connecting the center to the edges
//   will not be drawn.
//   default is to draw the edges.

   if (noEdges) SetBit(kNoEdges);
   else         ResetBit(kNoEdges);
}

//______________________________________________________________________________
void TEllipse::Streamer(TBuffer &R__b)
{
   // Stream an object of class TEllipse.

   if (R__b.IsReading()) {
      UInt_t R__s, R__c;
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c);
      if (R__v > 1) {
         TEllipse::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
         return;
      }
      //====process old versions before automatic schema evolution
      TObject::Streamer(R__b);
      TAttLine::Streamer(R__b);
      TAttFill::Streamer(R__b);
      Float_t x1,y1,r1,r2,phimin,phimax,theta;
      R__b >> x1;     fX1 = x1;
      R__b >> y1;     fY1 = y1;
      R__b >> r1;     fR1 = r1;
      R__b >> r2;     fR2 = r2;
      R__b >> phimin; fPhimin = phimin;
      R__b >> phimax; fPhimax = phimax;
      R__b >> theta;  fTheta  = theta;
      R__b.CheckByteCount(R__s, R__c, TEllipse::IsA());
      //====end of old versions

   } else {
      TEllipse::Class()->WriteBuffer(R__b,this);
   }
}
