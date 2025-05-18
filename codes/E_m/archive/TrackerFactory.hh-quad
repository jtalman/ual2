#include <map>

#include "UAL/APF/PropagatorNodePtr.hh"

//#include "E_m/rfCavity.cc"
#include "quad.cc"

namespace{
#include "ETEAPOT2/Integrator/genMethods/spinExtern"
#include "ETEAPOT2/Integrator/genMethods/designExtern"
#include "ETEAPOT2/Integrator/genMethods/bunchParticleExtern"
}

namespace E_m {

  /** Factory of the E_m Trackers */

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

    static quad* createquad();

    /** Returns the rf cavity tracker */
//  static rfCavity* createrfCavity();

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
