
#include "UAL/APF/PropagatorFactory.hh"
#include "PAC/Beam/Bunch.hh"
#include "SMF/PacLattice.h"

#include "DoNothingTracker.hh"

UAL::USPAS::DoNothingTracker::DoNothingTracker() 
{
}

UAL::USPAS::DoNothingTracker::DoNothingTracker(const UAL::USPAS::DoNothingTracker& t)
{
}

UAL::USPAS::DoNothingTracker::~DoNothingTracker()
{
}

UAL::PropagatorNode* UAL::USPAS::DoNothingTracker::clone()
{
  return new UAL::USPAS::DoNothingTracker(*this);
}

UAL::AcceleratorNode& UAL::USPAS::DoNothingTracker::getFrontAcceleratorNode()
{
  return m_frontNode;
}

UAL::AcceleratorNode& UAL::USPAS::DoNothingTracker::getBackAcceleratorNode()
{
  return m_backNode;
}

void UAL::USPAS::DoNothingTracker::setLatticeElements(const UAL::AcceleratorNode& sequence, 
						   int is0, 
						   int is1,
						   const UAL::AttributeSet& attSet)
{
  std::cout << "DoNothingTracker::setLatticeElements" << std::endl;
  if(is0 < sequence.getNodeCount()) m_frontNode = *((PacLattElement*) sequence.getNodeAt(is0));
  if(is1 < sequence.getNodeCount()) m_backNode  = *((PacLattElement*) sequence.getNodeAt(is1));

}

void UAL::USPAS::DoNothingTracker::propagate(UAL::Probe& probe)
{
  PAC::Bunch& bunch = static_cast<PAC::Bunch&>(probe);
  
  for(int i=0; i < bunch.size(); i++){
    if(bunch[i].isLost()) continue;
    PAC::Position& pos = bunch[i].getPosition();
    double de = pos.getDE();
    pos.setDE(0.99*de);
  }
  // std::cout << "damping " << std::endl;
}

UAL::USPAS::DoNothingTrackerRegister::DoNothingTrackerRegister()
{
  UAL::PropagatorNodePtr nodePtr(new UAL::USPAS::DoNothingTracker());
  UAL::PropagatorFactory::getInstance().add("UAL::USPAS::DoNothingTracker", nodePtr);
}

static UAL::USPAS::DoNothingTrackerRegister the_UAL_USPAS_DoNothingTracker_Register; 
