// @(#)root/graf:$Name:  $:$Id: TPolyLine.h,v 1.5 2002/10/31 07:27:34 brun Exp $
// Author: Rene Brun   12/12/94

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TPolyLine
#define ROOT_TPolyLine


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TPolyLine                                                            //
//                                                                      //
// A PolyLine.                                                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef ROOT_TString
#include "TString.h"
#endif
#ifndef ROOT_TObject
#include "TObject.h"
#endif
#ifndef ROOT_TAttLine
#include "TAttLine.h"
#endif
#ifndef ROOT_TAttFill
#include "TAttFill.h"
#endif

class TCollection;

class TPolyLine : public TObject, public TAttLine, public TAttFill {

protected:
        Int_t        fN;            //Number of points
        Int_t        fLastPoint;    //The index of the last filled point
        Double_t    *fX;            //[fN] Array of X coordinates
        Double_t    *fY;            //[fN] Array of Y coordinates
        TString      fOption;       //options

public:
        TPolyLine();
        TPolyLine(Int_t n, Option_t *option="");
        TPolyLine(Int_t n, Float_t *x, Float_t *y, Option_t *option="");
        TPolyLine(Int_t n, Double_t *x, Double_t *y, Option_t *option="");
        TPolyLine(const TPolyLine &polyline);
        virtual ~TPolyLine();
        virtual void    Copy(TObject &polyline) const;
        virtual Int_t   DistancetoPrimitive(Int_t px, Int_t py);
        virtual void    Draw(Option_t *option="");
        virtual void    DrawPolyLine(Int_t n, Double_t *x, Double_t *y, Option_t *option="");
        virtual void    ExecuteEvent(Int_t event, Int_t px, Int_t py);
        virtual Int_t   GetLastPoint() const { return fLastPoint;}
        Int_t           GetN() const {return fN;}
        Double_t       *GetX() const {return fX;}
        Double_t       *GetY() const {return fY;}
        Option_t        *GetOption() const {return fOption.Data();}
        virtual void    ls(Option_t *option="") const;
        virtual Int_t   Merge(TCollection *list);
        virtual void    Paint(Option_t *option="");
        virtual void    PaintPolyLine(Int_t n, Double_t *x, Double_t *y, Option_t *option="");
        virtual void    PaintPolyLineNDC(Int_t n, Double_t *x, Double_t *y, Option_t *option="");
        virtual void    Print(Option_t *option="") const;
        virtual void    SavePrimitive(ofstream &out, Option_t *option);
        virtual Int_t   SetNextPoint(Double_t x, Double_t y); // *MENU*
        virtual void    SetOption(Option_t *option="") {fOption = option;}
        virtual void    SetPoint(Int_t point, Double_t x, Double_t y); // *MENU*
        virtual void    SetPolyLine(Int_t n);
        virtual void    SetPolyLine(Int_t n, Float_t *x, Float_t *y, Option_t *option="");
        virtual void    SetPolyLine(Int_t n, Double_t *x, Double_t *y3, Option_t *option="");
        virtual Int_t   Size() const {return fLastPoint+1;}

        ClassDef(TPolyLine,3)  //A PolyLine
};

#endif

