#ifndef UAL_E4_interim_TRACKER_FACTORY_HH
#define UAL_E4_interim_TRACKER_FACTORY_HH

#include <map>

#include "UAL/APF/PropagatorNodePtr.hh"

/*
#include "E4_interim/Integrator/drift.cc"
#include "E4_interim/Integrator/bend.cc"
#include "E4_interim/Integrator/marker.cc"
#include "E4_interim/Integrator/quad.cc"
#include "E4_interim/Integrator/sext.cc"
#include "E4_interim/Integrator/oct.cc"
*/
#include "E4_interim/Integrator/rfCavity.cc"

namespace{
#include "E4_interim/Integrator/genMethods/spinExtern"
#include "E4_interim/Integrator/genMethods/designExtern"
#include "E4_interim/Integrator/genMethods/bunchParticleExtern"
}

namespace E4_interim {

  /** Factory of the E4_interim Trackers */

  class TrackerFactory {

  public:

    /** Returns singleton */
    static TrackerFactory* getInstance();

    /** Returns the tracker specified by the element type */
    static UAL::PropagatorNode* createTracker(const std::string& type);

    /** Returns the default tracker */
/*
    static ETEAPOT::BasicTracker* createDefaultTracker();

    static marker* createmarker();

    static drift* createdrift();

    static bend* createbend();

    static quad* createquad();
    static sext* createsext();
    static oct* createoct();
*/

    /** Returns the rf cavity tracker */
    static rfCavity* createrfCavity();

    /** Returns the matrix tracker */
//  static MatrixTracker* createMatrixTracker();

  private:

    static TrackerFactory* s_theInstance;
    std::map<std::string, UAL::PropagatorNodePtr> m_trackers;

  private:

    // constructor
    TrackerFactory();

  };

  class TrackerRegister 
  {
    public:

    TrackerRegister(); 
  };


}

#endif
