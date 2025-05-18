
#include "BunchGenerator.hh"

// Constructor

UAL::BunchGenerator::BunchGenerator()
{
}

// Destructor

UAL::BunchGenerator::~BunchGenerator()
{
}

// Update the bunch distribution by  ellipses with binominal distribution.

void 
UAL::BunchGenerator::addGaussianEllipses(/*inout*/ PAC::Bunch& bunch,			      
					   /*in*/ const PacTwissData& twiss,
					   /*in*/ const PAC::Position& emittance,
					   /*inout*/ int& seed)
{

  double gamma = 0.0, halfWidth1 = 0.0, halfWidth2 = 0.0;

  // Horizontal plane
  if(emittance.getX()) {

    if(!twiss.beta(0)) {
      cerr << "Error: UAL::BunchGenerator : horizontal beta  == 0 \n";
      exit(1);
    }
    gamma = (1 + twiss.alpha(0)*twiss.alpha(0))/twiss.beta(0);

    halfWidth1 = sqrt(twiss.beta(0)*emittance.getX());
    halfWidth2 = sqrt(gamma*emittance.getX());

    addGaussianEllipse1D(bunch, 
			 0,  halfWidth1,
			 1, halfWidth2,
			 twiss.alpha(0), 
			 seed);
  }

  // Vertical plane
  if(emittance.getY()) {

    if(!twiss.beta(1)) {
      cerr << "Error: UAL::BunchGenerator : vertical beta  == 0 \n";
      exit(1);
    }
 
    gamma = (1 + twiss.alpha(1)*twiss.alpha(1))/twiss.beta(1);

    halfWidth1 = sqrt(twiss.beta(1)*emittance.getY());
    halfWidth2 = sqrt(gamma*emittance.getY());

    addGaussianEllipse1D(bunch, 
			 2,  halfWidth1,
			 3, halfWidth2,
			 twiss.alpha(1), 
			 seed);
   
  }

  // Longitudinal plane

  if(emittance.getCT()*emittance.getDE()) {

    halfWidth1 = emittance.getCT()/2.;
    halfWidth2 = emittance.getDE()/2.;

    addGaussianEllipse1D(bunch, 
			 4, halfWidth1,
			 5, halfWidth2,
			 0.0, 
			 seed);
   
  }
}

// Add a binominal distribution in the index1/index2 plane. 

void 
UAL::BunchGenerator::addGaussianEllipse1D(/*inout*/ PAC::Bunch& bunch,
					    /*in*/ int index1,
					    /*in*/ double halfWidth1,
					    /*in*/ int index2,
					    /*in*/ double halfWidth2,
					    /*in*/ double alpha,
					    /*inout*/ int& seed)
{
  if(index1 >= 6) return;
  if(index2 >= 6) return;

  // Number of particles
  int npart = bunch.size();

  double ran1, ran2, a, b, u, v, coschi, sinchi;

  for(int i = 0; i < npart; i++) {

    ran1 = uran(seed);
    if(ran1 == 0.0) ran1 = 1.e-6;

    ran2 = uran(seed);

    a = sqrt(-2.0*log(ran1));

    b = 2.*UAL::pi*ran2;

    u = a*cos(b);
    v = a*sin(b);

    coschi = sqrt(1./(1.+alpha*alpha));
    sinchi = -alpha*coschi;

    bunch[i].getPosition()[index1] += u*halfWidth1;
    bunch[i].getPosition()[index2] += (u*sinchi + v*coschi)*halfWidth2;
  }
}



// Local random number generator

double
UAL::BunchGenerator::uran(int& idum)
{
  return uniformGenerator_.getNumber(idum);
}

