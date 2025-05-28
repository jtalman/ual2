// @(#)root/qt:$Name:  $:$Id: TQtBrush.h,v 1.3 2005/03/01 07:24:01 brun Exp $
// Author: Valeri Fine   21/01/2002

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * Copyright (C) 2002 by Valeri Fine.                                    *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TQtBrush
#define ROOT_TQtBrush

#ifndef __CINT__
#  include <qbrush.h>
#  include <qcolor.h>
#else
   class  QColor;
   class  QBrush;
#endif

#include "Rtypes.h"
   //
   // TQtBrush creates the QBrush Qt object based on the ROOT "fill" attributes 
   //
class TQtBrush : public QBrush
{
protected:
  QColor fBackground;
  int fStyle;
  int fFasi;
public:
   TQtBrush();
   TQtBrush(const TQtBrush &src):QBrush(src)
   {
      fBackground=src.fBackground;
      fStyle=src.fStyle;
      fFasi=src.fFasi;
   }
   virtual ~TQtBrush(){;}
   void SetStyle(int style=1000){  SetStyle(style/1000,style%1000); };
   void SetStyle(int style, int fasi);
   void SetColor(QColor &color);
   const QColor &GetColor() const { return fBackground;}
   int   GetStyle() const  { return 1000*fStyle + fFasi; }
   ClassDef(TQtBrush,0); // create QBrush object based on the ROOT "fill" attributes 
};

#endif
