// @(#)root/tree:$Name:  $:$Id: TDSet.h,v 1.11 2005/03/10 17:57:04 rdm Exp $
// Author: Fons Rademakers   11/01/02

/*************************************************************************
 * Copyright (C) 1995-2001, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TDSet
#define ROOT_TDSet


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

#ifndef ROOT_TNamed
#include "TNamed.h"
#endif

#ifndef ROOT_TEventList
#include "TEventList.h"
#endif

class TList;
class TDSet;
class TEventList;
class TCut;
class TTree;
class TChain;
class TVirtualProof;
class TEventList;


class TDSetElementPfn : public TObject {

private:
   TString    fPfn;    // Physical File Name
   TString    fMsn;    // Mass Storage Name
   TString    fCen;    // Computing Element Name
   Long64_t   fSize;   // Size in bytes

public:
   TDSetElementPfn(const char *pfn = 0, const char *msn = 0, Long64_t size = -1)
   {
      fPfn  = pfn; fMsn  = msn; fSize = size;
   }

   virtual ~TDSetElementPfn() { }

   const char *GetPfn() const { return fPfn; }
   const char *GetMsn() const { return fMsn; }
   const char *GetCen() const { return fCen; }
   void        SetCen(const char *cen) { fCen = cen; }
   void        SetCen(const TString& cen) { fCen = cen; }
   Long64_t    GetSize() const { return fSize; }
   void        Print(Option_t *option = "") const;

   ClassDef(TDSetElementPfn,1)  // Describing physical locations of LFNs
};


class TDSetElementMsn : public TObject {

private:
   TString       fMsn;                   // Mass Storage Name
   Int_t         fNfiles;                // Number of files
   Long64_t      fDataSize;              // Size of file on mass storage
   Int_t         fNSiteDaemons;          // Number of daemons
   Int_t         fMaxSiteDaemons;        // Max number of daemons
   Long64_t      fDataPerSiteDaemon;
   Long64_t      fMaxDataPerSiteDaemon;

public:
   TDSetElementMsn() { }
   TDSetElementMsn(TDSetElementPfn *dse);
   virtual ~TDSetElementMsn() { }

   const char *GetMsn() const { return fMsn; }
   Int_t       GetNfiles() const { return fNfiles; }
   Long64_t    GetDataSize() const { return fDataSize; }
   const char *GetName() const { return fMsn; }
   Int_t       GetNSiteDaemons() const { return fNSiteDaemons; }
   void        SetNSiteDaemons(Int_t ndaemons) { fNSiteDaemons = ndaemons; }
   Int_t       GetMaxSiteDaemons() const { return fMaxSiteDaemons; }
   Long64_t    GetDataPerSiteDaemon() const { return fDataPerSiteDaemon; }
   Long64_t    GetMaxDataPerSiteDaemon() const { return fMaxDataPerSiteDaemon; }
   void        SetMaxDataPerSiteDaemon(Long64_t maxdata) { fMaxDataPerSiteDaemon = maxdata; }
   void        SetMaxSiteDaemons(Int_t maxdaemons) { fMaxSiteDaemons = maxdaemons; }
   Int_t       Increment() { if (fNfiles == -1) fNfiles = 1; else fNfiles++; return fNfiles; }
   Long64_t    AddData(Long64_t datasize) { fDataSize += datasize; return fDataSize; }
   void        Print(Option_t *option ="") const;

   ClassDef(TDSetElementMsn,1)  // Describing the files to be processed in a mass storage system
};


class TDSetElement : public TObject {

private:
   TString          fFileName;   // physical or logical file name
   TString          fObjName;    // name of objects to be analyzed in this file
   TString          fDirectory;  // directory in file where to look for objects
   Long64_t         fFirst;      // first entry to process
   Long64_t         fNum;        // number of entries to process
   const TDSet     *fSet;        // set to which element belongs
   TString          fMsd;        // mass storage domain name
   TList           *fPfnList;    // physical location information for Grid files
   TIter           *fIterator;   //! iterator on fPfnList
   TDSetElementPfn *fCurrent;    //! current element of fPfnList
   Long64_t         fTDSetOffset;//! offset in the whole TDSet of the first
                                 //  entry in this element
   TEventList      *fEventList;  // event list to be used in processing
   Bool_t           fValid;      // whether or not the input values are valid
   Long64_t         fEntries;    // total number of possible entries in file

public:
   TDSetElement() { fSet = 0; fPfnList = 0; fIterator = 0; fCurrent = 0; fValid = kFALSE; fEventList = 0;}
   TDSetElement(const TDSet *set, const char *file, const char *objname = 0,
                const char *dir = 0, Long64_t first = 0, Long64_t num = -1,
                const char *msd = 0);
   virtual ~TDSetElement();

   void AddPfn(const char *pfn, const char *se = 0, Long64_t size = -1);

   const char      *GetFileName() const { return fFileName; }
   TDSetElementPfn *GetFirstPfnElement() const { return fPfnList ? (TDSetElementPfn*)fPfnList->First() : 0; }
   Long64_t         GetFirst() const { return fFirst; }
   void             SetFirst(Long64_t first) { fFirst = first; }
   Long64_t         GetNum() const { return fNum; }
   const char      *GetMsd() const { return fMsd; }
   void             SetNum(Long64_t num) { fNum = num; }
   Bool_t           GetValid() const { return fValid; }
   Int_t            GetNumPfnList() const { return fPfnList ? fPfnList->GetSize() : 0; }
   const char      *GetObjName() const;
   const char      *GetDirectory() const;
   void             Reset();
   TDSetElementPfn *Next();
   void             Print(Option_t *options="") const;
   Long64_t         GetTDSetOffset() const { return fTDSetOffset; }
   void             SetTDSetOffset(Long64_t offset) { fTDSetOffset = offset; }
   TEventList      *GetEventList() const { return fEventList; }
   void             SetEventList(TEventList *aList) { fEventList = aList; }
   void             Validate();
   void             Validate(TDSetElement *elem);
   Int_t            Compare(const TObject *obj) const;
   Bool_t           IsSortable() const { return kTRUE; }

   ClassDef(TDSetElement,2)  // A TDSet element
};


class TDSet : public TNamed {

private:
   TString  fObjName;     // name of objects to be analyzed (e.g. TTree name)
   TList   *fElements;    //-> list of TDSetElements
   Bool_t   fIsTree;      // true if type is a TTree (or TTree derived)
   TIter   *fIterator;    //! iterator on fElements
   TList   *fElementsMsn; //-> list of mass storage names and the located files
   TEventList *fEventList; //! event list for processing
protected:
   TDSetElement  *fCurrent;  //! current element

   Int_t GridAdd(const char *file, const char *objname = 0,
                 const char *dir = 0, Long64_t first = 0,
                 Long64_t num = -1);
   void GridAddElementMsn(TDSetElementPfn *dsepfn);

public:
   TDSet();
   TDSet(const char *type, const char *objname = "*", const char *dir = "/");
   virtual ~TDSet();

   virtual Bool_t        Add(const char *file, const char *objname = 0,
                             const char *dir = 0, Long64_t first = 0,
                             Long64_t num = -1, const char *msd = 0);
   virtual Bool_t        Add(TDSet *set);
   virtual void          AddFriend(TDSet *friendset);
   virtual Bool_t        AddQuery(const char *path, const char *file,
                                  const char *conditions = 0);

   virtual Bool_t        Request();
   virtual Bool_t        Connect();

   virtual Int_t         Process(const char *selector, Option_t *option = "",
                                 Long64_t nentries = -1,
                                 Long64_t firstentry = 0,
                                 TEventList *evl = 0); // *MENU*
   virtual Int_t         Draw(const char *varexp, const char *selection,
                              Option_t *option = "", Long64_t nentries = -1,
                              Long64_t firstentry = 0); // *MENU*
   virtual Int_t         Draw(const char *varexp, const TCut &selection,
                              Option_t *option = "", Long64_t nentries = -1,
                              Long64_t firstentry = 0); // *MENU*
   virtual void          Draw(Option_t *opt) { Draw(opt, "", "", 1000000000, 0); }

   void                  Print(Option_t *option="") const;

   void                  SetObjName(const char *objname);
   void                  SetDirectory(const char *dir);

   Bool_t                IsTree() const { return fIsTree; }
   Bool_t                IsValid() const { return !fName.IsNull(); }
   Bool_t                ElementsValid() const;
   const char           *GetType() const { return fName; }
   const char           *GetObjName() const { return fObjName; }
   const char           *GetDirectory() const { return fTitle; }
   TList                *GetListOfElements() const { return fElements; }
   TList                *GetListOfElementsMsn() const { return fElementsMsn; }

   virtual void          GridPack();
   virtual void          GridPrintPackList();

   virtual void          Reset();
   virtual TDSetElement *Next();
   TDSetElement         *Current() const { return fCurrent; };

   static Long64_t       GetEntries(Bool_t isTree, const char *filename,
                                    const char *path, const char *objname);

   void                  AddInput(TObject *obj);
   void                  ClearInput();
   TObject              *GetOutput(const char *name);
   TList                *GetOutputList();
   virtual void          StartViewer(); // *MENU*

   virtual TTree        *GetTreeHeader(TVirtualProof *proof);
   static TDSet         *MakeTDSet(TChain *chain);
   virtual void          SetEventList(TEventList* evl) { fEventList = evl;}
   TEventList           *GetEventList() const {return fEventList; }
   void                  Validate();
   void                  Validate(TDSet *dset);

   ClassDef(TDSet,1)  // Data set for remote processing (PROOF)
};

#endif
