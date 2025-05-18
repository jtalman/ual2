#ifndef UAL_USPAS_ONE_TURN_KICKER_TRACKER_HH
#define UAL_USPAS_ONE_TURN_KICKER_TRACKER_HH

#include <fstream>
#include <list>

#include "UAL/Common/RCIPtr.hh"
#include "UAL/APF/PropagatorNode.hh"
#include "SMF/PacLattElement.h"
#include "PAC/Beam/Bunch.hh"
#include "PAC/Beam/Position.hh"

namespace UAL {

  namespace USPAS {

  /**  */

  class OneTurnKicker : public UAL::PropagatorNode {

  public:

    char* getType(){
     return "JDT_USPAS_OneTurnKicker";
    }

    /** Constructor */
    OneTurnKicker();

    /** Copy constructor */
    OneTurnKicker(const OneTurnKicker& t);

    /** Destructor */
    virtual ~OneTurnKicker();

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

  protected:

    void findMaxSize(PAC::Bunch& bunch, double& xmax, double& ymax);

  };


  class OneTurnKickerRegister 
  {
    public:

    OneTurnKickerRegister(); 
  };

  };
};

#endif
