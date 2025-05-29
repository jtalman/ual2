#include <math.h>

#include "UAL/APF/PropagatorFactory.hh"
#include "Tracker.hh"


NM::Tracker::Tracker()
{
  m_l = 0.0;
}

NM::Tracker::Tracker(const NM::Tracker& st)
{
  m_l = st.m_l;
}

NM::Tracker::~Tracker()
{
}



void NM::Tracker::setLatticeElements(const UAL::AcceleratorNode& sequence, 
						int is0, int is1,
						const UAL::AttributeSet& attSet)
{
  m_l = 2.0; 
}


void NM::Tracker::propagate(UAL::Probe& probe)
{
  PAC::Bunch& bunch = static_cast<PAC::Bunch& >(probe);

  PAC::BeamAttributes& ba = bunch.getBeamAttributes();

  double e0    = ba.getEnergy(), m0 = ba.getMass();
  double p0    = sqrt(e0*e0 - m0*m0);
  double v0byc = p0/e0;
  double oldT  = ba.getElapsedTime();

  PAC::Position tmp;

  int size = bunch.size();
  for(int i=0; i < size; i++){
    if(bunch[i].isLost()) continue;
    PAC::Position& p = bunch[i].getPosition();
    tmp = p;
    makeVelocity(p, tmp, v0byc);
    makeRV(p, tmp, e0, p0, m0);
    passDrift(m_l, p, tmp, v0byc);       
  }

  ba.setElapsedTime(oldT + m_l/v0byc/UAL::clight);  

}

void NM::Tracker::makeVelocity(PAC::Position& p, PAC::Position& tmp, double v0byc)
{
  double t0, t1;

  t0  = 1.;
 
  if(p.size() > 5){ 
    t1  = p[5];
    t1 += 2./v0byc;
    t1 *= p[5];
    t0 += t1;
  }

  t1  = p[1];
  t1 *= t1;
  t0 -= t1;

  t1  = p[3];
  t1 *= t1;
  t0 -= t1;

  t0  = sqrt(t0);
  tmp[0]   = t0;

  t0  = 1./t0;
  tmp[1]  = p[1]*t0;    // vx/vs
  tmp[3]  = p[3]*t0;    // vy/vs

  return;
}

void NM::Tracker::makeRV(PAC::Position& p, PAC::Position& tmp,
  double e0, 
  double p0,
  double m0)
{
 
  double e, p2, rv;

  e  = e0;
  e += (p0*p[5]);    // was : p[PacPosition::DE]*p0; 

  p2  = e;
  p2 *= e;
  p2 -= m0*m0;
  p2  = sqrt(p2);
  
  rv  = e;
  rv /= p2;

  tmp[5] = rv;

}


void NM::Tracker::passDrift(
  double rlipl, 
  PAC::Position& p, 
  PAC::Position& tmp, 
  double v0byc)
{
  double rvbyc, p1, p2, p4;

  // Transform coordinates into frame of multipole

  p[0] += (rlipl*tmp[1]); // was :tmp[PacPosition::PX]*rlipl;                 
  p[2] += (rlipl*tmp[3]); // was :tmp[PacPosition::PY]*rlipl; 


  if(p.size() > 5){

    // -p[5] = cdt = L/(v/c) -Lo/(vo/c) = (L - Lo)/(v/c) + Lo*(c/v - c/vo) =
    //                                  = cdt_circ       + cdt_vel

    // 1. cdt_circ = (c/v)(L - Lo) = (c/v)(L**2 - Lo**2)/(L + Lo) ~ 

/*
    rvbyc  = v0byc*(1.+ tmp[PacPosition::CT]);  
    rvbyc  = 1./rvbyc;                                // c/v
*/

    rvbyc  = tmp[5];                     // c/v

    p1  = tmp[1];
    p1 *= p1;
    p2  = tmp[3];
    p2 *= p2;
    p1 += p2;

    // Define L + Lo instead of 2*Lo (April 98)
    p4  = 1.;
    p4 += p1;
    p4  = sqrt(p4);                                    // L/Lo
    p4 += 1.;                                          // (L+Lo)/Lo
    p4 /= 2.;

    p1 /= p4;

    p1 *= rvbyc;
    p1 *= rlipl/2.;

    // 2. cdt_vel  = Lo*(c/v -c/vo) 
 
    p2  =  1./v0byc;
    p2 -=  rvbyc;
    p2 *=  rlipl;

    // Total

    p[4] -= p1; 
    p[4] += p2;

  }
               
  return;
}
