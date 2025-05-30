//
// File generated by utils/src/rootcint_tmp at Sun May 25 10:25:24 2025.
// Do NOT change. Changes will be lost next time file is generated
//

#include "RConfig.h"
#if !defined(R__ACCESS_IN_SYMBOL)
//Break the privacy of classes -- Disabled for the moment
#define private public
#define protected public
#endif

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

#include "G__X11TTF.h"
#include "TClass.h"
#include "TBuffer.h"
#include "TStreamerInfo.h"
#include "TMemberInspector.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"

#include "TCollectionProxy.h"

namespace ROOT {
   namespace Shadow {
   } // Of namespace ROOT::Shadow
} // Of namespace ROOT

namespace ROOT {
   void TGX11TTF_ShowMembers(void *obj, TMemberInspector &R__insp, char *R__parent);
   static TClass *TGX11TTF_IsA(const void*);
   static void delete_TGX11TTF(void *p);
   static void deleteArray_TGX11TTF(void *p);
   static void destruct_TGX11TTF(void *p);

   // Function generating the singleton type initializer
   TGenericClassInfo *GenerateInitInstance(const ::TGX11TTF*)
   {
      ::TGX11TTF *ptr = 0;
      static ::ROOT::TGenericClassInfo 
         instance("TGX11TTF", ::TGX11TTF::Class_Version(), "x11ttf/inc/TGX11TTF.h", 38,
                  typeid(::TGX11TTF), DefineBehavior(ptr, ptr),
                  &::TGX11TTF::Dictionary, &TGX11TTF_IsA, 0,
                  sizeof(::TGX11TTF) );
      instance.SetDelete(&delete_TGX11TTF);
      instance.SetDeleteArray(&deleteArray_TGX11TTF);
      instance.SetDestructor(&destruct_TGX11TTF);
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstance((const ::TGX11TTF*)0x0); R__UseDummy(_R__UNIQUE_(Init));
}

//______________________________________________________________________________
TClass *TGX11TTF::fgIsA = 0;  // static to hold class pointer

//______________________________________________________________________________
const char *TGX11TTF::Class_Name()
{
   return "TGX11TTF";
}

//______________________________________________________________________________
const char *TGX11TTF::ImplFileName()
{
   return ::ROOT::GenerateInitInstance((const ::TGX11TTF*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TGX11TTF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstance((const ::TGX11TTF*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void TGX11TTF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstance((const ::TGX11TTF*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *TGX11TTF::Class()
{
   if (!fgIsA) fgIsA = ::ROOT::GenerateInitInstance((const ::TGX11TTF*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
void TGX11TTF::Streamer(TBuffer &R__b)
{
   // Stream an object of class TGX11TTF.

   TGX11::Streamer(R__b);
}

//______________________________________________________________________________
void TGX11TTF::ShowMembers(TMemberInspector &R__insp, char *R__parent)
{
      // Inspect the data members of an object of class TGX11TTF.

      TClass *R__cl = ::TGX11TTF::IsA();
      Int_t R__ncp = strlen(R__parent);
      if (R__ncp || R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__parent, "fAlign", (void*)&fAlign);
      ::ROOT::GenericShowMembers("FT_Vector", (void*)&fAlign, R__insp, strcat(R__parent,"fAlign."),false);
      R__parent[R__ncp] = 0;
      TGX11::ShowMembers(R__insp, R__parent);
}

namespace ROOT {
   // Return the actual TClass for the object argument
   static TClass *TGX11TTF_IsA(const void *obj) {
      return ((::TGX11TTF*)obj)->IsA();
   }
   // Wrapper around operator delete
   static void delete_TGX11TTF(void *p) {
      delete ((::TGX11TTF*)p);
   }
   static void deleteArray_TGX11TTF(void *p) {
      delete [] ((::TGX11TTF*)p);
   }
   static void destruct_TGX11TTF(void *p) {
      typedef ::TGX11TTF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TGX11TTF

/********************************************************
* x11ttf/src/G__X11TTF.cxx
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************/

#ifdef G__MEMTEST
#undef malloc
#undef free
#endif

extern "C" void G__cpp_reset_tagtableG__X11TTF();

extern "C" void G__set_cpp_environmentG__X11TTF() {
  G__add_compiledheader("base/inc/TROOT.h");
  G__add_compiledheader("base/inc/TMemberInspector.h");
  G__add_compiledheader("x11ttf/inc/TGX11TTF.h");
  G__cpp_reset_tagtableG__X11TTF();
}
class G__x11ttfdIsrcdIG__X11TTFdOcxx_tag {};

void* operator new(size_t size,G__x11ttfdIsrcdIG__X11TTFdOcxx_tag* p) {
  if(p && G__PVOID!=G__getgvp()) return((void*)p);
#ifndef G__ROOT
  return(malloc(size));
#else
  return(::operator new(size));
#endif
}

/* dummy, for exception */
#ifdef G__EH_DUMMY_DELETE
void operator delete(void *p,G__x11ttfdIsrcdIG__X11TTFdOcxx_tag* x) {
  if((long)p==G__getgvp() && G__PVOID!=G__getgvp()) return;
#ifndef G__ROOT
  free(p);
#else
  ::operator delete(p);
#endif
}
#endif

static void G__operator_delete(void *p) {
  if((long)p==G__getgvp() && G__PVOID!=G__getgvp()) return;
#ifndef G__ROOT
  free(p);
#else
  ::operator delete(p);
#endif
}

void G__DELDMY_x11ttfdIsrcdIG__X11TTFdOcxx() { G__operator_delete(0); }

extern "C" int G__cpp_dllrevG__X11TTF() { return(30051515); }

/*********************************************************
* Member function Interface Method
*********************************************************/

/* TGX11TTF */
static int G__G__X11TTF_178_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   TGX11TTF *p=NULL;
      p = new TGX11TTF(*(TGX11*)libp->para[0].ref);
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTF);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__X11TTF_178_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((TGX11TTF*)(G__getstructoffset()))->DrawText((Int_t)G__int(libp->para[0]),(Int_t)G__int(libp->para[1])
,(Float_t)G__double(libp->para[2]),(Float_t)G__double(libp->para[3])
,(const char*)G__int(libp->para[4]),(TVirtualX::ETextMode)G__int(libp->para[5]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__X11TTF_178_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((TGX11TTF*)(G__getstructoffset()))->SetTextFont((Font_t)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__X11TTF_178_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((TGX11TTF*)(G__getstructoffset()))->SetTextFont((char*)G__int(libp->para[0]),(TVirtualX::ETextSetMode)G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__X11TTF_178_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((TGX11TTF*)(G__getstructoffset()))->SetTextSize((Float_t)G__double(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__X11TTF_178_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)TGX11TTF::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__X11TTF_178_2_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)TGX11TTF::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__X11TTF_178_3_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,115,(long)TGX11TTF::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__X11TTF_178_4_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      TGX11TTF::Dictionary();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__X11TTF_178_5_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((const TGX11TTF*)(G__getstructoffset()))->IsA());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__X11TTF_178_6_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((TGX11TTF*)(G__getstructoffset()))->ShowMembers(*(TMemberInspector*)libp->para[0].ref,(char*)G__int(libp->para[1]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__X11TTF_178_7_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((TGX11TTF*)(G__getstructoffset()))->Streamer(*(TBuffer*)libp->para[0].ref);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__X11TTF_178_8_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((TGX11TTF*)(G__getstructoffset()))->StreamerNVirtual(*(TBuffer*)libp->para[0].ref);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__X11TTF_178_9_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)TGX11TTF::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__X11TTF_178_0_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)TGX11TTF::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__X11TTF_178_1_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)TGX11TTF::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__X11TTF_178_2_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)TGX11TTF::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__G__X11TTF_178_3_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)
{
   TGX11TTF *p;
   void *xtmp = (void*)G__int(libp->para[0]);
   p=new TGX11TTF(*(TGX11TTF*)xtmp);
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTF);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef TGX11TTF G__TTGX11TTF;
static int G__G__X11TTF_178_4_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (TGX11TTF *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         delete (TGX11TTF *)((G__getstructoffset())+sizeof(TGX11TTF)*i);
   else  delete (TGX11TTF *)(G__getstructoffset());
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__G__X11TTF_178_5_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   TGX11TTF *dest = (TGX11TTF*)(G__getstructoffset());
   *dest = (*(TGX11TTF*)libp->para[0].ref);
   const TGX11TTF& obj = *dest;
   result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* Setting up global function */

/*********************************************************
* Member function Stub
*********************************************************/

/* TGX11TTF */

/*********************************************************
* Global function Stub
*********************************************************/

/*********************************************************
* Get size of pointer to member function
*********************************************************/
class G__Sizep2memfuncG__X11TTF {
 public:
  G__Sizep2memfuncG__X11TTF() {p=&G__Sizep2memfuncG__X11TTF::sizep2memfunc;}
    size_t sizep2memfunc() { return(sizeof(p)); }
  private:
    size_t (G__Sizep2memfuncG__X11TTF::*p)();
};

size_t G__get_sizep2memfuncG__X11TTF()
{
  G__Sizep2memfuncG__X11TTF a;
  G__setsizep2memfunc((int)a.sizep2memfunc());
  return((size_t)a.sizep2memfunc());
}


/*********************************************************
* virtual base class offset calculation interface
*********************************************************/

   /* Setting up class inheritance */

/*********************************************************
* Inheritance information setup/
*********************************************************/
extern "C" void G__cpp_setup_inheritanceG__X11TTF() {

   /* Setting up class inheritance */
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTF))) {
     TGX11TTF *G__Lderived;
     G__Lderived=(TGX11TTF*)0x1000;
     {
       TGX11 *G__Lpbase=(TGX11*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTF),G__get_linked_tagnum(&G__G__X11TTFLN_TGX11),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
     {
       TVirtualX *G__Lpbase=(TVirtualX*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTF),G__get_linked_tagnum(&G__G__X11TTFLN_TVirtualX),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
     {
       TNamed *G__Lpbase=(TNamed*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTF),G__get_linked_tagnum(&G__G__X11TTFLN_TNamed),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTF),G__get_linked_tagnum(&G__G__X11TTFLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
     {
       TAttLine *G__Lpbase=(TAttLine*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTF),G__get_linked_tagnum(&G__G__X11TTFLN_TAttLine),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
     {
       TAttFill *G__Lpbase=(TAttFill*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTF),G__get_linked_tagnum(&G__G__X11TTFLN_TAttFill),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
     {
       TAttText *G__Lpbase=(TAttText*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTF),G__get_linked_tagnum(&G__G__X11TTFLN_TAttText),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
     {
       TAttMarker *G__Lpbase=(TAttMarker*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTF),G__get_linked_tagnum(&G__G__X11TTFLN_TAttMarker),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
   }
}

/*********************************************************
* typedef information setup/
*********************************************************/
extern "C" void G__cpp_setup_typetableG__X11TTF() {

   /* Setting up typedef entry */
   G__search_typename2("Int_t",105,-1,0,
-1);
   G__setnewtype(-1,"Signed integer 4 bytes (int)",0);
   G__search_typename2("Float_t",102,-1,0,
-1);
   G__setnewtype(-1,"Float 4 bytes (float)",0);
   G__search_typename2("Version_t",115,-1,0,
-1);
   G__setnewtype(-1,"Class version identifier (short)",0);
   G__search_typename2("vector<TStreamerInfo*>",117,G__get_linked_tagnum(&G__G__X11TTFLN_vectorlETStreamerInfomUcOallocatorlETStreamerInfomUgRsPgR),0,-1);
   G__setnewtype(-1,"// @(#)root/base:$Name:  $:$Id: TROOT.h,v 1.42 2005/03/10 17:57:04 rdm Exp $",0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__G__X11TTFLN_reverse_iteratorlEvectorlETStreamerInfomUcOallocatorlETStreamerInfomUgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__G__X11TTFLN_vectorlETStreamerInfomUcOallocatorlETStreamerInfomUgRsPgR));
   G__setnewtype(-1,"// @(#)root/base:$Name:  $:$Id: TROOT.h,v 1.42 2005/03/10 17:57:04 rdm Exp $",0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__G__X11TTFLN_reverse_iteratorlEvectorlETStreamerInfomUcOallocatorlETStreamerInfomUgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__G__X11TTFLN_vectorlETStreamerInfomUcOallocatorlETStreamerInfomUgRsPgR));
   G__setnewtype(-1,"// @(#)root/base:$Name:  $:$Id: TROOT.h,v 1.42 2005/03/10 17:57:04 rdm Exp $",0);
   G__search_typename2("Font_t",115,-1,0,
-1);
   G__setnewtype(-1,"Font number (short)",0);
}

/*********************************************************
* Data Member information setup/
*********************************************************/

   /* Setting up class,struct,union tag member variable */

   /* TGX11TTF */
static void G__setup_memvarTGX11TTF(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTF));
   { TGX11TTF *p; p=(TGX11TTF*)0x1000; if (p) { }
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTFcLcLEAlign),-1,-2,1,"kNone=0",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTFcLcLEAlign),-1,-2,1,"kTLeft=1",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTFcLcLEAlign),-1,-2,1,"kTCenter=2",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTFcLcLEAlign),-1,-2,1,"kTRight=3",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTFcLcLEAlign),-1,-2,1,"kMLeft=4",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTFcLcLEAlign),-1,-2,1,"kMCenter=5",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTFcLcLEAlign),-1,-2,1,"kMRight=6",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTFcLcLEAlign),-1,-2,1,"kBLeft=7",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTFcLcLEAlign),-1,-2,1,"kBCenter=8",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTFcLcLEAlign),-1,-2,1,"kBRight=9",0,(char*)NULL);
   G__memvar_setup((void*)NULL,117,0,0,G__get_linked_tagnum(&G__G__X11TTFLN_FT_Vector),-1,-1,4,"fAlign=",0,"alignment vector");
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__G__X11TTFLN_TClass),-1,-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}

extern "C" void G__cpp_setup_memvarG__X11TTF() {
}
/***********************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
***********************************************************/

/*********************************************************
* Member function information setup for each class
*********************************************************/
static void G__setup_memfuncTGX11TTF(void) {
   /* TGX11TTF */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTF));
   G__memfunc_setup("Align",491,(G__InterfaceMethod)NULL,121,-1,-1,0,0,1,4,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DrawImage",881,(G__InterfaceMethod)NULL,121,-1,-1,0,6,1,4,0,
"U 'FT_Bitmap' - 0 - source k - 'ULong_t' 0 - fore "
"k - 'ULong_t' 0 - back U 'XImage' - 0 - xim "
"i - 'Int_t' 0 - bx i - 'Int_t' 0 - by",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsVisible",906,(G__InterfaceMethod)NULL,103,-1,G__defined_typename("Bool_t"),0,4,1,4,0,
"i - 'Int_t' 0 - x i - 'Int_t' 0 - y "
"h - 'UInt_t' 0 - w h - 'UInt_t' 0 - h",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("GetBackground",1312,(G__InterfaceMethod)NULL,85,G__get_linked_tagnum(&G__G__X11TTFLN_XImage),-1,0,4,1,4,0,
"i - 'Int_t' 0 - x i - 'Int_t' 0 - y "
"h - 'UInt_t' 0 - w h - 'UInt_t' 0 - h",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("RenderString",1239,(G__InterfaceMethod)NULL,121,-1,-1,0,3,1,4,0,
"i - 'Int_t' 0 - x i - 'Int_t' 0 - y "
"i 'TVirtualX::ETextMode' - 0 - mode",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("TGX11TTF",579,G__G__X11TTF_178_6_0,105,G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTF),-1,0,1,1,1,0,"u 'TGX11' - 11 - org",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DrawText",819,G__G__X11TTF_178_7_0,121,-1,-1,0,6,1,1,0,
"i - 'Int_t' 0 - x i - 'Int_t' 0 - y "
"f - 'Float_t' 0 - angle f - 'Float_t' 0 - mgn "
"C - - 10 - text i 'TVirtualX::ETextMode' - 0 - mode",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("SetTextFont",1128,G__G__X11TTF_178_8_0,121,-1,-1,0,1,1,1,0,"s - 'Font_t' 0 - fontnumber",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("SetTextFont",1128,G__G__X11TTF_178_9_0,105,-1,G__defined_typename("Int_t"),0,2,1,1,0,
"C - - 0 - fontname i 'TVirtualX::ETextSetMode' - 0 - mode",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("SetTextSize",1132,G__G__X11TTF_178_0_1,121,-1,-1,0,1,1,1,0,"f - 'Float_t' 0 - textsize",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("Class",502,G__G__X11TTF_178_1_1,85,G__get_linked_tagnum(&G__G__X11TTFLN_TClass),-1,0,0,3,1,0,"",(char*)NULL,(void*)(TClass* (*)())(&TGX11TTF::Class),0);
   G__memfunc_setup("Class_Name",982,G__G__X11TTF_178_2_1,67,-1,-1,0,0,3,1,1,"",(char*)NULL,(void*)(const char* (*)())(&TGX11TTF::Class_Name),0);
   G__memfunc_setup("Class_Version",1339,G__G__X11TTF_178_3_1,115,-1,G__defined_typename("Version_t"),0,0,3,1,0,"",(char*)NULL,(void*)(Version_t (*)())(&TGX11TTF::Class_Version),0);
   G__memfunc_setup("Dictionary",1046,G__G__X11TTF_178_4_1,121,-1,-1,0,0,3,1,0,"",(char*)NULL,(void*)(void (*)())(&TGX11TTF::Dictionary),0);
   G__memfunc_setup("IsA",253,G__G__X11TTF_178_5_1,85,G__get_linked_tagnum(&G__G__X11TTFLN_TClass),-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("ShowMembers",1132,G__G__X11TTF_178_6_1,121,-1,-1,0,2,1,1,0,
"u 'TMemberInspector' - 1 - insp C - - 0 - parent",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("Streamer",835,G__G__X11TTF_178_7_1,121,-1,-1,0,1,1,1,0,"u 'TBuffer' - 1 - b",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("StreamerNVirtual",1656,G__G__X11TTF_178_8_1,121,-1,-1,0,1,1,1,0,"u 'TBuffer' - 1 - b",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DeclFileName",1145,G__G__X11TTF_178_9_1,67,-1,-1,0,0,3,1,1,"",(char*)NULL,(void*)(const char* (*)())(&TGX11TTF::DeclFileName),0);
   G__memfunc_setup("ImplFileLine",1178,G__G__X11TTF_178_0_2,105,-1,-1,0,0,3,1,0,"",(char*)NULL,(void*)(int (*)())(&TGX11TTF::ImplFileLine),0);
   G__memfunc_setup("ImplFileName",1171,G__G__X11TTF_178_1_2,67,-1,-1,0,0,3,1,1,"",(char*)NULL,(void*)(const char* (*)())(&TGX11TTF::ImplFileName),0);
   G__memfunc_setup("DeclFileLine",1152,G__G__X11TTF_178_2_2,105,-1,-1,0,0,3,1,0,"",(char*)NULL,(void*)(int (*)())(&TGX11TTF::DeclFileLine),0);
   // automatic copy constructor
   G__memfunc_setup("TGX11TTF",579,G__G__X11TTF_178_3_2,(int)('i'),G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTF),-1,0,1,1,1,0,"u 'TGX11TTF' - 11 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~TGX11TTF",705,G__G__X11TTF_178_4_2,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   // automatic assignment operator
   G__memfunc_setup("operator=",937,G__G__X11TTF_178_5_2,(int)('u'),G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTF),-1,1,1,1,1,0,"u 'TGX11TTF' - 11 - -",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}


/*********************************************************
* Member function information setup
*********************************************************/
extern "C" void G__cpp_setup_memfuncG__X11TTF() {
}

/*********************************************************
* Global variable information setup for each class
*********************************************************/
static void G__cpp_setup_global0() {

   /* Setting up global variables */
   G__resetplocal();

}

static void G__cpp_setup_global1() {
}

static void G__cpp_setup_global2() {
}

static void G__cpp_setup_global3() {

   G__resetglobalenv();
}
extern "C" void G__cpp_setup_globalG__X11TTF() {
  G__cpp_setup_global0();
  G__cpp_setup_global1();
  G__cpp_setup_global2();
  G__cpp_setup_global3();
}

/*********************************************************
* Global function information setup for each class
*********************************************************/
static void G__cpp_setup_func0() {
   G__lastifuncposition();

}

static void G__cpp_setup_func1() {
}

static void G__cpp_setup_func2() {
}

static void G__cpp_setup_func3() {

   G__resetifuncposition();
}

extern "C" void G__cpp_setup_funcG__X11TTF() {
  G__cpp_setup_func0();
  G__cpp_setup_func1();
  G__cpp_setup_func2();
  G__cpp_setup_func3();
}

/*********************************************************
* Class,struct,union,enum tag information setup
*********************************************************/
/* Setup class/struct taginfo */
G__linked_taginfo G__G__X11TTFLN_TClass = { "TClass" , 99 , -1 };
G__linked_taginfo G__G__X11TTFLN_TBuffer = { "TBuffer" , 99 , -1 };
G__linked_taginfo G__G__X11TTFLN_TMemberInspector = { "TMemberInspector" , 99 , -1 };
G__linked_taginfo G__G__X11TTFLN_TObject = { "TObject" , 99 , -1 };
G__linked_taginfo G__G__X11TTFLN_TNamed = { "TNamed" , 99 , -1 };
G__linked_taginfo G__G__X11TTFLN_vectorlETStreamerInfomUcOallocatorlETStreamerInfomUgRsPgR = { "vector<TStreamerInfo*,allocator<TStreamerInfo*> >" , 99 , -1 };
G__linked_taginfo G__G__X11TTFLN_reverse_iteratorlEvectorlETStreamerInfomUcOallocatorlETStreamerInfomUgRsPgRcLcLiteratorgR = { "reverse_iterator<vector<TStreamerInfo*,allocator<TStreamerInfo*> >::iterator>" , 99 , -1 };
G__linked_taginfo G__G__X11TTFLN_TAttLine = { "TAttLine" , 99 , -1 };
G__linked_taginfo G__G__X11TTFLN_TAttFill = { "TAttFill" , 99 , -1 };
G__linked_taginfo G__G__X11TTFLN_TAttText = { "TAttText" , 99 , -1 };
G__linked_taginfo G__G__X11TTFLN_TAttMarker = { "TAttMarker" , 99 , -1 };
G__linked_taginfo G__G__X11TTFLN_TVirtualX = { "TVirtualX" , 99 , -1 };
G__linked_taginfo G__G__X11TTFLN_TVirtualXcLcLETextMode = { "TVirtualX::ETextMode" , 101 , -1 };
G__linked_taginfo G__G__X11TTFLN_TVirtualXcLcLETextSetMode = { "TVirtualX::ETextSetMode" , 101 , -1 };
G__linked_taginfo G__G__X11TTFLN_XImage = { "XImage" , 115 , -1 };
G__linked_taginfo G__G__X11TTFLN_TGX11 = { "TGX11" , 99 , -1 };
G__linked_taginfo G__G__X11TTFLN_FT_Vector = { "FT_Vector" , 115 , -1 };
G__linked_taginfo G__G__X11TTFLN_FT_Bitmap = { "FT_Bitmap" , 115 , -1 };
G__linked_taginfo G__G__X11TTFLN_TGX11TTF = { "TGX11TTF" , 99 , -1 };
G__linked_taginfo G__G__X11TTFLN_TGX11TTFcLcLEAlign = { "TGX11TTF::EAlign" , 101 , -1 };

/* Reset class/struct taginfo */
extern "C" void G__cpp_reset_tagtableG__X11TTF() {
  G__G__X11TTFLN_TClass.tagnum = -1 ;
  G__G__X11TTFLN_TBuffer.tagnum = -1 ;
  G__G__X11TTFLN_TMemberInspector.tagnum = -1 ;
  G__G__X11TTFLN_TObject.tagnum = -1 ;
  G__G__X11TTFLN_TNamed.tagnum = -1 ;
  G__G__X11TTFLN_vectorlETStreamerInfomUcOallocatorlETStreamerInfomUgRsPgR.tagnum = -1 ;
  G__G__X11TTFLN_reverse_iteratorlEvectorlETStreamerInfomUcOallocatorlETStreamerInfomUgRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__G__X11TTFLN_TAttLine.tagnum = -1 ;
  G__G__X11TTFLN_TAttFill.tagnum = -1 ;
  G__G__X11TTFLN_TAttText.tagnum = -1 ;
  G__G__X11TTFLN_TAttMarker.tagnum = -1 ;
  G__G__X11TTFLN_TVirtualX.tagnum = -1 ;
  G__G__X11TTFLN_TVirtualXcLcLETextMode.tagnum = -1 ;
  G__G__X11TTFLN_TVirtualXcLcLETextSetMode.tagnum = -1 ;
  G__G__X11TTFLN_XImage.tagnum = -1 ;
  G__G__X11TTFLN_TGX11.tagnum = -1 ;
  G__G__X11TTFLN_FT_Vector.tagnum = -1 ;
  G__G__X11TTFLN_FT_Bitmap.tagnum = -1 ;
  G__G__X11TTFLN_TGX11TTF.tagnum = -1 ;
  G__G__X11TTFLN_TGX11TTFcLcLEAlign.tagnum = -1 ;
}


extern "C" void G__cpp_setup_tagtableG__X11TTF() {

   /* Setting up class,struct,union tag entry */
   G__get_linked_tagnum(&G__G__X11TTFLN_TClass);
   G__get_linked_tagnum(&G__G__X11TTFLN_TBuffer);
   G__get_linked_tagnum(&G__G__X11TTFLN_TMemberInspector);
   G__get_linked_tagnum(&G__G__X11TTFLN_TObject);
   G__get_linked_tagnum(&G__G__X11TTFLN_TNamed);
   G__get_linked_tagnum(&G__G__X11TTFLN_vectorlETStreamerInfomUcOallocatorlETStreamerInfomUgRsPgR);
   G__get_linked_tagnum(&G__G__X11TTFLN_reverse_iteratorlEvectorlETStreamerInfomUcOallocatorlETStreamerInfomUgRsPgRcLcLiteratorgR);
   G__get_linked_tagnum(&G__G__X11TTFLN_TAttLine);
   G__get_linked_tagnum(&G__G__X11TTFLN_TAttFill);
   G__get_linked_tagnum(&G__G__X11TTFLN_TAttText);
   G__get_linked_tagnum(&G__G__X11TTFLN_TAttMarker);
   G__get_linked_tagnum(&G__G__X11TTFLN_TVirtualX);
   G__get_linked_tagnum(&G__G__X11TTFLN_TVirtualXcLcLETextMode);
   G__get_linked_tagnum(&G__G__X11TTFLN_TVirtualXcLcLETextSetMode);
   G__get_linked_tagnum(&G__G__X11TTFLN_XImage);
   G__get_linked_tagnum(&G__G__X11TTFLN_TGX11);
   G__get_linked_tagnum(&G__G__X11TTFLN_FT_Vector);
   G__get_linked_tagnum(&G__G__X11TTFLN_FT_Bitmap);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTF),sizeof(TGX11TTF),-1,62464,"Interface to X11 + TTF font handling",G__setup_memvarTGX11TTF,G__setup_memfuncTGX11TTF);
   G__get_linked_tagnum(&G__G__X11TTFLN_TGX11TTFcLcLEAlign);
}
extern "C" void G__cpp_setupG__X11TTF(void) {
  G__check_setup_version(30051515,"G__cpp_setupG__X11TTF()");
  G__set_cpp_environmentG__X11TTF();
  G__cpp_setup_tagtableG__X11TTF();

  G__cpp_setup_inheritanceG__X11TTF();

  G__cpp_setup_typetableG__X11TTF();

  G__cpp_setup_memvarG__X11TTF();

  G__cpp_setup_memfuncG__X11TTF();
  G__cpp_setup_globalG__X11TTF();
  G__cpp_setup_funcG__X11TTF();

   if(0==G__getsizep2memfunc()) G__get_sizep2memfuncG__X11TTF();
  return;
}
class G__cpp_setup_initG__X11TTF {
  public:
    G__cpp_setup_initG__X11TTF() { G__add_setup_func("G__X11TTF",(G__incsetup)(&G__cpp_setupG__X11TTF)); G__call_setup_funcs(); }
   ~G__cpp_setup_initG__X11TTF() { G__remove_setup_func("G__X11TTF"); }
};
G__cpp_setup_initG__X11TTF G__cpp_setup_initializerG__X11TTF;

