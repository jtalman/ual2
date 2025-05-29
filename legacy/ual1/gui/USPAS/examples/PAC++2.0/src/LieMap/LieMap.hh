//  File:         LieMap.hh
//  Description:  This file contains the definition of class LieMap /exp(:f:)/
//  Created    :  October 1, 1994
//  Authors    :  Nikolay Malitsky  (malitsky@ivory.ssc.gov)
//
//
//  Copyright SSCL
//  2550 Beckleymeade Ave.
//  Dallas, TX, 75237
//

#ifndef LIEMAP_H
#define LIEMAP_H

#define LIEMAP_NUMBER  10
#define LIEMAP_ORDER   10


#include "Map/Map.hh"

class LieMap : public Beam
{
 public :

	LieMap();
	LieMap(Series& f);
	LieMap(LieMap& l);
	LieMap(void (*vectorPotential)(Map y, Series* vectorPotential,
            double* param), double* extParameters);
       ~LieMap();

// Access operators

	LieMap& operator()(int order);
        LieMap& operator()(void (*vectorPotential)(Map y, 
         Series* vectorPotential, double* param), double* extParameters, 
         int order);

// Assignment operators

	void operator= (ZSeries& f);
	void operator= (LieMap& l);
	void operator*=(LieMap& l);

// Additive & Multiplicative Operators

	Series& operator*(Series& g);
	Map&    operator*(Map& vg);
	LieMap& operator*(LieMap& l);

 protected:

	Series  body;
        double* parameters;
	int     prmOrder;
	int     tmpOrder;

// information about TmpLieMap, temporary object for intermediate operations

	int             whoAreYou;   // if 0 - authentic
	static int      numberTmpLieMap;
	static int      lastTmpLieMap;
	static LieMap*  tmpLieMap[LIEMAP_NUMBER+1];

// To work with the tmpLieMap

	int  inline isTemporary(void){return(whoAreYou > 0);}
	int  createNextTmpLieMap(void);
	int  prepareTmpLieMap(void);
	int  prepareTmpLieMap(LieMap& l);
	void deleteTmpLieMap(void);

// Auxiliary Function

        void createParameters(double* extParameters);
        void deleteParameters(void);
        void createBody(void (*vectorPotential)(Map y, Series* vectorPotential,
            double* param), double* extParameters);


};

#endif

