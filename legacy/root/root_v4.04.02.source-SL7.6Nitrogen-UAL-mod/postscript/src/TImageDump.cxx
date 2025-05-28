// @(#)root/postscript:$Name:  $:$Id: TImageDump.cxx,v 1.5 2005/05/03 16:06:18 brun Exp $
// Author: Valeriy Onuchin

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TImageDump                                                           //
//                                                                      //
// save canvas as an image (GIF, JPEG, PNG, etc.) in batch mode     .   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TImageDump.h"
#include "TImage.h"
#include "TMath.h"
#include "TPoint.h"
#include "TColor.h"
#include "TVirtualPad.h"
#include "TEnv.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TText.h"


const char p_bits[26][32] = {
   {
      0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55,
      0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55,
      0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55
   },  //0
   {
      0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55,
      0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55,
      0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55
   },  //1
   {
      0x44, 0x44, 0x11, 0x11, 0x44, 0x44, 0x11, 0x11, 0x44, 0x44, 0x11, 0x11,
      0x44, 0x44, 0x11, 0x11, 0x44, 0x44, 0x11, 0x11, 0x44, 0x44, 0x11, 0x11,
      0x44, 0x44, 0x11, 0x11, 0x44, 0x44, 0x11, 0x11
   },  //2
   {
      0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x44, 0x44,
      0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x11, 0x11,
      0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x11, 0x11
   }, //3
   {
      0x80, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04,
      0x02, 0x02, 0x01, 0x01, 0x80, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10,
      0x08, 0x08, 0x04, 0x04, 0x02, 0x02, 0x01, 0x01
   }, //4
   {
      0x20, 0x20, 0x40, 0x40, 0x80, 0x80, 0x01, 0x01, 0x02, 0x02, 0x04, 0x04,
      0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x80, 0x80, 0x01, 0x01,
      0x02, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10
   }, //5
   {
      0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
      0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
      0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44
   }, //6
   {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff
   }, //7
   {
      0x11, 0x11, 0xb8, 0xb8, 0x7c, 0x7c, 0x3a, 0x3a, 0x11, 0x11, 0xa3, 0xa3,
      0xc7, 0xc7, 0x8b, 0x8b, 0x11, 0x11, 0xb8, 0xb8, 0x7c, 0x7c, 0x3a, 0x3a,
      0x11, 0x11, 0xa3, 0xa3, 0xc7, 0xc7, 0x8b, 0x8b
   }, //8
   {
      0x10, 0x10, 0x10, 0x10, 0x28, 0x28, 0xc7, 0xc7, 0x01, 0x01, 0x01, 0x01,
      0x82, 0x82, 0x7c, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x28, 0x28, 0xc7, 0xc7,
      0x01, 0x01, 0x01, 0x01, 0x82, 0x82, 0x7c, 0x7c
   }, //9
   {
      0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xff, 0xff, 0x01, 0x01, 0x01, 0x01,
      0x01, 0x01, 0xff, 0xff, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xff, 0xff,
      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xff, 0xff
   }, //10
   {
      0x08, 0x08, 0x49, 0x49, 0x2a, 0x2a, 0x1c, 0x1c, 0x2a, 0x2a, 0x49, 0x49,
      0x08, 0x08, 0x00, 0x00, 0x80, 0x80, 0x94, 0x94, 0xa2, 0xa2, 0xc1, 0xc1,
      0xa2, 0xa2, 0x94, 0x94, 0x80, 0x80, 0x00, 0x00
   }, //11
   {
      0x1c, 0x1c, 0x22, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x22, 0x22,
      0x1c, 0x1c, 0x00, 0x00, 0xc1, 0xc1, 0x22, 0x22, 0x14, 0x14, 0x14, 0x14,
      0x14, 0x14, 0x22, 0x22, 0xc1, 0xc1, 0x00, 0x00
   }, //12
   {
      0x01, 0x01, 0x82, 0x82, 0x44, 0x44, 0x28, 0x28, 0x10, 0x10, 0x28, 0x28,
      0x44, 0x44, 0x82, 0x82, 0x01, 0x01, 0x82, 0x82, 0x44, 0x44, 0x28, 0x28,
      0x10, 0x10, 0x28, 0x28, 0x44, 0x44, 0x82, 0x82
   }, //13
   {
      0xff, 0xff, 0x11, 0x10, 0x11, 0x10, 0x11, 0x10, 0xf1, 0x1f, 0x11, 0x11,
      0x11, 0x11, 0x11, 0x11, 0xff, 0x11, 0x01, 0x11, 0x01, 0x11, 0x01, 0x11,
      0xff, 0xff, 0x01, 0x10, 0x01, 0x10, 0x01, 0x10
   }, //14
   {
      0x22, 0x22, 0x55, 0x55, 0x22, 0x22, 0x00, 0x00, 0x88, 0x88, 0x55, 0x55,
      0x88, 0x88, 0x00, 0x00, 0x22, 0x22, 0x55, 0x55, 0x22, 0x22, 0x00, 0x00,
      0x88, 0x88, 0x55, 0x55, 0x88, 0x88, 0x00, 0x00
   }, //15
   {
      0x0e, 0x0e, 0x11, 0x11, 0xe0, 0xe0, 0x00, 0x00, 0x0e, 0x0e, 0x11, 0x11,
      0xe0, 0xe0, 0x00, 0x00, 0x0e, 0x0e, 0x11, 0x11, 0xe0, 0xe0, 0x00, 0x00,
      0x0e, 0x0e, 0x11, 0x11, 0xe0, 0xe0, 0x00, 0x00
   }, //16
   {
      0x44, 0x44, 0x22, 0x22, 0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x22, 0x22,
      0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x22, 0x22, 0x11, 0x11, 0x00, 0x00,
      0x44, 0x44, 0x22, 0x22, 0x11, 0x11, 0x00, 0x00
   }, //17
   {
      0x11, 0x11, 0x22, 0x22, 0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x22, 0x22,
      0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x22, 0x22, 0x44, 0x44, 0x00, 0x00,
      0x11, 0x11, 0x22, 0x22, 0x44, 0x44, 0x00, 0x00
   }, //18
   {
      0xe0, 0x03, 0x98, 0x0c, 0x84, 0x10, 0x42, 0x21, 0x42, 0x21, 0x21, 0x42,
      0x19, 0x4c, 0x07, 0xf0, 0x19, 0x4c, 0x21, 0x42, 0x42, 0x21, 0x42, 0x21,
      0x84, 0x10, 0x98, 0x0c, 0xe0, 0x03, 0x80, 0x00
   }, //19
   {
      0x22, 0x22, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x44, 0x44,
      0x44, 0x44, 0x44, 0x44, 0x22, 0x22, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
      0x22, 0x22, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44
   }, //20
   {
      0xf1, 0xf1, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f, 0x1f, 0x01, 0x01,
      0x01, 0x01, 0x01, 0x01, 0xf1, 0xf1, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
      0x1f, 0x1f, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01
   }, //21
   {
      0x8f, 0x8f, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xf8, 0xf8, 0x80, 0x80,
      0x80, 0x80, 0x80, 0x80, 0x8f, 0x8f, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
      0xf8, 0xf8, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
   }, //22
   {
      0xAA, 0xAA, 0x55, 0x55, 0x6a, 0x6a, 0x74, 0x74, 0x78, 0x78, 0x74, 0x74,
      0x6a, 0x6a, 0x55, 0x55, 0xAA, 0xAA, 0x55, 0x55, 0x6a, 0x6a, 0x74, 0x74,
      0x78, 0x78, 0x74, 0x74, 0x6a, 0x6a, 0x55, 0x55
   }, //23
   {
      0x80, 0x00, 0xc0, 0x00, 0xea, 0xa8, 0xd5, 0x54, 0xea, 0xa8, 0xd5, 0x54,
      0xeb, 0xe8, 0xd5, 0xd4, 0xe8, 0xe8, 0xd4, 0xd4, 0xa8, 0xe8, 0x54, 0xd5,
      0xa8, 0xea, 0x54, 0xd5, 0xfc, 0xff, 0xfe, 0xff
   }, //24
   {
      0x80, 0x00, 0xc0, 0x00, 0xe0, 0x00, 0xf0, 0x00, 0xff, 0xf0, 0xff, 0xf0,
      0xfb, 0xf0, 0xf9, 0xf0, 0xf8, 0xf0, 0xf8, 0x70, 0xf8, 0x30, 0xff, 0xf0,
      0xff, 0xf8, 0xff, 0xfc, 0xff, 0xfe, 0xff, 0xff
   }, //25
};



ClassImp(TImageDump)

//______________________________________________________________________________
TImageDump::TImageDump() : TVirtualPS()
{
   // Default SVG constructor

   fStream = 0;
   fImage  = 0;
   gVirtualPS = this;
}

//______________________________________________________________________________
TImageDump::TImageDump(const char *fname, Int_t wtype) : TVirtualPS(fname, wtype)
{
   // Initialize batch image interface
   //
   //  fname : image file name

   fStream = 0;
   fImage = TImage::Create();
}

//______________________________________________________________________________
void TImageDump::Open(const char *fname, Int_t type)
{
   // Open a image file

   fImage = TImage::Create();
   SetName(fname);
   fType = type;
}

//______________________________________________________________________________
TImageDump::~TImageDump()
{
   // destructor

   Close();

   delete fImage;
   fImage = 0;

   gVirtualPS = 0;
}

//______________________________________________________________________________
void TImageDump::Close(Option_t *)
{
   // Close a image file

   if (!fImage) return;
   if (fType == 112) fImage->Flip(90);
   
   fImage->WriteImage(GetName());
}

//______________________________________________________________________________
void TImageDump::DrawBox(Double_t x1, Double_t y1, Double_t x2, Double_t  y2)
{
   // Draw a Box

   if (!gPad || !fImage) {
      return;
   }

   Int_t ix1 = x1 < x2 ? gPad->XtoAbsPixel(x1) : gPad->XtoAbsPixel(x2);
   Int_t ix2 = x1 < x2 ? gPad->XtoAbsPixel(x2) : gPad->XtoAbsPixel(x1);
   Int_t iy1 = y1 < y2 ? gPad->YtoAbsPixel(y1) : gPad->YtoAbsPixel(y2);
   Int_t iy2 = y1 < y2 ? gPad->YtoAbsPixel(y2) : gPad->YtoAbsPixel(y1);

   TColor *col = gROOT->GetColor(fFillColor);
   fImage->DrawBox(ix1, iy1, ix2, iy2, col->AsHexString(), 1, TVirtualX::kFilled);
}

//______________________________________________________________________________
void TImageDump::DrawFrame(Double_t x1, Double_t y1, Double_t x2, Double_t  y2,
                            Int_t mode, Int_t bordersize, Int_t dark, Int_t light)
{
   // Draw a Frame around a box
   //
   // mode = -1  the box looks as it is behind the screen
   // mode =  1  the box looks as it is in front of the screen
   // border is the border size in already pre-computed SVG units dark is the
   // color for the dark part of the frame light is the color for the light
   // part of the frame

   if (!gPad || !fImage || (bordersize < 1)) {
      return;
   }
   TColor *col;
   TColor *lo = gROOT->GetColor(dark);
   TColor *hi = gROOT->GetColor(light);

   Short_t pxl,pyl,pxt,pyt,px1,py1,px2,py2;
   Double_t xl, xt, yl, yt;

   px1 = gPad->XtoAbsPixel(x1);   py1 = gPad->YtoAbsPixel(y1);
   px2 = gPad->XtoAbsPixel(x2);   py2 = gPad->YtoAbsPixel(y2);
   if (px1 < px2) {pxl = px1; pxt = px2; xl = x1; xt = x2; }
   else           {pxl = px2; pxt = px1; xl = x2; xt = x1;}
   if (py1 > py2) {pyl = py1; pyt = py2; yl = y1; yt = y2;}
   else           {pyl = py2; pyt = py1; yl = y2; yt = y1;}

   if (bordersize == 1) {
      fImage->DrawBox(pxl, pyl, pxt, pyt-1, 
                      gROOT->GetColor(fLineColor)->AsHexString());
      return;
   }

   TPoint frame[6];

   frame[0].fX = pxl;                 frame[0].fY = pyl;
   frame[1].fX = pxl + bordersize;    frame[1].fY = pyl - bordersize;
   frame[2].fX = pxl + bordersize;    frame[2].fY = pyt + bordersize;
   frame[3].fX = pxt - bordersize;    frame[3].fY = pyt + bordersize;;
   frame[4].fX = pxt;                 frame[4].fY = pyt;
   frame[5].fX = pxl;                 frame[5].fY = pyt;

   if (mode == -1) col = lo;
   else            col = hi;

   fImage->DrawFillArea(6, frame, col->AsHexString());

   frame[0].fX = pxl;                 frame[0].fY = pyl;
   frame[1].fX = pxl + bordersize;    frame[1].fY = pyl - bordersize;
   frame[2].fX = pxt - bordersize;    frame[2].fY = frame[1].fY;
   frame[3].fX = frame[2].fX;         frame[3].fY = pyt + bordersize;
   frame[4].fX = pxt;                 frame[4].fY = pyt;
   frame[5].fX = pxt;                 frame[5].fY = pyl;

   if (mode == -1) col = hi;
   else            col = lo;
   fImage->DrawFillArea(6, frame, col->AsHexString());
}

//______________________________________________________________________________
void TImageDump::DrawPolyMarker(Int_t, Float_t *, Float_t *)
{
   // not used

   if (!gPad || !fImage) {
      return;
   }
}

//______________________________________________________________________________
void TImageDump::DrawPolyMarker(Int_t n, Double_t *xw, Double_t *yw)
{
   // draw polymarker

   if (!gPad || !fImage) {
      return;
   }
   Int_t ms = TMath::Abs(fMarkerStyle);
   static TPoint pt[20];

   if (ms >= 6 && ms <= 19) ms = 20;
   if (ms == 4) ms = 24;

   // Define the marker size
   Double_t msize = 0.23*fMarkerSize*TMath::Max(fImage->GetWidth(), fImage->GetHeight())/20;
   if (ms == 6) msize *= 0.2;
   if (ms == 7) msize *= 0.3;
   Double_t m  = msize;
   Double_t m2 = m/2;
   Double_t m3 = m/3;
//   Double_t m4 = m2*1.333333333333;
   Double_t m6 = m/6;

   TColor *col = gROOT->GetColor(fMarkerColor);

   // Draw the marker according to the type
   Short_t ix,iy;
   for (Int_t i=0;i<n;i++) {
      ix = gPad->XtoAbsPixel(xw[i]);
      iy = gPad->YtoAbsPixel(yw[i]);

      switch (ms) {
      // Dot (.)
      case 1:
         fImage->PutPixel((UInt_t)ix, (UInt_t)iy, col->AsHexString());
         break;
      // Plus (+)
      case 2:
         fImage->DrawLine(UInt_t(ix-m2), UInt_t(iy), UInt_t(ix+m2), UInt_t(iy), col->AsHexString());
         fImage->DrawLine(UInt_t(ix), UInt_t(iy-m2), UInt_t(ix), UInt_t(iy+m2), col->AsHexString());
         break;
      // X shape (X)
      case 5:
         fImage->DrawLine(UInt_t(ix-m2), UInt_t(iy-m2), UInt_t(ix+m2), UInt_t(iy+m2), col->AsHexString());
         fImage->DrawLine(UInt_t(ix-m2), UInt_t(iy+m2), UInt_t(ix+m2), UInt_t(iy-m2), col->AsHexString());
         break;
      // Asterisk shape (*)
      case 3:
      case 31:
         fImage->DrawLine(UInt_t(ix-m2), UInt_t(iy), UInt_t(ix+m2), UInt_t(iy), col->AsHexString());
         fImage->DrawLine(UInt_t(ix), UInt_t(iy-m2), UInt_t(ix), UInt_t(iy+m2), col->AsHexString());
         fImage->DrawLine(UInt_t(ix-m2), UInt_t(iy-m2), UInt_t(ix+m2), UInt_t(iy+m2), col->AsHexString());
         fImage->DrawLine(UInt_t(ix-m2), UInt_t(iy+m2), UInt_t(ix+m2), UInt_t(iy-m2), col->AsHexString());
         break;
      // Circle
      case 24:
      case 20:
         //MoveTo(ix-m2, iy);
         //WriteReal(ix-m2); WriteReal(iy+m4);
         //WriteReal(ix+m2); WriteReal(iy+m4);
         //WriteReal(ix+m2); WriteReal(iy)   ; PrintFast(2," c");
         //WriteReal(ix+m2); WriteReal(iy-m4);
         //WriteReal(ix-m2); WriteReal(iy-m4);
         //WriteReal(ix-m2); WriteReal(iy)   ; PrintFast(4," c h");

         // temporary
         ms == 24 ? 
         fImage->DrawRectangle(UInt_t(ix-m2), UInt_t(iy-m2), UInt_t(m), UInt_t(m), col->AsHexString()) :
         fImage->FillRectangle(col->AsHexString(), UInt_t(ix-m2), UInt_t(iy-m2), UInt_t(m), UInt_t(m));
         break;
      // Square
      case 21:
         fImage->FillRectangle(col->AsHexString(), UInt_t(ix-m2), UInt_t(iy-m2), UInt_t(m), UInt_t(m));
         break;
      case 25:
         fImage->DrawRectangle(UInt_t(ix-m2), UInt_t(iy-m2), UInt_t(m), UInt_t(m), col->AsHexString());
         break;
      // Down triangle
      case 23:
         pt[0].fX = Short_t(ix);    pt[0].fY = Short_t(iy-m2);
         pt[1].fX = Short_t(ix+m2); pt[1].fY = Short_t(iy+m2);
         pt[2].fX = Short_t(ix-m2); pt[2].fY = Short_t(iy+m2);
         fImage->FillPolygon(3, pt, col->AsHexString());
         break;
      // Up triangle
      case 26:
      case 22:
         pt[0].fX = Short_t(ix-m2); pt[0].fY = Short_t(iy-m2);
         pt[1].fX = Short_t(ix+m2); pt[1].fY = Short_t(iy-m2);
         pt[2].fX = Short_t(ix);    pt[2].fY = Short_t(iy+m2);
         ms == 26 ? fImage->DrawPolyLine(3, pt, col->AsHexString()) : 
                    fImage->FillPolygon(3, pt, col->AsHexString());
         break;
      case 27:
         fImage->DrawLine(UInt_t(ix), UInt_t(iy-m2), UInt_t(ix+m3), UInt_t(iy), col->AsHexString());
         fImage->DrawLine(UInt_t(ix+m3), UInt_t(iy), UInt_t(ix), UInt_t(iy+m2), col->AsHexString());
         fImage->DrawLine(UInt_t(ix), UInt_t(iy+m2), UInt_t(ix-m3), UInt_t(iy), col->AsHexString());
         fImage->DrawLine(UInt_t(ix-m3), UInt_t(iy), UInt_t(ix), UInt_t(iy-m2), col->AsHexString());
         break;
      case 28:
         fImage->DrawLine(UInt_t(ix-m6), UInt_t(iy-m6), UInt_t(ix-m6), UInt_t(iy-m2), col->AsHexString());
         fImage->DrawLine(UInt_t(ix-m6), UInt_t(iy-m2), UInt_t(ix+m6), UInt_t(iy-m2), col->AsHexString());
         fImage->DrawLine(UInt_t(ix+m6), UInt_t(iy-m2), UInt_t(ix+m6), UInt_t(iy-m6), col->AsHexString());
         fImage->DrawLine(UInt_t(ix+m6), UInt_t(iy-m6), UInt_t(ix+m2), UInt_t(iy-m6), col->AsHexString());
         fImage->DrawLine(UInt_t(ix+m2), UInt_t(iy-m6), UInt_t(ix+m2), UInt_t(iy+m6), col->AsHexString());
         fImage->DrawLine(UInt_t(ix+m2), UInt_t(iy+m6), UInt_t(ix+m6), UInt_t(iy+m6), col->AsHexString());
         fImage->DrawLine(UInt_t(ix+m6), UInt_t(iy+m6), UInt_t(ix+m6), UInt_t(iy+m2), col->AsHexString());
         fImage->DrawLine(UInt_t(ix+m6), UInt_t(iy+m2), UInt_t(ix-m6), UInt_t(iy+m2), col->AsHexString());
         fImage->DrawLine(UInt_t(ix-m6), UInt_t(iy+m2), UInt_t(ix-m6), UInt_t(iy+m6), col->AsHexString());
         fImage->DrawLine(UInt_t(ix-m6), UInt_t(iy+m6), UInt_t(ix-m2), UInt_t(iy+m6), col->AsHexString());
         fImage->DrawLine(UInt_t(ix-m2), UInt_t(iy+m6), UInt_t(ix-m2), UInt_t(iy-m6), col->AsHexString());
         fImage->DrawLine(UInt_t(ix-m2), UInt_t(iy-m6), UInt_t(ix-m6), UInt_t(iy-m6), col->AsHexString());
         break;
      case 29:
      case 30:
         pt[0].fX = Short_t(ix);             pt[0].fY = Short_t(iy+m2);
         pt[1].fX = Short_t(ix+0.112255*m);  pt[1].fY = Short_t(iy+0.15451*m);
         pt[2].fX = Short_t(ix+0.47552*m);   pt[2].fY = Short_t(iy+0.15451*m);
         pt[3].fX = Short_t(ix+0.181635*m);  pt[3].fY = Short_t(iy-0.05902*m);
         pt[4].fX = Short_t(ix+0.29389*m);   pt[4].fY = Short_t(iy-0.40451*m);
         pt[5].fX = Short_t(ix);             pt[5].fY = Short_t(iy-0.19098*m);
         pt[6].fX = Short_t(ix-0.29389*m);   pt[6].fY = Short_t(iy-0.40451*m);
         pt[7].fX = Short_t(ix-0.181635*m);  pt[7].fY = Short_t(iy-0.05902*m);
         pt[8].fX = Short_t(ix-0.47552*m);   pt[8].fY = Short_t(iy+0.15451*m);
         pt[9].fX = Short_t(ix-0.112255*m);  pt[9].fY = Short_t(iy+0.15451*m);
         ms == 300 ? fImage->DrawPolyLine(10, pt, col->AsHexString()) : 
                     fImage->FillPolygon(10, pt, col->AsHexString());
         break;
      default:
         fImage->PutPixel(UInt_t(ix), UInt_t(iy), col->AsHexString());
         break;
      }
   }
}

//______________________________________________________________________________
void TImageDump::DrawPS(Int_t nn, Double_t *x, Double_t *y)
{
   // This function defines a path with xw and yw and draw it according the
   // value of nn:
   //
   //  If nn>0 a line is drawn.
   //  If nn<0 a closed polygon is drawn.

   if (!gPad || !fImage || !nn) {
      return;
   }
   TColor *col = 0;
   Int_t  fais = 0 , fasi = 0;
   Bool_t line = nn > 0;
   UInt_t n = TMath::Abs(nn); 

   fais = fFillStyle/1000;
   fasi = fFillStyle%1000;

   Short_t px1, py1, px2, py2;
   static const UInt_t gCachePtSize = 200;
   static TPoint gPointCache[gCachePtSize];
   Bool_t del = kTRUE;

   switch (n) {
      case 1:
         col = gROOT->GetColor(fFillColor);
         px1 = gPad->XtoAbsPixel(x[0]);   py1 = gPad->YtoAbsPixel(y[0]);
         fImage->PutPixel(px1, py1, col->AsHexString());
         break;

      case 2:
      {
         col = gROOT->GetColor(fLineColor);
         px1 = gPad->XtoAbsPixel(x[0]);   py1 = gPad->YtoAbsPixel(y[0]);
         px2 = gPad->XtoAbsPixel(x[1]);   py2 = gPad->YtoAbsPixel(y[1]);

         switch (fLineStyle) {
            case 1:
               fImage->DrawLine(px1, py1, px2, py2, col->AsHexString(), fLineWidth);
               break;
            case 2:
               fImage->DrawDashLine(px1, py1, px2, py2, 2, "\x5\x5", col->AsHexString(), fLineWidth);
               break;
            case 3:
               fImage->DrawDashLine(px1, py1, px2, py2, 2, "\x1\x3", col->AsHexString(), fLineWidth);
               break;
            case 4:
               fImage->DrawDashLine(px1, py1, px2, py2, 4,"\x5\x3\x1\x3", col->AsHexString(), fLineWidth);
               break;
            default:
               fImage->DrawLine(px1, py1, px2, py2, col->AsHexString());
               break;
         }
         break;
      }
      default:
      {
         if (!line && ((fais == 3) || (fais == 2)) && (fasi > 100) ) {
            return;
         }

         TPoint *pt = 0;
         if (n+1 < gCachePtSize) {
            pt = (TPoint*)&gPointCache;
            del = kFALSE;
         } else {
            pt = new TPoint[n+1];
            del = kTRUE;
         }

         TColor *fcol = gROOT->GetColor(fFillColor);
         TColor *lcol = gROOT->GetColor(fLineColor);

         for (UInt_t i = 0; i < n; i++) {
            pt[i].fX = gPad->XtoAbsPixel(x[i]);
            pt[i].fY = gPad->YtoAbsPixel(y[i]);
         }
         pt[n].fX = pt[0].fX;
         pt[n].fY = pt[0].fY;

         const char *stipple = (fais == 3) && (fasi > 0) && (fasi < 26) ? p_bits[fasi] : 0;

         if (!line && fFillStyle) {
            if (n < 5) {
               fImage->FillPolygon(n, pt, fcol->AsHexString(), stipple);
            } else {
               fImage->DrawFillArea(n, pt, fcol->AsHexString(), stipple);
            }          
         }
         if (line || !fFillStyle || stipple) {
            fImage->DrawPolyLine(line ? n : n+1, pt, lcol->AsHexString(), fLineWidth);
         }
         if (del) delete [] pt;
      }   
      break;
   }
}

//______________________________________________________________________________
void TImageDump::DrawPS(Int_t, Float_t *, Float_t *)
{
   // not used

   if (!gPad || !fImage) {
      return;
   }
}

//______________________________________________________________________________
void TImageDump::NewPage()
{
   // Start new page. This function initialize the pad conversion
   // coefficients and ouput the <svg> directive which is close later in the
   // the function Close.

   if (!gPad) {
      return;
   }

   delete fImage;

   UInt_t w = (UInt_t)gPad->XtoPixel(gPad->GetX2());
   UInt_t h = (UInt_t)gPad->YtoPixel(gPad->GetY1());

   fImage = new TImage(w, h);
}

//______________________________________________________________________________
void TImageDump::Text(Double_t xx, Double_t yy, const char *chars)
{
   // Draw text
   //
   // xx: x position of the text
   // yy: y position of the text

   // To scale fonts to the same size as the old TT version
   const Float_t kScale = 1;
   const Double_t kDEGRAD = TMath::Pi()/180.;

   if (!gPad || !fImage || (fTextSize < 0)) {
      return;
   }

   Double_t x = gPad->XtoAbsPixel(xx);
   Double_t y = gPad->YtoAbsPixel(yy);

   const char *fontname;

   switch (TMath::Abs(fTextFont/10)) {
      case 1:
          fontname = "timesi.ttf";
          break;
      case 2:
          fontname = "timesbd.ttf";
          break;
      case 3:
          fontname = "timesbi.ttf";
          break;
      case 4:
          fontname = "arial.ttf";
          break;
      case 5:
          fontname = "ariali.ttf";
          break;
      case 6:
          fontname = "arialbd.ttf";
          break;
      case 7:
          fontname = "arialbi.ttf";
          break;
      case 8:
          fontname = "cour.ttf";
          break;
      case 9:
          fontname = "couri.ttf";
          break;
      case 10:
          fontname = "courbd.ttf";
          break;
      case 11:
          fontname = "courbi.ttf";
          break;
      case 12:
          fontname = "symbol.ttf";
          break;
      case 13:
          fontname = "times.ttf";
          break;
      case 14:
          fontname = "wingding.ttf";
          break;
      default:
          fontname = "arialbd.ttf";
          break;
   }

   // try to load font (font must be in Root.TTFontPath resource)
   const char *ttpath = gEnv->GetValue("Root.TTFontPath",
                                       "$(ROOTSYS)/fonts");

   char *ttfont = gSystem->Which(ttpath, fontname, kReadPermission);

   if (!ttfont) {
      Error("Text", "font file %s not found in path", fontname);
      return;
   }

   Double_t wh = (Double_t)gPad->XtoPixel(gPad->GetX2());
   Double_t hh = (Double_t)gPad->YtoPixel(gPad->GetY1());

   Int_t ttfsize;

   if (wh < hh) {
      ttfsize = (Int_t)(fTextSize*wh*kScale);
   } else {
      ttfsize = (Int_t)(fTextSize*hh*kScale);
   }

   if (ttfsize <= 0) return;

   // Text alignment
   Int_t txalh = fTextAlign/10;
   if (txalh < 1) txalh = 1;
   if (txalh > 3) txalh = 3;

   Int_t txalv = fTextAlign%10;
   if (txalv < 1) txalv = 1;
   if (txalv > 3) txalv = 3;

   UInt_t w, h;

   TText t;   
   t.SetTextSize(fTextSize);
   t.SetTextFont(fTextFont);
   t.GetTextExtent(w, h, chars);

   if (txalh == 2) x -= (w>>1);
   if (txalh == 3) x -= w;

   Float_t angle = kDEGRAD*fTextAngle;

   if (txalv == 3) {
     y += (fTextAngle != 0. ? h * TMath::Cos(angle) : h);
   } 
   if (txalv == 2) {
     y += (fTextAngle != 0. ? (h>>1) * TMath::Cos(angle) : h>>1);
   } 

   TColor *col = gROOT->GetColor(fTextColor);
   fImage->DrawText((int)x, angle ? (int)y - w: (int)y - h, chars, ttfsize, col->AsHexString(),
                     ttfont, TImage::kPlain, 0, fTextAngle);

   delete [] ttfont;
}


////////////////////////// CellArray code ////////////////////////////////////
static UInt_t *cellArrayColors = 0;
static Int_t cellArrayN = 0;
static Int_t cellArrayW = 0;
static Int_t cellArrayH = 0;
static Int_t cellArrayX1 = 0;
static Int_t cellArrayX2 = 0;
static Int_t cellArrayY1 = 0;
static Int_t cellArrayY2 = 0;
static Int_t cellArrayIdx = 0;

//______________________________________________________________________________
void TImageDump::CellArrayBegin(Int_t w, Int_t h, Double_t x1, Double_t x2,
                                Double_t y1, Double_t y2)
{
   //

   if (!gPad || !fImage || (w <= 0) || (h <= 0)) {
      return;
   }

   if (cellArrayColors) {
      delete [] cellArrayColors;
   }

   cellArrayN = w * h;
   cellArrayW = w;
   cellArrayH = h;
   cellArrayColors = new UInt_t[cellArrayN];

   cellArrayX1 = x1 < x2 ? gPad->XtoPixel(x1) : gPad->XtoPixel(x2);
   cellArrayX2 = x1 > x2 ? gPad->XtoPixel(x2) : gPad->XtoPixel(x1);
   cellArrayY1 = y1 < y2 ? gPad->YtoPixel(y1) : gPad->YtoPixel(y2);
   cellArrayY2 = y1 < y2 ? gPad->YtoPixel(y2) : gPad->YtoPixel(y1);

   cellArrayIdx = 0;
}

//______________________________________________________________________________
void TImageDump::CellArrayFill(Int_t r, Int_t g, Int_t b)
{
   //

   if (cellArrayIdx >= cellArrayN) return;

   cellArrayColors[cellArrayIdx] = ((r & 0xFF) << 16) + ((g & 0xFF) << 8) + (b & 0xFF);
   cellArrayIdx++;
}

//______________________________________________________________________________
void TImageDump::CellArrayEnd()
{
   //

   if (!fImage || !cellArrayColors || !cellArrayW || !cellArrayH) {
      return;
   }

   fImage->DrawCellArray(cellArrayX1, cellArrayX2, cellArrayY1, cellArrayY2, 
                         cellArrayW, cellArrayH, cellArrayColors);

   delete [] cellArrayColors;
   cellArrayColors = 0;
   cellArrayN = 0;
   cellArrayW = 0;
   cellArrayH = 0;
   cellArrayX1 = 0;
   cellArrayX2 = 0;
   cellArrayY1 = 0;
   cellArrayY2 = 0;
   cellArrayIdx = 0;
}

//______________________________________________________________________________
void TImageDump::SetColor(Float_t /*r*/, Float_t /*g*/, Float_t /*b*/)
{
   // Set color with its R G B components
   //
   //  r: % of red in [0,1]
   //  g: % of green in [0,1]
   //  b: % of blue in [0,1]

}
