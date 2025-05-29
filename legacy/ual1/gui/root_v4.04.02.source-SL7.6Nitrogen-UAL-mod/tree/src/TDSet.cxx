// @(#)root/tree:$Name:  $:$Id: TDSet.cxx,v 1.18 2005/05/02 10:57:32 rdm Exp $
// Author: Fons Rademakers   11/01/02

/*************************************************************************
 * Copyright (C) 1995-2001, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TDSet                                                                //
//                                                                      //
// This class implements a data set to be used for PROOF processing.    //
// The TDSet defines the class of which objects will be processed,      //
// the directory in the file where the objects of that type can be      //
// found and the list of files to be processed. The files can be        //
// specified as logical file names (LFN's) or as physical file names    //
// (PFN's). In case of LFN's the resolution to PFN's will be done       //
// according to the currently active GRID interface.                    //
// Examples:                                                            //
//   TDSet treeset("TTree", "AOD");                                     //
//   treeset.Add("lfn:/alien.cern.ch/alice/prod2002/file1");            //
//   ...                                                                //
//   treeset.AddFriend(friendset);                                      //
//                                                                      //
// or                                                                   //
//                                                                      //
//   TDSet objset("MyEvent", "*", "/events");                           //
//   objset.Add("root://cms.cern.ch/user/prod2002/hprod_1.root");       //
//   ...                                                                //
//   objset.Add(set2003);                                               //
//                                                                      //
// Validity of file names will only be checked at processing time       //
// (typically on the PROOF master server), not at creation time.        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TDSet.h"

#include "Riostream.h"
#include "TClass.h"
#include "TClassTable.h"
#include "TCut.h"
#include "TError.h"
#include "TFile.h"
#include "TGrid.h"
#include "TGridProof.h"
#include "TGridResult.h"
#include "TKey.h"
#include "TList.h"
#include "TMap.h"
#include "TROOT.h"
#include "TTimeStamp.h"
#include "TTree.h"
#include "TUrl.h"
#include "TVirtualPerfStats.h"
#include "TVirtualProof.h"
#include "TChainProof.h"
#include "TPluginManager.h"
#include "TChain.h"
#include "TChainElement.h"


ClassImp(TDSetElementPfn)
ClassImp(TDSetElementMsn)
ClassImp(TDSetElement)
ClassImp(TDSet)

//______________________________________________________________________________
void TDSetElementPfn::Print(Option_t *) const
{
   // Print contents of a physical file element.

   printf("\tPFN: %-40s MSN: %-25s SIZE: %9lld CEN: %25s\n",
          GetPfn(), GetMsn(), GetSize(), GetCen());
}


//______________________________________________________________________________
TDSetElementMsn::TDSetElementMsn(TDSetElementPfn *dse)
{
   // Create mass storage information element.

   fMsn                  = dse ? dse->GetMsn() : "";
   fNfiles               = 1;
   fDataSize             = dse ? dse->GetSize() : 0;
   fNSiteDaemons         = -1;
   fMaxSiteDaemons       = 50;
   fDataPerSiteDaemon    = -1;
   fMaxDataPerSiteDaemon = R__LL(5000000000); // heuristic 5 GByte
}

//______________________________________________________________________________
void TDSetElementMsn::Print(Option_t *) const
{
   // Print contents of a Mass Storage element.

   printf("MSN: %-32s nfiles: %-8d ndaemon: %-8d data[bytes]: %16lld\n",
          GetMsn(), GetNfiles(), GetNSiteDaemons(), GetDataSize());
}


//______________________________________________________________________________
TDSetElement::TDSetElement(const TDSet *set, const char *file,
                           const char *objname, const char *dir,
                           Long64_t first, Long64_t num,
                           const char *msd)
{
   // Create a TDSet element.

   fSet      = set;
   fFileName = file;
   if (first < 0) {
      Warning("TDSetElement", "first must be >= 0, %d is not allowed - setting to 0", first);
      fFirst = 0;
   } else {
      fFirst = first;
   }
   if (num < -1) {
      Warning("TDSetElement", "num must be >= -1, %d is not allowed - setting to -1", num);
      fNum   = -1;
   } else {
      fNum   = num;
   }
   fMsd         = msd;
   fPfnList     = 0;
   fIterator    = 0;
   fCurrent     = 0;
   fTDSetOffset = 0;
   fEventList   = 0;
   fValid       = kFALSE;

   if (objname)
      fObjName = objname;
   if (dir)
      fDirectory = dir;
}

//______________________________________________________________________________
TDSetElement::~TDSetElement()
{
   // Clean up the element.

   delete fIterator;
   delete fPfnList;
//   SafeDelete(fEventList);
}

//______________________________________________________________________________
void TDSetElement::AddPfn(const char *pfn, const char *msn, Long64_t size)
{
   // Add associated physical file name to this element.

   if (!fPfnList) {
      fPfnList = new TList;
      fPfnList->SetOwner();
   }
   fPfnList->Add(new TDSetElementPfn(pfn, msn, size));
}

//______________________________________________________________________________
const char *TDSetElement::GetObjName() const
{
   // Return object name.

   if (fSet && fObjName.IsNull())
      return fSet->GetObjName();
   return fObjName;
}

//______________________________________________________________________________
const char *TDSetElement::GetDirectory() const
{
   // Return directory where to look for object.

   if (fSet && fDirectory.IsNull())
      return fSet->GetDirectory();
   return fDirectory;
}

//______________________________________________________________________________
void TDSetElement::Reset()
{
   // Reset PFN list iterator.

  if (!fIterator) {
     fIterator = new TIter(fPfnList);
  } else {
     fIterator->Reset();
  }
}

//______________________________________________________________________________
TDSetElementPfn *TDSetElement::Next()
{
   // Get next PFN element.

   if (!fIterator) {
     fIterator = new TIter(fPfnList);
   }
   fCurrent = (TDSetElementPfn *) fIterator->Next();
   return fCurrent;
}

//______________________________________________________________________________
void TDSetElement::Print(Option_t *opt) const
{
   // Print a TDSetElement. When option="a" print full data.

   if (opt && opt[0] == 'a') {
      cout << IsA()->GetName()
           << " file='" << fFileName
           << "' dir='" << fDirectory
           << "' obj='" << fObjName
           << "' first=" << fFirst
           << " num=" << fNum
           << " msd=" << fMsd
           << endl;
   } else
      cout << "\tLFN: " << fFileName << endl;

   TIter next(fPfnList);

   while (TDSetElementPfn *pfn = (TDSetElementPfn *) next())
      pfn->Print(opt);
}

//______________________________________________________________________________
void TDSetElement::Validate()
{
   // Validate by opening the file.

   if (fSet) {
      Long64_t entries = TDSet::GetEntries(fSet->IsTree(), fFileName,
                                           fDirectory, fObjName);
      if (entries < 0) return; // Error should be reported by GetEntries()
      if (fFirst < entries) {
         if (fNum == -1) {
            fNum = entries - fFirst;
            fValid = kTRUE;
         } else {
            if (fNum <= entries - fFirst) {
               fValid = kTRUE;
            } else {
               Error("Validate", "TDSetElement has only %d entries starting"
                     " with entry %d, while %d were requested",
                     entries - fFirst, fFirst, fNum);
            }
         }
      } else {
         Error("Validate", "TDSetElement has only %d entries with"
               " first entry requested as %d", entries, fFirst);
      }
   } else {
      Error("Validate", "No TDSet associated with TDSetElement"
            " - cannot figure out type");
   }
}

//______________________________________________________________________________
void TDSetElement::Validate(TDSetElement* elem)
{
   // Validate by checking against another element.

   // NOTE: Since this function validates against another TDSetElement,
   //       if the other TDSetElement (elem) did not use -1 to request all
   //       entries, this TDSetElement may get less than all entries if it
   //       requests all (with -1). For the application it was developed for
   //       (TProofSuperMaster::ValidateDSet) it works, since the design was
   //       to send the elements to their mass storage domain and let them
   //       look at the file and send the info back to the supermaster. The
   //       ability to set fValid was also required to be only exist in
   //       TDSetElement through certain function and not be set externally.
   //       TDSetElement may need to be extended for more general applications.

   if (!elem || !elem->GetValid()) {
      Error("Validate", "TDSetElement to validate against is not valid");
      return;
   }

   if (!strcmp(GetFileName(), elem->GetFileName()) &&
       !strcmp(GetDirectory(), elem->GetDirectory()) &&
       !strcmp(GetObjName(), elem->GetObjName())) {
      Long64_t entries = elem->fFirst + elem->fNum;
      if (fFirst < entries) {
         if (fNum == -1) {
            fNum = entries - fFirst;
            fValid = kTRUE;
         } else {
            if (fNum <= entries - fFirst) {
               fValid = kTRUE;
            } else {
               Error("Validate", "TDSetElement requests %d entries starting"
                     " with entry %d, while TDSetElement to validate against"
                     " has only %d entries", fNum, fFirst, entries);
            }
         }
      } else {
         Error("Validate", "TDSetElement to validate against has only %d"
               " entries, but this TDSetElement requested %d as its first"
               " entry", entries, fFirst);
      }
   } else {
      Error("Validate", "TDSetElements do not refer to same objects");
   }
}

//______________________________________________________________________________
Int_t TDSetElement::Compare(const TObject *obj) const
{
   //Compare elements by filename (and the fFirst).

   if (this == obj) return 0;

   const TDSetElement *elem = dynamic_cast<const TDSetElement*>(obj);
   if (!elem) {
      if (obj)
         return fFileName.CompareTo(obj->GetName());
      return -1;
   }

   Int_t order = fFileName.CompareTo(elem->GetFileName());
   if (order == 0) {
      if (GetFirst() < elem->GetFirst())
         return -1;
      else if (GetFirst() > elem->GetFirst())
         return 1;
      return 0;
   }
   return order;
}

//______________________________________________________________________________
TDSet::TDSet()
{
   // Default ctor.

   fElements = new TList;
   fElements->SetOwner();
   fElementsMsn = new TList;
   fElementsMsn->SetOwner();
   fIsTree    = kFALSE;
   fIterator  = 0;
   fCurrent   = 0;
   fEventList = 0;
}

//______________________________________________________________________________
TDSet::TDSet(const char *type, const char *objname, const char *dir)
{
   // Create a TDSet object. The "type" defines the class of which objects
   // will be processed. The optional "objname" argument specifies the
   // name of the objects of the specified class (the name is mandatory
   // if the type inherits from a TTree). If the "objname" is not given all
   // objects of the class found in the specified directory are processed.
   // The "dir" argument specifies in which directory the objects are
   // to be found, the top level directory ("/") is the default.
   // Directories can be specified using wildcards, e.g. "*" or "/*"
   // means to look in all top level directories, "/dir/*" in all
   // directories under "/dir", and "/*/*" to look in all directories
   // two levels deep.

   fElements = new TList;
   fElements->SetOwner();
   fElementsMsn = new TList;
   fElementsMsn->SetOwner();
   fIterator = 0;
   fCurrent  = 0;
   fEventList = 0;

   if (!type || !*type) {
      Error("TDSet", "type name must be specified");
      return;
   }

   TClass *c;
   if (!(c = gROOT->GetClass(type)))
      Warning("TDSet", "type %s not yet known", type);

   fName   = type;
   fIsTree = kFALSE;

   if (c && c->InheritsFrom("TTree"))
      fIsTree = kTRUE;

   if (objname)
      fObjName = objname;

   if (dir)
      fTitle = dir;
}

//______________________________________________________________________________
TDSet::~TDSet()
{
   // Cleanup.

   delete fElementsMsn;
   delete fElements;
   delete fIterator;
}

//______________________________________________________________________________
Bool_t TDSet::Request()
{
   // Request a connection to a GRID based PROOF.

   if (!gGrid) {
      if (!gProof) {
         Error("Request", "no need to do Request(), you have no active Grid");
         return kFALSE;
      } else {
         return kTRUE;
      }
   }

   if (!gGrid->GetGridProof()) {
      gGrid->CreateGridProof();
   }

   TGridProof* gridproof = gGrid->GetGridProof();
   return gridproof->Request(this);
}

//______________________________________________________________________________
Bool_t TDSet::Connect()
{
   // Connect to GRID based PROOF.

   if (!gGrid) {
      if (!gProof) {
         Error("Connect", "cannot connect, no active Grid or PROOF session open");
         return kFALSE;
      } else {
         return kTRUE;
      }
   }

   TGridProof *gridproof = 0;
   if (!(gridproof = gGrid->GetGridProof())) {
      Error("Connect", "first execute Request() to obtain a GridProof object");
      return kFALSE;
   }

   gridproof->Connect();
   return kTRUE;
}

//______________________________________________________________________________
Int_t TDSet::Process(const char *selector, Option_t *option, Long64_t nentries,
                     Long64_t first, TEventList *evl)
{
   // Process TDSet on currently active PROOF session.
   // Returns -1 in case of error, 0 otherwise.

   if (!IsValid() || !fElements->GetSize()) {
      Error("Process", "not a correctly initialized TDSet");
      return -1;
   }

   if (gProof)
      return gProof->Process(this, selector, option, nentries, first, evl);

   Error("Process", "no active PROOF session");
   return -1;
}

//______________________________________________________________________________
void TDSet::AddInput(TObject *obj)
{
   // Add objects that might be needed during the processing of
   // the selector (see Process()).

   if (gProof) {
      gProof->AddInput(obj);
   } else {
      Error("AddInput","No PROOF session active");
   }
}

//______________________________________________________________________________
void TDSet::ClearInput()
{
   // Clear input object list.

   if (gProof)
      gProof->ClearInput();
}

//______________________________________________________________________________
TObject *TDSet::GetOutput(const char *name)
{
   // Get specified object that has been produced during the processing
   // (see Process()).

   if (gProof)
      return gProof->GetOutput(name);
   return 0;
}

//______________________________________________________________________________
TList *TDSet::GetOutputList()
{
   // Get list with all object created during processing (see Process()).

   if (gProof)
      return gProof->GetOutputList();
   return 0;
}

//______________________________________________________________________________
void TDSet::Print(const Option_t *opt) const
{
   // Print TDSet basic or full data. When option="a" print full data.

   cout <<"OBJ: " << IsA()->GetName() << "\ttype " << GetName() << "\t"
        << fObjName << "\tin " << GetTitle()
        << "\telements " << GetListOfElements()->GetSize() << endl;

   if (opt && opt[0] == 'a') {
      TIter next(GetListOfElements());
      TObject *obj;
      while ((obj = next())) {
         obj->Print(opt);
      }
   }
}

//______________________________________________________________________________
void TDSet::SetObjName(const char *objname)
{
   // Set/change object name.

   if (objname)
      fObjName = objname;
}

//______________________________________________________________________________
void TDSet::SetDirectory(const char *dir)
{
   // Set/change directory.

   if (dir)
      fTitle = dir;
}

//______________________________________________________________________________
Bool_t TDSet::Add(const char *file, const char *objname, const char *dir,
                  Long64_t first, Long64_t num, const char *msd)
{
   // Add file to list of files to be analyzed. Optionally with the
   // objname and dir arguments the default, TDSet wide, objname and
   // dir can be overridden.

   if (!file || !*file) {
      Error("Add", "file name must be specified");
      return kFALSE;
   }

   // check, if it already exists in the TDSet
   TDSetElement *el;
   Reset();
   while ((el = Next())) {
      if (!(strcmp(el->GetFileName(), file))) {
         Warning("Add", "duplicate, %40s is already in dataset, ignored", file);
         return kFALSE;
      }
   }

   // try, if it is a GRID lfn
   if ((GridAdd(file, objname, dir, first, num)) < 1) {
      // could not be resolved with the grid, just take it as it is
      fElements->Add(new TDSetElement(this, file, objname, dir, first, num, msd));
   }
   return kTRUE;
}

//______________________________________________________________________________
Int_t TDSet::GridAdd(const char *lfn, const char *objname, const char *dir,
                     Long64_t first, Long64_t num)
{
   // Resolve logical file names using TGrid methods. Returns 1 on success,
   // 0 if there is no grid, -1 if the grid could not resolve the name.

   TUrl lUrl(lfn);
   if (!(strcmp(lUrl.GetProtocol(), ""))) {
      if (!gGrid) {
         Error("TDSet", "cannot resolve LFN, no active GRID");
         return 0;
      }

      if ((strstr(lUrl.GetUrl(), "://"))) {
         if ((strcmp(lUrl.GetProtocol(), "http"))) {
            if ((strcmp(lUrl.GetProtocol(), gGrid->GetGrid()))) {
               Error("TDSet", "LFN %s does not belong to the active Grid %s",
                     lUrl.GetProtocol(), gGrid->GetGrid());
               return 0;
            }
         }
      }

      // get the file size
      TGrid::gridstat_t statbuf;
      statbuf.st_size = -1;
      int gridstat =  gGrid->GridStat(lfn, &statbuf);
      if (gridstat < 0) {
         Error("TDSet", "cannot stat LFN using TGrid::GridStat()");
         return -1;
      }

      TGridResult *lPFN = gGrid->CreateGridResult(gGrid->GetPhysicalFileNames(lUrl.GetFile()));

      fElements->Add(new TDSetElement(this, lfn, objname, dir, first, num));

      TDSetElement *current = (TDSetElement *) fElements->Last();

      while (Grid_Result_t *result = (Grid_Result_t*) lPFN->Next()) {
         char newpfn[4096];
         printf(" SE: %-25s PFN: %-25s\n", result->name.c_str(), result->name2.c_str());
         sprintf(newpfn, "%s@%s", result->name2.c_str(), result->name.c_str());
         current->AddPfn(result->name2.c_str(), result->name.c_str(), statbuf.st_size);
      }

      lPFN->Close();
      delete lPFN;
      return 1;
   }
   return 0;
}

//______________________________________________________________________________
Bool_t TDSet::Add(TDSet *set)
{
   // Add specified data set to the this set.

   if (!set)
      return kFALSE;

   if (set->fName != fName) {
      Error("Add", "cannot add a set with a different type");
      return kFALSE;
   }

   TDSetElement *el;
   TIter next(set->fElements);
   TObject *last = set == this ? fElements->Last() : 0;
   while ((el = (TDSetElement*) next())) {
      Add(el->GetFileName(), el->GetObjName(), el->GetDirectory(),
          el->GetFirst(), el->GetNum(), el->GetMsd());
      if (el == last) break;
   }

   return kTRUE;
}

//______________________________________________________________________________
void TDSet::AddFriend(TDSet *friendset)
{
   // Add friend dataset to this set. Only possible if the TDSet type is
   // a TTree or derived class.

   if (!friendset)
      return;

   if (!fIsTree) {
      Error("AddFriend", "a friend set can only be added to a TTree TDSet");
      return;
   }

   // to be implemented
   Error("AddFriend", "not implemented");
}

//______________________________________________________________________________
void TDSet::Reset()
{
   // Reset or initialize access to the elements.

   if (!fIterator) {
      fIterator = new TIter(fElements);
   } else {
      fIterator->Reset();
   }
}

//______________________________________________________________________________
TDSetElement *TDSet::Next()
{
   // Returns next TDSetElement.

   if (!fIterator) {
      fIterator = new TIter(fElements);
   }

   fCurrent = (TDSetElement *) fIterator->Next();
   return fCurrent;
}

//______________________________________________________________________________
Long64_t TDSet::GetEntries(Bool_t isTree, const char *filename, const char *path,
                           const char *objname)
{
   // Returns number of entries in tree or objects in file. Returns -1 in
   // case of error.

   Double_t start = 0;
   if (gPerfStats != 0) start = TTimeStamp();

   TFile *file = TFile::Open(filename);

   if (gPerfStats != 0) {
      gPerfStats->FileOpenEvent(file, filename, double(TTimeStamp())-start);
   }

   if (file == 0) {
      ::SysError("TDSet::GetEntries", "cannot open file %s", filename);
      return -1;
   }

   TDirectory *dirsave = gDirectory;
   if (!file->cd(path)) {
      ::Error("TDSet::GetEntries", "cannot cd to %s", path);
      delete file;
      return -1;
   }

   TDirectory *dir = gDirectory;
   dirsave->cd();

   Long64_t entries;
   if (isTree) {
      TKey *key = dir->GetKey(objname);
      if (key == 0) {
         ::Error("TDSet::GetEntries", "cannot find tree \"%s\" in %s",
                 objname, filename);
         delete file;
         return -1;
      }
      TTree *tree = (TTree *) key->ReadObj();
      if (tree == 0) {
         // Error always reported?
         delete file;
         return -1;
      }
      entries = tree->GetEntries();
      delete tree;

   } else {
      TList *keys = dir->GetListOfKeys();
      entries = keys->GetSize();
   }

   delete file;
   return entries;
}

//______________________________________________________________________________
Int_t TDSet::Draw(const char *varexp, const TCut &selection, Option_t *option,
                  Long64_t nentries, Long64_t firstentry)
{
   // Draw expression varexp for specified entries.
   // This function accepts a TCut objects as argument.
   // Use the operator+ to concatenate cuts.
   // Example:
   //   dset.Draw("x",cut1+cut2+cut3);

   return Draw(varexp, selection.GetTitle(), option, nentries, firstentry);
}

//______________________________________________________________________________
Int_t TDSet::Draw(const char *varexp, const char *selection, Option_t *option,
                  Long64_t nentries, Long64_t firstentry)
{
   // Draw expression varexp for specified entries.
   // See TTree::Draw().

   if (!IsValid() || !fElements->GetSize()) {
      Error("Draw", "not a correctly initialized TDSet");
      return -1;
   }

   if (gProof)
      return gProof->DrawSelect(this, varexp, selection, option, nentries,
                                firstentry);

   Error("Draw", "no active PROOF session");
   return -1;
}

//______________________________________________________________________________
Bool_t TDSet::AddQuery(const char *path, const char *file,
                       const char *conditions)
{
   // Queries the connected GRID catalog for the file.

   if (!gGrid)
      return kFALSE;

   cout << "--------------------------------------------------------" << endl;
   TGridResult *fQUERY = gGrid->CreateGridResult(gGrid->FindEx(path, file, conditions));

   // loop over all results ....
   int nFile = 0;
   fQUERY->Reset();
   while (Grid_Result_t *result = (Grid_Result_t*) fQUERY->Next()) {
      nFile++;
      // Add the LFN
      if (!Add(result->name.c_str()))
         continue;
      printf("  %4d      LFN: %-40s SZ: %10d PERM: %3x TIME: %10d\n",
             nFile, result->name.c_str(), (int)result->info.st_size,
             result->info.st_mode, (int)result->info.st_atime);

      if (result->data != 0) {

         // find the last TDSetElement to add PFN information
         TDSetElement *current = (TDSetElement *) fElements->Last();

         Grid_Result_t *pfn;
         while ((pfn = (Grid_Result_t*) gGrid->ReadResult(result->data)) != 0) {
            printf("            PFN: %-40s    MSN: %-25s\n",
                   pfn->name.c_str(), pfn->name2.c_str());
            TUrl PfnUrl(pfn->name.c_str());
            current->AddPfn(PfnUrl.GetFile(), pfn->name2.c_str(), result->info.st_size);
         }
      }
   }
   cout << "--------------------------------------------------------" << endl;
   return kTRUE;
}

//______________________________________________________________________________
void TDSet::GridPack()
{
   // Pack a data set corresponding to the mass storage name for
   // PROOF processing on the Grid.

   fElementsMsn->Clear();
   Reset();

   // sum up all data per site
   while (TDSetElement *lfnE = Next()) {
      lfnE->Reset();
      // for the moment, we just consider the primary location of a file
      while (TDSetElementPfn *pfnE = lfnE->Next()) {
         GridAddElementMsn(pfnE);
         break;
      }
   }
   GridPrintPackList();
}

//______________________________________________________________________________
void TDSet::GridPrintPackList()
{
   // Print list of files per mass storage device.

   fElementsMsn->ForEach(TDSetElementMsn,Print)();
   printf("--------------------------------------------------------\n");
}

//______________________________________________________________________________
void TDSet::GridAddElementMsn(TDSetElementPfn *dsepfn)
{
   // Assign a physical file name to a Msn of the mass storage name list.

   TDSetElementMsn *dseme = (TDSetElementMsn*) fElementsMsn->FindObject(dsepfn->GetMsn());
   if (!dseme) {
      dseme = new TDSetElementMsn(dsepfn);
      fElementsMsn->Add(dseme);
   } else {
      dseme->Increment();
      dseme->AddData(dsepfn->GetSize());
   }
}

//_______________________________________________________________________
void TDSet::StartViewer()
{
   // Start the TTreeViewer on this TTree.

   if (gROOT->IsBatch()) {
      Warning("StartViewer", "viewer cannot run in batch mode");
      return;
   }

   if (!gProof) {
      Error("StartViewer", "no PROOF found");
      return;
   }
   if (!IsTree()) {
      Error("StartViewer", "TDSet contents should be of type TTree (or subtype)");
      return;
   }
   TChainProof *w = TChainProof::MakeChainProof(this, gProof);
   // TODO: w should be freed somewhere, probably in the TTreeViewer destructor.
   if (!w) {
      Error("StartViewer", "failure creating a TChainProof");
      return;
   }

   TPluginHandler *h;
   if ((h = gROOT->GetPluginManager()->FindHandler("TVirtualTreeViewer"))) {
      if (h->LoadPlugin() == -1)
         return;
      h->ExecPlugin(1,w);
   }
}

//_______________________________________________________________________
TTree* TDSet::GetTreeHeader(TVirtualProof* proof)
{
   // Returns a tree header containing the branches' structure of the dataset.

   return proof->GetTreeHeader(this);
}

//_______________________________________________________________________
TDSet* TDSet::MakeTDSet(TChain *chain)
{
   // Creates a new TDSet new containing files from te given chain.

   TIter next(chain->GetListOfFiles());
   TChainElement *element;
   TDSet *dset = new TDSet("TTree", chain->GetName());
   while ((element = (TChainElement*)next())) {
      TString file(element->GetTitle());
      TString tree(element->GetName());
      Int_t slashpos = tree.Index("/");
      TString dir;
      if (slashpos>=0) {
         // Copy the tree name specification
         TString behindSlash = tree(slashpos+1,tree.Length()-slashpos-1);
         // and remove it from basename
         tree.Remove(slashpos);
         dir = tree;
         tree = behindSlash;
      }
      dset->Add(file, tree, dir);
   }
   dset->SetDirectory(0);
   return dset;
}

//______________________________________________________________________________
Bool_t TDSet::ElementsValid() const
{
   // Check if all elemnts are valid.

   TIter NextElem(GetListOfElements());
   while (TDSetElement *elem = dynamic_cast<TDSetElement*>(NextElem())) {
      if (!elem->GetValid()) return kFALSE;
   }
   return kTRUE;

}

//______________________________________________________________________________
void TDSet::Validate()
{
   // Validate the TDSet by opening files.

   TIter NextElem(GetListOfElements());
   while (TDSetElement *elem = dynamic_cast<TDSetElement*>(NextElem())) {
      if (!elem->GetValid()) elem->Validate();
   }

}

//______________________________________________________________________________
void TDSet::Validate(TDSet* dset)
{
   // Validate the TDSet against another TDSet.
   // Only validates elements in common from input TDSet.

   THashList BestElements;
   BestElements.SetOwner();
   TList NamedHolder;
   NamedHolder.SetOwner();
   TIter NextOtherElem(dset->GetListOfElements());
   while (TDSetElement *elem = dynamic_cast<TDSetElement*>(NextOtherElem())) {
      if (!elem->GetValid()) continue;
      TString dir_file_obj = elem->GetDirectory();
      dir_file_obj += "_";
      dir_file_obj += elem->GetFileName();
      dir_file_obj += "_";
      dir_file_obj += elem->GetObjName();
      TPair *p = dynamic_cast<TPair*>(BestElements.FindObject(dir_file_obj));
      if (p) {
         TDSetElement *prevelem = dynamic_cast<TDSetElement*>(p->Value());
         Long64_t entries = prevelem->GetFirst()+prevelem->GetNum();
         if (entries<elem->GetFirst()+elem->GetNum()) {
            BestElements.Remove(p);
            BestElements.Add(new TPair(p->Key(), elem));
            delete p;
         }
      } else {
         TNamed* named = new TNamed(dir_file_obj, dir_file_obj);
         NamedHolder.Add(named);
         BestElements.Add(new TPair(named, elem));
      }
   }

   TIter NextElem(GetListOfElements());
   while (TDSetElement *elem = dynamic_cast<TDSetElement*>(NextElem())) {
      if (!elem->GetValid()) {
         TString dir_file_obj = elem->GetDirectory();
         dir_file_obj += "_";
         dir_file_obj += elem->GetFileName();
         dir_file_obj += "_";
         dir_file_obj += elem->GetObjName();
         if (TPair *p = dynamic_cast<TPair*>(BestElements.FindObject(dir_file_obj))) {
            TDSetElement* validelem = dynamic_cast<TDSetElement*>(p->Value());
            elem->Validate(validelem);
         }
      }
   }
}
