// @(#)root/base:$Name:  $:$Id: TVirtualFitter.cxx,v 1.7 2004/01/20 17:59:38 rdm Exp $
// Author: Rene Brun   31/08/99
/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "TROOT.h"
#include "TVirtualFitter.h"
#include "TPluginManager.h"
#include "TEnv.h"
#include "Api.h"


TVirtualFitter *TVirtualFitter::fgFitter    = 0;
Int_t           TVirtualFitter::fgMaxpar    = 0;
Int_t           TVirtualFitter::fgMaxiter   = 5000;
Double_t        TVirtualFitter::fgPrecision = 1e-6;
Double_t        TVirtualFitter::fgErrorDef  = 1;
TString         TVirtualFitter::fgDefault   = "";

ClassImp(TVirtualFitter)

//______________________________________________________________________________
TVirtualFitter::TVirtualFitter()
{
   fMethodCall = 0;
   fFCN        = 0;
}

//______________________________________________________________________________
TVirtualFitter::~TVirtualFitter()
{
   // Cleanup virtual fitter.

   delete fMethodCall;
   fgFitter    = 0;
   fgMaxpar    = 0;
   fMethodCall = 0;
   fFCN        = 0;
}

//______________________________________________________________________________
TVirtualFitter *TVirtualFitter::Fitter(TObject *obj, Int_t maxpar)
{
   // Static function returning a pointer to the current fitter.
   // If the fitter does not exist, the default TFitter is created.

   if (fgFitter && maxpar > fgMaxpar) {
      delete fgFitter;
      fgFitter = 0;
   }

   if (!fgFitter) {
      TPluginHandler *h;
      if (fgDefault.Length() == 0) fgDefault = gEnv->GetValue("Root.Fitter","Minuit");
      if ((h = gROOT->GetPluginManager()->FindHandler("TVirtualFitter",fgDefault))) {
         if (h->LoadPlugin() == -1)
            return 0;
         fgFitter = (TVirtualFitter*) h->ExecPlugin(1, maxpar);
         fgMaxpar = maxpar;
      }
   }

   if (fgFitter) fgFitter->SetObjectFit(obj);
   return fgFitter;
}

//______________________________________________________________________________
const char *TVirtualFitter::GetDefaultFitter()
{
   // static: return the name of the default fitter

   return fgDefault.Data();
}

//______________________________________________________________________________
TVirtualFitter *TVirtualFitter::GetFitter()
{
   // static: return the current Fitter
   return fgFitter;
}

//______________________________________________________________________________
Int_t TVirtualFitter::GetMaxIterations()
{
   // static: Return the maximum number of iterations

   return fgMaxiter;
}

//______________________________________________________________________________
Double_t TVirtualFitter::GetErrorDef()
{
   // static: Return the Error Definition

   return fgErrorDef;
}

//______________________________________________________________________________
Double_t TVirtualFitter::GetPrecision()
{
   // static: Return the fit relative precision

   return fgPrecision;
}

//______________________________________________________________________________
void TVirtualFitter::SetDefaultFitter(const char *name)
{
   // static: set name of default fitter

   if (fgDefault == name) return;
   delete fgFitter;
   fgFitter = 0;
   fgDefault = name;
}

//______________________________________________________________________________
void TVirtualFitter::SetFitter(TVirtualFitter *fitter, Int_t maxpar)
{
   // Static function to set an alternative fitter

   fgFitter = fitter;
   fgMaxpar = maxpar;
}

//______________________________________________________________________________
void TVirtualFitter::SetFCN(void (*fcn)(Int_t &, Double_t *, Double_t &f, Double_t *, Int_t))
{
   // To set the address of the minimization objective function
   // called by the native compiler (see function below when called by CINT)

   fFCN = fcn;
}

//______________________________________________________________________________
void InteractiveFCN(Int_t &npar, Double_t *gin, Double_t &f, Double_t *u, Int_t flag)
{
   // Static function called when SetFCN is called in interactive mode

   TMethodCall *m = TVirtualFitter::GetFitter()->GetMethodCall();
   if (!m) return;

   Long_t args[5];
   args[0] = (Long_t)&npar;
   args[1] = (Long_t)gin;
   args[2] = (Long_t)&f;
   args[3] = (Long_t)u;
   args[4] = (Long_t)flag;
   m->SetParamPtrs(args);
   Double_t result;
   m->Execute(result);
}

//______________________________________________________________________________
void TVirtualFitter::SetFCN(void *fcn)
{
   //  To set the address of the minimization objective function
   //
   //     this function is called by CINT instead of the function above

   if (!fcn) return;

   char *funcname = G__p2f2funcname(fcn);
   if (funcname) {
      delete fMethodCall;
      fMethodCall = new TMethodCall();
      fMethodCall->InitWithPrototype(funcname,"Int_t&,Double_t*,Double_t&,Double_t*,Int_t");
    }
   fFCN = InteractiveFCN;
}

//______________________________________________________________________________
void TVirtualFitter::SetMaxIterations(Int_t niter)
{
   // static: Set the maximum number of iterations

   fgMaxiter  = niter;
}

//______________________________________________________________________________
void TVirtualFitter::SetErrorDef(Double_t errdef)
{
   // static: Set the Error Definition (default=1)

   fgErrorDef = errdef;
}

//______________________________________________________________________________
void TVirtualFitter::SetPrecision(Double_t prec)
{
   // static: Set the fit relative precision

   fgPrecision = prec;
}
