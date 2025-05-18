
#ifndef NM_TRACKER_HH
#define NM_TRACKER_HH

#include "UAL/Common/Def.hh"
#include "UAL/APF/PropagatorComponent.hh"
#include "SMF/PacLattice.h"
#include "Optics/PacTMap.h"
#include "Main/Teapot.h"
#include "TEAPOT/Integrator/BasicTracker.hh"


namespace NM {

  /** Map-based tracker. */

  class Tracker  {

  public:

    /** Constructor */
    Tracker();

    /** Copy Constructor */
    Tracker(const Tracker& st);

    /** Destructor */
    ~Tracker();

    /** Sets lattice elements and generates matrix (inherited from UAL::PropagatorNode) */
    void setLatticeElements(const UAL::AcceleratorNode& lattice, int i0, int i1, 
			    const UAL::AttributeSet& beamAttributes);

    /** Propagates bunch (inherited from UAL::PropagatorNode) */
    void propagate(UAL::Probe& probe);

  protected:

    void makeVelocity(PAC::Position& p, PAC::Position& tmp, double v0byc);
    void makeRV(PAC::Position& p, PAC::Position& tmp,double e0, double p0,double m0);
    void passDrift(double rlipl, PAC::Position& p, PAC::Position& tmp, double v0byc);

  protected:

    /** sector length */
    double m_l;

  };


  }

#endif
