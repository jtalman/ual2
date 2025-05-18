
#include "UAL/APF/PropagatorFactory.hh"
#include "PAC/Beam/Bunch.hh"
#include "SMF/PacLattice.h"

#include "DampingTracker.hh"

UAL::USPAS::DampingTracker::DampingTracker() 
{
}

UAL::USPAS::DampingTracker::DampingTracker(const UAL::USPAS::DampingTracker& t)
{
}

UAL::USPAS::DampingTracker::~DampingTracker()
{
}

UAL::PropagatorNode* UAL::USPAS::DampingTracker::clone()
{
  return new UAL::USPAS::DampingTracker(*this);
}

UAL::AcceleratorNode& UAL::USPAS::DampingTracker::getFrontAcceleratorNode()
{
  return m_frontNode;
}

UAL::AcceleratorNode& UAL::USPAS::DampingTracker::getBackAcceleratorNode()
{
  return m_backNode;
}

void UAL::USPAS::DampingTracker::setLatticeElements(const UAL::AcceleratorNode& sequence, 
						   int is0, 
						   int is1,
						   const UAL::AttributeSet& attSet)
{
  std::cout << "DampingTracker::setLatticeElements" << std::endl;
  if(is0 < sequence.getNodeCount()) m_frontNode = *((PacLattElement*) sequence.getNodeAt(is0));
  if(is1 < sequence.getNodeCount()) m_backNode  = *((PacLattElement*) sequence.getNodeAt(is1));

}

void UAL::USPAS::DampingTracker::propagate(UAL::Probe& probe)
{
  PAC::Bunch& bunch = static_cast<PAC::Bunch&>(probe);
  
  for(int i=0; i < bunch.size(); i++){
    if(bunch[i].isLost()) continue;
    PAC::Position& pos = bunch[i].getPosition();
    double x = pos.getX();
    double px = pos.getPX();
    pos.setX(0.99*x);
    pos.setPX(0.99*px);
  }
  // std::cout << "damping " << std::endl;
}

UAL::USPAS::DampingTrackerRegister::DampingTrackerRegister()
{
  UAL::PropagatorNodePtr nodePtr(new UAL::USPAS::DampingTracker());
  UAL::PropagatorFactory::getInstance().add("UAL::USPAS::DampingTracker", nodePtr);
}

static UAL::USPAS::DampingTrackerRegister the_UAL_USPAS_DampingTracker_Register; 
