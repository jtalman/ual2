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

  class compromise_quad : public UAL::PropagatorNode {

  public:
    const char*  getType(){
     return "JDT:::E_m::compromise_quad";
    }

#include"ETEAPOT2/Integrator/bendMethods/classGlobals"
#include"quadMethods/classMethods"

    inline ETEAPOT::MltData& getMltData();

#include"quadMethods/propagate.method"
#include"ETEAPOT2/Integrator/genMethods/get_vlcyMKS.method"
#include"ETEAPOT2/Integrator/genMethods/passDrift.method"

    /** Complexity number */
    double m_ir;

    double m_q;

//                 symmetry: eXit = eNtry!
    double m_NhD;             //   eNtry half Drift length;
    double m_Nb;              //   eNtry body length, zero, 0, here (kick)
    double m_NL;              //   Total eNtry Length: m_NL=m_NhD+m_Nb+m_NhD

    double m_NQ;              //   eNtry strength;
//                 symmetry: eXit = eNtry!

    double m_MhD;             //   Middle half Drift length;
    double m_Mb;              //   Middle length, zero, 0, here (kick)
    double m_ML;              //   Total Middle Length: m_ML=m_MhD+m_Mb+m_MhD

    double m_MQ;              //   Middle strength;

//                 symmetry: eXit = eNtry!
    double m_XhD;             //   eXit half Drift length;
    double m_Xb;              //   eXit body length, zero, 0, here (kick)
    double m_XL;              //   Total eXit Length: m_XL=m_XhD+m_Xb+m_XhD

    double m_XQ;              //   eXit strength;
//                 symmetry: eXit = eNtry!

    double m_L;               //   Total compromise quad Length: m_L=m_NL+m_ML+m_XL
                              //  
  };

}
