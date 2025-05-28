// @(#)root/tree:$Name:  $:$Id: TLeafObject.h,v 1.10 2004/09/24 19:44:30 brun Exp $
// Author: Rene Brun   27/01/96

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TLeafObject
#define ROOT_TLeafObject


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLeafObject                                                          //
//                                                                      //
// A TLeaf for a general object derived from TObject.                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef ROOT_TLeaf
#include "TLeaf.h"
#endif

class TClass;
class TMethodCall;

class TLeafObject : public TLeaf {

protected:
    TClass      *fClass;          //! pointer to class
    void       **fObjAddress;     //! Address of Pointer to object
    Bool_t       fVirtual;        // Support for Virtuality
    
public:
    enum { kWarn = BIT(12) };

    TLeafObject();
    TLeafObject(const char *name, const char *type);
    virtual ~TLeafObject();

    virtual void    FillBasket(TBuffer &b);
    TClass         *GetClass() const {return fClass;}
    TMethodCall    *GetMethodCall(const char *name);
    TObject        *GetObject() const {return (TObject*)(*fObjAddress);}
    const char     *GetTypeName() const ;
    virtual void   *GetValuePointer() const {return fObjAddress;}
    Bool_t          IsOnTerminalBranch() const    { return kFALSE; }
    Bool_t          IsVirtual() const {return fVirtual;}
    virtual Bool_t  Notify();
    virtual void    PrintValue(Int_t i=0) const;
    virtual void    ReadBasket(TBuffer &b);
    virtual void    SetAddress(void *add=0);
    virtual void    SetVirtual(Bool_t virt=kTRUE) {fVirtual=virt;}
    
    ClassDef(TLeafObject,4)  //A TLeaf for a general object derived from TObject.
};

#endif
