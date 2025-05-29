// @(#)root/hist:$Name:  $:$Id: TAxis.cxx,v 1.64 2005/03/21 09:15:05 brun Exp $
// Author: Rene Brun   12/12/94

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "Riostream.h"
#include "TAxis.h"
#include "TVirtualPad.h"
#include "TVirtualX.h"
#include "TStyle.h"
#include "TView.h"
#include "TError.h"
#include "TH1.h"
#include "TObjString.h"
#include "TDatime.h"
#include <time.h>

ClassImp(TAxis)

//______________________________________________________________________________
//
// This class manages histogram axis. It is referenced by TH1 and TGraph.
// To make a graphical representation of an histogram axis, this class
// references the TGaxis class.
//
// TAxis supports axis with fixed or variable bin sizes.
// Labels may be associated to individual bins.
//
//    see examples of various axis representations drawn by class TGaxis.
//

//______________________________________________________________________________
TAxis::TAxis(): TNamed(), TAttAxis()
{
   fNbins   = 1;
   fXmin    = 0;
   fXmax    = 1;
   fFirst   = 0;
   fLast    = 0;
   fParent  = 0;
   fLabels  = 0;
   fBits2   = 0;
   fTimeDisplay = 0;
}

//______________________________________________________________________________
TAxis::TAxis(Int_t nbins,Axis_t xlow,Axis_t xup): TNamed(), TAttAxis()
{
//*-*-*-*-*-*-*-*Axis constructor for axis with fix bin size*-*-*-*-*-*-*-*
//*-*            ===========================================

   fParent  = 0;
   fLabels  = 0;
   Set(nbins,xlow,xup);
}

//______________________________________________________________________________
TAxis::TAxis(Int_t nbins,const Axis_t *xbins): TNamed(), TAttAxis()
{
//*-*-*-*-*-*-*-*Axis constructor for variable bin size*-*-*-*-*-*-*-*-*-*-*
//*-*            ======================================

   fParent  = 0;
   fLabels  = 0;
   Set(nbins,xbins);
}

//______________________________________________________________________________
TAxis::~TAxis()
{
}

//______________________________________________________________________________
TAxis::TAxis(const TAxis &axis) : TNamed(axis), TAttAxis(axis)
{
   ((TAxis&)axis).Copy(*this);
}

//______________________________________________________________________________
void TAxis::CenterLabels(Bool_t center)
{
//   if center = kTRUE axis labels will be centered (hori axes only)
//   on the bin center
//   default is to center on the primary tick marks
//   This option does not make sense if there are more bins than tick marks

   if (center) SetBit(kCenterLabels);
   else        ResetBit(kCenterLabels);
}

//______________________________________________________________________________
Bool_t TAxis::GetCenterLabels() const
{
   // Return kTRUE if kCenterLabels bit is set, kFALSE otherwise.

   return TestBit(kCenterLabels) ? kTRUE : kFALSE;
}

//______________________________________________________________________________
void TAxis::CenterTitle(Bool_t center)
{
//   if center = kTRUE axis title will be centered
//   default is right adjusted

   if (center) SetBit(kCenterTitle);
   else        ResetBit(kCenterTitle);
}

//______________________________________________________________________________
Bool_t TAxis::GetCenterTitle() const
{
   // Return kTRUE if kCenterTitle bit is set, kFALSE otherwise.

   return TestBit(kCenterTitle) ? kTRUE : kFALSE;
}

//______________________________________________________________________________
const char *TAxis::ChooseTimeFormat(Double_t axislength)
{
// Choose a reasonable time format from the coordinates in the active pad
// and the number of divisions in this axis
// If orientation = "X", the horizontal axis of the pad will be used for ref.
// If orientation = "Y", the vertical axis of the pad will be used for ref.

   const char *formatstr;
   Int_t reasformat = 0;
   Int_t ndiv,nx1,nx2,N;
   Double_t awidth;
   Double_t length;

   if (!axislength) {
      length = gPad->GetUxmax() - gPad->GetUxmin();
   } else {
      length = axislength;
   }

   ndiv = GetNdivisions();
   if (ndiv > 1000) {
      nx2   = ndiv/100;
      nx1   = TMath::Max(1, ndiv%100);
      ndiv = 100*nx2 + Int_t(Double_t(nx1)*gPad->GetAbsWNDC());
   }
   ndiv = TMath::Abs(ndiv);
   N = ndiv - (ndiv/100)*100;
   awidth = length/N;

//  width in seconds ?
   if (awidth>=.5) {
      reasformat = 1;
//  width in minutes ?
      if (awidth>=30) {
         awidth /= 60; reasformat = 2;
//   width in hours ?
         if (awidth>=30) {
            awidth /=60; reasformat = 3;
//   width in days ?
            if (awidth>=12) {
               awidth /= 24; reasformat = 4;
//   width in months ?
               if (awidth>=15.218425) {
                  awidth /= 30.43685; reasformat = 5;
//   width in years ?
                  if (awidth>=6) {
                     awidth /= 12; reasformat = 6;
                     if (awidth>=2) {
                        awidth /= 12; reasformat = 7;
                     }
                  }
               }
            }
         }
      }
   }
//   set reasonable format
   switch (reasformat) {
      case 0:
        formatstr = "%S";
        break;
      case 1:
        formatstr = "%Mm%S";
        break;
      case 2:
        formatstr = "%Hh%M";
        break;
      case 3:
        formatstr = "%d-%Hh";
        break;
      case 4:
        formatstr = "%d/%m";
        break;
      case 5:
        formatstr = "%d/%m/%y";
        break;
      case 6:
        formatstr = "%d/%m/%y";
        break;
      case 7:
        formatstr = "%m/%y";
        break;
      default:
        formatstr = "%H:%M:%S";
        break;
   }
   return formatstr;
}

//______________________________________________________________________________
void TAxis::Copy(TObject &obj) const
{
//*-*-*-*-*-*-*-*-*-*Copy axis structure to another axis-*-*-*-*-*-*-*-*-*-*-*
//*-*                ===================================

   TNamed::Copy(obj);
   TAttAxis::Copy(((TAxis&)obj));
   ((TAxis&)obj).fNbins  = fNbins;
   ((TAxis&)obj).fXmin   = fXmin;
   ((TAxis&)obj).fXmax   = fXmax;
   ((TAxis&)obj).fFirst  = fFirst;
   ((TAxis&)obj).fLast   = fLast;
   ((TAxis&)obj).fBits2  = fBits2;
   fXbins.Copy(((TAxis&)obj).fXbins);
   ((TAxis&)obj).fTimeFormat   = fTimeFormat;
   ((TAxis&)obj).fTimeDisplay  = fTimeDisplay;
   ((TAxis&)obj).fParent       = fParent;
}

//______________________________________________________________________________
Int_t TAxis::DistancetoPrimitive(Int_t, Int_t)
{
//*-*-*-*-*-*-*-*-*-*-*Compute distance from point px,py to an axis*-*-*-*-*-*
//*-*                  ============================================

   return 9999;
}

//______________________________________________________________________________
void TAxis::ExecuteEvent(Int_t event, Int_t px, Int_t py)
{
//*-*-*-*-*-*-*-*-*-*-*Execute action corresponding to one event*-*-*-*
//*-*                  =========================================
//  This member function is called when an axis is clicked with the locator
//
//  The axis range is set between the position where the mouse is pressed
//  and the position where it is released.
//  If the mouse position is outside the current axis range when it is released
//  the axis is unzoomed with the corresponding proportions.
//  Note that the mouse does not need to be in the pad or even canvas
//  when it is released.

   if (!gPad->IsEditable()) return;

   gPad->SetCursor(kHand);

   TView *view = gPad->GetView();
   static Int_t axisNumber;
   static Double_t ratio1, ratio2;
   static Int_t px1old, py1old, px2old, py2old;
   Int_t bin1, bin2, first, last;
   Double_t temp, xmin,xmax;

   switch (event) {

   case kButton1Down:
      axisNumber = 1;
      if (!strcmp(GetName(),"xaxis")) {
         axisNumber = 1;
         if (!gPad->IsVertical()) axisNumber = 2;
      }
      if (!strcmp(GetName(),"yaxis")) {
         axisNumber = 2;
         if (!gPad->IsVertical()) axisNumber = 1;
      }
      if (!strcmp(GetName(),"zaxis")) {
         axisNumber = 3;
      }
      if (view) {
         view->GetDistancetoAxis(axisNumber, px, py, ratio1);
      } else {
         if (axisNumber == 1) {
            ratio1 = (gPad->AbsPixeltoX(px) - gPad->GetUxmin())/(gPad->GetUxmax() - gPad->GetUxmin());
            px1old = gPad->XtoAbsPixel(gPad->GetUxmin()+ratio1*(gPad->GetUxmax() - gPad->GetUxmin()));
            py1old = gPad->YtoAbsPixel(gPad->GetUymin());
            px2old = px1old;
            py2old = gPad->YtoAbsPixel(gPad->GetUymax());
         } else if (axisNumber == 2) {
            ratio1 = (gPad->AbsPixeltoY(py) - gPad->GetUymin())/(gPad->GetUymax() - gPad->GetUymin());
            py1old = gPad->YtoAbsPixel(gPad->GetUymin()+ratio1*(gPad->GetUymax() - gPad->GetUymin()));
            px1old = gPad->XtoAbsPixel(gPad->GetUxmin());
            px2old = gPad->XtoAbsPixel(gPad->GetUxmax());
            py2old = py1old;
         } else {
            ratio1 = (gPad->AbsPixeltoY(py) - gPad->GetUymin())/(gPad->GetUymax() - gPad->GetUymin());
            py1old = gPad->YtoAbsPixel(gPad->GetUymin()+ratio1*(gPad->GetUymax() - gPad->GetUymin()));
            px1old = gPad->XtoAbsPixel(gPad->GetUxmax());
            px2old = gPad->XtoAbsPixel(gPad->GetX2());
            py2old = py1old;
         }
         gVirtualX->DrawBox(px1old, py1old, px2old, py2old, TVirtualX::kHollow);
      }
      gVirtualX->SetLineColor(-1);
      // No break !!!

   case kButton1Motion:
      if (view) {
         view->GetDistancetoAxis(axisNumber, px, py, ratio2);
      } else {
         gVirtualX->DrawBox(px1old, py1old, px2old, py2old, TVirtualX::kHollow);
         if (axisNumber == 1) {
            ratio2 = (gPad->AbsPixeltoX(px) - gPad->GetUxmin())/(gPad->GetUxmax() - gPad->GetUxmin());
            px2old = gPad->XtoAbsPixel(gPad->GetUxmin()+ratio2*(gPad->GetUxmax() - gPad->GetUxmin()));
         } else {
            ratio2 = (gPad->AbsPixeltoY(py) - gPad->GetUymin())/(gPad->GetUymax() - gPad->GetUymin());
            py2old = gPad->YtoAbsPixel(gPad->GetUymin()+ratio2*(gPad->GetUymax() - gPad->GetUymin()));
         }
         gVirtualX->DrawBox(px1old, py1old, px2old, py2old, TVirtualX::kHollow);
      }
   break;

   case kButton1Up:
      if (view) {
         view->GetDistancetoAxis(axisNumber, px, py, ratio2);
         if (ratio1 > ratio2) {
            temp   = ratio1;
            ratio1 = ratio2;
            ratio2 = temp;
         }
         if (ratio2 - ratio1 > 0.05) {
            TH1 *hobj = (TH1*)fParent;
            if (axisNumber == 3 && hobj && hobj->GetDimension() != 3) {
               Float_t zmin = hobj->GetMinimum();
               Float_t zmax = hobj->GetMaximum();
               if(gPad->GetLogz()){
                  if (zmin <= 0 && zmax > 0) zmin = TMath::Min((Double_t)1,
                                                               (Double_t)0.001*zmax);
                  zmin = TMath::Log10(zmin);
                  zmax = TMath::Log10(zmax);
               }
               Float_t newmin = zmin + (zmax-zmin)*ratio1;
               Float_t newmax = zmin + (zmax-zmin)*ratio2;
               if(newmin < zmin)newmin = hobj->GetBinContent(hobj->GetMinimumBin());
               if(newmax > zmax)newmax = hobj->GetBinContent(hobj->GetMaximumBin());
               if(gPad->GetLogz()){
                  newmin = TMath::Exp(2.302585092994*newmin);
                  newmax = TMath::Exp(2.302585092994*newmax);
               }
               hobj->SetMinimum(newmin);
               hobj->SetMaximum(newmax);
               hobj->SetBit(TH1::kIsZoomed);
            } else {
               if (fFirst > 0) first = fFirst;
               else            first = 1;
               if (fLast > 0) last = fLast;
               else           last = fNbins;
               bin1 = first + Int_t((last-first+1)*ratio1);
               bin2 = first + Int_t((last-first+1)*ratio2);
               SetRange(bin1, bin2);
            }
            delete view;
            gPad->SetView(0);
            gPad->Modified(kTRUE);
         }
      } else {
         if (axisNumber == 1) {
            ratio2 = (gPad->AbsPixeltoX(px) - gPad->GetUxmin())/(gPad->GetUxmax() - gPad->GetUxmin());
            xmin = gPad->GetUxmin() +ratio1*(gPad->GetUxmax() - gPad->GetUxmin());
            xmax = gPad->GetUxmin() +ratio2*(gPad->GetUxmax() - gPad->GetUxmin());
            if (gPad->GetLogx()) {
               xmin = gPad->PadtoX(xmin);
               xmax = gPad->PadtoX(xmax);
            }
         } else if (axisNumber == 2) {
            ratio2 = (gPad->AbsPixeltoY(py) - gPad->GetUymin())/(gPad->GetUymax() - gPad->GetUymin());
            xmin = gPad->GetUymin() +ratio1*(gPad->GetUymax() - gPad->GetUymin());
            xmax = gPad->GetUymin() +ratio2*(gPad->GetUymax() - gPad->GetUymin());
            if (gPad->GetLogy()) {
               xmin = gPad->PadtoY(xmin);
               xmax = gPad->PadtoY(xmax);
            }
         } else {
            ratio2 = (gPad->AbsPixeltoY(py) - gPad->GetUymin())/(gPad->GetUymax() - gPad->GetUymin());
            xmin = ratio1;
            xmax = ratio2;
         }
         if (xmin > xmax) {
            temp   = xmin;
            xmin   = xmax;
            xmax   = temp;
            temp   = ratio1;
            ratio1 = ratio2;
            ratio2 = temp;
         }
         if (!strcmp(GetName(),"xaxis")) axisNumber = 1;
         if (!strcmp(GetName(),"yaxis")) axisNumber = 2;
         if (ratio2 - ratio1 > 0.05) {
            TH1 *hobj = (TH1*)fParent;
            bin1 = FindFixBin(xmin);
            bin2 = FindFixBin(xmax);
            if (axisNumber == 1) SetRange(bin1,bin2);
            if (axisNumber == 2 && hobj) {
               if (hobj->GetDimension() == 1) {
                  hobj->SetMinimum(xmin);
                  hobj->SetMaximum(xmax);
                  hobj->SetBit(TH1::kIsZoomed);
               } else {
                  SetRange(bin1,bin2);
               }
            }
            gPad->Modified(kTRUE);
         }
      }
      gVirtualX->SetLineColor(-1);
      break;
   }
}

//______________________________________________________________________________
Int_t TAxis::FindBin(Axis_t x)
{
//*-*-*-*-*-*-*-*-*Find bin number corresponding to abscissa x-*-*-*-*-*-*
//*-*              ===========================================
//
// If x is underflow or overflow, attempt to rebin histogram

   Int_t bin;
   if (x < fXmin) {              //*-* underflow
      bin = 0;
      if (fParent == 0) return bin;
      if (!fParent->TestBit(TH1::kCanRebin)) return bin;
      ((TH1*)fParent)->RebinAxis(x,GetName());
      return FindFixBin(x);
   } else  if ( !(x < fXmax)) {     //*-* overflow  (note the way to catch NaN
      bin = fNbins+1;
      if (fParent == 0) return bin;
      if (!fParent->TestBit(TH1::kCanRebin)) return bin;
      ((TH1*)fParent)->RebinAxis(x,GetName());
      return FindFixBin(x);
   } else {
      if (!fXbins.fN) {        //*-* fix bins
         bin = 1 + int (fNbins*(x-fXmin)/(fXmax-fXmin) );
      } else {                  //*-* variable bin sizes
         //for (bin =1; x >= fXbins.fArray[bin]; bin++);
         bin = 1 + TMath::BinarySearch(fXbins.fN,fXbins.fArray,x);
      }
   }
   return bin;
}

//______________________________________________________________________________
Int_t TAxis::FindBin(const char *label)
{
// Find bin number with label.
// If label is not in the list of labels, add it.
// If the number of labels exceeds the number of bins, double the number
// of bins via TH1::LabelsInflate (if the histogram can be rebinned).
//

   //create list of labels if it does not exist yet
   if (!fLabels) {
      if (!fParent) return -1;
      fLabels = new THashList(1,1);
      fParent->SetBit(TH1::kCanRebin);
      if (fXmax <= fXmin) fXmax = fXmin+1;
   }

   // search for label in the existing list
   TObjString *obj = (TObjString*)fLabels->FindObject(label);
   if (obj) return (Int_t)obj->GetUniqueID();

   //Not yet in the list. Can we rebin the histogram ?
   if (!fParent->TestBit(TH1::kCanRebin)) return -1;

   // count number of labels in the list
   Int_t n = 0;
   TIter next(fLabels);
   while ((obj = (TObjString*)next())) {
      n++;
   }
   TH1 *h = (TH1*)fParent;

   //may be we have to resize the histogram (doubling number of channels)
   if (n >= fNbins) h->LabelsInflate(GetName());

   //add new label to the list: assign bin number
   obj = new TObjString(label);
   fLabels->Add(obj);
   obj->SetUniqueID(n+1);
   return n+1;
}

//______________________________________________________________________________
Int_t TAxis::FindFixBin(Axis_t x) const
{
//*-*-*-*-*-*-*-*-*Find bin number corresponding to abscissa x-*-*-*-*-*-*
//*-*              ===========================================
   Int_t bin;
   if (x < fXmin) {              //*-* underflow
      bin = 0;
   } else  if ( !(x < fXmax)) {     //*-* overflow  (note the way to catch NaN
      bin = fNbins+1;
   } else {
      if (!fXbins.fN) {        //*-* fix bins
         bin = 1 + int (fNbins*(x-fXmin)/(fXmax-fXmin) );
      } else {                  //*-* variable bin sizes
//         for (bin =1; x >= fXbins.fArray[bin]; bin++);
         bin = 1 + TMath::BinarySearch(fXbins.fN,fXbins.fArray,x);
      }
   }
   return bin;
}

//______________________________________________________________________________
const char *TAxis::GetBinLabel(Int_t bin) const
{
// Return label for bin

   if (!fLabels) return "";
   if (bin <= 0 || bin > fNbins) return "";
   TIter next(fLabels);
   TObjString *obj;
   while ((obj=(TObjString*)next())) {
      Int_t binid = (Int_t)obj->GetUniqueID();
      if (binid == bin) return obj->GetName();
   }
   return "";
}

//______________________________________________________________________________
Int_t TAxis::GetFirst() const
{
//             return first bin on the axis
//       - 0 if no range defined

   if (!TestBit(kAxisRange)) return 1;
   return fFirst;
}

//______________________________________________________________________________
Int_t TAxis::GetLast() const
{
//             return last bin on the axis
//       - fNbins if no range defined

   if (!TestBit(kAxisRange)) return fNbins;
   return fLast;
}

//______________________________________________________________________________
Axis_t TAxis::GetBinCenter(Int_t bin) const
{
//*-*-*-*-*-*-*-*-*-*-*Return center of bin*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ====================
  Axis_t binwidth;
  if (!fXbins.fN || bin<1 || bin>fNbins) {
     binwidth = (fXmax - fXmin) / Axis_t(fNbins);
     return fXmin + (bin-1) * binwidth + 0.5*binwidth;
  } else {
     binwidth = fXbins.fArray[bin] - fXbins.fArray[bin-1];
     return fXbins.fArray[bin-1] + 0.5*binwidth;
  }
}

//______________________________________________________________________________
Axis_t TAxis::GetBinLowEdge(Int_t bin) const
{
//*-*-*-*-*-*-*-*-*-*-*Return low edge of bin-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ======================

  if (fXbins.fN && bin > 0 && bin <=fNbins) return fXbins.fArray[bin-1];
  Axis_t binwidth = (fXmax - fXmin) / Axis_t(fNbins);
  return fXmin + (bin-1) * binwidth;
}

//______________________________________________________________________________
Axis_t TAxis::GetBinUpEdge(Int_t bin) const
{
//*-*-*-*-*-*-*-*-*-*-*Return up edge of bin-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ======================

  Axis_t binwidth;
  if (!fXbins.fN || bin<1 || bin>fNbins) {
     binwidth = (fXmax - fXmin) / Axis_t(fNbins);
     return fXmin + bin*binwidth;
  } else {
     binwidth = fXbins.fArray[bin] - fXbins.fArray[bin-1];
     return fXbins.fArray[bin-1] + binwidth;
  }
}

//______________________________________________________________________________
Axis_t TAxis::GetBinWidth(Int_t bin) const
{
//*-*-*-*-*-*-*-*-*-*-*Return bin width-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ================
  if (bin <1 ) bin = 1;
  if (bin >fNbins) bin = fNbins;
  if (!fXbins.fN)  return (fXmax - fXmin) / Axis_t(fNbins);
   return fXbins.fArray[bin] - fXbins.fArray[bin-1];
}


//______________________________________________________________________________
void TAxis::GetCenter(Axis_t *center) const
{
//*-*-*-*-*-*-*-*-*Return an array with the center of all bins-*-*-*-*-*-*-*
//*-*              ===========================================
  Int_t bin;
  for (bin=1; bin<=fNbins; bin++) *(center + bin-1) = GetBinCenter(bin);
}

//______________________________________________________________________________
void TAxis::GetLowEdge(Axis_t *edge) const
{
//*-*-*-*-*-*-*-*-*Return an array with the lod edge of all bins-*-*-*-*-*-*-*
//*-*              =============================================
  Int_t bin;
  for (bin=1; bin<=fNbins; bin++) *(edge + bin-1) = GetBinLowEdge(bin);
}

//______________________________________________________________________________
const char *TAxis::GetTimeFormatOnly() const
{
   // Return *only* the time format from the string fTimeFormat
   static TString timeformat;
   Int_t IdF = fTimeFormat.Index("%F");
   if (IdF>=0) {
      timeformat = fTimeFormat(0,IdF);
   } else {
      timeformat = fTimeFormat;
   }
   return timeformat.Data();
}

//______________________________________________________________________________
const char *TAxis::GetTicks() const
{
// return the ticks option (see SetTicks)

   if (TestBit(kTickPlus) && TestBit(kTickMinus)) return "+-";
   if (TestBit(kTickMinus)) return "-";
   return "+";
}

//______________________________________________________________________________
void TAxis::LabelsOption(Option_t *option)
{
//  Set option(s) to draw axis with labels
//  option = "a" sort by alphabetic order
//         = ">" sort by decreasing values
//         = "<" sort by increasing values
//         = "h" draw labels horizonthal
//         = "v" draw labels vertical
//         = "u" draw labels up (end of label right adjusted)
//         = "d" draw labels down (start of label left adjusted)


   if (!fLabels) {
      Warning("Sort","Cannot sort. No labels");
      return;
   }
   TH1 *h = (TH1*)GetParent();
   if (!h) {
      Error("Sort","Axis has no parent");
      return;
   }

   h->LabelsOption(option,GetName());
}

//___________________________________________________________________________
Bool_t TAxis::GetRotateTitle() const
{
   // Return kTRUE if kRotateTitle bit is set, kFALSE otherwise.

   return TestBit(kRotateTitle) ? kTRUE : kFALSE;
}

//______________________________________________________________________________
void TAxis::ImportAttributes(const TAxis *axis)
{
// Copy axis attributes to this 

   SetTitle(axis->GetTitle());
   SetNdivisions(axis->GetNdivisions());
   SetAxisColor(axis->GetAxisColor());
   SetLabelColor(axis->GetLabelColor());
   SetLabelFont(axis->GetLabelFont());
   SetLabelOffset(axis->GetLabelOffset());
   SetLabelSize(axis->GetLabelSize());
   SetTickLength(axis->GetTickLength());
   SetTitleOffset(axis->GetTitleOffset());
   SetTitleSize(axis->GetTitleSize());
   SetTitleColor(axis->GetTitleColor());
   SetTitleFont(axis->GetTitleFont());
   SetBit(TAxis::kCenterTitle,   axis->TestBit(TAxis::kCenterTitle));
   SetBit(TAxis::kCenterLabels,  axis->TestBit(TAxis::kCenterLabels));
   SetBit(TAxis::kRotateTitle,   axis->TestBit(TAxis::kRotateTitle));
   SetBit(TAxis::kNoExponent,    axis->TestBit(TAxis::kNoExponent));
   SetBit(TAxis::kTickPlus,      axis->TestBit(TAxis::kTickPlus));
   SetBit(TAxis::kTickMinus,     axis->TestBit(TAxis::kTickMinus));
   SetBit(TAxis::kMoreLogLabels, axis->TestBit(TAxis::kMoreLogLabels));
   if (axis->GetDecimals())      SetBit(TAxis::kDecimals); //the bit is in TAxis::fAxis2   
   SetTimeFormat(axis->GetTimeFormat());
}

//___________________________________________________________________________
void TAxis::RotateTitle(Bool_t rotate)
{
//    rotate title by 180 degrees
//    by default the title is drawn right adjusted.
//    if rotate is TRUE, the title is left adjusted at the end of the axis
//    and rotated by 180 degrees

   if (rotate) SetBit(kRotateTitle);
   else        ResetBit(kRotateTitle);
}

//______________________________________________________________________________
void TAxis::SaveAttributes(ofstream &out, const char *name, const char *subname)
{
    // Save axis attributes as C++ statement(s) on output stream out

   char quote = '"';
   if (strlen(GetTitle())) {
      out<<"   "<<name<<subname<<"->SetTitle("<<quote<<GetTitle()<<quote<<");"<<endl;
   }
   if (fTimeDisplay) {
      out<<"   "<<name<<subname<<"->SetTimeDisplay(1);"<<endl;
      out<<"   "<<name<<subname<<"->SetTimeFormat("<<quote<<GetTimeFormat()<<quote<<");"<<endl;
   }
   if (fLabels) {
      TIter next(fLabels);
      TObjString *obj;
      while ((obj=(TObjString*)next())) {
         out<<"   "<<name<<subname<<"->SetBinLabel("<<obj->GetUniqueID()<<","<<quote<<obj->GetName()<<quote<<");"<<endl;
      }
   }

   if (fFirst || fLast) {
      out<<"   "<<name<<subname<<"->SetRange("<<fFirst<<","<<fLast<<");"<<endl;
   }

   if (TestBit(kLabelsHori)) {
      out<<"   "<<name<<subname<<"->SetBit(TAxis::kLabelsHori);"<<endl;
   }

   if (TestBit(kLabelsVert)) {
      out<<"   "<<name<<subname<<"->SetBit(TAxis::kLabelsVert);"<<endl;
   }

   if (TestBit(kLabelsDown)) {
      out<<"   "<<name<<subname<<"->SetBit(TAxis::kLabelsDown);"<<endl;
   }

   if (TestBit(kLabelsUp)) {
      out<<"   "<<name<<subname<<"->SetBit(TAxis::kLabelsUp);"<<endl;
   }

   if (TestBit(kCenterTitle)) {
      out<<"   "<<name<<subname<<"->CenterTitle(true);"<<endl;
   }

   if (TestBit(kRotateTitle)) {
      out<<"   "<<name<<subname<<"->RotateTitle(true);"<<endl;
   }

   if (TestBit(kMoreLogLabels)) {
      out<<"   "<<name<<subname<<"->SetMoreLogLabels();"<<endl;
   }

   if (TestBit(kNoExponent)) {
      out<<"   "<<name<<subname<<"->SetNoExponent();"<<endl;
   }

   TAttAxis::SaveAttributes(out,name,subname);
}

//______________________________________________________________________________
void TAxis::Set(Int_t nbins, Axis_t xlow, Axis_t xup)
{
//*-*-*-*-*-*-*-*-*Initialize axis with fix bins*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*              =============================
   fNbins   = nbins;
   fXmin    = xlow;
   fXmax    = xup;
   SetDefaults();
}

//______________________________________________________________________________
void TAxis::Set(Int_t nbins, const Float_t *xbins)
{
//*-*-*-*-*-*-*-*-*Initialize axis with variable bins*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*              ==================================
   Int_t bin;
   fNbins  = nbins;
   fXbins.Set(fNbins+1);
   for (bin=0; bin<= fNbins; bin++)
      fXbins.fArray[bin] = xbins[bin];
   for (bin=1; bin<= fNbins; bin++)
      if (fXbins.fArray[bin] < fXbins.fArray[bin-1])
         Error("TAxis::Set", "bins must be in increasing order");
   fXmin      = fXbins.fArray[0];
   fXmax      = fXbins.fArray[fNbins];
   SetDefaults();
}

//______________________________________________________________________________
void TAxis::Set(Int_t nbins, const Axis_t *xbins)
{
//*-*-*-*-*-*-*-*-*Initialize axis with variable bins*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*              ==================================
   Int_t bin;
   fNbins  = nbins;
   fXbins.Set(fNbins+1);
   for (bin=0; bin<= fNbins; bin++)
      fXbins.fArray[bin] = xbins[bin];
   for (bin=1; bin<= fNbins; bin++)
      if (fXbins.fArray[bin] < fXbins.fArray[bin-1])
         Error("TAxis::Set", "bins must be in increasing order");
   fXmin      = fXbins.fArray[0];
   fXmax      = fXbins.fArray[fNbins];
   SetDefaults();
}

//______________________________________________________________________________
void TAxis::SetDefaults()
{
// Set axis default values (from TStyle)
   
   fFirst   = 0;
   fLast    = 0;
   fBits2   = 0;
   char name[64];
   sprintf(name,"%s%s",GetName(),"x");
   TAttAxis::ResetAttAxis(name);
   fTimeDisplay = 0;
   SetTimeFormat();
}

//______________________________________________________________________________
Bool_t TAxis::GetDecimals() const
{
   // Returns kTRUE if kDecimals bit is set, kFALSE otherwise.
   // see TAxis::SetDecimals
   
   if ((fBits2 & kDecimals) != 0) return kTRUE;
   else                           return kFALSE;
}


//______________________________________________________________________________
void TAxis::SetDecimals(Bool_t dot)
{
// Set the Decimals flag
// By default, blank characters are stripped, and then the
// label is correctly aligned. The dot, if last character of the string, 
// is also stripped, unless this option is specified.
// One can disable the option by calling axis.SetDecimals(kTRUE).
// The flag (in fBits2) is passed to the drawing function TGaxis::PaintAxis

   if (dot) fBits2 |=  kDecimals;
   else     fBits2 &= ~kDecimals;
}

//______________________________________________________________________________
void TAxis::SetBinLabel(Int_t bin, const char *label)
{
// Set label for bin

   if (!fLabels) fLabels = new THashList(fNbins,3);
   if (bin <= 0 || bin > fNbins) {
      Error("SetBinLabel","Illegal bin number: %d",bin);
      return;
   }

   // Check whether this bin already has a label.
   TIter next(fLabels);
   TObjString *obj;
   while ((obj=(TObjString*)next()))
     if ( obj->GetUniqueID()==(UInt_t)bin ) {
       // It does. Overwrite it.
       obj->SetString(label);
       return;
     }
   // It doesn't. Add this new label.
   obj = new TObjString(label);
   fLabels->Add(obj);
   obj->SetUniqueID((UInt_t)bin);
}

//______________________________________________________________________________
void TAxis::SetLimits(Axis_t xmin, Axis_t xmax)
{
//          Set the axis limits

   fXmin = xmin;
   fXmax = xmax;
}

//______________________________________________________________________________
Bool_t TAxis::GetMoreLogLabels() const
{
   // Return kTRUE if kMoreLogLabels bit is set, kFALSE otherwise.

   return TestBit(kMoreLogLabels) ? kTRUE : kFALSE;
}

//______________________________________________________________________________
void TAxis::SetMoreLogLabels(Bool_t more)
{
// Set the kMoreLogLabels bit flag
// When this option is selected more labels are drawn when in log scale
// and there is a small number of decades  (<3).
// The flag (in fBits) is passed to the drawing function TGaxis::PaintAxis

   if (more) SetBit(kMoreLogLabels);
   else      ResetBit(kMoreLogLabels);
}

//______________________________________________________________________________
Bool_t TAxis::GetNoExponent() const
{
   // Returns kTRUE if kNoExponent bit is set, kFALSE otherwise.
   // see TAxis::SetNoExponent

   return TestBit(kNoExponent) ? kTRUE : kFALSE;
}

//______________________________________________________________________________
void TAxis::SetNoExponent(Bool_t noExponent)
{
// Set the NoExponent flag
// By default, an exponent of the form 10^N is used when the label values
// are either all very small or very large.
// One can disable the exponent by calling axis.SetNoExponent(kTRUE).
// The flag (in fBits) is passed to the drawing function TGaxis::PaintAxis

   if (noExponent) SetBit(kNoExponent);
   else            ResetBit(kNoExponent);
}

//______________________________________________________________________________
void TAxis::SetRange(Int_t first, Int_t last)
{
//  Set the viewing range for the axis from bin first to last
//  To set a range using the axis coordinates, use TAxis::SetRangeUser.

   if (last == 0) last = fNbins;
   if (last > fNbins) last = fNbins;
   if (last  < first) first = 1;
   if (first < 1)     first = 1;
   if (first == 1 && last == fNbins) {
      SetBit(kAxisRange,0);
      fFirst = 0;
      fLast  = 0;
   } else {
      SetBit(kAxisRange,1);
      fFirst = first;
      fLast  = last;
   }
}


//______________________________________________________________________________
void TAxis::SetRangeUser(Axis_t ufirst, Axis_t ulast)
{
//  Set the viewing range for the axis from ufirst to ulast (in user coordinates)
//  To set a range using the axis bin numbers, use TAxis::SetRange.

   if (!strstr(GetName(),"xaxis")) {
      TH1 *hobj = (TH1*)GetParent();
      if ((hobj->GetDimension() == 2 && strstr(GetName(),"zaxis")) 
       || (hobj->GetDimension() == 1 && strstr(GetName(),"yaxis"))) {
         hobj->SetMinimum(ufirst);
         hobj->SetMaximum(ulast);
         return;
      }
   }
   SetRange(FindBin(ufirst),FindBin(ulast));
}

//______________________________________________________________________________
void TAxis::SetTicks(Option_t *option)
{
//  set ticks orientation
//  option = "+"  ticks drawn on the "positive side" (default)
//  option = "-"  ticks drawn on the "negative side"
//  option = "+-" ticks drawn on both sides

   ResetBit(kTickPlus);
   ResetBit(kTickMinus);
   if (strchr(option,'+')) SetBit(kTickPlus);
   if (strchr(option,'-')) SetBit(kTickMinus);
}

//______________________________________________________________________________
void TAxis::SetTimeFormat(const char *tformat)
{
//*-*-*-*-*-*-*-*-*-*-*Change the format used for time plotting *-*-*-*-*-*-*-*
//*-*                  ========================================
//  The format string for date and time use the same options as the one used
//  in the standard strftime C function, i.e. :
//    for date :
//      %a abbreviated weekday name
//      %b abbreviated month name
//      %d day of the month (01-31)
//      %m month (01-12)
//      %y year without century
//
//    for time :
//      %H hour (24-hour clock)
//      %I hour (12-hour clock)
//      %p local equivalent of AM or PM
//      %M minute (00-59)
//      %S seconds (00-61)
//      %% %
//
//    This function allows also to define the time offset. It is done via %F
//    which should be appended at the end of the format string. The time
//    offset has the following format: 'yyyy-mm-dd hh:mm:ss'
//    Example:
//
//          h = new TH1F("Test","h",3000,0.,200000.);
//          h->GetXaxis()->SetTimeDisplay(1);
//          h->GetXaxis()->SetTimeFormat("%d\/%m\/%y%F2000-02-28 13:00:01");
//
//    This defines the time format being "dd/mm/yy" and the time offset as the
//    February 28th 2003 at 13:00:01
//
//    If %F is not specified, the time offset used will be the one defined by:
//    gStyle->SetTimeOffset. For example like that:
//
//          TDatime da(2003,02,28,12,00,00);
//          gStyle->SetTimeOffset(da.Convert());
//
   TString timeformat = tformat;

   if (timeformat.Index("%F")>=0 || timeformat.IsNull()) {
      fTimeFormat = timeformat;
      return;
   }

   Int_t IdF = fTimeFormat.Index("%F");
   if (IdF>=0) {
      Int_t LnF = fTimeFormat.Length();
      TString stringtimeoffset = fTimeFormat(IdF,LnF);
      fTimeFormat = tformat;
      fTimeFormat.Append(stringtimeoffset);
   } else {
      fTimeFormat = tformat;
      SetTimeOffset(gStyle->GetTimeOffset());
   }
}


//______________________________________________________________________________
void TAxis::SetTimeOffset(Double_t toffset, Option_t *option)
{
   // Change the time offset
   // If option = "gmt" the time offset is treated as a GMT time.

   TString opt = option;
   opt.ToLower();

   Bool_t gmt = kFALSE;
   if (opt.Contains("gmt")) gmt = kTRUE;

   char tmp[20];
   time_t timeoff;
   struct tm* utctis;
   Int_t IdF = fTimeFormat.Index("%F");
   if (IdF>=0) fTimeFormat.Remove(IdF);
   fTimeFormat.Append("%F");

   timeoff = (time_t)((Long_t)(toffset));
   utctis = gmtime(&timeoff);

   strftime(tmp,256,"%Y-%m-%d %H:%M:%S",utctis);
   fTimeFormat.Append(tmp);

   // append the decimal part of the time offset
   Double_t ds = toffset-(Int_t)toffset;
   if(ds!= 0) {
      sprintf(tmp,"s%g",ds);
      fTimeFormat.Append(tmp);
   }

   // If the time is GMT, stamp fTimeFormat
   if (gmt) fTimeFormat.Append(" GMT");
}


//______________________________________________________________________________
void TAxis::Streamer(TBuffer &R__b)
{
   // Stream an object of class TAxis.

   if (R__b.IsReading()) {
      UInt_t R__s, R__c;
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c);
      if (R__v > 5) {
         TAxis::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
         return;
      }
      //====process old versions before automatic schema evolution
      TNamed::Streamer(R__b);
      TAttAxis::Streamer(R__b);
      R__b >> fNbins;
      if (R__v < 5) {
         Float_t xmin,xmax;
         R__b >> xmin; fXmin = xmin;
         R__b >> xmax; fXmax = xmax;
         Float_t *xbins = 0;
         Int_t n = R__b.ReadArray(xbins);
         fXbins.Set(n);
         for (Int_t i=0;i<n;i++) fXbins.fArray[i] = xbins[i];
         delete [] xbins;
      } else {
         R__b >> fXmin;
         R__b >> fXmax;
         fXbins.Streamer(R__b);
      }
      if (R__v > 2) {
         R__b >> fFirst;
         R__b >> fLast;
          // following lines required to repair for a bug in Root version 1.03
         if (fFirst < 0 || fFirst > fNbins) fFirst = 0;
         if (fLast  < 0 || fLast  > fNbins) fLast  = 0;
         if (fLast  < fFirst) { fFirst = 0; fLast = 0;}
         if (fFirst ==0 && fLast == 0) SetBit(kAxisRange,0);
      }
      if (R__v > 3) {
         R__b >> fTimeDisplay;
         fTimeFormat.Streamer(R__b);
      } else {
         SetTimeFormat();
      }
      R__b.CheckByteCount(R__s, R__c, TAxis::IsA());
      //====end of old versions

   } else {
      TAxis::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
void TAxis::UnZoom()
{
   // Reset first & last bin to the full range

   TView *view = gPad->GetView();
   if (view) {
      delete view;
      gPad->SetView(0);
   }
   SetRange(0,0);
   if (!strstr(GetName(),"xaxis")) {
      TH1 *hobj = (TH1*)GetParent();
      if (hobj->GetDimension() == 2 && strstr(GetName(),"zaxis")) {
         hobj->SetMinimum();
         hobj->SetMaximum();
         hobj->ResetBit(TH1::kIsZoomed);
         return;
      }
      if (strcmp(hobj->GetName(),"hframe") == 0 ) {
         hobj->SetMinimum(fXmin);
         hobj->SetMaximum(fXmax);
      } else {
         hobj->SetMinimum();
         hobj->SetMaximum();
         hobj->ResetBit(TH1::kIsZoomed);
      }
   }
}
