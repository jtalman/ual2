//  File:         Particle.hh
//  Description:  This file contains the definition of Particle class 
//                (BEAM_DIM-dimensional particle coordinates.)
//  Created    :  April 1, 1994
//  Authors    :  Nikolay Malitsky  (malitsky@ivory.ssc.gov )
//                Alexander Reshetov(reshetov@vernon.ssc.gov)
//
//
//  Copyright SSCL
//  2550 Beckleymeade Ave.
//  Dallas, TX, 75237
//

#ifndef PARTICLE_H
#define PARTICLE_H

#include "Beam/Beam.hh"

//class Rmatrix;
//class Twiss;

class Particle : public Beam   
{
 public :

	Particle();
        Particle(const Particle& P);
//        Particle(const Rmatrix&  R);
//        Particle(const Twiss&    Tw);

// Access operators

        double& operator[](int number);

// Assignment operators

        void operator=(const Particle& P);
//        void operator=(const Rmatrix&  R);
//        void operator=(const Twiss&    Tw);

        void operator+=(const Particle& P);
        void operator-=(const Particle& P);
        void operator*=(const double v);
        void operator/=(const double v);

// Friend operators

        friend ostream& operator<<(ostream& out, const Particle& P);
        
       ~Particle();

 protected:
       
        double* variable;       // BEAM_DIM-dimensional particle coordinates.

// Auxiliary functions

        void create();
        void initialize();
        void initialize(const Particle& P);
//        void initialize(const Rmatrix&  R);
//        void initialize(const Twiss&    Tw); 
        void clean();
 
};

#define   Position   Particle

#endif
        
