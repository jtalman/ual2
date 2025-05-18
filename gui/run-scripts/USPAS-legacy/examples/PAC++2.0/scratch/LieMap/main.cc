#include <time.h>
#include "RKIntegrator/RKIntegrator.hh"
#include "LieMap/LieMap.hh"
#include "Field/Field.hh"
#include "VectorA/VectorA.hh"

main()
{
  int i;
  BEAM_DIM   = 4;
  ZLIB_ORDER = 4;

// helixParameters

  double gamma  = 27;
  double BR     = gamma*PROTON/0.3;
  double length = 2.4;

  int nPieces = 100;

  double helixParameters[7]; 

  helixParameters[0] = 6.;                              // number of parameters
  helixParameters[1] = length;                          // Length
  helixParameters[2] = 2*PI/helixParameters[1];         // k 
  helixParameters[3] = 0.;
  helixParameters[4] = 0.;                              // gap
  helixParameters[5] = -2.*1.458/BR/helixParameters[2]; // -2*B0/BR/k
  helixParameters[6] = 0.0;

// Initial coordinates

  Particle p;

  ZMap     map;
  time_t   now;


// Lie Transformation

  map = 1.;
  LieMap lieMap(simpleHelixVectorA, helixParameters);
  cerr << lieMap(20)*map;

  cerr << "Lie Transformation \n";
  now = time(NULL), printf("%s", ctime(&now));

  helixParameters[1] = length/nPieces;
  for(i=1; i <= 1; i++){
      helixParameters[3] = helixParameters[2]*helixParameters[1]*(i-1);
      map = lieMap(simpleHelixVectorA,helixParameters,20)*map;
  }

  now = time(NULL), printf("%s", ctime(&now));
  cerr << "End\n";

  ofstream daFile("helix.out");
  daFile << map << "\n";
  daFile.close();

  return(1);
}
