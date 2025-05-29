#include "UAL/APF/PropagatorFactory.hh"
#include "TrackerFactory.hh"

#include"ETEAPOT2/Integrator/genMethods/spinExtern"

E_m::TrackerFactory* E_m::TrackerFactory::s_theInstance = 0;

E_m::TrackerFactory::TrackerFactory()
{
  UAL::PropagatorNodePtr compromise_quadPtr(new E_m::compromise_quad());
  m_trackers["Quadrupole"] = compromise_quadPtr;

}

E_m::TrackerFactory* E_m::TrackerFactory::getInstance()
{
  if(s_theInstance == 0){
    s_theInstance = new E_m::TrackerFactory();
  }
  return s_theInstance;
}

UAL::PropagatorNode* E_m::TrackerFactory::createTracker(const std::string& type)
{
  E_m::TrackerFactory* factory = getInstance(); 

  std::map<std::string, UAL::PropagatorNodePtr>::const_iterator it = factory->m_trackers.find(type);
  if(it == factory->m_trackers.end()) return createcompromise_quad();
//if(it == factory->m_trackers.end()) return createrfCavity();
//if(it == factory->m_trackers.end()) return createDefaultTracker();
  return it->second->clone();  
}

/*
ETEAPOT::BasicTracker* E_m::TrackerFactory::createDefaultTracker()
{
  return new E_m::drift();
}

E_m::marker* E_m::TrackerFactory::createmarker()
{
  return new E_m::marker();
}

E_m::drift* E_m::TrackerFactory::createdrift()
{
  return new E_m::drift();
}

E_m::bend* E_m::TrackerFactory::createbend()
{
  return new E_m::bend();
}

E_m::compromise_quad* E_m::TrackerFactory::createcompromise_quad()
{
  return new E_m::compromise_quad();
}

E_m::sext* E_m::TrackerFactory::createsext()
{
  return new E_m::sext();
}

E_m::oct* E_m::TrackerFactory::createoct()
{
  return new E_m::oct();
}
*/

/*
E_m::rfCavity* E_m::TrackerFactory::createrfCavity()
{
  return new E_m::rfCavity();
}
*/

// E_m::MatrixTracker* E_m::TrackerFactory::createMatrixTracker()
// {
//  return new E_m::MatrixTracker();
// }

E_m::compromise_quad* E_m::TrackerFactory::createcompromise_quad()
{
  return new E_m::compromise_quad();
}

E_m::TrackerRegister::TrackerRegister()
{
/*
  UAL::PropagatorNodePtr markerPtr(new E_m::marker());
  UAL::PropagatorFactory::getInstance().add("E_m::marker", markerPtr);

  UAL::PropagatorNodePtr driftPtr(new E_m::drift());
  UAL::PropagatorFactory::getInstance().add("E_m::drift", driftPtr);

  UAL::PropagatorNodePtr dipolePtr(new E_m::bend());
  UAL::PropagatorFactory::getInstance().add("E_m::bend", dipolePtr);

  UAL::PropagatorNodePtr compromise_quadPtr(new E_m::compromise_quad());
  UAL::PropagatorFactory::getInstance().add("E_m::compromise_quad", compromise_quadPtr);

  UAL::PropagatorNodePtr sextPtr(new E_m::sext());
  UAL::PropagatorFactory::getInstance().add("E_m::sext", sextPtr);

  UAL::PropagatorNodePtr octPtr(new E_m::oct());
  UAL::PropagatorFactory::getInstance().add("E_m::oct", octPtr);

  // UAL::PropagatorNodePtr matrixPtr(new E_m::MatrixTracker());
  // UAL::PropagatorFactory::getInstance().add("E_m::MatrixTracker", matrixPtr);
*/

/*
  UAL::PropagatorNodePtr rfPtr(new E_m::rfCavity());
  UAL::PropagatorFactory::getInstance().add("E_m::rfCavity", rfPtr);
*/

  UAL::PropagatorNodePtr compromise_quadPtr(new E_m::compromise_quad());
  UAL::PropagatorFactory::getInstance().add("E_m::compromise_quad", compromise_quadPtr);
}

static E_m::TrackerRegister theSingleton; 
