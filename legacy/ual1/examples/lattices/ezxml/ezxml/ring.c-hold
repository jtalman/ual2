#include <stdio.h>

double dx=1e-6,dxp=0,dy=0,dyp=0,dct=0,de=0;

double AT=0;

#include"C-tracking-methods"              // lattice independent 

#include"C-lattice-elements"              // lattice dependent

#include"C-ring-methods"                  // lattice dependent

void main(){
 printf("%s:\n", "RING");
 printf("   %s:AT:: %+e %+e %+e %+e %+e %+e %+e\n", "INJECT    ",AT,dx,dxp,dy,dyp,dct,de);

#include"C-ring-turn-sequence"            // lattice dependent

}
