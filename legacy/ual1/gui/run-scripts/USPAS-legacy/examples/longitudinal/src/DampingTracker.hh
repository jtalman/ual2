#ifndef UAL_USPAS_DAMPING_TRACKER_HH
#define UAL_USPAS_DAMPING_TRACKER_HH

#include <fstream>
#include <list>

#include "UAL/Common/RCIPtr.hh"
#include "UAL/APF/PropagatorNode.hh"
#include "SMF/PacLattElement.h"
#include "PAC/Beam/Position.hh"

namespace UAL {

  namespace USPAS {

  /**  */

  class DampingTracker : public UAL::PropagatorNode {

  public:

    /** Constructor */
    DampingTracker();

    /** Copy constructor */
    DampingTracker(const DampingTracker& t);

    /** Destructor */
    virtual ~DampingTracker();

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

  protected:

    /** front node */
    PacLattElement m_frontNode;

    /** back node */
    PacLattElement m_backNode;

  };


  class DampingTrackerRegister 
  {
    public:

    DampingTrackerRegister(); 
  };

  };
};

#endif
