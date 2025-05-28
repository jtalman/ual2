// @(#)root/graf:$Name:  $:$Id: TMultiGraph.cxx,v 1.21 2005/05/02 21:45:05 brun Exp $
// Author: Rene Brun   12/10/2000

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "TROOT.h"
#include "TMultiGraph.h"
#include "TGraph.h"
#include "TH1.h"
#include "TVirtualPad.h"
#include "Riostream.h"
#include "TVirtualFitter.h"


#include <ctype.h>

extern void H1LeastSquareSeqnd(Int_t n, Double_t *a, Int_t idim, Int_t &ifail, Int_t k, Double_t *b);

ClassImp(TMultiGraph)

//______________________________________________________________________________
//
//   A TMultiGraph is a collection of TGraph (or derived) objects
//   Use TMultiGraph::Add to add a new graph to the list.
//   The TMultiGraph owns the objects in the list.
//   Drawing options are the same as for TGraph
//   Example;
//     TGraph *gr1 = new TGraph(...
//     TGraphErrors *gr2 = new TGraphErrors(...
//     TMultiGraph *mg = new TMultiGraph();
//     mg->Add(gr1,"lp");
//     mg->Add(gr2,"cp");
//     mg->Draw("a");
//
//  The drawing option for each TGraph may be specified as an optional
//  second argument of the Add function.
//  If a draw option is specified, it will be used to draw the graph,
//  otherwise the graph will be drawn with the option specified in
//  TMultiGraph::Draw

//______________________________________________________________________________
TMultiGraph::TMultiGraph(): TNamed()
{
// TMultiGraph default constructor

   fGraphs    = 0;
   fFunctions = 0;
   fHistogram = 0;
   fMaximum   = -1111;
   fMinimum   = -1111;
}

//______________________________________________________________________________
TMultiGraph::TMultiGraph(const char *name, const char *title)
       : TNamed(name,title)
{
// constructor with name and title
   fGraphs    = 0;
   fFunctions = 0;
   fHistogram = 0;
   fMaximum   = -1111;
   fMinimum   = -1111;
}

//______________________________________________________________________________
TMultiGraph::~TMultiGraph()
{
// TMultiGraph destructor

   if (!fGraphs) return;
   TGraph *g;
   TIter   next(fGraphs);
   while ((g = (TGraph*) next())) {
     g->ResetBit(kMustCleanup);
   }
   fGraphs->Delete();
   delete fGraphs;
   fGraphs = 0;
   delete fHistogram;
   fHistogram = 0;
   if (fFunctions) {
      fFunctions->SetBit(kInvalidObject);
      //special logic to support the case where the same object is
      //added multiple times in fFunctions.
      //This case happens when the same object is added with different
      //drawing modes
      TObject *obj;
      while ((obj  = fFunctions->First())) {
         while(fFunctions->Remove(obj));
         delete obj;
      }
      delete fFunctions;
   }
}

//______________________________________________________________________________
void TMultiGraph::Add(TGraph *graph, Option_t *chopt)
{
   // add a new graph to the list of graphs
   // note that the graph is now owned by the TMultigraph.
   // Deleting the TMultiGraph object will automatically delete the graphs.
   // You should not delete the graphs when the TMultigraph is still active.

   if (!fGraphs) fGraphs = new TList();
   graph->SetBit(kMustCleanup);
   fGraphs->Add(graph,chopt);
}

//______________________________________________________________________________
void TMultiGraph::Browse(TBrowser *)
{
    Draw("alp");
    gPad->Update();
}

//______________________________________________________________________________
Int_t TMultiGraph::DistancetoPrimitive(Int_t px, Int_t py)
{
// Compute distance from point px,py to each graph
//

//*-*- Are we on the axis?
   const Int_t kMaxDiff = 10;
   Int_t distance = 9999;
   if (fHistogram) {
      distance = fHistogram->DistancetoPrimitive(px,py);
      if (distance <= 0) return distance;
   }


//*-*- Loop on the list of graphs
   if (!fGraphs) return distance;
   TGraph *g;
   TIter   next(fGraphs);
   while ((g = (TGraph*) next())) {
      Int_t dist = g->DistancetoPrimitive(px,py);
      if (dist <= 0) return 0;
      if (dist < kMaxDiff) {gPad->SetSelected(g); return dist;}
   }
   return distance;
}

//______________________________________________________________________________
void TMultiGraph::Draw(Option_t *option)
{
//*-*-*-*-*-*-*-*-*-*-*Draw this multigraph with its current attributes*-*-*-*-*-*-*
//*-*                  ==========================================
//
//   Options to draw a graph are described in TGraph::PainGraph
//
//  The drawing option for each TGraph may be specified as an optional
//  second argument of the Add function. You can use GetGraphDrawOption
//  to return this option.
//  If a draw option is specified, it will be used to draw the graph,
//  otherwise the graph will be drawn with the option specified in
//  TMultiGraph::Draw. Use GetDrawOption to return the option specified
//  when drawin the TMultiGraph.

  AppendPad(option);
}

//______________________________________________________________________________
Int_t TMultiGraph::Fit(const char *fname, Option_t *option, Option_t *, Axis_t xmin, Axis_t xmax)
{
//*-*-*-*-*-*Fit this graph with function with name fname*-*-*-*-*-*-*-*-*-*
//*-*        ============================================
//  interface to TF1::Fit(TF1 *f1...

   char *linear;
   linear= (char*)strstr(fname, "++");
   TF1 *f1=0;
   if (linear)
	 f1=new TF1(fname, fname, xmin, xmax);
   else {
      f1 = (TF1*)gROOT->GetFunction(fname);
      if (!f1) { Printf("Unknown function: %s",fname); return -1; }
   }

   return Fit(f1,option,"",xmin,xmax);
}

//______________________________________________________________________________
Int_t TMultiGraph::Fit(TF1 *f1, Option_t *option, Option_t *, Axis_t rxmin, Axis_t rxmax)
{
//*-*-*-*-*-*-*-*-*-*-*Fit this multigraph with function f1*-*-*-*-*-*-*-*-*-*
//*-*                  ==================================
//
//   In this function all graphs of the multigraph are fitted simultaneously
//   
//   f1 is an already predefined function created by TF1.
//   Predefined functions such as gaus, expo and poln are automatically
//   created by ROOT.
//
//   The list of fit options is given in parameter option.
//      option = "W"  Set all errors to 1
//             = "U" Use a User specified fitting algorithm (via SetFCN)
//             = "Q" Quiet mode (minimum printing)
//             = "V" Verbose mode (default is between Q and V)
//             = "B" Use this option when you want to fix one or more parameters
//                   and the fitting function is like "gaus","expo","poln","landau".
//             = "R" Use the Range specified in the function range
//             = "N" Do not store the graphics function, do not draw
//             = "0" Do not plot the result of the fit. By default the fitted function
//                   is drawn unless the option"N" above is specified.
//             = "+" Add this new fitted function to the list of fitted functions
//                   (by default, any previous function is deleted)
//             = "C" In case of linear fitting, not calculate the chisquare
//                    (saves time)
//             = "F" If fitting a polN, switch to minuit fitter
//
//   When the fit is drawn (by default), the parameter goption may be used
//   to specify a list of graphics options. See TGraph::Paint for a complete
//   list of these options.
//
//   In order to use the Range option, one must first create a function
//   with the expression to be fitted. For example, if your graph
//   has a defined range between -4 and 4 and you want to fit a gaussian
//   only in the interval 1 to 3, you can do:
//        TF1 *f1 = new TF1("f1","gaus",1,3);
//        graph->Fit("f1","R");
//
//
//   who is calling this function
//   ============================
//   Note that this function is called when calling TGraphErrors::Fit
//   or TGraphAsymmErrors::Fit ot TGraphBentErrors::Fit
//   see the discussion below on the errors calulation.
//
//   Setting initial conditions
//   ==========================
//   Parameters must be initialized before invoking the Fit function.
//   The setting of the parameter initial values is automatic for the
//   predefined functions : poln, expo, gaus, landau. One can however disable
//   this automatic computation by specifying the option "B".
//   You can specify boundary limits for some or all parameters via
//        f1->SetParLimits(p_number, parmin, parmax);
//   if parmin>=parmax, the parameter is fixed
//   Note that you are not forced to fix the limits for all parameters.
//   For example, if you fit a function with 6 parameters, you can do:
//     func->SetParameters(0,3.1,1.e-6,0.1,-8,100);
//     func->SetParLimits(4,-10,-4);
//     func->SetParLimits(5, 1,1);
//   With this setup, parameters 0->3 can vary freely
//   Parameter 4 has boundaries [-10,-4] with initial value -8
//   Parameter 5 is fixed to 100.
//
//  Fit range
//  =========
//  The fit range can be specified in two ways:
//    - specify rxmax > rxmin (default is rxmin=rxmax=0)
//    - specify the option "R". In this case, the function will be taken
//      instead of the full graph range.
//
//   Changing the fitting function
//   =============================
//  By default the fitting function GraphFitChisquare is used.
//  To specify a User defined fitting function, specify option "U" and
//  call the following functions:
//    TVirtualFitter::Fitter(mygraph)->SetFCN(MyFittingFunction)
//  where MyFittingFunction is of type:
//  extern void MyFittingFunction(Int_t &npar, Double_t *gin, Double_t &f, Double_t *u, Int_t flag);
//
//  How errors are used in the chisquare function (see TFitter GraphFitChisquare)//   Access to the fit results
//   ============================================
// In case of a TGraphErrors object, ex, the error along x,  is projected
// along the y-direction by calculating the function at the points x-exlow and
// x+exhigh.
//
// The chisquare is computed as the sum of the quantity below at each point:
//
//                     (y - f(x))**2
//         -----------------------------------
//         ey**2 + ((f(x+exhigh) - f(x-exlow))/2)**2
//
// where x and y are the point coordinates.
//
// In case the function lies below (above) the data point, ey is ey_low (ey_high).
//
//  thanks to Andy Haas (haas@yahoo.com) for adding the case with TGraphasymmerrors
//            University of Washington
//
// a little different approach to approximating the uncertainty in y because of the
// errors in x, is to make it equal the error in x times the slope of the line.
// The improvement, compared to the first method (f(x+ exhigh) - f(x-exlow))/2
// is of (error of x)**2 order. This approach is called "effective variance method".  
// This improvement has been made in version 4.00/08 by Anna Kreshuk.
//
//   Associated functions
//   ====================
//  One or more object (typically a TF1*) can be added to the list
//  of functions (fFunctions) associated to each graph.
//  When TGraph::Fit is invoked, the fitted function is added to this list.
//  Given a graph gr, one can retrieve an associated function
//  with:  TF1 *myfunc = gr->GetFunction("myfunc");
//
//  If the graph is made persistent, the list of
//  associated functions is also persistent. Given a pointer (see above)
//  to an associated function myfunc, one can retrieve the function/fit
//  parameters with calls such as:
//    Double_t chi2 = myfunc->GetChisquare();
//    Double_t par0 = myfunc->GetParameter(0); //value of 1st parameter
//    Double_t err0 = myfunc->GetParError(0);  //error on first parameter
//
//   Fit Statistics
//   ==============
//  You can change the statistics box to display the fit parameters with
//  the TStyle::SetOptFit(mode) method. This mode has four digits.
//  mode = pcev  (default = 0111)
//    v = 1;  print name/values of parameters
//    e = 1;  print errors (if e=1, v must be 1)
//    c = 1;  print Chisquare/Number of degress of freedom
//    p = 1;  print Probability
//
//  For example: gStyle->SetOptFit(1011);
//  prints the fit probability, parameter names/values, and errors.
//  You can change the position of the statistics box with these lines
//  (where g is a pointer to the TGraph):
//
//  Root > TPaveStats *st = (TPaveStats*)g->GetListOfFunctions()->FindObject("stats")
//  Root > st->SetX1NDC(newx1); //new x start position
//  Root > st->SetX2NDC(newx2); //new x end position



   Int_t fitResult = 0;
   Double_t xmin, xmax, ymin, ymax;
   Int_t i, npar,nvpar,nparx;
   Double_t par, we, al, bl;
   Double_t eplus,eminus,eparab,globcc,amin,edm,errdef,werr;
   Int_t np;
   TF1 *fnew1;
   
   // Check validity of function
   if (!f1) {
      Error("Fit", "function may not be null pointer");
      return 0;
   }
   if (f1->IsZombie()) {
      Error("Fit", "function is zombie");
      return 0;
   }
   
   npar = f1->GetNpar();
   if (npar <= 0) {
      Error("Fit", "function %s has illegal number of parameters = %d", f1->GetName(), npar);
      return 0;
   }
   
   // Check that function has same dimension as graph
   if (f1->GetNdim() > 1) {
      Error("Fit", "function %s is not 1-D", f1->GetName());
      return 0;
   }

   TGraph *g;
   TIter next(fGraphs);

   Double_t *arglist = new Double_t[100];
   // Decode string choptin and fill fitOption structure
   Foption_t fitOption;
   fitOption.Quiet   = 0;
   fitOption.Verbose = 0;
   fitOption.Bound   = 0;
   fitOption.Like    = 0;
   fitOption.W1      = 0;
   fitOption.Errors  = 0;
   fitOption.Range   = 0;
   fitOption.Gradient= 0;
   fitOption.Nograph = 0;
   fitOption.Nostore = 0;
   fitOption.Plus    = 0;
   fitOption.User    = 0;
   fitOption.Nochisq = 0;
   fitOption.Minuit  = 0;
   fitOption.Integral= 0;
   fitOption.More    = 0;
   TString opt = option;
   opt.ToUpper();

   if (opt.Contains("U")) fitOption.User    = 1;
   if (opt.Contains("Q")) fitOption.Quiet   = 1;
   if (opt.Contains("V")){fitOption.Verbose = 1; fitOption.Quiet   = 0;}
   if (opt.Contains("W")) fitOption.W1      = 1;
   if (opt.Contains("E")) fitOption.Errors  = 1;
   if (opt.Contains("R")) fitOption.Range   = 1;
   if (opt.Contains("N")) fitOption.Nostore = 1;
   if (opt.Contains("0")) fitOption.Nograph = 1;
   if (opt.Contains("+")) fitOption.Plus    = 1;
   if (opt.Contains("B")) fitOption.Bound   = 1;
   if (opt.Contains("C")) fitOption.Nochisq = 1;
   if (opt.Contains("F")) fitOption.Minuit  = 1;

   if (rxmax > rxmin) {
      xmin = rxmin;
      xmax = rxmax;
   } else {
      g=(TGraph *)fGraphs->First();
      if (!g) {
	 Error("Fit", "No graphs in the multigraph");
	 return 0;
      }
      Double_t *px, *py;
      np=g->GetN();
      px=g->GetX();
      py=g->GetY();
      xmin=px[0];
      xmax=py[np-1];
      ymin=px[0];
      ymax=py[np-1];
      Double_t err0=g->GetErrorX(0);
      Double_t errn=g->GetErrorX(np-1);
      if (err0 > 0) xmin -= 2*err0;
      if (errn > 0) xmax += 2*errn;

      next.Reset();
      while ((g = (TGraph*) next())) {
	 np=g->GetN();
	 px=g->GetX();
	 py=g->GetY();
	 for (i=0; i<np; i++) {
	    if (px[i] < xmin) xmin = px[i];
	    if (px[i] > xmax) xmax = px[i];
	    if (py[i] < ymin) ymin = py[i];
	    if (py[i] > ymax) ymax = py[i];
	 }
      }
   }

   ///////////////
   //set the fitter
   //////////////

   Int_t special=f1->GetNumber();
   Bool_t linear = f1->IsLinear();
   if (special==299+npar)
      linear=kTRUE;
   if (fitOption.Bound || fitOption.User || fitOption.Errors || fitOption.Minuit)
      linear = kFALSE;

   char l[]="TLinearFitter";
   Int_t strdiff = 0;
   Bool_t IsSet = kFALSE;
   if (TVirtualFitter::GetFitter()){
      //Is a fitter already set? Is it linear?
      IsSet = kTRUE;
      strdiff = strcmp(TVirtualFitter::GetFitter()->IsA()->GetName(), l);
   }
   if (linear){
      TClass *cl = gROOT->GetClass("TLinearFitter");
      if (IsSet && strdiff!=0) {
	 delete TVirtualFitter::GetFitter();
	 IsSet=kFALSE;
      }
      if (!IsSet) {
	 TVirtualFitter::SetFitter((TVirtualFitter *)cl->New());
      }
   } else {
      if (IsSet && strdiff==0){
	 delete TVirtualFitter::GetFitter();
	 IsSet=kFALSE;
      }
      if (!IsSet)	
	 TVirtualFitter::SetFitter(0);	       
   }

   TVirtualFitter *grFitter = TVirtualFitter::Fitter(this, f1->GetNpar());
   grFitter->Clear();
   
//*-*- Get pointer to the function by searching in the list of functions in ROOT
   grFitter->SetUserFunc(f1);
   grFitter->SetFitOption(fitOption);

//*-*- Is a Fit range specified?
   if (fitOption.Range) {
      f1->GetRange(xmin, xmax);
   } else {
      f1->SetRange(xmin, xmax);
   }

   if (linear){
      grFitter->ExecuteCommand("FitMultiGraph", 0, 0);
     
   } else {

      //Int_t special = f1->GetNumber();
      if (fitOption.Bound) special = 0;
      if      (special == 100)      InitGaus(xmin,xmax);
      else if (special == 400)      InitGaus(xmin,xmax);
      else if (special == 200)      InitExpo(xmin,xmax);
      else if (special == 299+npar) InitPolynom(xmin,xmax);
      
      //*-*- Some initialisations
      if (!fitOption.Verbose) {
      arglist[0] = -1;
      grFitter->ExecuteCommand("SET PRINT", arglist,1);
      arglist[0] = 0;
      grFitter->ExecuteCommand("SET NOW",   arglist,0);
      }
   
      /////////////////////////////////////////////////////////      
      //*-*- Set error criterion for chisquare
      arglist[0] = TVirtualFitter::GetErrorDef();
      if (!fitOption.User) grFitter->SetFitMethod("MultiGraphFitChisquare");


      fitResult = grFitter->ExecuteCommand("SET ERR",arglist,1);
      if (fitResult != 0) {
	 //   Abnormal termination, MIGRAD might not have converged on a
	 //   minimum.
	 if (!fitOption.Quiet) {
	    Warning("Fit","Abnormal termination of minimization.");
	 }
	 delete [] arglist;
	 return fitResult;
      }
      
      //*-*- Transfer names and initial values of parameters to Minuit
      Int_t nfixed = 0;
      for (i=0;i<npar;i++) {
	 par = f1->GetParameter(i);
	 f1->GetParLimits(i,al,bl);
	 if (al*bl != 0 && al >= bl) {
	    al = bl = 0;
	    arglist[nfixed] = i+1;
	    nfixed++;
	 }
	 we  = 0.3*TMath::Abs(par);
	 if (we <= TMath::Abs(par)*1e-6) we = 1;
	 grFitter->SetParameter(i,f1->GetParName(i),par,we,al,bl);
      }
      if(nfixed > 0)grFitter->ExecuteCommand("FIX",arglist,nfixed); // Otto
      
      //*-*- Reset Print level
      if (!fitOption.Quiet) {
	 if (fitOption.Verbose) { arglist[0] = 2; grFitter->ExecuteCommand("SET PRINT", arglist,1); }
	 else                   { arglist[0] = 0; grFitter->ExecuteCommand("SET PRINT", arglist,1); }
      }
      //*-*- Compute sum of squares of errors in the bin range
      Bool_t hasErrors = kFALSE;
      Double_t ex, ey, sumw2=0;
      next.Reset();
      while ((g = (TGraph*) next())) {
	 np=g->GetN();
	 for (i=0; i<np; i++){
	    ex=g->GetErrorX(i);
	    ey=g->GetErrorY(i);
	    if (ex > 0 || ey > 0) hasErrors=kTRUE;
	    sumw2+=ey*ey;
	 }
      }

      //*-*- Perform minimization

      arglist[0] = TVirtualFitter::GetMaxIterations();
      arglist[1] = sumw2*TVirtualFitter::GetPrecision();
      grFitter->ExecuteCommand("MIGRAD",arglist,2);
      if (fitOption.Errors) {
	 grFitter->ExecuteCommand("HESSE",arglist,0);
	 grFitter->ExecuteCommand("MINOS",arglist,0);
      }
      
      grFitter->GetStats(amin,edm,errdef,nvpar,nparx);
      f1->SetChisquare(amin);
      Int_t ndf = f1->GetNumberFitPoints()-npar+nfixed;
      f1->SetNDF(ndf);

      //*-*- Get return status
      char parName[50];
      for (i=0;i<npar;i++) {
	 grFitter->GetParameter(i,parName, par,we,al,bl);
	 if (!fitOption.Errors) werr = we;
	 else {
	    grFitter->GetErrors(i,eplus,eminus,eparab,globcc);
	    if (eplus > 0 && eminus < 0) werr = 0.5*(eplus-eminus);
	    else                         werr = we;
	 }
	 if (!hasErrors && ndf > 1) werr *= TMath::Sqrt(amin/(ndf-1));
	 f1->SetParameter(i,par);
	 f1->SetParError(i,werr);
      }
   }
//*-*- Print final values of parameters.
   if (!fitOption.Quiet) {
      if (fitOption.Errors) grFitter->PrintResults(4,amin);
      else                  grFitter->PrintResults(3,amin);
   }
   delete [] arglist;

//*-*- Store fitted function in histogram functions list and draw

   if (!fitOption.Nostore) {
      if (!fFunctions) fFunctions = new TList;
      if (!fitOption.Plus) {
         TIter next2(fFunctions, kIterBackward);
         TObject *obj;
         while ((obj = next2())) {
            if (obj->InheritsFrom(TF1::Class())){
	       obj = fFunctions->Remove(obj); 
	       delete obj;
	    }
         }
      }
      fnew1 = new TF1();
      f1->Copy(*fnew1);
      fFunctions->Add(fnew1);
      fnew1->SetParent(this);
      fnew1->Save(xmin,xmax,0,0,0,0);
      if (fitOption.Nograph) fnew1->SetBit(TF1::kNotDraw);
      fnew1->SetBit(TFormula::kNotGlobal);

      if (TestBit(kCanDelete)) return fitResult;
      if (gPad) gPad->Modified();
   }


   return fitResult;

}


//______________________________________________________________________________
Option_t *TMultiGraph::GetGraphDrawOption(const TGraph *gr) const
{
// Return the draw option for the TGraph gr in this TMultiGraph
// The return option is the one specified when calling TMultiGraph::Add(gr,option).
   
   if (!fGraphs || !gr) return "";
   TListIter next(fGraphs);
   TObject *obj;
   while ((obj = next())) {
      if (obj == (TObject*)gr) return next.GetOption();
   }
   return "";
}

//______________________________________________________________________________
void TMultiGraph::InitGaus(Double_t xmin, Double_t xmax)
{
//*-*-*-*-*-*Compute Initial values of parameters for a gaussian*-*-*-*-*-*-*
//*-*        ===================================================

   Double_t allcha, sumx, sumx2, x, val, rms, mean;
   Int_t bin;
   const Double_t sqrtpi = 2.506628;

//*-*- Compute mean value and RMS of the graph in the given range
   Int_t np = 0;
   allcha = sumx = sumx2 = 0;
   TGraph *g;
   TIter next(fGraphs);
   Double_t *px, *py;
   Int_t npp; //number of points in each graph
   while ((g = (TGraph*) next())) {
      px=g->GetX();
      py=g->GetY();
      npp=g->GetN();
      for (bin=0; bin<npp; bin++){
	 x=px[bin];
	 if (x<xmin || x>xmax) continue;
	 np++;
	 val=py[bin];
	 sumx+=val*x;
	 sumx2+=val*x*x;
	 allcha+=val;
      }
   }
   if (np == 0 || allcha == 0) return;
   mean = sumx/allcha;
   rms  = TMath::Sqrt(sumx2/allcha - mean*mean);

   Double_t binwidx = TMath::Abs((xmax-xmin)/np);
   if (rms == 0) rms = 1;
   TVirtualFitter *grFitter = TVirtualFitter::GetFitter();
   TF1 *f1 = (TF1*)grFitter->GetUserFunc();
   f1->SetParameter(0,binwidx*allcha/(sqrtpi*rms));
   f1->SetParameter(1,mean);
   f1->SetParameter(2,rms);
   f1->SetParLimits(2,0,10*rms);
}

//______________________________________________________________________________
void TMultiGraph::InitExpo(Double_t xmin, Double_t xmax)
{
//*-*-*-*-*-*Compute Initial values of parameters for an exponential*-*-*-*-*
//*-*        =======================================================

   Double_t constant, slope;
   Int_t ifail;

   LeastSquareLinearFit(-1, constant, slope, ifail, xmin, xmax);

   TVirtualFitter *grFitter = TVirtualFitter::GetFitter();
   TF1 *f1 = (TF1*)grFitter->GetUserFunc();
   f1->SetParameter(0,constant);
   f1->SetParameter(1,slope);

}

//______________________________________________________________________________
void TMultiGraph::InitPolynom(Double_t xmin, Double_t xmax)
{
//*-*-*-*-*-*Compute Initial values of parameters for a polynom*-*-*-*-*-*-*
//*-*        ===================================================

   Double_t fitpar[25];

   TVirtualFitter *grFitter = TVirtualFitter::GetFitter();
   TF1 *f1 = (TF1*)grFitter->GetUserFunc();
   Int_t npar   = f1->GetNpar();

   LeastSquareFit(npar, fitpar, xmin, xmax);

   for (Int_t i=0;i<npar;i++) f1->SetParameter(i, fitpar[i]);

}

//______________________________________________________________________________
void TMultiGraph::LeastSquareFit(Int_t m, Double_t *a, Double_t xmin, Double_t xmax)
{
//*-*-*-*-*-*-*-*Least squares lpolynomial fitting without weights*-*-*-*-*-*-*
//*-*            =================================================
//
//  m     number of parameters
//  a     array of parameters
//  first 1st point number to fit (default =0)
//  last  last point number to fit (default=fNpoints-1)
//
//   based on CERNLIB routine LSQ: Translated to C++ by Rene Brun
//
//
    const Double_t zero = 0.;
    const Double_t one = 1.;
    const Int_t idim = 20;

    Double_t  b[400]	/* was [20][20] */;
    Int_t i, k, l, ifail, bin;
    Double_t power;
    Double_t da[20], xk, yk;


    //count the total number of points to fit
    TGraph *g;
    TIter next(fGraphs);
    Double_t *px, *py;
    Int_t n=0; 
    Int_t npp;
    while ((g = (TGraph*) next())) {
       px=g->GetX();
       py=g->GetY();
       npp=g->GetN();
       for (bin=0; bin<npp; bin++){
	  xk=px[bin];
	  if (xk < xmin || xk > xmax) continue;
	  n++;
       }
    }
    if (m <= 2) {
       LeastSquareLinearFit(n, a[0], a[1], ifail, xmin, xmax);
       return;
    }
    if (m > idim || m > n) return;
    da[0] = zero;
    for (l = 2; l <= m; ++l) {
	b[l-1]           = zero;
	b[m + l*20 - 21] = zero;
	da[l-1]          = zero;
    }
    Int_t np = 0;

    next.Reset();
    while ((g = (TGraph*) next())) {
       px=g->GetX();
       py=g->GetY();
       npp=g->GetN();

       for (k = 0; k <= npp; ++k) {
	  xk     = px[k];
	  if (xk < xmin || xk > xmax) continue;
	  np++;
	  yk     = py[k];
	  power  = one;
	  da[0] += yk;
	  for (l = 2; l <= m; ++l) {
	     power   *= xk;
	    b[l-1]  += power;
	    da[l-1] += power*yk;
	  }
	  for (l = 2; l <= m; ++l) {
	     power            *= xk;
	     b[m + l*20 - 21] += power;
	  }
       }
    }
    b[0]  = Double_t(np);
    for (i = 3; i <= m; ++i) {
	for (k = i; k <= m; ++k) {
	    b[k - 1 + (i-1)*20 - 21] = b[k + (i-2)*20 - 21];
	}
    }
    H1LeastSquareSeqnd(m, b, idim, ifail, 1, da);

    if (ifail < 0) {
       //a[0] = fY[0];
       py=((TGraph *)fGraphs->First())->GetY();
       a[0]=py[0];
       for (i=1; i<m; ++i) a[i] = 0;
       return;
    }
    for (i=0; i<m; ++i) a[i] = da[i];

}

//______________________________________________________________________________
void TMultiGraph::LeastSquareLinearFit(Int_t ndata, Double_t &a0, Double_t &a1, Int_t &ifail, Double_t xmin, Double_t xmax)
{
//*-*-*-*-*-*-*-*-*-*Least square linear fit without weights*-*-*-*-*-*-*-*-*
//*-*                =======================================
//
//  Fit a straight line (a0 + a1*x) to the data in this graph.
//  ndata:  number of points to fit
//  first:  first point number to fit
//  last:   last point to fit O(ndata should be last-first
//  ifail:  return parameter indicating the status of the fit (ifail=0, fit is OK)
//
//   extracted from CERNLIB LLSQ: Translated to C++ by Rene Brun
//

    Double_t xbar, ybar, x2bar;
    Int_t i;
    Double_t xybar;
    Double_t fn, xk, yk;
    Double_t det;

    ifail = -2;
    xbar  = ybar = x2bar = xybar = 0;
    Int_t np = 0;
    TGraph *g;
    TIter next(fGraphs);
    Double_t *px, *py;
    Int_t npp;    
    while ((g = (TGraph*) next())) {
       px=g->GetX();
       py=g->GetY();
       npp=g->GetN();
       for (i = 0; i < npp; ++i) {
	  xk = px[i];
	  if (xk < xmin || xk > xmax) continue;
	  np++;
	  yk = py[i];
	  if (ndata < 0) {
	     if (yk <= 0) yk = 1e-9;
	     yk = TMath::Log(yk);
	  }
	  xbar  += xk;
	  ybar  += yk;
	  x2bar += xk*xk;
	  xybar += xk*yk;
       }
    }
    fn    = Double_t(np);
    det   = fn*x2bar - xbar*xbar;
    ifail = -1;
    if (det <= 0) {
       if (fn > 0) a0 = ybar/fn;
       else        a0 = 0;
       a1 = 0;
       return;
    }
    ifail = 0;
    a0 = (x2bar*ybar - xbar*xybar) / det;
    a1 = (fn*xybar - xbar*ybar) / det;



}

//______________________________________________________________________________
TH1F *TMultiGraph::GetHistogram() const
{
//    Returns a pointer to the histogram used to draw the axis
//    Takes into account the two following cases.
//       1- option 'A' was specified in TMultiGraph::Draw. Return fHistogram
//       2- user had called TPad::DrawFrame. return pointer to hframe histogram

   if (fHistogram) return fHistogram;
   if (!gPad) return 0;
   gPad->Modified();
   gPad->Update();
   if (fHistogram) return fHistogram;
   TH1F *h1 = (TH1F*)gPad->FindObject("hframe");
   return h1;
}

//______________________________________________________________________________
TF1 *TMultiGraph::GetFunction(const char *name) const
{
//*-*-*-*-*Return pointer to function with name*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*      ===================================
//
// Functions such as TGraph::Fit store the fitted function in the list of
// functions of this graph.

   if (!fFunctions) return 0;
   return (TF1*)fFunctions->FindObject(name);
}

//______________________________________________________________________________
TAxis *TMultiGraph::GetXaxis() const
{
   // Get x axis of the graph.

   if (!gPad) return 0;
   TH1 *h = GetHistogram();
   if (!h) return 0;
   return h->GetXaxis();
}

//______________________________________________________________________________
TAxis *TMultiGraph::GetYaxis() const
{
   // Get y axis of the graph.

   if (!gPad) return 0;
   TH1 *h = GetHistogram();
   if (!h) return 0;
   return h->GetYaxis();
}

//______________________________________________________________________________
void TMultiGraph::Paint(Option_t *option)
{
// paint all the graphs of this multigraph

  if (fGraphs->GetSize() == 0) return;
  
  char *l;
  static char chopt[33];
  Int_t nch = strlen(option);
  Int_t i;
  for (i=0;i<nch;i++) chopt[i] = toupper(option[i]);
  chopt[nch] = 0;
  Double_t *x, *y;
  TGraph *g;

  l = strstr(chopt,"A");
  if (l) {
     *l = ' ';
     TIter   next(fGraphs);
     Int_t npt = 100;
     Double_t maximum, minimum, rwxmin, rwxmax, rwymin, rwymax, uxmin, uxmax, dx, dy;
     rwxmin    = gPad->GetUxmin();
     rwxmax    = gPad->GetUxmax();
     rwymin    = gPad->GetUymin();
     rwymax    = gPad->GetUymax();
     char *xtitle = 0;
     char *ytitle = 0;
     Int_t firstx = 0;
     Int_t lastx  = 0;
     
     if (fHistogram) {
        //cleanup in case of a previous unzoom
        if (fHistogram->GetMinimum() >= fHistogram->GetMaximum()) {
           Int_t nch = strlen(fHistogram->GetXaxis()->GetTitle());
           firstx = fHistogram->GetXaxis()->GetFirst();
           lastx  = fHistogram->GetXaxis()->GetLast();
           if (nch) {
              xtitle = new char[nch+1];
              strcpy(xtitle,fHistogram->GetXaxis()->GetTitle());
           }
           nch = strlen(fHistogram->GetYaxis()->GetTitle());
           if (nch) {
              ytitle = new char[nch+1];
              strcpy(ytitle,fHistogram->GetYaxis()->GetTitle());
           }
           delete fHistogram;
           fHistogram = 0;
        }
     }
     if (fHistogram) {
        minimum = fHistogram->GetYaxis()->GetXmin();
        maximum = fHistogram->GetYaxis()->GetXmax();
        uxmin   = gPad->PadtoX(rwxmin);
        uxmax   = gPad->PadtoX(rwxmax);
     } else {
        rwxmin = 1e100;
        rwxmax = -rwxmin;
        rwymin = rwxmin;
        rwymax = -rwymin;
        while ((g = (TGraph*) next())) {
           Int_t npoints = g->GetN();
           x = g->GetX();
           y = g->GetY();
           for (i=0;i<npoints;i++) {
              if (x[i] < rwxmin) rwxmin = x[i];
              if (x[i] > rwxmax) rwxmax = x[i];
              if (y[i] > rwymax) rwymax = y[i];
              if (y[i] < rwymin) rwymin = y[i];
           }
           g->ComputeRange(rwxmin, rwymin, rwxmax, rwymax);
           if (g->GetN() > npt) npt = g->GetN();
        }
        if (rwxmin == rwxmax) rwxmax += 1.;
        if (rwymin == rwymax) rwymax += 1.;
        dx = 0.05*(rwxmax-rwxmin);
        dy = 0.05*(rwymax-rwymin);
        uxmin    = rwxmin - dx;
        uxmax    = rwxmax + dx;
        if (gPad->GetLogy()) {
           if (rwymin <= 0) rwymin = 0.001*rwymax;
           minimum = rwymin/(1+0.5*TMath::Log10(rwymax/rwymin));
           maximum = rwymax*(1+0.2*TMath::Log10(rwymax/rwymin));
        } else {
           minimum  = rwymin - dy;
           maximum  = rwymax + dy;
        }
        if (minimum < 0 && rwymin >= 0) minimum = 0;
        if (maximum > 0 && rwymax <= 0) maximum = 0;
     }

     if (fMinimum != -1111) rwymin = minimum = fMinimum;
     if (fMaximum != -1111) rwymax = maximum = fMaximum;
     if (uxmin < 0 && rwxmin >= 0) {
        if (gPad->GetLogx()) uxmin = 0.9*rwxmin;
        //else                 uxmin = 0;
     }
     if (uxmax > 0 && rwxmax <= 0) {
        if (gPad->GetLogx()) uxmax = 1.1*rwxmax;
        //else                 uxmax = 0;
     }
     if (minimum < 0 && rwymin >= 0) {
        if(gPad->GetLogy()) minimum = 0.9*rwymin;
        //else                minimum = 0;
     }
     if (maximum > 0 && rwymax <= 0) {
        if(gPad->GetLogy()) maximum = 1.1*rwymax;
        //else                maximum = 0;
     }
     if (minimum <= 0 && gPad->GetLogy()) minimum = 0.001*maximum;
     if (uxmin <= 0 && gPad->GetLogx()) {
        if (uxmax > 1000) uxmin = 1;
        else              uxmin = 0.001*uxmax;
     }
     rwymin = minimum;
     rwymax = maximum;
     if (fHistogram) {
        fHistogram->GetYaxis()->SetLimits(rwymin,rwymax);
     }

//*-*-  Create a temporary histogram to draw the axis
     if (!fHistogram) {
        // the graph is created with at least as many channels as there are points
        // to permit zooming on the full range
        rwxmin = uxmin;
        rwxmax = uxmax;
        fHistogram = new TH1F(GetName(),GetTitle(),npt,rwxmin,rwxmax);
        if (!fHistogram) return;
        fHistogram->SetMinimum(rwymin);
        fHistogram->SetBit(TH1::kNoStats);
        fHistogram->SetMaximum(rwymax);
        fHistogram->GetYaxis()->SetLimits(rwymin,rwymax);
        fHistogram->SetDirectory(0);
        if (xtitle) {fHistogram->GetXaxis()->SetTitle(xtitle); delete [] xtitle;}
        if (ytitle) {fHistogram->GetYaxis()->SetTitle(ytitle); delete [] ytitle;}
        if (firstx != lastx) fHistogram->GetXaxis()->SetRange(firstx,lastx);
     }
     fHistogram->Paint("0");
   }

   if (fGraphs) {
      TObjOptLink *lnk = (TObjOptLink*)fGraphs->FirstLink();
      TObject *obj;

      while (lnk) {
         obj = lnk->GetObject();
         if (strlen(lnk->GetOption())) obj->Paint(lnk->GetOption());
         else                          obj->Paint(chopt);
         lnk = (TObjOptLink*)lnk->Next();
      }
   }

   TObject *f;
   if (fFunctions) {
     TIter   next(fFunctions);
     while ((f = (TObject*) next())) {
      if (f->InheritsFrom(TF1::Class())) {
         if (f->TestBit(TF1::kNotDraw) == 0) f->Paint("lsame");
      } else  {
         f->Paint();
      }
     }
   }


}

//______________________________________________________________________________
void TMultiGraph::Print(Option_t *option) const
{
// Print the list of graphs

   TGraph *g;
   if (fGraphs) {
     TIter   next(fGraphs);
     while ((g = (TGraph*) next())) {
       g->Print(option);
     }
   }
}

//______________________________________________________________________________
void TMultiGraph::RecursiveRemove(TObject *obj)
{
   // Recursively remove this object from a list. Typically implemented
   // by classes that can contain mulitple references to a same object.

   if (!fGraphs) return;
   TObject *objr = fGraphs->Remove(obj);
   if (!objr) return;
   delete fHistogram; fHistogram = 0;
   if (gPad) gPad->Modified();
}

//______________________________________________________________________________
void TMultiGraph::SavePrimitive(ofstream &out, Option_t *option)
{
    // Save primitive as a C++ statement(s) on output stream out

   char quote = '"';
   out<<"   "<<endl;
   if (gROOT->ClassSaved(TMultiGraph::Class())) {
       out<<"   ";
   } else {
       out<<"   TMultiGraph *";
   }
   out<<"multigraph = new TMultiGraph();"<<endl;
   out<<"   multigraph->SetName("<<quote<<GetName()<<quote<<");"<<endl;
   out<<"   multigraph->SetTitle("<<quote<<GetTitle()<<quote<<");"<<endl;

   if (fGraphs) {
      TObjOptLink *lnk = (TObjOptLink*)fGraphs->FirstLink();
      TObject *g;

      while (lnk) {
         g = lnk->GetObject();
         g->SavePrimitive(out,"multigraph");

         out<<"   multigraph->Add(graph,"<<quote<<lnk->GetOption()<<quote<<");"<<endl;

         lnk = (TObjOptLink*)lnk->Next();

     }
   }
   out<<"   multigraph->Draw("
      <<quote<<option<<quote<<");"<<endl;
}

//______________________________________________________________________________
void TMultiGraph::SetMaximum(Double_t maximum)
{
   fMaximum = maximum;
   if (fHistogram)  fHistogram->SetMaximum(maximum);
}

//______________________________________________________________________________
void TMultiGraph::SetMinimum(Double_t minimum)
{
   fMinimum = minimum;
   if (fHistogram) fHistogram->SetMinimum(minimum);
}
