// File : copied from TEAPOT/Integrator/RFCavityTracker.hh
// for comparing output via APDF with direct TEAPOT code output

#ifndef UAL_USPAS_TEAPOT_RFCAVITY_TRACKER_HH
#define UAL_USPAS_TEAPOT_RFCAVITY_TRACKER_HH

#include <fstream>
#include <list>

#include "UAL/Common/RCIPtr.hh"
#include "UAL/APF/PropagatorNode.hh"
#include "SMF/PacLattElement.h"
#include "PAC/Beam/Position.hh"

namespace UAL {

  /** TeapotRF Cavity Tracker */

  class TeapotRFCavityTracker : UAL::PropagatorNode {

  public:

    /** Constructor */
    TeapotRFCavityTracker();

    /** Copy Constructor */
    TeapotRFCavityTracker(const TeapotRFCavityTracker& rft);

    /** Destructor */
    virtual ~TeapotRFCavityTracker();

    bool isSequence() { return false; }

    /** Returns a deep copy of this object 
	(inherited from UAL::PropagatorNode) 
    */
    UAL::PropagatorNode* clone();    

    /** Set lattice elements (inherited from UAL::PropagatorNode */
    void setLatticeElements(const UAL::AcceleratorNode& lattice, 
			    int i0, int i1, 
			    const UAL::AttributeSet& beamAttributes);
    /** Returns the first node of the accelerator sector associated 
     *  with this propagator (PropagatorNode method)
     */
    virtual UAL::AcceleratorNode& getFrontAcceleratorNode();

    /** Returns the last node of the accelerator sector associated 
     * with this propagator (PropagatorNode method)
     */
    virtual UAL::AcceleratorNode& getBackAcceleratorNode();

    /** Propagates a bunch of particles */
    void propagate(UAL::Probe& probe);

    /** Sets Rf patameters */
    void setTeapotRF(double V, double harmon, double lag);

  protected:

    /** Sets the lattice element */
    void setLatticeElement(const PacLattElement& e);

    /** Propagates the particle through a drift */
    void passDrift(double l, PAC::Position& p, double v0byc, double vbyc);

  protected:

    /** front node */
    PacLattElement m_frontNode;

    /** back node */
    PacLattElement m_backNode;

    /** Element length */
    double m_l;

    /** Peak RF voltage [GeV] */ 
    double m_V;

    /** Phase lag in multiples of 2 pi */
    double m_lag;

    /** Harmonic number */
    double m_h;

  private:

    void init();
    void copy(const TeapotRFCavityTracker& rft);
    
  };

  class TeapotRFCavityTrackerRegister 
  {
    public:

    TeapotRFCavityTrackerRegister(); 
  };

  }
}

#endif
