// @(#)root/base:$Name:  $:$Id: TArchiveFile.cxx,v 1.2 2004/07/19 09:40:48 rdm Exp $
// Author: Fons Rademakers   30/6/04

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TArchiveFile                                                         //
//                                                                      //
// This is an abstract class that describes an archive file containing  //
// multiple sub-files, like a ZIP or TAR archive.                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TArchiveFile.h"
#include "TPluginManager.h"
#include "TROOT.h"
#include "TObjArray.h"
#include "TError.h"


ClassImp(TArchiveFile)

//______________________________________________________________________________
TArchiveFile::TArchiveFile(const char *archive, const char *member, TFile *file)
{
   // Specify the archive name and member name. The member can be a decimal
   // number which allows to access the n-th sub-file. This method is
   // normally only called via TFile.

   if (!file)
      Error("TArchiveFile", "must specify a valid TFile");

   fFile        = file;
   fArchiveName = archive;
   fMemberName  = member;
   fMemberIndex = -1;
   if (fMemberName.IsDigit())
      fMemberIndex = atoi(fMemberName);
   fMembers     = new TObjArray;
   fMembers->SetOwner();
   fCurMember   = 0;
}

//______________________________________________________________________________
TArchiveFile::~TArchiveFile()
{
   // Dtor.

   delete fMembers;
}

//______________________________________________________________________________
Long64_t TArchiveFile::GetMemberFilePosition() const
{
   // Return position in archive of current member.

   return fCurMember ? fCurMember->GetFilePosition() : 0;
}

//______________________________________________________________________________
Int_t TArchiveFile::GetNumberOfMembers() const
{
   // Returns number of members in archive.

   return fMembers->GetEntriesFast();
}

//______________________________________________________________________________
Int_t TArchiveFile::SetMember(const char *member)
{
   // Explicitely make the specified member the current member.
   // Returns -1 in case of error, 0 otherwise.

   fMemberName  = member;
   fMemberIndex = -1;

   return SetCurrentMember();
}

//______________________________________________________________________________
Int_t TArchiveFile::SetMember(Int_t idx)
{
   // Explicitely make the member with the specified index the current member.
   // Returns -1 in case of error, 0 otherwise.

   fMemberName  = "";
   fMemberIndex = idx;

   return SetCurrentMember();
}

//______________________________________________________________________________
TArchiveFile *TArchiveFile::Open(const char *url, TFile *file)
{
   // Return proper archive file handler depending on passed url.
   // The handler is loaded via the plugin manager and is triggered by
   // the extension of the archive file. In case no handler is found 0
   // is returned. The file argument is used to access the archive.
   // The archive should be specified as url with the member name as the
   // anchor, e.g. "root://pcsalo.cern.ch/alice/event_1.zip#tpc.root",
   // where tpc.root is the file in the archive to be opened.
   // Alternatively the sub-file can be specified via its index number,
   // e.g. "root://pcsalo.cern.ch/alice/event_1.zip#3".
   // This function is normall only called via TFile::Open().

   if (!file) {
      ::Error("TArchiveFile::Open", "must specify a valid TFile to access %s",
              url);
      return 0;
   }

   TString archive, member;

   if (!ParseUrl(url, archive, member))
      return 0;

   TArchiveFile *f = 0;
   TPluginHandler *h;
   if ((h = gROOT->GetPluginManager()->FindHandler("TArchiveFile", archive))) {
      if (h->LoadPlugin() == -1)
         return 0;
      f = (TArchiveFile*) h->ExecPlugin(3, archive.Data(), member.Data(), file);
   }

   return f;
}

//______________________________________________________________________________
Bool_t TArchiveFile::ParseUrl(const char *url, TString &archive, TString &member)
{
   // Try to determine if url contains an anchor specifying an archive member.
   // Returns kFALSE in case of an error.

   TString u = url;
   archive   = "";
   member    = "";

   Int_t idx;
   if ((idx = u.Last('#')) == kNPOS) {
      archive = url;
      return kTRUE;
   }

   archive = u(0, idx);
   member  = u(idx+1, u.Length()-1);

   if (archive == "" || member == "") {
      archive = "";
      member  = "";
      return kFALSE;
   }
   return kTRUE;
}


ClassImp(TArchiveMember)

//______________________________________________________________________________
TArchiveMember::TArchiveMember()
{
   // Default ctor.

   fName         = "";
   fComment      = "";
   fPosition     = 0;
   fFilePosition = 0;
   fCsize        = 0;
   fDsize        = 0;
   fDirectory    = kFALSE;
}

//______________________________________________________________________________
TArchiveMember::TArchiveMember(const char *name)
{
   // Create an archive member file.

   fName         = name;
   fComment      = "";
   fPosition     = 0;
   fFilePosition = 0;
   fCsize        = 0;
   fDsize        = 0;
   fDirectory    = kFALSE;
}

//______________________________________________________________________________
TArchiveMember::TArchiveMember(const TArchiveMember &member)
   : TObject(member)
{
   // Copy ctor.

   fName         = member.fName;
   fComment      = member.fComment;
   fModTime      = member.fModTime;
   fPosition     = member.fPosition;
   fFilePosition = member.fFilePosition;
   fCsize        = member.fCsize;
   fDsize        = member.fDsize;
   fDirectory    = member.fDirectory;
}

//______________________________________________________________________________
TArchiveMember &TArchiveMember::operator=(const TArchiveMember &rhs)
{
   // Assignment operator.

   if (this != &rhs) {
      TObject::operator=(rhs);
      fName         = rhs.fName;
      fComment      = rhs.fComment;
      fModTime      = rhs.fModTime;
      fPosition     = rhs.fPosition;
      fFilePosition = rhs.fFilePosition;
      fCsize        = rhs.fCsize;
      fDsize        = rhs.fDsize;
      fDirectory    = rhs.fDirectory;
   }
   return *this;
}
