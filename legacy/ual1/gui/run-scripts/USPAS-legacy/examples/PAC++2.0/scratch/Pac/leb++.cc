//
// leb++.cc
//

#include <time.h>
#include "Pac/Pac.hh"

double staticGenerator(int& s)
{
  return(1.);
}

main()
{

#include "LEB.hh"

   time_t now;

// Assignment

   Pac lebT("LEB: Pac implementation");
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
}
