#include <time.h>
#include "Teapot/Teapot.hh"

main()
{

  BEAM_DIM = 6;
  ENERGY   = 5.0*GEV;
  MASS     = ELECTRON;

  // Initialize Teapot
  Teapot cesr("fort.7");


  // Initialize particles
  Particle p[2];
  int i, j;
  for(j=0; j < 2; j++){
     for(int i=1; i <= 6;   i++) p[j][i] = 0.0;
     p[j][1] = 0.7e-3;
     p[j][3] = 0.1e-3;
  }
  p[1][6] = 0.1e-3;

  // Tracking
  int nelem = cesr.numberElements();

  int iw1 = cesr.elemNumber("wigw1");
  int iw2 = cesr.elemNumber("wigw2");

  for(j=0; j < 2; j++) {
        p[j] = cesr.gentrk(p[j], 0,   iw1);      // 0       -> wiggler
        p[j] = cesr.gentrk(p[j], iw1, iw2);      // wiggler
        p[j] = cesr.gentrk(p[j], iw2, nelem+1);  // wiggler -> end
        cerr << p[j] << "\n";
  }
  return(1);
}
