#include "RKIntegrator/RKIntegrator.hh"
#include "Field/Field.hh"

main()
{

  BEAM_DIM   = 4;
  ZLIB_ORDER = 2;

// Helix Parameters ( main field b1)

  double gamma  = 27;
  double BR     = gamma*PROTON/0.3;      // 1/m

  double length = 2.4;                   // m              
  double k      = 2*PI/length;           // 1/m
  double B0     = 1.458;                 // T

  double helixParameters[7]; 

  helixParameters[0] = 6.;               // number of parameters
  helixParameters[1] = length;           // Length
  helixParameters[2] = k;                // k
  helixParameters[3] = 0.0;              // phase         
  helixParameters[4] = 0.0;              // gap
  helixParameters[5] = -2.*B0/BR/k;      // b1 = -2*B0/BR/k 
  helixParameters[6] = 0.0;              // a1

// Initialization
 
  ZMap z;  z = 1.;
  RKIntegrator<ZSeries, ZMap> helix(helixField, helixParameters, 0.01);

// DA Runge-Kutta integrator

  z = helix*z;

// Print 

  ofstream daFile("helicalSnake.out");
  daFile << z << "\n";
  daFile.close();

  return(1);
}
