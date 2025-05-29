//
// leb++.cc
//

#include <time.h>
#include "Teapot/Teapot.hh"
#include "Zmap/Zmap.hh"

double staticGenerator(int& s)
{
  return(1.);
}

main()
{

#include "LEB.hh"

   time_t now;

// Assignment

   Teapot lebT("LEB: Teapot implementation");   
   lebT = leb;
   cerr << lebT.element(lebT.numberElements());

// Set Error

   cerr <<  "Set Error\n";
   now = time(NULL); printf("%s", ctime(&now));

   int seed = 1;
   lebT.setError(seed, staticGenerator, hb);
   lebT.setError(seed, staticGenerator, qf1h*qf2h*qd1h*qd2h);
   lebT.setError(seed, staticGenerator, qfs1h*qfs2h*qds1h*qds2h);

   now = time(NULL); printf("%s", ctime(&now));
   cerr <<  "End\n";

   cerr << lebT.element(lebT.numberElements());

// Tracking 
 
   Particle p;
   for(int i=1; i <= 6; i++) p[i] = 1.0e-3;

   cerr <<  "Tracking\n";
   now = time(NULL); printf("%s", ctime(&now));

   for(i=1; i <= 100; i++)   p = lebT*p;

   now = time(NULL); printf("%s", ctime(&now));
   cerr <<  "End\n";

   cerr << p;

// Extract one-turn Taylor Map

   ZLIB_ORDER = 5;

   cerr <<  "Zmap\n";
   now = time(NULL); printf("%s", ctime(&now));
   Zmap lebZ = lebT;
   now = time(NULL); printf("%s", ctime(&now));
   cerr << lebZ;
   
}
