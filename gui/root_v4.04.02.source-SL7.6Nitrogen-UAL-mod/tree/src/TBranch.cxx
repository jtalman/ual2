// @(#)root/tree:$Name:  $:$Id: TBranch.cxx,v 1.87 2005/04/07 13:28:30 brun Exp $
// Author: Rene Brun   12/01/96

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include <string.h>
#include <stdio.h>

#include "TROOT.h"
#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"
#include "TBasket.h"
#include "TBrowser.h"
#include "TLeaf.h"
#include "TLeafObject.h"
#include "TLeafB.h"
#include "TLeafC.h"
#include "TLeafI.h"
#include "TLeafF.h"
#include "TLeafS.h"
#include "TLeafD.h"
#include "TLeafL.h"
#include "TLeafO.h"
#include "TMessage.h"
#include "TClonesArray.h"
#include "TVirtualPad.h"
#include "TSystem.h"
#include "TStreamerInfo.h"
#include "TBranchBrowsable.h"

TBranch *gBranch;

R__EXTERN TTree *gTree;

Int_t TBranch::fgCount = 0;

const Int_t kMaxRAM = 10;
const Int_t kMaxLen = 512;

ClassImp(TBranch)

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// A TTree is a list of TBranches                                       //                                                                      //
//                                                                      //
// A TBranch supports:                                                  //
//   - The list of TLeaf describing this branch.                        //
//   - The list of TBasket (branch buffers).                            //
//                                                                      //
//       See TBranch structure in TTree.                                //
//                                                                      //
// See also specialized branches:                                       //
//     TBranchObject in case the branch is one object                   //
//     TBranchClones in case the branch is an array of clone objects    //
//////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
TBranch::TBranch(): TNamed(), TAttFill(0,1001)
{
//*-*-*-*-*-*Default constructor for Branch*-*-*-*-*-*-*-*-*-*
//*-*        ===================================
   fCompress       = 0;
   fBasketSize     = 32000;
   fEntryOffsetLen = 1000;
   fMaxBaskets     = 10;
   fReadBasket     = 0;
   fReadEntry      = -1;
   fWriteBasket    = 0;
   fEntries        = 0;
   fTotBytes       = 0;
   fZipBytes       = 0;
   fSplitLevel     = 0;
   fNBasketRAM     = kMaxRAM+1;
   fBasketRAM      = new Int_t[kMaxRAM]; for (Int_t i=0;i<kMaxRAM;i++) fBasketRAM[i] = -1;
   fBasketEntry    = 0;
   fBasketBytes    = 0;
   fBasketSeek     = 0;
   fEntryNumber    = 0;
   fEntryBuffer    = 0;
   fNleaves        = 0;
   fTree           = 0;
   fAddress        = 0;
   fOffset         = 0;
   fDirectory      = 0;
   fSkipZip        = kFALSE;
   fFileName       = "";
   fBrowsables     = 0;
   gBranch = this;
}

//______________________________________________________________________________
TBranch::TBranch(const char *name, void *address, const char *leaflist, Int_t basketsize, Int_t compress)
    :TNamed(name,leaflist), TAttFill(0,1001)
{
//*-*-*-*-*-*-*-*-*-*-*-*-*Create a Branch*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                =====================
//
//       * address is the address of the first item of a structure
//         or the address of a pointer to an object (see example).
//       * leaflist is the concatenation of all the variable names and types
//         separated by a colon character :
//         The variable name and the variable type are separated by a slash (/).
//         The variable type may be 0,1 or 2 characters. If no type is given,
//         the type of the variable is assumed to be the same as the previous
//         variable. If the first variable does not have a type, it is assumed
//         of type F by default. The list of currently supported types is given below:
//            - C : a character string terminated by the 0 character
//            - B : an 8 bit signed integer (Char_t)
//            - b : an 8 bit unsigned integer (UChar_t)
//            - S : a 16 bit signed integer (Short_t)
//            - s : a 16 bit unsigned integer (UShort_t)
//            - I : a 32 bit signed integer (Int_t)
//            - i : a 32 bit unsigned integer (UInt_t)
//            - F : a 32 bit floating point (Float_t)
//            - D : a 64 bit floating point (Double_t)
//            - L : a 64 bit signed integer (Long64_t)
//            - l : a 64 bit unsigned integer (ULong64_t)
//
//         By default, a variable will be copied to the buffer with the number of
//         bytes specified in the type descriptor character. However, if the type
//         consists of 2 characters, the second character is an integer that
//         specifies the number of bytes to be used when copying the variable
//         to the output buffer. Example:
//             X         ; variable X, type Float_t
//             Y/I       : variable Y, type Int_t
//             Y/I2      ; variable Y, type Int_t converted to a 16 bits integer
//
//   See an example of a Branch definition in the TTree constructor.
//
//   Note that in case the data type is an object, this branch can contain
//   only this object.
//
//    Note that this function is invoked by TTree::Branch
//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

   gBranch = this;
   Int_t i;
   fCompress = compress;
   if (compress == -1 && gTree->GetDirectory()) {
      TFile *bfile = gTree->GetDirectory()->GetFile();
      if (bfile) fCompress = bfile->GetCompressionLevel();
   }
   if (basketsize < 100) basketsize = 100;
   fBasketSize     = basketsize;
   fEntryOffsetLen = 0;
   fMaxBaskets     = 10;
   fReadBasket     = 0;
   fReadEntry      = -1;
   fWriteBasket    = 0;
   fEntryNumber    = 0;
   fEntryBuffer    = 0;
   fEntries        = 0;
   fTotBytes       = 0;
   fZipBytes       = 0;
   fSplitLevel     = 0;
   fOffset         = 0;
   fNleaves        = 0;
   fBrowsables     = 0;
   fSkipZip        = kFALSE;
   fAddress        = (char*)address;
   fNBasketRAM     = kMaxRAM+1;
   fBasketRAM      = new Int_t[kMaxRAM]; for (i=0;i<kMaxRAM;i++) fBasketRAM[i] = -1;
   fBasketBytes    = new Int_t[fMaxBaskets];
   fBasketEntry    = new Long64_t[fMaxBaskets];
   fBasketSeek     = new Long64_t[fMaxBaskets];

   for (i=0;i<fMaxBaskets;i++) {
      fBasketBytes[i] = 0;
      fBasketEntry[i] = 0;
      fBasketSeek[i]  = 0;
   }
//*-*- Decode the leaflist (search for : as separator)
   char * varcur  = (char*)leaflist;
   char * var     = varcur;
   Int_t lenvar   = 0;
   Int_t offset   = 0;
   char *leafname = new char[64];
   char *leaftype = new char[32];
   strcpy(leaftype,"F");
   while (1) {
      lenvar++;
      if (*var == ':' || *var == 0) {
         strncpy(leafname,varcur,lenvar-1);
         leafname[lenvar-1] = 0;
         char *ctype = strstr(leafname,"/");
         if (ctype)  { *ctype=0; strcpy(leaftype,ctype+1);}
         TLeaf *leaf = 0;
         if (*leaftype == 'C') {
            leaf = new TLeafC(leafname,leaftype);
         } else if (*leaftype == 'O') {
            leaf = new TLeafO(leafname,leaftype);
         } else if (*leaftype == 'B') {
            leaf = new TLeafB(leafname,leaftype);
         } else if (*leaftype == 'b') {
            leaf = new TLeafB(leafname,leaftype);
            leaf->SetUnsigned();
         } else if (*leaftype == 'S') {
            leaf = new TLeafS(leafname,leaftype);
         } else if (*leaftype == 's') {
            leaf = new TLeafS(leafname,leaftype);
            leaf->SetUnsigned();
         } else if (*leaftype == 'I') {
            leaf = new TLeafI(leafname,leaftype);
         } else if (*leaftype == 'i') {
            leaf = new TLeafI(leafname,leaftype);
            leaf->SetUnsigned();
         } else if (*leaftype == 'F') {
            leaf = new TLeafF(leafname,leaftype);
         } else if (*leaftype == 'f') {
            leaf = new TLeafF(leafname,leaftype);
         } else if (*leaftype == 'L') {
            leaf = new TLeafL(leafname,leaftype);
         } else if (*leaftype == 'l') {
            leaf = new TLeafL(leafname,leaftype);
            leaf->SetUnsigned();
         } else if (*leaftype == 'D') {
            leaf = new TLeafD(leafname,leaftype);
         } else if (*leaftype == 'd') {
            leaf = new TLeafD(leafname,leaftype);
         }
         if (!leaf) {
            Error("TLeaf","Illegal data type");
            MakeZombie();
            return;
         }
         if (leaf->IsZombie()) {
            delete leaf;
            Error("TBranch","Illegal leaf:%s/%s",name,leaflist);
            MakeZombie();
            return;
         }
         leaf->SetBranch(this);
         leaf->SetAddress((char*)(fAddress+offset));
         leaf->SetOffset(offset);
         if (leaf->GetLeafCount()) fEntryOffsetLen = 1000;
         if (leaf->InheritsFrom("TLeafC")) fEntryOffsetLen = 1000;
         fNleaves++;
         fLeaves.Add(leaf);
         gTree->GetListOfLeaves()->Add(leaf);
         if (*var == 0) break;
         varcur  = var+1;
         offset += leaf->GetLenType()*leaf->GetLen();
         lenvar  = 0;
      }
      var++;
   }
   delete [] leafname;
   delete [] leaftype;

//*-*-  Create the first basket
   fTree       = gTree;
   fDirectory  = fTree->GetDirectory();
   fFileName   = "";

   TBasket *basket = new TBasket(name,fTree->GetName(),this);
   fBaskets.AddAt(basket,0);
}

//______________________________________________________________________________
TBranch::~TBranch()
{
//*-*-*-*-*-*Default destructor for a Branch*-*-*-*-*-*-*-*-*-*-*-*
//*-*        ===============================

   if (fBasketRAM)   delete [] fBasketRAM;
   if (fBasketEntry) delete [] fBasketEntry;
   if (fBasketBytes) delete [] fBasketBytes;
   if (fBasketSeek)  delete [] fBasketSeek;
   fBasketRAM   = 0;
   fBasketEntry = 0;
   fBasketBytes = 0;
   fBasketSeek  = 0;

   fLeaves.Delete();
   fBaskets.Delete();
   delete fBrowsables;
   // Warning. Must use FindObject by name instead of fDirectory->GetFile()
   // because two branches<may point to the same file and the file
   // already deleted in the previous branch
   if (fDirectory && fDirectory != fTree->GetDirectory()) {
      TFile *file = (TFile*)gROOT->GetListOfFiles()->FindObject(GetFileName());
      if (file ) delete file;
   }
   fTree        = 0;
   fDirectory   = 0;
   gBranch      = 0;
}


//______________________________________________________________________________
void TBranch::Browse(TBrowser *b)
{
   // Browser interface.

   if (fNleaves > 1) {
      fLeaves.Browse(b);
   } else {
      // Get the name and strip any extra brackets
      // in order to get the full arrays.
      TString name = GetName();
      Int_t pos = name.First('[');
      if (pos!=kNPOS) name.Remove(pos);

      GetTree()->Draw(name, "", b ? b->GetDrawOption() : "");
      if (gPad) gPad->Update();
   }
}

//______________________________________________________________________________
void TBranch::DropBaskets(Option_t* option)
{
   // Loop on all branch baskets. Drop all except readbasket.
   // If the option contains "all", drop all baskets including
   // read- and write-baskets.

   Bool_t all = kFALSE;
   TString opt = option;
   opt.ToLower();
   if (opt.Contains("all"))
      all = kTRUE;

   Int_t i,j;
   TBasket *basket;
   // fast algorithm in case of only a few baskets in memory
   if (fNBasketRAM < kMaxRAM) {
      for (i=0;i<kMaxRAM;i++) {
         j = fBasketRAM[i];
         if (j < 0) continue;
         if ((j == fReadBasket || j == fWriteBasket) && !all) continue;
         basket = (TBasket*)fBaskets.UncheckedAt(j);
         if (!basket) continue;
         basket->DropBuffers();
         GetListOfBaskets()->RemoveAt(j);
         delete basket;
         fBasketRAM[i] = -1;
         fNBasketRAM--;
      }
      if (fNBasketRAM < 0) {
         Error("DropBaskets", "fNBasketRAM =%d",fNBasketRAM);
         fNBasketRAM = 0;
      }
      i = 0;
      for (j=0;j<kMaxRAM;j++) {
         if (fBasketRAM[j] < 0) continue;
         fBasketRAM[i] = fBasketRAM[j];
         i++;
      }
      return;
   }

   //general algorithm looping on the full baskets table.
   Int_t nbaskets = GetListOfBaskets()->GetEntriesFast();
   fNBasketRAM = 0;
   for (j=0;j<nbaskets-1;j++)  {
      basket = (TBasket*)fBaskets.UncheckedAt(j);
      if (!basket) continue;
      if (fNBasketRAM < kMaxRAM) fBasketRAM[fNBasketRAM] = j;
      fNBasketRAM++;
      if ((j == fReadBasket || j == fWriteBasket) && !all) continue;
      basket->DropBuffers();
      GetListOfBaskets()->RemoveAt(j);
      delete basket;
      fNBasketRAM--;
      fBasketRAM[fNBasketRAM] = -1;
      if (!fTree->MemoryFull(0)) break;
   }

   // process subbranches
   if (all) {
      TObjArray *lb = GetListOfBranches();
      Int_t nb = lb->GetEntriesFast();
      for (Int_t j = 0; j < nb; j++) {
         TBranch* branch = (TBranch*) lb->UncheckedAt(j);
         if (!branch) continue;
         branch->DropBaskets("all");
      }
   }
}

//______________________________________________________________________________
Int_t TBranch::Fill()
{
//*-*-*-*-*-*-*-*Loop on all leaves of this branch to fill Basket buffer*-*-*
//*-*            =======================================================
//
// The function returns the number of bytes committed to the memory basket.
// If a write error occurs, the number of bytes returned is -1.
// If no data are written, because e.g. the branch is disabled,
// the number of bytes returned is 0.
//

   if (TestBit(kDoNotProcess)) return 0;

   TBasket *basket = GetBasket(fWriteBasket);
   if (!basket) return 0;
   TBuffer *buf    = basket->GetBufferRef();

//*-*- Fill basket buffer
   Int_t nsize  = 0;
   if (buf->IsReading()) {
      basket->SetWriteMode();
   }
   buf->ResetMap();
   Int_t lold = buf->Length();
   Int_t objectStart = 0;
   Int_t last = 0;
   Int_t lnew = 0;
   Int_t nbytes = 0;
   if ( fEntryBuffer!=0 ) {
      if ( fEntryBuffer->IsA() == TMessage::Class() ) {
         objectStart = 8;
      }
      if ( fEntryBuffer->TestBit(TBuffer::kNotDecompressed) ) {
         // The buffer given as input as not been decompressed.

         if (basket->GetNevBuf()) {
             // If the basket already contains entry we need to close it
             // out. (This is because we can only transfer full compressed
             // buffer)
             WriteBasket(basket);

             // And restart from scratch
             return Fill();
         }

         Int_t startpos = fEntryBuffer->Length();
         fEntryBuffer->SetBufferOffset(0);
         static TBasket toread_fLast;
         fEntryBuffer->SetReadMode();
         toread_fLast.Streamer(*fEntryBuffer);
         fEntryBuffer->SetWriteMode();
         last = toread_fLast.GetLast(); // last now contains the decompressed number of bytes.
         fEntryBuffer->SetBufferOffset(startpos);

         buf->SetBufferOffset(0);
         buf->SetBit(TBuffer::kNotDecompressed);

         basket->Update(lold);

      } else {
         // We are required to copy starting at the version number (so not
         // including the class name.
         // See if byte count is here, if not it class still be a newClass
         const UInt_t kNewClassTag       = 0xFFFFFFFF;
         const UInt_t kByteCountMask     = 0x40000000;  // OR the byte count with this
         UInt_t tag, startpos = fEntryBuffer->Length();
         fEntryBuffer->SetBufferOffset(objectStart);
         *fEntryBuffer >> tag;
         if ( tag & kByteCountMask ) {
            *fEntryBuffer >> tag;
         }
         if ( tag == kNewClassTag ) {
            char s[80];

            fEntryBuffer->ReadString(s, 80);
         } else {
            fEntryBuffer->SetBufferOffset(objectStart);
         }
         objectStart = fEntryBuffer->Length();
         fEntryBuffer->SetBufferOffset(startpos);

         basket->Update(lold, objectStart - fEntryBuffer->GetBufferDisplacement());
      }
      fEntries++;
      fEntryNumber++;

      UInt_t len, startpos = fEntryBuffer->Length();
      if (startpos > UInt_t(objectStart)) {
         // We assume this buffer have just been directly filled
         // the current position in the buffer indicates the end of the object!
         len = fEntryBuffer->Length() - objectStart;
      } else {
         // The buffer have been acquired either via TSocket or via
         // TBuffer::SetBuffer(newloc,newsize)
         // Only the actual size of the memory buffer gives us an hint about where
         // the object ends.
         len = fEntryBuffer->BufferSize() - objectStart;
      }
      buf->WriteBuf( fEntryBuffer->Buffer() + objectStart , len );

      if ( fEntryBuffer->TestBit(TBuffer::kNotDecompressed) ) {
         // The original buffer came pre-compressed and thus the buffer Length
         // does not really show the really object size
         // lnew = nbytes = basket->GetLast();
         lnew = nbytes = last;

      } else {
         lnew   = buf->Length();
         nbytes = lnew - lold;
      }

   } else {

      basket->Update(lold);
      fEntries++;
      fEntryNumber++;

      FillLeaves(*buf);

      lnew   = buf->Length();
      nbytes = lnew - lold;

   }

   if (fEntryOffsetLen) {
      Int_t nevbuf = basket->GetNevBuf();
      nsize = nevbuf*sizeof(Int_t); //total size in bytes of EntryOffset table
   } else {
      if (!basket->GetNevBufSize()) basket->SetNevBufSize(nbytes);
   }

//*-*- Should we create a new basket?
   if ( (fSkipZip && (lnew>=TBuffer::kMinimalSize))  // fSkipZip force one entry per buffer
        || (buf->TestBit(TBuffer::kNotDecompressed)) // Transfer full compressed buffer only
        || (lnew +2*nsize +nbytes >= fBasketSize) ) {
      Int_t nout  = basket->WriteBuffer();    //  Write buffer
      fBasketBytes[fWriteBasket]  = basket->GetNbytes();
      fBasketSeek[fWriteBasket]   = basket->GetSeekKey();
      Int_t addbytes = basket->GetObjlen() + basket->GetKeylen() ;
      if (fDirectory != 0 && fDirectory != gROOT && fDirectory->IsWritable()) {
         delete basket;
         fBaskets[fWriteBasket] = 0;
      }
      fZipBytes += nout;
      fTotBytes += addbytes;
      fTree->AddTotBytes(addbytes);
      fTree->AddZipBytes(nout);
      basket = new TBasket(GetName(),fTree->GetName(),this);   //  create a new basket
      fWriteBasket++;
      fBaskets.AddAtAndExpand(basket,fWriteBasket);
      if (fWriteBasket >= fMaxBaskets) {
           //Increase BasketEntry buffer of a minimum of 10 locations
           // and a maximum of 50 per cent of current size
         Int_t newsize = TMath::Max(10,Int_t(1.5*fMaxBaskets));
         fBasketBytes  = TStorage::ReAllocInt(fBasketBytes, newsize, fMaxBaskets);
         fBasketEntry  = (Long64_t*)TStorage::ReAlloc(fBasketEntry,
                             newsize*sizeof(Long64_t),fMaxBaskets*sizeof(Long64_t));
         fBasketSeek   = (Long64_t*)TStorage::ReAlloc(fBasketSeek,
                             newsize*sizeof(Long64_t),fMaxBaskets*sizeof(Long64_t));
         fMaxBaskets   = newsize;
      }

      for (Int_t i=fWriteBasket;i<fMaxBaskets;i++) {
         fBasketBytes[i] = 0;
         fBasketEntry[i] = 0;
         fBasketSeek[i]  = 0;
      }
      fBasketEntry[fWriteBasket] = fEntryNumber;
      return (nout >= 0) ? nbytes : -1;
   }
   return nbytes;
}

//______________________________________________________________________________
void TBranch::FillLeaves(TBuffer &b)
{
  for (Int_t i=0;i<fNleaves;i++) {
    TLeaf *leaf = (TLeaf*)fLeaves.UncheckedAt(i);
    leaf->FillBasket(b);
  }
}

//______________________________________________________________________________
TBranch *TBranch::FindBranch(const char* searchname)
{
   char brname[kMaxLen];
   char longsearchname[kMaxLen];
   TIter next(GetListOfBranches());

   // For branches we allow for one level up to be prefixed to the
   // name

   strcpy(longsearchname,GetName());
   char *dim = (char*)strstr(longsearchname,"[");
   if (dim) dim[0]='\0';
   strcat(longsearchname,".");
   strcat(longsearchname,searchname);

   TBranch *branch;
   while ((branch = (TBranch*)next())) {
      strcpy(brname,branch->GetName());
      dim = (char*)strstr(brname,"[");
      if (dim) dim[0]='\0';
      if (!strcmp(searchname,brname)) return branch;

      if (!strcmp(longsearchname,brname)) return branch;
   }

   //search in list of friends
   return 0;
}

//______________________________________________________________________________
TLeaf *TBranch::FindLeaf(const char* searchname)
{
   char leafname[kMaxLen];
   char leaftitle[kMaxLen];
   char longname[kMaxLen];
   char longtitle[kMaxLen];

   // For leaves we allow for one level up to be prefixed to the
   // name

   TIter next (GetListOfLeaves());
   TLeaf *leaf;
   while ((leaf = (TLeaf*)next())) {
      strcpy(leafname,leaf->GetName());
      char *dim = (char*)strstr(leafname,"[");
      if (dim) dim[0]='\0';

      if (!strcmp(searchname,leafname)) return leaf;

      // The TLeafElement contains the branch name in its name,
      // let's use the title....
      strcpy(leaftitle,leaf->GetTitle());
      dim = (char*)strstr(leaftitle,"[");
      if (dim) dim[0]='\0';

      if (!strcmp(searchname,leaftitle)) return leaf;

      TBranch * branch = leaf->GetBranch();
      if (branch) {
         sprintf(longname,"%s.%s",branch->GetName(),leafname);
         char *dim = (char*)strstr(longname,"[");
         if (dim) dim[0]='\0';
         if (!strcmp(searchname,longname)) return leaf;

         // The TLeafElement contains the branch name in its name
         sprintf(longname,"%s.%s",branch->GetName(),searchname);
         if (!strcmp(longname,leafname)) return leaf;

         sprintf(longtitle,"%s.%s",branch->GetName(),leaftitle);
         dim = (char*)strstr(longtitle,"[");
         if (dim) dim[0]='\0';
         if (!strcmp(searchname,longtitle)) return leaf;

         // The following is for the case where the branch is only
         // a sub-branch.  Since we do not see it through
         // TTree::GetListOfBranches, we need to see it indirectly.
         // This is the less sturdy part of this search ... it may
         // need refining ...
         if (strstr(searchname,".")
             && !strcmp(searchname,branch->GetName())) return leaf;
         //printf("found leaf3=%s/%s, branch=%s, i=%d\n",leaf->GetName(),leaf->GetTitle(),branch->GetName(),i);
      }
   }

   //search in list of friends
   return 0;
}


//______________________________________________________________________________
TBasket *TBranch::GetBasket(Int_t basketnumber)
{
//*-*-*-*-*Return pointer to basket basketnumber in this Branch*-*-*-*-*-*
//*-*      ====================================================

   static Int_t nerrors = 0;

      // reference to an existing basket in memory ?
   if (basketnumber <0 || basketnumber > fWriteBasket) return 0;
   TBasket *basket = (TBasket*)fBaskets.UncheckedAt(basketnumber);
   if (basket) return basket;

      // must create a new basket
   gBranch = this;

     // create/decode basket parameters from buffer
   TDirectory *cursav = gDirectory;
   TFile *file = GetFile(0);
   basket = new TBasket();
   if (fSkipZip) basket->SetBit(TBuffer::kNotDecompressed);
   basket->SetBranch(this);
   if (fBasketBytes[basketnumber] == 0) {
      fBasketBytes[basketnumber] = basket->ReadBasketBytes(fBasketSeek[basketnumber],file);
   }
   Int_t badread = basket->ReadBasketBuffers(fBasketSeek[basketnumber],fBasketBytes[basketnumber],file);
   if (badread || basket->GetSeekKey() != fBasketSeek[basketnumber]) {
      cursav->cd();
      nerrors++;
      if (nerrors > 10) return 0;
      if (nerrors == 10) {
         printf(" file probably overwritten: stopping reporting error messages\n");
         if (fBasketSeek[basketnumber] > 2000000000) {
            printf("===>File is more than 2 Gigabytes\n");
            return 0;
         }
         if (fBasketSeek[basketnumber] > 1000000000) {
            printf("===>Your file is may be bigger than the maximum file size allowed on your system\n");
            printf("    Check your AFS maximum file size limit for example\n");
            return 0;
         }
      }
      Error("GetBasket","File: %s at byte:%d, branch:%s, entry:%d",file->GetName(),basket->GetSeekKey(),GetName(),fReadEntry);
      return 0;
   }

   cursav->cd();
   fBaskets.AddAt(basket,basketnumber);
   if (fNBasketRAM < kMaxRAM) fBasketRAM[fNBasketRAM] = basketnumber;
   fNBasketRAM++;
   return basket;
}

//______________________________________________________________________________
Long64_t TBranch::GetBasketSeek(Int_t basketnumber) const
{
//*-*-*-*-*Return address of basket in the file*-*-*-*-*-*
//*-*      ====================================

   if (basketnumber <0 || basketnumber > fWriteBasket) return 0;
   return fBasketSeek[basketnumber];
}

//______________________________________________________________________________
TList *TBranch::GetBrowsables() {
// Returns (and, if 0, creates) browsable objects for this branch
// See TVirtualBranchBrowsable::FillListOfBrowsables.
   if (fBrowsables) return fBrowsables;
   fBrowsables=new TList();
   TVirtualBranchBrowsable::FillListOfBrowsables(*fBrowsables, this);
   return fBrowsables;
}

//______________________________________________________________________________
const char *TBranch::GetIconName() const
{
   // Return icon name depending on type of branch.

   if (IsFolder())
      return "TBranchElement-folder";
   else
      return "TBranchElement-leaf";
}

//______________________________________________________________________________
Int_t TBranch::GetEntry(Long64_t entry, Int_t getall)
{
//*-*-*-*-*-*Read all leaves of entry and return total number of bytes*-*-*
//*-*        =========================================================
// The input argument entry is the entry serial number in the current tree.
// In case of a TChain, the entry number in the current Tree must be found
//  before calling this function. example with TChain *chain;
//  Long64_t localEntry = chain->LoadTree(entry);
//  branch->GetEntry(localEntry);
//
//  The function returns the number of bytes read from the input buffer.
//  If entry does not exist  the function returns 0.
//  If an I/O error occurs,  the function returns -1.
//
//  See IMPORTANT REMARKS in TTree::GetEntry

   if (TestBit(kDoNotProcess) && !getall) return 0;
   //if (fReadEntry == entry) return 1;  //side effects in case user Clear his structures
   if (entry < 0 || entry >= fEntryNumber) return 0;
   Int_t nbytes;
   Long64_t first  = fBasketEntry[fReadBasket];
   Long64_t last;
   if (fReadBasket == fWriteBasket) last = fEntryNumber - 1;
   else                             last = fBasketEntry[fReadBasket+1] - 1;
//
//      Are we still in the same ReadBasket?
   if (entry < first || entry > last) {
      fReadBasket = TMath::BinarySearch(fWriteBasket+1, fBasketEntry, entry);
      first       = fBasketEntry[fReadBasket];
   }

//     We have found the basket containing this entry.
//     make sure basket buffers are in memory.
   TBasket *basket = (TBasket*)fBaskets.UncheckedAt(fReadBasket);
   if (!basket) {
      basket = GetBasket(fReadBasket);
      if (!basket) return -1;
   }
   TBuffer *buf    = basket->GetBufferRef();
//     This test necessary to read very old Root files (NvE)
   if (!buf) {
      TFile *file = GetFile(0);
      basket->ReadBasketBuffers(fBasketSeek[fReadBasket],fBasketBytes[fReadBasket],file);
      buf    = basket->GetBufferRef();
   }
//     Set entry offset in buffer and read data from all leaves
   buf->ResetMap();
   if (!buf->IsReading()) {
      basket->SetReadMode();
   }

   Int_t bufbegin;
   Int_t *entryOffset = basket->GetEntryOffset();
   if (entryOffset) bufbegin = entryOffset[entry-first];
   else             bufbegin = basket->GetKeylen() + (entry-first)*basket->GetNevBufSize();
   buf->SetBufferOffset(bufbegin);
   Int_t *displacement = basket->GetDisplacement();
   if (displacement) buf->SetBufferDisplacement(displacement[entry-first]);
   else buf->SetBufferDisplacement();

   ReadLeaves(*buf);

   nbytes = buf->Length() - bufbegin;
   fReadEntry = entry;
   return nbytes;
}


//______________________________________________________________________________
Int_t TBranch::GetEntryExport(Long64_t entry, Int_t /*getall*/,
                              TClonesArray *list, Int_t nentries)
{
//*-*-*-*-*-*Read all leaves of entry and return total number of bytes*-*-*
//*-* export buffers to real objects in the TClonesArray list.
//*-*

   if (TestBit(kDoNotProcess)) return 0;
   //if (fReadEntry == entry) return 1;  //side effects in case user Clear his structures
   if (entry < 0 || entry >= fEntryNumber) return 0;
   Int_t nbytes;
   Long64_t first  = fBasketEntry[fReadBasket];
   Long64_t last;
   if (fReadBasket == fWriteBasket) last = fEntryNumber - 1;
   else                             last = fBasketEntry[fReadBasket+1] - 1;
//
//      Are we still in the same ReadBasket?
   if (entry < first || entry > last) {
      fReadBasket = TMath::BinarySearch(fWriteBasket+1, fBasketEntry, entry);
      first       = fBasketEntry[fReadBasket];
   }

//     We have found the basket containing this entry.
//     make sure basket buffers are in memory.
   TBasket *basket = GetBasket(fReadBasket);
   if (!basket) return 0;
   TBuffer *buf    = basket->GetBufferRef();
//     Set entry offset in buffer and read data from all leaves
   if (!buf->IsReading()) {
      basket->SetReadMode();
   }
   Int_t bufbegin;
   Int_t *entryOffset = basket->GetEntryOffset();
   if (entryOffset) bufbegin = entryOffset[entry-first];
   else             bufbegin = basket->GetKeylen() + (entry-first)*basket->GetNevBufSize();
   buf->SetBufferOffset(bufbegin);
   Int_t *displacement = basket->GetDisplacement();
   if (displacement) buf->SetBufferDisplacement(displacement[entry-first]);
   else buf->SetBufferDisplacement();

   TLeaf *leaf = (TLeaf*)fLeaves.UncheckedAt(0);
   leaf->ReadBasketExport(*buf,list,nentries);

   nbytes = buf->Length() - bufbegin;
   fReadEntry = entry;

   return nbytes;
}

//______________________________________________________________________________
TFile *TBranch::GetFile(Int_t mode)
{
   // Return pointer to the file where branch buffers reside, returns 0
   // in case branch buffers reside in the same file as tree header.
   // If mode is 1 the branch buffer file is recreated.

   if (fDirectory) return fDirectory->GetFile();

   // check if a file with this name is in the list of Root files
   TFile *file = (TFile*)gROOT->GetListOfFiles()->FindObject(fFileName.Data());
   if (file) {
      fDirectory = (TDirectory*)file;
      return file;
   }

   if (fFileName.Length() == 0) return 0;

   TString bFileName = fFileName;

   // check if branch file name is absolute or a URL (e.g. /castor/...,
   // root://host/..., rfio:/path/...)
   char *bname = gSystem->ExpandPathName(fFileName.Data());
   if (!gSystem->IsAbsoluteFileName(bname) && !strstr(bname, ":/")) {

      // if not, get filename where tree header is stored
      const char *tfn = fTree->GetCurrentFile()->GetName();

      // if this is an absolute path or a URL then prepend this path
      // to the branch file name
      char *tname = gSystem->ExpandPathName(tfn);
      if (gSystem->IsAbsoluteFileName(tname) || strstr(tname, ":/")) {
         bFileName = gSystem->DirName(tname);
         bFileName += "/";
         bFileName += fFileName;
      }
      delete [] tname;
   }
   delete [] bname;

   // Open file (new file if mode = 1)
   if (mode) file = TFile::Open(bFileName, "recreate");
   else      file = TFile::Open(bFileName);
   if (file->IsZombie()) {delete file; return 0;}
   fDirectory = (TDirectory*)file;
   return file;
}

//______________________________________________________________________________
TLeaf *TBranch::GetLeaf(const char *name) const
{
//*-*-*-*-*-*Return pointer to the 1st Leaf named name in thisBranch-*-*-*-*-*
//*-*        =======================================================

   Int_t i;
   for (i=0;i<fNleaves;i++) {
      TLeaf *leaf = (TLeaf*)fLeaves.UncheckedAt(i);
      if (!strcmp(leaf->GetName(),name)) return leaf;
   }
   return 0;
}


//______________________________________________________________________________
Int_t TBranch::GetRow(Int_t)
{
//*-*-*-*-*Return all elements of one row unpacked in internal array fValues*-*
//*-*      =================================================================

   return 1;
}


//______________________________________________________________________________
TBranch *TBranch::GetMother() const
{
// Get top level branch parent of this branch
// A top level branch has its fID negative.

   TIter next(fTree->GetListOfBranches());
   TBranch *branch;
   while ((branch=(TBranch*)next())) {
      TBranch *br = branch->GetSubBranch(this);
      if (br) {
         //if (br == this) return 0;
         return branch;
      }
   }
   return 0;
}

//______________________________________________________________________________
TBranch *TBranch::GetSubBranch(const TBranch *br) const
{
// recursively find branch br in the list of branches of this branch.
// return null if br is not in this branch hierarchy.

   if (br == this) return (TBranch*)this;

   Int_t len = fBranches.GetLast();
   for(Int_t i = 0; i <= len; ++i) {
      TBranch *branch = dynamic_cast<TBranch*>(fBranches.UncheckedAt( i ));
      if (branch == 0) continue;
      if (branch == br) return (TBranch*)this;
      TBranch *br2 = branch->GetSubBranch(br);
      if (br2) return br2;
   }
   return 0;
}

//______________________________________________________________________________
Long64_t TBranch::GetTotalSize() const
{
// Return total number of bytes in the branch (including current buffer)
// =====================================================================

   TBuffer b(TBuffer::kWrite,10000);
   TBranch::Class()->WriteBuffer(b,(TBranch*)this);
   Long64_t totbytes = 0;
   if (fZipBytes > 0) totbytes = fTotBytes;
   return totbytes + b.Length();
}


//______________________________________________________________________________
Bool_t TBranch::IsAutoDelete() const
{
//*-*-*-*-*Return TRUE if an existing object in a TBranchObject must be deleted
//*-*      ==================================================

   return TestBit(kAutoDelete);
}


//______________________________________________________________________________
Bool_t TBranch::IsFolder() const
{
//*-*-*-*-*Return TRUE if more than one leaf or browsables, FALSE otherwise*-*
//*-*      ================================================================
   if (fNleaves > 1) return kTRUE;
   TList* browsables=const_cast<TBranch*>(this)->GetBrowsables();
   return (browsables && browsables->GetSize());
}

//______________________________________________________________________________
void TBranch::KeepCircular(Long64_t maxEntries)
{
   // keep a maximum of fMaxEntries in memory

   Int_t dentries = (Int_t)(fEntries - maxEntries);
   Int_t nbaskets = fBaskets.GetEntriesFast();
   Int_t i, ndrop = -1;
   TBasket *basket;
   for (i=0;i<nbaskets;i++) {
      fBasketEntry[i] -= dentries;
      if (fBasketEntry[i] < 0) {
         ndrop++;
         basket = (TBasket*)fBaskets.UncheckedAt(i);
         basket->MoveEntries(-fBasketEntry[i]);
         if (!i) fBasketEntry[i] = 0;
      }
   }
   if (ndrop > 0) {
      for (i=0;i<ndrop;i++) {
         fBaskets.RemoveAt(i);
      }
      fBaskets.Compress();
      nbaskets -= ndrop;
      fBasketEntry[0] = 0;
      for (i=1;i<nbaskets;i++) {
         basket = (TBasket*)fBaskets.UncheckedAt(i-1);
         fBasketEntry[i] = fBasketEntry[i-1] + basket->GetNevBuf();
      }
      fMaxBaskets = fBaskets.GetEntriesFast();
      fWriteBasket -= ndrop;
      fReadBasket = 0;
      fReadEntry = -1;
   }
   fEntries     = maxEntries;
   fEntryNumber = maxEntries;
}

//______________________________________________________________________________
Int_t TBranch::LoadBaskets()
{
//  Baskets associated to this branch are forced to be in memory.
//  You can call TTree::SetMaxVirtualSize(maxmemory) to instruct
//  the system that the total size of the imported baskets does not
//  exceed maxmemory bytes.
//  The function returns the number of baskets that have been put in memory.
//  This method may be called to force all baskets of one or more branches
//  in memory when random access to entries in this branch is required.
//  See also TTree::LoadBaskets to load all baskets of all branches in memory.

   Int_t nimported = 0;
   Int_t nbaskets = fBaskets.GetEntriesFast();
   TFile *file = GetFile(0);
   TBasket *basket;
   gBranch = this;
   for (Int_t i=0;i<nbaskets;i++) {
      basket = (TBasket*)fBaskets.UncheckedAt(i);
      if (basket) continue;
      basket = new TBasket();
      basket->SetBranch(this);
      if (fBasketBytes[i] == 0) {
         fBasketBytes[i] = basket->ReadBasketBytes(fBasketSeek[i],file);
      }
      Int_t badread = basket->ReadBasketBuffers(fBasketSeek[i],fBasketBytes[i],file);
      if (badread) {
         Error("Loadbaskets","Error while reading basket buffer %d of branch %s",i,GetName());
         return -1;
      }
      fBaskets.AddAt(basket,i);
      nimported++;
   }
   return nimported;
}

//______________________________________________________________________________
void TBranch::Print(Option_t *) const
{
//*-*-*-*-*-*-*-*-*-*-*-*Print TBranch parameters*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                    ========================

  const int kLINEND = 77;
  Float_t cx = 1;
  int aLength = strlen (GetTitle());
  if (strcmp(GetName(),GetTitle()) == 0) aLength = 0;
  int len = aLength;
  aLength += (aLength / 54 + 1) * 80 + 100;
  if (aLength < 200) aLength = 200;
  char *bline = new char[aLength];
  Long64_t totBytes = GetTotalSize();
  if (fZipBytes) cx = (fTotBytes+0.00001)/fZipBytes;
  if (len) sprintf(bline,"*Br%5d :%-9s : %-54s *",fgCount,GetName(),GetTitle());
  else     sprintf(bline,"*Br%5d :%-9s : %-54s *",fgCount,GetName()," ");
  if (strlen(bline) > UInt_t(kLINEND)) {
     char *tmp = new char[strlen(bline)+1];
     if (len) strcpy(tmp, GetTitle());
     sprintf(bline,"*Br%5d :%-9s : ",fgCount,GetName());
     int pos = strlen (bline);
     int npos = pos;
     int beg=0, end;
     while (beg < len) {
        for (end=beg+1; end < len-1; end ++)
          if (tmp[end] == ':')  break;
        if (npos + end-beg+1 >= 78) {
           while (npos < kLINEND) {
              bline[pos ++] = ' ';
              npos ++;
           }
           bline[pos ++] = '*';
           bline[pos ++] = '\n';
           bline[pos ++] = '*';
           npos = 1;
           for (; npos < 12; npos ++)
               bline[pos ++] = ' ';
           bline[pos-2] = '|';
        }
        for (int n = beg; n <= end; n ++)
           bline[pos+n-beg] = tmp[n];
        pos += end-beg+1;
        npos += end-beg+1;
        beg = end+1;
     }
     while (npos < kLINEND) {
        bline[pos ++] = ' ';
        npos ++;
     }
     bline[pos ++] = '*';
     bline[pos] = '\0';
     delete[] tmp;
  }
  Printf(bline);
  if (fTotBytes > 2000000000) {
     Printf("*Entries :%lld : Total  Size=%11lld bytes  File Size  = %lld *",fEntries,totBytes,fZipBytes);
  } else {
     if (fZipBytes > 0) {
        Printf("*Entries :%9lld : Total  Size=%11lld bytes  File Size  = %10lld *",fEntries,totBytes,fZipBytes);
     } else {
        if (fWriteBasket > 0) {
           Printf("*Entries :%9lld : Total  Size=%11lld bytes  All baskets in memory   *",fEntries,totBytes);
        } else {
           Printf("*Entries :%9lld : Total  Size=%11lld bytes  One basket in memory    *",fEntries,totBytes);
        }
     }
  }
  Printf("*Baskets :%9d : Basket Size=%11d bytes  Compression= %6.2f     *",fWriteBasket,fBasketSize,cx);
  Printf("*............................................................................*");
  delete [] bline;
  fgCount++;
}


//______________________________________________________________________________
void TBranch::ReadBasket(TBuffer &)
{
//*-*-*-*-*-*-*-*Loop on all leaves of this branch to read Basket buffer*-*-*
//*-*            =======================================================

//   fLeaves->ReadBasket(basket);
}

//______________________________________________________________________________
void TBranch::ReadLeaves(TBuffer &b)
{
  for (Int_t i=0;i<fNleaves;i++) {
    TLeaf *leaf = (TLeaf*)fLeaves.UncheckedAt(i);
    leaf->ReadBasket(b);
  }
}

//______________________________________________________________________________
void TBranch::Refresh(TBranch *b)
{
//  refresh this branch using new information in b
//  This function is called by TTree::Refresh

   fEntryOffsetLen = b->fEntryOffsetLen;
   fWriteBasket    = b->fWriteBasket;
   fEntryNumber    = b->fEntryNumber;
   fMaxBaskets     = b->fMaxBaskets;
   fEntries        = b->fEntries;
   fTotBytes       = b->fTotBytes;
   fZipBytes       = b->fZipBytes;
   fReadBasket     = 0;
   fNBasketRAM     = 0;
   delete [] fBasketBytes;
   delete [] fBasketEntry;
   delete [] fBasketSeek;
   fBasketBytes = new Int_t[fMaxBaskets];
   fBasketEntry = new Long64_t[fMaxBaskets];
   fBasketSeek  = new Long64_t[fMaxBaskets];
   Int_t i;
   for (i=0;i<fMaxBaskets;i++) {
      fBasketBytes[i] = b->fBasketBytes[i];
      fBasketEntry[i] = b->fBasketEntry[i];
      fBasketSeek[i]  = b->fBasketSeek[i];
   }
   fBaskets.Delete();
   Int_t nbaskets = b->fBaskets.GetSize();
   fBaskets.Expand(nbaskets);
   //The current fWritebasket must always be in memory.
   //Take it (just s swap) from the Tree being read
   TBasket *basket = (TBasket*)b->fBaskets.UncheckedAt(fWriteBasket);
   fBaskets.AddAt(basket,fWriteBasket);
   b->fBaskets.RemoveAt(fWriteBasket);
   basket->SetBranch(this);
}

//______________________________________________________________________________
void TBranch::Reset(Option_t *)
{
//*-*-*-*-*-*-*-*Reset a Branch*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*            ====================
//
//    Existing buffers are deleted
//    Entries, max and min are reset
//

   Int_t nbaskets = fBaskets.GetEntries();
   fBaskets.Delete();
   fReadBasket     = 0;
   fReadEntry      = -1;
   fWriteBasket    = 0;
   fEntries        = 0;
   fTotBytes       = 0;
   fZipBytes       = 0;
   fEntryNumber    = 0;
   for (Int_t i=0;i<fMaxBaskets;i++) {
      if (fBasketBytes) fBasketBytes[i] = 0;
      if (fBasketEntry) fBasketEntry[i] = 0;
      if (fBasketSeek)  fBasketSeek[i]  = 0;
   }
   if (nbaskets) {
      TBasket *basket = new TBasket(GetName(),fTree->GetName(),this);
      fBaskets.AddAt(basket,0);
   }
}

//______________________________________________________________________________
void TBranch::ResetAddress()
{
//*-*-*-*-*-*-*-*Reset the address of the branch*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*            ===============================
//

   fAddress = 0;
   Int_t i;
   for (i=0;i<fNleaves;i++) {
      TLeaf *leaf = (TLeaf*)fLeaves.UncheckedAt(i);
      leaf->SetAddress(0);
   }
   Int_t nbranches = fBranches.GetEntriesFast();
   for (i=0;i<nbranches;i++)  {
      TBranch *abranch = (TBranch*)fBranches[i];
      abranch->ResetAddress();
   }
}

//______________________________________________________________________________
void TBranch::SetAddress(void *add)
{
//*-*-*-*-*-*-*-*Set address of this branch*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*            ====================
//

   if (TestBit(kDoNotProcess)) return;
   fReadEntry = -1;
   fAddress = (char*)add;
   Int_t i,offset;
   for (i=0;i<fNleaves;i++) {
      TLeaf *leaf = (TLeaf*)fLeaves.UncheckedAt(i);
      offset = leaf->GetOffset();
      if (TestBit(kIsClone)) offset = 0;
      leaf->SetAddress(fAddress+offset);
   }
}

//______________________________________________________________________________
void TBranch::SetAutoDelete(Bool_t autodel)
{
//*-*-*-*-*-*-*-*Set the AutoDelete bit
//*-*            ====================
// this bit is used by TBranchObject::ReadBasket to decide if an object
// referenced by a TBranchObject must be deleted or not before reading
// a new entry
// if autodel is kTRUE, this existing object will be deleted, a new object
//    created by the default constructor, then object->Streamer
// if autodel is kFALSE, the existing object is not deleted. Root assumes
//    that the user is taking care of deleting any internal object or array
//    This can be done in Streamer itself.

   if (autodel) SetBit(kAutoDelete,1);
   else         SetBit(kAutoDelete,0);
}

//______________________________________________________________________________
void TBranch::SetBasketSize(Int_t buffsize)
{
// Set the basket size
// The function makes sure that the basket size is greater than fEntryOffsetlen

   if (buffsize < 100+fEntryOffsetLen) buffsize = 100+fEntryOffsetLen;
   fBasketSize = buffsize;
}

//______________________________________________________________________________
void TBranch::SetBufferAddress(TBuffer *buf)
{
   // Set address of this branch directly from a TBuffer to avoid streaming.

   // Check this is possible
   if ( (fNleaves != 1)
       || (strcmp("TLeafObject",fLeaves.UncheckedAt(0)->ClassName())!=0) ) {
      Error("TBranch::SetAddress","Filling from a TBuffer can only be done with a not split object branch.  Request ignored.");
   } else {
      fReadEntry = -1;
      fEntryBuffer = buf;
   }
}

//______________________________________________________________________________
void TBranch::SetCompressionLevel(Int_t level)
{
//*-*-*-*-*-*-*-*Set the branch/subbranches compression level
//*-*            ============================================

   fCompress = level;
   Int_t nb = fBranches.GetEntriesFast();

   for (Int_t i=0;i<nb;i++) {
      TBranch *branch = (TBranch*)fBranches.UncheckedAt(i);
      branch->SetCompressionLevel(level);
   }
}

//______________________________________________________________________________
void TBranch::SetFile(TFile *file)
{
   // Set file where this branch writes/reads its buffers.
   // By default the branch buffers reside in the file where the
   // Tree was created.
   // If the file name where the tree was created is an absolute
   // path name or an URL (e.g. /castor/... or root://host/...)
   // and if the fname is not an absolute path name or an URL then
   // the path of the tree file is prepended to fname to make the
   // branch file relative to the tree file. In this case one can
   // move the tree + all branch files to a different location in
   // the file system and still access the branch files.
   // The ROOT file will be connected only when necessary.
   // If called by TBranch::Fill (via TBasket::WriteFile), the file
   // will be created with the option "recreate".
   // If called by TBranch::GetEntry (via TBranch::GetBasket), the file
   // will be opened in read mode.
   // To open a file in "update" mode or with a certain compression
   // level, use TBranch::SetFile(TFile *file).

   if (file == 0) file = fTree->GetCurrentFile();
   fDirectory = (TDirectory*)file;
   if (file == fTree->GetCurrentFile()) fFileName = "";
   else                                 fFileName = file->GetName();

   //apply to all existing baskets
   TIter nextb(GetListOfBaskets());
   TBasket *basket;
   while ((basket = (TBasket*)nextb())) {
      basket->SetParent(file);
   }

   //apply to sub-branches as well
   TIter next(GetListOfBranches());
   TBranch *branch;
   while ((branch = (TBranch*)next())) {
      branch->SetFile(file);
   }
}

//______________________________________________________________________________
void TBranch::SetFile(const char *fname)
{
   // Set file where this branch writes/reads its buffers.
   // By default the branch buffers reside in the file where the
   // Tree was created.
   // If the file name where the tree was created is an absolute
   // path name or an URL (e.g. /castor/... or root://host/...)
   // and if the fname is not an absolute path name or an URL then
   // the path of the tree file is prepended to fname to make the
   // branch file relative to the tree file. In this case one can
   // move the tree + all branch files to a different location in
   // the file system and still access the branch files.
   // The ROOT file will be connected only when necessary.
   // If called by TBranch::Fill (via TBasket::WriteFile), the file
   // will be created with the option "recreate".
   // If called by TBranch::GetEntry (via TBranch::GetBasket), the file
   // will be opened in read mode.
   // To open a file in "update" mode or with a certain compression
   // level, use TBranch::SetFile(TFile *file).

   fFileName  = fname;
   fDirectory = 0;

   //apply to sub-branches as well
   TIter next(GetListOfBranches());
   TBranch *branch;
   while ((branch = (TBranch*)next())) {
      branch->SetFile(fname);
   }
}

//_______________________________________________________________________
void TBranch::Streamer(TBuffer &b)
{
//*-*-*-*-*-*-*-*-*Stream a class object*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*              =========================================
   if (b.IsReading()) {
      UInt_t R__s, R__c;
      fTree = gTree;
      fAddress = 0;
      gROOT->SetReadingObject(kTRUE);
      Version_t v = b.ReadVersion(&R__s, &R__c);
      if (v > 9) {
         TBranch::Class()->ReadBuffer(b, this, v, R__s, R__c);

         fDirectory = gDirectory;
         if (fFileName.Length() != 0) fDirectory = 0;
         fNleaves = fLeaves.GetEntriesFast();
         for (Int_t i=0;i<fNleaves;i++) {
            TLeaf *leaf = (TLeaf*)fLeaves.UncheckedAt(i);
            leaf->SetBranch(this);
         }
         if (!fSplitLevel && fBranches.GetEntriesFast()) fSplitLevel = 1;
         gROOT->SetReadingObject(kFALSE);
         // Check Byte Count is not needed since it was done in ReadBuffer
         return;
      }
      //====process old versions before automatic schema evolution
      if (v > 5) {

         Int_t i,ijunk;
         Stat_t djunk;
         TNamed::Streamer(b);
         if (v > 7) TAttFill::Streamer(b);
         b >> fCompress;
         b >> fBasketSize;
         b >> fEntryOffsetLen;
         b >> fWriteBasket;
         b >> ijunk; fEntryNumber = (Long64_t)ijunk;
         b >> fOffset;
         b >> fMaxBaskets;
         if (v > 6) b >> fSplitLevel;
         b >> djunk; fEntries  = (Long64_t)djunk;
         b >> djunk; fTotBytes = (Long64_t)djunk;
         b >> djunk; fZipBytes = (Long64_t)djunk;

         fBranches.Streamer(b);
         gBranch = this;  // must be set again, was changed in previous statement
         fLeaves.Streamer(b);
         fBaskets.Streamer(b);
         fBasketBytes = new Int_t[fMaxBaskets];
         fBasketEntry = new Long64_t[fMaxBaskets];
         fBasketSeek  = new Long64_t[fMaxBaskets];
         Char_t isArray;
         b >> isArray;
         b.ReadFastArray(fBasketBytes,fMaxBaskets);
         b >> isArray;
         for (i=0;i<fMaxBaskets;i++) {b >> ijunk; fBasketEntry[i] = ijunk;}
         b >> isArray;
         for (i=0;i<fMaxBaskets;i++) {
            if (isArray == 2) b >> fBasketSeek[i];
            else              {Int_t bsize; b >> bsize; fBasketSeek[i] = (Long64_t)bsize;};
         }
         fFileName.Streamer(b);
         b.CheckByteCount(R__s, R__c, TBranch::IsA());
         fDirectory = gDirectory;
         if (fFileName.Length() != 0) fDirectory = 0;
         fNleaves = fLeaves.GetEntriesFast();
         for (i=0;i<fNleaves;i++) {
            TLeaf *leaf = (TLeaf*)fLeaves.UncheckedAt(i);
            leaf->SetBranch(this);
         }
         if (!fSplitLevel && fBranches.GetEntriesFast()) fSplitLevel = 1;
         gROOT->SetReadingObject(kFALSE);
         b.CheckByteCount(R__s, R__c, TBranch::IsA());
         return;
      }
      //====process very old versions
      Int_t n,i,ijunk;
      Stat_t djunk;
      TNamed::Streamer(b);
      b >> fCompress;
      b >> fBasketSize;
      b >> fEntryOffsetLen;
      b >> fMaxBaskets;
      b >> fWriteBasket;
      b >> ijunk; fEntryNumber = (Long64_t)ijunk;
      b >> djunk; fEntries  = (Long64_t)djunk;
      b >> djunk; fTotBytes = (Long64_t)djunk;
      b >> djunk; fZipBytes = (Long64_t)djunk;
      b >> fOffset;
      fBranches.Streamer(b);
      gBranch = this;  // must be set again, was changed in previous statement
      fLeaves.Streamer(b);
      fBaskets.Streamer(b);
      fNleaves = fLeaves.GetEntriesFast();
      fBasketEntry = new Long64_t[fMaxBaskets];
      b >> n;
      for (i=0;i<n;i++) {b >> ijunk; fBasketEntry[i] = ijunk;}
      fBasketBytes = new Int_t[fMaxBaskets];
      if (v > 4) {
         n  = b.ReadArray(fBasketBytes);
      } else {
         for (n=0;n<fMaxBaskets;n++) fBasketBytes[n] = 0;
      }
      if (v < 2) {
         fBasketSeek = new Long64_t[fMaxBaskets];
         for (n=0;n<fWriteBasket;n++) {
            fBasketSeek[n] = GetBasket(n)->GetSeekKey();
         }
      } else {
         fBasketSeek = new Long64_t[fMaxBaskets];
         b >> n;
         for (n=0;n<fMaxBaskets;n++) {
            Int_t aseek;
            b >> aseek;
            fBasketSeek[n] = Long64_t(aseek);
         }
      }
      fDirectory = gDirectory;
      if (v > 2) {
         fFileName.Streamer(b);
         if (fFileName.Length() != 0) fDirectory = 0;
      }
      if (v < 4) SetAutoDelete(kTRUE);
      if (!fSplitLevel && fBranches.GetEntriesFast()) fSplitLevel = 1;
      gROOT->SetReadingObject(kFALSE);
      b.CheckByteCount(R__s, R__c, TBranch::IsA());
      //====end of old versions

   } else {
      fMaxBaskets = fBaskets.GetEntriesFast();
      if (fMaxBaskets < 10) fMaxBaskets=10;
      TBranch::Class()->WriteBuffer(b,this);
   }
}

//_______________________________________________________________________
void TBranch::WriteBasket(TBasket* basket)
{
//*-*-*-*-*-*-*-*-*Write the current basket to disk*-*-*-*-*
//*-*              =========================================

   Int_t nout  = basket->WriteBuffer();    //  Write buffer
   fBasketBytes[fWriteBasket]  = basket->GetNbytes();
   fBasketSeek[fWriteBasket]   = basket->GetSeekKey();
   Int_t addbytes = basket->GetObjlen() + basket->GetKeylen() ;
   if (fDirectory != gROOT && fDirectory->IsWritable()) {
      delete basket;
      fBaskets[fWriteBasket] = 0;
   }
   fZipBytes += nout;
   fTotBytes += addbytes;
   fTree->AddTotBytes(addbytes);
   fTree->AddZipBytes(nout);
   basket = new TBasket(GetName(),fTree->GetName(),this);   //  create a new basket
   fWriteBasket++;
   fBaskets.AddAtAndExpand(basket,fWriteBasket);
   if (fWriteBasket >= fMaxBaskets) {
      //Increase BasketEntry buffer of a minimum of 10 locations
      // and a maximum of 50 per cent of current size
      Int_t newsize = TMath::Max(10,Int_t(1.5*fMaxBaskets));
      fBasketBytes  = TStorage::ReAllocInt(fBasketBytes, newsize, fMaxBaskets);
      fBasketEntry  = (Long64_t*)TStorage::ReAlloc(fBasketEntry,
                          newsize*sizeof(Long64_t),fMaxBaskets*sizeof(Long64_t));
      fBasketSeek   = (Long64_t*)TStorage::ReAlloc(fBasketSeek,
                          newsize*sizeof(Long64_t),fMaxBaskets*sizeof(Long64_t));
      fMaxBaskets   = newsize;
   }

   for (Int_t i=fWriteBasket;i<fMaxBaskets;i++) {
      fBasketBytes[i] = 0;
      fBasketEntry[i] = 0;
      fBasketSeek[i]  = 0;
   }
   fBasketEntry[fWriteBasket] = fEntryNumber;
}

