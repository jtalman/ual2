// @(#)root/base:$Name:  $:$Id: TAttMarker.h,v 1.3 2002/05/03 10:48:53 brun Exp $
// Author: Rene Brun   12/05/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TAttMarker
#define ROOT_TAttMarker


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TAttMarker                                                           //
//                                                                      //
// Marker attributes.                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_Gtypes
#include "Gtypes.h"
#endif
#ifndef ROOT_TString
#include "TString.h"
#endif


class TAttMarker {

protected:
        Color_t    fMarkerColor;       //Marker color index
        Style_t    fMarkerStyle;       //Marker style
        Size_t     fMarkerSize;        //Marker size

public:
        TAttMarker();
        TAttMarker(Color_t color, Style_t style, Size_t msize);
        virtual ~TAttMarker();
                void     Copy(TAttMarker &attmarker) const;
        virtual Color_t  GetMarkerColor() const {return fMarkerColor;}
        virtual Style_t  GetMarkerStyle() const {return fMarkerStyle;}
        virtual Size_t   GetMarkerSize()  const {return fMarkerSize;}
        virtual void     Modify();
        virtual void     ResetAttMarker(Option_t *toption="");
        virtual void     SaveMarkerAttributes(ofstream &out, const char *name, Int_t coldef=1, Int_t stydef=1, Int_t sizdef=1);
        virtual void     SetMarkerAttributes();  // *MENU*
        virtual void     SetMarkerColor(Color_t tcolor=1) { fMarkerColor = tcolor;}
        virtual void     SetMarkerStyle(Style_t mstyle=1) { fMarkerStyle = mstyle;}
        virtual void     SetMarkerSize(Size_t msize=1)    { fMarkerSize  = msize;}

        ClassDef(TAttMarker,1)  //Marker attributes
};

#endif

