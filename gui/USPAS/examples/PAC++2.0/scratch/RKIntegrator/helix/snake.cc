#include "RKIntegrator/RKIntegrator.hh"
#include "Field/Field.hh"

main()
{

  BEAM_DIM   = 4;
  ZLIB_ORDER = 2;

// Helix Parameters ( main field b1)

  double gamma  = 27;
  double BR     = gamma*PROTON/0.3;                  // 1/m

  double length = 2.4;                               // m              
  double k      = 2*PI/length;                       // 1/m
  double B0[4]  = {1.458, 4.0, -4.0, -1.458};        // T

  double helixParameters[7]; 

  helixParameters[0] = 6.;                            // number of parameters
  helixParameters[1] = length;                        // Length
  helixParameters[2] = k;                             // k
  helixParameters[3] = 0.0;                           // phase         
  helixParameters[4] = 0.0;                           // gap
  helixParameters[6] = 0.0;                           // a1

// Drift Parameters

  double drift[5]  = {0.72, 0.32, 0.32, 0.32, 0.72};  // m

// Initialization

  ZSeries psp0;  
  ZMap map;  map = 1.;

  RKIntegrator<ZSeries, ZMap> *helix[4];
  for(int i=0; i < 4; i++){
    helixParameters[5] = -2.*B0[i]/BR/k;              // b1 = -2*B0/BR/k    
    helix[i] = new RKIntegrator<ZSeries, ZMap>(simpleHelixField, helixParameters,0.01);
  }

  if(!helix[3]) {cerr << "No memory \n"; exit(1);}

// DA Runge-Kutta integrator

  for(i=0; i < 5; i++){
     psp0    = sqrt(1. - map[2]*map[2] - map[4]*map[4]);
     psp0    = drift[i]/psp0;
     map[1] += map[2]*psp0; 
     map[3] += map[4]*psp0;
     if(i < 4) map = (*helix[i])*map;
  }

// Print 

  ofstream daFile("helicalSnake.out");
  daFile << map << "\n";
  daFile.close();

  for(i=1; i < 4; i++) delete helix[i];

  return(1);
}
