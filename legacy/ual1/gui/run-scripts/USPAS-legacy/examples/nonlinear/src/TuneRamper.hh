
#ifndef UAL_USPAS_TUNE_RAMPER_HH
#define UAL_USPAS_TUNE_RAMPER_HH

#include "UAL/Common/Def.hh"
#include "UAL/APF/PropagatorComponent.hh"
#include "SMF/PacLattice.h"
#include "Optics/PacTMap.h"
#include "Main/Teapot.h"
#include "TEAPOT/Integrator/BasicTracker.hh"

namespace UAL {

  namespace USPAS {

  /** Map-based tracker. */

  class TuneRamper : public TEAPOT::BasicTracker {

  public:

    /** Constructor */
    TuneRamper();

    /** Copy Constructor */
    TuneRamper(const TuneRamper& st);

    /** Destructor */
    ~TuneRamper();

    /** Returns a deep copy of this object (inherited from UAL::PropagatorNode) */
    UAL::PropagatorNode* clone();

    /** Sets lattice elements and generates matrix (inherited from UAL::PropagatorNode) */
    void setLatticeElements(const UAL::AcceleratorNode& lattice, int i0, int i1, 
			    const UAL::AttributeSet& beamAttributes);

    /** Propagates bunch (inherited from UAL::PropagatorNode) */
    void propagate(UAL::Probe& probe);


  protected:

    double m_q0;
    double m_q1;

  private:

    static PacLattice s_lattice;
    static Teapot     s_teapot;

  private:

  

  };

  class TuneRamperRegister 
  {
    public:
    ZLIB::Space space;
    TuneRamperRegister(); 
  };
  
  }
}

#endif
