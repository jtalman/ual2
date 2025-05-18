
#include "UAL/APF/PropagatorFactory.hh"
#include "PAC/Beam/Bunch.hh"
#include "SMF/PacLattice.h"

#include "UAL/QT/Player/TurnCounter.hh"
#include "OneTurnKicker.hh"

UAL::USPAS::OneTurnKicker::OneTurnKicker() 
{
}

UAL::USPAS::OneTurnKicker::OneTurnKicker(const UAL::USPAS::OneTurnKicker& t)
{
}

UAL::USPAS::OneTurnKicker::~OneTurnKicker()
{
}

UAL::PropagatorNode* UAL::USPAS::OneTurnKicker::clone()
{
  return new UAL::USPAS::OneTurnKicker(*this);
}

UAL::AcceleratorNode& UAL::USPAS::OneTurnKicker::getFrontAcceleratorNode()
{
  return m_frontNode;
}

UAL::AcceleratorNode& UAL::USPAS::OneTurnKicker::getBackAcceleratorNode()
{
  return m_backNode;
}

void UAL::USPAS::OneTurnKicker::setLatticeElements(const UAL::AcceleratorNode& sequence, 
						   int is0, 
						   int is1,
						   const UAL::AttributeSet& attSet)
{
  std::cout << "OneTurnKicker::setLatticeElements" << std::endl;
  if(is0 < sequence.getNodeCount()) m_frontNode = 
				      *((PacLattElement*) sequence.getNodeAt(is0));
  if(is1 < sequence.getNodeCount()) m_backNode  = 
				      *((PacLattElement*) sequence.getNodeAt(is1));
}

void UAL::USPAS::OneTurnKicker::propagate(UAL::Probe& probe)
{
  PAC::Bunch& bunch = static_cast<PAC::Bunch&>(probe);
  
  int turn = UAL::QT::TurnCounter::getInstance()->getTurn();
  if(turn != 0) return;


  double xmax, ymax;
  findMaxSize(bunch, xmax, ymax);

  double xkick = 0.5*xmax;
  double ykick = 0.5*ymax;

  // std::cout << "OneTurnKicker:" 
  //	    << " xkick=" << xkick 
  //	    << " ykick=" <<  ykick << std::endl;

  for(int i=0; i < bunch.size(); i++){
    if(bunch[i].isLost()) continue;
    PAC::Position& pos = bunch[i].getPosition();
    double x = pos.getX();
    pos.setX(x+xkick);
    double y = pos.getY();
    pos.setY(y+ykick);
  }
}

void UAL::USPAS::OneTurnKicker::findMaxSize(PAC::Bunch& bunch, 
					    double& xmax, 
					    double& ymax)
{
  xmax = 0.0; 
  ymax = 0.0;
  for(int i=0; i < bunch.size(); i++){
    if(bunch[i].isLost()) continue;
    PAC::Position& pos = bunch[i].getPosition();
    double absx = fabs(pos.getX());
    if(absx > xmax) xmax = absx;
    double absy = fabs(pos.getY());
    if(absy > ymax) ymax = absy;
  }
}

UAL::USPAS::OneTurnKickerRegister::OneTurnKickerRegister()
{
  UAL::PropagatorNodePtr nodePtr(new UAL::USPAS::OneTurnKicker());
  UAL::PropagatorFactory::getInstance().add("UAL::USPAS::OneTurnKicker", nodePtr);
}

static UAL::USPAS::OneTurnKickerRegister the_UAL_USPAS_OneTurnKicker_Register; 
