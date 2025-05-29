// @(#)root/physics:$Name:  $:$Id: TRolke.cxx,v 1.8 2005/02/11 18:40:08 rdm Exp $
// Author: Jan Conrad    9/2/2004

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//
//  TRolke
//
//  This class computes confidence intervals for the rate of a Poisson
//  in the presence of background and efficiency with a fully frequentist
//  treatment of the uncertainties in the efficiency and background estimate
//  using the profile likelihood method.
//
//  The signal is always assumed to be Poisson.
//
//  The method is very similar to the one used in MINUIT (MINOS).
//
//  Two options are offered to deal with cases where the maximum likelihood
//  estimate (MLE) is not in the physical region. Version "bounded likelihood" 
//  is the one used by MINOS if bounds for the physical region are chosen. Versi//  on "unbounded likelihood (the default) allows the MLE to be in the 
//  unphysical region. It has however better coverage. 
//  For more details consult the reference (see below). 
//
//
//   It allows the following Models:
//
//       1: Background - Poisson, Efficiency - Binomial  (cl,x,y,z,tau,m)
//       2: Background - Poisson, Efficiency - Gaussian  (cl,xd,y,em,tau,sde)
//       3: Background - Gaussian, Efficiency - Gaussian (cl,x,bm,em,sd)
//       4: Background - Poisson, Efficiency - known     (cl,x,y,tau,e)
//       5: Background - Gaussian, Efficiency - known    (cl,x,y,z,sdb,e)
//       6: Background - known, Efficiency - Binomial    (cl,x,z,m,b)
//       7: Background - known, Efficiency - Gaussian    (cl,x,em,sde,b)
//
//  Parameter definition:
//
//  cl  =  Confidence level
//
//  x = number of observed events
//
//  y = number of background events
//
//  z = number of simulated signal events
//
//  em = measurement of the efficiency.
//
//  bm = background estimate
//
//  tau = ratio between signal and background region (in case background is
//  observed) ratio between observed and simulated livetime in case
//  background is determined from MC.
//
//  sd(x) = sigma of the Gaussian
//
//  e = true efficiency (in case known)
//
//  b = expected background (in case known)
//
//  m = number of MC runs
//
//  mid = ID number of the model ...
//
//  For a description of the method and its properties:
//
//  W.Rolke, A. Lopez, J. Conrad and Fred James
//  "Limits and Confidence Intervals in presence of nuisance parameters"
//   http://lanl.arxiv.org/abs/physics/0403059
//
//  Should I use TRolke, TFeldmanCousins, TLimit?
//  ============================================
//  1. I guess TRolke makes TFeldmanCousins obsolete?
//
//  Certainly not. TFeldmanCousins is the fully frequentist construction and 
//  should be used in case of no (or negligible uncertainties). It is however 
//  not capable of treating uncertainties in nuisance parameters.
//  TRolke is desined for this case and it is shown in the reference above
//  that it has good coverage properties for most cases, ie it might be
//  used where FeldmannCousins can't.
//
//  2. What are the advantages of TRolke over TLimit?
//
//  TRolke is fully frequentist. TLimit treats nuisance parameters Bayesian. 
//  For a coverage study of a Bayesian method refer to 
//  physics/0408039 (Tegenfeldt & J.C). However, this note studies 
//  the coverage of Feldman&Cousins with Bayesian treatment of nuisance 
//  parameters. To make a long story short: using the Bayesian method you 
//  might introduce a small amount of over-coverage (though I haven't shown it 
//  for TLimit). On the other hand, coverage of course is a not so interesting 
//  when you consider yourself a Bayesian.
//
// Author: Jan Conrad (CERN)
//
// see example in tutorial Rolke.C
//
// Copyright CERN 2004                Jan.Conrad@cern.ch
//
///////////////////////////////////////////////////////////////////////////


#include "TRolke.h"
#include "TMath.h"
#include "Riostream.h"

ClassImp(TRolke)

//__________________________________________________________________________
TRolke::TRolke(Double_t CL, Option_t * /*option*/)
{
  fUpperLimit  = 0.0;
  fLowerLimit  = 0.0;
  fCL          = CL;
  fSwitch      = 0; // 0: unbounded likelihood
                    // 1: bounded likelihood
}

//___________________________________________________________________________
TRolke::~TRolke()
{
}


//___________________________________________________________________________
Double_t TRolke::CalculateInterval(Int_t x, Int_t y, Int_t z, Double_t bm, Double_t em,Double_t e, Int_t mid, Double_t sde, Double_t sdb, Double_t tau, Double_t b, Int_t m)
{

  Int_t done = 0;
  Double_t limit[2];

  limit[1] = Interval(x,y,z,bm,em,e,mid, sde,sdb,tau,b,m);

  if (limit[1] > 0) {
    done = 1;
  }

  if (fSwitch == 0) {

    Int_t trial_x = x;

    while (done == 0) {
      trial_x++;
      limit[1] = Interval(trial_x,y,z,bm,em,e,mid, sde,sdb,tau,b,m);
      if (limit[1] > 0) done = 1;
    } 
  }

  return limit[1];
}




//_____________________________________________________________________
Double_t TRolke::Interval(Int_t x, Int_t y, Int_t z, Double_t bm, Double_t em,Double_t e, Int_t mid, Double_t sde, Double_t sdb, Double_t tau, Double_t b, Int_t m)
{
  // Calculates the Confidence Interval

  Double_t dchi2 =  Chi2Percentile(1,1-fCL);


  Double_t tempxy[2],limits[2] = {0,0};
  Double_t slope,fmid,low,flow,high,fhigh,test,ftest,mu0,maximum,target,l,f0;
  Double_t med = 0;
  Double_t maxiter=1000, acc = 0.00001;
  Int_t i;
  Int_t bp = 0;

  if ((mid != 3) && (mid != 5)) bm = (Double_t)y;

  if ((mid == 3) || (mid == 5)) {
    if (bm == 0) bm = 0.00001;
  } 

  if ((mid <= 2) || (mid == 4)) bp = 1;
  

  if (bp == 1 && x == 0 && bm > 0 ){

    for(Int_t i = 0; i < 2; i++) {
       x++;
       tempxy[i] = Interval(x,y,z,bm,em,e,mid,sde,sdb,tau,b,m);

    }
    slope = tempxy[1] - tempxy[0];
    limits[1] = tempxy[0] - slope;
    limits[0] = 0.0;
    if (limits[1] < 0) limits[1] = 0.0;
    goto done;
  }

  if (bp != 1 && x == 0){

    for(Int_t i = 0; i < 2; i++) {
       x++;
       tempxy[i] = Interval(x,y,z,bm,em,e,mid,sde,sdb,tau,b,m);

    }
    slope = tempxy[1] - tempxy[0];
    limits[1] = tempxy[0] - slope;
    limits[0] = 0.0;
    if (limits[1] < 0) limits[1] = 0.0;
    goto done;
  }

  if (bp != 1  && bm == 0){
    for(Int_t i = 0; i < 2; i++) {
       bm++;
       limits[1] = Interval(x,y,z,bm,em,e,mid,sde,sdb,tau,b,m);
       tempxy[i] = limits[1];
    }
    slope = tempxy[1] - tempxy[0];
    limits[1] = tempxy[0] - slope;
    if (limits[1] < 0) limits[1] = 0;
    goto done;
 }


 if (x == 0 && bm == 0){
    x++;
    bm++;

    limits[1] = Interval(x,y,z,bm,em,e,mid,sde,sdb,tau,b,m);
    tempxy[0] = limits[1];
    x  = 1;
    bm = 2;
    limits[1] = Interval(x,y,z,bm,em,e,mid,sde,sdb,tau,b,m);
    tempxy[1] = limits[1];
    x  = 2;
    bm = 1;
    limits[1] = Interval(x,y,z,bm,em,e,mid,sde,sdb,tau,b,m);
    limits[1] = 3*tempxy[0] -tempxy[1] - limits[1];
    if (limits[1] < 0) limits[1] = 0;
    goto done;
  }

  mu0 = Likelihood(0,x,y,z,bm,em,e,mid,sde,sdb,tau,b,m,1);

  maximum = Likelihood(0,x,y,z,bm,em,e,mid,sde,sdb,tau,b,m,2);

  test = 0;

  f0 = Likelihood(test,x,y,z,bm,em,e,mid,sde,sdb,tau,b,m,3);

  if ( fSwitch == 1 ) {  // do this only for the unbounded likelihood case
    if ( mu0 < 0 ) maximum = f0;
  }

  target = maximum - dchi2;

  if (f0 > target) {
     limits[0] = 0;
  } else {
    if (mu0 < 0){
       limits[0] = 0;
       limits[1] = 0;
    }

    low   = 0;
    flow  = f0;
    high  = mu0;
    fhigh = maximum;

    for(Int_t i = 0; i < maxiter; i++) {
      l = (target-fhigh)/(flow-fhigh);
      if (l < 0.2) l = 0.2;
      if (l > 0.8) l = 0.8;

      med = l*low + (1-l)*high;
      if(med < 0.01){
	limits[1]=0.0;                           
	goto done;
      }

    fmid = Likelihood(med,x,y,z,bm,em,e,mid,sde,sdb,tau,b,m,3);

      if (fmid > target) {
	 high  = med;
	 fhigh = fmid;
      }	else {
	 low  = med;
	 flow = fmid;
      }
      if ((high-low) < acc*high) break;
    }
    limits[0] = med;
  }


  if(mu0 > 0) {
     low  = mu0;
     flow = maximum;
  } else {
     low  = 0;
     flow = f0;
  }

  test = low +1 ;

  ftest = Likelihood(test,x,y,z,bm,em,e,mid,sde,sdb,tau,b,m,3);

  if (ftest < target) {
     high  = test;
     fhigh = ftest;
  } else {
     slope = (ftest - flow)/(test - low);
     high  = test + (target -ftest)/slope;
     fhigh = Likelihood(high,x,y,z,bm,em,e,mid,sde,sdb,tau,b,m,3);
  }

  for(i = 0; i < maxiter; i++) {
     l = (target-fhigh)/(flow-fhigh);
     if (l < 0.2) l = 0.2;
     if (l > 0.8) l = 0.8;
     med  = l * low + (1.-l)*high;
     fmid = Likelihood(med,x,y,z,bm,em,e,mid,sde,sdb,tau,b,m,3);

     if (fmid < target) {
        high  = med;
	fhigh = fmid;
     }	else {
	low  = med;
	flow = fmid;
     }
     if (high-low < acc*high) break;
  }
  limits[1] = med;

  done:

  if ( (mid == 4) || (mid==5) ) {
     limits[0] /= e;
     limits[1] /= e;
  }


  fUpperLimit = limits[1];
  fLowerLimit = TMath::Max(limits[0],0.0);

  
  return limits[1];
}


//___________________________________________________________________________
Double_t TRolke::Likelihood(Double_t mu, Int_t x, Int_t y, Int_t z, Double_t bm,Double_t em, Double_t e, Int_t mid, Double_t sde, Double_t sdb, Double_t tau, Double_t b, Int_t m, Int_t what)
{
  // Chooses between the different profile likelihood functions to use for the
  // different models.
  // Returns evaluation of the profile likelihood functions.

  switch (mid) {
     case 1: return EvalLikeMod1(mu,x,y,z,e,tau,b,m,what);
     case 2: return EvalLikeMod2(mu,x,y,em,e,sde,tau,b,what);
     case 3: return EvalLikeMod3(mu,x,bm,em,e,sde,sdb,b,what);
     case 4: return EvalLikeMod4(mu,x,y,tau,b,what);
     case 5: return EvalLikeMod5(mu,x,bm,sdb,b,what);
     case 6: return EvalLikeMod6(mu,x,z,e,b,m,what);
     case 7: return EvalLikeMod7(mu,x,em,e,sde,b,what);
  }

  return 0;
}

//_________________________________________________________________________
Double_t TRolke::EvalLikeMod1(Double_t mu, Int_t x, Int_t y, Int_t z, Double_t e, Double_t tau, Double_t b, Int_t m, Int_t what)
{
  // Calculates the Profile Likelihood for MODEL 1:
  //  Poisson background/ Binomial Efficiency
  // what = 1: Maximum likelihood estimate is returned
  // what = 2: Profile Likelihood of Maxmimum Likelihood estimate is returned.
  // what = 3: Profile Likelihood of Test hypothesis is returned
  // otherwise parameters as described in the beginning of the class)

  Double_t f  = 0;
  Double_t zm = Double_t(z)/m;

  if (what == 1) {
     f = (x-y/tau)/zm;
  }

  if (what == 2) {
     mu = (x-y/tau)/zm;
     b  = y/tau;
     Double_t e = zm;
     f = LikeMod1(mu,b,e,x,y,z,tau,m);
  }

  if (what == 3) {
    if (mu == 0){
       b = (x+y)/(1.0+tau);
       e = zm;
       f = LikeMod1(mu,b,e,x,y,z,tau,m);
     } else {
       TRolke g;
       g.ProfLikeMod1(mu,b,e,x,y,z,tau,m);
       f = LikeMod1(mu,b,e,x,y,z,tau,m);
     }
  }

  return f;
}

//________________________________________________________________________
Double_t TRolke::LikeMod1(Double_t mu,Double_t b, Double_t e, Int_t x, Int_t y, Int_t z, Double_t tau, Int_t m)
{
  // Profile Likelihood function for MODEL 1:
  // Poisson background/ Binomial Efficiency

  return 2*(x*TMath::Log(e*mu+b)-(e*mu +b)-TMath::Log(TMath::Factorial(x))+y*TMath::Log(tau*b)-tau*b-TMath::Log(TMath::Factorial(y)) + TMath::Log(TMath::Factorial(m)) - TMath::Log(TMath::Factorial(m-z)) - TMath::Log(TMath::Factorial(z))+ z * TMath::Log(e) + (m-z)*TMath::Log(1-e));
}

//________________________________________________________________________
void TRolke::ProfLikeMod1(Double_t mu,Double_t &b,Double_t &e,Int_t x,Int_t y, Int_t z,Double_t tau,Int_t m)
{
  // Void needed to calculate estimates of efficiency and background for model 1

  Double_t med = 0.0,fmid;
  Int_t maxiter =1000;
  Double_t acc = 0.00001;
  Double_t emin = ((m+mu*tau)-TMath::Sqrt((m+mu*tau)*(m+mu*tau)-4 * mu* tau * z))/2/mu/tau;

  Double_t low  = TMath::Max(1e-10,emin+1e-10);
  Double_t high = 1 - 1e-10;

  for(Int_t i = 0; i < maxiter; i++) {
     med = (low+high)/2.;

     fmid = LikeGradMod1(med,mu,x,y,z,tau,m);

     if(high < 0.5) acc = 0.00001*high;
     else           acc = 0.00001*(1-high);

     if ((high - low) < acc*high) break;

     if(fmid > 0) low  = med;
     else         high = med;
  }

  e = med;
  Double_t eta = Double_t(z)/e -Double_t(m-z)/(1-e);

  b = Double_t(y)/(tau -eta/mu);
}

//___________________________________________________________________________
Double_t TRolke::LikeGradMod1(Double_t e, Double_t mu, Int_t x,Int_t y,Int_t z,Double_t tau,Int_t m)
{
  Double_t eta, etaprime, bprime,f;
  eta = static_cast<double>(z)/e - static_cast<double>(m-z)/(1.0 - e);
  etaprime = (-1) * (static_cast<double>(m-z)/((1.0 - e)*(1.0 - e)) + static_cast<double>(z)/(e*e));
  Double_t b = y/(tau - eta/mu);
  bprime = (b*b * etaprime)/mu/y;
  f =  (mu + bprime) * (x/(e * mu + b) - 1)+(y/b - tau) * bprime + eta;
  return f;
}

//___________________________________________________________________________
Double_t TRolke::EvalLikeMod2(Double_t mu, Int_t x, Int_t y, Double_t em, Double_t e,Double_t sde, Double_t tau, Double_t b, Int_t what)
{
  // Calculates the Profile Likelihood for MODEL 2:
  //  Poisson background/ Gauss Efficiency
  // what = 1: Maximum likelihood estimate is returned
  // what = 2: Profile Likelihood of Maxmimum Likelihood estimate is returned.
  // what = 3: Profile Likelihood of Test hypothesis is returned
  // otherwise parameters as described in the beginning of the class)

  Double_t v =  sde*sde;
  Double_t coef[4],roots[3];
  Double_t f = 0;

  if (what == 1) {
    f = (x-y/tau)/em;
  }

  if (what == 2) {
    mu = (x-y/tau)/em;
    b = y/tau;
    e = em;
    f = LikeMod2(mu,b,e,x,y,em,tau,v);
  }

  if (what == 3) {
    if (mu == 0 ) {
      b = (x+y)/(1+tau);
      f = LikeMod2(mu,b,e,x,y,em,tau,v);
    } else {
      coef[3] = mu;
      coef[2] = mu*mu*v-2*em*mu-mu*mu*v*tau;
      coef[1] = ( - x)*mu*v - mu*mu*mu*v*v*tau - mu*mu*v*em + em*mu*mu*v*tau + em*em*mu - y*mu*v;
      coef[0] = x*mu*mu*v*v*tau + x*em*mu*v - y*mu*mu*v*v + y*em*mu*v;

      TMath::RootsCubic(coef,roots[0],roots[1],roots[2]);

      e = roots[1];
      b = y/(tau + (em - e)/mu/v);
      f = LikeMod2(mu,b,e,x,y,em,tau,v);
    }
  }

  return f;
}

//_________________________________________________________________________
Double_t TRolke::LikeMod2(Double_t mu, Double_t b, Double_t e,Int_t x,Int_t y,Double_t em,Double_t tau, Double_t v)
{
  // Profile Likelihood function for MODEL 2:
  // Poisson background/Gauss Efficiency

  return 2*(x*TMath::Log(e*mu+b)-(e*mu+b)-TMath::Log(TMath::Factorial(x))+y*TMath::Log(tau*b)-tau*b-TMath::Log(TMath::Factorial(y))-0.9189385-TMath::Log(v)/2-(em-e)*(em-e)/v/2);
}

//_____________________________________________________________________

Double_t TRolke::EvalLikeMod3(Double_t mu, Int_t x, Double_t bm, Double_t em, Double_t e, Double_t sde, Double_t sdb, Double_t b, Int_t what)
{
  // Calculates the Profile Likelihood for MODEL 3:
  // Gauss  background/ Gauss Efficiency
  // what = 1: Maximum likelihood estimate is returned
  // what = 2: Profile Likelihood of Maxmimum Likelihood estimate is returned.
  // what = 3: Profile Likelihood of Test hypothesis is returned
  // otherwise parameters as described in the beginning of the class)

  Double_t f = 0.;
  Double_t  v = sde*sde;
  Double_t  u = sdb*sdb;

  if (what == 1) {
     f = (x-bm)/em;
  }


  if (what == 2) {
     mu = (x-bm)/em;
     b  = bm;
     e  = em;
     f  = LikeMod3(mu,b,e,x,bm,em,u,v);
  }


  if(what == 3) {
    if(mu == 0.0){
      b = ((bm-u)+TMath::Sqrt((bm-u)*(bm-u)+4*x*u))/2.;
      e = em;
      f = LikeMod3(mu,b,e,x,bm,em,u,v);
    } else {
      Double_t temp[3];
      temp[0] = mu*mu*v+u;
      temp[1] = mu*mu*mu*v*v+mu*v*u-mu*mu*v*em+mu*v*bm-2*u*em;
      temp[2] = mu*mu*v*v*bm-mu*v*u*em-mu*v*bm*em+u*em*em-mu*mu*v*v*x;
      e = (-temp[1]+TMath::Sqrt(temp[1]*temp[1]-4*temp[0]*temp[2]))/2/temp[0];
      b = bm-(u*(em-e))/v/mu;
      f = LikeMod3(mu,b,e,x,bm,em,u,v);
    }
  }

  return f;
}

//____________________________________________________________________
Double_t TRolke::LikeMod3(Double_t mu,Double_t b,Double_t e,Int_t x,Double_t bm,Double_t em,Double_t u,Double_t v)
{
  // Profile Likelihood function for MODEL 3:
  // Gauss background/Gauss Efficiency

  return 2*(x * TMath::Log(e*mu+b)-(e*mu+b)-TMath::Log(TMath::Factorial(x))-1.837877-TMath::Log(u)/2-(bm-b)*(bm-b)/u/2-TMath::Log(v)/2-(em-e)*(em-e)/v/2);
}

//____________________________________________________________________
Double_t TRolke::EvalLikeMod4(Double_t mu, Int_t x, Int_t y, Double_t tau, Double_t b, Int_t what)
{
  // Calculates the Profile Likelihood for MODEL 4:
  // Poiss  background/Efficiency known
  // what = 1: Maximum likelihood estimate is returned
  // what = 2: Profile Likelihood of Maxmimum Likelihood estimate is returned.
  // what = 3: Profile Likelihood of Test hypothesis is returned
  // otherwise parameters as described in the beginning of the class)

  Double_t f = 0.0;

  if (what == 1) f = x-y/tau;
  if (what == 2) {
     mu = x-y/tau;
     b  = Double_t(y)/tau;
     f  = LikeMod4(mu,b,x,y,tau);
  }
  if (what == 3) {
    if (mu == 0.0) {
       b = Double_t(x+y)/(1+tau);
       f = LikeMod4(mu,b,x,y,tau);
    } else {
       b = (x+y-(1+tau)*mu+sqrt((x+y-(1+tau)*mu)*(x+y-(1+tau)*mu)+4*(1+tau)*y*mu))/2/(1+tau);
       f = LikeMod4(mu,b,x,y,tau);
    }
  }
  return f;
}

//___________________________________________________________________
Double_t TRolke::LikeMod4(Double_t mu,Double_t b,Int_t x,Int_t y,Double_t tau)
{
  // Profile Likelihood function for MODEL 4:
  // Poiss background/Efficiency known

  return 2*(x*TMath::Log(mu+b)-(mu+b)-TMath::Log(TMath::Factorial(x))+y*TMath::Log(tau*b)-tau*b-TMath::Log(TMath::Factorial(y)) );
}

//___________________________________________________________________
Double_t TRolke::EvalLikeMod5(Double_t mu, Int_t x, Double_t bm, Double_t sdb, Double_t b, Int_t what)
{
  // Calculates the Profile Likelihood for MODEL 5:
  // Gauss  background/Efficiency known
  // what = 1: Maximum likelihood estimate is returned
  // what = 2: Profile Likelihood of Maxmimum Likelihood estimate is returned.
  // what = 3: Profile Likelihood of Test hypothesis is returned
  // otherwise parameters as described in the beginning of the class)

  Double_t u=sdb*sdb;
  Double_t f = 0;

  if(what == 1) {
     f = x - bm;
  }
  if(what == 2) {
     mu = x-bm;
     b  = bm;
     f  = LikeMod5(mu,b,x,bm,u);
  }

  if (what == 3) {
     b = ((bm-u-mu)+TMath::Sqrt((bm-u-mu)*(bm-u-mu)-4*(mu*u-mu*bm-u*x)))/2;
     f = LikeMod5(mu,b,x,bm,u);
  }
  return f;
}

//_______________________________________________________________________
Double_t TRolke::LikeMod5(Double_t mu,Double_t b,Int_t x,Double_t bm,Double_t u)
{
  // Profile Likelihood function for MODEL 5:
  // Gauss background/Efficiency known

  return 2*(x*TMath::Log(mu+b)-(mu + b)-TMath::Log(TMath::Factorial(x))-0.9189385-TMath::Log(u)/2-((bm-b)*(bm-b))/u/2);
}

//_______________________________________________________________________
Double_t TRolke::EvalLikeMod6(Double_t mu, Int_t x, Int_t z, Double_t e, Double_t b, Int_t m, Int_t what)
{
  // Calculates the Profile Likelihood for MODEL 6:
  // Gauss  known/Efficiency binomial
  // what = 1: Maximum likelihood estimate is returned
  // what = 2: Profile Likelihood of Maxmimum Likelihood estimate is returned.
  // what = 3: Profile Likelihood of Test hypothesis is returned
  // otherwise parameters as described in the beginning of the class)

  Double_t coef[4],roots[3];
  Double_t f = 0.;
  Double_t zm = Double_t(z)/m;

  if(what==1){
     f = (x-b)/zm;
  }

  if(what==2){
     mu = (x-b)/zm;
     e  = zm;
     f  = LikeMod6(mu,b,e,x,z,m);
  }
  if(what == 3){
    if(mu==0){
       e = zm;
    } else {
       coef[3] = mu*mu;
       coef[2] = mu * b - mu * x - mu*mu - mu * m;
       coef[1] = mu * x - mu * b + mu * z - m * b;
       coef[0] = b * z;
       TMath::RootsCubic(coef,roots[0],roots[1],roots[2]);
       e = roots[1];
    }
    f =LikeMod6(mu,b,e,x,z,m);
  }
  return f;
}

//_______________________________________________________________________
Double_t TRolke::LikeMod6(Double_t mu,Double_t b,Double_t e,Int_t x,Int_t z,Int_t m)
{
  // Profile Likelihood function for MODEL 6:
  // background known/ Efficiency binomial

  Double_t f = 0.0;

  if (z > 100 || m > 100) {
     f = 2*(x*TMath::Log(e*mu+b)-(e*mu+b)-TMath::Log(TMath::Factorial(x))+(m*TMath::Log(m)  - m)-(z*TMath::Log(z) - z)  - ((m-z)*TMath::Log(m-z) - m + z)+z*TMath::Log(e)+(m-z)*TMath::Log(1-e));
  } else {
     f = 2*(x*TMath::Log(e*mu+b)-(e*mu+b)-TMath::Log(TMath::Factorial(x))+TMath::Log(TMath::Factorial(m))-TMath::Log(TMath::Factorial(z))-TMath::Log(TMath::Factorial(m-z))+z*TMath::Log(e)+(m-z)*TMath::Log(1-e));
  }
  return f;
}


//___________________________________________________________________________
Double_t TRolke::EvalLikeMod7(Double_t mu, Int_t x, Double_t em, Double_t e, Double_t sde, Double_t b, Int_t what)
{
  // Calculates the Profile Likelihood for MODEL 7:
  // background known/Efficiency Gauss
  // what = 1: Maximum likelihood estimate is returned
  // what = 2: Profile Likelihood of Maxmimum Likelihood estimate is returned.
  // what = 3: Profile Likelihood of Test hypothesis is returned
  // otherwise parameters as described in the beginning of the class)

  Double_t v=sde*sde;
  Double_t f = 0.;

  if(what ==  1) {
     f = (x-b)/em;
  }

  if(what == 2) {
     mu = (x-b)/em;
     e  = em;
     f  = LikeMod7(mu, b, e, x, em, v);
  }

  if(what == 3) {
    if(mu==0) {
       e = em;
    } else {
       e = ( -(mu*em-b-mu*mu*v)-TMath::Sqrt((mu*em-b-mu*mu*v)*(mu*em-b-mu*mu*v)+4*mu*(x*mu*v-mu*b*v + b * em)))/( - mu)/2;
    }
    f = LikeMod7(mu, b, e, x, em, v);
  }

  return f;
}

//___________________________________________________________________________
Double_t TRolke::LikeMod7(Double_t mu,Double_t b,Double_t e,Int_t x,Double_t em,Double_t v)
{
  // Profile Likelihood function for MODEL 6:
  // background known/ Efficiency binomial

  return 2*(x*TMath::Log(e*mu+b)-(e*mu + b)-TMath::Log(TMath::Factorial(x))-0.9189385-TMath::Log(v)/2-(em-e)*(em-e)/v/2);
}

//______________________________________________________________________
Double_t TRolke::Chi2Percentile(Double_t df,Double_t CL1)
{
 //
 // Finds the Chi-square argument x such that the integral
 // from x to infinity of the Chi-square density is equal
 // to the given cumulative probability y.
 //
 // This is accomplished using the inverse gamma integral
 // function and the relation
 //    x/2 = igami( df/2, y );
 //
 //   Author Jan Conrad (CERN) Jan.Conrad@cern.ch
 //   Copyright by Stephen L. Moshier (Cephes Math Library)

  return  2*InverseIncompleteGamma(df/2,CL1);
}

//______________________________________________________________________
Double_t TRolke::InverseIncompleteGamma(Double_t df,Double_t CL1)
{
  // calculates the inverse of the incomplete (complemented)gamma integral
  // for df degrees of freedom and fraction CL1
  // * Given p, the function finds x such that
  // Starting with the approximate value
  //          3
  //  x = df t
  //
  // where
  //
  //  t = 1 - d - InverseNormal(p) sqrt(df)
  //
  // and
  //
  // d = 1/9df,
  //
  // the routine performs up to 10 Newton iterations to find the
  // root of 1- TMath::Gamma(df,CL1) - p = 0.
  // Author Jan Conrad (CERN) Jan.Conrad@cern.ch
  // Copyright by Stephen L. Moshier (Cephes Math Library)


  const Double_t MACHEP = 1.11022302462515654042E-12;   /* 2**-53 */
  const Double_t MAXLOG = 8.8029691931113054295988E1;
  const Double_t MAXNUM = 1.701411834604692317316873e38;
  const Double_t dithresh = 5.0*MACHEP;

  // bound the solution
  Double_t x0 = MAXNUM;
  Double_t y1 = 0;
  Double_t x1 = 0;
  Double_t yh = 1.0;

  // approximation to the inverse function
  Double_t d = 1/(9*df);
  Double_t y = ( 1 - d - InverseNormal(CL1) * TMath::Sqrt(d) );
  Double_t x = df * y * y * y;
  Double_t lgm = TMath::LnGamma(df);

  Int_t i,dir;

  for (i = 0; i < 10; i++) {
     if (x > x0 || x < x1) goto ihalve;
     y = 1 - TMath::Gamma(df,x);
     if (y < y1 || y > yh) goto ihalve;
     if (y < CL1) {
	x0 = x;
	y1 = y;
     } else {
	x1 =x;
        yh =y;
     }

     // compute derivative of the function at this point
     d = (df  - 1)*TMath::Log(x) -x - TMath::LnGamma(df);
     if (d < -MAXLOG ) goto ihalve;
     d = -TMath::Exp(d);

     // compute the step to the next approximatioin of x
     d = (y -CL1)/d;
     if (TMath::Abs(d/x) < MACHEP) goto done;
     x = x -d;

     // Resort to interval halving if Newton iteration did not converge

     ihalve:
	d = 0.0625;
        if ( x0 == MAXNUM ) {
           if( x <= 0 ) x = 1;
           while( x0 == MAXNUM ) {
	      x = (1 + d) * x;
	      y = 1 - TMath::Gamma(df,x);
	      if (y < CL1 ) {
	         x0 = x;
	         y1 = y;
	         break;
	      }
	      d = d +d;
           }
        }
        d = 0.5;
  }

  dir = 0;

  for (i=0; i < 400; i++) {
      x = x1 + d * (x0-x1);
      y = 1 - TMath::Gamma(df,x);

      lgm = (x0-x1)/(x1+x0);
      if (TMath::Abs(lgm) < dithresh ) break;

      lgm = (y-CL1)/CL1;
      if (TMath::Abs(lgm) < dithresh ) break;
      if (y >= CL1) {
	  x1 = x;
	  yh = y;
	  if (dir < 0) {
	      dir = 0;
	      d   = 0.5;
	  }
	  else if(dir > 0)
	    d = 0.5*d + 0.5;
	  else
	    d = (CL1-y1)/(yh-y1);
	  dir += 1;
      } else {
	  x0 = x;
	  CL1 = y;
	  if (dir > 0 ) {
	      dir = 0;
	      d   = 0.5;
	  }
	  else if(dir < -1)
	    d = 0.5* d;
	  else
	    d = (CL1-y1)/(yh-y1);
	  dir -= 1;
      }

   }

   if ( x == 0) {
      cout << "ERROR" << endl;
   }

   done:
   return x;
}


//___________________________________________________________________
Double_t TRolke::InverseNormal(Double_t CL1)
{

  // Returns the argument, x, for which the area under the
  // Gaussian probability density function (integrated from
  // minus infinity to x) is equal to y.
  //
  // For small arguments 0 < y < exp(-2), the program computes
  // z = sqrt( -2.0 * log(y) );  then the approximation is
  // x = z - log(z)/z  - (1/z) P(1/z) / Q(1/z).
  // There are two rational functions P/Q, one for 0 < y < exp(-32)
  // and the other for y up to exp(-2).  For larger arguments,
  // w = y - 0.5, and  x/sqrt(2pi) = w + w**3 R(w**2)/S(w**2)).
  //
  //   Author Jan Conrad (CERN) Jan.Conrad@cern.ch
  //   Copyright by Stephen L. Moshier (Cephes Math Library)

  const Double_t MAXNUM = 1.7976931348623158E308;
  const Double_t s2pi   = 2.50662827463100050242E0;


  const Int_t P0[20] = {5165,3678,64335,49229,60889,40609,32785,16472,56250,34822,22160,49228,49661,15319,56526,16427,51838,36598,54175,49139};

  const Int_t Q0[32] = {29945,53792,17813,16383,58806,33815,46210,16402,33236,13579,38670,16469,13919,22210,11982,49260,52223,43241,2131,16489,47065,59277,33377,49236,30051,49412,53165,16431,52693,64191,61148,49138};

  const Int_t P1[36] = {36571,39484,14535,16400,31106,37589,34413,16447,32703,17062,38103,16460,7081,4334,2638,16454,17982,37887,24242,16429,49057,31340,32313,16385,38519,17549,62441,49089,16839,61027,61717,49057,59279,27851,6366,48972};

  const Int_t Q1[32] = {54299,41275,36698,16431,10607,35310,45572,16454,17948,8844,43162,16452,55365,40327,5575,16430,47648,43068,2437,16388,562,52699,13068,49090,45853,17773,32590,49059,1566,31101,38079,48974};

  const Int_t P2[36] = {54644,59283,59112,16393,30731,49959,43313,16411,45228,62251,33454,16399,39436,6375,21532,16373,9809,62302,51781,16329,13645,36969,22096,16265,6162,59598,50098,16179,8756,19468,19497,16070,36857,12376,52396,15930};

  const Int_t Q2[32] = {59432,22155,6362,16408,14358,43730,28749,16397,25088,10927,2119,16374,15515,24534,44455,16331,51083,44470,31783,16267,10499,26111,32555,16181,52471,62454,17292,16072,27453,47222,10725,15933};


  Double_t x, y, z, y2, x0, x1;
  Int_t code;

  if( CL1 <= 0.0 ) return -MAXNUM;
  if( CL1 >= 1.0 ) return MAXNUM;
  code = 1;
  y = CL1;
  if (y > (1.0 - 0.13533528323661269189) ) { // 0.135... = exp(-2)
     y = 1.0 - y;
     code = 0;
  }

  if (y > 0.13533528323661269189 ) {
     y  = y - 0.5;
     y2 = y * y;
     x  = y + y * (y2 * EvalPolynomial( y2, P0, 4)/EvalMonomial( y2, Q0, 8 ));
     x  = x * s2pi;
     return x;
  }

  x  = TMath::Sqrt( -2.0 * TMath::Log(y) );
  x0 = x - TMath::Log(x)/x;

  z = 1.0/x;
  if ( x < 8.0 ) { // y > exp(-32) = 1.2664165549e-14
     x1 = z * EvalPolynomial( z, P1, 8 )/EvalMonomial( z, Q1, 8 );
  } else {
     x1 = z * EvalPolynomial( z, P2, 8 )/EvalMonomial( z, Q2, 8 );
  }
  x = x0 - x1;
  if ( code != 0 ) x = -x;
  return x;
}

//______________________________________________________________________
Double_t TRolke::EvalPolynomial(Double_t x, const Int_t  coef[], Int_t N)
{
 // evaluate polynomial

  const Int_t   *p;
  p = coef;
  Double_t ans = *p++;
  Int_t i = N;

  do
     ans = ans * x  +  *p++;
  while( --i );

  return ans;
}

//______________________________________________________________________
Double_t TRolke::EvalMonomial(Double_t x, const Int_t coef[], Int_t N)
{
   // evaluate mononomial

  Double_t ans;
  const Int_t   *p;

  p   = coef;
  ans = x + *p++;
  Int_t i = N-1;

  do
     ans = ans * x  + *p++;
  while( --i );

  return ans;
}
