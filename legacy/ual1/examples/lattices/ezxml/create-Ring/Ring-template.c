/*
The Ring is made up of:
   - markers
   - rfcavity
   - ring methods
A ring method is made up of:
   - markers
   - sector methods
A sector method is made up of:
   - markers and other elements (drifts, quadrupoles, sbends)
*/

#include <stdio.h>

char*thr="   ";
char*six="      ";
char*nne="         ";

double dx=1e-6,dxp=0,dy=0,dyp=0,dct=0,de=0;     // initial/injection probe phase space coordinates

double AT=0;                                    // injection probe s position
                                                // s is integrated longitudinal distance along probe path
                                                // $UAL/examples/ETEAPOT/statusAndRegression/Cornell.pdf
                                                // page 115

#include"C-element-tracking-methods"            // algorithms

#include"C-lattice-elements"                    // elements in this lattice, and their types

#include"C-sector-methods"

void main(){
 printf("%s:\n", "Ring");
 printf("   %s:AT:: %+e %+e %+e %+e %+e %+e %+e\n", "INJECT    ",AT,dx,dxp,dy,dyp,dct,de);

  #include"C-Ring"                              // Ring (and/or "lattice")
}
