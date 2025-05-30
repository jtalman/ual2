// @(#)root/proof:$Name:  $:$Id: TDrawFeedback.cxx,v 1.8 2005/03/24 16:32:28 rdm Exp $
// Author: Maarten Ballintijn   28/10/2003

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TDrawFeedback                                                        //
//                                                                      //
// Utility class to draw objects in the feedback list during queries.   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TDrawFeedback.h"

#include "THashList.h"
#include "TObjString.h"
#include "TProof.h"
#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TError.h"
#include "TSeqCollection.h"
#include "TVirtualPad.h"
#include "TProofDebug.h"

ClassImp(TDrawFeedback)


//______________________________________________________________________________
TDrawFeedback::TDrawFeedback(TVirtualProof *proof, TSeqCollection *names)
  : fAll(kFALSE)
{
   fNames = new THashList;

   if (proof == 0) proof = gProof;

   TProof *p = dynamic_cast<TProof*>(proof);
   if (p == 0) {
      Error("TDrawFeedback","no valid proof session found");
      return;
   }
   fProof = p;

   Bool_t ok = proof->Connect("Feedback(TList *objs)", "TDrawFeedback",
                  this, "Feedback(TList *objs)");

   if ( !ok ) {
      Error("TDrawFeedback","Connect() failed");
      return;
   }

   if (names != 0) {
      TIter next(names);
      TObjString *name;
      while((name = dynamic_cast<TObjString*>(next())) != 0) {
         fNames->Add(new TNamed(name->GetName(),""));
      }
   } else {
      fAll = kTRUE;
   }
   fOption = 0;
}

//______________________________________________________________________________
TDrawFeedback::~TDrawFeedback()
{
   delete fNames;
   fProof->Disconnect("Feedback(TList*)", this, "Feedback(TList*");
}

//______________________________________________________________________________
void TDrawFeedback::Feedback(TList *objs)
{
   TSeqCollection *canvases = gROOT->GetListOfCanvases();
   TVirtualPad *save = gPad;

   PDB(kFeedback,1) Info("Feedback","%d Objects", objs->GetSize());

   TIter next(objs);
   TObject *o;
   while( (o = next()) )
   {
      TString name = o->GetName();
      if (fAll || fNames->FindObject(name.Data())) {

         name += "_canvas";

         TVirtualPad *p = (TVirtualPad*) canvases->FindObject(name.Data());

         if ( p == 0 ) {
            (gROOT->GetMakeDefCanvas())();
            gPad->SetName(name);
            PDB(kFeedback,2) Info("Feedback","Created canvas %s", name.Data());
         } else {
            p->cd();
            PDB(kFeedback,2) Info("Feedback","Used canvas %s", name.Data());
         }

         if (TH1 *h = dynamic_cast<TH1*>(o)) {
            h->DrawCopy(fOption);
         }

         gPad->Update();
      }
   }

   if (save != 0) {
      save->cd();
   } else {
      gPad = 0;
   }
}
