#ifndef UAL_BB_TRACKER_HH
#define UAL_BB_TRACKER_HH

#include "UAL/Common/Probe.hh"
#include "PAC/Beam/Bunch.hh"
#include "Optics/PacVTps.h"
#include "Optics/PacTwissData.h"

namespace UAL {

  class BBTracker {

  public:

    /** Constructor */
    BBTracker();

    /** Defines a core bunch */
    void setCoreBunch(double n, double s);

    /** Defines a tail bunch */
    void setTailBunch(double n, double s);

    /** Propagates a bunch of particles */
    void propagate(UAL::Probe& probe);

  protected:

    void propagate(PAC::Bunch& bunch, double N, double sigma2);   

  private:

    double m_n1;
    double m_s1;

    double m_n2;   
    double m_s2;

  private:

    void init();

  };

};

#endif
