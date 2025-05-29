
#ifndef UAL_USPAS_MAP_TRACKER_HH
#define UAL_USPAS_MAP_TRACKER_HH

#include "UAL/Common/Def.hh"
#include "UAL/APF/PropagatorComponent.hh"
#include "SMF/PacLattice.h"
#include "Optics/PacTMap.h"
#include "Main/Teapot.h"
#include "TEAPOT/Integrator/BasicTracker.hh"

namespace UAL {

  namespace USPAS {

  /** Map-based tracker. */

  class MapTracker : public TEAPOT::BasicTracker {

  public:

    /** Constructor */
    MapTracker();

    /** Copy Constructor */
    MapTracker(const MapTracker& st);

    /** Destructor */
    ~MapTracker();

    /** Returns a deep copy of this object (inherited from UAL::PropagatorNode) */
    UAL::PropagatorNode* clone();

    /** Sets lattice elements and generates matrix (inherited from UAL::PropagatorNode) */
    void setLatticeElements(const UAL::AcceleratorNode& lattice, int i0, int i1, 
			    const UAL::AttributeSet& beamAttributes);

    /** Propagates bunch (inherited from UAL::PropagatorNode) */
    void propagate(UAL::Probe& probe);

  protected:

    /** Defines map data */
    void setMap(const PacVTps& map);

  protected:

    /** map order */
    int m_order;

    /** sector length */
    double m_l;

    /** Taylor map */
    PacTMap m_map;

  private:

    static PacLattice s_lattice;
    static Teapot     s_teapot;

  private:

    void init();
    void copy(const MapTracker& st);

  };

  class MapTrackerRegister 
  {
    public:
    ZLIB::Space space;
    MapTrackerRegister(); 
  };
  
  }
}

#endif
