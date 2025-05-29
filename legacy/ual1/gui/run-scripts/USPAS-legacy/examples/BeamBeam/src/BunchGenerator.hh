#ifndef UAL_BUNCH_GENERATOR_HH
#define UAL_BUNCH_GENERATOR_HH

#include "UAL/Common/Def.hh"
#include "PAC/Beam/Bunch.hh"
#include "Optics/PacTwissData.h"
#include "ACCSIM/Base/Def.hh"
#include "ACCSIM/Base/UniformGenerator.hh"

namespace UAL {


  class BunchGenerator 
  {

  public:

    /** Constructor */
    BunchGenerator();  

    /** Destructor */
    virtual ~BunchGenerator();
 

    /** Updates the bunch distribution by  ellipses with Gaussian distribution. */
    void addGaussianEllipses(PAC::Bunch& bunch,
			     const PacTwissData& twiss,
			     const PAC::Position& emittance,
			     int& seed);

    // Add a Gaussian distribution in the index1/index2 plane. 
    void addGaussianEllipse1D(/*inout*/ PAC::Bunch& bunch,
			    /*in*/ int index1,
			    /*in*/ double halfWidth1,
			    /*in*/ int index2,
			    /*in*/ double halfWidth2,
			    /*in*/ double alpha,
			    /*inout*/ int& seed);

  private:

    // Invoke local random number generator
    double uran(int& seed);

  private:

    ACCSIM::UniformGenerator   uniformGenerator_;

  };
};

#endif

