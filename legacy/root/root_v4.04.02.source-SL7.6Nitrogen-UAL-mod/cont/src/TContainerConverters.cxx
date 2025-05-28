// @(#)root/cony:$Name:  $:$Id: TContainerConverters.cxx,v 1.2 2004/11/17 19:48:19 brun Exp $
// Author: Philippe Canal  11/11/2004

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Helper classes to convert collection from ROOT collection to STL     //
// collections                                                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TContainerConverters.h"
#include "TClonesArray.h"
#include "TVirtualCollectionProxy.h"
#include "TError.h"

namespace {
   const Int_t  kMapOffset = 2;
}

//________________________________________________________________________
TConvertClonesArrayToProxy::TConvertClonesArrayToProxy(
   TVirtualCollectionProxy *proxy,                           
   Bool_t isPointer, Bool_t isPrealloc) :
      fIsPointer(isPointer),
      fIsPrealloc(isPrealloc),
      fProxy(proxy?proxy->Generate():0) 
{
   if (isPointer) fOffset = sizeof(TClonesArray*);
   else fOffset = sizeof(TClonesArray*);
}

void TConvertClonesArrayToProxy::operator()(TBuffer &b, void *pmember, Int_t size) 
{
   // Read a TClonesArray in the TBuffer b and load it into a (stl) collection

   TStreamerInfo *subinfo = fProxy->GetValueClass()->GetStreamerInfo();
   Assert(subinfo); 

   Int_t   nobjects, dummy;
   char    nch;
   TString s;
   char classv[256];
   void *env;
   UInt_t R__s, R__c;

   Assert(b.IsReading());

   Bool_t needAlloc = fIsPointer && !fIsPrealloc;

   if (needAlloc) {
      char *addr = (char*)pmember;
      for(Int_t k=0; k<size; ++k, addr += fOffset ) {
         if (*(void**)addr && TStreamerInfo::CanDelete()) {
            fProxy->GetValueClass()->Destructor(*(void**)addr,kFALSE); // call delete and desctructor
         }
         //*(void**)addr = fProxy->New();
         //TClonesArray *clones = (TClonesArray*)ReadObjectAny(TClonesArray::Class());
      }
   }

   char *addr = (char*)pmember;
   if (size==0) size=1;
   for(Int_t k=0; k<size; ++k, addr += fOffset ) {

      if (needAlloc) {
         // Read the class name.

         // make sure fMap is initialized
         b.InitMap();

         // before reading object save start position
         UInt_t startpos = b.Length();
 
         // attempt to load next object as TClass clCast
         UInt_t tag;       // either tag or byte count
         TClass *clRef = b.ReadClass(TClonesArray::Class(), &tag);
         
         if (clRef==0) {
            // got a reference to an already read object
            // got a reference to an already read object
            if (b.GetBufferVersion() > 0) {
               tag += b.GetBufferDisplacement();
             } else {
               if (tag > (UInt_t)b.GetMapCount()) {
                  Error("TConvertClonesArrayToProxy", "object tag too large, I/O buffer corrupted");
                  return;
               }
            }
            void *objptr;
            b.GetMappedObject( tag, objptr, clRef);
            if ( objptr == (void*)-1 ) {
               Error("TConvertClonesArrayToProxy",
                  "Object can not be found in the buffer's map (at %d)",tag);
               continue;
            }
            if ( objptr == 0 ) {
               if (b.GetBufferVersion()==0) continue;

               // No object found at this location in map. It might have been skipped
               // as part of a skipped object. Try to explicitely read the object.
               b.MapObject(*(void**)addr, fProxy->GetCollectionClass(), 0);
               Int_t currentpos = b.Length();
               b.SetBufferOffset( tag - kMapOffset );

               (*this)(b,&objptr,1);
               b.SetBufferOffset( currentpos);

               if (objptr==0) continue;

               clRef = fProxy->GetCollectionClass(); 

            }
            Assert(clRef);
            if (clRef==TClonesArray::Class()) {
               Error("TConvertClonesArrayToProxy",
                  "Object refered to has not been converted from TClonesArray to %s",
                  fProxy->GetCollectionClass()->GetName());
               continue;
            } else if (clRef!=fProxy->GetCollectionClass()) {
               Error("TConvertClonesArrayToProxy",
                  "Object refered to is of type %s instead of %s",
                  clRef->GetName(),fProxy->GetCollectionClass()->GetName());
               continue;
            }
            *(void**)addr = objptr;
            continue;

         } else if (clRef != TClonesArray::Class()) {
            Warning("TConvertClonesArrayToProxy",
                    "Only the TClonesArray part of %s will be read into %s!\n",
                    (clRef!=((TClass*)-1)&&clRef) ? clRef->GetName() : "N/A",
                    fProxy->GetCollectionClass()->GetName());
         } else {
            *(void**)addr = fProxy->New();
            if (b.GetBufferVersion()>0) {
               b.MapObject(*(void**)addr, fProxy->GetCollectionClass(), startpos+kMapOffset);
            } else {
               b.MapObject(*(void**)addr, fProxy->GetCollectionClass(), b.GetMapCount() );
            }
         }
      }
      void *obj;
      if (fIsPointer) obj = *(void**)addr;
      else obj = addr;

      TObject TObjDummy;
      Version_t v = b.ReadVersion(&R__s, &R__c);

      //if (v == 3) {
      //   const int_t koldbypassstreamer = bit(14);
      //   if (testbit(koldbypassstreamer)) bypassstreamer();
      //}
      if (v > 2) TObjDummy.Streamer(b);
      TString fName;
      if (v > 1) fName.Streamer(b);
      s.Streamer(b);
      strncpy(classv,s.Data(),255);
      Int_t clv = 0;
      char *semicolon = strchr(classv,';');
      if (semicolon) {
         *semicolon = 0;
         clv = atoi(semicolon+1);
      }
      TClass *cl = gROOT->GetClass(classv);
      if (!cl) {
         printf("TClonesArray::Streamer expecting class %s\n", classv);
         b.CheckByteCount(R__s, R__c,TClonesArray::Class());
         return;
      }

      b >> nobjects;
      if (nobjects < 0) nobjects = -nobjects;  // still there for backward compatibility
      b >> dummy; // fLowerBound is ignored
      if (cl != subinfo->GetClass()) {
         Error("TClonesArray::Conversion to vector","Bad class");
      }
      TVirtualCollectionProxy::TPushPop helper( fProxy, obj );
      env = fProxy->Allocate(nobjects,true);

      if (TObjDummy.TestBit(TClonesArray::kBypassStreamer)) {

         subinfo->ReadBufferSTL(b,fProxy,nobjects,-1,0);

      } else {
         for (Int_t i = 0; i < nobjects; i++) {
            b >> nch;
            if (nch) {
               void* elem = fProxy->At(i);
               b.StreamObject(elem,subinfo->GetClass());
            }
         }
      }
      fProxy->Commit(env);
      b.CheckByteCount(R__s, R__c,TClonesArray::Class());
   }
}
