// @(#)root/treeplayer:$Name:  $:$Id: TTreeFormula.h,v 1.45 2005/04/19 19:39:58 brun Exp $
// Author: Rene Brun   19/01/96

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/
// ---------------------------------- TreeFormula.h

#ifndef ROOT_TTreeFormula
#define ROOT_TTreeFormula



//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTreeFormula                                                         //
//                                                                      //
// The Tree formula class                                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TFormula
#include "TFormula.h"
#endif

#ifndef ROOT_TLeaf
#include "TLeaf.h"
#endif

#ifndef ROOT_TObjArray
#include "TObjArray.h"
#endif

#include <string>
#include <vector>

#ifdef R__OLDHPACC
namespace std {
   using ::string;
   using ::vector;
}
#endif

const Int_t kMAXCODES = kMAXFOUND; // must be the same as kMAXFOUND in TFormula
const Int_t kMAXFORMDIM = 5; // Maximum number of array dimensions support in TTreeFormula

class TTree;
class TArrayI;
class TMethodCall;
class TLeafObject;
class TDataMember;
class TStreamerElement;
class TFormLeafInfoMultiVarDim;
class TFormLeafInfo;
class TBranchElement;
class TAxis;
class TTreeFormulaManager;

class TTreeFormula : public TFormula {

protected:
   enum { kIsCharacter = BIT(12),
          kMissingLeaf = BIT(15) // true if some of the needed leaves are missing in the current TTree
        };
   enum { kDirect, kDataMember, kMethod, 
          kIndexOfEntry, kEntries, kLength, kIteration, kLengthFunc, kSum };
   enum { kAlias           = 200,
          kAliasString     = 201,
          kAlternate       = 202,
          kAlternateString = 203
   };

   TTree       *fTree;            //! pointer to Tree
   Short_t     fCodes[kMAXCODES]; //  List of leaf numbers referenced in formula
   Int_t       fNdata[kMAXCODES]; //! This caches the physical number of element in the leaf or datamember.
   Int_t       fNcodes;           //  Number of leaves referenced in formula
   Bool_t      fHasCast;          //  Record whether the formula contain a cast operation or not
   Int_t       fMultiplicity;     //  Indicator of the variability of the formula
   Int_t       fNindex;           //  Size of fIndex
   Int_t      *fLookupType;       //[fNindex] array indicating how each leaf should be looked-up
   TObjArray   fLeaves;           //!  List of leaf used in this formula.
   TObjArray   fDataMembers;      //!  List of leaf data members
   TObjArray   fMethods;          //!  List of leaf method calls
   TObjArray   fAliases;          //!  List of TTreeFormula for each alias used.
   TObjArray   fLeafNames;        //   List of TNamed describing leaves
   TObjArray   fBranches;         //!  List of branches to read.  Similar to fLeaces but duplicates are zeroed out.
   Bool_t      fQuickLoad;        //!  If true, branch GetEntry is only called when the entry number changes.
   

   Int_t         fNdimensions[kMAXCODES];              //Number of array dimensions in each leaf
   Int_t         fFixedSizes[kMAXCODES][kMAXFORMDIM];  //Physical sizes of lower dimensions for each leaf
   UChar_t       fHasMultipleVarDim[kMAXCODES];        //True if the corresponding variable is an array with more than one variable dimension.

   //the next line should have a mutable in front. See GetNdata()
   Int_t         fCumulSizes[kMAXCODES][kMAXFORMDIM];  //Accumulated sizes of lower dimensions for each leaf after variable dimensions has been calculated
   Int_t         fIndexes[kMAXCODES][kMAXFORMDIM];     //Index of array selected by user for each leaf
   TTreeFormula *fVarIndexes[kMAXCODES][kMAXFORMDIM];  //Pointer to a variable index.

   virtual Double_t   GetValueFromMethod(Int_t i, TLeaf *leaf) const;
   virtual void*      GetValuePointerFromMethod(Int_t i, TLeaf *leaf) const;
   Int_t       GetRealInstance(Int_t instance, Int_t codeindex);

   // Helper members and function used during the construction and parsing
   TList                    *fDimensionSetup; //! list of dimension setups, for delayed creation of the dimension information.
   std::vector<std::string>  fAliasesUsed;    //! List of aliases used during the parsing of the expression.

   TTreeFormula(const char *name, const char *formula, TTree *tree, const std::vector<string>& aliases);
   void Init(const char *name, const char *formula);
   Bool_t      BranchHasMethod(TLeaf* leaf, TBranch* branch, const char* method,const char* params, Long64_t readentry) const;
   Int_t       DefineAlternate(const char* expression);
   void        DefineDimensions(Int_t code, Int_t size, TFormLeafInfoMultiVarDim * info, Int_t& virt_dim);
   Int_t       FindLeafForExpression(const char* expression, TLeaf *&leaf, TString &leftover, Bool_t &final, UInt_t &paran_level, TObjArray &castqueue, std::vector<std::string>& aliasUsed, Bool_t &useLeafCollectionObject, const char *fullExpression);
   TLeaf*      GetLeafWithDatamember(const char* topchoice, const char* nextchice, Long64_t readentry) const;
   Int_t       ParseWithLeaf(TLeaf *leaf, const char *expression, Bool_t final, UInt_t paran_level, TObjArray &castqueue, Bool_t useLeafCollectionObject, const char *fullExpression);
   Int_t       RegisterDimensions(Int_t code, Int_t size, TFormLeafInfoMultiVarDim * multidim = 0);
   Int_t       RegisterDimensions(Int_t code, TBranchElement *branch);
   Int_t       RegisterDimensions(Int_t code, TFormLeafInfo *info, Bool_t useCollectionObject);
   Int_t       RegisterDimensions(Int_t code, TLeaf *leaf);
   Int_t       RegisterDimensions(const char *size, Int_t code);


   TAxis      *fAxis;           //! pointer to histogram axis if this is a string

   Bool_t      fDidBooleanOptimization;  //! True if we executed one boolean optimization since the last time instance number 0 was evaluated
   void        LoadBranches();

   TTreeFormulaManager *fManager; //! The dimension coordinator.
   friend class TTreeFormulaManager;

   void       ResetDimensions();
   Bool_t     LoadCurrentDim();
   
   virtual Bool_t     IsLeafInteger(Int_t code) const;

   virtual Bool_t     IsString(Int_t oper) const;
   virtual Bool_t     IsLeafString(Int_t code) const;

   void  Convert(UInt_t fromVersion);

private:
   // Not implemented yet
   TTreeFormula(const TTreeFormula&);
   TTreeFormula& operator=(const TTreeFormula&);

public:
             TTreeFormula();
             TTreeFormula(const char *name,const char *formula, TTree *tree);
   virtual   ~TTreeFormula();
   virtual Int_t       DefinedVariable(TString &variable, Int_t &action);
   virtual TClass*     EvalClass() const;
   virtual Double_t    EvalInstance(Int_t i=0, const char *stringStack[]=0);
   virtual const char *EvalStringInstance(Int_t i=0);
   virtual void*       EvalObject(Int_t i=0);
   // EvalInstance should be const.  See comment on GetNdata()
   TFormLeafInfo      *GetLeafInfo(Int_t code) const;
   TTreeFormulaManager*GetManager() const { return fManager; }
   TMethodCall        *GetMethodCall(Int_t code) const;
   virtual Int_t       GetMultiplicity() const {return fMultiplicity;}
   virtual TLeaf      *GetLeaf(Int_t n) const;
   virtual Int_t       GetNcodes() const {return fNcodes;}
   virtual Int_t       GetNdata();
   //GetNdata should probably be const.  However it need to cache some information about the actual dimension
   //of arrays, so if GetNdata is const, the variables fUsedSizes and fCumulUsedSizes need to be declared
   //mutable.  We will be able to do that only when all the compilers supported for ROOT actually implemented
   //the mutable keyword. 
   //NOTE: Also modify the code in PrintValue which current goes around this limitation :(
   virtual Bool_t      IsInteger() const;
           Bool_t      IsQuickLoad() const { return fQuickLoad; }
   virtual Bool_t      IsString() const;
   virtual Bool_t      Notify() { UpdateFormulaLeaves(); return kTRUE; }
   virtual char       *PrintValue(Int_t mode=0) const;
   virtual char       *PrintValue(Int_t mode, Int_t instance, const char *decform = "9.9") const;
   virtual void        SetAxis(TAxis *axis=0);
           void        SetQuickLoad(Bool_t quick) { fQuickLoad = quick; }
   virtual void        SetTree(TTree *tree) {fTree = tree;}
   virtual void        UpdateFormulaLeaves();

   ClassDef(TTreeFormula,9)  //The Tree formula
};

#endif
