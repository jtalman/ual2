
#include <math.h>
#include "UAL/Common/Def.hh"
#include "MltTracker.hh"


MltTracker::MltTracker()
{
  init();
}

void MltTracker::setTwiss(const PacTwissData& twiss)
{
  m_twiss = twiss;
}

void MltTracker::setSextupole(double kl2)
{
  m_kl2 = kl2;
}

void MltTracker::propagate(UAL::Probe& probe)
{
  PAC::Bunch& bunch = static_cast<PAC::Bunch& >(probe);

  PAC::BeamAttributes& ba = bunch.getBeamAttributes();

  double betax  = m_twiss.beta(0);
  double betay  = m_twiss.beta(1);

  double x0, px0, y0, py0, ct0, de0;
  double px, py;

  int size = bunch.size();
  for(int i=0; i < size; i++){

    if(bunch[i].isLost()) continue;

    PAC::Position& pos = bunch[i].getPosition();    
    x0  = pos.getX();
    px0 = pos.getPX();
    y0  = pos.getY();
    py0 = pos.getPY();
    ct0 = pos.getCT();
    de0 = pos.getDE();

    px = px0 - m_kl2/2*sqrt(betax)*(betax*x0*x0-betay*y0*y0);
    py = py0 + m_kl2/2*sqrt(betay)*sqrt(betax*betay)*x0*y0;
        
    pos.set(x0, px, y0, py, ct0, de0);
    
  }
}

void MltTracker::init()
{
  m_kl2 = 0.0;

}


