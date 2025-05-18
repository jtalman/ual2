
#include <math.h>
#include "UAL/Common/Def.hh"
#include "CMTracker.hh"


CMTracker::CMTracker()
{
  init();
}

void CMTracker::setTunes(double qx, double qy)
{
  PacTwissData twiss = m_twiss;

  twiss.mu(0, 2.0*UAL::pi*qx);
  twiss.mu(1, 2.0*UAL::pi*qy);

  setTwiss(twiss);
}

void CMTracker::setChromaticities(double chx, double chy)
{
  m_chx = 2.0*UAL::pi*chx;
  m_chy = 2.0*UAL::pi*chy;
}

void CMTracker::setTwiss(const PacTwissData& twiss)
{
  m_twiss = twiss;
}

void CMTracker::setRF(double V, double lag, double h)
{
  m_V   = V;
  m_lag = lag;
  m_h   = h;
}

void CMTracker::setLattice(double suml, double alpha)
{
  m_suml  = suml;
  m_alpha = alpha;
}


void CMTracker::propagate(UAL::Probe& probe)
{
  PAC::Bunch& bunch = static_cast<PAC::Bunch& >(probe);

  PAC::BeamAttributes& ba = bunch.getBeamAttributes();
  double e0    = ba.getEnergy(), m0 = ba.getMass();
  double p0    = sqrt(e0*e0 - m0*m0);
  double v0byc = p0/e0;
  double gamma = e0/m0;

  double eta0    = m_alpha - 1./gamma/gamma;
  double ct2phi  = -m_h*(v0byc/m_suml);
  double a56     = -eta0*m_suml/v0byc;

  double x0, px0, y0, py0, ct0, de0;
  double x, px, y, py, ct, de;
  double phase;

  double betax  = m_twiss.beta(0);
  double betay  = m_twiss.beta(1);
  double alphax = m_twiss.alpha(0);
  double alphay = m_twiss.alpha(1);
  double gammax = (1. + alphax*alphax)/betax;
  double gammay = (1. + alphay*alphay)/betay;

  double mux    = m_twiss.mu(0);
  double muy    = m_twiss.mu(1);

  double cosx, sinx, cosy, siny;

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

    cosx = cos(mux + m_chx*de0);
    sinx = sin(mux + m_chx*de0);
    cosy = cos(muy + m_chy*de0);
    siny = sin(muy + m_chy*de0);
        
    x   = cosx*x0         + betax*sinx*px0;
    px  = -gammax*sinx*x0 + cosx*px0;
    y   = cosy*y0         + betay*siny*py0;
    py  = -gammay*siny*y0 + cosy*py0;

    ct    = ct0 + a56*de0;  
    phase = ct2phi*ct;
    de    = m_V*sin(2.*UAL::pi*(m_lag + phase)); // like MAD

    pos.set(x, px, y, py, ct, (de0*p0 + de)/p0);
    
  }
}

void CMTracker::init()
{
  m_chx = 0.0;
  m_chy = 0.0;

  m_V   = 0.0;
  m_lag = 0.0;
  m_h   = 1.;

  m_suml  = 0.0;
  m_alpha = 0;
}


