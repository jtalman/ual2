/* @(#)root/hist:$Name:  $:$Id: LinkDef.h,v 1.16 2004/01/23 23:30:31 brun Exp $ */

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ enum EErrorType;

#pragma link C++ class TAxis-;
#pragma link C++ class TFormula-;
#pragma link C++ class TFractionFitter+;
#pragma link C++ class TF1-;
#pragma link C++ class TF12+;
#pragma link C++ class TF2-;
#pragma link C++ class TF3+;
#pragma link C++ class TGraph2D-;
#pragma link C++ class TGraph2DErrors-;
#pragma link C++ class TGraphDelaunay+;
#pragma link C++ class TH1-;
#pragma link C++ class TH1C+;
#pragma link C++ class TH1D+;
#pragma link C++ class TH1F+;
#pragma link C++ class TH1S+;
#pragma link C++ class TH1I+;
#pragma link C++ class TH1K+;
#pragma link C++ class TH2-;
#pragma link C++ class TH2C-;
#pragma link C++ class TH2D-;
#pragma link C++ class TH2F-;
#pragma link C++ class TH2S-;
#pragma link C++ class TH2I+;
#pragma link C++ class TH3-;
#pragma link C++ class TH3C-;
#pragma link C++ class TH3D-;
#pragma link C++ class TH3F-;
#pragma link C++ class TH3S-;
#pragma link C++ class TH3I+;
#pragma link C++ class THLimitsFinder+;
#pragma link C++ class THStack+;
#pragma link C++ class TLimit+;
#pragma link C++ class TLimitDataSource+;
#pragma link C++ class TConfidenceLevel+;
#pragma link C++ class TMultiDimFit+;
#pragma link C++ class TPolyMarker-;
#pragma link C++ class TPrincipal+;
#pragma link C++ class TProfile-;
#pragma link C++ class TProfile2D-;
#pragma link C++ class TSpectrum+;
#pragma link C++ class TUtilHist;
#pragma link C++ class TVirtualHistPainter+;

#pragma link C++ function operator*(Float_t,TH1C&);
#pragma link C++ function operator*(TH1C&, Float_t);
#pragma link C++ function operator+(TH1C&, TH1C&);
#pragma link C++ function operator-(TH1C&, TH1C&);
#pragma link C++ function operator*(TH1C&, TH1C&);
#pragma link C++ function operator/(TH1C&, TH1C&);

#pragma link C++ function operator*(Float_t,TH1S&);
#pragma link C++ function operator*(TH1S&, Float_t);
#pragma link C++ function operator+(TH1S&, TH1S&);
#pragma link C++ function operator-(TH1S&, TH1S&);
#pragma link C++ function operator*(TH1S&, TH1S&);
#pragma link C++ function operator/(TH1S&, TH1S&);

#pragma link C++ function operator*(Float_t,TH1I&);
#pragma link C++ function operator*(TH1I&, Float_t);
#pragma link C++ function operator+(TH1I&, TH1I&);
#pragma link C++ function operator-(TH1I&, TH1I&);
#pragma link C++ function operator*(TH1I&, TH1I&);
#pragma link C++ function operator/(TH1I&, TH1I&);

#pragma link C++ function operator*(Float_t,TH1F&);
#pragma link C++ function operator*(TH1F&, Float_t);
#pragma link C++ function operator+(TH1F&, TH1F&);
#pragma link C++ function operator-(TH1F&, TH1F&);
#pragma link C++ function operator*(TH1F&, TH1F&);
#pragma link C++ function operator/(TH1F&, TH1F&);

#pragma link C++ function operator*(Float_t,TH1D&);
#pragma link C++ function operator*(TH1D&, Float_t);
#pragma link C++ function operator+(TH1D&, TH1D&);
#pragma link C++ function operator-(TH1D&, TH1D&);
#pragma link C++ function operator*(TH1D&, TH1D&);
#pragma link C++ function operator/(TH1D&, TH1D&);

#pragma link C++ function operator*(Float_t,TH2C&);
#pragma link C++ function operator*(TH2C&, Float_t);
#pragma link C++ function operator+(TH2C&, TH2C&);
#pragma link C++ function operator-(TH2C&, TH2C&);
#pragma link C++ function operator*(TH2C&, TH2C&);
#pragma link C++ function operator/(TH2C&, TH2C&);

#pragma link C++ function operator*(Float_t,TH2S&);
#pragma link C++ function operator*(TH2S&, Float_t);
#pragma link C++ function operator+(TH2S&, TH2S&);
#pragma link C++ function operator-(TH2S&, TH2S&);
#pragma link C++ function operator*(TH2S&, TH2S&);
#pragma link C++ function operator/(TH2S&, TH2S&);

#pragma link C++ function operator*(Float_t,TH2I&);
#pragma link C++ function operator*(TH2I&, Float_t);
#pragma link C++ function operator+(TH2I&, TH2I&);
#pragma link C++ function operator-(TH2I&, TH2I&);
#pragma link C++ function operator*(TH2I&, TH2I&);
#pragma link C++ function operator/(TH2I&, TH2I&);

#pragma link C++ function operator*(Float_t,TH2F&);
#pragma link C++ function operator*(TH2F&, Float_t);
#pragma link C++ function operator+(TH2F&, TH2F&);
#pragma link C++ function operator-(TH2F&, TH2F&);
#pragma link C++ function operator*(TH2F&, TH2F&);
#pragma link C++ function operator/(TH2F&, TH2F&);

#pragma link C++ function operator*(Float_t,TH2D&);
#pragma link C++ function operator*(TH2D&, Float_t);
#pragma link C++ function operator+(TH2D&, TH2D&);
#pragma link C++ function operator-(TH2D&, TH2D&);
#pragma link C++ function operator*(TH2D&, TH2D&);
#pragma link C++ function operator/(TH2D&, TH2D&);

#pragma link C++ function operator*(Float_t,TH3C&);
#pragma link C++ function operator*(TH3C&, Float_t);
#pragma link C++ function operator+(TH3C&, TH3C&);
#pragma link C++ function operator-(TH3C&, TH3C&);
#pragma link C++ function operator*(TH3C&, TH3C&);
#pragma link C++ function operator/(TH3C&, TH3C&);

#pragma link C++ function operator*(Float_t,TH3S&);
#pragma link C++ function operator*(TH3S&, Float_t);
#pragma link C++ function operator+(TH3S&, TH3S&);
#pragma link C++ function operator-(TH3S&, TH3S&);
#pragma link C++ function operator*(TH3S&, TH3S&);
#pragma link C++ function operator/(TH3S&, TH3S&);

#pragma link C++ function operator*(Float_t,TH3I&);
#pragma link C++ function operator*(TH3I&, Float_t);
#pragma link C++ function operator+(TH3I&, TH3I&);
#pragma link C++ function operator-(TH3I&, TH3I&);
#pragma link C++ function operator*(TH3I&, TH3I&);
#pragma link C++ function operator/(TH3I&, TH3I&);

#pragma link C++ function operator*(Float_t,TH3F&);
#pragma link C++ function operator*(TH3F&, Float_t);
#pragma link C++ function operator+(TH3F&, TH3F&);
#pragma link C++ function operator-(TH3F&, TH3F&);
#pragma link C++ function operator*(TH3F&, TH3F&);
#pragma link C++ function operator/(TH3F&, TH3F&);

#pragma link C++ function operator*(Float_t,TH3D&);
#pragma link C++ function operator*(TH3D&, Float_t);
#pragma link C++ function operator+(TH3D&, TH3D&);
#pragma link C++ function operator-(TH3D&, TH3D&);
#pragma link C++ function operator*(TH3D&, TH3D&);
#pragma link C++ function operator/(TH3D&, TH3D&);

#pragma link C++ function R__H(Int_t);
#pragma link C++ function R__H(const char*);

#endif
