// @(#)root/graf:$Name:  $:$Id: TLatex.h,v 1.11 2003/05/08 16:55:25 brun Exp $
// Author: Nicolas Brun   07/08/98

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/
#ifndef ROOT_TLatex
#define ROOT_TLatex

#ifndef ROOT_Riosfwd
#include "Riosfwd.h"
#endif
#ifndef ROOT_TText
#include "TText.h"
#endif
#ifndef ROOT_TAttLine
#include "TAttLine.h"
#endif
#ifndef ROOT_TMath
#include "TMath.h"
#endif

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLatex                                                               //
//                                                                      //
// The Latex-style text processor class                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


struct FormSize_t {
      Double_t width,dessus,dessous;
};

struct TextSpec_t {
   Double_t angle,size;
   Int_t color,font;
};

// compute size of a portion of a formula
class FormSize {
private:
      Double_t width, dessus, dessous;
public:
      FormSize() { width=0; dessus=0; dessous=0; } // constructeur par defaut
      FormSize(Double_t x, Double_t y1, Double_t y2) { width=x; dessus=y1; dessous=y2; } // constructeur
      virtual ~FormSize() {} //destructeur
      FormSize(const FormSize& form) {width=form.width; dessus=form.dessus; dessous=form.dessous;}
      
      // definition of operators + and +=
      FormSize operator+(FormSize f)
         { return FormSize(f.Width()+width,TMath::Max(f.Dessus(),dessus),TMath::Max(f.Dessous(),dessous)); }
      void operator+=(FormSize f)
         { width += f.Width(); dessus = TMath::Max(dessus,f.Dessus()); dessous = TMath::Max(dessous,f.Dessous()); }

      inline void Set(Double_t x, Double_t y1, Double_t y2) { width=x; dessus=y1; dessous=y2; }
      FormSize add_Dessus(FormSize f)
         { return FormSize(f.Width()+width,f.Height()+dessus,dessous); }
      FormSize add_Dessous(FormSize f)
         { return FormSize(f.Width()+width,dessus,f.Height()+dessous); }
      FormSize add_DessusDessous(FormSize f1, FormSize f2)
         { return FormSize(width+TMath::Max(f1.Width(),f2.Width()),dessus+f1.Dessus(),dessous+f2.Dessous()); }

      // return members
      inline Double_t Width()   { return width; }
      inline Double_t Dessus()  { return dessus; }
      inline Double_t Dessous() { return dessous; }
      inline Double_t Height()  { return dessus+dessous; }
};

class TLatex : public TText, public TAttLine {
protected:
      Double_t      fFactorSize;      //!Relative size of subscripts and superscripts
      Double_t      fFactorPos;       //!Relative position of subscripts and superscripts
      Int_t         fLimitFactorSize; // lower bound for subscripts/superscripts size
      const Char_t *fError;           //!error code
      Bool_t        fShow;            //!is true during the second pass (Painting)
      FormSize_t   *fTabSize;         //!array of values for the different zones
      Double_t      fOriginSize;      // Font size of the starting font
      Int_t         fTabMax;          //!Maximum allocation for array fTabSize;
      Int_t         fPos;             //!Current position in array fTabSize;

      //Text analysis and painting
      FormSize Analyse(Double_t x, Double_t y, TextSpec_t spec, const Char_t* t,Int_t length);
      FormSize Anal1(TextSpec_t spec, const Char_t* t,Int_t length);

      void DrawLine(Double_t x1, Double_t y1, Double_t x2, Double_t y2, TextSpec_t spec);
      void DrawCircle(Double_t x1, Double_t y1, Double_t r, TextSpec_t spec);
      void DrawParenthesis(Double_t x1, Double_t y1, Double_t r1, Double_t r2, Double_t phimin, Double_t phimax, TextSpec_t spec);

      FormSize FirstParse(Double_t angle, Double_t size, const Char_t *text);

      void Savefs(FormSize *fs);
      FormSize Readfs();

      Int_t CheckLatexSyntax(TString &text) ;

public:
      // TLatex status bits
      enum { kTextNDC = BIT(14) };

      TLatex();
      TLatex(Double_t x, Double_t y, const char *text);
      TLatex(const TLatex &text);
      virtual ~TLatex();
      void             Copy(TObject &text) const;

      TLatex          *DrawLatex(Double_t x, Double_t y, const char *text);
      Double_t         GetHeight() const;
      Double_t         GetXsize();
      Double_t         GetYsize();
      void             GetBoundingBox(UInt_t &w, UInt_t &h);
      virtual void     Paint(Option_t *option="");
      virtual void     PaintLatex(Double_t x, Double_t y, Double_t angle, Double_t size, const char *text);

      virtual void     SavePrimitive(ofstream &out, Option_t *option);
      virtual void     SetIndiceSize(Double_t factorSize);
      virtual void     SetLimitIndiceSize(Int_t limitFactorSize);

      ClassDef(TLatex,2)  //The Latex-style text processor class
};

#endif
