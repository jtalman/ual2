// @(#)root/base:$Name:  $:$Id: TROOT.h,v 1.42 2005/03/10 17:57:04 rdm Exp $
// Author: Rene Brun   08/12/94

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TROOT
#define ROOT_TROOT


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TROOT                                                                //
//                                                                      //
// The TROOT object is the entry point to the system.                   //
// The single instance of TROOT is accessable via the global gROOT.     //
// Using the gROOT pointer one has access to basically every object     //
// created in a ROOT based program. The TROOT object is essentially a   //
// "dispatcher" with several lists pointing to the ROOT main objects.   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TDirectory
#include "TDirectory.h"
#endif
#ifndef ROOT_TList
#include "TList.h"
#endif

class TClass;
class TColor;
class TDataType;
class TFile;
class TStyle;
class TVirtualPad;
class TVirtualProof;
class TApplication;
class TInterpreter;
class TBrowser;
class TGlobal;
class TFunction;
class TFolder;
class TPluginManager;
class TProcessUUID;
class TClassGenerator;

namespace ROOT {
   class TMapTypeToTClass;
}

class TROOT : public TDirectory {

friend class TCint;

private:
   Int_t           fLineIsProcessing;     //To synchronize multi-threads

   static Int_t    fgDirLevel;            //Indentation level for ls()
   static Bool_t   fgRootInit;            //Singleton initialization flag
   static Bool_t   fgMemCheck;            //Turn on memory leak checker

protected:
   typedef ROOT::TMapTypeToTClass IdMap_t;

   TString         fConfigOptions;        //ROOT ./configure build options
   TString         fVersion;              //ROOT version (from CMZ VERSQQ) ex 0.05/01
   Int_t           fVersionInt;           //ROOT version in integer format (501)
   Int_t           fVersionDate;          //Date of ROOT version (ex 951226)
   Int_t           fVersionTime;          //Time of ROOT version (ex 1152)
   Int_t           fTimer;                //Timer flag
   TApplication    *fApplication;         //Pointer to current application
   TInterpreter    *fInterpreter;         //Command interpreter
   Bool_t          fBatch;                //True if session without graphics
   Bool_t          fEditHistograms;       //True if histograms can be edited with the mouse
   Bool_t          fFromPopUp;            //True if command executed from a popup menu
   Bool_t          fMustClean;            //True if object destructor scans canvases
   Bool_t          fReadingObject;        //True while reading an object
   Bool_t          fForceStyle;           //Force setting of current style when reading objects
   Bool_t          fInterrupt;            //True if macro should be interrupted
   Int_t           fEditorMode;           //Current Editor mode
   TObject         *fPrimitive;           //Currently selected primitive
   TVirtualPad     *fSelectPad;           //Currently selected pad
   TSeqCollection  *fClasses;             //List of classes definition
   IdMap_t         *fIdMap;               //Map from typeid to TClass pointer
   TSeqCollection  *fTypes;               //List of data types definition
   TSeqCollection  *fGlobals;             //List of global variables
   TSeqCollection  *fGlobalFunctions;     //List of global functions
   TSeqCollection  *fFiles;               //List of files
   TSeqCollection  *fMappedFiles;         //List of memory mapped files
   TSeqCollection  *fSockets;             //List of network sockets
   TSeqCollection  *fCanvases;            //List of canvases
   TSeqCollection  *fStyles;              //List of styles
   TSeqCollection  *fFunctions;           //List of analytic functions
   TSeqCollection  *fTasks;               //List of tasks
   TSeqCollection  *fColors;              //List of colors
   TSeqCollection  *fGeometries;          //List of geometries
   TSeqCollection  *fBrowsers;            //List of browsers
   TSeqCollection  *fSpecials;            //List of special objects
   TSeqCollection  *fCleanups;            //List of recursiveRemove collections
   TSeqCollection  *fMessageHandlers;     //List of message handlers
   TSeqCollection  *fStreamerInfo;        //List of active StreamerInfo classes
   TSeqCollection  *fClassGenerators;     //List of user defined class generators;
   TSeqCollection  *fSecContexts;         //List of security contexts (TSecContext)
   TSeqCollection  *fProofs;              //List of proof sessions
   TProcessUUID    *fUUIDs;               //Pointer to TProcessID managing TUUIDs
   TFolder         *fRootFolder;          //top level folder //root
   TList           *fBrowsables;          //List of browsables
   TPluginManager  *fPluginManager;       //Keeps track of plugin library handlers
   TString         fCutClassName;         //Name of default CutG class in graphics editor
   TString         fDefCanvasName;        //Name of default canvas

   static VoidFuncPtr_t fgMakeDefCanvas;  //Pointer to default canvas constructor

                   TROOT();               //Only used by Dictionary
   void            AuthCleanup();         //Cleanup remote auth info
   void            InitSystem();          //Operating System interface
   void            InitThreads();         //Initialize threads library
   TClass         *FindSTLClass(const char *name, Bool_t load) const;
   TClass         *LoadClass(const char *name) const;
   void           *operator new(size_t l) { return TObject::operator new(l); }

public:
                     TROOT(const char *name, const char *title, VoidFuncPtr_t *initfunc = 0);
   virtual           ~TROOT();
   void              AddClass(TClass *cl);
   void              AddClassGenerator(TClassGenerator *gen);
   void              Browse(TBrowser *b);
   Bool_t            ClassSaved(TClass *cl);
   virtual TObject  *FindObject(const char *name) const;
   virtual TObject  *FindObject(const TObject *obj) const;
   virtual TObject  *FindObjectAny(const char *name) const;
   TObject          *FindSpecialObject(const char *name, void *&where);
   const char       *FindObjectClassName(const char *name) const;
   const char       *FindObjectPathName(const TObject *obj) const;
   void              ForceStyle(Bool_t force = kTRUE) { fForceStyle = force; }
   Bool_t            FromPopUp() const { return fFromPopUp; }
   TPluginManager   *GetPluginManager() const { return fPluginManager; }
   TApplication     *GetApplication() const { return fApplication; }
   TInterpreter     *GetInterpreter() const { return fInterpreter; }
   TClass           *GetClass(const char *name, Bool_t load = kTRUE) const;
   TClass           *GetClass(const type_info &typeinfo, Bool_t load = kTRUE) const;
   TColor           *GetColor(Int_t color) const;
   const char       *GetConfigOptions() const { return fConfigOptions; }
   const char       *GetCutClassName() const { return fCutClassName; }
   const char       *GetDefCanvasName() const { return fDefCanvasName; }
   Bool_t            GetEditHistograms() const { return fEditHistograms; }
   Int_t             GetEditorMode() const { return fEditorMode; }
   Bool_t            GetForceStyle() const { return fForceStyle; }
   VoidFuncPtr_t     GetMakeDefCanvas() const;
   Int_t             GetVersionDate() const { return fVersionDate; }
   Int_t             GetVersionTime() const { return fVersionTime; }
   Int_t             GetVersionInt() const { return fVersionInt; }
   const char       *GetVersion() const { return fVersion; }
   TSeqCollection   *GetListOfClasses() const { return fClasses; }
   TSeqCollection   *GetListOfColors() const { return fColors; }
   TSeqCollection   *GetListOfTypes(Bool_t load = kFALSE);
   TSeqCollection   *GetListOfGlobals(Bool_t load = kFALSE);
   TSeqCollection   *GetListOfGlobalFunctions(Bool_t load = kFALSE);
   TSeqCollection   *GetListOfFiles() const       { return fFiles; }
   TSeqCollection   *GetListOfMappedFiles() const { return fMappedFiles; }
   TSeqCollection   *GetListOfSockets() const     { return fSockets; }
   TSeqCollection   *GetListOfCanvases() const    { return fCanvases; }
   TSeqCollection   *GetListOfStyles() const      { return fStyles; }
   TSeqCollection   *GetListOfFunctions() const   { return fFunctions; }
   TSeqCollection   *GetListOfGeometries() const  { return fGeometries; }
   TSeqCollection   *GetListOfBrowsers() const    { return fBrowsers; }
   TSeqCollection   *GetListOfSpecials() const    { return fSpecials; }
   TSeqCollection   *GetListOfTasks() const       { return fTasks; }
   TSeqCollection   *GetListOfCleanups() const    { return fCleanups; }
   TSeqCollection   *GetListOfStreamerInfo() const { return fStreamerInfo; }
   TSeqCollection   *GetListOfMessageHandlers() const { return fMessageHandlers; }
   TSeqCollection   *GetListOfClassGenerators() const { return fClassGenerators; }
   TSeqCollection   *GetListOfSecContexts() const { return fSecContexts; }
   TList            *GetListOfBrowsables() const { return fBrowsables; }
   TDataType        *GetType(const char *name, Bool_t load = kFALSE) const;
   TFile            *GetFile() const { return fFile; }
   TFile            *GetFile(const char *name) const;
   TStyle           *GetStyle(const char *name) const;
   TObject          *GetFunction(const char *name) const;
   TGlobal          *GetGlobal(const char *name, Bool_t load = kFALSE) const;
   TGlobal          *GetGlobal(const TObject *obj, Bool_t load = kFALSE) const;
   TFunction        *GetGlobalFunction(const char *name, const char *params = 0, Bool_t load = kFALSE);
   TFunction        *GetGlobalFunctionWithPrototype(const char *name, const char *proto = 0, Bool_t load = kFALSE);
   TObject          *GetGeometry(const char *name) const;
   TObject          *GetSelectedPrimitive() const { return fPrimitive; }
   TVirtualPad      *GetSelectedPad() const { return fSelectPad; }
   Int_t             GetNclasses() const { return fClasses->GetSize(); }
   Int_t             GetNtypes() const { return fTypes->GetSize(); }
   TFolder          *GetRootFolder() const { return fRootFolder; }
   TProcessUUID     *GetUUIDs() const { return fUUIDs; }
   void              Idle(UInt_t idleTimeInSec, const char *command = 0);
   Int_t             IgnoreInclude(const char *fname, const char *expandedfname);
   Bool_t            IsBatch() const { return fBatch; }
   Bool_t            IsFolder() const { return kTRUE; }
   Bool_t            IsInterrupted() const { return fInterrupt; }
   Bool_t            IsLineProcessing() const { return fLineIsProcessing ? kTRUE : kFALSE; }
   Bool_t            IsProofServ() const { return fName == "proofserv" ? kTRUE : kFALSE; }
   void              ls(Option_t *option = "") const;
   Int_t             LoadClass(const char *classname, const char *libname, Bool_t check = kFALSE);
   Int_t             LoadMacro(const char *filename, Int_t *error = 0, Bool_t check = kFALSE);
   Int_t             Macro(const char *filename, Int_t *error = 0);
   void              Message(Int_t id, const TObject *obj);
   Bool_t            MustClean() const { return fMustClean; }
   void              ProcessLine(const char *line, Int_t *error = 0);
   void              ProcessLineSync(const char *line, Int_t *error = 0);
   Long_t            ProcessLineFast(const char *line, Int_t *error = 0);
   TVirtualProof    *Proof(const char *cluster = "proof://localhost", const char *configfile = 0);
   Bool_t            ReadingObject() const { return fReadingObject; }
   void              RefreshBrowsers();
   void              RemoveClass(TClass *);
   void              Reset(Option_t *option="");
   void              SaveContext();
   void              SetApplication(TApplication *app) { fApplication = app; }
   void              SetBatch(Bool_t batch = kTRUE) { fBatch = batch; }
   void              SetCutClassName(const char *name = "TCutG");
   void              SetDefCanvasName(const char *name = "c1") { fDefCanvasName = name; }
   void              SetEditHistograms(Bool_t flag = kTRUE) { fEditHistograms = flag; }
   void              SetEditorMode(const char *mode = "");
   void              SetFromPopUp(Bool_t flag = kTRUE) { fFromPopUp = flag; }
   void              SetInterrupt(Bool_t flag = kTRUE) { fInterrupt = flag; }
   void              SetLineIsProcessing() { fLineIsProcessing++; }
   void              SetLineHasBeenProcessed() { if (fLineIsProcessing) fLineIsProcessing--; }
   void              SetReadingObject(Bool_t flag = kTRUE) { fReadingObject = flag; }
   void              SetMustClean(Bool_t flag = kTRUE) { fMustClean=flag; }
   void              SetSelectedPrimitive(const TObject *obj) { fPrimitive = (TObject*)obj; }
   void              SetSelectedPad(TVirtualPad *pad) { fSelectPad = pad; }
   void              SetStyle(const char *stylename = "Default");
   void              Time(Int_t casetime=1) { fTimer = casetime; }
   Int_t             Timer() const { return fTimer; }

   //---- static functions
   static Int_t       DecreaseDirLevel();
   static Int_t       GetDirLevel();
   static const char *GetMacroPath();
   static void        SetMacroPath(const char *newpath);
   static Int_t       IncreaseDirLevel();
   static void        IndentLevel();
   static Bool_t      Initialized();
   static Bool_t      MemCheck();
   static void        SetDirLevel(Int_t level = 0);
   static void        SetMakeDefCanvas(VoidFuncPtr_t makecanvas);

   ClassDef(TROOT,0)  //Top level (or root) structure for all classes
};


R__EXTERN TROOT  *gROOT;
namespace ROOT {
   TROOT* GetROOT();
}
#endif

