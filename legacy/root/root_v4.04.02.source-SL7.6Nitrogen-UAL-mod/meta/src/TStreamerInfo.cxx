// @(#)root/meta:$Name:  $:$Id: TStreamerInfo.cxx,v 1.223 2005/04/18 19:14:56 brun Exp $
// Author: Rene Brun   12/10/2000

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/


//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TROOT.h"
#include "TStreamerInfo.h"
#include "TStreamerElement.h"
#include "TClass.h"
#include "TClassEdit.h"
#include "TDataMember.h"
#include "TMethodCall.h"
#include "TDataType.h"
#include "TRealData.h"
#include "TBaseClass.h"
#include "TBuffer.h"
#include "TArrayC.h"
#include "TArrayI.h"
#include "TArrayF.h"
#include "TArrayD.h"
#include "TArrayS.h"
#include "TArrayL.h"
#include "TError.h"
#include "TRef.h"
#include "TProcessID.h"
#include "TVirtualCollectionProxy.h"
#include "TStreamer.h"
#include "TInterpreter.h"
#include "TContainerConverters.h"

Int_t   TStreamerInfo::fgCount = 0;
Bool_t  TStreamerInfo::fgCanDelete        = kTRUE;
Bool_t  TStreamerInfo::fgOptimize         = kTRUE;
Bool_t  TStreamerInfo::fgStreamMemberWise = kFALSE;
TStreamerElement *TStreamerInfo::fgElement = 0;

const Int_t kRegrouped = TStreamerInfo::kOffsetL;

const Int_t kMaxLen = 1024;

ClassImp(TStreamerInfo)

//______________________________________________________________________________
TStreamerInfo::TStreamerInfo()
{
   // Default ctor.

   fNumber   = fgCount;
   fClass    = 0;
   fElements = 0;
   fComp     = 0;
   fType     = 0;
   fNewType  = 0;
   fOffset   = 0;
   fLength   = 0;
   fElem     = 0;
   fMethod   = 0;
   fCheckSum = 0;
   fNdata    = 0;
   fSize     = 0;
   fClassVersion = 0;
   fOptimized = kFALSE;
   fOldVersion = Class()->GetClassVersion();
   fIsBuilt  = kFALSE;
}

//______________________________________________________________________________
TStreamerInfo::TStreamerInfo(TClass *cl, const char *info)
   : TNamed(cl->GetName(),info)
{
   // Create a TStreamerInfo object.

   fgCount++;
   fNumber   = fgCount;
   fClass    = cl;
   fElements = new TObjArray();
   fComp     = 0;
   fType     = 0;
   fNewType  = 0;
   fOffset   = 0;
   fLength   = 0;
   fElem     = 0;
   fMethod   = 0;
   fCheckSum = 0;
   fNdata    = 0;
   fSize     = 0;
   fOptimized = kFALSE;
   fIsBuilt  = kFALSE;
   fClassVersion = fClass->GetClassVersion();
   fOldVersion = Class()->GetClassVersion();

   if (info) BuildUserInfo(info);

}

//______________________________________________________________________________
TStreamerInfo::~TStreamerInfo()
{
   // TStreamerInfo dtor.

   delete [] fType;    fType   =0;
   delete [] fNewType; fNewType=0;
   delete [] fOffset;  fOffset =0;
   delete [] fLength;  fLength =0;
   delete [] fElem;    fElem   =0;
   delete [] fMethod;  fMethod =0;
   delete [] fComp;    fComp   =0;

   if (!fElements) return;
   fElements->Delete();
   delete fElements; fElements=0;
}

//______________________________________________________________________________
void TStreamerInfo::Build()
{
   // Build the I/O data structure for the current class version
   // A list of TStreamerElement derived classes is built by scanning
   // one by one the list of data members of the analyzed class.

   // This is used to avoid unwanted recursive call to Build
   fIsBuilt = kTRUE;

   if (fClass->GetCollectionProxy()) {
      TStreamerElement *element = new TStreamerSTL("This","Used to call the proper TStreamerInfo case",0,fClass->GetName(),fClass->GetName(),0);
      fElements->Add(element);
      Compile();
      return;
   }

   TStreamerElement::Class()->IgnoreTObjectStreamer();
   //if (!strcmp(fClass->GetName(),"TVector3"))       fClass->IgnoreTObjectStreamer();

   fClass->BuildRealData();

   fCheckSum = fClass->GetCheckSum();
   Int_t i, ndim, offset;
   TClass *clm;
   TDataType *dt;
   TDataMember *dm;
   TBaseClass *base;
   TStreamerElement *element;
   TIter nextb(fClass->GetListOfBases());
   TMemberStreamer *streamer = 0;

   //iterate on list of base classes
   while((base = (TBaseClass*)nextb())) {
      element  = 0;
      offset   = base->GetDelta();
      if (offset == kMissing) continue;
      streamer = 0; // base->GetStreamer();
      do { //do block in while

         // this case appears with STL collections as base class.
         if (strcmp(base->GetName(),"string") == 0) {
            element = new TStreamerSTLstring(base->GetName(),base->GetTitle(),offset,base->GetName(),kFALSE);
            continue;
         }
         if (base->IsSTLContainer()) {
            element = new TStreamerSTL(base->GetName(),base->GetTitle(),offset,base->GetName(),0,kFALSE);
//             if (!streamer)  element->SetType(-1);
            continue;
         }
         clm = gROOT->GetClass(base->GetName());
         if (!clm) {
            Error("Build","%s, unknown type: %s %s\n",GetName(),base->GetName(),base->GetTitle());
            continue;
         }
         clm->GetStreamerInfo();
         offset = fClass->GetBaseClassOffset(clm);
         element = new TStreamerBase(base->GetName(),base->GetTitle(),offset);
         if (clm == TObject::Class() && fClass->CanIgnoreTObjectStreamer()) {
            SetBit(TClass::kIgnoreTObjectStreamer);
            element->SetType(-1);
         }
         if (!clm->IsLoaded()) {
            Warning("Build:","%s: base class %s has no streamer or dictionary it will not be saved",
                    GetName(), clm->GetName());
         }
      } while(0); {
         // continue block in while
      
         if (!element) continue;
         fElements->Add(element);
//          element->SetStreamer(streamer);
      }//end continue block
   }//end base class loop


   //iterate on list of data members
   TIter nextd(fClass->GetListOfDataMembers());

   Int_t dsize,dtype;
   while((dm=(TDataMember*)nextd()))  {
      if (fClass->GetClassVersion() == 0) continue;
      if (!dm->IsPersistent())            continue;
      streamer = 0;
      offset = GetDataMemberOffset(dm,streamer);
      if (offset == kMissing)             continue;
//       streamer = dm->GetStreamer();
      element = 0;
      dsize   = 0;

      do { //do block in while

         //look for a data member with a counter in the comment string [n]
         TRealData *refcount = 0;
         TDataMember *dmref = 0;
         if (dm->IsaPointer()) {
            const char *title = (char*)dm->GetTitle();
            const char *lbracket = strchr(title,'[');
            const char *rbracket = strchr(title,']');
            if (lbracket && rbracket) {
               refcount = (TRealData*)fClass->GetListOfRealData()->FindObject(dm->GetArrayIndex());
               if (!refcount) {
                  Error("Build","%s, discarding: %s %s, illegal %s\n",GetName(),dm->GetFullTypeName(),dm->GetName(),title);
                  continue;
               }
               dmref = refcount->GetDataMember();
               TDataType *reftype = dmref->GetDataType();
               Bool_t isInteger = reftype->GetType() == 3 || reftype->GetType() == 13;
               if (!reftype || !isInteger) {
                  Error("Build","%s, discarding: %s %s, illegal [%s] (must be Int_t)\n",GetName(),dm->GetFullTypeName(),dm->GetName(),dm->GetArrayIndex());
                  continue;
               }
               TStreamerBasicType *bt = TStreamerInfo::GetElementCounter(dm->GetArrayIndex(),dmref->GetClass());
               if (!bt) {
                  if (dmref->GetClass()->Property() & kIsAbstract) continue;
                  Error("Build","%s, discarding: %s %s, illegal [%s] must be placed before \n",GetName(),dm->GetFullTypeName(),dm->GetName(),dm->GetArrayIndex());
                  continue;
               }
            }
         }

         dt=dm->GetDataType();

         if (dt) {  // found a basic type
            dtype = dt->GetType();
            dsize = dt->Size();
            if (!refcount && (strstr(dm->GetFullTypeName(),"char*")
                              || strstr(dm->GetFullTypeName(),"Char_t*"))) {
               dtype = kCharStar;
               dsize = sizeof(char*);
            }
            if (dm->IsaPointer() && dtype != kCharStar) {
               if (refcount) {
                  // data member is pointer to an array of basic types
                  element = new TStreamerBasicPointer(dm->GetName(),dm->GetTitle(),offset,dtype,
                                                      dm->GetArrayIndex(),
                                                      dmref->GetClass()->GetName(),
                                                      dmref->GetClass()->GetClassVersion(),
                                                      dm->GetFullTypeName());
                  continue;
               } else {
                  if (fName == "TString" || fName == "TClass") continue;
                  Error("Build","%s, discarding: %s %s, no [dimension]\n",GetName(),dm->GetFullTypeName(),dm->GetName());
                  continue;
               }
            }
            // data member is a basic type
            if (fClass == TObject::Class() && !strcmp(dm->GetName(),"fBits")) {
               //printf("found fBits, changing dtype from %d to 15\n",dtype);
               dtype = kBits;
            }
            element = new TStreamerBasicType(dm->GetName(),dm->GetTitle(),offset,dtype,dm->GetFullTypeName());
            continue;

         }  else {
            // try STL container or string
            static const char *full_string_name = "basic_string<char,char_traits<char>,allocator<char> >";
            if (strcmp(dm->GetTypeName(),"string") == 0
                ||strcmp(dm->GetTypeName(),full_string_name)==0 ) {
               element = new TStreamerSTLstring(dm->GetName(),dm->GetTitle(),offset,dm->GetFullTypeName(),dm->IsaPointer());
               continue;
            }
            if (dm->IsSTLContainer()) {
               element = new TStreamerSTL(dm->GetName(),dm->GetTitle(),offset,dm->GetFullTypeName(),dm->GetTrueTypeName(),dm->IsaPointer());
               continue;
            }
            clm = gROOT->GetClass(dm->GetTypeName());
            if (!clm) {
               Error("Build","%s, unknow type: %s %s\n",GetName(),dm->GetFullTypeName(),dm->GetName());
               continue;
            }
            // a pointer to a class
            if (dm->IsaPointer()) {
               if(refcount) {
                  element = new TStreamerLoop(dm->GetName(),
                                              dm->GetTitle(),offset,
                                              dm->GetArrayIndex(),
                                              dmref->GetClass()->GetName(),
                                              dmref->GetClass()->GetClassVersion(),
                                              dm->GetFullTypeName());
                  continue;
               } else {
                  if (clm->InheritsFrom(TObject::Class())) {
                     element = new TStreamerObjectPointer(dm->GetName(),dm->GetTitle(),offset,dm->GetFullTypeName());
                     continue;
                  } else {
                     element = new TStreamerObjectAnyPointer(dm->GetName(),dm->GetTitle(),offset,dm->GetFullTypeName());
                     if (!streamer && !clm->GetStreamer() && !clm->IsLoaded()) {
                        Error("Build:","%s: %s has no streamer or dictionary, data member %s will not be saved",
                              GetName(), dm->GetFullTypeName(),dm->GetName());
                     }
                     continue;
                  }
               }
            }
            // a class
            if (clm->InheritsFrom(TObject::Class())) {
               element = new TStreamerObject(dm->GetName(),dm->GetTitle(),offset,dm->GetFullTypeName());
               continue;
            } else if(clm == TString::Class() && !dm->IsaPointer()) {
               element = new TStreamerString(dm->GetName(),dm->GetTitle(),offset);
               continue;
            } else {
               element = new TStreamerObjectAny(dm->GetName(),dm->GetTitle(),offset,dm->GetFullTypeName());
               if (!streamer && !clm->GetStreamer() && !clm->IsLoaded()) {
                  Warning("Build:","%s: %s has no streamer or dictionary, data member \"%s\" will not be saved",
                          GetName(), dm->GetFullTypeName(),dm->GetName());
               }
               continue;
            }
         }
      } while(0); {//continue block

         if(!element)  continue;
         ndim = dm->GetArrayDim();
         if (!dsize) dsize = dm->GetUnitSize();
         for (i=0;i<ndim;i++) element->SetMaxIndex(i,dm->GetMaxIndex(i));
         element->SetArrayDim(ndim);
         int narr = element->GetArrayLength(); if (!narr) narr = 1;
         element->SetSize(dsize*narr);
         element->SetStreamer(streamer);
         fElements->Add(element);
         if (streamer) continue;
         int k = element->GetType();
//          if (k!=kSTL && k!=kSTL+kOffsetL && k!=kStreamer && k!=kStreamLoop) continue;
         if (k!=kStreamer && k!=kStreamLoop) continue;
         element->SetType(-1);
      

      }//end continue block
   }//end member loop

   Compile();

}


//______________________________________________________________________________
void TStreamerInfo::BuildCheck()
{
   // check if the TStreamerInfo structure is already created
   // called by TFile::ReadStreamerInfo

   fClass = gROOT->GetClass(GetName());
   TObjArray *array;
   if (fClass) {

      if (TClassEdit::IsSTLCont(fClass->GetName())) return;

      array = fClass->GetStreamerInfos();
      TStreamerInfo *info = 0;

      // If we have a foreign class, we need to search for a StreamerInfo 
      // with same checksum.
      Bool_t searchOnChecksum = kFALSE;
      if( !fClass->IsLoaded() ) {
         // When the class is not loaded 
         // the result of IsForeign is not what we are looking 
         // for (Technically IsForeign means IsLoaded and do not 
         // have a Streamer method).
         
         // A foreign class would have the ClassVersion equal to 1
         // Also we only care if a StreamerInfo has already been loaded
         if ( fClassVersion==1 && array->At(1) != 0 ) {
            if (fCheckSum != ((TStreamerInfo*)array->At(1))->GetCheckSum()) {
               searchOnChecksum = kTRUE;
               fClassVersion = array->GetLast()+1;
            }
         }
      } else if (fClass->IsForeign()) {
         searchOnChecksum = kTRUE;
      }

      if (searchOnChecksum) {
         Int_t ninfos = array->GetEntriesFast();
         for (Int_t i=1;i<ninfos;i++) {
            info = (TStreamerInfo*)array->At(i);
            if (!info) continue;
            if (fCheckSum == info->GetCheckSum()) {
               fClassVersion = i;
               //printf("found class with checksum, version=%d\n",i);
               break;
            } else {
               info = 0;
            }
         }
      } else {
         info = (TStreamerInfo *)array->At(fClassVersion);
      }
      // NOTE: Should we check if the already existing info is the same as
      // the current one? Yes
      // In case a class (eg Event.h) has a TClonesArray of Tracks, it could be
      // that the old info does not have the class name (Track) in the data
      // member title. Set old title to new title
      if (info) {

         if (info->IsBuilt()) {
            SetBit(kCanDelete);
            fNumber = info->GetNumber();
            Int_t nel = fElements->GetEntriesFast();
            TObjArray *elems = info->GetElements();
            TStreamerElement *e1, *e2;
            for (Int_t i=0;i<nel;i++) {
               e1 = (TStreamerElement *)fElements->At(i);
               e2 = (TStreamerElement *)elems->At(i);
               if (!e1 || !e2) continue;
               if (strlen(e1->GetTitle()) != strlen(e2->GetTitle())) {
                  e2->SetTitle(e1->GetTitle());
               }
            }
            return;
         } else {
            array->RemoveAt(fClassVersion);
            delete info; info = 0;
         }
      }
      if (fClass->GetListOfDataMembers()
          && (fClassVersion == fClass->GetClassVersion())
          && (fCheckSum != fClass->GetCheckSum())
          && (fClass->GetClassInfo()) ) {
         //give a last chance. Due to a new CINT behaviour with enums
         //verify the checksum ignoring members of type enum
         if (fCheckSum != fClass->GetCheckSum(1)) {
            if (fClass->IsForeign()) {
               fClassVersion = fClass->GetClassVersion() + 1;
               //printf("setting fClassVersion=%d\n",fClassVersion);
            } else {
               int warn = 1;
               if (fOldVersion<=2) {
                  // Names of STL base classes was modified in vers==3. Allocators removed
                  //              
                  TIter nextBC(fClass->GetListOfBases());
                  TBaseClass *bc;
                  while ((bc=(TBaseClass*)nextBC())) 
                  {if (TClassEdit::IsSTLCont(bc->GetName())) warn = 0;}
               }

               if (warn) Warning("BuildCheck","\n\
        The StreamerInfo of class %s read from file %s\n\
        has the same version (=%d) as the active class but a different checksum.\n\
        You should update the version to ClassDef(%s,%d).\n\
        Do not try to write objects with the current class definition,\n\
        the files will not be readable.\n",GetName(),gDirectory->GetFile()->GetName()
                              ,fClassVersion,GetName(),fClassVersion+1);
            }
         }
      } else {
         if (info) {
            Error("BuildCheck","Wrong class info");
            SetBit(kCanDelete);
            return;
         }
      }
   } else {
      fClass = new TClass(GetName(),fClassVersion, 0, 0, -1, -1 );
      fClass->SetBit(TClass::kIsEmulation);
      array = fClass->GetStreamerInfos();
   }
   if (TestBit(TClass::kIgnoreTObjectStreamer)) fClass->IgnoreTObjectStreamer();
   if (fClassVersion < 0 || fClassVersion > 65000) {
      printf("ERROR reading TStreamerInfo: %s fClassVersion=%d\n",GetName(),fClassVersion);
      SetBit(kCanDelete);
      fNumber = -1;
      return;
   }
   array->AddAtAndExpand(this,fClassVersion);
   fgCount++;
   fNumber = fgCount;

   // Since we just read this streamerInfo from file, it has already been built.
   fIsBuilt = kTRUE;  

   //add to the global list of StreamerInfo
   TObjArray *infos = (TObjArray*)gROOT->GetListOfStreamerInfo();
   infos->AddAtAndExpand(this,fNumber);
}

//______________________________________________________________________________
void TStreamerInfo::BuildEmulated(TFile *file)
{
   // Create an Emulation TStreamerInfo object.
   char duName[100];
   Assert(file);
   Int_t fv = file->GetVersion()%100000;
   Assert(fv < 30000);
   fClassVersion = -1;
   fCheckSum = 2001;
   TObjArray *elements = GetElements();
   if (!elements) return;
   Int_t ndata = elements->GetEntries();
   if (ndata == 0) return;
   TStreamerElement *element;
   Int_t i;
   for (i=0;i<ndata;i++) {
      element = (TStreamerElement*)elements->At(i);
      if (!element) break;
      int ty = element->GetType();
      if (ty < kChar || ty >kULong+kOffsetL)    continue;
      if (ty == kLong )                         element->SetType(kInt          );
      if (ty == kULong)                         element->SetType(kUInt         );
      if (ty == kLong +kOffsetL)                element->SetType(kInt +kOffsetL);
      if (ty == kULong+kOffsetL)                element->SetType(kUInt+kOffsetL);
      if (ty <= kULong)                         continue;
      strcpy(duName,element->GetName());
      strcat(duName,"QWERTY");
      TStreamerBasicType *bt = new TStreamerBasicType(duName, "", 0, kInt,"Int_t");
      {for (int j=ndata-1;j>=i;j--) {elements->AddAtAndExpand(elements->At(j),j+1);}}
      elements->AddAt(bt,i);
      ndata++;
      i++;
   }
   BuildOld();
}

//______________________________________________________________________________
// Helper function for BuildOld
namespace {
   Bool_t ClassWasMovedToNamespace(TClass *oldClass, TClass *newClass) 
   {
      // Returns true if oldClass is the same as newClass but newClass is in a 
      // namespace (and oldClass was not in a namespace).

      if (oldClass == 0 || newClass == 0) return kFALSE;

      UInt_t newlen = strlen(newClass->GetName());
      UInt_t oldlen = strlen(oldClass->GetName());

      const char *oldname = oldClass->GetName();
      for(UInt_t i = oldlen, done = false, nest = 0; (i>0) && !done ; --i) {
         switch ( oldClass->GetName()[i-1] ) {
            case '>' : ++nest; break;
            case '<' : --nest; break;
            case ':' : if (nest==0) oldname= &(oldClass->GetName()[i]); done = kTRUE; break;
         }
      }
      oldlen = strlen(oldname);
      if ( ! (strlen(newClass->GetName()) > strlen(oldClass->GetName()) ) ) return kFALSE;

      const char* newEnd = & ( newClass->GetName()[ newlen - oldlen ] );

      if (0 != strcmp( newEnd, oldname ) ) return kFALSE;
 
      Int_t oldv = oldClass->GetStreamerInfo()->GetClassVersion();

      if (   newClass->GetStreamerInfos()
          && oldv < newClass->GetStreamerInfos()->GetSize() 
          && newClass->GetStreamerInfos()->At(oldv)
          && strcmp( newClass->GetStreamerInfos()->At(oldv)->GetName(),
                     oldClass->GetName() ) != 0 ) {
         // The new class has already a TStreamerInfo for the the same version as
         // the old class and this was not the result of an import.  So we do not 
         // have a match
         return kFALSE;
      }
      return kTRUE;
   }

   Int_t ImportStreamerInfo(TClass *oldClass, TClass *newClass) {
      // Import the streamerInfo from oldClass to newClass
      // In case of conflict, returns the version number of the StreamerInfo
      // with the conflict.
      // Return 0 in case of success

      TIter next(oldClass->GetStreamerInfos());
      TStreamerInfo *info;
      while ((info = (TStreamerInfo*)next())) {
         info = (TStreamerInfo*)info->Clone();
         info->SetClass(newClass);
         Int_t oldv = info->GetClassVersion();
         if (   oldv > newClass->GetStreamerInfos()->GetSize() 
             || newClass->GetStreamerInfos()->At(oldv)==0 ) {
            // All is good.
            newClass->GetStreamerInfos()->AddAtAndExpand(info,oldv);
         } else {
            // We verify that we are consitent and that 
            //   newcl->GetStreamerInfos()->UncheckedAt(info->GetClassVersion)
            // is already the same as info.
            if ( strcmp( newClass->GetStreamerInfos()->At(oldv)->GetName(),
                         oldClass->GetName() ) != 0 ) {
               // The existing StreamerInfo does not already come from OldClass.
               // This is a real problem!
               return oldv;
            }
         }
      }
      return 0;
   }
   
   Bool_t ContainerMatchTClonesArray(TClass *newClass) 
   {
      // Return true if newClass is a likely valid conversion from
      // a TClonesArray

      return newClass->GetCollectionProxy()
             && newClass->GetCollectionProxy()->GetValueClass() 
             && !newClass->GetCollectionProxy()->HasPointers();
   }

   Bool_t CollectionMatch(const TClass *oldClass, const TClass* newClass)
   {
      // Return true if oldClass and newClass points to 2 compatible collection.
      // i.e. they contains the exact same type.

      TVirtualCollectionProxy *oldProxy = oldClass->GetCollectionProxy();
      TVirtualCollectionProxy *newProxy = newClass->GetCollectionProxy();

      if (oldProxy->GetValueClass() == newProxy->GetValueClass()) {
         if (  (oldProxy->GetValueClass() ==0 && oldProxy->GetType() == newProxy->GetType()) 
             ||(oldProxy->GetValueClass() && oldProxy->HasPointers() == newProxy->HasPointers()) ) {
            // We have compatibles collections (they have the same content!
            return kTRUE;
         }
      }
      return kFALSE;
   }
}

//______________________________________________________________________________
void TStreamerInfo::BuildOld()
{
   // rebuild the TStreamerInfo structure

   if (gDebug > 0) printf("\n====>Rebuilding TStreamerInfo for class: %s, version: %d\n",GetName(),fClassVersion);

   // This is used to avoid unwanted recursive call to Build
   fIsBuilt = kTRUE;

   if (fClass->GetClassVersion()==fClassVersion) fClass->BuildRealData();
   else {
      // This is to support the following case
      //  Shared library: Event v2
      //  calling cl->GetStreamerInfo(1)->BuildOld();  (or equivalent)
      //  which calls cl->BuildReadData()
      //  which set fRealData to some value
      //  then call Event()
      //  which call cl->GetStreamerInfo()
      //  which call cl->BuildRealData();
      //  which returns immediately (upon seeing fRealData!=0)
      //  then the main StreamerInfo build using the partial content of fRealData
      //  then BuildRealData returns
      //  then GetStreamerInfo() returns
      //  then Event() returns 
      //  then fRealData is finished being populated
      //  then this function continue,
      //  then it uses the main streamerInfo 
      //  .... which is incomplete.
      //
      //  Instead we force the creation of the main streamerInfo object 
      //  before the creation of fRealData.
      fClass->GetStreamerInfo();
   }

   if (fClass->GetCollectionProxy() && 
      fElements->GetEntries()==1 &&
      strcmp(fElements->At(0)->GetName(),"This")==0) {
   }
   TIter next(fElements);
   TStreamerElement *element;
   Int_t offset = 0;
   TMemberStreamer *streamer = 0;
   Int_t sp = sizeof(void *);
#if defined(R__SGI64)
   sp = 8;
#endif
   int nBaze=0;
   if (fClass->GetCollectionProxy() && 
      fElements->GetEntries()==1 &&
      strcmp(fElements->At(0)->GetName(),"This")==0) {
      next();
   }
   while ((element = (TStreamerElement*)next())) {
      element->SetNewType(element->GetType());
      element->Init();

      if (element->IsBase()) {

         if (element->IsA()==TStreamerBase::Class()) {

            TStreamerBase *base = (TStreamerBase*)element;
            TClass *baseclass = base->GetClassPointer();
            if (!baseclass) {
               Warning("BuildOld","Missing base class: %s skipped",base->GetName());
               baseclass = new TClass(element->GetName(),1,0,0,-1,-1);
               element->Update(0,baseclass);
            }
            baseclass->BuildRealData();
            Int_t version = base->GetBaseVersion();
            TStreamerInfo *infobase = baseclass->GetStreamerInfo(version);
            if (infobase->GetTypes() == 0) infobase->BuildOld();
            Int_t baseOffset = fClass->GetBaseClassOffset(baseclass);
            if (baseOffset < 0) baseOffset = 0;
            element->SetOffset(baseOffset);
            offset += baseclass->Size();
            continue;
            
         } else {
         
            // Not a base elem but still base, string or STL as a base
            nBaze++;
            TBaseClass *bc  = 0;
            TList *listOfBases = fClass->GetListOfBases();
            if (listOfBases) {
               TIter nextBC(fClass->GetListOfBases());
               while ((bc=(TBaseClass*)nextBC())) {

                  if (strchr(bc->GetName(),'<')!=0) {
                     TString bcName( TClassEdit::ShortType(bc->GetName()         ,TClassEdit::kDropStlDefault).c_str() );
                     TString elName( TClassEdit::ShortType(element->GetTypeName(),TClassEdit::kDropStlDefault).c_str() );
                     if (bcName==elName) break;

                  }
               }

            }
            if (bc==0) {
               Error("BuildOld","Could not find STL base class: %s for %s\n",
                     element->GetName(),GetName());
               continue;
            }
            int baseOffset = bc->GetDelta();
            if (baseOffset==-1) {
               TClass *cb = element->GetClassPointer();
               if (!cb) { element->SetNewType(-1); continue;}
               baseOffset = fClass->GetBaseClassOffset(cb);
            }
            //  we know how to read but do we know where to read?
            if (baseOffset<0) { element->SetNewType(-1); continue;}
            element->SetOffset(baseOffset);  
            continue;
         }

      }

      TDataMember *dm = 0;

      // First set the offset and sizes.
      if (fClass->GetDeclFileLine() < 0) {

         // Note the initilization in this case are 
         // delayed until __after__ the schema evolution
         // section, just in case the info has changed.

         // We are in the emulated case
         streamer = 0;
         element->Init(fClass);

      } else {
         // The class is loaded.

         // First look for the data member in the current class
         dm = (TDataMember*)fClass->GetListOfDataMembers()->FindObject(element->GetName());
         if (dm && dm->IsPersistent()) {
            fClass->BuildRealData();
            streamer = 0;
            offset = GetDataMemberOffset(dm,streamer);
            element->SetOffset(offset);
            element->Init(fClass);
            element->SetStreamer(streamer);
            int narr = element->GetArrayLength(); if (!narr) narr=1;
            int dsize = dm->GetUnitSize();
            element->SetSize(dsize*narr);

         } else {
            // We did not find it, let's look for it in the base classes via TRealData
            TRealData *rd = fClass->GetRealData(element->GetName());
            if (rd && rd->GetDataMember() ) {
               element->SetOffset(rd->GetThisOffset());
               element->Init(fClass);
               dm = rd->GetDataMember();
               int narr = element->GetArrayLength(); if (!narr) narr=1;
               int dsize = dm->GetUnitSize();
               element->SetSize(dsize*narr);
            }
         }
      }

      // Now let's deal with Schema evolution
      Int_t newType = kNoType_t;
      TClass *newClass = 0;
      if (dm && dm->IsPersistent()) {
         if (dm->GetDataType()) {
            newType = dm->GetDataType()->GetType();
            Bool_t isPointer = dm->IsaPointer();
            if (newType == kChar && isPointer && !(element->GetArrayLength() > 1) ) {
              newType=kCharStar;
            } else if (isPointer) {
               newType += kOffsetP;
            } else if (element->GetArrayLength() > 1) {
               newType += kOffsetL;
            }
         }
         if (newType==0) newClass = gROOT->GetClass( dm->GetTypeName() );
      } else {
         // Either the class is not loaded or the data member is gone
         if (! fClass->IsLoaded() ) {
            TStreamerInfo *newInfo = (TStreamerInfo*)fClass->GetStreamerInfos()->At(fClass->GetClassVersion());
            if (newInfo && newInfo != this) {
               TStreamerElement *newElems = (TStreamerElement*)
                  newInfo->GetElements()->FindObject(element->GetName());
               newClass = newElems ?  newElems->GetClassPointer() : 0;
               if (newClass==0) {
                  newType = newElems ? newElems->GetType() : kNoType_t;
                  if (! (newType<kObject) ) newType = kNoType_t; // sanity check. 
               }
            } else {
               newClass = element->GetClassPointer();
               if (newClass==0) {
                  newType = element->GetType();
                  if (! (newType<kObject) ) newType = kNoType_t; // sanity check. 
               }
            }
         }
      }

      if (newType) {
         // Case of a numerical type
         if (element->GetType() != newType ) {
            element->SetNewType(newType);
            if (gDebug > 0) Warning("BuildOld","element: %s::%s %s has new type: %s/%d",
                                    GetName(),element->GetTypeName(),element->GetName(),
                                    dm->GetFullTypeName(),newType);
         }
      } else if (newClass) {
         // Sometime BuildOld is called again.
         // In that case we migth already have fix up the streamer element.
         // So we need to go back to the original information!
         TClass *oldClass = gROOT->GetClass(
            TClassEdit::ShortType(element->GetTypeName(),TClassEdit::kDropTrailStar).c_str());

         if (oldClass == newClass) {
            // Nothing to do :)

         } else if ( ClassWasMovedToNamespace(oldClass,newClass) ) {
            Int_t oldv;
            if (0 != (oldv=ImportStreamerInfo(oldClass,newClass))) {
                Warning("BuildOld","Can not properly load the TStreamerInfo from %s into %s due to a conflict for the class version %d",
                        oldClass->GetName(), newClass->GetName(), oldv);
            } else {
               element->SetTypeName(dm->GetFullTypeName());

               if (gDebug>0) Warning("BuildOld","element: %s::%s %s has new type %s",
                                     GetName(),element->GetTypeName(), element->GetName(),
                                     newClass->GetName());
            }

         } else if ( oldClass == TClonesArray::Class() ) {

            if ( ContainerMatchTClonesArray( newClass ) ) {

               Int_t elemType = element->GetType();
               Bool_t isPrealloc = elemType==kObjectp || elemType==kAnyp
                  || elemType==kObjectp+kOffsetL || elemType==kAnyp+kOffsetL;

               element->Update( oldClass, newClass );
               element->SetStreamer( 
                  new TConvertClonesArrayToProxy(newClass->GetCollectionProxy(),
                                                 element->IsaPointer(),
                                                 isPrealloc) );
               // When the type is kObject, the TObject::Streamer is used instead
               // of the TStreamerElement's streamer.  So let force the usage
               // of our streamer
               if (element->GetType()==kObject) {
                  element->SetNewType(kAny);
                  element->SetType(kAny);
               }
               if (gDebug>0) Warning("BuildOld","element: %s::%s %s has new type %s",
                                     GetName(),element->GetTypeName(), element->GetName(),
                                     newClass->GetName());
            } else {
               element->SetNewType(-2);
            }

         } else if ( oldClass->GetCollectionProxy() &&
                     newClass->GetCollectionProxy() ) {
            if ( CollectionMatch ( oldClass, newClass ) ) {

               element->Update( oldClass, newClass );
               // Is this needed ? : element->SetSTLtype(newelement->GetSTLtype()); 

               if (gDebug>0) Warning("BuildOld","element: %s::%s %s has new type %s",
                                     GetName(),element->GetTypeName(), element->GetName(),
                                     newClass->GetName());
            } else {
                element->SetNewType(-2);
            }
         } else {
            element->SetNewType(-2);
         }
      } else {
         element->SetNewType(-1);
         element->SetOffset(kMissing);
      }
      if (element->GetNewType()==-2) {
         Warning("BuildOld","Cannot convert %s::%s from type:%s to type:%s, skip element",
                 GetName(),element->GetName(),element->GetTypeName(),newClass->GetName());
      }

      if (fClass->GetDeclFileLine() < 0) {

         // Note the initilization in this case are 
         // delayed until __after__ the schema evolution
         // section, just in case the info has changed.

         Int_t asize = element->GetSize();
         //align the non-basic data types (required on alpha and IRIX!!)
         if (offset%sp != 0) offset = offset - offset%sp + sp;
         element->SetOffset(offset);
         offset += asize;
      }

   }

   // change order , move "bazes" to the end. Workaround old bug
   if (fOldVersion<=2 && nBaze) {
      SetBit(kRecovered);
      TObjArray &arr = *fElements;
      TObjArray  tai(nBaze);
      int narr = arr.GetLast()+1;
      int iel,jel=0,kel=0;
      for (iel=0;iel<narr;iel++) {
         element = (TStreamerElement*)arr[iel];
         if (element->IsBase() && element->IsA()!=TStreamerBase::Class()) tai[kel++] = element;
         else                                        arr[jel++] = element;
      }
      for (kel=0;jel<narr;) arr[jel++]=tai[kel++];
   }

   Compile();
}
//______________________________________________________________________________
void TStreamerInfo::BuildUserInfo(const char * /*info*/)
{
   // Build the I/O data structure for the current class version

#ifdef TOBEIMPLEMENTED

   Int_t nch = strlen(title);
   char *info = new char[nch+1];
   strcpy(info,title);
   char *pos = info;
   TDataType *dt;
   TDataMember *dm;
   TRealData *rdm, *rdm1;
   TIter nextrdm(rmembers);

   // search tokens separated by a semicolon
   //tokens can be of the following types
   // -baseclass
   // -class     membername
   // -basictype membername
   // -classpointer     membername
   // -basictypepointer membername
   while(1) {
      Bool_t isPointer = kFALSE;
      while (*pos == ' ') pos++;
      if (*pos == 0) break;
      char *colon = strchr(pos,';');
      char *col = colon;
      if (colon) {
         *col = 0; col--;
         while (*col == ' ') {*col = 0; col--;}
         if (pos > col) break;
         char *star = strchr(pos,'*');
         if (star) {*star = ' '; isPointer = kTRUE;}
         char *blank;
         while(1) {
            blank = strstr(pos,"  ");
            if (blank == 0) break;
            strcpy(blank,blank+1);
         }
         blank = strrchr(pos,' '); //start in reverse order (case like unsigned short xx)
         if (blank) {
            *blank = 0;
            //check that this is a known data member
            dm  = (TDataMember*)members->FindObject(blank+1);
            rdm1 = 0;
            nextrdm.Reset();
            while((rdm = (TRealData*)nextrdm())) {
               if (rdm->GetDataMember() != dm) continue;
               rdm1 = rdm;
               break;
            }
            if (!dm || !rdm1) {
               printf("Unknown data member:%s %s in class:%s\n",pos,blank+1,fClass->GetName());
               pos = colon+1;
               continue;
            }
            // Is type a class name?
            TClass *clt = gROOT->GetClass(pos);
            if (clt) {
               //checks that the class matches with the data member declaration
               if (strcmp(pos,dm->GetTypeName()) == 0) {
                  newtype[fNdata] = 20;
                  fNdata++;
                  printf("class: %s member=%s\n",pos,blank+1);
               } else {
                  printf("Mismatch between class:%s %s and data member:%s %s in class:%s\n",
                         pos,blank+1,dm->GetTypeName(),blank+1,fClass->GetName());
               }
               // Is type a basic type?
            } else {
               dt = (TDataType*)gROOT->GetListOfTypes()->FindObject(pos);
               if (dt) {
                  Int_t dtype = dt->GetType();
                  //check that this is a valid data member and that the
                  //declared type matches with the data member type
                  if (dm->GetDataType()->GetType() == dtype) {
                     //store type and offset
                     newtype[fNdata]   = dtype;
                     newoffset[fNdata] = rdm->GetThisOffset();
                     fNdata++;
                     printf("type=%s, basic type=%s dtype=%d, member=%s\n",pos,dt->GetFullTypeName(),dtype,blank+1);
                  } else {
                     printf("Mismatch between type:%s %s and data member:%s %s in class:%s\n",
                            pos,blank+1,dm->GetTypeName(),blank+1,fClass->GetName());
                  }

               } else {
                  printf("found unknown type:%s, member=%s\n",pos,blank+1);
               }
            }
         } else {
            // very likely a base class
            TClass *base = gROOT->GetClass(pos);
            if (base && fClass->GetBaseClass(pos)) {
               printf("base class:%s\n",pos);
               //get pointer to method baseclass::Streamer
               TMethodCall *methodcall = new TMethodCall(base,"Streamer","");
               newtype[fNdata]   = 0;
               newmethod[fNdata] = (ULong_t)methodcall;
               fNdata++;
            }
         }
         pos = colon+1;
      }
   }
   fType     = new Int_t[fNdata+1];
   fOffset   = new Int_t[fNdata+1];
   fMethod   = new ULong_t[fNdata+1];
   for (Int_t i=0;i<fNdata;i++) {
      fType[i]   = newtype[i];
      fOffset[i] = newoffset[i];
      fMethod[i] = newmethod[i];
   }
   delete [] info;
   delete [] newtype;
   delete [] newoffset;
   delete [] newmethod;
#endif
}

//______________________________________________________________________________
Bool_t TStreamerInfo::CanDelete()
{
   // static function returning true if ReadBuffer can delete object
   return fgCanDelete;
}

//______________________________________________________________________________
Bool_t TStreamerInfo::CanOptimize()
{
   // static function returning true if optimization can be on
   return fgOptimize;
}

//______________________________________________________________________________
void TStreamerInfo::Compile()
{
   // loop on the TStreamerElement list
   // regroup members with same type
   // Store predigested information into local arrays. This saves a huge amount
   // of time compared to an explicit iteration on all elements.

   TObjArray *infos = (TObjArray*)gROOT->GetListOfStreamerInfo();
   if (fNumber >= infos->GetSize()) {
      infos->AddAtAndExpand(this,fNumber);
   } else {
      if (!infos->At(fNumber)) {
         infos->AddAt(this,fNumber);
      }
   }

   delete [] fType;     fType    = 0;
   delete [] fNewType;  fNewType = 0;
   delete [] fOffset;   fOffset  = 0;
   delete [] fLength;   fLength  = 0;
   delete [] fElem;     fElem    = 0;
   delete [] fMethod;   fMethod  = 0;
   delete [] fComp;     fComp    = 0;

   fOptimized = kFALSE;
   fNdata = 0;
   Int_t ndata = fElements->GetEntries();
   fOffset = new Int_t[ndata+1];
   fType   = new Int_t[ndata+1];
   if (ndata == 0) return;  //this may be the case for empty classes(eg TAtt3D)
   fComp   = new CompInfo[ndata];
   fNewType= new Int_t[ndata];
   fLength = new Int_t[ndata];
   fElem   = new ULong_t[ndata];
   fMethod = new ULong_t[ndata];
   TStreamerElement *element;
   Int_t keep = -1;
   Int_t i;
   if (!fgOptimize) SetBit(kCannotOptimize);
   for (i=0;i<ndata;i++) {
      element = (TStreamerElement*)fElements->At(i);
      if (!element) break;
      if (element->GetType() < 0) continue;
      Int_t asize = element->GetSize();
      if (element->GetArrayLength()) asize /= element->GetArrayLength();
      fType[fNdata]   = element->GetType();
      fNewType[fNdata]= element->GetNewType();
      fOffset[fNdata] = element->GetOffset();
      fLength[fNdata] = element->GetArrayLength();
      fElem[fNdata]   = (ULong_t)element;
      fMethod[fNdata] = element->GetMethod();
      // try to group consecutive members of the same type
      if (!TestBit(kCannotOptimize) 
          && (keep>=0)
          && (element->GetType() < 10)
          && (fType[fNdata] == fNewType[fNdata])
          && (fMethod[keep] == 0)
          && (element->GetType() > 0)
          && (element->GetArrayDim() == 0)
          && (fType[keep] < kObject)
          && (fType[keep] != kCharStar) /* do not optimize char* */
          && (element->GetType() == (fType[keep]%kRegrouped))
          && ((element->GetOffset()-fOffset[keep]) == (fLength[keep])*asize)) {
         if (fLength[keep] == 0) fLength[keep]++;
         fLength[keep]++;
         fType[keep] = element->GetType() + kRegrouped;
         fOptimized = kTRUE;
      } else {
         if (fNewType[fNdata] != fType[fNdata]) {
            if (fNewType[fNdata] > 0) {
              if (fType[fNdata] != kCounter) 
                 fType[fNdata] += kConv;
            } else {
               if (fType[fNdata] == kCounter) 
                  Warning("Compile","Counter %s should not be skipped from class %s", element->GetName(), GetName());
               fType[fNdata] += kSkip;   
            }
         }
         keep = fNdata;
         if (fLength[keep] == 0) fLength[keep] = 1;
         fNdata++;
      }
   }

   for (i=0;i<fNdata;i++) {
      element = (TStreamerElement*)fElem[i];
      if (!element)  continue;
      fComp[i].fClass    = element->GetClassPointer();
      fComp[i].fClassName= TString(element->GetTypeName()).Strip(TString::kTrailing, '*');
      fComp[i].fStreamer = element->GetStreamer();
   }
   ComputeSize();

   if (gDebug > 0) ls();
}


//______________________________________________________________________________
void TStreamerInfo::ComputeSize()
{
   // Compute total size of all persistent elements of the class

   TIter next(fElements);
   TStreamerElement *element = (TStreamerElement*)fElements->Last();
   //faster and more precise to use last element offset +size
   //on 64 bit machines, offset may be forced to be a multiple of 8 bytes
   fSize = element->GetOffset() + element->GetSize();
   //fSize = 0;
   //while ((element = (TStreamerElement*)next())) {
   //   fSize += element->GetSize();
   //}
}

//______________________________________________________________________________
void TStreamerInfo::ForceWriteInfo(TFile *file, Bool_t force)
{
   // will force this TStreamerInfo to the file and also
   // all the dependencies.
   // This function is called when streaming a class that contains
   // a null pointer. In this case, the TStreamerInfo for the class
   // with the null pointer must be written to the file and also all the
   // TStreamerInfo of all the classes referenced by the class.
   //
   // if argument force > 0 the loop on class dependencies is forced

   // flag this class
   if (!file) return;
   TArrayC *cindex = file->GetClassIndex();
   //the test below testing fArray[fNumber]>1 is to avoid a recursivity
   //problem in some cases like:
   //        class aProblemChild: public TNamed {
   //        aProblemChild *canBeNull;
   //        };
   if ((cindex->fArray[fNumber] && !force) || cindex->fArray[fNumber]>1) return;
   cindex->fArray[fNumber] = 2;
   cindex->fArray[0] = 1;

   // flag all its dependencies
   TIter next(fElements);
   TStreamerElement *element;
   while ((element = (TStreamerElement*)next())) {
      TClass *cl = element->GetClassPointer();
      if (cl) {
         const char *name = cl->GetName();
         static const char *full_string_name = "basic_string<char,char_traits<char>,allocator<char> >";
         if (!strcmp(name, "string")||!strcmp(name,full_string_name)) continue; //reject string
         if (strstr(name, "vector<")   || strstr(name, "list<") ||
             strstr(name, "set<")      || strstr(name, "map<")  ||
             strstr(name, "deque<")    || strstr(name, "multimap<") ||
             strstr(name, "multiset<") || strstr(name, "::" ))
            continue; //reject STL containers
         
         cl->BuildRealData();
         cl->GetStreamerInfo()->ForceWriteInfo(file, force);
      }
   }
}


//______________________________________________________________________________
Int_t TStreamerInfo::GenerateHeaderFile(const char *dirname)
{
   // Generate header file for the class described by this TStreamerInfo
   // the function is called by TFile::MakeProject for each class in the file

   TClass *cl = gROOT->GetClass(GetName());
   if (cl) {
      if (cl->GetClassInfo()) return 0; // skip known classes
   }
   if (gDebug) printf("generating code for class %s\n",GetName());

   //open the file
   Int_t nch = strlen(dirname) + strlen(GetName()) + 4;
   char *filename = new char[nch];
   sprintf(filename,"%s/%s.h",dirname,GetName());
   FILE *fp = fopen(filename,"w");
   if (!fp) {
      printf("Cannot open output file:%s\n",filename);
      delete [] filename;
      return 0;
   }

   // generate class header
   TDatime td;
   fprintf(fp,"//////////////////////////////////////////////////////////\n");
   fprintf(fp,"//   This class has been generated by TFile::MakeProject\n");
   fprintf(fp,"//     (%s by ROOT version %s)\n",td.AsString(),gROOT->GetVersion());
   fprintf(fp,"//      from the StreamerInfo in file %s\n",gDirectory->GetFile()->GetName());
   fprintf(fp,"//////////////////////////////////////////////////////////\n");
   fprintf(fp,"\n");
   fprintf(fp,"\n");
   fprintf(fp,"#ifndef %s_h\n",GetName());
   fprintf(fp,"#define %s_h\n",GetName());
   fprintf(fp,"\n");

   // compute longest typename and member name
   // in the same loop, generate list of include files
   Int_t ltype = 10;
   Int_t ldata = 10;
   Int_t i,lt,ld;

   char *line = new char[kMaxLen];
   char name[128];
   char cdim[8];
   char *inclist = new char[5000];
   inclist[0] = 0;

   TIter next(fElements);
   TStreamerElement *element;
   Int_t ninc = 0;
   Bool_t Riostream = kFALSE;
   while ((element = (TStreamerElement*)next())) {
      //if (element->IsA() == TStreamerBase::Class()) continue;
      sprintf(name,element->GetName());
      for (i=0;i<element->GetArrayDim();i++) {
         sprintf(cdim,"[%d]",element->GetMaxIndex(i));
         strcat(name,cdim);
      }
      ld = strlen(name);
      lt = strlen(element->GetTypeName());
      if (ltype < lt) ltype = lt;
      if (ldata < ld) ldata = ld;
      //must include Riostream.h in case of an STL container
      if (!Riostream && element->InheritsFrom(TStreamerSTL::Class())) {
         Riostream = kTRUE;
         fprintf(fp,"#include \"Riostream.h\"\n");
      }
      //get include file name if any
      const char *include = element->GetInclude();
      if (strlen(include) == 0) continue;
      const char *slash = strrchr(include,'/');
      if (slash) include = slash+1;
      // do not generate the include if already done
      if (strstr(inclist,include)) continue;
      ninc++;
      strcat(inclist,include);
      if (strstr(include,"include/") || strstr(include,"include\\"))
         fprintf(fp,"#include \"%s\n",include+9);
      else {
         if (slash) fprintf(fp,"#include \"%s\n",include);
         else       fprintf(fp,"#include %s\n",include);
      }
   }
   ltype += 2;
   ldata++; // to take into account the semi colon
   if (ninc == 0) fprintf(fp,"#include \"TNamed.h\"\n");

   // generate class statement with base classes
   fprintf(fp,"\nclass %s",GetName());
   next.Reset();
   Int_t nbase = 0;
   while ((element = (TStreamerElement*)next())) {
      if (element->IsA() != TStreamerBase::Class()) continue;
      nbase++;
      if (nbase == 1) fprintf(fp," : public %s",element->GetName());
      else            fprintf(fp," , public %s",element->GetName());
   }
   fprintf(fp," {\n");

   // generate data members
   fprintf(fp,"\npublic:\n");
   next.Reset();
   while ((element = (TStreamerElement*)next())) {
      for (i=0;i<kMaxLen;i++) line[i] = ' ';
      if (element->IsA() == TStreamerBase::Class()) continue;
      sprintf(name,element->GetName());
      for (Int_t i=0;i<element->GetArrayDim();i++) {
         sprintf(cdim,"[%d]",element->GetMaxIndex(i));
         strcat(name,cdim);
      }
      strcat(name,";");
      ld = strlen(name);
      lt = strlen(element->GetTypeNameBasic());
      strncpy(line+3,element->GetTypeNameBasic(),lt);
      strncpy(line+3+ltype,name,ld);
      if (element->IsaPointer() && !strchr(line,'*')) line[2+ltype] = '*';
      sprintf(line+3+ltype+ldata,"   //%s",element->GetTitle());
      fprintf(fp,"%s\n",line);
   }

   // generate default functions, ClassDef and trailer
   fprintf(fp,"\n   %s();\n",GetName());
   fprintf(fp,"   virtual ~%s();\n\n",GetName());
   fprintf(fp,"   ClassDef(%s,%d) //\n",GetName(),fClassVersion);
   fprintf(fp,"};\n");

   //generate constructor code
   fprintf(fp,"%s::%s() {\n",GetName(),GetName());
   next.Reset();
   while ((element = (TStreamerElement*)next())) {
      if (element->GetType() == kObjectp || element->GetType() == kObjectP ||
          element->GetType() == kAnyp || element->GetType() == kAnyP) {
         fprintf(fp,"   %s = 0;\n",element->GetName());
      }
   }
   fprintf(fp,"}\n\n");
   //generate destructor code
   fprintf(fp,"%s::~%s() {\n",GetName(),GetName());
   next.Reset();
   while ((element = (TStreamerElement*)next())) {
      if (element->GetType() == kObjectp || element->GetType() == kObjectP||
          element->GetType() == kAnyp || element->GetType() == kAnyP
          || element->GetType() == kAnyPnoVT) {
         fprintf(fp,"   delete %s;   %s = 0;\n",element->GetName(),element->GetName());
      }
   }
   fprintf(fp,"}\n\n");
   fprintf(fp,"#endif\n");

   fclose(fp);
   delete [] filename;
   delete [] inclist;
   delete [] line;
   return 1;
}

//______________________________________________________________________________
TStreamerElement *TStreamerInfo::GetCurrentElement()
{
   //static function returning a pointer to the current TStreamerElement
   //fgElement points to the current TStreamerElement being read in ReadBuffer
   return fgElement;
}

//______________________________________________________________________________
Int_t TStreamerInfo::GetDataMemberOffset(TDataMember *dm, TMemberStreamer *&streamer) const
{
   // Compute data member offset
   // return pointer to the Streamer function if one exists

   TIter nextr(fClass->GetListOfRealData());
   char dmbracket[256];
   sprintf(dmbracket,"%s[",dm->GetName());
   Int_t offset = kMissing;
   if (fClass->GetDeclFileLine() < 0) offset = dm->GetOffset();
   TRealData *rdm;
   while ((rdm = (TRealData*)nextr())) {
      char *rdmc = (char*)rdm->GetName();
      //next statement required in case a class and one of its parent class
      //have data members with the same name
      if (dm->IsaPointer() && rdmc[0] == '*') rdmc++;

      if (rdm->GetDataMember() != dm) continue;
      if (strcmp(rdmc,dm->GetName()) == 0) {
         offset   = rdm->GetThisOffset();
         streamer = rdm->GetStreamer();
         break;
      }
      if (strcmp(rdm->GetName(),dm->GetName()) == 0) {
         if (rdm->IsObject()) {
            offset = rdm->GetThisOffset();
            streamer = rdm->GetStreamer();
            break;
         }
      }
      if (strstr(rdm->GetName(),dmbracket)) {
         offset   = rdm->GetThisOffset();
         streamer = rdm->GetStreamer();
         break;
      }
   }
   return offset;
}

//______________________________________________________________________________
TStreamerBasicType *TStreamerInfo::GetElementCounter(const char *countName, TClass *cl)
{
   // Get pointer to a TStreamerBasicType in TClass *cl
   //static function

   TObjArray *sinfos = cl->GetStreamerInfos();
   TStreamerInfo *info = (TStreamerInfo *)sinfos->At(cl->GetClassVersion());

   if (!info || !info->IsBuilt()) {
      // Even if the streamerInfo exist, it could still need to be 'build'
      // It is important to figure this out, because
      //   a) if it is not build, we need to build
      //   b) if is build, we should not build it (or we could end up in an
      //      infinite loop, if the element and its counter are in the same
      //      class!
      
      info = cl->GetStreamerInfo();
   } 
   if (!info) return 0;
   TStreamerElement *element = (TStreamerElement *)info->fElements->FindObject(countName);
   if (!element) return 0;
   if (element->IsA() == TStreamerBasicType::Class()) return (TStreamerBasicType*)element;
   return 0;
}

//______________________________________________________________________________
Int_t TStreamerInfo::GetOffset(const char *elementName) const
{
   // return the offset of the data member as indicated by this StreamerInfo

   if (elementName==0) return 0;

   Int_t offset = 0;
   TStreamerElement *elem = (TStreamerElement*)fElements->FindObject(elementName);
   if (elem) offset = elem->GetOffset();

   return offset;
}

//______________________________________________________________________________
Int_t TStreamerInfo::GetSize() const
{
   //  return total size of all persistent elements of the class (with offsets)

   return fSize;
}

//______________________________________________________________________________
Int_t TStreamerInfo::GetSizeElements() const
{
   //  return total size of all persistent elements of the class
   //  use GetSize if you want to get the real size in memory

   TIter next(fElements);
   TStreamerElement *element;
   Int_t asize = 0;
   while ((element = (TStreamerElement*)next())) {
      asize += element->GetSize();
   }
   return asize;
}

//______________________________________________________________________________
TStreamerElement* TStreamerInfo::GetStreamerElement(const char* datamember, Int_t &offset) const
{
   //  Return the StreamerElement of "datamember" inside this class of any of its
   //  base class.  The offset information contained in the StreamerElement is related
   //  to its immediate containing class, so we return in 'offset' the offset inside
   //  the class of this streamerInfo.

   if (!fElements) return 0;
   TStreamerElement *element = (TStreamerElement*)fElements->FindObject(datamember);
   if (element) {
      offset = element->GetOffset();
      return element;
   }

   if (fClass->GetClassInfo()) {
      // We have the class's shared library.

      TStreamerElement *base_element;
      TBaseClass *base;
      TClass *base_cl;
      Int_t base_offset = 0;
      Int_t local_offset = 0;
      TIter nextb(fClass->GetListOfBases());
      //iterate on list of base classes
      while((base = (TBaseClass*)nextb())) {
         base_cl = gROOT->GetClass(base->GetName());
         base_element = (TStreamerElement*)fElements->FindObject(base->GetName());
         if (!base_cl || !base_element) continue;
         base_offset = base_element->GetOffset();
         
         element = base_cl->GetStreamerInfo()->GetStreamerElement(datamember,local_offset);
         if (element) {
            offset = base_offset + local_offset;
            return element;
         }
      }
   } else {      
      // We do not have the class's shared library

      TIter next( fElements );
      TStreamerElement * curelem;
      while ((curelem = (TStreamerElement*)next())) {

         if (curelem->InheritsFrom(TStreamerBase::Class())) {

            TClass *baseClass = curelem->GetClassPointer();
            if (!baseClass) continue;
            Int_t base_offset = curelem->GetOffset();
            
            Int_t local_offset = 0;
            element = baseClass->GetStreamerInfo()->GetStreamerElement(datamember,local_offset);
            if (element) {
               offset = base_offset + local_offset;
               return element;
            }
         }
      }
   }
   return 0;
}

//______________________________________________________________________________
TStreamerElement* TStreamerInfo::GetStreamerElementReal(Int_t i, Int_t j) const
{
   //  TStreamerInfo  holds two types of data structures
   //    -TObjArray* fElements; containing the list of all TStreamerElement
   //       objects for this class version.
   //    -ULong_t*  fElem;  containing the preprocessed information 
   //       by TStreamerInfo::Compile In case consecutive data members
   //       are of the same type, the Compile function declares the consecutive
   //       elements as one single element in fElems.
   //
   //  example with the class TAttLine
   //   gROOT->GetClass("TAttLine")->GetStreamerInfo()->ls(); produces;
   //      StreamerInfo for class: TAttLine, version=1
   //       short        fLineColor      offset=  4 type= 2 line color
   //       short        fLineStyle      offset=  6 type= 2 line style
   //       short        fLineWidth      offset=  8 type= 2 line width
   //        i= 0, fLineColor      type= 22, offset=  4, len=3, method=0
   //  For I/O implementations (eg. XML) , one has to know the original name
   //  of the data member. This function can be used to return a pointer
   //  to the original TStreamerElement object corresponding to the j-th
   //  element of a compressed array in fElems.
   //
   //  parameters description:
   //    - i: the serial number in array fElem
   //    - j: the element number in the array of consecutive types
   //  In the above example the class TAttLine has 3 consecutive data members
   //  of the same type "short". Compile makes one single array of 3 elements.
   //  To access the TStreamerElement for the second element
   //  of this array, one can call:
   //     TStreamerElement *el = GetStreamerElementReal(0,1);
   //     const char* membername = el->GetName();
   //  This function is typically called from Tbuffer, TXmlBuffer
   
   if (i < 0 || i >= fNdata) return 0;
   if (j < 0) return 0;
   if (!fElements) return 0;
   TStreamerElement *se = (TStreamerElement*)fElem[i];
   if (!se) return 0;
   Int_t nelems = fElements->GetEntriesFast();
   for (Int_t ise=0;ise<nelems;ise++) {
      if (se != (TStreamerElement*)fElements->UncheckedAt(ise)) continue;   
      if (ise+j >= nelems) return 0;
      return (TStreamerElement*)fElements->UncheckedAt(ise+j);   
   }
   return 0;
}

//______________________________________________________________________________
Bool_t TStreamerInfo::GetStreamMemberWise()
{
   // Return whether the TStreamerInfos will save the collections in 
   // "member-wise" order whenever possible.    The default is to store member-wise.
   // kTRUE indicates member-wise storing
   // kFALSE inddicates object-wise storing
   // 
   // A collection can be saved member wise when it contain is guaranteed to be
   // homogeneous.  For example std::vector<THit> can be stored member wise, 
   // while std::vector<THit*> can not (possible use of polymorphism).

   return fgStreamMemberWise;
}

//______________________________________________________________________________
Double_t  TStreamerInfo::GetValueAux(Int_t type, void *ladd, Int_t k, Int_t len)
{
   switch (type) {
      // basic types
      case kBool:              {Bool_t *val   = (Bool_t*)ladd;   return Double_t(*val);}
      case kChar:              {Char_t *val   = (Char_t*)ladd;   return Double_t(*val);}
      case kShort:             {Short_t *val  = (Short_t*)ladd;  return Double_t(*val);}
      case kInt:               {Int_t *val    = (Int_t*)ladd;    return Double_t(*val);}
      case kLong:              {Long_t *val   = (Long_t*)ladd;   return Double_t(*val);}
      case kLong64:            {Long64_t *val = (Long64_t*)ladd; return Double_t(*val);}
      case kFloat:             {Float_t *val  = (Float_t*)ladd;  return Double_t(*val);}
      case kDouble:            {Double_t *val = (Double_t*)ladd; return Double_t(*val);}
      case kDouble32:          {Double_t *val = (Double_t*)ladd; return Double_t(*val);}
      case kUChar:             {UChar_t *val  = (UChar_t*)ladd;  return Double_t(*val);}
      case kUShort:            {UShort_t *val = (UShort_t*)ladd; return Double_t(*val);}
      case kUInt:              {UInt_t *val   = (UInt_t*)ladd;   return Double_t(*val);}
      case kULong:             {ULong_t *val  = (ULong_t*)ladd;  return Double_t(*val);}
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
      case kULong64:           {Long64_t *val = (Long64_t*)ladd;  return Double_t(*val);}
#else
      case kULong64:           {ULong64_t *val= (ULong64_t*)ladd; return Double_t(*val);}
#endif
      case kBits:              {UInt_t *val   = (UInt_t*)ladd;   return Double_t(*val);}

         // array of basic types  array[8]
      case kOffsetL + kBool:    {Bool_t *val   = (Bool_t*)ladd;   return Double_t(val[k]);}
      case kOffsetL + kChar:    {Char_t *val   = (Char_t*)ladd;   return Double_t(val[k]);}
      case kOffsetL + kShort:   {Short_t *val  = (Short_t*)ladd;  return Double_t(val[k]);}
      case kOffsetL + kInt:     {Int_t *val    = (Int_t*)ladd;    return Double_t(val[k]);}
      case kOffsetL + kLong:    {Long_t *val   = (Long_t*)ladd;   return Double_t(val[k]);}
      case kOffsetL + kLong64:  {Long64_t *val = (Long64_t*)ladd; return Double_t(val[k]);}
      case kOffsetL + kFloat:   {Float_t *val  = (Float_t*)ladd;  return Double_t(val[k]);}
      case kOffsetL + kDouble:  {Double_t *val = (Double_t*)ladd; return Double_t(val[k]);}
      case kOffsetL + kDouble32:{Double_t *val = (Double_t*)ladd; return Double_t(val[k]);}
      case kOffsetL + kUChar:   {UChar_t *val  = (UChar_t*)ladd;  return Double_t(val[k]);}
      case kOffsetL + kUShort:  {UShort_t *val = (UShort_t*)ladd; return Double_t(val[k]);}
      case kOffsetL + kUInt:    {UInt_t *val   = (UInt_t*)ladd;   return Double_t(val[k]);}
      case kOffsetL + kULong:   {ULong_t *val  = (ULong_t*)ladd;  return Double_t(val[k]);}
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
      case kOffsetL + kULong64: {Long64_t *val = (Long64_t*)ladd;  return Double_t(val[k]);}
#else
      case kOffsetL + kULong64:{ULong64_t *val= (ULong64_t*)ladd; return Double_t(val[k]);}
#endif

#define READ_ARRAY(TYPE_t)                               \
         {                                               \
            Int_t sub_instance, index;                   \
            Int_t instance = k;                          \
            if (len) {                                   \
               index = instance / len;                   \
               sub_instance = instance % len;            \
            } else {                                     \
               index = instance;                         \
               sub_instance = 0;                         \
            }                                            \
            TYPE_t **val =(TYPE_t**)(ladd);              \
            return Double_t((val[sub_instance])[index]); \
         }

         // pointer to an array of basic types  array[n]
      case kOffsetP + kBool_t:    READ_ARRAY(Bool_t)
      case kOffsetP + kChar_t:    READ_ARRAY(Char_t)
      case kOffsetP + kShort_t:   READ_ARRAY(Short_t)
      case kOffsetP + kInt_t:     READ_ARRAY(Int_t)
      case kOffsetP + kLong_t:    READ_ARRAY(Long_t)
      case kOffsetP + kLong64_t:  READ_ARRAY(Long64_t)
      case kOffsetP + kFloat_t:   READ_ARRAY(Float_t)
      case kOffsetP + kDouble32_t:
      case kOffsetP + kDouble_t:  READ_ARRAY(Double_t)
      case kOffsetP + kUChar_t:   READ_ARRAY(UChar_t)
      case kOffsetP + kUShort_t:  READ_ARRAY(UShort_t)
      case kOffsetP + kUInt_t:    READ_ARRAY(UInt_t)
      case kOffsetP + kULong_t:   READ_ARRAY(ULong_t)
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
      case kOffsetP + kULong64_t: READ_ARRAY(Long64_t)
#else
      case kOffsetP + kULong64_t: READ_ARRAY(ULong64_t)
#endif

          // array counter //[n]
      case kCounter:           {Int_t *val    = (Int_t*)ladd;    return Double_t(*val);}
   }
   return 0;
}
//______________________________________________________________________________
Double_t TStreamerInfo::GetValue(char *pointer, Int_t i, Int_t j, Int_t len) const
{
   //  return value of element i in object at pointer.
   //  The function may be called in two ways:
   //    -method1  len < 0
   //           i is assumed to be the TStreamerElement number i in StreamerInfo
   //    -method2  len >= 0
   //           i is the type
   //           address of variable is directly pointer.

   char *ladd;
   Int_t atype;
   if (len >= 0) {
      ladd  = pointer;
      atype = i;
   } else {
      if (i < 0) return 0;
      ladd  = pointer + fOffset[i];
      atype = fNewType[i];
      len = ((TStreamerElement*)fElem[i])->GetArrayLength();
   }
   return GetValueAux(atype,ladd,j,len);   
}


//______________________________________________________________________________
Double_t TStreamerInfo::GetValueClones(TClonesArray *clones, Int_t i, Int_t j, int k, Int_t eoffset) const
{
   //  return value of element i in object number j in a TClonesArray and eventually
   // element k in a sub-array.

   Int_t nc = clones->GetEntriesFast();
   if (j >= nc) return 0;

   char *pointer = (char*)clones->UncheckedAt(j);
   char *ladd    = pointer + eoffset + fOffset[i];
   return GetValueAux(fType[i],ladd,k,((TStreamerElement*)fElem[i])->GetArrayLength());
}

//______________________________________________________________________________
Double_t TStreamerInfo::GetValueSTL(TVirtualCollectionProxy *cont, Int_t i, Int_t j, int k, Int_t eoffset) const
{
   //  return value of element i in object number j in a TClonesArray and eventually
   // element k in a sub-array.

   Int_t nc = cont->Size();
   if (j >= nc) return 0;

   char *pointer = (char*)cont->At(j);
   char *ladd    = pointer + eoffset + fOffset[i];
   return GetValueAux(fType[i],ladd,k,((TStreamerElement*)fElem[i])->GetArrayLength());
}
//______________________________________________________________________________
void TStreamerInfo::ls(Option_t *option) const
{
   //  List the TStreamerElement list and also the precomputed tables
   printf("\nStreamerInfo for class: %s, version=%d\n",GetName(),fClassVersion);

   if (fElements) fElements->ls(option);
   for (Int_t i=0;i<fNdata;i++) {
      TStreamerElement *element = (TStreamerElement*)fElem[i];
      printf("   i=%2d, %-15s type=%3d, offset=%3d, len=%d, method=%ld\n",i,element->GetName(),fType[i],fOffset[i],fLength[i],fMethod[i]);
   }
}

//______________________________________________________________________________
Int_t TStreamerInfo::New(const char *p)
{
   //  emulated constructor for this class.
   //  An emulated object is created at address p

   TIter next(fElements);
   TStreamerElement *element;
   while ((element = (TStreamerElement*)next())) {
      Int_t etype = element->GetType();
      if (element->GetOffset() == kMissing) continue;
      //cle->GetStreamerInfo(); //necessary in case "->" is not specified
      if (etype == kObjectp || etype == kAnyp) {
         // if the option "->" is given in the data member comment field
         // it is assumed that the object exist before reading data in.
         // In this case an object must be created
         if (strstr(element->GetTitle(),"->") == element->GetTitle()) {
            char line[200];
            char pname[100];
            char clonesClass[40];
            // in case of a TClonesArray, the class name of the contained objects
            // must be specified
            sprintf(clonesClass,"%s"," ");
            if (element->GetClassPointer() == TClonesArray::Class()) {
               char *bracket1 = (char*)strrchr(element->GetTitle(),'(');
               char *bracket2 = (char*)strrchr(element->GetTitle(),')');
               if (bracket1 && bracket2) {
                  clonesClass[0] = '"';
                  strncat(clonesClass,bracket1+1,bracket2-bracket1-1);
                  strcat(clonesClass,"\"");
               }
            }
            // object is created via the interpreter
            sprintf(pname,"R__%s_%s",GetName(),element->GetName());
            sprintf(line,"%s* %s = (%s*)0x%lx; *%s = new %s(%s);",
                    element->GetTypeName(),pname,element->GetTypeName(),
                    (Long_t)((char*)p + element->GetOffset()),pname,
                    element->GetClassPointer()->GetName(),clonesClass);
            gROOT->ProcessLine(line);
         }
      }
      if (etype == kObject || etype == kAny || etype == kBase ||
          etype == kTObject || etype == kTString || etype == kTNamed ||
          etype == kSTL
          ) {
         TClass *cle = element->GetClassPointer();
         if (!cle) continue;
         cle->New((char*)p + element->GetOffset());
      }
      if (etype == kObject +kOffsetL || etype == kAny+kOffsetL || 
          etype == kTObject+kOffsetL || etype == kTString+kOffsetL || 
          etype == kTNamed +kOffsetL || etype == kSTL+kOffsetL) {
         TClass *cle = element->GetClassPointer();
         if (!cle) continue;
 
         Int_t size = cle->Size();
         char *start= (char*)p + element->GetOffset();
         Int_t len = element->GetArrayLength();

         for (Int_t j=0;j<len;j++) {
            cle->New(start);
            start += size;
         }
      }
      if (etype == kAnyP || etype == kObjectP || etype == kAnyPnoVT || etype == kSTLp) {
         // Initialize to zero
         void **where = (void**)((char*)p + element->GetOffset());
         Int_t len = element->GetArrayLength();
         for (Int_t j=0;j<len;j++) {         
            where[j] = 0; 
         }
      }
   }
   return 0;
}

//______________________________________________________________________________
void TStreamerInfo::Optimize(Bool_t opt)
{
   //  This is a static function.
   //  Set optimization option.
   //  When this option is activated (default), consecutive data members
   //  of the same type are merged into an array (faster).
   //  Optimization must be off in TTree split mode.

   fgOptimize = opt;
}


//______________________________________________________________________________
void TStreamerInfo::PrintValue(const char *name, char *pointer, Int_t i, Int_t len, Int_t lenmax) const
{
   //  print value of element i in object at pointer
   //  The function may be called in two ways:
   //    -method1  len < 0
   //           i is assumed to be the TStreamerElement number i in StreamerInfo
   //    -method2  len >= 0
   //           i is the type
   //           address of variable is directly pointer.
   //           len is the number of elements to be printed starting at pointer.

   char *ladd;
   Int_t atype,aleng;
   printf(" %-15s = ",name);

   TStreamerElement * aElement  = 0;
   Int_t *count  = 0;
   if (len >= 0) {
      ladd  = pointer;
      atype = i;
      aleng = len;
   } else        {
      if (i < 0) {printf("NULL\n"); return;}
      ladd  = pointer + fOffset[i];
      atype = fNewType[i];
      aleng = fLength[i];
      aElement  = (TStreamerElement*)fElem[i];
      count = (Int_t*)(pointer+fMethod[i]);
   }
   if (aleng > lenmax) aleng = lenmax;
   
   PrintValueAux(ladd,atype,aElement,aleng,count);
   printf("\n");
}


//______________________________________________________________________________
void TStreamerInfo::PrintValueClones(const char *name, TClonesArray *clones, Int_t i, Int_t eoffset, Int_t lenmax) const
{
   //  print value of element i in a TClonesArray

   if (!clones) {printf(" %-15s = \n",name); return;}
   printf(" %-15s = ",name);
   Int_t nc = clones->GetEntriesFast();
   if (nc > lenmax) nc = lenmax;

   Int_t offset = eoffset + fOffset[i];
   TStreamerElement *aElement  = (TStreamerElement*)fElem[i];
   int aleng = fLength[i];
   if (aleng > lenmax) aleng = lenmax;

   for (Int_t k=0;k<nc;k++) {
      char *pointer = (char*)clones->UncheckedAt(k);
      char *ladd = pointer+offset;
      Int_t *count = (Int_t*)(pointer+fMethod[i]);
      PrintValueAux(ladd,fNewType[i],aElement, aleng, count);
      if (k < nc-1) printf(", ");
   }
   printf("\n");

}

//______________________________________________________________________________
void TStreamerInfo::PrintValueSTL(const char *name, TVirtualCollectionProxy *cont, Int_t i, Int_t eoffset, Int_t lenmax) const
{
   //  print value of element i in a TClonesArray

   if (!cont) {printf(" %-15s = \n",name); return;}
   printf(" %-15s = ",name);
   Int_t nc = cont->Size();
   if (nc > lenmax) nc = lenmax;

   Int_t offset = eoffset + fOffset[i];
   TStreamerElement *aElement  = (TStreamerElement*)fElem[i];
   int aleng = fLength[i];
   if (aleng > lenmax) aleng = lenmax;

   for (Int_t k=0;k<nc;k++) {
      char *pointer = (char*)cont->At(k);
      char *ladd = pointer+offset;
      Int_t *count = (Int_t*)(pointer+fMethod[i]);
      PrintValueAux(ladd,fNewType[i],aElement, aleng, count);
      if (k < nc-1) printf(", ");
   }
   printf("\n");

}

//______________________________________________________________________________
void TStreamerInfo::SetCanDelete(Bool_t opt)
{
   //  This is a static function.
   //  Set object delete option.
   //  When this option is activated (default), ReadBuffer automatically
   //  delete objects when a data member is a pointer to an object.
   //  If your constructor is not presetting pointers to 0, you must
   //  call this static function TStreamerInfo::SetCanDelete(kFALSE);

   fgCanDelete = opt;
}

//______________________________________________________________________________
Bool_t TStreamerInfo::SetStreamMemberWise(Bool_t enable) 
{
   // Set whether the TStreamerInfos will save the collections in 
   // "member-wise" order whenever possible.  The default is to store member-wise.
   // kTRUE indicates member-wise storing
   // kFALSE inddicates object-wise storing
   // This function returns the previous value of fgStreamMemberWise.

   // A collection can be saved member wise when it contain is guaranteed to be
   // homogeneous.  For example std::vector<THit> can be stored member wise, 
   // while std::vector<THit*> can not (possible use of polymorphism).
   
   Bool_t prev = fgStreamMemberWise;
   fgStreamMemberWise = enable;
   return prev;
}


//______________________________________________________________________________
void TStreamerInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class TStreamerInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c);
      fOldVersion = R__v;
      if (R__v > 1) {
         TStreamerInfo::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
         return;
      }
      //====process old versions before automatic schema evolution
      TNamed::Streamer(R__b);
      R__b >> fCheckSum;
      R__b >> fClassVersion;
      R__b >> fElements;
      R__b.CheckByteCount(R__s, R__c, TStreamerInfo::IsA());
   } else {
      TStreamerInfo::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
void TStreamerInfo::TagFile(TFile *file)
{
   // Mark the classindex of the current file as using this TStreamerInfo

   if (file) {
      TArrayC *cindex = file->GetClassIndex();
      Int_t nindex = cindex->GetSize();
      if (fNumber < 0 || fNumber >= nindex) {
         Error("TagFile","StreamerInfo: %s number: %d out of range[0,%d] in file: %s",
               GetName(),fNumber,nindex,file->GetName());
         return;
      }
      if (cindex->fArray[fNumber] == 0) {
         cindex->fArray[0]       = 1;
         cindex->fArray[fNumber] = 1;
      }
   }

}

//______________________________________________________________________________
#ifdef DOLOOP
#undef DOLOOP
#endif
#define DOLOOP for(k=0,pointer=arr[0]; k<narr; pointer=arr[++k]) 


namespace {
   static void PrintCR(int j,Int_t aleng, UInt_t ltype) 
   {
      if (j == aleng-1) printf("\n"); 
      else { 
         printf(", "); 
         if (j%ltype==ltype-1) printf("\n                    "); 
      }
   }
}  

//______________________________________________________________________________
void TStreamerInfo::PrintValueAux(char *ladd, Int_t atype, 
                                  TStreamerElement * aElement, Int_t aleng, 
                                  Int_t *count)
{
   //  print value of element  in object at pointer, type atype, leng aleng or *count
   //  The function may be called in two ways:
   //    -method1  len < 0
   //           i is assumed to be the TStreamerElement number i in StreamerInfo
   //    -method2  len >= 0
   //           i is the type
   //           address of variable is directly pointer.
   //           len is the number of elements to be printed starting at pointer.
   int j;

   //assert(  ! ((kOffsetP + kChar)<=atype && atype<=(kOffsetP + kBool) && count==0) );
   switch (atype) {
      // basic types
      case kBool:              {Bool_t    *val = (Bool_t*   )ladd; printf("%d" ,*val);  break;}
      case kChar:              {Char_t    *val = (Char_t*   )ladd; printf("%d" ,*val);  break;}
      case kShort:             {Short_t   *val = (Short_t*  )ladd; printf("%d" ,*val);  break;}
      case kInt:               {Int_t     *val = (Int_t*    )ladd; printf("%d" ,*val);  break;}
      case kLong:              {Long_t    *val = (Long_t*   )ladd; printf("%ld",*val);  break;}
      case kLong64:            {Long64_t  *val = (Long64_t* )ladd; printf("%lld",*val);  break;}
      case kFloat:             {Float_t   *val = (Float_t*  )ladd; printf("%f" ,*val);  break;}
      case kDouble:            {Double_t  *val = (Double_t* )ladd; printf("%g" ,*val);  break;}
      case kDouble32:          {Double_t  *val = (Double_t* )ladd; printf("%g" ,*val);  break;}
      case kUChar:             {UChar_t   *val = (UChar_t*  )ladd; printf("%u" ,*val);  break;}
      case kUShort:            {UShort_t  *val = (UShort_t* )ladd; printf("%u" ,*val);  break;}
      case kUInt:              {UInt_t    *val = (UInt_t*   )ladd; printf("%u" ,*val);  break;}
      case kULong:             {ULong_t   *val = (ULong_t*  )ladd; printf("%lu",*val);  break;}
      case kULong64:           {ULong64_t *val = (ULong64_t*)ladd; printf("%llu",*val);  break;}
      case kBits:              {UInt_t    *val = (UInt_t*   )ladd; printf("%d" ,*val);  break;}

         // array of basic types  array[8]
      case kOffsetL + kBool:    {Bool_t    *val = (Bool_t*   )ladd; for(j=0;j<aleng;j++) { printf("%c " ,val[j]); PrintCR(j,aleng,20); } break;}
      case kOffsetL + kChar:    {Char_t    *val = (Char_t*   )ladd; for(j=0;j<aleng;j++) { printf("%c " ,val[j]); PrintCR(j,aleng,20); } break;}
      case kOffsetL + kShort:   {Short_t   *val = (Short_t*  )ladd; for(j=0;j<aleng;j++) { printf("%d " ,val[j]); PrintCR(j,aleng,10); } break;}
      case kOffsetL + kInt:     {Int_t     *val = (Int_t*    )ladd; for(j=0;j<aleng;j++) { printf("%d " ,val[j]); PrintCR(j,aleng,10); } break;}
      case kOffsetL + kLong:    {Long_t    *val = (Long_t*   )ladd; for(j=0;j<aleng;j++) { printf("%ld ",val[j]); PrintCR(j,aleng, 5); } break;}
      case kOffsetL + kLong64:  {Long64_t  *val = (Long64_t* )ladd; for(j=0;j<aleng;j++) { printf("%lld ",val[j]);PrintCR(j,aleng, 5); } break;}
      case kOffsetL + kFloat:   {Float_t   *val = (Float_t*  )ladd; for(j=0;j<aleng;j++) { printf("%f " ,val[j]); PrintCR(j,aleng, 5); } break;}
      case kOffsetL + kDouble:  {Double_t  *val = (Double_t* )ladd; for(j=0;j<aleng;j++) { printf("%g " ,val[j]); PrintCR(j,aleng, 5); } break;}
      case kOffsetL + kDouble32:{Double_t  *val = (Double_t* )ladd; for(j=0;j<aleng;j++) { printf("%g " ,val[j]); PrintCR(j,aleng, 5); } break;}
      case kOffsetL + kUChar:   {UChar_t   *val = (UChar_t*  )ladd; for(j=0;j<aleng;j++) { printf("%u " ,val[j]); PrintCR(j,aleng,20); } break;}
      case kOffsetL + kUShort:  {UShort_t  *val = (UShort_t* )ladd; for(j=0;j<aleng;j++) { printf("%u " ,val[j]); PrintCR(j,aleng,10); } break;}
      case kOffsetL + kUInt:    {UInt_t    *val = (UInt_t*   )ladd; for(j=0;j<aleng;j++) { printf("%u " ,val[j]); PrintCR(j,aleng, 5); } break;}
      case kOffsetL + kULong:   {ULong_t   *val = (ULong_t*  )ladd; for(j=0;j<aleng;j++) { printf("%lu ",val[j]); PrintCR(j,aleng, 5); } break;}
      case kOffsetL + kULong64: {ULong64_t *val = (ULong64_t*)ladd; for(j=0;j<aleng;j++) { printf("%llu ",val[j]);PrintCR(j,aleng, 5); } break;}
      case kOffsetL + kBits:    {UInt_t    *val = (UInt_t*   )ladd; for(j=0;j<aleng;j++) { printf("%d " ,val[j]); PrintCR(j,aleng, 5); } break;}

         // pointer to an array of basic types  array[n]
      case kOffsetP + kBool:    {Bool_t   **val = (Bool_t**  )ladd; for(j=0;j<*count;j++) { printf("%d " ,(*val)[j]);  PrintCR(j,aleng,20); } break;}
      case kOffsetP + kChar:    {Char_t   **val = (Char_t**  )ladd; for(j=0;j<*count;j++) { printf("%d " ,(*val)[j]);  PrintCR(j,aleng,20); } break;}
      case kOffsetP + kShort:   {Short_t  **val = (Short_t** )ladd; for(j=0;j<*count;j++) { printf("%d " ,(*val)[j]);  PrintCR(j,aleng,10); } break;}
      case kOffsetP + kInt:     {Int_t    **val = (Int_t**   )ladd; for(j=0;j<*count;j++) { printf("%d " ,(*val)[j]);  PrintCR(j,aleng,10); } break;}
      case kOffsetP + kLong:    {Long_t   **val = (Long_t**  )ladd; for(j=0;j<*count;j++) { printf("%ld ",(*val)[j]);  PrintCR(j,aleng, 5); } break;}
      case kOffsetP + kLong64:  {Long64_t **val = (Long64_t**)ladd; for(j=0;j<*count;j++) { printf("%lld ",(*val)[j]); PrintCR(j,aleng, 5); } break;}
      case kOffsetP + kFloat:   {Float_t  **val = (Float_t** )ladd; for(j=0;j<*count;j++) { printf("%f " ,(*val)[j]);  PrintCR(j,aleng, 5); } break;}
      case kOffsetP + kDouble:  {Double_t **val = (Double_t**)ladd; for(j=0;j<*count;j++) { printf("%g " ,(*val)[j]);  PrintCR(j,aleng, 5); } break;}
      case kOffsetP + kDouble32:{Double_t **val = (Double_t**)ladd; for(j=0;j<*count;j++) { printf("%g " ,(*val)[j]);  PrintCR(j,aleng, 5); } break;}
      case kOffsetP + kUChar:   {UChar_t  **val = (UChar_t** )ladd; for(j=0;j<*count;j++) { printf("%u " ,(*val)[j]);  PrintCR(j,aleng,20); } break;}
      case kOffsetP + kUShort:  {UShort_t **val = (UShort_t**)ladd; for(j=0;j<*count;j++) { printf("%u " ,(*val)[j]);  PrintCR(j,aleng,10); } break;}
      case kOffsetP + kUInt:    {UInt_t   **val = (UInt_t**  )ladd; for(j=0;j<*count;j++) { printf("%u " ,(*val)[j]);  PrintCR(j,aleng, 5); } break;}
      case kOffsetP + kULong:   {ULong_t  **val = (ULong_t** )ladd; for(j=0;j<*count;j++) { printf("%lu ",(*val)[j]);  PrintCR(j,aleng, 5); } break;}
      case kOffsetP + kULong64: {ULong64_t**val = (ULong64_t**)ladd; for(j=0;j<*count;j++){ printf("%llu ",(*val)[j]); PrintCR(j,aleng, 5); } break;}

         // array counter //[n]
      case kCounter:            {Int_t *val    = (Int_t*)ladd;    printf("%d",*val);  break;}
         // char *
      case kCharStar:{
         char **val = (char**)ladd;
         if (*val) printf("%s",*val);
         break;
      }
      // Class *  derived from TObject with comment field  //->
      case kObjectp: {
         TObject **obj = (TObject**)(ladd);
         TStreamerObjectPointer *el = (TStreamerObjectPointer*)aElement;
         printf("(%s*)%lx",el->GetClass()->GetName(),(Long_t)(*obj));
         break;
      }

      // Class*   derived from TObject
      case kObjectP: {
         TObject **obj = (TObject**)(ladd);
         TStreamerObjectPointer *el = (TStreamerObjectPointer*)aElement;
         printf("(%s*)%lx",el->GetClass()->GetName(),(Long_t)(*obj));
         break;
      }

      // Class    derived from TObject
      case kObject:  {
         TObject *obj = (TObject*)(ladd);
         printf("%s",obj->GetName());
         break;
      }

      // Special case for TString, TObject, TNamed
      case kTString: {
         TString *st = (TString*)(ladd);
         printf("%s",st->Data());
         break;
      }
      case kTObject: {
         TObject *obj = (TObject*)(ladd);
         printf("%s",obj->GetName());
         break;
      }
      case kTNamed:  {
         TNamed *named = (TNamed*) (ladd);
         printf("%s/%s",named->GetName(),named->GetTitle());
         break;
      }

      // Class *  not derived from TObject with comment field  //->
      case kAnyp:    {
         TObject **obj = (TObject**)(ladd);
         TStreamerObjectAnyPointer *el = (TStreamerObjectAnyPointer*)aElement;
         printf("(%s*)%lx",el->GetClass()->GetName(),(Long_t)(*obj));
         break;
      }

      // Class*   not derived from TObject
      case kAnyP:    {
         TObject **obj = (TObject**)(ladd);
         TStreamerObjectAnyPointer *el = (TStreamerObjectAnyPointer*)aElement;
         printf("(%s*)%lx",el->GetClass()->GetName(),(Long_t)(*obj));
         break;
      }
      // Any Class not derived from TObject
      case kOffsetL + kObjectp:
      case kOffsetL + kObjectP:
      case kAny:     {
         printf("printing kAny case (%d)",atype);
         TMemberStreamer *pstreamer = aElement->GetStreamer();
         if (pstreamer == 0) {
            //printf("ERROR, Streamer is null\n");
            //aElement->ls();
            break;
         }
         //(*pstreamer)(b,ladd,0);
         break;
      }
      // Base Class
      case kBase:    {
         printf("printing kBase case (%d)",atype);
         //aElement->ReadBuffer(b,pointer);
         break;
      }

      case kOffsetL + kObject:
      case kOffsetL + kTString:
      case kOffsetL + kTObject:
      case kOffsetL + kTNamed:
      case kStreamer: {
         printf("printing kStreamer case (%d)",atype);
         TMemberStreamer *pstreamer = aElement->GetStreamer();
         if (pstreamer == 0) {
            //printf("ERROR, Streamer is null\n");
            //aElement->ls();
            break;
         }
         //UInt_t start,count;
         //b.ReadVersion(&start, &count);
         //(*pstreamer)(b,ladd,0);
         //b.CheckByteCount(start,count,IsA());
         break;
      }

      case kStreamLoop: {
         printf("printing kStreamLoop case (%d)",atype);
         TMemberStreamer *pstreamer = aElement->GetStreamer();
         if (pstreamer == 0) {
            //printf("ERROR, Streamer is null\n");
            //aElement->ls();
            break;
         }
         //Int_t *counter = (Int_t*)(count);
         //UInt_t start,count;
         ///b.ReadVersion(&start, &count);
         //(*pstreamer)(b,ladd,*counter);
         //b.CheckByteCount(start,count,IsA());
         break;
      }
   }
}

//______________________________________________________________________________
void TStreamerInfo::Update(const TClass *oldcl, TClass *newcl) 
{
   //function called by the TClass constructor when replacing an emulated class
   //by the real class
   
   TStreamerElement *element;
   TIter nextElement(GetElements());
   while ((element = (TStreamerElement*)nextElement())) {
      element->Update(oldcl,newcl);
   }
   for (Int_t i=0;i<fNdata;i++) {
      fComp[i].Update(oldcl,newcl);
   }   
   
}

