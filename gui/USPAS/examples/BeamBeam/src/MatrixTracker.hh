#ifndef UAL_MATRIX_TRACKER_HH
#define UAL_MATRIX_TRACKER_HH

#include "UAL/Common/Probe.hh"
#include "Optics/PacVTps.h"
#include "Optics/PacTwissData.h"

namespace UAL {

  class MatrixTracker {

  public:

    /** Constructor */
    MatrixTracker();

    /** Defines matrix data */
    void setMap(const PacVTps& vtps);

    /** Defines twiss data */
    void setTwiss(const PacTwissData& twiss);

    /** Defines tunes */
    void setTunes(double qx, double qy);

    /** Propagates a bunch of particles */
    void propagate(UAL::Probe& probe);

  private:

    // double m_l;

    double a10, a11, a12, a13, a14, a15, a16;
    double a20, a21, a22, a23, a24, a25, a26;
    double a30, a31, a32, a33, a34, a35, a36;
    double a40, a41, a42, a43, a44, a45, a46;   
    double a50, a51, a52, a53, a54, a55, a56;
    double a60, a61, a62, a63, a64, a65, a66;  

    PacTwissData m_twiss;

  private:

    void init();

  };

};

#endif
