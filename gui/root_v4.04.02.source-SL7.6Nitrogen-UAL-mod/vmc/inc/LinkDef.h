// @(#)root/vmc:$Name:  $:$Id: LinkDef.h,v 1.2 2003/09/23 14:01:36 brun Exp $
#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
 
#pragma link C++ global gMC;
 
#pragma link C++ enum PDG_t;
#pragma link C++ enum TMCProcess;
#pragma link C++ enum TMCParticleType;

#pragma link C++ class  TVirtualMC+;
#pragma link C++ class  TVirtualMCGeometry+;
#pragma link C++ class  TVirtualMCApplication+;
#pragma link C++ class  TVirtualMCStack+;
#pragma link C++ class  TVirtualMCDecayer+;
#pragma link C++ class  TMCVerbose+;
#pragma link C++ class  TGeoMCGeometry+;

#endif





