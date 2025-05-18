#include <time.h>
#include "RKIntegrator/RKIntegrator.hh"
#include "LieMap/LieMap.hh"
#include "Field/Field.hh"
#include "VectorA/VectorA.hh"

main()
{

    ZLIB_ORDER = 2;
    BEAM_DIM   = 4;
    BESSI_PSEUDO_NUMBER = 10;

// Input File

 int i, order = 1;
 double length = 0.0, k = 0.0, phase = 0.0, gap = 0.0; 
 double a[21], b[21];
 for(i= 0; i < 21; i++) a[i] = b[i] = 0.0;

#include "tmp/helixParameters.hh"

 if(order < 1 || order > 20){
    cerr << "Error: order[" << order << "] out of [1,20] \n";
    exit(1);
 }

// helixParameters

  double* helixParameters;
  helixParameters = new double[2*order+5];

  helixParameters[0] = 2*order + 4;                     // number of parameters
  helixParameters[1] = length;                          // Length
  helixParameters[2] = k;                               // k
  helixParameters[3] = phase;                           // phase
  helixParameters[4] = gap;                             // gap
  for(i = 1; i <= order ; i++){
      helixParameters[4+i]       = b[i];
      helixParameters[4+order+i] = a[i];
  }

// DA Runge-Kutta integrator

  ZMap map; map = 1.;
  RKIntegrator<ZSeries, ZMap> daIntegrator(helixField, helixParameters, 0.01);
  cout <<  daIntegrator*map;

  delete helixParameters;
  return(1);
}
