// @(#)root/hist:$Name:  $:$Id: TProfile.h,v 1.29 2005/03/23 12:41:01 brun Exp $
// Author: Rene Brun   29/09/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TProfile
#define ROOT_TProfile


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TProfile                                                             //
//                                                                      //
// Profile histogram class.                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TH1
#include "TH1.h"
#endif

enum EErrorType { kERRORMEAN = 0, kERRORSPREAD, kERRORSPREADI, kERRORSPREADG };

class TF1;

class TProfile : public TH1D {

protected:
    TArrayD     fBinEntries;      //number of entries per bin
    EErrorType  fErrorMode;       //Option to compute errors
    Double_t    fYmin;            //Lower limit in Y (if set)
    Double_t    fYmax;            //Upper limit in Y (if set)
    Bool_t      fScaling;         //!True when TProfile::Scale is called
    Stat_t      fTsumwy;          //Total Sum of weight*Y
    Stat_t      fTsumwy2;         //Total Sum of weight*Y*Y
static Bool_t   fgApproximate;    //bin error approximation option

   virtual Int_t    BufferFill(Axis_t, Stat_t) {return -2;} //may not use
   virtual Int_t    BufferFill(Axis_t x, Axis_t y, Stat_t w);

private:
   Int_t Fill(Axis_t) { MayNotUse("Fill(Axis_t)"); return -1;}
   void FillN(Int_t, const Axis_t *, const Double_t *, Int_t) { MayNotUse("FillN(Int_t, Axis_t*, Double_t*, Int_t)"); }
   void SetBins(Int_t, Axis_t, Axis_t, Int_t, Axis_t, Axis_t)
      { MayNotUse("SetBins(Int_t, Axis_t, Axis_t, Int_t, Axis_t, Axis_t"); }
   void SetBins(Int_t, const Axis_t*, Int_t, const Axis_t*)
      { MayNotUse("SetBins(Int_t, const Axis_t*, Int_t, const Axis_t*"); }
   void SetBins(Int_t, Axis_t, Axis_t, Int_t, Axis_t, Axis_t, Int_t, Axis_t, Axis_t)
      { MayNotUse("SetBins(Int_t, Axis_t, Axis_t, Int_t, Axis_t, Axis_t, Int_t, Axis_t, Axis_t"); }
   Double_t *GetB()  {return &fBinEntries.fArray[0];}
   Double_t *GetW()  {return &fArray[0];}
   Double_t *GetW2() {return &fSumw2.fArray[0];}

public:
    TProfile();
    TProfile(const char *name,const char *title,Int_t nbinsx,Axis_t xlow,Axis_t xup, Option_t *option="");
    TProfile(const char *name,const char *title,Int_t nbinsx,Axis_t xlow,Axis_t xup,Axis_t ylow,Axis_t yup,Option_t *option="");
    TProfile(const char *name,const char *title,Int_t nbinsx,const Float_t  *xbins, Option_t *option="");
    TProfile(const char *name,const char *title,Int_t nbinsx,const Double_t *xbins, Option_t *option="");
    TProfile(const char *name,const char *title,Int_t nbinsx,const Double_t *xbins, Axis_t ylow,Axis_t yup, Option_t *option="");
    TProfile(const TProfile &profile);
    virtual ~TProfile();
    virtual void    Add(TF1 *h1, Double_t c1=1, Option_t *option="");
    virtual void    Add(const TH1 *h1, Double_t c1=1);
    virtual void    Add(const TH1 *h1, const TH1 *h2, Double_t c1=1, Double_t c2=1); // *MENU*
    static  void    Approximate(Bool_t approx=kTRUE);
    virtual Int_t   BufferEmpty(Int_t action=0);
            void    BuildOptions(Double_t ymin, Double_t ymax, Option_t *option);
    virtual void    Copy(TObject &hnew) const;
    virtual void    Divide(TF1 *h1, Double_t c1=1);
    virtual void    Divide(const TH1 *h1);
    virtual void    Divide(const TH1 *h1, const TH1 *h2, Double_t c1=1, Double_t c2=1, Option_t *option=""); // *MENU*
    virtual TH1    *DrawCopy(Option_t *option="") const;
    virtual Int_t   Fill(Axis_t x, Axis_t y);
    virtual Int_t   Fill(const char *namex, Axis_t y);
    virtual Int_t   Fill(Axis_t x, Axis_t y, Stat_t w);
    virtual Int_t   Fill(const char *namex, Axis_t y, Stat_t w);
    virtual void    FillN(Int_t ntimes, const Axis_t *x, const Axis_t *y, const Double_t *w, Int_t stride=1);
    virtual Stat_t  GetBinContent(Int_t bin) const;
    virtual Stat_t  GetBinContent(Int_t bin, Int_t) const {return GetBinContent(bin);}
    virtual Stat_t  GetBinContent(Int_t bin, Int_t, Int_t) const {return GetBinContent(bin);}
    virtual Stat_t  GetBinError(Int_t bin) const;
    virtual Stat_t  GetBinError(Int_t bin, Int_t) const {return GetBinError(bin);}
    virtual Stat_t  GetBinError(Int_t bin, Int_t, Int_t) const {return GetBinError(bin);}
    virtual Stat_t  GetBinEntries(Int_t bin) const;
    Option_t       *GetErrorOption() const;
    virtual void    GetStats(Stat_t *stats) const;
    virtual Double_t GetYmin() const {return fYmin;}
    virtual Double_t GetYmax() const {return fYmax;}
    virtual void    LabelsDeflate(Option_t *axis="X");
    virtual void    LabelsInflate(Option_t *axis="X");
    virtual void    LabelsOption(Option_t *option="h", Option_t *axis="X");
    virtual Long64_t Merge(TCollection *list);
    virtual void    Multiply(TF1 *h1, Double_t c1=1);
    virtual void    Multiply(const TH1 *h1);
    virtual void    Multiply(const TH1 *h1, const TH1 *h2, Double_t c1=1, Double_t c2=1, Option_t *option=""); // *MENU*
            TH1D   *ProjectionX(const char *name="_px", Option_t *option="e") const;
            TH1    *Rebin(Int_t ngroup=2, const char*newname="");
    virtual void    RebinAxis(Axis_t x, const char *ax);
    virtual void    Reset(Option_t *option="");
    virtual void    SavePrimitive(ofstream &out, Option_t *option);
    virtual void    Scale(Double_t c1=1);
    virtual void    SetBinEntries(Int_t bin, Stat_t w);
    virtual void    SetBins(Int_t nbins, Axis_t xmin, Axis_t xmax);
    virtual void    SetBins(Int_t nx, const Axis_t *xbins);
    virtual void    SetBuffer(Int_t buffersize, Option_t *option="");
    virtual void    SetErrorOption(Option_t *option=""); // *MENU*

    ClassDef(TProfile,4)  //Profile histogram class
};

#endif

