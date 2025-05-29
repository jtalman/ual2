#ifndef UAL_CM_TRACKER_HH
#define UAL_CM_TRACKER_HH

#include "UAL/Common/Probe.hh"
#include "Optics/PacVTps.h"
#include "Optics/PacTwissData.h"


  class CMTracker {

  public:

    /** Constructor */
    CMTracker();

    /** Defines twiss data */
    void setTwiss(const PacTwissData& twiss);

    /** Defines chromaticities */
    void setChromaticities(double chx, double chy);

    /** Defines tunes */
    void setTunes(double qx, double qy);

    void setRF(double V, double lag, double h);

    void setLattice(double suml, double alpha);

    /** Propagates a bunch of particles */
    void propagate(UAL::Probe& probe);

  private:

    PacTwissData m_twiss;

    double m_chx, m_chy;

    double m_V, m_lag, m_h;

    double m_suml, m_alpha;

  private:

    void init();

  };


#endif
