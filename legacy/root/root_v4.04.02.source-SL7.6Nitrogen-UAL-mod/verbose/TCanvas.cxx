// @(#)root/gpad:$Name:  $:$Id: TCanvas.cxx,v 1.89 2005/04/23 10:55:07 brun Exp $
// Author: Rene Brun   12/12/94

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include<typeinfo>

#include <string.h>
#include <stdlib.h>

#include "Riostream.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TClass.h"
#include "TDirectory.h"
#include "TStyle.h"
#include "TText.h"
#include "TBox.h"
#include "TCanvasImp.h"
#include "TDialogCanvas.h"
#include "TGuiFactory.h"
#include "TEnv.h"
#include "TError.h"
#include "TContextMenu.h"
#include "TControlBar.h"
#include "TInterpreter.h"
#include "TApplication.h"
#include "TColor.h"
#include "TVirtualPadEditor.h"


// This small class and the static object makedefcanvas_init make sure that
// the TCanvas::MakeDefCanvas method is registered with TROOT as soon as
// the shared library containing TCanvas is loaded.

class TInitMakeDefCanvas {
public:
   TInitMakeDefCanvas() { TROOT::SetMakeDefCanvas(&TCanvas::MakeDefCanvas); }
};

static TInitMakeDefCanvas makedefcanvas_init;

//*-*x16 macros/layout_canvas

Bool_t TCanvas::fgIsFolder = kFALSE;

const Size_t kDefaultCanvasSize   = 20;

ClassImpQ(TCanvas)

//______________________________________________________________________________
//
//   A Canvas is an area mapped to a window directly under
//   the control of the display manager.
//   A ROOT session may have several canvases open at any given time.
//
//   A Canvas may be subdivided into independent graphical areas: the PADs
//   A canvas has a default pad which has the name of the canvas itself.
//   An example of a Canvas layout is sketched in the picture below.
//
//   ***********************************************************************
//   *          Tool Bar menus    for Canvas                               *
//   ***********************************************************************
//   *                                                                     *
//   *  ************************************    *************************  *
//   *  *                                  *    *                       *  *
//   *  *                                  *    *                       *  *
//   *  *                                  *    *                       *  *
//   *  *                                  *    *                       *  *
//   *  *                                  *    *                       *  *
//   *  *                                  *    *                       *  *
//   *  *              P1                  *    *        P2             *  *
//   *  *                                  *    *                       *  *
//   *  *                                  *    *                       *  *
//   *  *                                  *    *                       *  *
//   *  *                                  *    *                       *  *
//   *  *                                  *    *                       *  *
//   *  *                                  *    *                       *  *
//   *  ************************************    *************************  *
//   *                                                                     *
//   ***********************************************************************
//
//   This canvas contains two pads named P1 and P2.
//   Both Canvas, P1 and P2 can be moved, grown, shrinked using the
//   normal rules of the Display manager.
//   A copy of a real canvas with 4 pads is shown in the picture below.
//Begin_Html
/*
<img src="gif/canvas_layout.gif">
*/
//End_Html
//
//  Once objects have been drawn in a canvas, they can be edited/moved
//  by pointing directly to them. The cursor shape is changed
//  to suggest the type of action that one can do on this object.
//  Clicking with the right mouse button on an object pops-up
//  a contextmenu with a complete list of actions possible on this object.
//
//  A graphical editor may be started from the canvas "View" menu.
//  An interactive HELP is available by clicking on the HELP button
//  at the top right of the canvas.
//
//  A canvas may be automatically divided into pads via TPad::Divide.
//

//______________________________________________________________________________
TCanvas::TCanvas(Bool_t build) : TPad()
{
   // Canvas default constructor.

   if (!build || TClass::IsCallingNew()) {
      Constructor();
   } else {
      const char *defcanvas = gROOT->GetDefCanvasName();
      char *cdef;

      TList *lc = (TList*)gROOT->GetListOfCanvases();
      if (lc->FindObject(defcanvas))
         cdef = StrDup(Form("%s_n%d",defcanvas,lc->GetSize()+1));
      else
         cdef = StrDup(Form("%s",defcanvas));
      Constructor(cdef, cdef, 1);
      delete [] cdef;
   }
}

//______________________________________________________________________________
void TCanvas::Constructor()
{
   // Canvas default constructor

   if (gThreadXAR) {
      void *arr[2];
      arr[1] = this;
      if ((*gThreadXAR)("CANV", 2, arr, 0)) return;
   }

   fCanvas    = 0;
   fCanvasID  = -1;
   fCanvasImp = 0;
   fBatch     = kTRUE;
   fUpdating  = kFALSE;

   fContextMenu = 0;
   fSelected    = 0;
   fSelectedPad = 0;
   fPadSave     = 0;
   fAutoExec    = kTRUE;
   fShowEditor  = kFALSE;
   fShowToolBar = kFALSE;
}

//______________________________________________________________________________
//       TCanvas(const char *name, Int_t ww, Int_t wh, Int_t winid);
TCanvas::TCanvas(const char *name, Int_t ww, Int_t wh, Int_t winid)
{
std::cerr << __FILE__ << " " << __LINE__ << " enter TCanvas::TCanvas(const char *name, Int_t ww, Int_t wh, Int_t winid)\n";
std::cerr << __LINE__ << "\n";
   // Create an embedded canvas, i.e. a canvas that is in a TGCanvas widget
   // which is placed in a TGFrame. This ctor is only called via the
   // TRootEmbeddedCanvas class.

std::cerr << __LINE__ << "\n";
   Init();
std::cerr << __LINE__ << "\n";

   fCanvasID     = winid;
   fWindowTopX   = 0;
   fWindowTopY   = 0;
   fWindowWidth  = ww;
   fWindowHeight = wh;
   fCw           = ww + 4;
   fCh           = wh +28;
   fMenuBar      = kFALSE;
   fBatch        = kFALSE;
   fUpdating     = kFALSE;

std::cerr << __LINE__ << "\n";
std::cerr << __LINE__ << " gBatchGuiFactory " << gBatchGuiFactory << "\n";
   fCanvasImp    = gBatchGuiFactory->CreateCanvasImp(this, name, fCw, fCh);
std::cerr << __LINE__ << " typeid(fCanvasImp).name() " << typeid(fCanvasImp).name() << "\n";
std::cerr << __LINE__ << "\n";
   SetName(name);
std::cerr << __LINE__ << "\n";
   Build();
std::cerr << __FILE__ << " " << __LINE__ << " leave TCanvas::TCanvas(const char *name, Int_t ww, Int_t wh, Int_t winid)\n";
}

//_____________________________________________________________________________
TCanvas::TCanvas(const char *name, const char *title, Int_t form) : TPad()
{
std::cerr << __FILE__ << " " << __LINE__ << " enter TCanvas::TCanvas(const char *name, const char *title, Int_t form) : TPad()\n";
   //  Create a new canvas with a predefined size form.
   //  If form < 0  the menubar is not shown.
   //
   //  form = 1    700x500 at 10,10 (set by TStyle::SetCanvasDefH,W,X,Y)
   //  form = 2    500x500 at 20,20
   //  form = 3    500x500 at 30,30
   //  form = 4    500x500 at 40,40
   //  form = 5    500x500 at 50,50

std::cerr << __LINE__ << "\n";
   Constructor(name, title, form);
std::cerr << __FILE__ << " " << __LINE__ << " leave TCanvas::TCanvas(const char *name, const char *title, Int_t form) : TPad()\n";
}

//_____________________________________________________________________________
//                TCanvas(const char *name, Int_t ww, Int_t wh, Int_t winid);
void TCanvas::Constructor(const char *name, const char *title, Int_t form)
{
std::cerr << __FILE__ << " " << __LINE__ << " enter void TCanvas::Constructor(const char *name, const char *title, Int_t form)\n";
   //  Create a new canvas with a predefined size form.
   //  If form < 0  the menubar is not shown.
   //
   //  form = 1    700x500 at 10,10 (set by TStyle::SetCanvasDefH,W,X,Y)
   //  form = 2    500x500 at 20,20
   //  form = 3    500x500 at 30,30
   //  form = 4    500x500 at 40,40
   //  form = 5    500x500 at 50,50

   if (gThreadXAR) {
      void *arr[6];
      static Int_t ww = 500;
      static Int_t wh = 500;
      arr[1] = this; arr[2] = (void*)name; arr[3] = (void*)title; arr[4] =&ww; arr[5] = &wh;
      if ((*gThreadXAR)("CANV", 6, arr, NULL)) return;
   }

   Init();
   fMenuBar = kTRUE;
   if (form < 0) {
      form     = -form;
      fMenuBar = kFALSE;
   }
   fCanvasID = -1;
   TCanvas *old = (TCanvas*)gROOT->GetListOfCanvases()->FindObject(name);
   if (old && old->IsOnHeap()) delete old;
   if (strlen(name) == 0 || gROOT->IsBatch()) {   //We are in Batch mode
      fWindowTopX   = fWindowTopY = 0;
      fWindowWidth  = gStyle->GetCanvasDefW()-4;
      fWindowHeight = gStyle->GetCanvasDefH()-28;
      fCw           = fWindowWidth;
      fCh           = fWindowHeight;
      fCanvasImp    = gBatchGuiFactory->CreateCanvasImp(this, name, fCw, fCh);
      fBatch        = kTRUE;
   } else {                  //normal mode with a screen window
      Float_t cx = gStyle->GetScreenFactor();
      if (form < 1 || form > 5) form = 1;
      if (form == 1) {
         UInt_t uh = UInt_t(cx*gStyle->GetCanvasDefH());
         UInt_t uw = UInt_t(cx*gStyle->GetCanvasDefW());
         Int_t  ux = Int_t(cx*gStyle->GetCanvasDefX());
         Int_t  uy = Int_t(cx*gStyle->GetCanvasDefY());
         fCanvasImp = gGuiFactory->CreateCanvasImp(this, name, ux, uy, uw, uh);
      }
      fCw = 500;
      fCh = 500;
      if (form == 2) fCanvasImp = gGuiFactory->CreateCanvasImp(this, name, 20, 20, UInt_t(cx*500), UInt_t(cx*500));
      if (form == 3) fCanvasImp = gGuiFactory->CreateCanvasImp(this, name, 30, 30, UInt_t(cx*500), UInt_t(cx*500));
      if (form == 4) fCanvasImp = gGuiFactory->CreateCanvasImp(this, name, 40, 40, UInt_t(cx*500), UInt_t(cx*500));
      if (form == 5) fCanvasImp = gGuiFactory->CreateCanvasImp(this, name, 50, 50, UInt_t(cx*500), UInt_t(cx*500));
      fCanvasImp->ShowMenuBar(fMenuBar);
      fBatch = kFALSE;
   }
   SetName(name);
   SetTitle(title); // requires fCanvasImp set
   Build();

   // Popup canvas
   fCanvasImp->Show();
std::cerr << __FILE__ << " " << __LINE__ << " leave void TCanvas::Constructor(const char *name, const char *title, Int_t form)\n";
}

//_____________________________________________________________________________
//                TCanvas(const char *name, Int_t ww, Int_t wh, Int_t winid);
TCanvas::TCanvas(const char *name, const char *title, Int_t ww, Int_t wh) : TPad()
{
std::cerr << __FILE__ << " " << __LINE__ << " enter TCanvas::TCanvas(const char *name, const char *title, Int_t ww, Int_t wh) : TPad()\n";
   //  Create a new canvas at a random position.
   //
   //  ww is the canvas size in pixels along X
   //      (if ww < 0  the menubar is not shown)
   //  wh is the canvas size in pixels along Y

   Constructor(name, title, ww, wh);
std::cerr << __FILE__ << " " << __LINE__ << " leave TCanvas::TCanvas(const char *name, const char *title, Int_t ww, Int_t wh) : TPad()\n";
}

//_____________________________________________________________________________
//                TCanvas(const char *name, Int_t ww, Int_t wh, Int_t winid);
void TCanvas::Constructor(const char *name, const char *title, Int_t ww, Int_t wh)
{
std::cerr << __FILE__ << " " << __LINE__ << " enter void TCanvas::Constructor(const char *name, const char *title, Int_t ww, Int_t wh)\n";
   //  Create a new canvas at a random position.
   //
   //  ww is the canvas size in pixels along X
   //      (if ww < 0  the menubar is not shown)
   //  wh is the canvas size in pixels along Y

   if (gThreadXAR) {
       void *arr[6];
       arr[1] = this; arr[2] = (void*)name; arr[3] = (void*)title; arr[4] =&ww; arr[5] = &wh;
       if ((*gThreadXAR)("CANV", 6, arr, NULL)) return;
   }

   Init();
   fMenuBar = kTRUE;
   if (ww < 0) {
      ww       = -ww;
      fMenuBar = kFALSE;
   }
   fCw       = ww;
   fCh       = wh;
   fCanvasID = -1;
   TCanvas *old = (TCanvas*)gROOT->GetListOfCanvases()->FindObject(name);
   if (old && old->IsOnHeap()) delete old;
   if (strlen(name) == 0 || gROOT->IsBatch()) {   //We are in Batch mode
      fWindowTopX   = fWindowTopY = 0;
      fWindowWidth  = ww;
      fWindowHeight = wh;
      fCw           = ww;
      fCh           = wh;
      fCanvasImp    = gBatchGuiFactory->CreateCanvasImp(this, name, fCw, fCh);
      fBatch        = kTRUE;
   } else {
      Float_t cx = gStyle->GetScreenFactor();
      fCanvasImp = gGuiFactory->CreateCanvasImp(this, name, UInt_t(cx*ww), UInt_t(cx*wh));
      fCanvasImp->ShowMenuBar(fMenuBar);
      fBatch = kFALSE;
   }
   SetName(name);
   SetTitle(title); // requires fCanvasImp set
   Build();

   // Popup canvas
   fCanvasImp->Show();
std::cerr << __FILE__ << " " << __LINE__ << " leave void TCanvas::Constructor(const char *name, const char *title, Int_t ww, Int_t wh)\n";
}

//_____________________________________________________________________________
//       TCanvas(const char *name, Int_t ww, Int_t wh, Int_t winid);
TCanvas::TCanvas(const char *name, const char *title, Int_t wtopx, Int_t wtopy, Int_t ww, Int_t wh)
        : TPad()
{
std::cerr << __FILE__ << " " << __LINE__ << " enter TCanvas::TCanvas(const char *name, const char *title, Int_t wtopx, Int_t wtopy, Int_t ww, Int_t wh)\n";
   //  Create a new canvas.
   //
   //  wtopx,wtopy are the pixel coordinates of the top left corner of
   //  the canvas (if wtopx < 0) the menubar is not shown)
   //  ww is the canvas size in pixels along X
   //  wh is the canvas size in pixels along Y

   Constructor(name, title, wtopx, wtopy, ww, wh);
std::cerr << __FILE__ << " " << __LINE__ << " leave TCanvas::TCanvas(const char *name, const char *title, Int_t wtopx, Int_t wtopy, Int_t ww, Int_t wh)\n";
   //  Create a new canvas.
}

//_____________________________________________________________________________
void TCanvas::Constructor(const char *name, const char *title, Int_t wtopx,
                          Int_t wtopy, Int_t ww, Int_t wh)
{
std::cerr << __FILE__ << " " << __LINE__ << " enter void TCanvas::Constructor(const char *name, const char *title, Int_t wtopx,Int_t wtopy, Int_t ww, Int_t wh)\n";
   //  Create a new canvas.
   //
   //  wtopx,wtopy are the pixel coordinates of the top left corner of
   //  the canvas (if wtopx < 0) the menubar is not shown)
   //  ww is the canvas size in pixels along X
   //  wh is the canvas size in pixels along Y

   if (gThreadXAR) {
      void *arr[8];
      arr[1] = this;   arr[2] = (void*)name;   arr[3] = (void*)title;
      arr[4] = &wtopx; arr[5] = &wtopy; arr[6] = &ww; arr[7] = &wh;
      if ((*gThreadXAR)("CANV", 8, arr, NULL)) return;
   }

   Init();
   fMenuBar = kTRUE;
   if (wtopx < 0) {
      wtopx    = -wtopx;
      fMenuBar = kFALSE;
   }
   fCw       = ww;
   fCh       = wh;
   fCanvasID = -1;
   TCanvas *old = (TCanvas*)gROOT->GetListOfCanvases()->FindObject(name);
   if (old && old->IsOnHeap()) delete old;
   if (strlen(name) == 0 || gROOT->IsBatch()) {   //We are in Batch mode
      fWindowTopX   = fWindowTopY = 0;
      fWindowWidth  = ww;
      fWindowHeight = wh;
      fCw           = ww;
      fCh           = wh;
      fCanvasImp    = gBatchGuiFactory->CreateCanvasImp(this, name, fCw, fCh);
      fBatch        = kTRUE;
   } else {                   //normal mode with a screen window
      Float_t cx = gStyle->GetScreenFactor();
      fCanvasImp = gGuiFactory->CreateCanvasImp(this, name, Int_t(cx*wtopx), Int_t(cx*wtopy), UInt_t(cx*ww), UInt_t(cx*wh));
      fCanvasImp->ShowMenuBar(fMenuBar);
      fBatch = kFALSE;
   }
   SetName(name);
   SetTitle(title); // requires fCanvasImp set
   Build();

   // Popup canvas
   fCanvasImp->Show();
std::cerr << __FILE__ << " " << __LINE__ << " leave void TCanvas::Constructor(const char *name, const char *title, Int_t wtopx,Int_t wtopy, Int_t ww, Int_t wh)\n";
}

//_____________________________________________________________________________
void TCanvas::Init()
{
std::cerr << __FILE__ << " " << __LINE__ << " enter void TCanvas::Init()\n";
std::cerr << __LINE__ << "\n";
   // Initialize the TCanvas members. Called by all constructors.

   // Make sure the application environment exists. It is need for graphics
   // (colors are initialized in the TApplication ctor).
   if (!gApplication){
std::cerr << __LINE__ << "\n";
      TApplication::CreateApplication();
   }
std::cerr << __LINE__ << "\n";

   // Get some default from .rootrc. Used in fCanvasImp->InitWindow().
   fMoveOpaque      = gEnv->GetValue("Canvas.MoveOpaque", 0);
   fResizeOpaque    = gEnv->GetValue("Canvas.ResizeOpaque", 0);
   fHighLightColor  = gEnv->GetValue("Canvas.HighLightColor", kRed);
   fShowEventStatus = gEnv->GetValue("Canvas.ShowEventStatus", kFALSE);
   fShowToolBar     = gEnv->GetValue("Canvas.ShowToolBar", kFALSE);
   fShowEditor      = gEnv->GetValue("Canvas.ShowEditor", kFALSE);
   fAutoExec        = gEnv->GetValue("Canvas.AutoExec", kTRUE);
std::cerr << __LINE__ << "\n";

   // Fill canvas ROOT data structure
   fXsizeUser = 0;
   fYsizeUser = 0;
   fXsizeReal = kDefaultCanvasSize;
   fYsizeReal = kDefaultCanvasSize;
std::cerr << __LINE__ << "\n";

   fDISPLAY         = "$DISPLAY";
std::cerr << __LINE__ << "\n";
   fUpdating        = kFALSE;
   fRetained        = kTRUE;
   fSelected        = 0;
   fSelectedX       = 0;
   fSelectedY       = 0;
   fSelectedPad     = 0;
   fPadSave         = 0;
   fEvent           = -1;
   fEventX          = -1;
   fEventY          = -1;
   fContextMenu     = 0;
   SetBit(kMustCleanup);
std::cerr << __FILE__ << " " << __LINE__ << " leave void TCanvas::Init()\n";
}

//_____________________________________________________________________________
void TCanvas::Build()
{
std::cerr << __FILE__ << " " << __LINE__ << " enter void TCanvas::Build()\n";
std::cerr << __LINE__ << "\n";
   // Build a canvas. Called by all constructors.

   // Get window identifier
   if (fCanvasID == -1 && fCanvasImp){
std::cerr << __LINE__ << "\n";
      fCanvasID = fCanvasImp->InitWindow();
   }
std::cerr << __LINE__ << "\n";
   if (fCanvasID == -1) return;
std::cerr << __LINE__ << "\n";

   if (fCw < fCh) fXsizeReal = fYsizeReal*Float_t(fCw)/Float_t(fCh);
   else           fYsizeReal = fXsizeReal*Float_t(fCh)/Float_t(fCw);
std::cerr << "fXsizeReal " << fXsizeReal << "\n";
std::cerr << "fYsizeReal " << fYsizeReal << "\n";

std::cerr << __LINE__ << "\n";
   if (!IsBatch()) {    //normal mode with a screen window
std::cerr << __LINE__ << "\n";
std::cerr << __LINE__ << " fCanvasID " << fCanvasID << "\n";
std::cerr << __LINE__ << " fCanvasImp " << fCanvasImp << "\n";
std::cerr << __LINE__ << " gVirtualX " << gVirtualX << "\n";
std::cerr << __FILE__ << " " << __LINE__ << " gVirtualX->GetName() " << gVirtualX->GetName() << "\n";
std::cerr << __LINE__ << " typeid(gVirtualX).name() " << typeid(gVirtualX).name() << "\n";
      // Set default physical canvas attributes
      gVirtualX->SelectWindow(fCanvasID);
std::cerr << __LINE__ << "\n";
      gVirtualX->SetFillColor(1);         //Set color index for fill area
std::cerr << __LINE__ << "\n";
      gVirtualX->SetLineColor(1);         //Set color index for lines
std::cerr << __LINE__ << "\n";
      gVirtualX->SetMarkerColor(1);       //Set color index for markers
std::cerr << __LINE__ << "\n";
      gVirtualX->SetTextColor(1);         //Set color index for text
std::cerr << __LINE__ << "\n";

      // Clear workstation
std::cerr << __LINE__ << "\n";
      gVirtualX->ClearWindow();
std::cerr << __LINE__ << "\n";

      // Set Double Buffer on by default
std::cerr << __LINE__ << "\n";
      SetDoubleBuffer(1);
std::cerr << __LINE__ << "\n";

      // Get effective window parameters (with borders and menubar)
std::cerr << __LINE__ << "\n";
      fCanvasImp->GetWindowGeometry(fWindowTopX, fWindowTopY,
                                    fWindowWidth, fWindowHeight);

      // Get effective canvas parameters without borders
      Int_t dum1, dum2;
std::cerr << __LINE__ << "\n";
      gVirtualX->GetGeometry(fCanvasID, dum1, dum2, fCw, fCh);

std::cerr << __LINE__ << "\n";
      fContextMenu = new TContextMenu("ContextMenu");
   }
std::cerr << __LINE__ << "\n";
   gROOT->GetListOfCanvases()->Add(this);
std::cerr << __LINE__ << "\n";

   // Set Pad parameters
   gPad            = this;
   fCanvas         = this;
   fMother         = (TPad*)gPad;
std::cerr << __LINE__ << "\n";
   if (!fPrimitives) {
std::cerr << __LINE__ << "\n";
      fPrimitives     = new TList;
      SetFillColor(gStyle->GetCanvasColor());
      SetFillStyle(1001);
      SetGrid(gStyle->GetPadGridX(),gStyle->GetPadGridY());
      SetTicks(gStyle->GetPadTickX(),gStyle->GetPadTickY());
      SetLogx(gStyle->GetOptLogx());
      SetLogy(gStyle->GetOptLogy());
      SetLogz(gStyle->GetOptLogz());
      SetBottomMargin(gStyle->GetPadBottomMargin());
      SetTopMargin(gStyle->GetPadTopMargin());
      SetLeftMargin(gStyle->GetPadLeftMargin());
      SetRightMargin(gStyle->GetPadRightMargin());
      SetBorderSize(gStyle->GetCanvasBorderSize());
      SetBorderMode(gStyle->GetCanvasBorderMode());
      fBorderMode=gStyle->GetCanvasBorderMode(); // do not call SetBorderMode (function redefined in TCanvas)
      SetPad(0, 0, 1, 1);
      Range(0, 0, 1, 1);   //pad range is set by default to [0,1] in x and y
      gVirtualX->SelectPixmap(fPixmapID);    //pixmap must be selected
      PaintBorder(GetFillColor(), kTRUE);    //paint background
   }
std::cerr << __LINE__ << "\n";

   // transient canvases have typically no menubar and should not get
   // by default the event status bar (if set by default)
   if (fShowEventStatus && fMenuBar && fCanvasImp){
std::cerr << __LINE__ << "\n";
      fCanvasImp->ShowStatusBar(fShowEventStatus);
   }
std::cerr << __LINE__ << "\n";
   // ... and toolbar + editor
   if (fShowToolBar && fMenuBar && fCanvasImp){
std::cerr << __LINE__ << "\n";
      fCanvasImp->ShowToolBar(fShowToolBar);
   }
   if (fShowEditor && fMenuBar && fCanvasImp){
std::cerr << __LINE__ << "\n";
      fCanvasImp->ShowEditor(fShowEditor);
   }
std::cerr << __LINE__ << "\n";

#if defined(WIN32) && !defined(GDK_WIN32)
   if (!strcmp(gVirtualX->GetName(), "Win32"))
      gVirtualX->UpdateWindow(1);
#endif
std::cerr << __FILE__ << " " << __LINE__ << " leave void TCanvas::Build()\n";
}

//______________________________________________________________________________
TCanvas::TCanvas(const TCanvas &) : TPad()
{
   // Intentionally not implemented
}

//______________________________________________________________________________
TCanvas::~TCanvas()
{
   // Canvas destructor

   Destructor();
}

//______________________________________________________________________________
void TCanvas::Browse(TBrowser *b)
{
    Draw();
    cd();
    if (fgIsFolder) fPrimitives->Browse(b);
}


//______________________________________________________________________________
void TCanvas::Destructor()
{
   // Actual canvas destructor.

   if (gThreadXAR) {
      void *arr[2];
      arr[1] = this;
      if ((*gThreadXAR)("CDEL", 2, arr, NULL)) return;
   }

   if (!TestBit(kNotDeleted)) return;

   if (fContextMenu) { delete fContextMenu; fContextMenu = 0; }
   if (!gPad) return;

   Close();
}

//______________________________________________________________________________
TVirtualPad *TCanvas::cd(Int_t subpadnumber)
{
   // Set current canvas & pad. Returns the new current pad,
   // or 0 in case of failure.
   // See TPad::cd() for an explanation of the parameter.

   if (fCanvasID == -1) return 0;

   TPad::cd(subpadnumber);

   // in case doublebuffer is off, draw directly onto display window
   if (!IsBatch()) {
      if (!fDoubleBuffer)
         gVirtualX->SelectWindow(fCanvasID);
   }
   return gPad;
}

//______________________________________________________________________________
void TCanvas::Clear(Option_t *option)
{
   // Remove all primitives from the canvas.
   // If option "D" is specified, direct subpads are cleared but not deleted.
   // This option is not recursive, i.e. pads in direct subpads are deleted.

   if (fCanvasID == -1) return;

   if (!gVirtualX->IsCmdThread()) {
      gInterpreter->Execute(this, IsA(), "Clear", option);
      return;
   }

   TString opt = option;
   opt.ToLower();
   if (opt.Contains("d")) {
      // clear subpads, but do not delete pads in case the canvas
      // has been divided (note: option "D" is propagated so could cause
      // conflicts for primitives using option "D" for something else)
      if (fPrimitives) {
         TIter next(fPrimitives);
         TObject *obj;
         while ((obj=next())) {
            obj->Clear(option);
         }
      }
   } else {
      //default, clear everything in the canvas. Subpads are deleted
      TPad::Clear(option);   //Remove primitives from pad
   }

   fSelected    = 0;
   fSelectedPad = 0;
}

//______________________________________________________________________________
void TCanvas::Closed()
{
   // emit Closed signal

   Emit("Closed()");
}

//______________________________________________________________________________
void TCanvas::Close(Option_t *option)
{
//*-*-*-*-*-*-*-*-*-*-*-*Close canvas*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                    ============
//  Delete window/pads data structure
//

   if (fCanvasID == -1) return;

   if (!gVirtualX->IsCmdThread()) {
      gInterpreter->Execute(this, IsA(), "Close", option);
      return;
   }

   TCanvas *cansave = (TCanvas*)gPad->GetCanvas();
   TPad    *padsave = (TPad*)gPad;

   FeedbackMode(kFALSE);

   TPad::Close(option);

   if (!IsBatch()) {
      gVirtualX->SelectWindow(fCanvasID);    //select current canvas
      if (fCanvasImp) fCanvasImp->Close();
   }
   fCanvasID = -1;
   fBatch    = kTRUE;

   gROOT->GetListOfCanvases()->Remove(this);

   // Close actual window on screen
   SafeDelete(fCanvasImp);

   if (cansave == this) {
      gPad = (TCanvas *) gROOT->GetListOfCanvases()->First();
   } else {
      gPad = padsave;
   }

   Closed();
}

//______________________________________________________________________________
void TCanvas::CopyPixmaps()
{
std::cerr << __FILE__ << " " << __LINE__ << " enter void TCanvas::CopyPixmaps()\n";
std::cerr << __LINE__ << "\n";
//*-*-*-*-*-*-*-*-*Copy the canvas pixmap of the pad to the canvas*-*-*-*-*-*-*
//*-*              ===============================================
std::cerr << __LINE__ << " gVirtualX " << gVirtualX << "\n";
std::cerr << __LINE__ << " fCanvasID " << fCanvasID << "\n";
std::cerr << __LINE__ << " typeid(gVirtualX).name() " << typeid(gVirtualX).name() << "\n";

   if (!gVirtualX->IsCmdThread()) {
std::cerr << __LINE__ << "\n";
      gInterpreter->Execute(this, IsA(), "CopyPixmaps", "");
std::cerr << __LINE__ << "\n";
      return;
   }

   if (!IsBatch()) {
std::cerr << __LINE__ << "\n";
std::cerr << "COMMENTED OUT CopyPixmap() and TPad::CopyPixmaps()\n";
//    CopyPixmap();
std::cerr << __LINE__ << "\n";
//    TPad::CopyPixmaps();
std::cerr << "COMMENTED OUT CopyPixmap() and TPad::CopyPixmaps()\n";
std::cerr << __LINE__ << "\n";
   }
std::cerr << __FILE__ << " " << __LINE__ << " leave void TCanvas::CopyPixmaps()\n";
}

//______________________________________________________________________________
void TCanvas::Draw(Option_t *)
{
std::cerr << __FILE__ << " " << __LINE__ << " enter void TCanvas::Draw(Option_t *)\n";
   //  Draw a canvas.
   //  If a canvas with the name is already on the screen, the canvas is repainted.
   //  This function is useful when a canvas object has been saved in a Root file.
   //  One can then do:
   //     Root > Tfile f("file.root");
   //     Root > canvas.Draw();


   TCanvas *old = (TCanvas*)gROOT->GetListOfCanvases()->FindObject(GetName());
   if (old == this) {
      Paint();
      return;
   }
   if (old) { gROOT->GetListOfCanvases()->Remove(old); delete old;}

   if (fWindowWidth  == 0) fWindowWidth  = 800;
   if (fWindowHeight == 0) fWindowHeight = 600;
std::cerr << __LINE__ << "\n";
   fCanvasImp = gGuiFactory->CreateCanvasImp(this, GetName(), fWindowTopX, fWindowTopY,
                                             fWindowWidth, fWindowHeight);
std::cerr << __LINE__ << "\n";
   fCanvasImp->ShowMenuBar(fMenuBar);
std::cerr << __LINE__ << "\n";
   fCanvasImp->Show();
std::cerr << __LINE__ << "\n";

   Build();
   ResizePad();
   Modified();
std::cerr << __FILE__ << " " << __LINE__ << " leave void TCanvas::Draw(Option_t *)\n";
}

//______________________________________________________________________________
TObject *TCanvas::DrawClone(Option_t *option) const
{
   // Draw a clone of this canvas
   // A new canvas is created that is a clone of this canvas

   const char *defcanvas = gROOT->GetDefCanvasName();
   char *cdef;

   TList *lc = (TList*)gROOT->GetListOfCanvases();
   if (lc->FindObject(defcanvas))
      cdef = Form("%s_n%d",defcanvas,lc->GetSize()+1);
   else
      cdef = Form("%s",defcanvas);

   TCanvas *newCanvas = (TCanvas*)Clone();
   newCanvas->SetName(cdef);

   newCanvas->Draw(option);
   newCanvas->Update();
   return newCanvas;
}


//______________________________________________________________________________
TObject *TCanvas::DrawClonePad()
{
   // Draw a clone of this canvas into the current pad
   // In an interactive session, select the destination/current pad
   // with the middle mouse button, then point to the canvas area to select
   // the canvas context menu item DrawClonePad.
   // Note that the original canvas may have subpads.


  TPad *padsav = (TPad*)gPad;
  TPad *selpad = (TPad*)gROOT->GetSelectedPad();
  TPad *pad = padsav;
  if (pad == this) pad = selpad;
  if (fCanvasID == -1 || padsav == 0 || pad == this) {
     return DrawClone();
  }
  this->cd();
  TObject *obj, *clone;
  //copy pad attributes
  pad->Range(fX1,fY1,fX2,fY2);
  pad->SetTickx(GetTickx());
  pad->SetTicky(GetTicky());
  pad->SetGridx(GetGridx());
  pad->SetGridy(GetGridy());
  pad->SetLogx(GetLogx());
  pad->SetLogy(GetLogy());
  pad->SetLogz(GetLogz());
  pad->SetBorderSize(GetBorderSize());
  pad->SetBorderMode(GetBorderMode());
  TAttLine::Copy((TAttLine&)*pad);
  TAttFill::Copy((TAttFill&)*pad);
  TAttPad::Copy((TAttPad&)*pad);

  //copy primitives
  TIter next(GetListOfPrimitives());
  while ((obj=next())) {
     pad->cd();
     clone = obj->Clone();
     pad->GetListOfPrimitives()->Add(clone,obj->GetDrawOption());
  }
  pad->ResizePad();
  pad->Modified();
  pad->Update();
  if (padsav) padsav->cd();
  return 0;
}

//______________________________________________________________________________
void TCanvas::DrawEventStatus(Int_t event, Int_t px, Int_t py, TObject *selected)
{
//*-*-*-*-*-*-*Report name and title of primitive below the cursor*-*-*-*-*-*
//*-*          ===================================================
//
//    This function is called when the option "Event Status"
//    in the canvas menu "Options" is selected.
//

   const Int_t kTMAX=256;
   static char atext[kTMAX];

   if (!fShowEventStatus || !selected) return;

   if (!fCanvasImp) return; //this may happen when closing a TAttCanvas

   TVirtualPad* savepad;
   savepad = gPad;
   gPad = GetSelectedPad();

   fCanvasImp->SetStatusText(selected->GetTitle(),0);
   fCanvasImp->SetStatusText(selected->GetName(),1);
   if (event == kKeyPress)
      sprintf(atext, "%c", (char) px);
   else
      sprintf(atext, "%d,%d", px, py);
   fCanvasImp->SetStatusText(atext,2);
   fCanvasImp->SetStatusText(selected->GetObjectInfo(px,py),3);
   gPad = savepad;
}

//______________________________________________________________________________
void TCanvas::EditorBar()
{
//   TVirtualPadEditor::GetPadEditor()->LoadEditor();

   TVirtualPadEditor::GetPadEditor();
}

//______________________________________________________________________________
void TCanvas::EnterLeave(TPad *prevSelPad, TObject *prevSelObj)
{
   // Generate kMouseEnter and kMouseLeave events depending on the previously
   // selected object and the currently selected object. Does nothing if the
   // selected object does not change.

   if (prevSelObj == fSelected) return;

   TPad *padsav = (TPad *)gPad;
   Int_t sevent = fEvent;

   if (prevSelObj) {
      gPad = prevSelPad;
      prevSelObj->ExecuteEvent(kMouseLeave, fEventX, fEventY);
      fEvent = kMouseLeave;
      if (fAutoExec) RunAutoExec();
      ProcessedEvent(kMouseLeave, fEventX, fEventY, prevSelObj);  // emit signal
   }

   gPad = fSelectedPad;

   if (fSelected) {
      fSelected->ExecuteEvent(kMouseEnter, fEventX, fEventY);
      fEvent = kMouseEnter;
      if (fAutoExec) RunAutoExec();
      ProcessedEvent(kMouseEnter, fEventX, fEventY, fSelected);  // emit signal
   }

   fEvent = sevent;
   gPad   = padsav;
}

//______________________________________________________________________________
void TCanvas::ExecuteEvent(Int_t event, Int_t px, Int_t py)
{
//*-*-*-*-*-*-*-*-*-*-*Execute action corresponding to one event*-*-*-*
//*-*                  =========================================
//  This member function must be implemented to realize the action
//  corresponding to the mouse click on the object in the canvas
//
//  Only handle mouse motion events in TCanvas, all other events are
//  ignored for the time being
//

   if (gROOT->GetEditorMode()) {
      TPad::ExecuteEvent(event,px,py);
      return;
   }

   switch (event) {

   case kMouseMotion:
      SetCursor(kCross);
      break;
   }
}

//______________________________________________________________________________
void TCanvas::FeedbackMode(Bool_t set)
{
//*-*-*-*-*-*-*-*-*Turn rubberband feedback mode on or off*-*-*-*-*-*-*-*-*-*-*
//*-*              =======================================
   if (set) {
      SetDoubleBuffer(0);             // turn off double buffer mode
      gVirtualX->SetDrawMode(TVirtualX::kInvert);  // set the drawing mode to XOR mode
   } else {
      SetDoubleBuffer(1);             // turn on double buffer mode
      gVirtualX->SetDrawMode(TVirtualX::kCopy); // set drawing mode back to normal (copy) mode
   }
}

//______________________________________________________________________________
void TCanvas::Flush()
{
std::cerr << __FILE__ << " " << __LINE__ << " enter void TCanvas::Flush()\n";
std::cerr << __LINE__ << "\n";
std::cerr << __LINE__ << " gVirtualX " << gVirtualX << "\n";
std::cerr << __LINE__ << " fCanvasID " << fCanvasID << "\n";
std::cerr << __LINE__ << " typeid(gVirtualX).name() " << typeid(gVirtualX).name() << "\n";
//*-*-*-*-*-*-*-*-*Flush canvas buffers*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*              ====================
   if (fCanvasID == -1) return;

std::cerr << __LINE__ << "\n";
   TPad *padsav = (TPad*)gPad;
std::cerr << __LINE__ << "\n";
   cd();
std::cerr << __LINE__ << "\n";
   if (!IsBatch()) {
std::cerr << __LINE__ << "\n";
      gVirtualX->SelectWindow(fCanvasID);
std::cerr << __LINE__ << "\n";
      gPad = padsav; //don't do cd() because than also the pixmap is changed
std::cerr << __LINE__ << "\n";
std::cerr << "COMMENTED OUT CopyPixmaps()\n";
//    CopyPixmaps();
std::cerr << "COMMENTED OUT CopyPixmaps()\n";
std::cerr << __LINE__ << "\n";
      gVirtualX->UpdateWindow(1);
std::cerr << __LINE__ << "\n";
   }
   if (padsav) padsav->cd();
std::cerr << __FILE__ << " " << __LINE__ << " leave void TCanvas::Flush()\n";
}

//______________________________________________________________________________
void TCanvas::UseCurrentStyle()
{
//*-*-*-*-*-*Force a copy of current style for all objects in canvas*-*-*-*-*
//*-*        =======================================================

   if (!gVirtualX->IsCmdThread()) {
      gInterpreter->Execute(this, IsA(), "UseCurrentStyle", "");
      return;
   }
   TPad::UseCurrentStyle();

   SetFillColor(gStyle->GetCanvasColor());
   fBorderSize = gStyle->GetCanvasBorderSize();
   fBorderMode = gStyle->GetCanvasBorderMode();
}

//______________________________________________________________________________
Int_t TCanvas::GetWindowTopX()
{
   // Returns current top x position of window on screen.

   if (fCanvasImp) fCanvasImp->GetWindowGeometry(fWindowTopX, fWindowTopY,
                                                 fWindowWidth,fWindowHeight);

   return fWindowTopX;
}

//______________________________________________________________________________
Int_t TCanvas::GetWindowTopY()
{
   // Returns current top y position of window on screen.

   if (fCanvasImp) fCanvasImp->GetWindowGeometry(fWindowTopX, fWindowTopY,
                                                 fWindowWidth,fWindowHeight);

   return fWindowTopY;
}

//______________________________________________________________________________
void TCanvas::HandleInput(EEventType event, Int_t px, Int_t py)
{
//*-*-*-*-*-*-*-*-*-*Handle Input Events*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                ===================
//  Handle input events, like button up/down in current canvas.
//

   TPad    *pad;
   TPad    *prevSelPad = 0;
   TObject *prevSelObj = 0;

   if (fSelected && fSelected->TestBit(kNotDeleted))
      prevSelObj = fSelected;
   if (fSelectedPad && fSelectedPad->TestBit(kNotDeleted))
      prevSelPad = (TPad*) fSelectedPad;

   fPadSave = (TPad*)gPad;
   cd();        // make sure this canvas is the current canvas

   fEvent  = event;
   fEventX = px;
   fEventY = py;

   switch (event) {

   case kMouseMotion:
      // highlight object tracked over
      pad = Pick(px, py, prevSelObj);
      if (!pad) return;

      EnterLeave(prevSelPad, prevSelObj);

      gPad = pad;   // don't use cd() we will use the current
                    // canvas via the GetCanvas member and not via
                    // gPad->GetCanvas

      fSelected->ExecuteEvent(event, px, py);

      if (fAutoExec) RunAutoExec();

      break;

   case kMouseEnter:
      // mouse enters canvas
      if (!fDoubleBuffer) FeedbackMode(kTRUE);
      break;
      
   case kMouseLeave:
      // mouse leaves canvas
      {
         // force popdown of tooltips
         TObject *sobj = fSelected;
         TPad    *spad = fSelectedPad;
         fSelected     = 0;
         fSelectedPad  = 0;
         EnterLeave(prevSelPad, prevSelObj);
         fSelected     = sobj;
         fSelectedPad  = spad;
         if (!fDoubleBuffer) FeedbackMode(kFALSE);
      }
      break;

   case kButton1Double:
      // triggered on the second button down within 350ms and within
      // 3x3 pixels of the first button down, button up finishes action

   case kButton1Down:
      // find pad in which input occured
      pad = Pick(px, py, prevSelObj);
      if (!pad) return;

      gPad = pad;   // don't use cd() because we won't draw in pad
                    // we will only use its coordinate system

      FeedbackMode(kTRUE);   // to draw in rubberband mode
      fSelected->ExecuteEvent(event, px, py);

      if (fAutoExec) RunAutoExec();

      break;

   case kButton1Motion:

      if (fSelected) {
         gPad = fSelectedPad;

         fSelected->ExecuteEvent(event, px, py);

         {
            Bool_t resize = kFALSE;
            if (fSelected->InheritsFrom(TBox::Class()))
               resize = ((TBox*)fSelected)->IsBeingResized();
            if (fSelected->InheritsFrom(TVirtualPad::Class()))
               resize = ((TVirtualPad*)fSelected)->IsBeingResized();

            if ((!resize && fMoveOpaque) || (resize && fResizeOpaque)) {
               gPad = fPadSave;
               Update();
               FeedbackMode(kTRUE);
            }
         }
         if (fAutoExec) RunAutoExec();
      }

      break;

   case kButton1Up:

      if (fSelected) {
         gPad = fSelectedPad;

         fSelected->ExecuteEvent(event, px, py);

         if (fAutoExec) RunAutoExec();

         if (fPadSave->TestBit(kNotDeleted))
            gPad = fPadSave;
         else {
            gPad     = this;
            fPadSave = this;
         }

         Update();    // before calling update make sure gPad is reset
      }
      break;

//*-*----------------------------------------------------------------------

   case kButton2Down:
      // find pad in which input occured
      pad = Pick(px, py, prevSelObj);
      if (!pad) return;

      gPad = pad;   // don't use cd() because we won't draw in pad
                    // we will only use its coordinate system

      FeedbackMode(kTRUE);

      fSelected->Pop();           // pop object to foreground
      pad->cd();                  // and make its pad the current pad
      if (gDebug)
         printf("Current Pad: %s / %s\n", pad->GetName(), pad->GetTitle());

      // loop over all canvases to make sure that only one pad is highlighted
      {
         TIter next(gROOT->GetListOfCanvases());
         TCanvas *tc;
         while ((tc = (TCanvas *)next()))
            tc->Update();
      }

      break;   // don't want fPadSave->cd() to be executed at the end

   case kButton2Motion:
      break;

   case kButton2Up:
      break;

   case kButton2Double:
      break;

//*-*----------------------------------------------------------------------

   case kButton3Down:
      // popup context menu
      pad = Pick(px, py, prevSelObj);
      if (!pad) return;

      if (!fDoubleBuffer) FeedbackMode(kFALSE);

      if (fContextMenu && !fSelected->TestBit(kNoContextMenu) &&
          !pad->TestBit(kNoContextMenu) && !TestBit(kNoContextMenu))
          fContextMenu->Popup(px, py, fSelected, this, pad);

      break;

   case kButton3Motion:
      break;

   case kButton3Up:
      if (!fDoubleBuffer) FeedbackMode(kTRUE);
      break;

   case kButton3Double:
      break;

   case kKeyPress:

      // find pad in which input occured
      pad = Pick(px, py, prevSelObj);
      if (!pad) return;

      gPad = pad;   // don't use cd() because we won't draw in pad
                    // we will only use its coordinate system

      fSelected->ExecuteEvent(event, px, py);

      if (fAutoExec) RunAutoExec();

      break;

   default:
      break;
   }

   if (fPadSave && event != kButton2Down)
      fPadSave->cd();

   if (event != kMouseLeave) { // signal was already emitted for this event
      ProcessedEvent(event, px, py, fSelected);  // emit signal
      DrawEventStatus(event, px, py, fSelected);
   }
}

//______________________________________________________________________________
Bool_t TCanvas::IsFolder() const
{
   return fgIsFolder;
}

//______________________________________________________________________________
void TCanvas::ls(Option_t *option) const
{
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*List all pads*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                          =============
   TROOT::IndentLevel();
   cout <<"Canvas Name=" <<GetName()<<" Title="<<GetTitle()<<" Option="<<option<<endl;
   TROOT::IncreaseDirLevel();
   TPad::ls(option);
   TROOT::DecreaseDirLevel();
}

//______________________________________________________________________________
void TCanvas::MakeDefCanvas()
{
   // Static function to build a default canvas.

   const char *defcanvas = gROOT->GetDefCanvasName();
   char *cdef;

   TList *lc = (TList*)gROOT->GetListOfCanvases();
   if (lc->FindObject(defcanvas)) {
      Int_t n = lc->GetSize() + 1;
      cdef = new char[strlen(defcanvas)+15];
      do {
         strcpy(cdef, Form("%s_n%d", defcanvas, n++));
      } while (lc->FindObject(cdef));
   } else
      cdef = StrDup(Form("%s",defcanvas));

   new TCanvas(cdef, cdef, 1);

   Printf("<TCanvas::MakeDefCanvas>: created default TCanvas with name %s",cdef);
   delete [] cdef;
}

//______________________________________________________________________________
void TCanvas::MoveOpaque(Int_t set)
{
//*-*-*-*-*-*-*-*-*Set option to move objects/pads in a canvas*-*-*-*-*-*-*-*
//*-*              ===========================================
//
//  if set = 1 (default) graphics objects are moved in opaque mode
//         = 0 only the outline of objects is drawn when moving them
//  The option opaque produces the best effect. It requires however a
//  a reasonably fast workstation or response time.
//
   fMoveOpaque = set;
}

//______________________________________________________________________________
void TCanvas::Paint(Option_t *option)
{
//*-*-*-*-*-*-*-*-*Paint canvas*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*              ============

   if (fCanvas)
      TPad::Paint(option);

}

//______________________________________________________________________________
TPad *TCanvas::Pick(Int_t px, Int_t py, TObject *prevSelObj)
{
   // Prepare for pick, call TPad::Pick() and when selected object
   // is different from previous then emit Picked() signal.

   TObjLink *pickobj = 0;

   fSelected    = 0;
   fSelectedOpt = "";
   fSelectedPad = 0;

   TPad *pad = Pick(px, py, pickobj);
   if (!pad) return 0;

   if (!pickobj) {
      fSelected    = pad;
      fSelectedOpt = "";
   } else {
      if (!fSelected) {   // can be set via TCanvas::SetSelected()
         fSelected    = pickobj->GetObject();
         fSelectedOpt = pickobj->GetOption();
      }
   }
   fSelectedPad = pad;

   if (fSelected != prevSelObj)
      Picked(fSelectedPad, fSelected, fEvent);  // emit signal

   if ((fEvent == kButton1Down) || (fEvent == kButton2Down) || (fEvent == kButton3Down)) {
      if (fSelected) {
         Selected(fSelectedPad, fSelected, fEvent);  // emit signal
         fSelectedX = px;
         fSelectedY = py;
      }
   }
   return pad;
}

//______________________________________________________________________________
void TCanvas::Picked(TPad *pad, TObject *obj, Int_t event)
{
   // Emit Picked() signal.

   Long_t args[3];

   args[0] = (Long_t) pad;
   args[1] = (Long_t) obj;
   args[2] = event;

   Emit("Picked(TPad*,TObject*,Int_t)", args);
}

//______________________________________________________________________________
void TCanvas::Selected(TVirtualPad *pad, TObject *obj, Int_t event)
{
   // Emit Selected() signal.

   Long_t args[3];

   args[0] = (Long_t) pad;
   args[1] = (Long_t) obj;
   args[2] = event;

   Emit("Selected(TVirtualPad*,TObject*,Int_t)", args);
}

//______________________________________________________________________________
void TCanvas::ProcessedEvent(Int_t event, Int_t x, Int_t y, TObject *obj)
{
   // Emit ProcessedEvent() signal.

   Long_t args[4];

   args[0] = event;
   args[1] = x;
   args[2] = y;
   args[3] = (Long_t) obj;

   Emit("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", args);
}

//______________________________________________________________________________
void TCanvas::Resize(Option_t *)
{
//*-*-*-*-*-*-*Recompute canvas parameters following a X11 Resize*-*-*-*-*-*-*
//*-*          ==================================================

   if (fCanvasID == -1) return;

   if (!gVirtualX->IsCmdThread()) {
      gInterpreter->Execute(this, IsA(), "Resize", "");
      return;
   }

   TPad *padsav  = (TPad*)gPad;
   cd();

   if (!IsBatch()) {
      gVirtualX->SelectWindow(fCanvasID);      //select current canvas
      gVirtualX->ResizeWindow(fCanvasID);      //resize canvas and off-screen buffer

      // Get effective window parameters including menubar and borders
      fCanvasImp->GetWindowGeometry(fWindowTopX, fWindowTopY,
                                    fWindowWidth, fWindowHeight);

      // Get effective canvas parameters without borders
      Int_t dum1, dum2;
      gVirtualX->GetGeometry(fCanvasID, dum1, dum2, fCw, fCh);
   }

   if (fXsizeUser && fYsizeUser) {
      UInt_t nwh = fCh;
      UInt_t nww = fCw;
      Double_t rxy = fXsizeUser/fYsizeUser;
      if (rxy < 1) {
         UInt_t twh = UInt_t(Double_t(fCw)/rxy);
         if (twh > fCh)
            nww = UInt_t(Double_t(fCh)*rxy);
         else
            nwh = twh;
         if (nww > fCw) {
            nww = fCw; nwh = twh;
         }
         if (nwh > fCh) {
            nwh = fCh; nww = UInt_t(Double_t(fCh)/rxy);
         }
      } else {
         UInt_t twh = UInt_t(Double_t(fCw)*rxy);
         if (twh > fCh)
            nwh = UInt_t(Double_t(fCw)/rxy);
         else
            nww = twh;
         if (nww > fCw) {
            nww = fCw; nwh = twh;
         }
         if (nwh > fCh) {
            nwh = fCh; nww = UInt_t(Double_t(fCh)*rxy);
         }
      }
      fCw = nww;
      fCh = nwh;
   }

   if (fCw < fCh) {
      fYsizeReal = kDefaultCanvasSize;
      fXsizeReal = fYsizeReal*Double_t(fCw)/Double_t(fCh);
   }
   else {
      fXsizeReal = kDefaultCanvasSize;
      fYsizeReal = fXsizeReal*Double_t(fCh)/Double_t(fCw);
   }

//*-*- Loop on all pads to recompute conversion coefficients
   TPad::ResizePad();

   if (padsav) padsav->cd();
}

//______________________________________________________________________________
void TCanvas::RunAutoExec()
{
   // Execute the list of TExecs in the current pad.

   if (!gPad) return;
   ((TPad*)gPad)->AutoExec();
}

//______________________________________________________________________________
void TCanvas::SavePrimitive(ofstream &out, Option_t *option)
{
   // Save primitives in this canvas in C++ macro file with GUI

   Bool_t invalid = kFALSE;

   // Write canvas options (in $TROOT or $TStyle)
   if (gStyle->GetOptFit()) {
      out<<"   gStyle->SetOptFit(1);"<<endl;
   }
   if (!gStyle->GetOptStat()) {
      out<<"   gStyle->SetOptStat(0);"<<endl;
   }
   if (gROOT->GetEditHistograms()) {
      out<<"   gROOT->SetEditHistograms();"<<endl;
   }
   if (GetShowEventStatus()) {
      out<<"   "<<GetName()<<"->ToggleEventStatus();"<<endl;
   }
   if (GetShowToolBar()) {
      out<<"   "<<GetName()<<"->ToggleToolBar();"<<endl;
   }
   if (GetHighLightColor() != 5) {
      if (GetHighLightColor() > 228) {
         TColor::SaveColor(out, GetHighLightColor());
         out<<"   "<<GetName()<<"->SetHighLightColor(ci);" << endl;
      } else 
         out<<"   "<<GetName()<<"->SetHighLightColor("<<GetHighLightColor()<<");"<<endl;
   }

   // Now recursively scan all pads of this canvas
   cd();
   if (invalid) SetName("c1");
   TPad::SavePrimitive(out,option);
   if (invalid) SetName(" ");
}

//______________________________________________________________________________
void TCanvas::SaveSource(const char *filename, Option_t *option)
{
//*-*-*-*-*-*-*Save primitives in this canvas as a C++ macro file*-*-*-*-*-*
//*-*          ==================================================

//    reset bit TClass::kClassSaved for all classes
   TIter next(gROOT->GetListOfClasses());
   TClass *cl;
   while((cl = (TClass*)next())) {
      cl->ResetBit(TClass::kClassSaved);
   }

   char quote = '"';
   ofstream out;
   Int_t lenfile = strlen(filename);
   char * fname;
   char lcname[10];
   const char *cname = GetName();
   Bool_t invalid = kFALSE;
//    if filename is given, open this file, otherwise create a file
//    with a name equal to the canvasname.C
   if (lenfile) {
       fname = (char*)filename;
       out.open(fname, ios::out);
   } else {
       Int_t nch = strlen(cname);
       if (nch < 10) {
          strcpy(lcname,cname);
          for (Int_t k=1;k<=nch;k++) {if (lcname[nch-k] == ' ') lcname[nch-k] = 0;}
          if (lcname[0] == 0) {invalid = kTRUE; strcpy(lcname,"c1"); nch = 2;}
          cname = lcname;
       }
       fname = new char[nch+3];
       strcpy(fname,cname);
       strcat(fname,".C");
       out.open(fname, ios::out);
   }
   if (!out.good ()) {
      Printf("SaveSource cannot open file: %s",fname);
      if (!lenfile) delete [] fname;
      return;
   }

//   Write macro header and date/time stamp
   TDatime t;
   Float_t cx = gStyle->GetScreenFactor();
   Int_t topx,topy;
   UInt_t w, h;
   UInt_t editorWidth = fCanvasImp->GetWindowGeometry(topx,topy,w,h);
   w = UInt_t((fWindowWidth - editorWidth)/cx);
   h = UInt_t((fWindowHeight)/cx);
   topx = GetWindowTopX();
   topy = GetWindowTopY();

   if (w == 0) {
      w = GetWw()+4; h = GetWh()+4;
      topx = 1;    topy = 1;
   }

   out <<"{"<<endl;
   out <<"//=========Macro generated from canvas: "<<GetName()<<"/"<<GetTitle()<<endl;
   out <<"//=========  ("<<t.AsString()<<") by ROOT version"<<gROOT->GetVersion()<<endl;
//   out <<"   gROOT->Reset();"<<endl;

//   Write canvas parameters (TDialogCanvas case)
   if (InheritsFrom(TDialogCanvas::Class())) {
      out<<"   "<<ClassName()<<" *"<<cname<<" = new "<<ClassName()<<"("<<quote<<GetName()
         <<quote<<", "<<quote<<GetTitle()<<quote<<","<<w<<","<<h<<");"<<endl;
   } else {
//   Write canvas parameters (TCanvas case)
      out<<"   TCanvas *"<<cname<<" = new TCanvas("<<quote<<GetName()<<quote<<", "<<quote<<GetTitle()
         <<quote;
      if (!HasMenuBar())
         out<<",-"<<topx<<","<<topy<<","<<w<<","<<h<<");"<<endl;
      else
         out<<","<<topx<<","<<topy<<","<<w<<","<<h<<");"<<endl;
   }
//   Write canvas options (in $TROOT or $TStyle)
   if (gStyle->GetOptFit()) {
      out<<"   gStyle->SetOptFit(1);"<<endl;
   }
   if (!gStyle->GetOptStat()) {
      out<<"   gStyle->SetOptStat(0);"<<endl;
   }
   if (gROOT->GetEditHistograms()) {
      out<<"   gROOT->SetEditHistograms();"<<endl;
   }
   if (GetShowEventStatus()) {
      out<<"   "<<GetName()<<"->ToggleEventStatus();"<<endl;
   }
   if (GetHighLightColor() != 5) {
      if (GetHighLightColor() > 228) {
         TColor::SaveColor(out, GetHighLightColor());
         out<<"   "<<GetName()<<"->SetHighLightColor(ci);" << endl;
      } else 
         out<<"   "<<GetName()<<"->SetHighLightColor("<<GetHighLightColor()<<");"<<endl;
   }


//   Now recursively scan all pads of this canvas
   cd();
   if (invalid) SetName("c1");
   TPad::SavePrimitive(out,option);
//   Write canvas options related to pad editor
   out<<"   "<<GetName()<<"->SetSelected("<<GetName()<<");"<<endl;
   if (GetShowToolBar()) {
      out<<"   "<<GetName()<<"->ToggleToolBar();"<<endl;
   }
   if (invalid) SetName(" ");

   out <<"}"<<endl;
   out.close();
   Printf("C++ Macro file: %s has been generated", fname);

//    reset bit TClass::kClassSaved for all classes
   next.Reset();
   while((cl = (TClass*)next())) {
      cl->ResetBit(TClass::kClassSaved);
   }
   if (!lenfile) delete [] fname;
}

//______________________________________________________________________________
void TCanvas::SetBatch(Bool_t batch)
{
   // Toggle batch mode. However, if the canvas is created without a window
   // then batch mode always stays set.

   if (gROOT->IsBatch())
      fBatch = kTRUE;
   else
      fBatch = batch;
}

//______________________________________________________________________________
void TCanvas::SetCanvasSize(UInt_t ww, UInt_t wh)
{
   // Set Width and Height of canvas to ww and wh respectively
   // If ww and/or wh are greater than the current canvas window
   // a scroll bar is automatically generated.
   // Use this function to zoom in a canvas and naviguate via
   // the scroll bars.

   if (fCanvasImp) fCanvasImp->SetCanvasSize(ww, wh);
}

//______________________________________________________________________________
void TCanvas::SetCursor(ECursor cursor)
{
//*-*-*-*-*-*-*-*-*-*-*Set cursor*-*-**-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ==========

   if (IsBatch()) return;
   gVirtualX->SetCursor(fCanvasID, cursor);
}

//______________________________________________________________________________
void TCanvas::SetDoubleBuffer(Int_t mode)
{
//*-*-*-*-*-*-*-*-*-*-*Set Double Buffer On/Off*-*-**-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ========================

   if (IsBatch()) return;
   fDoubleBuffer = mode;
   gVirtualX->SetDoubleBuffer(fCanvasID, mode);

   // depending of the buffer mode set the drawing window to either
   // the canvas pixmap or to the canvas on-screen window
#if !defined(WIN32) || defined(GDK_WIN32)
   if (fDoubleBuffer)
      gVirtualX->SelectWindow(fPixmapID);
   else
      gVirtualX->SelectWindow(fCanvasID);
#else
   if (fDoubleBuffer && strcmp(gVirtualX->GetName(), "Win32"))
      gVirtualX->SelectWindow(fPixmapID);
   else
      gVirtualX->SelectWindow(fCanvasID);
#endif
}

//______________________________________________________________________________
void TCanvas::SetFixedAspectRatio(Bool_t fixed)
{
   // Fix canvas aspect ratio to current value if fixed is true.

   if (fixed) {
      if (!fFixedAspectRatio) {
         if (fCh != 0)
            fAspectRatio = Double_t(fCw) / fCh;
         else {
            Error("SetAspectRatio", "cannot fix aspect ratio, height of canvas is 0");
            return;
         }
         fFixedAspectRatio = kTRUE;
      }
   } else {
      fFixedAspectRatio = kFALSE;
      fAspectRatio = 0;
   }
}

//______________________________________________________________________________
void TCanvas::SetFolder(Bool_t isfolder)
{
// if isfolder=kTRUE, the canvas can be browsed like a folder
// by default a canvas is not browsable

   fgIsFolder = isfolder;
}

//______________________________________________________________________________
void TCanvas::SetSelected(TObject *obj)
{
   fSelected = obj;
   if (obj) obj->SetBit(kMustCleanup);
}

//______________________________________________________________________________
void TCanvas::SetTitle(const char *title)
{
//*-*-*-*-*-*-*-*-*-*-*Set Canvas title*-*-**-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ===============

   fTitle = title;
   if (fCanvasImp) fCanvasImp->SetWindowTitle(title);
}

//______________________________________________________________________________
void TCanvas::Size(Float_t xsize, Float_t ysize)
{
//*-*-*-*-*-*-*Set the canvas scale in centimeters*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*          ===================================
//  This information is used by PostScript to set the page size.
//  xsize  = size of the canvas in centimeters along X
//  ysize  = size of the canvas in centimeters along Y
//   if xsize and ysize are not equal to 0, then the scale factors will
//   be computed to keep the ratio ysize/xsize independently of the canvas
//   size (parts of the physical canvas will be unused).
//
//   if xsize = 0 and ysize is not zero, then xsize will be computed
//      to fit to the current canvas scale. If the canvas is resized,
//      a new value for xsize will be recomputed. In this case the aspect
//      ratio is not preserved.
//
//   if both xsize = 0 and ysize = 0, then the scaling is automatic.
//   the largest dimension will be allocated a size of 20 centimeters.
//

   fXsizeUser = xsize;
   fYsizeUser = ysize;

   Resize();
}

//_______________________________________________________________________
void TCanvas::Streamer(TBuffer &b)
{
//*-*-*-*-*-*-*-*-*Stream a class object*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*              =========================================
   UInt_t R__s, R__c;
   if (b.IsReading()) {
      Version_t v = b.ReadVersion(&R__s, &R__c);
      gPad    = this;
      fCanvas = this;
      TPad::Streamer(b);
      gPad    = this;
      //restore the colors
      TObjArray *colors = (TObjArray*)fPrimitives->FindObject("ListOfColors");
      if (colors) {
         TIter next(colors);
         TColor *colold;
         while ((colold = (TColor*)next())) {
            if (colold) {
               Int_t cn = 0;
               if (colold) cn = colold->GetNumber();
               TColor *colcur = gROOT->GetColor(cn);
               if (colcur) {
                  colcur->SetRGB(colold->GetRed(),colold->GetGreen(),colold->GetBlue());
               } else {
                  colcur = new TColor(cn,colold->GetRed(),
                                        colold->GetGreen(),
                                        colold->GetBlue(),
                                        colold->GetName());
               }
            }
         }
         fPrimitives->Remove(colors);
         colors->Delete();
         delete colors;
      }
      
      fDISPLAY.Streamer(b);
      b >> fDoubleBuffer;
      b >> fRetained;
      b >> fXsizeUser;
      b >> fYsizeUser;
      b >> fXsizeReal;
      b >> fYsizeReal;
      fCanvasID = -1;
      b >> fWindowTopX;
      b >> fWindowTopY;
      if (v > 2) {
         b >> fWindowWidth;
         b >> fWindowHeight;
      }
      b >> fCw;
      b >> fCh;
      if (v <= 2) {
         fWindowWidth  = fCw;
         fWindowHeight = fCh;
      }
      fCatt.Streamer(b);
      b >> fMoveOpaque;
      b >> fResizeOpaque;
      b >> fHighLightColor;
      b >> fBatch;
      fBatch = gROOT->IsBatch();
      if (v < 2) return;
      b >> fShowEventStatus;
      if (v > 3)
         b >> fAutoExec;
      b >> fMenuBar;
      b.CheckByteCount(R__s, R__c, TCanvas::IsA());
   } else {
      //save list of colors
      //we must protect the case when two or more canvases are saved
      //in the same buffer. If the list of colors has already been saved
      //in the buffer, do not add the list of colors to the list of primitives. 
      TObjArray *colors = 0;
      if (!b.CheckObject(gROOT->GetListOfColors(),TObjArray::Class())) {
         colors = (TObjArray*)gROOT->GetListOfColors();
         fPrimitives->Add(colors);
      }
      R__c = b.WriteVersion(TCanvas::IsA(), kTRUE);
      TPad::Streamer(b);
      if(colors) fPrimitives->Remove(colors);
      fDISPLAY.Streamer(b);
      b << fDoubleBuffer;
      b << fRetained;
      b << fXsizeUser;
      b << fYsizeUser;
      b << fXsizeReal;
      b << fYsizeReal;
      UInt_t w   = fWindowWidth,  h    = fWindowHeight;
      Int_t topx = fWindowTopX,   topy = fWindowTopY;
      UInt_t editorWidth = 0;
      if(fCanvasImp) editorWidth = fCanvasImp->GetWindowGeometry(topx,topy,w,h);
      b << topx;
      b << topy;
      b << (UInt_t)(w-editorWidth);
      b << h;
      b << fCw;
      b << fCh;
      fCatt.Streamer(b);
      b << fMoveOpaque;
      b << fResizeOpaque;
      b << fHighLightColor;
      b << fBatch;
      b << fShowEventStatus;
      b << fAutoExec;
      b << fMenuBar;
      b.SetByteCount(R__c, kTRUE);
   }
}

//______________________________________________________________________________
void TCanvas::ToggleAutoExec()
{
   // Toggle pad auto execution of list of TExecs.

   fAutoExec = fAutoExec ? kFALSE : kTRUE;
}

//______________________________________________________________________________
void TCanvas::ToggleEventStatus()
{
   // Toggle event statusbar.

   fShowEventStatus = fShowEventStatus ? kFALSE : kTRUE;

   if (fCanvasImp) fCanvasImp->ShowStatusBar(fShowEventStatus);
}

//______________________________________________________________________________
void TCanvas::ToggleToolBar()
{
   // Toggle toolbar.

   fShowToolBar = fShowToolBar ? kFALSE : kTRUE;

   if (fCanvasImp) fCanvasImp->ShowToolBar(fShowToolBar);
}

//______________________________________________________________________________
void TCanvas::ToggleEditor()
{
   // Toggle editor.

   fShowEditor = fShowEditor ? kFALSE : kTRUE;

   if (fCanvasImp) fCanvasImp->ShowEditor(fShowEditor);
}

//______________________________________________________________________________
void TCanvas::Update()
{
   // Update canvas pad buffers

   if (fUpdating) return;

   if (gThreadXAR) {
      void *arr[2];
      arr[1] = this;
      if ((*gThreadXAR)("CUPD", 2, arr, NULL)) return;
   }

   if (!gVirtualX->IsCmdThread()) {
      gInterpreter->Execute(this, IsA(), "Update", "");
      return;
   }
   fUpdating = kTRUE;

   if (!IsBatch()) FeedbackMode(kFALSE);      // Goto double buffer mode

   PaintModified();           // Repaint all modified pad's

   Flush();                   // Copy all pad pixmaps to the screen

   SetCursor(kCross);
   fUpdating = kFALSE;
}

//______________________________________________________________________________
void TCanvas::DisconnectWidget()
{
   // Used by friend class TCanvasImp.

   fCanvasID    = 0;
   fContextMenu = 0;
}
