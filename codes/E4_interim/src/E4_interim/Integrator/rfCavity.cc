#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<cstdlib>

#include<typeinfo> 

#include"SMF/PacLattice.h"
#include"SMF/PacElemRfCavity.h"

#include"ETEAPOT/Integrator/DipoleData.hh"
#include"ETEAPOT/Integrator/MltData.hh"

//#include "E4_interim/Integrator/BasicTracker.hh"
#include"UAL/APF/PropagatorComponent.hh"

#include"UAL/Common/Def.hh"
#include"SMF/PacLattElement.h"
#include"PAC/Beam/Position.hh"
#include"SMF/PacElemAperture.h"
#include"SMF/PacElemOffset.h"

#include"E4_interim/Integrator/genMethods/Matrices.hh"
#include"E4_interim/Integrator/genMethods/Vectors.h"
//#include"E4_interim/Integrator/genMethods/spinExtern"
#include"E4_interim/Integrator/genMethods/designExtern"
#include"E4_interim/Integrator/genMethods/bunchParticleExtern"

namespace E4_interim {

  /** RF Cavity Tracker */

  class rfCavity : public UAL::PropagatorNode {
//class rfCavity : public ETEAPOT::BasicTracker {

  public:

#include"E4_interim/Integrator/bendMethods/classGlobals"
#include"E4_interim/Integrator/rfCavityMethods/classMethods"

#include"E4_interim/Integrator/rfCavityMethods/propagate.method"
#include"E4_interim/Integrator/genMethods/get_vlcyMKS.method"
#include"E4_interim/Integrator/genMethods/passDrift.method"

   double m_V;
   double m_lag;
   double m_h;

  };

}
