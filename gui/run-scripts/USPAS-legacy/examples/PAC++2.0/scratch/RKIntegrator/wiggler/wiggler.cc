#include "Map/Map.hh"
#include "RKIntegrator/RKIntegrator.hh"
#include "Field/Field.hh"


main()
{

  BEAM_DIM   = 6;
  ZLIB_DIM   = 6;
  ZLIB_ORDER = 4;

// wiggler Parameter

  double energy  = 5.0;       // GeV 
  double c       = 0.3;       // 1.0e+9 m/c - the speed of light
  double b0      = 1.2;       // T
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

// Initial coordinates
  ZMap z; z = 1;

// DA Runge-Kutta integrator
  RKIntegrator<ZSeries, ZMap> wiggler(wigglerField, p, 0.01);

  ofstream daFile("wiggler.out");
  daFile << wiggler*z << "\n";
  daFile.close();

  return(1);
}
