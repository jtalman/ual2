// @(#)root/proof:$Name:  $:$Id: TEventIter.cxx,v 1.14 2005/03/10 17:57:04 rdm Exp $
// Author: Maarten Ballintijn   07/01/02

/*************************************************************************
 * Copyright (C) 1995-2001, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TEventIter                                                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TEventIter.h"

#include "TCollection.h"
#include "TDSet.h"
#include "TError.h"
#include "TFile.h"
#include "TKey.h"
#include "TProofDebug.h"
#include "TSelector.h"
#include "TTimeStamp.h"
#include "TTree.h"
#include "TVirtualPerfStats.h"
#include "TEventList.h"


//------------------------------------------------------------------------

ClassImp(TEventIter)


//______________________________________________________________________________
TEventIter::TEventIter()
{
   fDSet  = 0;
   fElem  = 0;
   fFile  = 0;
   fDir   = 0;
   fSel   = 0;
   fFirst = 0;
   fCur   = -1;
   fNum   = 0;
   fStop  = kFALSE;
}


//______________________________________________________________________________
TEventIter::TEventIter(TDSet *dset, TSelector *sel, Long64_t first, Long64_t num)
   : fDSet(dset), fSel(sel)
{
   fElem  = 0;
   fFile  = 0;
   fDir   = 0;
   fFirst = first;
   fCur   = -1;
   fNum   = num;
   fStop  = kFALSE;
   fEventList = 0;
   fEventListPos = 0;
}


//______________________________________________________________________________
TEventIter::~TEventIter()
{
   delete fFile;
}


//______________________________________________________________________________
void TEventIter::StopProcess(Bool_t /*abort*/)
{
   fStop = kTRUE;
}


//______________________________________________________________________________
TEventIter *TEventIter::Create(TDSet *dset, TSelector *sel, Long64_t first, Long64_t num)
{
   if ( dset->IsTree() ) {
      return new TEventIterTree(dset, sel, first, num);
   } else {
      return new TEventIterObj(dset, sel, first, num);
   }
}


//______________________________________________________________________________
Int_t TEventIter::LoadDir()
{
   Int_t ret = 0;

   // Check Filename
   if ( fFile == 0 || fFilename != fElem->GetFileName() ) {
      fDir = 0;
      delete fFile; fFile = 0;

      fFilename = fElem->GetFileName();

      TDirectory *dirsave = gDirectory;

      Double_t start = 0;
      if (gPerfStats != 0) start = TTimeStamp();

      fFile = TFile::Open(fFilename);

      if (gPerfStats != 0) {
         gPerfStats->FileOpenEvent(fFile, fFilename, double(TTimeStamp())-start);
         fOldBytesRead = 0;
      }

      if (dirsave) dirsave->cd();

      if (!fFile || fFile->IsZombie() ) {
         if (fFile)
            Error("Process","Cannot open file: %s (%s)",
               fFilename.Data(), strerror(fFile->GetErrno()) );
         else
            Error("Process","Cannot open file: %s (errno unavailable)",
               fFilename.Data());
         // cleanup ?
         return -1;
      }
      PDB(kLoop,2) Info("LoadDir","Opening file: %s", fFilename.Data() );
      ret = 1;
   }

   // Check Directory
   if ( fDir == 0 || fPath != fElem->GetDirectory() ) {
      TDirectory *dirsave = gDirectory;

      fPath = fElem->GetDirectory();
      if ( !fFile->cd(fPath) ) {
         Error("Process","Cannot cd to: %s",
            fPath.Data() );
         return -1;
      }
      PDB(kLoop,2) Info("Process","Cd to: %s", fPath.Data() );
      fDir = gDirectory;
      if (dirsave) dirsave->cd();
      ret = 1;
   }

   return ret;
}


//------------------------------------------------------------------------

ClassImp(TEventIterObj)


//______________________________________________________________________________
TEventIterObj::TEventIterObj()
{
   // Default ctor.

   fKeys     = 0;
   fNextKey  = 0;
   fObj      = 0;

}

//______________________________________________________________________________
TEventIterObj::TEventIterObj(TDSet *dset, TSelector *sel, Long64_t first, Long64_t num)
   : TEventIter(dset,sel,first,num)
{
   fClassName = dset->GetType();
   fKeys     = 0;
   fNextKey  = 0;
   fObj      = 0;
}


//______________________________________________________________________________
TEventIterObj::~TEventIterObj()
{
   // delete fKeys ?
   delete fNextKey;
   delete fObj;
}


//______________________________________________________________________________
Long64_t TEventIterObj::GetNextEvent()
{
   if ( fStop || fNum == 0 ) return -1;

   while ( fElem == 0 || fElemNum == 0 || fCur < fFirst-1 ) {

      if (gPerfStats != 0 && fFile != 0) {
         Long64_t bytesRead = (Long64_t) fFile->GetBytesRead();
         gPerfStats->SetBytesRead(bytesRead - fOldBytesRead);
         fOldBytesRead = bytesRead;
      }

      fElem = fDSet->Next();
      if (fElem->GetEventList()) {
         Error("GetNextEvent", "EventLists not implemented");
         return -1;
      }

      if ( fElem == 0 ) {
         fNum = 0;
         return -1;
      }

      Int_t r = LoadDir();

      if ( r == -1 ) {

         // Error has been reported
         fNum = 0;
         return -1;

      } else if ( r == 1 ) {

         // New file and/or directory
         fKeys = fDir->GetListOfKeys();
         fNextKey = new TIter(fKeys);
      }

      // Validate values for this element
      fElemFirst = fElem->GetFirst();
      fElemNum = fElem->GetNum();
      fEventList = fElem->GetEventList();
      fEventListPos = 0;
      if (fEventList) {
         fElemNum = fEventList->GetN();
      }

      Long64_t num = fKeys->GetSize();

      if ( fElemFirst > num ) {
         Error("GetNextEvent","First (%d) higher then number of keys (%d) in %d",
            fElemFirst, num, fElem->GetName() );
         fNum = 0;
         return -1;
      }

      if ( fElemNum == -1 ) {
         fElemNum = num - fElemFirst;
      } else if ( fElemFirst+fElemNum  > num ) {
         Error("GetNextEvent","Num (%d) + First (%d) larger then number of keys (%d) in %s",
            fElemNum, fElemFirst, num, fElem->GetDirectory() );
         fElemNum = num - fElemFirst;
      }

      // Skip this element completely?
      if ( fCur + fElemNum < fFirst ) {
         fCur += fElemNum;
         continue;
      }

      // Position within this element. TODO: more efficient?
      fNextKey->Reset();
      for(fElemCur = -1; fElemCur < fElemFirst-1 ; fElemCur++, fNextKey->Next());
   }

   --fElemNum;
   ++fElemCur;
   --fNum;
   ++fCur;
   TKey *key = (TKey*) fNextKey->Next();
   TDirectory *dirsave = gDirectory;
   fDir->cd();
   fObj = key->ReadObj();
   if (dirsave) dirsave->cd();
   fSel->SetObject( fObj );

   return fElemCur;
}


//------------------------------------------------------------------------

ClassImp(TEventIterTree)


//______________________________________________________________________________
TEventIterTree::TEventIterTree()
{
   // Default ctor.

   fTree = 0;
}

//______________________________________________________________________________
TEventIterTree::TEventIterTree(TDSet *dset, TSelector *sel, Long64_t first, Long64_t num)
   : TEventIter(dset,sel,first,num)
{
   fTreeName = dset->GetObjName();
   fTree = 0;
}


//______________________________________________________________________________
TEventIterTree::~TEventIterTree()
{
   // delete fTree ?
}


//______________________________________________________________________________
Long64_t TEventIterTree::GetNextEvent()
{
   if ( fStop || fNum == 0 ) return -1;

   Bool_t attach = kFALSE;

   while ( fElem == 0 || fElemNum == 0 || fCur < fFirst-1 ) {

      if (gPerfStats != 0 && fFile != 0) {
         Long64_t bytesRead = (Long64_t) fFile->GetBytesRead();
         gPerfStats->SetBytesRead(bytesRead - fOldBytesRead);
         fOldBytesRead = bytesRead;
      }

      fElem = fDSet->Next();

      if ( fElem == 0 ) {
         fNum = 0;
         return -1;
      }

      Int_t r = LoadDir();

      if ( r == -1 ) {

         // Error has been reported
         fNum = 0;
         return -1;

      } else if ( r == 1 || fTreeName != fElem->GetObjName() ) {

         // New file / directory / Tree
         TKey *key = fDir->GetKey(fTreeName);

         if ( key == 0 ) {
            Error("GetNextEvent","Cannot find tree \"%s\" in %s",
                  fTreeName.Data(), fElem->GetFileName() );
            fNum = 0;
            return -1;
         }

         // delete fTree;
         PDB(kLoop,2) Info("GetNextEvent","Reading: %s", fTreeName.Data() );
         TDirectory *dirsave = gDirectory;
         fDir->cd();
         fTree = (TTree *) key->ReadObj();
         if (dirsave) dirsave->cd();

         if ( fTree == 0 ) {
            // Error always reported?
            fNum = 0;
            return -1;
         }
         // TODO: check  type
         attach = kTRUE;
      }

      // Validate values for this element
      fElemFirst = fElem->GetFirst();
      fElemNum = fElem->GetNum();
      fEventList = fElem->GetEventList();
      fEventListPos = 0;
      if (fEventList) {
         fElemNum = fEventList->GetN();
      }

      Long64_t num = (Long64_t) fTree->GetEntries();

      if (!fEventList) {
         if ( fElemFirst > num ) {
            Error("GetNextEvent","First (%d) higher then number of entries (%d) in %s",
               fElemFirst, num, fElem->GetObjName() );
            fNum = 0;
            return -1;
         }
         if ( fElemNum == -1 ) {
            fElemNum = num - fElemFirst;
         } else if ( fElemFirst+fElemNum  > num ) {
            Error("GetNextEvent","Num (%d) + First (%d) larger then number of entries (%d) in %s",
               fElemNum, fElemFirst, num, fElem->GetName() );
            fElemNum = num - fElemFirst;
         }

         // Skip this element completely?
         if ( fCur + fElemNum < fFirst ) {
            fCur += fElemNum;
            continue;
         }
         // Position within this element. TODO: more efficient?
         fElemCur = fElemFirst-1;
      }
   }

   if ( attach ) {
      PDB(kLoop,1) Info("GetNextEvent","Call Init(%p)",fTree);
      fSel->Init( fTree );
      fSel->Notify();
      attach = kFALSE;
   }
   if (!fEventList) {
      --fElemNum;
      ++fElemCur;
      --fNum;
      ++fCur;
      return fElemCur;
   }
   else {
      --fElemNum;
      int rv = fEventList->GetEntry(fEventListPos);
      fEventListPos++;
      return rv;
   }
}
