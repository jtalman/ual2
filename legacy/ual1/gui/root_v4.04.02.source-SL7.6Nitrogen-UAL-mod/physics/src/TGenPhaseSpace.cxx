// @(#)root/physics:$Name:  $:$Id: TGenPhaseSpace.cxx,v 1.7 2004/09/01 11:09:48 brun Exp $
// Author: Rene Brun , Valerio Filippini  06/09/2000 

//_____________________________________________________________________________________
//
//  Utility class to generate n-body event,
//  with constant cross-section (default)
//  or with Fermi energy dependence (opt="Fermi").
//  The event is generated in the center-of-mass frame, 
//  but the decay products are finally boosted
//  using the betas of the original particle.
//
//  The code is based on the GENBOD function (W515 from CERNLIB)
//  using the Raubold and Lynch method
//      F. James, Monte Carlo Phase Space, CERN 68-15 (1968)
//
// see example of use in $ROOTSYS/tutorials/PhaseSpace.C

#include "TGenPhaseSpace.h"
#include "TRandom.h"
#include "TMath.h"
#include <stdlib.h>

const Int_t kMAXP = 18;

ClassImp(TGenPhaseSpace)

//_____________________________________________________________________________________
Double_t TGenPhaseSpace::pdk(Double_t a, Double_t b, Double_t c) 
{
  Double_t x = (a-b-c)*(a+b+c)*(a-b+c)*(a+b-c);
  x = TMath::Sqrt(x)/(2*a);
  return x;
}

//_____________________________________________________________________________________
Int_t DoubleMax(const void *a, const void *b) 
{

  Double_t A = * ((Double_t *) a); 
  Double_t B = * ((Double_t *) b); 
  if (A > B) return  1;
  if (A < B) return -1;
  return 0;

}

//__________________________________________________________________________________________________
TGenPhaseSpace::TGenPhaseSpace(const TGenPhaseSpace &gen) : TObject(gen)
{
   fNt      = gen.fNt;
   fWtMax   = gen.fWtMax;
   fTeCmTm  = gen.fTeCmTm;
   fBeta[0] = gen.fBeta[0];
   fBeta[1] = gen.fBeta[1];
   fBeta[2] = gen.fBeta[2];
   for (Int_t i=0;i<fNt;i++) {
      fMass[i]   = gen.fMass[i];
      fDecPro[i] = gen.fDecPro[i];
   }
}

//__________________________________________________________________________________________________
Double_t TGenPhaseSpace::Generate() 
{
  //  Generate a random final state.
  //  The function returns the weigth of the current event.
  //  The TLorentzVector of each decay product can be get using GetDecay(n).
  //  The maximum weigth can be get using GetWtMax().
  // 
  Double_t rno[kMAXP];
  rno[0] = 0;
  Int_t n;
  if (fNt>2) {
    for (n=1; n<fNt-1; n++)  rno[n]=gRandom->Rndm();   // fNt-2 random numbers
    qsort(rno+1 ,fNt-2 ,sizeof(Double_t) ,DoubleMax);  // sort them
  }
  rno[fNt-1] = 1;

  Double_t InvMas[kMAXP], sum=0;
  for (n=0; n<fNt; n++) {
    sum      += fMass[n];
    InvMas[n] = rno[n]*fTeCmTm + sum;
  }

  //
  //-----> compute the weight of the current event
  //
  Double_t wt=fWtMax;
  Double_t pd[kMAXP];
  for (n=0; n<fNt-1; n++) {
    pd[n] = pdk(InvMas[n+1],InvMas[n],fMass[n+1]);
    wt *= pd[n];
  }

  //
  //-----> complete specification of event (Raubold-Lynch method)
  //
  fDecPro[0].SetPxPyPzE(0, pd[0], 0 , TMath::Sqrt(pd[0]*pd[0]+fMass[0]*fMass[0]) );

  Int_t i=1;
  Int_t j;
  while (1) {
    fDecPro[i].SetPxPyPzE(0, -pd[i-1], 0 , TMath::Sqrt(pd[i-1]*pd[i-1]+fMass[i]*fMass[i]) );

    Double_t cZ   = 2*gRandom->Rndm() - 1;
    Double_t sZ   = TMath::Sqrt(1-cZ*cZ);
    Double_t angY = 2*TMath::Pi() * gRandom->Rndm();
    Double_t cY   = TMath::Cos(angY);
    Double_t sY   = TMath::Sin(angY);
    for (j=0; j<=i; j++) {
      TLorentzVector *v = fDecPro+j;
      Double_t x = v->Px();
      Double_t y = v->Py();
      v->SetPx( cZ*x - sZ*y );
      v->SetPy( sZ*x + cZ*y );   // rotation around Z
               x = v->Px();
      Double_t z = v->Pz();
      v->SetPx( cY*x - sY*z );
      v->SetPz( sY*x + cY*z );   // rotation around Y
    }

    if (i == (fNt-1)) break;

    Double_t beta = pd[i] / sqrt(pd[i]*pd[i] + InvMas[i]*InvMas[i]);
    for (j=0; j<=i; j++) fDecPro[j].Boost(0,beta,0);
    i++;
  }
 
  //
  //---> final boost of all particles  
  //
  for (n=0;n<fNt;n++) fDecPro[n].Boost(fBeta[0],fBeta[1],fBeta[2]);

  //
  //---> return the weigth of event
  //
  return wt;
}

//__________________________________________________________________________________
TLorentzVector *TGenPhaseSpace::GetDecay(Int_t n) 
{ 
  if (n>fNt) return 0;
  return fDecPro+n;
}


//_____________________________________________________________________________________
Bool_t TGenPhaseSpace::SetDecay(TLorentzVector &P, Int_t nt, 
   Double_t *mass, Option_t *opt) 
{
  //
  // input:
  // TLorentzVector &P:    decay particle
  // Int_t nt:             number of decay products
  // Double_t *mass:       array of decay product masses
  // Option_t *opt:        default -> constant cross section
  //                       "Fermi" -> Fermi energy dependece
  // return value:
  // kTRUE:      the decay is permitted by kinematics
  // kFALSE:     the decay is forbidden by kinematics
  //

  Int_t n;
  fNt = nt;
  if (fNt<2 || fNt>18) return kFALSE;  // no more then 18 particle

  //
  //
  //
  fTeCmTm = P.Mag();           // total energy in C.M. minus the sum of the masses
  for (n=0;n<fNt;n++) {
    fMass[n]  = mass[n];
    fTeCmTm  -= mass[n];
  }

  if (fTeCmTm<=0) return kFALSE;    // not enough energy for this decay

  //
  //------> the max weigth depends on opt:
  //   opt == "Fermi"  --> fermi energy dependence for cross section
  //   else            --> constant cross section as function of TECM (default)
  //
  if (strcasecmp(opt,"fermi")==0) {  
    // ffq[] = pi * (2*pi)**(FNt-2) / (FNt-2)!
    Double_t ffq[] = {0 
		     ,3.141592, 19.73921, 62.01255, 129.8788, 204.0131
		     ,256.3704, 268.4705, 240.9780, 189.2637
		     ,132.1308,  83.0202,  47.4210,  24.8295
		     ,12.0006,   5.3858,   2.2560,   0.8859 };
    fWtMax = TMath::Power(fTeCmTm,fNt-2) * ffq[fNt-1] / P.Mag();

  } else {
    Double_t emmax = fTeCmTm + fMass[0];
    Double_t emmin = 0;
    Double_t wtmax = 1;
    for (n=1; n<fNt; n++) {
      emmin += fMass[n-1];
      emmax += fMass[n];
      wtmax *= pdk(emmax, emmin, fMass[n]);
    }
    fWtMax = 1/wtmax;
  }

  //
  //---->  save the betas of the decaying particle
  //
  if (P.Beta()) {
    Double_t w = P.Beta()/P.Rho();
    fBeta[0] = P(0)*w;
    fBeta[1] = P(1)*w;
    fBeta[2] = P(2)*w;
  }
  else fBeta[0]=fBeta[1]=fBeta[2]=0; 

  return kTRUE; 
}
