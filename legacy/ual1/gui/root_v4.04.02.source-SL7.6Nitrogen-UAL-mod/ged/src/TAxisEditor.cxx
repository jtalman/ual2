// @(#)root/ged:$Name:  $:$Id: TAxisEditor.cxx,v 1.7 2004/10/21 10:01:27 brun Exp $
// Author: Ilka Antcheva   11/05/04

/*************************************************************************
 * Copyright (C) 1995-2002, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//  TAxisEditor                                                         //
//                                                                      //
//  Implements GUI for axis attributes.                                 //
//     axis color                                                       //
//     ticks parameters: length, setting on both axis sides,            //
//     logarithmic or linear scale along the selected axis,             //
//     primary, secondary and tertiary axis divisions,                  //
//     setting more logarithmic labels,                                 //
//     optimizing labels' position if available                         //
//     axis title - a title can be added via the text entry field       // 
//     and can be set to be centered or rotated via the corresponding   // 
//     check buttons, the title color, offset, font can be set easily   //
//     axis labels - their color, size, offset can be set similarly,    //
//     in addition there is a check box for no exponent choice,         //
//     and another one for setting the same decimal part for all labels //                                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
//Begin_Html
/*
<img src="gif/TAxisEditor.gif">
*/
//End_Html


#include "TAxisEditor.h"
#include "TGedFrame.h"
#include "TGColorSelect.h"
#include "TGColorDialog.h"
#include "TGComboBox.h"
#include "TGNumberEntry.h"
#include "TGTextEntry.h"
#include "TGToolTip.h"
#include "TGLabel.h"
#include "TGClient.h"
#include "TColor.h"
#include "TVirtualPad.h"
#include "TStyle.h"

ClassImp(TGedFrame)
ClassImp(TAxisEditor)

enum {
   kCOL_AXIS,
   kCOL_TIT,
   kCOL_LBL,
   kFONT_TIT,
   kFONT_LBL,
   kTITLE,
   kAXIS_TICKS,
   kAXIS_DIV1,
   kAXIS_DIV2,
   kAXIS_DIV3,
   kAXIS_OPTIM,
   kAXIS_LOG,
   kAXIS_TITSIZE,
   kAXIS_TITOFFSET,
   kAXIS_CENTERED,
   kAXIS_ROTATED,
   kAXIS_LBLSIZE,
   kAXIS_LBLOFFSET,
   kAXIS_TICKSBOTH,
   kAXIS_LBLLOG,
   kAXIS_LBLEXP,
   kAXIS_LBLDIR,
   kAXIS_LBLSORT,
   kAXIS_LBLDEC
};


//______________________________________________________________________________
TAxisEditor::TAxisEditor(const TGWindow *p, Int_t id, Int_t width,
                         Int_t height, UInt_t options, Pixel_t back)
   : TGedFrame(p, id, width, height, options | kVerticalFrame, back)
{
   // Constructor of axis attribute GUI.
   
   fAxis = 0;
   
   MakeTitle("Axis");

   TGCompositeFrame *f2 = new TGCompositeFrame(this, 80, 20, kHorizontalFrame);
   fAxisColor = new TGColorSelect(f2, 0, kCOL_AXIS);
   f2->AddFrame(fAxisColor, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
   fAxisColor->Associate(this);
   TGLabel *fTicksLabel = new TGLabel(f2, "Ticks:");
   f2->AddFrame(fTicksLabel, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 3, 0, 1, 1));
   fTickLength = new TGNumberEntry(f2, 0.03, 5, kAXIS_TICKS, 
                                       TGNumberFormat::kNESRealTwo,
                                       TGNumberFormat::kNEAAnyNumber,
                                       TGNumberFormat::kNELLimitMinMax,-1.,1.);
   fTickLength->GetNumberEntry()->SetToolTipText("Set ticks' length");
   f2->AddFrame(fTickLength, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
   AddFrame(f2, new TGLayoutHints(kLHintsTop, 1, 1, 0, 0));

   TGCompositeFrame *f3 = new TGCompositeFrame(this, 80, 20, kHorizontalFrame);
   fTicksBoth = new TGCheckButton(f3, "+-", kAXIS_TICKSBOTH);
   fTicksBoth->SetToolTipText("Draw ticks on both axis sides");
   f3->AddFrame(fTicksBoth, new TGLayoutHints(kLHintsLeft | kLHintsBottom, 3, 1, 1, 0));
   fOptimize = new TGCheckButton(f3, "Optimize", kAXIS_OPTIM);
   fOptimize->SetState(kButtonDown);
   fOptimize->SetToolTipText("Optimize the number of axis divisions");
   f3->AddFrame(fOptimize, new TGLayoutHints(kLHintsTop, 28, 1, 1, 0));
   AddFrame(f3, new TGLayoutHints(kLHintsLeft, 1, 1, 0, 0));
   
   TGCompositeFrame *f4 = new TGCompositeFrame(this, 80, 20, kHorizontalFrame);
   fLogAxis = new TGCheckButton(f4, "Log", kAXIS_LOG);
   f4->AddFrame(fLogAxis, new TGLayoutHints(kLHintsLeft | kLHintsBottom, 3, 1, 0, 0));
   fLogAxis->SetToolTipText("Draw logarithmic scale");
   fMoreLog = new TGCheckButton(f4, "MoreLog", kAXIS_LBLLOG);
   f4->AddFrame(fMoreLog, new TGLayoutHints(kLHintsLeft, 19, 1, 0, 0));
   fMoreLog->SetToolTipText("Draw more logarithmic labels");
   AddFrame(f4, new TGLayoutHints(kLHintsTop, 1, 1, 0, 0));

   TGCompositeFrame *f5 = new TGCompositeFrame(this, 80, 20, kHorizontalFrame);
   fDiv3 = new TGNumberEntry(f5, 10, 2,kAXIS_DIV1, TGNumberFormat::kNESInteger,
                                       TGNumberFormat::kNEANonNegative, 
                                       TGNumberFormat::kNELLimitMinMax, 0, 99);
   fDiv3->GetNumberEntry()->SetToolTipText("Tertiary axis divisions");
   f5->AddFrame(fDiv3, new TGLayoutHints(kLHintsLeft, 27, 0, 1, 1));
   fDiv2 = new TGNumberEntry(f5, 5, 2, kAXIS_DIV2, TGNumberFormat::kNESInteger,
                                       TGNumberFormat::kNEANonNegative, 
                                       TGNumberFormat::kNELLimitMinMax, 0, 99);
   fDiv2->GetNumberEntry()->SetToolTipText("Secondary axis divisions");
   f5->AddFrame(fDiv2, new TGLayoutHints(kLHintsLeft, 1, 0, 1, 1));
   fDiv1 = new TGNumberEntry(f5, 0, 2, kAXIS_DIV3, TGNumberFormat::kNESInteger,
                                       TGNumberFormat::kNEANonNegative, 
                                       TGNumberFormat::kNELLimitMinMax, 0, 99);
   fDiv1->GetNumberEntry()->SetToolTipText("Primary axis divisions");
   f5->AddFrame(fDiv1, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 1, 1, 1, 1));
   AddFrame(f5, new TGLayoutHints(kLHintsLeft, 1, 1, 4, 4));
   fTicksFlag = 1;

   MakeTitle("Title");
  
   fTitlePrec = 2;
   fTitle = new TGTextEntry(this, new TGTextBuffer(50), kTITLE);
   fTitle->Resize(135, fTitle->GetDefaultHeight());
   fTitle->SetToolTipText("Enter the axis title string");
   AddFrame(fTitle, new TGLayoutHints(kLHintsLeft, 3, 1, 2, 1));
   
   TGCompositeFrame *f6 = new TGCompositeFrame(this, 80, 20, kHorizontalFrame);
   fTitleColor = new TGColorSelect(f6, 0, kCOL_TIT);
   f6->AddFrame(fTitleColor, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
   fTitleColor->Associate(this);
   TGLabel *fSizeLbl1 = new TGLabel(f6, "Size:");                              
   f6->AddFrame(fSizeLbl1, new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 6, 1, 1, 1));
   fTitleSize = new TGNumberEntry(f6, 0.05, 5, kAXIS_TITSIZE, 
                                      TGNumberFormat::kNESRealTwo,
                                      TGNumberFormat::kNEANonNegative, 
                                      TGNumberFormat::kNELLimitMinMax, 0., 1.);
   fTitleSize->GetNumberEntry()->SetToolTipText("Set title size");
   f6->AddFrame(fTitleSize, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
   AddFrame(f6, new TGLayoutHints(kLHintsTop, 1, 1, 0, 0));

   fTitleFont = new TGFontTypeComboBox(this, kFONT_TIT);
   fTitleFont->Resize(137, 20);
   AddFrame(fTitleFont, new TGLayoutHints(kLHintsLeft, 3, 1, 2, 1));

   TGCompositeFrame *f7 = new TGCompositeFrame(this, 80, 20, kHorizontalFrame);
   fCentered = new TGCheckButton(f7, "Centered", kAXIS_CENTERED);
   fCentered->SetToolTipText("Center axis title");
   f7->AddFrame(fCentered, new TGLayoutHints(kLHintsTop, 3, 1, 1, 0));
   TGLabel *fOffsetLbl = new TGLabel(f7, "Offset:");                              
   f7->AddFrame(fOffsetLbl, new TGLayoutHints(kLHintsLeft, 23, 1, 3, 0));
   AddFrame(f7, new TGLayoutHints(kLHintsTop, 1, 1, 0, 0));

   TGCompositeFrame *f8 = new TGCompositeFrame(this, 80, 20, kHorizontalFrame);
   fRotated = new TGCheckButton(f8, "Rotated", kAXIS_ROTATED);
   fRotated->SetState(kButtonDown);
   fRotated->SetToolTipText("Rotate axis title by 180 degrees");
   f8->AddFrame(fRotated, new TGLayoutHints(kLHintsTop, 3, 1, 6, 0));
   fTitleOffset = new TGNumberEntry(f8, 1.00, 6, kAXIS_TITOFFSET, 
                                        TGNumberFormat::kNESRealTwo,
                                        TGNumberFormat::kNEAAnyNumber, 
                                        TGNumberFormat::kNELLimitMinMax, 0.1, 10.);
   fTitleOffset->GetNumberEntry()->SetToolTipText("Set title offset");
   f8->AddFrame(fTitleOffset, new TGLayoutHints(kLHintsLeft, 6, 1, 0, 0));
   AddFrame(f8, new TGLayoutHints(kLHintsTop, 1, 1, 0, 4));

   MakeTitle("Labels");

   fLabelPrec = 2;
   TGCompositeFrame *f9 = new TGCompositeFrame(this, 80, 20, kHorizontalFrame);
   fLabelColor = new TGColorSelect(f9, 0, kCOL_LBL);
   f9->AddFrame(fLabelColor, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
   fLabelColor->Associate(this);
   TGLabel *fSizeLbl2 = new TGLabel(f9, "Size:");                              
   f9->AddFrame(fSizeLbl2, new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 1, 0, 1, 1));
   fLabelSize = new TGNumberEntry(f9, 0.05, 6, kAXIS_LBLSIZE, 
                                      TGNumberFormat::kNESRealTwo,
                                      TGNumberFormat::kNEANonNegative, 
                                      TGNumberFormat::kNELLimitMinMax, 0., 1.);
   fLabelSize->GetNumberEntry()->SetToolTipText("Set labels' size");
   f9->AddFrame(fLabelSize, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
   AddFrame(f9, new TGLayoutHints(kLHintsTop, 1, 1, 0, 0));

   TGCompositeFrame *f10 = new TGCompositeFrame(this, 80, 20, kHorizontalFrame);
   fNoExponent = new TGCheckButton(f10, "NoExp", kAXIS_LBLEXP);
   fNoExponent->SetToolTipText("Labels drawn without exponent notation");
   f10->AddFrame(fNoExponent, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 3, 1, 8, 0));
   fLabelOffset = new TGNumberEntry(f10, 0.005, 6, kAXIS_LBLOFFSET, 
                                         TGNumberFormat::kNESRealThree,
                                         TGNumberFormat::kNEAAnyNumber, 
                                         TGNumberFormat::kNELLimitMinMax,-1.,1.);
   fLabelOffset->GetNumberEntry()->SetToolTipText("Set labels' offset");
   f10->AddFrame(fLabelOffset, new TGLayoutHints(kLHintsLeft, 11, 1, 3, 0));
   AddFrame(f10, new TGLayoutHints(kLHintsTop, 1, 1, 0, 0));

   fLabelFont = new TGFontTypeComboBox(this, kFONT_LBL);
   fLabelFont->Resize(137, 20);
   AddFrame(fLabelFont, new TGLayoutHints(kLHintsLeft, 3, 1, 2, 0));
   fLabelFont->Associate(this);

   fDecimal = new TGCheckButton(this, "Decimal labels' part", kAXIS_LBLDEC);
   fDecimal->SetToolTipText("Draw the decimal part of labels");
   AddFrame(fDecimal, new TGLayoutHints(kLHintsLeft | kLHintsBottom, 3, 1, 3, 0));

   MapSubwindows();
   Layout();
   MapWindow();
      
   TClass *cl = TAxis::Class();
   TGedElement *ge = new TGedElement;
   ge->fGedFrame = this;
   ge->fCanvas = 0;
   cl->GetEditorList()->Add(ge);
}

//______________________________________________________________________________
TAxisEditor::~TAxisEditor()
{
   // Destructor of axis editor.

   TGFrameElement *el;
   TIter next(GetList());
   
   while ((el = (TGFrameElement *)next())) {
      if (!strcmp(el->fFrame->ClassName(), "TGCompositeFrame"))
         ((TGCompositeFrame *)el->fFrame)->Cleanup();
   }
   Cleanup();
}

//______________________________________________________________________________
void TAxisEditor::ConnectSignals2Slots()
{
   // Connect signals to slots.

   fAxisColor->Connect("ColorSelected(Pixel_t)", "TAxisEditor", this, "DoAxisColor(Pixel_t)");
   fTickLength->Connect("ValueSet(Long_t)", "TAxisEditor", this, "DoTickLength()");
   (fTickLength->GetNumberEntry())->Connect("ReturnPressed()", "TAxisEditor", this, "DoTickLength()");
   fTicksBoth->Connect("Toggled(Bool_t)","TAxisEditor",this,"DoTicks()");
   fOptimize->Connect("Toggled(Bool_t)","TAxisEditor",this,"DoDivisions()");
   fLogAxis->Connect("Toggled(Bool_t)","TAxisEditor",this,"DoLogAxis()");
   fMoreLog->Connect("Toggled(Bool_t)","TAxisEditor",this,"DoMoreLog()");
   fDiv3->Connect("ValueSet(Long_t)", "TAxisEditor", this, "DoDivisions()");
   (fDiv3->GetNumberEntry())->Connect("ReturnPressed()", "TAxisEditor", this, "DoDivisions()");
   fDiv2->Connect("ValueSet(Long_t)", "TAxisEditor", this, "DoDivisions()");
   (fDiv2->GetNumberEntry())->Connect("ReturnPressed()", "TAxisEditor", this, "DoDivisions()");
   fDiv1->Connect("ValueSet(Long_t)", "TAxisEditor", this, "DoDivisions()");
   (fDiv1->GetNumberEntry())->Connect("ReturnPressed()", "TAxisEditor", this, "DoDivisions()");
   fTitle->Connect("TextChanged(const char *)", "TAxisEditor", this, "DoTitle(const char *)");;
   fTitleColor->Connect("ColorSelected(Pixel_t)", "TAxisEditor", this, "DoTitleColor(Pixel_t)");
   fTitleSize->Connect("ValueSet(Long_t)", "TAxisEditor", this, "DoTitleSize()");
   (fTitleSize->GetNumberEntry())->Connect("ReturnPressed()", "TAxisEditor", this, "DoTitleSize()");
   fTitleFont->Connect("Selected(Int_t)", "TAxisEditor", this, "DoTitleFont(Int_t)"); 
   fCentered->Connect("Toggled(Bool_t)","TAxisEditor",this, "DoTitleCentered()");
   fRotated->Connect("Toggled(Bool_t)","TAxisEditor",this,"DoTitleRotated()");
   fTitleOffset->Connect("ValueSet(Long_t)","TAxisEditor",this,"DoTitleOffset()");
   (fTitleOffset->GetNumberEntry())->Connect("ReturnPressed()","TAxisEditor",this,"DoTitleOffset()");
   fLabelColor->Connect("ColorSelected(Pixel_t)", "TAxisEditor", this, "DoLabelColor(Pixel_t)");
   fLabelSize->Connect("ValueSet(Long_t)","TAxisEditor",this,"DoLabelSize()");
   (fLabelSize->GetNumberEntry())->Connect("ReturnPressed()","TAxisEditor",this,"DoLabelSize()");
   fNoExponent->Connect("Toggled(Bool_t)","TAxisEditor",this,"DoNoExponent()");
   fDecimal->Connect("Toggled(Bool_t)","TAxisEditor",this,"DoDecimal(Bool_t)");
   fLabelOffset->Connect("ValueSet(Long_t)", "TAxisEditor", this, "DoLabelOffset()");
   (fLabelOffset->GetNumberEntry())->Connect("ReturnPressed()", "TAxisEditor", this, "DoLabelOffset()");
   fLabelFont->Connect("Selected(Int_t)", "TAxisEditor", this, "DoLabelFont(Int_t)"); 
   fInit = kFALSE;
}

//______________________________________________________________________________
void TAxisEditor::SetModel(TVirtualPad* pad, TObject* obj, Int_t)
{
   // Pick up the used values of axis attributes.
   
   fModel = 0;
   fPad = 0;

   if (obj == 0 || !obj->InheritsFrom("TAxis")) {
      SetActive(kFALSE);
      return;
   }

   fModel = obj;
   fPad = pad;

   fAxis = (TAxis *)fModel;

   Color_t c = fAxis->GetAxisColor();
   Pixel_t p = TColor::Number2Pixel(c);
   fAxisColor->SetColor(p);

   Float_t fl = fAxis->GetTickLength();
   fTickLength->SetNumber(fl);
   
   Int_t div = fAxis->GetNdivisions();
   fDiv1->SetNumber(div % 100);
   fDiv2->SetNumber((div/100) % 100);
   fDiv3->SetNumber((div/10000) % 100);
   
   if ((!strcmp(fAxis->GetName(),"xaxis") && fPad->GetLogx()) ||
       (!strcmp(fAxis->GetName(),"yaxis") && fPad->GetLogy()) ||
       (!strcmp(fAxis->GetName(),"zaxis") && fPad->GetLogz())) 

      fLogAxis->SetState(kButtonDown);
   else fLogAxis->SetState(kButtonUp);

   if (fLogAxis->GetState() == kButtonUp) {
      fMoreLog->SetState(kButtonDisabled);
   } else {
      Int_t morelog = fAxis->GetMoreLogLabels();
      if (morelog) fMoreLog->SetState(kButtonDown);
      else         fMoreLog->SetState(kButtonUp);
   }
   
   const char *both = fAxis->GetTicks();
   if (!strcmp(both,"+-")) {
      fTicksBoth->SetState(kButtonDown);
   } else {
      fTicksBoth->SetState(kButtonUp);
      if (!strcmp(both,"-")) fTicksFlag = -1;
      if (!strcmp(both,"+")) fTicksFlag =  1;
   }

   const char *text = fAxis->GetTitle();
   fTitle->SetText(text);
   
   c = fAxis->GetTitleColor();
   p = TColor::Number2Pixel(c);
   fTitleColor->SetColor(p);

   fl = fAxis->GetTitleSize();
   fTitleSize->SetNumber(fl);

   Style_t font = fAxis->GetTitleFont();
   fTitleFont->Select(font / 10);
   fTitlePrec = (Int_t)(font % 10);

   fl = fAxis->GetTitleOffset();
   fTitleOffset->SetNumber(fl);

   Int_t centered = fAxis->GetCenterTitle();
   if (centered) fCentered->SetState(kButtonDown);
   else          fCentered->SetState(kButtonUp);
   
   Int_t rotated = fAxis->GetRotateTitle();
   if (rotated) fRotated->SetState(kButtonDown);
   else         fRotated->SetState(kButtonUp);

   c = fAxis->GetLabelColor();
   p = TColor::Number2Pixel(c);
   fLabelColor->SetColor(p);

   fl = fAxis->GetLabelSize();
   fLabelSize->SetNumber(fl);

   font = fAxis->GetLabelFont();
   fLabelFont->Select(font / 10);
   fLabelPrec = (Int_t)(font % 10);

   fl = fAxis->GetLabelOffset();
   fLabelOffset->SetNumber(fl);

   Int_t noexp = fAxis->GetNoExponent();
   if (noexp) fNoExponent->SetState(kButtonDown);
   else       fNoExponent->SetState(kButtonUp);

   Bool_t on = fAxis->GetDecimals();
   if (on) fDecimal->SetState(kButtonDown);
   else    fDecimal->SetState(kButtonUp);
   
   if (fInit) ConnectSignals2Slots();
   SetActive();
}

//______________________________________________________________________________
void TAxisEditor::DoAxisColor(Pixel_t color)
{
   // Slot connected to the axis color.
   
   fAxis->SetAxisColor(TColor::GetColor(color));
   Update();
}

//______________________________________________________________________________
void TAxisEditor::DoTickLength()
{
   // Slot connected to the tick length settings.

   Float_t ticks = fTickLength->GetNumber();
   fAxis->SetTickLength(ticks);
   Update();
   if (fTickLength->GetNumber() < 0) fTicksFlag = -1;
   else fTicksFlag = 1;
}

//______________________________________________________________________________
void TAxisEditor::DoTicks()
{
   // Slot connected to the ticks draw settings.

   if (fTicksBoth->GetState() == kButtonDown) {
      fAxis->SetTicks("+-");
   } else {
      if (fTicksFlag == -1) fAxis->SetTicks("-");
      else fAxis->SetTicks("");
   }
   Update();
}

//______________________________________________________________________________
void TAxisEditor::DoDivisions()
{
   // Slot connected to the number of divisions.

   // the number of divisions are used 3 number entry widgets
   Int_t div = (Int_t)(fDiv1->GetNumber() + fDiv2->GetNumber()  * 100 
                                          + fDiv3->GetNumber() * 10000);
   fAxis->SetNdivisions(div, fOptimize->GetState());
   Update();
}

//______________________________________________________________________________
void TAxisEditor::DoLogAxis()
{
   // Slot for Log scale setting.

   gPad = fPad;
      
   if (fLogAxis->GetState() == kButtonDown) {

      if (!strcmp(fAxis->GetName(),"xaxis")) gPad->SetLogx(1);
      if (!strcmp(fAxis->GetName(),"yaxis")) gPad->SetLogy(1);
      if (!strcmp(fAxis->GetName(),"zaxis")) gPad->SetLogz(1);

      Int_t morelog = fAxis->GetMoreLogLabels();
      if (morelog)  fMoreLog->SetState(kButtonDown);
      else          fMoreLog->SetState(kButtonUp);
      fOptimize->SetState(kButtonDisabled);

   } else if (fLogAxis->GetState() == kButtonUp){
      if (!strcmp(fAxis->GetName(),"xaxis")) gPad->SetLogx(0);
      if (!strcmp(fAxis->GetName(),"yaxis")) gPad->SetLogy(0);
      if (!strcmp(fAxis->GetName(),"zaxis")) gPad->SetLogz(0);
      fMoreLog->SetState(kButtonDisabled);
      fOptimize->SetState(kButtonDown);
   }
   Update();
}

//______________________________________________________________________________
void TAxisEditor::DoMoreLog()
{
   // Slot connected to more Log labels flag

   Int_t flag = 0;
   if (fMoreLog->GetState() == kButtonDown)    flag = 1;
   else if (fMoreLog->GetState() == kButtonUp) flag = 0;
   fAxis->SetMoreLogLabels(flag);
   Update();
}

//______________________________________________________________________________
void TAxisEditor::DoTitle(const char *text)
{
   // Slot connected to the axis color.
   
   fAxis->SetTitle(text);
   Update();
}

//______________________________________________________________________________
void TAxisEditor::DoTitleColor(Pixel_t color)
{
   // Slot connected to the title color.
   
   fAxis->SetTitleColor(TColor::GetColor(color));
   Update();
}

//______________________________________________________________________________
void TAxisEditor::DoTitleSize()
{
   // Slot connected to the title font size.

   Float_t size = fTitleSize->GetNumber();
   fAxis->SetTitleSize(size);
   Update();
}

//______________________________________________________________________________
void TAxisEditor::DoTitleFont(Int_t font)
{
   // Slot connected to the title font.

   Int_t f = font * 10 + fTitlePrec;
   fAxis->SetTitleFont(f);
   Update();
}

//______________________________________________________________________________
void TAxisEditor::DoTitleOffset()
{
   // Slot connected to the title offset.

   Float_t offset = fTitleOffset->GetNumber();
   fAxis->SetTitleOffset(offset);
   Update();
}

//______________________________________________________________________________
void TAxisEditor::DoTitleCentered()
{
   // Slot connected to centered title option.

   Int_t ctr = fCentered->GetState();
   fAxis->CenterTitle(ctr);
   Update();
}

//______________________________________________________________________________
void TAxisEditor::DoTitleRotated()
{
   // Slot connected to the title rotation.

   Int_t rot = fRotated->GetState();
   fAxis->RotateTitle(rot);
   Update();
}

//______________________________________________________________________________
void TAxisEditor::DoLabelColor(Pixel_t color)
{
   // Slot connected to the label color.

   fAxis->SetLabelColor(TColor::GetColor(color));
   Update();
}

//______________________________________________________________________________
void TAxisEditor::DoLabelSize()
{
   // Slot connected to the label size.

   Float_t size = fLabelSize->GetNumber();
   fAxis->SetLabelSize(size);
   Update();
}

//______________________________________________________________________________
void TAxisEditor::DoLabelFont(Int_t font)
{
   // Slot connected to the label font.

   Int_t f = font * 10 + fLabelPrec;
   fAxis->SetLabelFont(f);
   Update();
}

//______________________________________________________________________________
void TAxisEditor::DoLabelOffset()
{
   // Slot connected to the label offset.

   Float_t offset = fLabelOffset->GetNumber();
   fAxis->SetLabelOffset(offset);
   Update();
}

//______________________________________________________________________________
void TAxisEditor::DoNoExponent()
{
   // Slot connected to the labels' exponent flag.

   Int_t exp = fNoExponent->GetState();
   fAxis->SetNoExponent(exp);
   Update();
}

//______________________________________________________________________________
void TAxisEditor::DoDecimal(Bool_t on)
{
   // Slot connected to the decimal part setting.

   fAxis->SetDecimals(on);
   gStyle->SetStripDecimals(!on);
   Update();
   gPad->Modified();
   gPad->Update();
}

