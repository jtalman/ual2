// @(#)root/graf:$Name:  $:$Id: TText.cxx,v 1.18 2005/01/13 10:24:10 brun Exp $
// Author: Nicolas Brun   12/12/94

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "Riostream.h"
#include "TROOT.h"
#include "TVirtualPad.h"
#include "TText.h"
#include "TTF.h"
#include "TVirtualX.h"
#include "TMath.h"
#include "TPoint.h"

ClassImp(TText)

//______________________________________________________________________________
//
//   TText is the base class for several text objects.
//   See TAttText for a list of text attributes or fonts,
//   and also for a discussion on text speed and font quality.
//
//  By default, the text is drawn in the pad coordinates system.
//  One can draw in NDC coordinates [0,1] if the function SetNDC
//  is called for a TText object.
//

//______________________________________________________________________________
TText::TText(): TNamed(), TAttText()
{
//*-*-*-*-*-*-*-*-*-*-*Text default constructor*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ========================
}
//______________________________________________________________________________
TText::TText(Double_t x, Double_t y, const char *text) : TNamed("",text), TAttText()
{
//*-*-*-*-*-*-*-*-*-*-*Text normal constructor*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  =======================
   fX=x; fY=y;
}

//______________________________________________________________________________
TText::~TText()
{
//*-*-*-*-*-*-*-*-*-*-*Text default destructor*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  =======================
}

//______________________________________________________________________________
TText::TText(const TText &text) : TNamed(text), TAttText(text)
{
   ((TText&)text).Copy(*this);
}

//______________________________________________________________________________
void TText::Copy(TObject &obj) const
{
//*-*-*-*-*-*-*-*-*-*-*Copy this text to text*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ======================

   ((TText&)obj).fX = fX;
   ((TText&)obj).fY = fY;
   TNamed::Copy(obj);
   TAttText::Copy(((TText&)obj));
}

//______________________________________________________________________________
Int_t TText::DistancetoPrimitive(Int_t px, Int_t py)
{
   // Compute distance from point px,py to a string.
   // The rectangle surrounding this string is evaluated.
   // If the point (px,py) is in the rectangle, the distance is set to zero.

   Int_t ptx, pty;

   TAttText::Modify();  // change text attributes only if necessary

   if (TestBit(kTextNDC)) {
      ptx = gPad->UtoPixel(fX);
      pty = gPad->VtoPixel(fY);
   } else {
      ptx = gPad->XtoAbsPixel(gPad->XtoPad(fX));
      pty = gPad->YtoAbsPixel(gPad->YtoPad(fY));
   }

   // Get the text control box
   Int_t CBoxX[5], CBoxY[5];
   GetControlBox(ptx, pty, -fTextAngle, CBoxX, CBoxY);
   CBoxY[4] = CBoxY[0];
   CBoxX[4] = CBoxX[0];

   // Check if the point (px,py) is inside the text control box
   if(TMath::IsInside(px, py, 5, CBoxX, CBoxY)){
      return 0;
   } else {
      return 9999;
   }
}

//______________________________________________________________________________
TText *TText::DrawText(Double_t x, Double_t y, const char *text)
{
//*-*-*-*-*-*-*-*-*-*-*Draw this text with new coordinates*-*-*-*-*-*-*-*-*-*
//*-*                  ===================================
   TText *newtext = new TText(x, y, text);
   TAttText::Copy(*newtext);
   newtext->SetBit(kCanDelete);
   if (TestBit(kTextNDC)) newtext->SetNDC();
   newtext->AppendPad();
   return newtext;
}

//______________________________________________________________________________
TText *TText::DrawTextNDC(Double_t x, Double_t y, const char *text)
{
//*-*-*-*-*-*-*-*-*-*-*Draw this text with new coordinates in NDC*-*-*-*-*-*
//*-*                  ==========================================
   TText *newtext = DrawText(x, y, text);
   newtext->SetNDC();
   return newtext;
}

//______________________________________________________________________________
void TText::ExecuteEvent(Int_t event, Int_t px, Int_t py)
{
//*-*-*-*-*-*-*-*-*-*-*Execute action corresponding to one event*-*-*-*
//*-*                  =========================================
//  This member function must be implemented to realize the action
//  corresponding to the mouse click on the object in the window
//

   static Int_t px1, py1, pxold, pyold, Size, hauteur, largeur;
   static Bool_t resize,turn;
   Int_t dx, dy;
   const char *text = GetTitle();
   Int_t len = strlen(text);
   Double_t sizetowin = gPad->GetAbsHNDC()*Double_t(gPad->GetWh());
   Double_t fh = (fTextSize*sizetowin);
   Int_t h     = Int_t(fh/2);
   Int_t w     = h*len;
   Short_t halign = fTextAlign/10;
   Short_t valign = fTextAlign - 10*halign;
   Double_t co, si, dtheta, norm;
   static Bool_t droite;
   static Double_t theta;
   Int_t Ax, Ay, Bx, By, Cx, Cy;
   Ax = Ay = 0;
   Double_t lambda, x2,y2;
   Double_t dpx,dpy,xp1,yp1;
   Int_t CBoxX[4], CBoxY[4], Part;

   if (!gPad->IsEditable()) return;
   switch (event) {

   case kButton1Down:
      // No break !!!

   case kMouseMotion:
      if (TestBit(kTextNDC)) {
         px1 = gPad->UtoPixel(fX);
         py1 = gPad->VtoPixel(fY);
      } else {
         px1 = gPad->XtoAbsPixel(gPad->XtoPad(fX));
         py1 = gPad->YtoAbsPixel(gPad->YtoPad(fY));
      }
      theta  = fTextAngle;
      Size   = 0;
      pxold  = px;
      pyold  = py;
      co     = TMath::Cos(fTextAngle*0.017453293);
      si     = TMath::Sin(fTextAngle*0.017453293);
      resize = kFALSE;
      turn   = kFALSE;
      GetControlBox(px1, py1, -theta, CBoxX, CBoxY);
      Part   = (Int_t)(3*((px-CBoxX[0])*co-(py-CBoxY[0])*si)/
                      ((CBoxX[3]-CBoxX[0])*co-(CBoxY[3]-CBoxY[0])*si));
      switch (Part) {
      case 0:
	 if (halign == 3) {
            turn   = kTRUE;
	    droite = kTRUE;
            gPad->SetCursor(kRotate);
         } else {
            resize  = kTRUE;
            hauteur = valign;
            largeur = halign;
            gPad->SetCursor(kArrowVer);
	 }
         break;
      case 1:
         gPad->SetCursor(kMove);
         break;
      case 2:
	 if (halign == 3) {
            resize  = kTRUE;
            hauteur = valign;
            largeur = halign;
            gPad->SetCursor(kArrowVer);
         } else {
            turn   = kTRUE;
	    droite = kFALSE;
            gPad->SetCursor(kRotate);
	 }
      }
      break;

   case kButton1Motion:
      PaintControlBox(px1, py1, -theta);
      if (turn) {
         norm = TMath::Sqrt(Double_t((py-py1)*(py-py1)+(px-px1)*(px-px1)));
         if (norm>0) {
            theta = TMath::ACos((px-px1)/norm);
            dtheta= TMath::ASin((py1-py)/norm);
            if (dtheta<0) theta = -theta;
            theta = theta/TMath::ACos(-1)*180;
            if (theta<0) theta += 360;
            if (droite) {theta = theta+180; if (theta>=360) theta -= 360;}
         }
      }
      else if (resize) {

         co = TMath::Cos(fTextAngle*0.017453293);
         si = TMath::Sin(fTextAngle*0.017453293);
         if (largeur == 1) {
            switch (valign) {
               case 1 : Ax = px1; Ay = py1; break;
               case 2 : Ax = px1+Int_t(si*h/2); Ay = py1+Int_t(co*h/2); break;
               case 3 : Ax = px1+Int_t(si*h*3/2); Ay = py1+Int_t(co*h*3/2); break;
            }
         }
         if (largeur == 2) {
            switch (valign) {
               case 1 : Ax = px1-Int_t(co*w/2); Ay = py1+Int_t(si*w/2); break;
               case 2 : Ax = px1-Int_t(co*w/2+si*h/2); Ay = py1+Int_t(si*w/2+co*h/2); break;
               case 3 : Ax = px1-Int_t(co*w/2+si*h*3/2); Ay = py1+Int_t(si*w/2+co*h*3/2); break;
            }
         }
         if (largeur == 3) {
            switch (valign) {
               case 1 : Ax = px1-Int_t(co*w); Ay = py1+Int_t(si*w); break;
               case 2 : Ax = px1-Int_t(co*w+si*h/2); Ay = py1+Int_t(si*w+co*h/2); break;
               case 3 : Ax = px1-Int_t(co*w+si*h*3/2); Ay = py1+Int_t(si*w+co*h*3/2); break;
            }
         }
         if (hauteur == 3) {Bx = Ax-Int_t(si*h); By = Ay-Int_t(co*h);}
         else {Bx = Ax; By = Ay;}
         Cx = Bx+Int_t(co*w); Cy = By-Int_t(si*w);
         lambda = Double_t(((px-Bx)*(Cx-Bx)+(py-By)*(Cy-By)))/Double_t(((Cx-Bx)*(Cx-Bx)+(Cy-By)*(Cy-By)));
         x2 = Double_t(px) - lambda*Double_t(Cx-Bx)-Double_t(Bx);
         y2 = Double_t(py) - lambda*Double_t(Cy-By)-Double_t(By);
         Size = Int_t(TMath::Sqrt(x2*x2+y2*y2)*2);
         if (Size<4) Size = 4;

         SetTextSize(Size/sizetowin);
         TAttText::Modify();
      }
      else {
         dx = px - pxold;  px1 += dx;
         dy = py - pyold;  py1 += dy;
      }
      PaintControlBox(px1, py1, -theta);
      pxold = px;  pyold = py;
      break;

   case kButton1Up:
      if (TestBit(kTextNDC)) {
         dpx  = gPad->GetX2() - gPad->GetX1();
         dpy  = gPad->GetY2() - gPad->GetY1();
         xp1  = gPad->GetX1();
         yp1  = gPad->GetY1();
         fX = (gPad->AbsPixeltoX(px1)-xp1)/dpx;
         fY = (gPad->AbsPixeltoY(py1)-yp1)/dpy;
      } else {
         fX = gPad->PadtoX(gPad->AbsPixeltoX(px1));
         fY = gPad->PadtoY(gPad->AbsPixeltoY(py1));
      }
      fTextAngle = theta;
      gPad->Modified(kTRUE);
      break;

   case kButton1Locate:
      ExecuteEvent(kButton1Down, px, py);

      while (1) {
         px = py = 0;
         event = gVirtualX->RequestLocator(1, 1, px, py);

         ExecuteEvent(kButton1Motion, px, py);

         if (event != -1) {                     // button is released
            ExecuteEvent(kButton1Up, px, py);
            return;
         }
      }
   }
}

//______________________________________________________________________________
void TText::GetControlBox(Int_t x, Int_t y, Double_t theta, 
                          Int_t CBoxX[4], Int_t CBoxY[4])
{
   // Return the text control box. The text position coordinates is (x,y) and
   // the text angle is theta. The control box coordinates are returned in CBoxX
   // and CBoxY.

   Short_t halign = fTextAlign/10;          // horizontal alignment
   Short_t valign = fTextAlign - 10*halign; // vertical alignment
   UInt_t CBoxW, CBoxH;                     // control box width and heigh
   UInt_t Dx = 0, Dy = 0;                   // delta along x and y to align the box

   GetBoundingBox(CBoxW, CBoxH);

   // compute the translations (Dx, Dy) required by the alignments
   switch (halign) {
      case 1 : Dx = 0      ; break;
      case 2 : Dx = CBoxW/2; break;
      case 3 : Dx = CBoxW  ; break;
   }
   switch (valign) {
      case 1 : Dy = 0      ; break;
      case 2 : Dy = CBoxH/2; break;
      case 3 : Dy = CBoxH  ; break;
   }

   // compute the control box coordinates before rotation
   CBoxX[0] = x-Dx;
   CBoxY[0] = y+Dy;
   CBoxX[1] = x-Dx;
   CBoxY[1] = y-CBoxH+Dy;
   CBoxX[2] = x+CBoxW-Dx;
   CBoxY[2] = y-CBoxH+Dy;
   CBoxX[3] = x+CBoxW-Dx;
   CBoxY[3] = y+Dy;

   // rotate the control box if needed
   if (theta) {
      Double_t cosTheta = TMath::Cos(theta*0.017453293);
      Double_t sinTheta = TMath::Sin(theta*0.017453293);
      for (int i=0; i<4 ; i++) {
         Int_t HCBoxX = CBoxX[i];
         Int_t HCBoxY = CBoxY[i];
         CBoxX[i] = (Int_t)((HCBoxX-x)*cosTheta-(HCBoxY-y)*sinTheta+x);
         CBoxY[i] = (Int_t)((HCBoxX-x)*sinTheta+(HCBoxY-y)*cosTheta+y);
      }
   }
}

//______________________________________________________________________________
void TText::GetBoundingBox(UInt_t &w, UInt_t &h)
{
   // Return text size in pixels

   if (TTF::IsInitialized() || gPad->IsBatch()) {
      TTF::GetTextExtent(w, h, (char*)GetTitle());
   } else {
      gVirtualX->GetTextExtent(w, h, (char*)GetTitle());
   }
}

//______________________________________________________________________________
void TText::GetTextAscentDescent(UInt_t &a, UInt_t &d, const char *text) const
{
   // Return text ascent and descent for string text
   //  in a return total text ascent
   //  in d return text descent

   Double_t     wh = (Double_t)gPad->XtoPixel(gPad->GetX2());
   Double_t     hh = (Double_t)gPad->YtoPixel(gPad->GetY1());
   Double_t tsize;
   if (wh < hh)  tsize = fTextSize*wh;
   else          tsize = fTextSize*hh;

   if (gVirtualX->HasTTFonts() || gPad->IsBatch()) {
      TTF::SetTextFont(fTextFont);
      TTF::SetTextSize(tsize);
      a = TTF::GetBox().yMax;
      d = TMath::Abs(TTF::GetBox().yMin);
   } else {
      UInt_t w;
      gVirtualX->SetTextSize((int)tsize);
      gVirtualX->GetTextExtent(w, a, (char*)text);
      d = 0;
   }
}

//______________________________________________________________________________
void TText::GetTextExtent(UInt_t &w, UInt_t &h, const char *text) const
{
   // Return text extent for string text
   //  in w return total text width
   //  in h return text height

   Double_t     wh = (Double_t)gPad->XtoPixel(gPad->GetX2());
   Double_t     hh = (Double_t)gPad->YtoPixel(gPad->GetY1());
   Double_t tsize;
   if (wh < hh)  tsize = fTextSize*wh;
   else          tsize = fTextSize*hh;

   if (gVirtualX->HasTTFonts() || gPad->IsBatch()) {
      TTF::SetTextFont(fTextFont);
      TTF::SetTextSize(tsize);
      TTF::GetTextExtent(w, h, (char*)text);
   } else {
      gVirtualX->SetTextSize((int)tsize);
      gVirtualX->GetTextExtent(w, h, (char*)text);
   }
}

//______________________________________________________________________________
void TText::ls(Option_t *) const
{
//*-*-*-*-*-*-*-*-*-*-*-*List this text with its attributes*-*-*-*-*-*-*-*-*
//*-*                    ==================================
   TROOT::IndentLevel();
   printf("Text  X=%f Y=%f Text=%s\n",fX,fY,GetTitle());
}

//______________________________________________________________________________
void TText::Paint(Option_t *)
{
//*-*-*-*-*-*-*-*-*-*-*Paint this text with its current attributes*-*-*-*-*-*-*
//*-*                  ===========================================

   TAttText::Modify();  //Change text attributes only if necessary
   if (TestBit(kTextNDC)) gPad->PaintTextNDC(fX,fY,GetTitle());
   else                   gPad->PaintText(gPad->XtoPad(fX),gPad->YtoPad(fY),GetTitle());
}

//______________________________________________________________________________
void TText::PaintControlBox(Int_t x, Int_t y, Double_t theta)
{
   // Paint the text control box. (x,y) are the coordinates where the control
   // box should be painted and theta is the angle of the box.

   Int_t CBoxX[4], CBoxY[4];
   Short_t halign = fTextAlign/10;               // horizontal alignment
   Short_t valign = fTextAlign - 10*halign;      // vertical alignment

   GetControlBox(x, y, theta, CBoxX, CBoxY);
   // Draw the text control box outline
   gVirtualX->SetLineStyle((Style_t)1);
   gVirtualX->SetLineWidth(1);
   gVirtualX->SetLineColor(1);
   gVirtualX->DrawLine(CBoxX[0], CBoxY[0], CBoxX[1], CBoxY[1]);
   gVirtualX->DrawLine(CBoxX[1], CBoxY[1], CBoxX[2], CBoxY[2]);
   gVirtualX->DrawLine(CBoxX[2], CBoxY[2], CBoxX[3], CBoxY[3]);
   gVirtualX->DrawLine(CBoxX[3], CBoxY[3], CBoxX[0], CBoxY[0]);

   // Draw a symbol at the text starting point
   TPoint p;
   Int_t ix = 0, iy = 0;
   switch (halign) {
      case 1 :
         switch (valign) {
            case 1 : ix = 0 ; iy = 0 ; break;
            case 2 : ix = 0 ; iy = 1 ; break;
            case 3 : ix = 1 ; iy = 1 ; break;
         }
      break;
      case 2 :
         switch (valign) {
            case 1 : ix = 0 ; iy = 3 ; break;
            case 2 : ix = 0 ; iy = 2 ; break;
            case 3 : ix = 1 ; iy = 2 ; break;
         }
      break;
      case 3 :
         switch (valign) {
            case 1 : ix = 3 ; iy = 3 ; break;
            case 2 : ix = 2 ; iy = 3 ; break;
            case 3 : ix = 2 ; iy = 2 ; break;
         }
      break;
   }
   p.fX = (CBoxX[ix]+CBoxX[iy])/2;
   p.fY = (CBoxY[ix]+CBoxY[iy])/2;
   gVirtualX->SetMarkerColor(1);
   gVirtualX->SetMarkerStyle(24);
   gVirtualX->SetMarkerSize(0.7);
   gVirtualX->DrawPolyMarker(1, &p);
}

//______________________________________________________________________________
void TText::PaintText(Double_t x, Double_t y, const char *text)
{
//*-*-*-*-*-*-*-*-*-*-*Draw this text with new coordinates*-*-*-*-*-*-*-*-*-*
//*-*                  ===================================

   TAttText::Modify();  //Change text attributes only if necessary
   gPad->PaintText(x,y,text);

}

//______________________________________________________________________________
void TText::PaintTextNDC(Double_t u, Double_t v, const char *text)
{
//*-*-*-*-*-*-*-*-*-*-*Draw this text with new coordinates in NDC*-*-*-*-*-*-*
//*-*                  ==========================================

   TAttText::Modify();  //Change text attributes only if necessary
   gPad->PaintTextNDC(u,v,text);

}

//______________________________________________________________________________
void TText::Print(Option_t *) const
{
//*-*-*-*-*-*-*-*-*-*-*Dump this text with its attributes*-*-*-*-*-*-*-*-*-*
//*-*                  ==================================

   printf("Text  X=%f Y=%f Text=%s Font=%d Size=%f",fX,fY,GetTitle(),GetTextFont(),GetTextSize());
   if (GetTextColor() != 1 ) printf(" Color=%d",GetTextColor());
   if (GetTextAlign() != 10) printf(" Align=%d",GetTextAlign());
   if (GetTextAngle() != 0 ) printf(" Angle=%f",GetTextAngle());
   printf("\n");
}

//______________________________________________________________________________
void TText::SavePrimitive(ofstream &out, Option_t *)
{
    // Save primitive as a C++ statement(s) on output stream out

   char quote = '"';
   if (gROOT->ClassSaved(TText::Class())) {
       out<<"   ";
   } else {
       out<<"   TText *";
   }
   TString s = GetTitle();
   s.ReplaceAll("\"","\\\"");
   out<<"text = new TText("<<fX<<","<<fY<<","<<quote<<s.Data()<<quote<<");"<<endl;
   if (TestBit(kTextNDC)) out<<"   text->SetNDC();"<<endl;

   SaveTextAttributes(out,"text",11,0,1,62,0.05);

   out<<"   text->Draw();"<<endl;
}

//______________________________________________________________________________
void TText::SetNDC(Bool_t isNDC)
{
    // Set NDC mode on if isNDC = kTRUE, off otherwise
   ResetBit(kTextNDC);
   if (isNDC) SetBit(kTextNDC);
}

//______________________________________________________________________________
void TText::Streamer(TBuffer &R__b)
{
   // Stream an object of class TText.

   if (R__b.IsReading()) {
      UInt_t R__s, R__c;
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c);
      if (R__v > 1) {
         TText::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
         return;
      }
      //====process old versions before automatic schema evolution
      TNamed::Streamer(R__b);
      TAttText::Streamer(R__b);
      Float_t x,y;
      R__b >> x; fX = x;
      R__b >> y; fY = y;
      //====end of old versions

   } else {
      TText::Class()->WriteBuffer(R__b,this);
   }
}
