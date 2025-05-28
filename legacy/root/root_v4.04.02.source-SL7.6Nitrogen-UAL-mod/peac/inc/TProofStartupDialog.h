// @(#)root/peac:$Name:  $:$Id: TProofStartupDialog.h,v 1.1 2005/02/07 18:02:36 rdm Exp $
// Author: Maarten Ballintijn    21/10/2004
// Author: Kris Gulbrandsen      21/10/2004

/*************************************************************************
 * Copyright (C) 1995-2005, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TProofStartupDialog
#define ROOT_TProofStartupDialog

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TProofStartupDialog                                                  //
//                                                                      //
// This class provides a query progress bar for data being staged.      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TTime
#include "TTime.h"
#endif


class TGTransientFrame;
class TGProgressBar;
class TGTextButton;
class TGCheckButton;
class TGLabel;
class TVirtualProof;


class TProofStartupDialog {

private:
   TVirtualProof      *fProof;
   TGTransientFrame   *fDialog;  // transient frame, main dialog window
   TGProgressBar      *fBar;     // progress bar
   TGLabel            *fFilesBytes;
   TGLabel            *fStaged;
   TGLabel            *fTotal;
   TGLabel            *fRate;
   TTime               fStartTime;
   TTime               fEndTime;
   Long64_t            fPrevStaged;
   Int_t               fFiles;
   Long64_t            fTotalBytes;


public:
   TProofStartupDialog(TVirtualProof *proof, const Char_t *dataset,
                       Int_t nfiles, Long64_t totalbytes);
   virtual ~TProofStartupDialog();
   void Progress(Long64_t totalbytes, Long64_t bytesready);

   void CloseWindow();
   void DoClose();

   ClassDef(TProofStartupDialog,0)  // PROOF startup and data staging dialog
};

#endif
