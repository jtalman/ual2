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

//#include "ETEAPOT2/Integrator/BasicTracker.hh"
#include"UAL/APF/PropagatorComponent.hh"

#include"UAL/Common/Def.hh"
#include"SMF/PacLattElement.h"
#include"PAC/Beam/Position.hh"
#include"SMF/PacElemAperture.h"
#include"SMF/PacElemOffset.h"

#include"ETEAPOT2/Integrator/genMethods/Matrices.hh"
#include"ETEAPOT2/Integrator/genMethods/Vectors.h"
#include"ETEAPOT2/Integrator/genMethods/spinExtern"
#include"ETEAPOT2/Integrator/genMethods/designExtern"
#include"ETEAPOT2/Integrator/genMethods/bunchParticleExtern"

namespace E_m{

  class quad : public UAL::PropagatorNode {
//class quad : public ETEAPOT::BasicTracker {

  public:
const char*  getType(){
  return "JDT:::E_m::compromise-quad";
}

#include"ETEAPOT2/Integrator/bendMethods/classGlobals"
#include"quadMethods/classMethods"

   inline ETEAPOT::MltData& getMltData();

#include "quadMethods/propagate.method"
#include "ETEAPOT2/Integrator/genMethods/get_vlcyMKS.method"
#include "ETEAPOT2/Integrator/genMethods/passDrift.method"

    /** Complexity number */
    double m_ir;

    /** Mlt attributes */
    ETEAPOT::MltData m_mdata;

    double m_q;

  };

  inline ETEAPOT::MltData& quad::getMltData()
  {
      return m_mdata;
  }

}
