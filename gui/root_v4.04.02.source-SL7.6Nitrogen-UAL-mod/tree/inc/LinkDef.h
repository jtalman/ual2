/* @(#)root/tree:$Name:  $:$Id: LinkDef.h,v 1.27 2005/03/21 16:20:56 rdm Exp $ */

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class TBasket-;
#pragma link C++ class TBranch-;
#pragma link C++ class TBranchClones-;
#pragma link C++ class TBranchElement-;
#pragma link C++ class TBranchObject-;
#pragma link C++ class TBranchRef+;
#pragma link C++ class TChain-;
#pragma link C++ class TChainElement;
#pragma link C++ class TCut+;
#pragma link C++ class TEventList-;
#pragma link C++ class TFriendElement+;
#pragma link C++ class TTreeFriendLeafIter;
#pragma link C++ class TLeaf-;
#pragma link C++ class TLeafElement+;
#pragma link C++ class TLeafObject-;
#pragma link C++ class TLeafB+;
#pragma link C++ class TLeafC+;
#pragma link C++ class TLeafD+;
#pragma link C++ class TLeafF+;
#pragma link C++ class TLeafI+;
#pragma link C++ class TLeafS+;
#pragma link C++ class TLeafL+;
#pragma link C++ class TLeafO+;
#pragma link C++ class TNtuple-;
#pragma link C++ class TNtupleD-;
#pragma link C++ class+protected TSelector+;
#pragma link C++ class TSelectorCint+;
#pragma link C++ class TTree-;
#pragma link C++ class TChainProof-;
#pragma link C++ class TVirtualTreePlayer;
#pragma link C++ class TVirtualIndex+;
#pragma link C++ class TTreeResult+;
#pragma link C++ class TTreeRow-;
#pragma link C++ class TDSet+;
#pragma link C++ class TDSetElement+;
#pragma link C++ class TDSetElementPfn+;
#pragma link C++ class TDSetElementMsn+;
#pragma link C++ class TVirtualBranchBrowsable+;
#pragma link C++ class TMethodBrowsable+;
#pragma link C++ class TNonSplitBrowsable+;
#pragma link C++ class TCollectionPropertyBrowsable+;
#pragma link C++ class TCollectionMethodBrowsable+;
#pragma link C++ class TSelectorScalar+;

#pragma link C++ function operator+(const TCut&, const char*);
#pragma link C++ function operator+(const char*, const TCut&);
#pragma link C++ function operator+(const TCut&, const TCut&);
#pragma link C++ function operator*(const TCut&, const char*);
#pragma link C++ function operator*(const char*, const TCut&);
#pragma link C++ function operator*(const TCut&, const TCut&);
#pragma link C++ function operator&&(const TCut&, const char*);
#pragma link C++ function operator&&(const char*, const TCut&);
#pragma link C++ function operator&&(const TCut&, const TCut&);
#pragma link C++ function operator||(const TCut&, const char*);
#pragma link C++ function operator||(const char*, const TCut&);
#pragma link C++ function operator||(const TCut&, const TCut&);
#pragma link C++ function operator!(const TCut&);
#pragma link C++ function operator+(const TEventList&, const TEventList&);
#pragma link C++ function operator-(const TEventList&, const TEventList&);
#pragma link C++ function operator*(const TEventList&, const TEventList&);

#endif
