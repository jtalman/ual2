// @(#)root/mlp:$Name:  $:$Id: TNeuron.h,v 1.7 2004/12/16 21:20:47 brun Exp $
// Author: Christophe.Delaere@cern.ch   20/07/03

/*************************************************************************
 * Copyright (C) 1995-2003, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TNeuron
#define ROOT_TNeuron

#ifndef ROOT_TNamed
#include "TNamed.h"
#endif
#ifndef ROOT_TObjArray
#include "TObjArray.h"
#endif

class TTreeFormula;
class TSynapse;
class TBranch;
class TTree;

//____________________________________________________________________
//
// TNeuron
//
// This class decribes an elementary neuron, which is the basic
// element for a Neural Network.
// A network is build connecting neurons by synapses.
// There are different types of neurons: linear (a+bx),
// sigmoid (1/(1+exp(-x)), tanh or gaussian.
// In a Multi Layer Perceptron, the input layer is made of
// inactive neurons (returning the normalized input), hidden layers
// are made of sigmoids and output neurons are linear.
//
// This implementation contains several methods to compute the value,
// the derivative, the DeDw, ...
// Values are stored in local buffers. The SetNewEvent() method is
// there to inform buffered values are outdated.
//
//____________________________________________________________________

class TNeuron : public TNamed {
   friend class TSynapse;

 public:
   enum NeuronType { kOff, kLinear, kSigmoid, kTanh, kGauss };

   TNeuron(NeuronType type = kSigmoid, 
           const char* name = "", const char* title = "");
   virtual ~ TNeuron() {}
   inline TSynapse* GetPre(Int_t n) const { return (TSynapse*) fpre.At(n); }
   inline TSynapse* GetPost(Int_t n) const { return (TSynapse*) fpost.At(n); }
   TTreeFormula* UseBranch(TTree*, const char*);
   Double_t GetValue() const;
   Double_t GetDerivative() const;
   Double_t GetError() const;
   Double_t GetDeDw() const;
   Double_t GetBranch() const;
   void SetWeight(Double_t w);
   inline Double_t GetWeight() const { return fWeight; }
   void SetNormalisation(Double_t mean, Double_t RMS);
   inline const Double_t* GetNormalisation() const { return fNorm; }
   void SetNewEvent() const;
   void SetDEDw(Double_t in);
   inline Double_t GetDEDw() const { return fDEDw; }
   void ForceExternalValue(Double_t value);

 protected:
   Double_t Sigmoid(Double_t x) const;
   Double_t DSigmoid(Double_t x) const;
   void AddPre(TSynapse*);
   void AddPost(TSynapse*);

 private:
   TObjArray fpre;        // pointers to the previous level in a network
   TObjArray fpost;       // pointers to the next level in a network
   Double_t fWeight;      // weight used for computation
   Double_t fNorm[2];     // normalisation to mean=0, RMS=1.
   NeuronType fType;      // neuron type
   //buffers
   //should be mutable when supported by all compilers
   TTreeFormula* fFormula;//! formula to be used for inputs and outputs
   Bool_t fNewValue;      //! do we need to compute fValue again ?
   Double_t fValue;       //! buffer containing the last neuron output
   Bool_t fNewDeriv;      //! do we need to compute fDerivative again ?
   Double_t fDerivative;  //! buffer containing the last neuron derivative
   Bool_t fNewDeDw;       //! do we need to compute fDeDw again ?
   Double_t fDeDw;        //! buffer containing the last derivative of the error
   Double_t fDEDw;        //! buffer containing the sum over all examples of DeDw

   ClassDef(TNeuron, 2)   // Neuron for MultiLayerPerceptrons
};

#endif
