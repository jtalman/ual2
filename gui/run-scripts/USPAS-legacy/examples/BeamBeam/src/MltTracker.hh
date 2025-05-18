#ifndef UAL_MLT_TRACKER_HH
#define UAL_MLT_TRACKER_HH

#include "UAL/Common/Probe.hh"
#include "Optics/PacVTps.h"
#include "Optics/PacTwissData.h"


  class MltTracker {

  public:

    /** Constructor */
    MltTracker();

    /** Sets twiss */
    void setTwiss(const PacTwissData& twissData);   

    /** Defines sextupole */
    void setSextupole(double kl2);

    /** Propagates a bunch of particles */
    void propagate(UAL::Probe& probe);

  private:

    PacTwissData m_twiss;

    double m_kl2;

  private:

    void init();

  };


#endif
