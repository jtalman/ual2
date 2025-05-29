//  File:         Beam.hh
//  Description:  This file contains the definition of Beam class 
//  Created    :  May 1, 1994
//  Authors    :  Nikolay Malitsky  (malitsky@ivory.ssc.gov )
//                Alexander Reshetov(reshetov@vernon.ssc.gov)
//
//
//  Copyright SSCL
//  2550 Beckleymeade Ave.
//  Dallas, TX, 75237
//

#ifndef BEAM_H
#define BEAM_H

#include "Beam/BeamDef.hh"

enum BeamKeyword
{
  BeamMass,
  BeamEnergy,
  BeamCharge,
  BeamKeywordSize
};

class Beam   
{
 public :

	Beam();
        Beam(BeamKeyword keyword);

// Assignment operators

        void operator= (double v);

 protected:

        static double m0c2;
        static double e0;
        static double p0c;
	static double v0byc;
        static double charge;
       
 private:
 
        BeamKeyword whoami;
      
        void initializeStaticMembers();
        void initializeMainParameter(BeamKeyword keyword, double v);
        void initializeMomentum();

};

extern Beam MASS, ENERGY, CHARGE;

#endif
        
