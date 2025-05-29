// @(#)root/cont:$Name:  $:$Id: TMap.cxx,v 1.13 2004/10/13 15:30:22 rdm Exp $
// Author: Fons Rademakers   12/11/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMap                                                                 //
//                                                                      //
// TMap implements an associative array of (key,value) pairs using a    //
// THashTable for efficient retrieval (therefore TMap does not conserve //
// the order of the entries). The hash value is calculated              //
// using the value returned by the keys Hash() function. Both key and   //
// value need to inherit from TObject.                                  //
//Begin_Html
/*
<img src=gif/tmap.gif>
*/
//End_Html
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TMap.h"
#include "THashTable.h"
#include "TROOT.h"
#include "TClass.h"
#include "TBrowser.h"
#include "TRegexp.h"

ClassImp(TMap)

//______________________________________________________________________________
TMap::TMap(Int_t capacity, Int_t rehashlevel)
{
   // TMap ctor. See THashTable for a description of the arguments.

   fSize  = 0;
   fTable = new THashTable(capacity, rehashlevel);
}

//______________________________________________________________________________
TMap::~TMap()
{
   // TMap dtor. Objects are not deleted unless the TMap is the
   // owner (set via SetOwner()).

   Clear();
   delete fTable;
}

//______________________________________________________________________________
void TMap::Add(TObject *)
{
   // This function may not be used (but we need to provide it since it is
   // a pure virtual in TCollection). Use Add(key,value) instead.

   MayNotUse("Add(TObject *obj)");
}

//______________________________________________________________________________
void TMap::Add(TObject *key, TObject *value)
{
   // Add a (key,value) pair to the map.

   if (IsArgNull("Add", key)) return;

   fTable->Add(new TPair(key, value));
   fSize++;
}

//______________________________________________________________________________
Float_t TMap::AverageCollisions() const
{
   // Return the ratio of entries vs occupied slots.

   return fTable->AverageCollisions();
}

//______________________________________________________________________________
Int_t TMap::Capacity() const
{
   // Return number of slots in the hashtable. Use GetSize() to get the
   // number of objects stored in the TMap.

   return fTable->Capacity();
}

//______________________________________________________________________________
void TMap::Clear(Option_t *option)
{
   // Remove all (key,value) pairs from the map but DO NOT delete the keys
   // and/or values. Key objects are not deleted unless the TMap is the
   // owner (set via SetOwner()). To delete only the value objects call
   // DeleteValues() and to delete both keys and values use DeleteAll().

   if (IsOwner())
      Delete();
   else {
      fTable->Delete(option);    // delete the TPair's
      fSize = 0;
   }
}

//______________________________________________________________________________
Int_t TMap::Collisions(const char *keyname) const
{
   // Returns the number of collisions for a key with a certain name
   // (i.e. number of objects in same slot in the hash table, i.e. length
   // of linked list).

   return fTable->Collisions(keyname);
}

//______________________________________________________________________________
Int_t TMap::Collisions(TObject *key) const
{
   // Returns the number of collisions for a key (i.e. number of objects
   // in same slot in the hash table, i.e. length of linked list).

   return fTable->Collisions(key);
}

//______________________________________________________________________________
void TMap::Delete(Option_t *option)
{
   // Remove all (key,value) pairs from the map AND delete the keys
   // when they are allocated on the heap.

   TIter next(fTable);
   TPair *a;

   while ((a = (TPair *)next()))
      if (a->Key() && a->Key()->IsOnHeap())
         TCollection::GarbageCollect(a->Key());

   fTable->Delete(option);   // delete the TPair's
   fSize = 0;
}

//______________________________________________________________________________
void TMap::DeleteValues()
{
   // Remove all (key,value) pairs from the map AND delete the values
   // when they are allocated on the heap.

   TIter next(fTable);
   TPair *a;

   while ((a = (TPair *)next()))
      if (a->Value() && a->Value()->IsOnHeap())
         TCollection::GarbageCollect(a->Value());

   fTable->Delete();   // delete the TPair's
   fSize = 0;
}

//______________________________________________________________________________
void TMap::DeleteAll()
{
   // Remove all (key,value) pairs from the map AND delete the keys AND
   // values when they are allocated on the heap.

   TIter next(fTable);
   TPair *a;

   while ((a = (TPair *)next())) {
      if (a->Key()   && a->Key()->IsOnHeap())
         TCollection::GarbageCollect(a->Key());
      if (a->Value() && a->Value()->IsOnHeap())
         TCollection::GarbageCollect(a->Value());
   }

   fTable->Delete();   // delete the TPair's
   fSize = 0;
}

//______________________________________________________________________________
TObject *TMap::FindObject(const char *keyname) const
{
   // Check if a (key,value) pair exists with keyname as name of the key.
   // Returns a TPair* (need to downcast from TObject). Use Key() and
   // Value() to get the pointers to the key and value, respectively.
   // Returns 0 if not found.

   return fTable->FindObject(keyname);
}

//______________________________________________________________________________
TObject *TMap::FindObject(const TObject *key) const
{
   // Check if a (key,value) pair exists with key as key.
   // Returns a TPair* (need to downcast from TObject). Use Key() and
   // Value() to get the pointers to the key and value, respectively.
   // Returns 0 if not found.

   if (IsArgNull("FindObject", key)) return 0;

   return fTable->FindObject(key);
}

//______________________________________________________________________________
TObject *TMap::GetValue(const char *keyname) const
{
   // Returns a pointer to the value associated with keyname as name of the key.

   TPair *a = (TPair *)fTable->FindObject(keyname);
   if (a) return a->Value();
   return 0;
}

//______________________________________________________________________________
TObject *TMap::GetValue(const TObject *key) const
{
   // Returns a pointer to the value associated with key.

   if (IsArgNull("GetValue", key)) return 0;

   TPair *a = (TPair *)fTable->FindObject(key);
   if (a) return a->Value();
   return 0;
}

//______________________________________________________________________________
TIterator *TMap::MakeIterator(Bool_t dir) const
{
   // Create an iterator for TMap.

   return new TMapIter(this, dir);
}

//______________________________________________________________________________
void TMap::Print(Option_t *wildcard) const
{
   // Print all objects in this collection.
   // Wildcarding is supported, e.g. wildcard="xxx*" prints only objects
   // with names matching xxx*.

   if (!wildcard) wildcard = "";
   TRegexp re(wildcard, kTRUE);
   Int_t nch = strlen(wildcard);
   TIter next(fTable);
   TPair *a;

   while ((a = (TPair*) next())) {
      TString s = a->Key()->GetName();
      if (nch && s != wildcard && s.Index(re) == kNPOS) continue;
      printf("Key:   ");
      a->Key()->Print();
      if (TStorage::IsOnHeap(a->Value())) {
         printf("Value: ");
         a->Value()->Print();
      } else
         printf("Value: 0x%lx\n", (ULong_t) a->Value());
   }
}

//______________________________________________________________________________
void TMap::Print(Option_t *wildcard, Option_t *option) const
{
   // Print all objects in this collection, passing option to the
   // objects Print() method.
   // Wildcarding is supported, e.g. wildcard="xxx*" prints only objects
   // with names matching xxx*.

   if (!wildcard) wildcard = "";
   TRegexp re(wildcard, kTRUE);
   Int_t nch = strlen(wildcard);
   TIter next(fTable);
   TPair *a;

   while ((a = (TPair*) next())) {
      TString s = a->Key()->GetName();
      if (nch && s != wildcard && s.Index(re) == kNPOS) continue;
      printf("Key:   ");
      a->Key()->Print(option);
      if (TStorage::IsOnHeap(a->Value())) {
         printf("Value: ");
         a->Value()->Print(option);
      } else
         printf("Value: 0x%lx\n", (ULong_t) a->Value());
   }
}

//______________________________________________________________________________
void TMap::Rehash(Int_t newCapacity, Bool_t checkObjValidity)
{
   // Rehash the underlaying THashTable (see THashTable::Rehash()).

   fTable->Rehash(newCapacity, checkObjValidity);
}

//______________________________________________________________________________
TObject *TMap::Remove(TObject *key)
{
   // Remove the (key,value) pair with key from the map. Returns the key
   // object or 0 in case key was not found.

   if (!key) return 0;

   TPair *a;
   if ((a = (TPair *)fTable->FindObject(key))) {
      if (fTable->Remove(key)) {
         TObject *kobj = a->Key();
         delete a;
         fSize--;
         return kobj;
      }
   }
   return 0;
}

//______________________________________________________________________________
void TMap::Streamer(TBuffer &b)
{
   // Stream all key/value pairs in the map to or from the I/O buffer.

   TObject *obj=0;
   UInt_t R__s, R__c;

   if (b.IsReading()) {
      Int_t    nobjects;
      TObject *value=0;

      Version_t v = b.ReadVersion(&R__s, &R__c);
      if (v > 2)
         TObject::Streamer(b);
      if (v > 1)
         fName.Streamer(b);
      b >> nobjects;
      for (Int_t i = 0; i < nobjects; i++) {
         b >> obj;
         b >> value;
         if (obj) Add(obj, value);
      }
      b.CheckByteCount(R__s, R__c,TMap::IsA());
   } else {
      R__c = b.WriteVersion(TMap::IsA(), kTRUE);
      TObject::Streamer(b);
      fName.Streamer(b);
      b << GetSize();
      TIter next(fTable);
      TPair *a;
      while ((a = (TPair*) next())) {
         b << a->Key();
         b << a->Value();
      }
      b.SetByteCount(R__c, kTRUE);
   }
}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TPair                                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void TPair::Browse(TBrowser *b)
{
   if (b) {
      if (fKey)   b->Add(fKey);
      if (fValue) b->Add(fValue);
   } else {
      if (fKey)   fKey->Browse(b);
      if (fValue) fValue->Browse(b);
   }
}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMapIter                                                             //
//                                                                      //
// Iterator of map.                                                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

ClassImp(TMapIter)

//______________________________________________________________________________
TMapIter::TMapIter(const TMap *m, Bool_t dir)
{
   // Create a map iterator. Use dir to specify the desired iteration direction.

   fMap        = m;
   fDirection  = dir;
   fCursor     = 0;
}

//______________________________________________________________________________
TMapIter::TMapIter(const TMapIter &iter) : TIterator(iter)
{
   // Copy ctor.

   fMap       = iter.fMap;
   fDirection = iter.fDirection;
   if (fCursor) {
      fCursor = (THashTableIter *)iter.fCursor->GetCollection()->MakeIterator();
      fCursor->operator=(*iter.fCursor);
   }
}

//______________________________________________________________________________
TIterator &TMapIter::operator=(const TIterator &rhs)
{
   // Overridden assignment operator.

   if (this != &rhs && rhs.IsA() == TMapIter::Class()) {
      const TMapIter &rhs1 = (const TMapIter &)rhs;
      fMap       = rhs1.fMap;
      fDirection = rhs1.fDirection;
      if (rhs1.fCursor) {
         fCursor = (THashTableIter *)rhs1.fCursor->GetCollection()->MakeIterator();
         fCursor->operator=(*rhs1.fCursor);
      }
   }
   return *this;
}

//______________________________________________________________________________
TMapIter &TMapIter::operator=(const TMapIter &rhs)
{
   // Overloaded assignment operator.

   if (this != &rhs) {
      fMap       = rhs.fMap;
      fDirection = rhs.fDirection;
      if (rhs.fCursor) {
         fCursor = (THashTableIter *)rhs.fCursor->GetCollection()->MakeIterator();
         fCursor->operator=(*rhs.fCursor);
      }
   }
   return *this;
}

//______________________________________________________________________________
TMapIter::~TMapIter()
{
   // Map iterator dtor.

   Reset();
}

//______________________________________________________________________________
TObject *TMapIter::Next()
{
   // Returns the next key from a map. Use TMap::GetValue() to get the value
   // associated with the key. Returns 0 when no more items in map.

   if (!fCursor)
      fCursor = new THashTableIter(fMap->fTable, fDirection);

   TPair *a = (TPair *)fCursor->Next();
   if (a) return a->Key();
   return 0;
}

//______________________________________________________________________________
void TMapIter::Reset()
{
   // Reset the map iterator.

   SafeDelete(fCursor);
}
