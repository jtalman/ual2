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
//../../../../codes/UAL/src/UAL/Common/Def.hh
#include                              "Def.h"

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

void octantDF(){                                // ring method
 printf("%soctantDF()\n",thr);
 QDcE_lsh();                                       // a sector method in this ring method
 SBPl();                                           // a sector method in this ring method
 mSBPlMi.marker_track(&mSBPlMi,"      ");          // marker
 mSixteenth.marker_track(&mSixteenth,"      ");    // marker
 SBMi();                                           // a sector method in this ring method
 lsh_QFcE();                                       // a sector method in this ring method
}

void octantFD(){                                // ring method
 printf("%soctantFD()\n",thr);
 QFcE_lsh();                                       // a sector method in this ring method
 SBMi();                                           // a sector method in this ring method
 mSBMiPl.marker_track(&mSBMiPl,"      ");          // marker
 mSixteenth.marker_track(&mSixteenth,"      ");    // marker
 SBPl();                                           // a sector method in this ring method
 lsh_QDcE();                                       // a sector method in this ring method
}

void main(){
//#include"constants.work"
//#include"print-constants"
//#include"Def.h"
//#include"print-UAL-Def"
 
 printf("%s:\n", "Ring");
 printf("   %s:AT:: %+e %+e %+e %+e %+e %+e %+e\n", "INJECT    ",AT,dx,dxp,dy,dyp,dct,de);

  #include"C-Ring"                              // Ring (and/or "lattice")
/*
 mbegin.marker_track(&mbegin,"   ");            // marker
 RF1.rfcavity_track(&RF1);                      // rfcavity
 octantDF();                                    // ring method
 octantFD();                                    // ring method
 mQuarter.marker_track(&mQuarter,"   ");        // marker
 octantDF();                                    // ring method
 octantFD();                                    // ring method
 mHalf.marker_track(&mHalf,"   ");              // marker
 octantDF();                                    // ring method
 octantFD();                                    // ring method
 mQuarter.marker_track(&mQuarter,"   ");        // marker
 octantDF();                                    // ring method
 octantFD();                                    // ring method
 mend.marker_track(&mend,"   ");                // marker
*/
}
