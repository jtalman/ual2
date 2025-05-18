#include "Map/Map.hh"
#include "RKIntegrator/RKIntegrator.hh"
#include "Field/Field.hh"


main()
{

  BEAM_DIM   = 4;
  ZLIB_DIM   = 4;
  ZLIB_ORDER = 4;

// Initial coordinates
// Wiggler 
 
  double energy  = 5.0;       // GeV 
  double c       = 0.3;       // 1.0e+9 m/c - the speed of light
  double b0      = 0.0;       // T
  double lambda  = 0.196;     // m
  double kx      = 6.4;       // 1/m

  double p[7]; 

  p[0] = 6.;                           // number of parameters
  p[1] = lambda;                       // Length
  p[2] = kx;                           // k_x 
  p[4] = 2*PI/lambda;                  // k_z 
  p[3] = sqrt(p[4]*p[4] - p[2]*p[2]);  // k_y
  p[5] = -PI;                          // phase
  p[6] = b0*c/energy;                  // B0/BR

// Drifts 

  double d1 = 2.13455,
         d2 = 0.95090;

  double pd1[2], pd2[2];

  pd1[0] = 1.;                           // number of parameters
  pd1[1] = d1;                           // Length

  pd2[0] = 1.;                           // number of parameters
  pd2[1] = d2;                           // Length

// Map
  ZMap map; map = 1; 

// First Drift 

  RKIntegrator<ZSeries, ZMap> drift1(driftField, pd1, 1.0); 
  map = drift1*map;  

// 12 periods
  RKIntegrator<ZSeries, ZMap> period(wigglerField, p, 0.01);
  
  ZMap z, mapPeriod; z = 1;
  mapPeriod  = period*z; 

  for(int i=0; i < 12; i++)
    map *= mapPeriod;

// Last Drift

  RKIntegrator<ZSeries, ZMap> drift2(driftField, pd2, 1.0); 
  map = drift2*map;  

  ofstream daFile("wiggler24.out");
  daFile << map << "\n";
  daFile.close();

  return(1);
}
