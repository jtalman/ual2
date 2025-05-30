// @(#)root/treeplayer:$Name:  $:$Id: TFileDrawMap.cxx,v 1.5 2003/12/30 13:16:51 brun Exp $
// Author: Rene Brun   15/01/2003

/*************************************************************************
 * Copyright (C) 1995-2003, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TFileDrawMap                                                         //
//                                                                      //
// This class is automatically called by TFile::DrawMap
// It draws a canvas showing the internal structure of a ROOT file.
// Each key or basket in a file is shown with a fill area drawn
// at the byte position of the key/basket in the file.
// The Y axis of the canvas shows the number of Kbytes/Mbytes.
// The X axis shows the bytes between y(i) and y(i+1).
// A color corresponding to the class in the key/basket is automatically
// selected using the class unique identifier.
//
// When moving the mouse in the canvas, the "Event Status" panels
// shows the object corresponding to the mouse position.
// if the object is a key, it shows the class and object name as well as
//    the file directory name if the file has sub-directories.
// if the object is a basket, it shows:
//   -the name of the Tree
//   -the name of the branch
//   -the basket number
//   -the entry number in the basket
//
// Special keys like the StreamerInfo record, the Keys List Record
// and the Free Blocks Record are also shown.
//
// When clicking the right mouse button, a pop-up menu is shown
// with its title identifying the picked object and with the items:
//   -DrawObject: in case of a key, the Draw function of the object is called
//                in case of a basket, the branch is drawn for all entries
//   -DumpObject: in case of a key, the Dump function of the object is called
//                in case of a basket, tree->Show(entry) is called
//   -InspectObject: the Inspect function is called for the object.
//
// The normal axis zoom functionality can be used to zoom or unzoom
// One can also use the TCanvas context menu SetCanvasSize to make
// a larger canvas and use the canvas scroll bars.
//
// When the class is built, it is possible to identify a subset of the
// objects to be shown. For example, to view only the keys with
// names starting with "abc", set the argument keys to "abc*".
// The default is to view all the objects.
// The argument options can also be used (only one option currently)
// When the option "same" is given, the new picture is suprimposed.
// The option "same" is useful, eg:
//  to draw all keys with names = "abc" in a first pass
// then all keys with names = "uv*" in a second pass, etc.
//
//Begin_Html
/*
<img src="gif/filedrawmap.gif">
*/
//End_Html
//
//  =============================================================================

#include "TFileDrawMap.h"
#include "TROOT.h"
#include "TClass.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TMath.h"
#include "TVirtualPad.h"
#include "TStyle.h"
#include "TH1.h"
#include "TBox.h"
#include "TKey.h"
#include "TRegexp.h"
#include "TSystem.h"

ClassImp(TFileDrawMap)

//______________________________________________________________________________
TFileDrawMap::TFileDrawMap() :TNamed()
{
// Default TreeFileMap constructor

   fFile   = 0;
   fFrame  = 0;
}

//______________________________________________________________________________
TFileDrawMap::TFileDrawMap(const TFile *file, const char *keys, Option_t *option)
         : TNamed("TFileDrawMap","")
{
// TFileDrawMap normal constructor
// see descriptions of arguments above
   
   fFile     = (TFile*)file;
   fKeys     = keys;
   fOption   = option;
   fOption.ToLower();
   SetBit(kCanDelete);
   
   //create histogram used to draw the map frame

   if (file->GetEND() > 1000000) {
      fXsize = 1000000;
   } else {
      fXsize = 1000;
   }
   fFrame = new TH1D("hmapframe","",1000,0,fXsize);
   fFrame->SetDirectory(0);
   fFrame->SetBit(TH1::kNoStats);
   fFrame->SetBit(kCanDelete);
   fFrame->SetMinimum(0);
   if (fXsize > 1000) {
      fFrame->GetYaxis()->SetTitle("MBytes");
   } else {
      fFrame->GetYaxis()->SetTitle("KBytes");
   }
   fFrame->GetXaxis()->SetTitle("Bytes");
   fYsize = 1 + Int_t(file->GetEND()/fXsize);
   fFrame->SetMaximum(fYsize);
   fFrame->GetYaxis()->SetLimits(0,fYsize);
   
   //Bool_t show = kFALSE;
   if (gPad) {
      gPad->Clear();
      //show = gPad->GetCanvas()->GetShowEventStatus();
   }
   Draw();
   if (gPad) {
      //if (!show) gPad->GetCanvas()->ToggleEventStatus();
      gPad->Update();
   }
}

//______________________________________________________________________________
TFileDrawMap::~TFileDrawMap()
{
//*-*-*-*-*-*-*-*-*-*-*Tree destructor*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  =================

   //delete fFrame; //should not be deleted (kCanDelete set)
}

//______________________________________________________________________________
void  TFileDrawMap::AnimateTree(const char *branches)
{
// Show sequence of baskets reads for the list of baskets involved 
// in the list of branches (separated by ",")
// if branches="", the branch pointed by the mouse is taken.
// if branches="*", all branches are taken
// Example:
//  AnimateTree("x,y,u");
   
   char info[512];
   strcpy(info,GetName());
   char *cbasket = strstr(info,", basket=");
   if (!cbasket) return;
   *cbasket = 0;
   char *cbranch = strstr(info,", branch=");
   if (!cbranch) return;
   *cbranch = 0;
   cbranch += 9;
   TTree *tree = (TTree*)fFile->Get(info);
   if (!tree) return;
   if (strlen(branches) > 0) strcpy(info,branches);
   else                      strcpy(info,cbranch);
   printf("Animating tree, branches=%s\n",info);      
   
   // create list of branches
   Int_t nzip = 0;
   TBranch *branch;
   TObjArray list;
   char *comma;
   while((comma = strrchr(info,','))) {
      *comma = 0;
      comma++;
      while (*comma == ' ') comma++;
      branch = tree->GetBranch(comma);
      if (branch) {
         nzip += (Int_t)branch->GetZipBytes(); 
         branch->SetUniqueID(0);
         list.Add(branch);
      }
   }
   comma = info;
   while (*comma == ' ') comma++;
   branch = tree->GetBranch(comma);
   if (branch) {
      nzip += (Int_t)branch->GetZipBytes(); 
      branch->SetUniqueID(0);
      list.Add(branch);
   }
   Double_t fractionRead = Double_t(nzip)/Double_t(fFile->GetEND());
   Int_t nbranches = list.GetEntries();
   
   // loop on all tree entries
   Int_t nentries = (Int_t)tree->GetEntries();
   Int_t sleep = 1;
   Int_t stime = (Int_t)(100./(nentries*fractionRead));
   if (stime < 10) {stime=1; sleep = nentries/400;}
   gPad->SetDoubleBuffer(0);             // turn off double buffer mode
   gVirtualX->SetDrawMode(TVirtualX::kInvert);  // set the drawing mode to XOR mode
   for (Int_t entry=0;entry<nentries;entry++) {
      for (Int_t ib=0;ib<nbranches;ib++) {
         branch = (TBranch*)list.At(ib);
         Int_t nbaskets = branch->GetListOfBaskets()->GetSize();
         Int_t basket = TMath::BinarySearch(nbaskets,branch->GetBasketEntry(),entry);
         Int_t nbytes = branch->GetBasketBytes()[basket];
         Int_t bseek  = branch->GetBasketSeek(basket);
         Int_t entry0 = branch->GetBasketEntry()[basket];
         Int_t entryn = branch->GetBasketEntry()[basket+1];
         Int_t eseek  = (Int_t)(bseek + nbytes*Double_t(entry-entry0)/Double_t(entryn-entry0));
         DrawMarker(ib,branch->GetUniqueID());
         DrawMarker(ib,eseek);
         branch->SetUniqueID(eseek);
         gSystem->ProcessEvents();
         if (entry%sleep == 0) gSystem->Sleep(stime);
      }
   }
}

//______________________________________________________________________________
Int_t TFileDrawMap::DistancetoPrimitive(Int_t px, Int_t py)
{
// Compute distance from point px,py to this TreeFileMap
// Find the closest object to the mouse, save its path in the TFileDrawMap name.
   
   Int_t pxmin = gPad->XtoAbsPixel(gPad->GetUxmin());
   Int_t pxmax = gPad->XtoAbsPixel(gPad->GetUxmax());
   Int_t pymin = gPad->YtoAbsPixel(gPad->GetUymin());
   Int_t pymax = gPad->YtoAbsPixel(gPad->GetUymax());
   if (px > pxmin && px < pxmax && py > pymax && py < pymin) {
      SetName(GetObjectInfo(px,py));
      return 0;
   }
   return fFrame->DistancetoPrimitive(px,py);
}

//______________________________________________________________________________
void TFileDrawMap::DrawMarker(Int_t marker, Long64_t eseek)
{
// Draw marker
   
   Int_t iy = gPad->YtoAbsPixel(eseek/fXsize);
   Int_t ix = gPad->XtoAbsPixel(eseek%fXsize);
   Int_t d;
   Int_t mark = marker%4;
   switch (mark) {
      case 0 : d = 6; //arrow
              gVirtualX->DrawLine(ix-3*d,iy,ix,iy);
              gVirtualX->DrawLine(ix-d,iy+d,ix,iy);
              gVirtualX->DrawLine(ix-d,iy-d,ix,iy);
              gVirtualX->DrawLine(ix-d,iy-d,ix-d,iy+d);
              break;
      case 1 : d = 5; //up triangle
              gVirtualX->DrawLine(ix-d,iy-d,ix+d,iy-d);
              gVirtualX->DrawLine(ix+d,iy-d,ix,iy+d);
              gVirtualX->DrawLine(ix,iy+d,ix-d,iy-d);
              break;
      case 2 : d = 5; //open square
              gVirtualX->DrawLine(ix-d,iy-d,ix+d,iy-d);
              gVirtualX->DrawLine(ix+d,iy-d,ix+d,iy+d);
              gVirtualX->DrawLine(ix+d,iy+d,ix-d,iy+d);
              gVirtualX->DrawLine(ix-d,iy+d,ix-d,iy-d);
              break;
      case 3 : d = 8; //cross
              gVirtualX->DrawLine(ix-d,iy,ix+d,iy);
              gVirtualX->DrawLine(ix,iy-d,ix,iy+d);
              break;
   }
}
 
//______________________________________________________________________________
void TFileDrawMap::DrawObject()
{
// Draw object at the mouse position

   TVirtualPad *padsave = gROOT->GetSelectedPad();
   if (padsave == gPad) {
      //must create a new canvas
      if (!gROOT->GetMakeDefCanvas()) return;
      (gROOT->GetMakeDefCanvas())();
   } else {
      padsave->cd();
   }
   
   // case of a TTree
   char info[512];
   strcpy(info,GetName());
   char *cbasket = (char*)strstr(info,", basket=");
   if (cbasket) {
      *cbasket = 0;
      char *cbranch = (char*)strstr(info,", branch=");
      if (!cbranch) return;
      *cbranch = 0;
      cbranch += 9;
      TTree *tree = (TTree*)fFile->Get(info);
      if (tree) tree->Draw(cbranch);
      return;
   }
   
   // other objects
   TObject *obj = GetObject();
   if (obj) obj->Draw();
}


//______________________________________________________________________________
void TFileDrawMap::DumpObject()
{
// Dump object at the mouse position

   TObject *obj = GetObject();
   if (obj) {
      obj->Dump();
      return;
   }
   char *centry = (char*)strstr(GetName(),"entry=");
   if (!centry) return;
   Int_t entry = 0;
   sscanf(centry+6,"%d",&entry);
   char info[512];
   strcpy(info,GetName());
   char *colon = (char*)strstr(info,"::");
   if (!colon) return;
   colon--;
   *colon = 0;
   TTree *tree = (TTree*)fFile->Get(info);
   if (tree) tree->Show(entry);   
}

//______________________________________________________________________________
void TFileDrawMap::ExecuteEvent(Int_t event, Int_t px, Int_t py)
{
// Execute action corresponding to one event
   
   fFrame->ExecuteEvent(event,px,py);
}

//______________________________________________________________________________
TObject *TFileDrawMap::GetObject()
{
// Retrieve object at the mouse position in memory

   if (strstr(GetName(),"entry=")) return 0;
   char info[512];
   strcpy(info,GetName());
   char *colon = strstr(info,"::");
   if (!colon) return 0;
   colon--;
   *colon = 0;
   return fFile->Get(info);
}

//______________________________________________________________________________
char *TFileDrawMap::GetObjectInfo(Int_t px, Int_t py) const
{
//   Redefines TObject::GetObjectInfo.
//   Displays the keys info in the file corresponding to cursor position px,py
//   in the canvas status bar info panel
      
   static char info[512];
   GetObjectInfoDir(fFile, px, py, info);
   return info;
}

//______________________________________________________________________________
Bool_t TFileDrawMap::GetObjectInfoDir(TDirectory *dir, Int_t px, Int_t py, char *info) const
{
//   Redefines TObject::GetObjectInfo.
//   Displays the keys info in the directory
//   corresponding to cursor position px,py
//
   Double_t x = gPad->AbsPixeltoX(px);
   Double_t y = gPad->AbsPixeltoY(py);
   Int_t iy   = (Int_t)y;
   Long64_t pbyte = (Long64_t)(fXsize*iy+x);
   Int_t nbytes;
   Long64_t bseek;
   TDirectory *dirsav = gDirectory;
   dir->cd();
   
   TIter next(dir->GetListOfKeys());
   TKey *key;
   while ((key = (TKey*)next())) {
      TDirectory *curdir = gDirectory;
      TClass *cl = gROOT->GetClass(key->GetClassName());
      // a TDirectory ?
      if (cl && cl == TDirectory::Class()) {
         curdir->cd(key->GetName());
         TDirectory *subdir = gDirectory;
         Bool_t gotInfo = GetObjectInfoDir(subdir, px, py, info);
         if (gotInfo) {
            dirsav->cd();
            return kTRUE;
         }
         curdir->cd();
         continue;
      }
      // a TTree ?
      if (cl && cl->InheritsFrom(TTree::Class())) {
         TTree *tree = (TTree*)gDirectory->Get(key->GetName());
         TIter nextb(tree->GetListOfLeaves());
         TLeaf *leaf;
         while ((leaf = (TLeaf*)nextb())) {
            TBranch *branch = leaf->GetBranch();
            Int_t nbaskets = branch->GetMaxBaskets();
            Int_t offsets = branch->GetEntryOffsetLen();
            Int_t len = leaf->GetLen();
            for (Int_t i=0;i<nbaskets;i++) {
               bseek = branch->GetBasketSeek(i);
               if (!bseek) break;
               nbytes = branch->GetBasketBytes()[i];
               if (pbyte >= bseek && pbyte < bseek+nbytes) {
                  Int_t entry = branch->GetBasketEntry()[i];
                  if (!offsets) entry += (pbyte-bseek)/len;
                  if (curdir == (TDirectory*)fFile) {
                     sprintf(info,"%s%s, branch=%s, basket=%d, entry=%d",curdir->GetPath(),key->GetName(),branch->GetName(),i,entry);
                  } else {
                     sprintf(info,"%s/%s, branch=%s, basket=%d, entry=%d",curdir->GetPath(),key->GetName(),branch->GetName(),i,entry);
                  }
                  return kTRUE;
               }
            }
         }
      }
      nbytes = key->GetNbytes();
      bseek = key->GetSeekKey();
      if (pbyte >= bseek && pbyte < bseek+nbytes) {
         if (curdir == (TDirectory*)fFile) {
            sprintf(info,"%s%s ::%s, nbytes=%d",curdir->GetPath(),key->GetName(),key->GetClassName(),nbytes);
         } else {
            sprintf(info,"%s/%s ::%s, nbytes=%d",curdir->GetPath(),key->GetName(),key->GetClassName(),nbytes);
         }
         dirsav->cd();
         return kTRUE;            
      }
   }
   // Are we in the Keys list
   if (pbyte >= dir->GetSeekKeys() && pbyte < dir->GetSeekKeys()+dir->GetNbytesKeys()) {
      sprintf(info,"%sKeys List, nbytes=%d",dir->GetPath(),dir->GetNbytesKeys());
      dirsav->cd();
      return kTRUE;            
   }
   if (dir == (TDirectory*)fFile) {
      // Are we in the TStreamerInfo
      if (pbyte >= fFile->GetSeekInfo() && pbyte < fFile->GetSeekInfo()+fFile->GetNbytesInfo()) {
         sprintf(info,"%sStreamerInfo List, nbytes=%d",dir->GetPath(),fFile->GetNbytesInfo());
         dirsav->cd();
         return kTRUE;            
      }
      // Are we in the Free Segments
      if (pbyte >= fFile->GetSeekFree() && pbyte < fFile->GetSeekFree()+fFile->GetNbytesFree()) {
         sprintf(info,"%sFree List, nbytes=%d",dir->GetPath(),fFile->GetNbytesFree());
         dirsav->cd();
         return kTRUE;            
      }
   }
   sprintf(info,"(byte=%lld)",pbyte);
   dirsav->cd();
   return kFALSE;
}

//______________________________________________________________________________
void TFileDrawMap::InspectObject()
{
// Inspect object at the mouse position

   TObject *obj = GetObject();
   if (obj) obj->Inspect();
}

//______________________________________________________________________________
void TFileDrawMap::Paint(Option_t *)
{
//  Paint this TFileDrawMap

   // draw map frame
   if (!fOption.Contains("same")) {
      gPad->Clear();
      //just in case axis Y has been unzoomed
      if (fFrame->GetMaximumStored() < -1000) {
         fFrame->SetMaximum(fYsize+1);
         fFrame->SetMinimum(0);
         fFrame->GetYaxis()->SetLimits(0,fYsize+1);
      }
      fFrame->Paint("a"); 
   }
   
   //draw keys
   PaintDir(fFile, fKeys.Data());
   
   fFrame->Draw("sameaxis");
}

//______________________________________________________________________________
void TFileDrawMap::PaintBox(TBox &box, Long64_t bseek, Int_t nbytes)
{
// Paint the object at bseek with nbytes using the box object

   Int_t iy = bseek/fXsize;
   Int_t ix = bseek%fXsize;
   Int_t ny = 1+(nbytes+ix)/fXsize;
   Double_t xmin,ymin,xmax,ymax;
   for (Int_t j=0;j<ny;j++) {
      if (j == 0) xmin = (Double_t)ix;
      else        xmin = 0;
      xmax = xmin + nbytes;
      if (xmax > fXsize) xmax = fXsize;
      ymin = iy+j;
      ymax = ymin+1;
      nbytes -= (Int_t)(xmax-xmin);
      if (xmax < gPad->GetUxmin()) continue;
      if (xmin > gPad->GetUxmax()) continue;
      if (xmin < gPad->GetUxmin()) xmin = gPad->GetUxmin();
      if (xmax > gPad->GetUxmax()) xmax = gPad->GetUxmax();
      if (ymax < gPad->GetUymin()) continue;
      if (ymin > gPad->GetUymax()) continue;
      if (ymin < gPad->GetUymin()) ymin = gPad->GetUymin();
      if (ymax > gPad->GetUymax()) ymax = gPad->GetUymax();
      //box.TAttFill::Modify();
      box.PaintBox(xmin,ymin,xmax,ymax);
   }
}

//______________________________________________________________________________
void TFileDrawMap::PaintDir(TDirectory *dir, const char *keys)
{
// Paint keys in a directory
   
   TDirectory *dirsav = gDirectory;
   TIter next(dir->GetListOfKeys());
   TKey *key;
   Int_t color = 0;
   TBox box;
   TRegexp re(keys,kTRUE);
   while ((key = (TKey*)next())) {
      Int_t nbytes = key->GetNbytes();
      Long64_t bseek = key->GetSeekKey();
      TClass *cl = gROOT->GetClass(key->GetClassName());
      if (cl) {
         color = (Int_t)(cl->GetUniqueID()%20);
      } else {
         color = 1;
      }
      box.SetFillColor(color);
      box.SetFillStyle(1001);
      TString s = key->GetName();
      if (strcmp(fKeys.Data(),key->GetName()) && s.Index(re) == kNPOS) continue;
      // a TDirectory ?
      if (cl && cl == TDirectory::Class()) {
         TDirectory *curdir = gDirectory;
         gDirectory->cd(key->GetName());
         TDirectory *subdir = gDirectory;
         PaintDir(subdir,"*");
         curdir->cd();
      }
      PaintBox(box,bseek,nbytes);
      // a TTree ?
      if (cl && cl->InheritsFrom(TTree::Class())) {
         TTree *tree = (TTree*)gDirectory->Get(key->GetName());
         TIter nextb(tree->GetListOfLeaves());
         TLeaf *leaf;
         while ((leaf = (TLeaf*)nextb())) {
            TBranch *branch = leaf->GetBranch();
            color = (Int_t)(branch->IsA()->GetUniqueID()%20);
            box.SetFillColor(color);
            Int_t nbaskets = branch->GetMaxBaskets();
            for (Int_t i=0;i<nbaskets;i++) {
               Long64_t bseek = branch->GetBasketSeek(i);
               if (!bseek) break;
               Int_t nbytes = branch->GetBasketBytes()[i];
               PaintBox(box,bseek,nbytes);
            }
         }
      }
   }
   // draw the box for Keys list
   box.SetFillColor(50);
   box.SetFillStyle(1001);
   PaintBox(box,dir->GetSeekKeys(),dir->GetNbytesKeys());
   if (dir == (TDirectory*)fFile) {
      // draw the box for TStreamerInfo
      box.SetFillColor(6);
      box.SetFillStyle(3008);
      PaintBox(box,fFile->GetSeekInfo(),fFile->GetNbytesInfo());
      // draw the box for Free Segments
      box.SetFillColor(1);
      box.SetFillStyle(1001);
      PaintBox(box,fFile->GetSeekFree(),fFile->GetNbytesFree());
   }
   dirsav->cd();
}
