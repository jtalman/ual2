#include "Map/Map.hh"
#include "RKIntegrator/RKIntegrator.hh"
#include "Field/Field.hh"


main()
{

  BEAM_DIM   = 4;
  ZLIB_DIM   = 4;
  ZLIB_ORDER = 4;

// wiggler Parameter

  double energy  = 5.0;       // GeV 
  double c       = 0.3;       // 1.0e+9 m/c - the speed of light
  double b0_1    = 0.0,       // T
         b0_2    = 0.0,       // T
         b0      = 1.2;       // T
  double lambda  = 0.196;     // m
  double kx      = 6.4;       // 1/m

  double drift1 = 2.13455 - lambda/2,
         drift2 = 0.95090;

  double p[7]; 

  p[0] = 6.;                           // number of parameters
  p[1] = lambda;                       // Length
  p[2] = kx;                           // k_x 
  p[4] = 2*PI/lambda;                  // k_z 
  p[3] = sqrt(p[4]*p[4] - p[2]*p[2]);  // k_y
  p[5] = -PI;                          // phase
  p[6] = b0*c/energy;                  // B0/BR

// Initial coordinates
  ZSeries psp0;
  ZMap map; map = 1; 

// First Drift
   psp0    = sqrt(1. - map[2]*map[2] - map[4]*map[4]);
   psp0    = drift1/psp0;
   map[1] += map[2]*psp0;
   map[3] += map[4]*psp0;

// 1st pole
  p[1] = lambda/2.;
  p[5] = +PI/4;
  p[6] = b0_1*c/energy;
  RKIntegrator<ZSeries, ZMap> pole1st(wigglerField, p, 0.01);
  map = pole1st*map;

// 2nd  pole
  p[1] = lambda/2.;
  p[5] = -PI/4;
  p[6] = b0_2*c/energy;
  RKIntegrator<ZSeries, ZMap> pole2nd(wigglerField, p, 0.01);
  map = pole2nd*map;

// 11 periods
  p[1] = lambda;
  p[5] = +PI/4;
  p[6] = b0*c/energy;
  RKIntegrator<ZSeries, ZMap> period(wigglerField, p, 0.01);
  
  ZMap z, mapPeriod; z = 1;
  mapPeriod  = period*z; 

  ofstream daFileBefore("mapBefore.out");
  daFileBefore << map << "\n";
  daFileBefore.close();

  ofstream daFilePeriod("wigglerPeriod.out");
  daFilePeriod << mapPeriod << "\n";
  daFilePeriod.close();

  for(int i=0; i < 1; i++)
    map *= mapPeriod;

  ofstream daFileAfter("mapAfter.out");
  daFileAfter << map << "\n";
  daFileAfter.close();


// Last pole
  map = pole1st*map;

// Last Drift
  psp0    = sqrt(1. - map[2]*map[2] - map[4]*map[4]);
  psp0    = drift2/psp0;
  map[1] += map[2]*psp0;
  map[3] += map[4]*psp0;

  ofstream daFile("wiggler25.out");
  daFile << map << "\n";
  daFile.close();

  return(1);
}
