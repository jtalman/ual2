#include <math.h>

#include "UAL/APF/PropagatorFactory.hh"
#include "MapTracker.hh"

PacLattice UAL::USPAS::MapTracker::s_lattice;
Teapot     UAL::USPAS::MapTracker::s_teapot;

UAL::USPAS::MapTracker::MapTracker()
  : TEAPOT::BasicTracker(), m_map(6)
{
  init();
}

UAL::USPAS::MapTracker::MapTracker(const UAL::USPAS::MapTracker& st)
  : TEAPOT::BasicTracker(st), m_map(st.m_map)
{
  copy(st);
}

UAL::USPAS::MapTracker::~MapTracker()
{
}

UAL::PropagatorNode* UAL::USPAS::MapTracker::clone()
{
  return new UAL::USPAS::MapTracker(*this);
}

void UAL::USPAS::MapTracker::setLatticeElements(const UAL::AcceleratorNode& sequence, 
						int is0, int is1,
						const UAL::AttributeSet& attSet)
{
  TEAPOT::BasicTracker::setLatticeElements(sequence, is0, is1, attSet);

  const PacLattice& lattice     = (PacLattice&) sequence;
  PAC::BeamAttributes ba = (PAC::BeamAttributes&) attSet;

  // Initialize lattice 
  if(s_lattice.name() != lattice.name()){
    s_lattice = lattice;
    s_teapot.use(lattice);
  }

  // Calculate length
  m_l = 0;
  for(int i = is0; i < is1; i++){
    m_l += lattice[i].getLength(); 
  }

  // Propagate the sector map

  PacTMap sectorMap(6);
  int oldMltOrder = sectorMap.mltOrder();

  sectorMap.mltOrder(m_order);
  s_teapot.trackMap(sectorMap, ba, is0, is1); 

  sectorMap.mltOrder(oldMltOrder);

  setMap(sectorMap);
}


void UAL::USPAS::MapTracker::setMap(const PacVTps& map)
{
  m_map = map;
}

void UAL::USPAS::MapTracker::propagate(UAL::Probe& probe)
{
  PAC::Bunch& bunch = static_cast<PAC::Bunch& >(probe);

  PAC::BeamAttributes& ba = bunch.getBeamAttributes();

  double e0    = ba.getEnergy(), m0 = ba.getMass();
  double p0    = sqrt(e0*e0 - m0*m0);
  double v0byc = p0/e0;
  double oldT  = ba.getElapsedTime();

  m_map.setEnergy(e0);
  m_map.setMass(m0);

  int size = bunch.size();
  for(int i=0; i < size; i++){

    if(bunch[i].isLost()) continue;

    PAC::Position& pos = bunch[i].getPosition();
    m_map.propagate(pos);        
  }

  ba.setElapsedTime(oldT + m_l/v0byc/UAL::clight);  

}

void UAL::USPAS::MapTracker::init()
{
  m_order = 5;
  m_l     = 0.0;
}

void UAL::USPAS::MapTracker::copy(const UAL::USPAS::MapTracker& mt)
{
  m_order = mt.m_order;
  m_l     = mt.m_l;
}

UAL::USPAS::MapTrackerRegister::MapTrackerRegister()
  : space(6, 5)
{
  UAL::PropagatorNodePtr nodePtr(new UAL::USPAS::MapTracker());
  UAL::PropagatorFactory::getInstance().add("UAL::USPAS::MapTracker", nodePtr);
}

static UAL::USPAS::MapTrackerRegister the_UAL_USPAS_MapTracker_Register; 




