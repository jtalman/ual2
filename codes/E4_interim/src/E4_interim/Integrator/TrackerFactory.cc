#include "UAL/APF/PropagatorFactory.hh"
#include "E4_interim/Integrator/TrackerFactory.hh"

#include"E4_interim/Integrator/genMethods/spinExtern"

E4_interim::TrackerFactory* E4_interim::TrackerFactory::s_theInstance = 0;

//int E4_interim::marker::turns=0;
//int E4_interim::marker::markerCount=0;
//double E4_interim::bend::dZFF;
//double E4_interim::bend::m_m;
//int E4_interim::bend::bnd=0;
//int E4_interim::bend::bndsPerTrn;
//double E4_interim::bend::spin[41][3];

E4_interim::TrackerFactory::TrackerFactory()
{
/*
  UAL::PropagatorNodePtr dipolePtr(new E4_interim::bend());
  m_trackers["Sbend"] = dipolePtr;
  m_trackers["Rbend"] = dipolePtr;

//UAL::PropagatorNodePtr mltPtr(new E4_interim::mlt());
  UAL::PropagatorNodePtr quadPtr(new E4_interim::quad());
  UAL::PropagatorNodePtr sextPtr(new E4_interim::sext());
  UAL::PropagatorNodePtr octPtr(new E4_interim::oct());
//m_trackers["Kicker"]     = mltPtr;
//m_trackers["Hkicker"]    = mltPtr;  
//m_trackers["Vkicker"]    = mltPtr;
  m_trackers["Quadrupole"] = quadPtr;
  m_trackers["Sextupole"]  = sextPtr;  
  m_trackers["Octupole"]  = octPtr;  
//m_trackers["Multipole"]  = mltPtr;

*/
  UAL::PropagatorNodePtr rfPtr(new E4_interim::rfCavity());
  m_trackers["RfCavity"]   = rfPtr;

}

E4_interim::TrackerFactory* E4_interim::TrackerFactory::getInstance()
{
  if(s_theInstance == 0){
    s_theInstance = new E4_interim::TrackerFactory();
  }
  return s_theInstance;
}

UAL::PropagatorNode* E4_interim::TrackerFactory::createTracker(const std::string& type)
{
  E4_interim::TrackerFactory* factory = getInstance(); 

  std::map<std::string, UAL::PropagatorNodePtr>::const_iterator it = factory->m_trackers.find(type);
  if(it == factory->m_trackers.end()) return createrfCavity();
//if(it == factory->m_trackers.end()) return createDefaultTracker();
  return it->second->clone();  
}

/*
ETEAPOT::BasicTracker* E4_interim::TrackerFactory::createDefaultTracker()
{
  return new E4_interim::drift();
}

E4_interim::marker* E4_interim::TrackerFactory::createmarker()
{
  return new E4_interim::marker();
}

E4_interim::drift* E4_interim::TrackerFactory::createdrift()
{
  return new E4_interim::drift();
}

E4_interim::bend* E4_interim::TrackerFactory::createbend()
{
  return new E4_interim::bend();
}

E4_interim::quad* E4_interim::TrackerFactory::createquad()
{
  return new E4_interim::quad();
}

E4_interim::sext* E4_interim::TrackerFactory::createsext()
{
  return new E4_interim::sext();
}

E4_interim::oct* E4_interim::TrackerFactory::createoct()
{
  return new E4_interim::oct();
}
*/

E4_interim::rfCavity* E4_interim::TrackerFactory::createrfCavity()
{
  return new E4_interim::rfCavity();
}

// E4_interim::MatrixTracker* E4_interim::TrackerFactory::createMatrixTracker()
// {
//  return new E4_interim::MatrixTracker();
// }

E4_interim::TrackerRegister::TrackerRegister()
{
/*
  UAL::PropagatorNodePtr markerPtr(new E4_interim::marker());
  UAL::PropagatorFactory::getInstance().add("E4_interim::marker", markerPtr);

  UAL::PropagatorNodePtr driftPtr(new E4_interim::drift());
  UAL::PropagatorFactory::getInstance().add("E4_interim::drift", driftPtr);

  UAL::PropagatorNodePtr dipolePtr(new E4_interim::bend());
  UAL::PropagatorFactory::getInstance().add("E4_interim::bend", dipolePtr);

  UAL::PropagatorNodePtr quadPtr(new E4_interim::quad());
  UAL::PropagatorFactory::getInstance().add("E4_interim::quad", quadPtr);

  UAL::PropagatorNodePtr sextPtr(new E4_interim::sext());
  UAL::PropagatorFactory::getInstance().add("E4_interim::sext", sextPtr);

  UAL::PropagatorNodePtr octPtr(new E4_interim::oct());
  UAL::PropagatorFactory::getInstance().add("E4_interim::oct", octPtr);

  // UAL::PropagatorNodePtr matrixPtr(new E4_interim::MatrixTracker());
  // UAL::PropagatorFactory::getInstance().add("E4_interim::MatrixTracker", matrixPtr);
*/

  UAL::PropagatorNodePtr rfPtr(new E4_interim::rfCavity());
  UAL::PropagatorFactory::getInstance().add("E4_interim::rfCavity", rfPtr);
}

static E4_interim::TrackerRegister theSingleton; 
