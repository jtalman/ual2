// @(#)root/ged:$Name:  $:$Id: TCurlyArcEditor.h
// Author: Ilka  Antcheva, Otto Schaile 15/12/04

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TCurlyArcEditor
#define ROOT_TCurlyArcEditor

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//  TCurlyArcEditor                                                     //
//                                                                      //
//  Implements GUI for editing CurlyArc attributes: radius, phi1, phi2. //                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TGButton
#include "TGWidget.h"
#endif
#ifndef ROOT_TGedFrame
#include "TGedFrame.h"
#endif

class TGComboBox;
class TGNumberEntry;
class TCurlyArc;

class TCurlyArcEditor : public TGedFrame {

protected:
   TCurlyArc            *fCurlyArc;         // CurlyArc object
   TGNumberEntry        *fRadiusEntry;      // radius entry
   TGNumberEntry        *fPhiminEntry;      // Phimin entry
   TGNumberEntry        *fPhimaxEntry;      // Phimax entry
   TGNumberEntry        *fCenterXEntry;     // center x entry
   TGNumberEntry        *fCenterYEntry;     // center y entry

   virtual void   ConnectSignals2Slots();
   TGComboBox    *BuildOptionComboBox(TGFrame* parent, Int_t id);
   
public:
   TCurlyArcEditor(const TGWindow *p, Int_t id,
                Int_t width = 140, Int_t height = 30,
                UInt_t options = kChildFrame,
                Pixel_t back = GetDefaultFrameBackground());
   virtual ~TCurlyArcEditor();

   virtual void   SetModel(TVirtualPad *pad, TObject *obj, Int_t event);
   virtual void   DoRadius();
   virtual void   DoPhimin();
   virtual void   DoPhimax();
   virtual void   DoCenterXY();

   ClassDef(TCurlyArcEditor,0)  // GUI for editing arrow attributes
};

#endif
