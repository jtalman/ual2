// @(#)root/gui:$Name:  $:$Id: TGColorDialog.h,v 1.5 2004/09/14 09:23:32 rdm Exp $
// Author: Bertrand Bellenot + Fons Rademakers   22/08/02

/*************************************************************************
 * Copyright (C) 1995-2002, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGColorDialog
#define ROOT_TGColorDialog

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGColorPalette, TGColorPick and TGColorDialog.                       //
//                                                                      //
// The TGColorPalette is a widget showing an matrix of color cells. The //
// colors can be set and selected.                                      //
//                                                                      //
// The TGColorPick is a widget which allows a color to be picked from   //
// HLS space. It consists of two elements: a color map window from      //
// where the user can select the hue and saturation level of a color,   //
// and a slider to select color's lightness.                            //
//                                                                      //
// Selecting a color in these two widgets will generate the event:      //
// kC_COLORSEL, kCOL_CLICK, widget id, 0.                               //
// and the signal:                                                      //
// ColorSelected(Pixel_t color)                                         //
//                                                                      //
// The TGColorDialog presents a full featured color selection dialog.   //
// It uses 2 TGColorPalette's and the TGColorPick widgets.              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGWidget
#include "TGWidget.h"
#endif


class TGTextEntry;
class TGTextBuffer;


//----------------------------------------------------------------------

class TGColorPalette : public TGFrame, public TGWidget {

protected:
   Int_t    fCx;           // x coordinate of currently selected color cell
   Int_t    fCy;           // y coordinate of currently selected color cell
   UInt_t   fCw;           // color cell width
   UInt_t   fCh;           // color cell height
   Int_t    fRows;         // number of color cell rows
   Int_t    fCols;         // number of color cell columns
   Pixel_t *fPixels;       // pixel value of colors
   TGGC     fDrawGC;       // graphics context used for drawing

   virtual void DoRedraw();
   virtual void GotFocus();
   virtual void LostFocus();

   void DrawFocusHilite(Int_t onoff);

public:
   TGColorPalette(const TGWindow *p = 0, Int_t cols = 8, Int_t rows = 8, Int_t id = -1);
   virtual ~TGColorPalette();

   virtual Bool_t HandleButton(Event_t *event);
   virtual Bool_t HandleMotion(Event_t *event);
   virtual Bool_t HandleKey(Event_t *event);

   virtual TGDimension GetDefaultSize() const
            { return TGDimension((fCw + 5) * fCols, (fCh + 5) * fRows); }

   void    SetColors(Pixel_t colors[]);
   void    SetColor(Int_t ix, Pixel_t color);
   void    SetCurrentCellColor(Pixel_t color);

   void    SetCellSize(Int_t w = 20, Int_t h = 17);

   Pixel_t GetColorByIndex(Int_t ix) const { return fPixels[ix]; }
   Pixel_t GetCurrentColor() const;

   virtual void ColorSelected(Pixel_t col = 0)
           { Emit("ColorSelected(Pixel_t)", col ? col : GetCurrentColor()); }  //*SIGNAL*

   ClassDef(TGColorPalette,0)  // Color palette widget
};

//----------------------------------------------------------------------

class TGColorPick : public TGFrame, public TGWidget {

private:
   Int_t    fColormap[64][3];    // colormap
   Pixel_t  fPixel[64];          // pixel values

protected:
   Pixmap_t     fHSimage;
   Pixmap_t     fLimage;
   Int_t        fNColors;
   Int_t        fClick;
   Int_t        fCx;
   Int_t        fCy;
   Int_t        fCz;
   Pixel_t      fCurrentColor;
   Rectangle_t  fColormapRect;
   Rectangle_t  fSliderRect;
   TGGC         fCursorGC;

   virtual void DoRedraw();

   void    DrawHScursor(Int_t onoff);
   void    DrawLcursor(Int_t onoff);
   void    SetHScursor(Int_t x, Int_t y);
   void    SetLcursor(Int_t z);

   void    CreateImages();
   void    InitImages();
   void    SetSliderColor();
   void    UpdateCurrentColor();

   void    AllocColors();
   void    FreeColors();
   void    CreateDitheredImage(Pixmap_t image, Int_t which);

public:
   TGColorPick(const TGWindow *p = 0, Int_t w = 1, Int_t h = 1, Int_t id = -1);
   virtual ~TGColorPick();

   virtual Bool_t HandleButton(Event_t *event);
   virtual Bool_t HandleMotion(Event_t *event);

   void     SetColor(Pixel_t color);
   Pixel_t  GetCurrentColor() const { return fCurrentColor; }

   virtual  void ColorSelected(Pixel_t col = 0)
            { Emit("ColorSelected(Pixel_t)", col ? col : GetCurrentColor()); }  //*SIGNAL*

   ClassDef(TGColorPick,0)  // Color picker widget
};

//----------------------------------------------------------------------

class TGColorDialog : public TGTransientFrame {

protected:
   Pixel_t         fCurrentColor;
   Int_t          *fRetc;
   Pixel_t        *fRetColor;

   TGColorPalette *fPalette;
   TGColorPalette *fCpalette;
   TGColorPick    *fColors;
   TGFrame        *fSample;
   TGTextEntry    *fRte, *fGte, *fBte, *fHte, *fLte, *fSte;
   TGTextBuffer   *fRtb, *fGtb, *fBtb, *fHtb, *fLtb, *fStb;

   void           UpdateRGBentries(Pixel_t *c);
   void           UpdateHLSentries(Pixel_t *c);
   virtual void   CloseWindow();
   virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

public:
   TGColorDialog(const TGWindow *p = 0, const TGWindow *m = 0, Int_t *retc = 0,
                 Pixel_t *color = 0);
   virtual ~TGColorDialog();

   ClassDef(TGColorDialog,0)  // Color selection dialog
};

#endif
