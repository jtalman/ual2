//  File:         Beam.hh
//  Description:  This file contains the implementation of Beam class 
//  Created    :  May 1, 1994
//  Authors    :  Nikolay Malitsky  (malitsky@ivory.ssc.gov )
//                Alexander Reshetov(reshetov@vernon.ssc.gov)
//
//
//  Copyright SSCL
//  2550 Beckleymeade Ave.
//  Dallas, TX, 75237
//

#include "Beam/Beam.hh"

// ------------------------------------------------

int  BEAM_DIM    = 6;
int  BEAM_STATIC = 0;

double Beam::m0c2;
double Beam::e0;
double Beam::p0c;
double Beam::v0byc;
double Beam::charge;

Beam MASS(BeamMass);
Beam ENERGY(BeamEnergy);
Beam CHARGE(BeamCharge);

// ---------   Constructors & destructor  ----------

Beam::Beam()
{
  initializeStaticMembers();
  whoami = BeamKeywordSize;
}

Beam::Beam(BeamKeyword keyword)
{
  initializeStaticMembers();
  whoami = keyword;
}

// Assignment operators

void Beam::operator= (double v)
{
  initializeMainParameter(whoami, v);
  initializeMomentum();
}


// Auxiliary functions

void Beam::initializeStaticMembers()
{
  if(BEAM_STATIC == 0)
  {
    BEAM_STATIC = 1;

    m0c2   = PROTON;
    e0     = BEAM_INFINITY;
    p0c    = BEAM_INFINITY;
    v0byc  = 1.;
    charge = 1.;

  }
  return;
}

void Beam::initializeMainParameter(BeamKeyword keyword, double v)
{
  switch(keyword)
  {

    case BeamMass   : m0c2 = v;
                      break;

    case BeamEnergy : e0 = v;
                      break;

    case BeamCharge : charge = v;
                      break;

    case BeamKeywordSize : 

        cerr << "Error: Beam::initializeMainParameter(keyword, v) : "
             << "keyword == BeamKeywordSize \n";
        exit(1);
  }
}
      
void Beam::initializeMomentum()
{
  if(e0 < m0c2)
  {
    cerr << "Error : Beam::initializeMomentum() : ";
    cerr << " e0(" << e0 << ") < m0c2(" << m0c2 << ")\n";
    exit(1);
  }

  if(e0 == BEAM_INFINITY) return;

  p0c   = sqrt(e0*e0 - m0c2*m0c2);
  v0byc = p0c/e0;

  return;
} 

 
