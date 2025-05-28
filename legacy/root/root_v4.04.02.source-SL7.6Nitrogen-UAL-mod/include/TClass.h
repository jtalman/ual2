// @(#)root/meta:$Name:  $:$Id: TClass.h,v 1.49 2005/03/20 21:25:12 brun Exp $
// Author: Rene Brun   07/01/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TClass
#define ROOT_TClass


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TClass                                                               //
//                                                                      //
// Dictionary of a class.                                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TDictionary
#include "TDictionary.h"
#endif
#ifndef ROOT_TString
#include "TString.h"
#endif
#ifndef ROOT_TROOT
#include "TROOT.h"
#endif
#ifndef ROOT_TObjArray
#include "TObjArray.h"
#endif
#ifndef ROOT_TStreamerInfo
#include "TStreamerInfo.h"
#endif
#include <list>

class TBaseClass;
class TBrowser;
class TDataMember;
class TClassRef;
class TMethod;
class TRealData;
class TCint;
class TBuffer;
class G__ClassInfo;
class TVirtualCollectionProxy;
class TMethodCall;

namespace ROOT { class TGenericClassInfo; }

class TClass : public TDictionary {

friend class TCint;
friend void ROOT::ResetClassVersion(TClass*, const char*, Short_t);
friend class ROOT::TGenericClassInfo;

public:
   // TClass status bits
   enum { kClassSaved  = BIT(12), kIgnoreTObjectStreamer = BIT(13),
          kUnloaded    = BIT(15), kIsTObject = BIT(16),
          kIsForeign   = BIT(17), kIsEmulation = BIT(18),
          kStartWithTObject = BIT(19)  // see comments for IsStartingWithTObject()
   };
   enum ENewType { kRealNew = 0, kClassNew, kDummyNew };

private:
   TObjArray         *fStreamerInfo;    //Array of TStreamerInfo
   TList             *fRealData;        //linked list for persistent members including base classes
   TList             *fBase;            //linked list for base classes
   TList             *fData;            //linked list for data members
   TList             *fMethod;          //linked list for methods
   TList             *fAllPubData;      //all public data members (including from base classes)
   TList             *fAllPubMethod;    //all public methods (including from base classes)
   const char        *fDeclFileName;    //name of class declaration file
   const char        *fImplFileName;    //name of class implementation file
   Short_t            fDeclFileLine;    //line of class declaration
   Short_t            fImplFileLine;    //line of class implementation
   UInt_t             fInstanceCount;   //number of instances of this class
   UInt_t             fOnHeap;          //number of instances on heap
   UInt_t             fCheckSum;        //checksum of data members and base classes
   TVirtualCollectionProxy *fCollectionProxy; //Collection interface
   Version_t          fClassVersion;    //Class version Identifier
   G__ClassInfo      *fClassInfo;       //pointer to CINT class info class
   TList             *fClassMenuList;   //list of class menu items
   TList             *fClassEditors;    //list of class editors
   const type_info   *fTypeInfo;        //pointer to the C++ type information.
   ShowMembersFunc_t  fShowMembers;     //pointer to the class's ShowMembers function
   TClassStreamer    *fStreamer;        //pointer to streamer function
   TString            fSharedLibs;      //shared libraries containing class code

   IsAFunc_t          fIsA;             //pointer to the class's IsA function.
   IsAGlobalFunc_t    fGlobalIsA;       //pointer to a global IsA function.
   TMethodCall       *fIsAMethod;       //!saved info to call a IsA member function

   ROOT::NewFunc_t    fNew;             //pointer to a function newing one object.
   ROOT::NewArrFunc_t fNewArray;        //pointer to a function newing an array of objects.
   ROOT::DelFunc_t    fDelete;          //pointer to a function deleting one object.
   ROOT::DelArrFunc_t fDeleteArray;     //pointer to a function deleting an array of objects.
   ROOT::DesFunc_t    fDestructor;      //pointer to a function call an object's destructor.
   Int_t              fSizeof;          //Sizeof the class.

   Bool_t             fVersionUsed;     //!Indicates whether GetClassVersion has been called
   Long_t             fProperty;        //!Property

   void              *fInterStreamer;   //!saved info to call Streamer
   Long_t             fOffsetStreamer;  //!saved info to call Streamer
   Int_t              fStreamerType;    //!cached of the streaming method to use
   TStreamerInfo     *fCurrentInfo;     //!cached current streamer info.
#if !defined(__CINT__)
   std::list<TClassRef*> *fRefs;        //!List of references to this object
#endif

   TMethod           *GetClassMethod(Long_t faddr);
   TMethod           *GetClassMethod(const char *name, const char *signature);
   TStreamerInfo     *GetCurrentStreamerInfo() {
      if (fCurrentInfo) return fCurrentInfo;
      else return (fCurrentInfo=(TStreamerInfo*)(fStreamerInfo->At(fClassVersion)));
   }
   void Init(const char *name, Version_t cversion, const type_info *info,
             IsAFunc_t isa, ShowMembersFunc_t showmember,
             const char *dfil, const char *ifil,
             Int_t dl, Int_t il);

   void               SetClassVersion(Version_t version) { fClassVersion = version; fCurrentInfo = 0; }
   void               SetClassSize(Int_t sizof) { fSizeof = sizof; }

   static ENewType    fgCallingNew;     //Intent of why/how TClass::New() is called
   static Int_t       fgClassCount;     //provides unique id for a each class
                                        //stored in TObject::fUniqueID
   // Internal status bits
   enum { kLoading = BIT(14) };
   // Internal streamer type.
   enum {kDefault=0, kEmulated=1, kTObject=2, kInstrumented=4, kForeign=8, kExternal=16};

public:
   TClass();
   TClass(const char *name);
   TClass(const char *name, Version_t cversion,
          const char *dfil = 0, const char *ifil = 0,
          Int_t dl = 0, Int_t il = 0);
   TClass(const char *name, Version_t cversion,
          const type_info &info, IsAFunc_t isa,
          ShowMembersFunc_t showmember,
          const char *dfil, const char *ifil,
          Int_t dl, Int_t il);
   virtual           ~TClass();

   void               AddInstance(Bool_t heap = kFALSE) { fInstanceCount++; if (heap) fOnHeap++; }
   void               AddImplFile(const char *filename, int line);
   void               AddRef(TClassRef *ref); 
   virtual void       Browse(TBrowser *b);
   void               BuildRealData(void *pointer=0);
   void               BuildEmulatedRealData(const char *name, Int_t offset, TClass *cl);
   Bool_t             CanSplit() const;
   Bool_t             CanIgnoreTObjectStreamer() { return TestBit(kIgnoreTObjectStreamer);}
   void               CopyCollectionProxy(const TVirtualCollectionProxy&);
   void               Draw(Option_t *option="");
   void               Dump() const { TDictionary::Dump(); }
   void               Dump(void *obj) const;
   char              *EscapeChars(const char *text) const;
   Bool_t             HasDefaultConstructor() const;
   UInt_t             GetCheckSum(UInt_t code=0) const;
   TVirtualCollectionProxy *GetCollectionProxy() const;
   Version_t          GetClassVersion() const { ((TClass*)this)->fVersionUsed = kTRUE; return fClassVersion; }
   TDataMember       *GetDataMember(const char *datamember) const;
   Int_t              GetDataMemberOffset(const char *membername) const;
   const char        *GetDeclFileName() const { return fDeclFileName; }
   Short_t            GetDeclFileLine() const { return fDeclFileLine; }
   ROOT::DelFunc_t    GetDelete() const;
   ROOT::DesFunc_t    GetDestructor() const;
   ROOT::DelArrFunc_t GetDeleteArray() const;
   G__ClassInfo      *GetClassInfo() const { return fClassInfo; }
   TList             *GetListOfDataMembers();
   TList             *GetListOfBases();
   TList             *GetListOfMethods();
   TList             *GetListOfRealData() const { return fRealData; }
   TList             *GetListOfAllPublicMethods();
   TList             *GetListOfAllPublicDataMembers();
   const char        *GetImplFileName() const { return fImplFileName; }
   Short_t            GetImplFileLine() const { return fImplFileLine; }
   TClass            *GetActualClass(const void *object) const;
   TClass            *GetBaseClass(const char *classname);
   TClass            *GetBaseClass(const TClass *base);
   Int_t              GetBaseClassOffset(const TClass *base);
   TClass            *GetBaseDataMember(const char *datamember);
   UInt_t             GetInstanceCount() const { return fInstanceCount; }
   UInt_t             GetHeapInstanceCount() const { return fOnHeap; }
   void               GetMenuItems(TList *listitems);
   TList             *GetMenuList() const { return fClassMenuList; }
   TList             *GetEditorList() const { return fClassEditors; }
   TMethod           *GetMethod(const char *method, const char *params);
   TMethod           *GetMethodWithPrototype(const char *method, const char *proto);
   TMethod           *GetMethodAny(const char *method);
   TMethod           *GetMethodAllAny(const char *method);
   Int_t              GetNdata();
   ROOT::NewFunc_t    GetNew() const;
   ROOT::NewArrFunc_t GetNewArray() const;
   Int_t              GetNmethods();
   TRealData         *GetRealData(const char *name) const;
   const char        *GetSharedLibs();
   ShowMembersFunc_t  GetShowMembersWrapper() const { return fShowMembers; }
   TClassStreamer    *GetStreamer() const { return fStreamer; }
   TObjArray         *GetStreamerInfos() const { return fStreamerInfo; }
   TStreamerInfo     *GetStreamerInfo(Int_t version=0);
   const type_info   *GetTypeInfo() const { return fTypeInfo; };
   void               IgnoreTObjectStreamer(Bool_t ignore=kTRUE);
   Bool_t             InheritsFrom(const char *cl) const;
   Bool_t             InheritsFrom(const TClass *cl) const;
   Bool_t             IsFolder() const { return kTRUE; }
   Bool_t             IsLoaded() const;
   Bool_t             IsForeign() const;
   Bool_t             IsStartingWithTObject() const;
   Bool_t             IsTObject() const;
   void               MakeCustomMenuList();
   void              *New(ENewType defConstructor = kClassNew);
   void              *New(void *arena, ENewType defConstructor = kClassNew);
   Long_t             Property() const;
   Int_t              ReadBuffer(TBuffer &b, void *pointer, Int_t version, UInt_t start, UInt_t count);
   Int_t              ReadBuffer(TBuffer &b, void *pointer);
   void               RemoveRef(TClassRef *ref); 
   void               ReplaceWith(TClass *newcl, Bool_t recurse = kTRUE) const;
   void               ResetClassInfo(Long_t tagnum);
   void               ResetInstanceCount() { fInstanceCount = fOnHeap = 0; }
   void               ResetMenuList();
   Int_t              Size() const;
   void               SetGlobalIsA(IsAGlobalFunc_t);
   void               SetDelete(ROOT::DelFunc_t deleteFunc);
   void               SetDeleteArray(ROOT::DelArrFunc_t deleteArrayFunc);
   void               SetDestructor(ROOT::DesFunc_t destructorFunc);
   void               SetNew(ROOT::NewFunc_t newFunc);
   void               SetNewArray(ROOT::NewArrFunc_t newArrayFunc);
   TStreamerInfo     *SetStreamerInfo(Int_t version, const char *info="");
   void               SetUnloaded();
   Int_t              WriteBuffer(TBuffer &b, void *pointer, const char *info="");

   void               AdoptStreamer(TClassStreamer *strm);
   void               AdoptMemberStreamer(const char *name, TMemberStreamer *strm);
   void               SetMemberStreamer(const char *name, MemberStreamerFunc_t strm);

   // Function to retrieve the TClass object and dictionary function
   static TClass        *GetClass(const char *name, Bool_t load = kTRUE);
   static TClass        *GetClass(const type_info &typeinfo, Bool_t load = kTRUE);
   static VoidFuncPtr_t  GetDict (const char *cname);
   static VoidFuncPtr_t  GetDict (const type_info &info);

   static Int_t       AutoBrowse(TObject *obj, TBrowser *browser);
   static ENewType    IsCallingNew();
   static TClass     *Load(TBuffer &b);
   void               Store(TBuffer &b) const;

   // Pseudo-method apply to the 'obj'. In particular those are used to
   // implement TObject like methods for non-TObject classes

   Int_t              Browse(void *obj, TBrowser *b) const;
   void               Destructor(void *obj, Bool_t dtorOnly = kFALSE);
   void              *DynamicCast(const TClass *base, void *obj, Bool_t up = kTRUE);
   Bool_t             IsFolder(void *obj) const;
   void               Streamer(void *obj, TBuffer &b);

   ClassDef(TClass,0)  //Dictionary containing class information
};

namespace ROOT {

   #ifndef R__NO_CLASS_TEMPLATE_SPECIALIZATION
      template <typename T> struct IsPointer { enum { val = 0 }; };
      template <typename T> struct IsPointer<T*> { enum { val = 1 }; };
   #else
      template <typename T> bool IsPointer(const T* /* dummy */) { return false; };
      template <typename T> bool IsPointer(const T** /* dummy */) { return true; };
   #endif

   template <typename T> TClass* GetClass(      T* /* dummy */)        { return GetROOT()->GetClass(typeid(T)); }
   template <typename T> TClass* GetClass(const T* /* dummy */)        { return GetROOT()->GetClass(typeid(T)); }

   #ifndef R__NO_CLASS_TEMPLATE_SPECIALIZATION
      // This can only be used when the template overload resolution can distringuish between
      // T* and T**
      template <typename T> TClass* GetClass(      T**       /* dummy */) { return GetClass((T*)0); }
      template <typename T> TClass* GetClass(const T**       /* dummy */) { return GetClass((T*)0); }
      template <typename T> TClass* GetClass(      T* const* /* dummy */) { return GetClass((T*)0); }
      template <typename T> TClass* GetClass(const T* const* /* dummy */) { return GetClass((T*)0); }
   #endif

   extern TClass *CreateClass(const char *cname, Version_t id,
                              const char *dfil, const char *ifil,
                              Int_t dl, Int_t il);
}

#endif
