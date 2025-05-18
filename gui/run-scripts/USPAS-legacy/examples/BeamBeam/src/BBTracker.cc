
#include <math.h>
#include "BBTracker.hh"


UAL::BBTracker::BBTracker()
{
  init();
}

void UAL::BBTracker::setCoreBunch(double n, double s)
{
  m_n1 = n;
  m_s1 = s;
}

void UAL::BBTracker::setTailBunch(double n, double s)
{
  m_n2 = n;
  m_s2 = s;
}

void UAL::BBTracker::propagate(UAL::Probe& probe)
{
  PAC::Bunch& bunch = static_cast<PAC::Bunch& >(probe);

  if(m_n1 > 0) {
    propagate(bunch, m_n1, m_s1*m_s1);
  }
  if(m_n2 > 0) {
    propagate(bunch, m_n2, m_s2*m_s2);
  }


}

void UAL::BBTracker::propagate(PAC::Bunch& bunch, double N, double sigma2)
{

  PAC::BeamAttributes& ba = bunch.getBeamAttributes();
  double e0    = ba.getEnergy(), m0 = ba.getMass();
  double gamma = e0/m0;
  double p0    = sqrt(e0*e0 - m0*m0);
  double v0byc = p0/e0;

  double x0, px0, y0, py0, ct0, de0;
  double px, py;

  double r2;

  double r_p = 1.54e-18;
  double factor = N*2*r_p/gamma;

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

    r2 = x0*x0 + y0*y0;
    px = px0 + x0*factor*(1 - exp(-r2/(2.0*sigma2)))/r2; // /(r2*(1 + energ[j]/energy));
    py = py0 + y0*factor*(1 - exp(-r2/(2.0*sigma2)))/r2; // /(r2*(1 + energ[j]/energy));
    pos.set(x0, px, y0, py, ct0, de0);    
  }
}

void UAL::BBTracker::init()
{  
  m_n1 = 0;
  m_s1 = 0;

  m_n2 = 0;
  m_s2 = 0;
}


