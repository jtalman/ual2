// @(#)root/vmc:$Name:  $:$Id: TGeoMCGeometry.h,v 1.6 2004/10/12 07:45:53 brun Exp $
// Authors: ... 25/06/2002

#ifndef ROOT_TGeoMCGeometry
#define ROOT_TGeoMCGeometry 

/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id: TGeoMCGeometry.h,v 1.6 2004/10/12 07:45:53 brun Exp $ */

//
// Class TGeoMCGeometry
// --------------------
// Implementation of the TVirtualMCGeometry interface
// for building TGeo geometry.
//  

#include "TVirtualMCGeometry.h" 

class TGeoMCGeometry : public TVirtualMCGeometry {

  public:
    TGeoMCGeometry(const char* name, const char* title, 
                   Bool_t g3CompatibleVolumeNames = false);
    TGeoMCGeometry();
    virtual ~TGeoMCGeometry();
  
    // detector composition
    virtual void  Material(Int_t& kmat, const char* name, Double_t a, 
                     Double_t z, Double_t dens, Double_t radl, Double_t absl,
                     Float_t* buf, Int_t nwbuf);
    virtual void  Material(Int_t& kmat, const char* name, Double_t a, 
                     Double_t z, Double_t dens, Double_t radl, Double_t absl,
                     Double_t* buf, Int_t nwbuf);
    virtual void  Mixture(Int_t& kmat, const char *name, Float_t *a, 
                     Float_t *z, Double_t dens, Int_t nlmat, Float_t *wmat);
    virtual void  Mixture(Int_t& kmat, const char *name, Double_t *a, 
                     Double_t *z, Double_t dens, Int_t nlmat, Double_t *wmat);
    virtual void  Medium(Int_t& kmed, const char *name, Int_t nmat, 
                     Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd, 
                     Double_t stemax, Double_t deemax, Double_t epsil, 
		     Double_t stmin, Float_t* ubuf, Int_t nbuf);
    virtual void  Medium(Int_t& kmed, const char *name, Int_t nmat, 
                     Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd, 
                     Double_t stemax, Double_t deemax, Double_t epsil, 
		     Double_t stmin, Double_t* ubuf, Int_t nbuf);
    virtual void  Matrix(Int_t& krot, Double_t thetaX, Double_t phiX, 
                     Double_t thetaY, Double_t phiY, Double_t thetaZ, 
		     Double_t phiZ);

    // functions from GGEOM 
    virtual Int_t  Gsvolu(const char *name, const char *shape, Int_t nmed,  
                          Float_t *upar, Int_t np); 
    virtual Int_t  Gsvolu(const char *name, const char *shape, Int_t nmed,  
                          Double_t *upar, Int_t np); 
    virtual void  Gsdvn(const char *name, const char *mother, Int_t ndiv, 
                         Int_t iaxis); 
    virtual void  Gsdvn2(const char *name, const char *mother, Int_t ndiv, 
                         Int_t iaxis, Double_t c0i, Int_t numed); 
    virtual void  Gsdvt(const char *name, const char *mother, Double_t step, 
                         Int_t iaxis, Int_t numed, Int_t ndvmx); 
    virtual void  Gsdvt2(const char *name, const char *mother, Double_t step, 
                         Int_t iaxis, Double_t c0, Int_t numed, Int_t ndvmx); 
    virtual void  Gsord(const char *name, Int_t iax); 
    virtual void  Gspos(const char *name, Int_t nr, const char *mother,  
                         Double_t x, Double_t y, Double_t z, Int_t irot, 
                         const char *konly); 
    virtual void  Gsposp(const char *name, Int_t nr, const char *mother,  
                         Double_t x, Double_t y, Double_t z, Int_t irot,
                         const char *konly, Float_t *upar, Int_t np);
    virtual void  Gsposp(const char *name, Int_t nr, const char *mother,  
                         Double_t x, Double_t y, Double_t z, Int_t irot,
                         const char *konly, Double_t *upar, Int_t np);
    virtual void  Gsbool(const char* /*onlyVolName*/, const char* /*manyVolName*/) {}
  
    
    // functions for drawing
    //virtual void  DrawOneSpec(const char* name);
    //virtual void  Gsatt(const char* name, const char* att, Int_t val);
    //virtual void  Gdraw(const char*,Double_t theta, Double_t phi,
    //		        Double_t psi, Double_t u0, Double_t v0,
    //		        Double_t ul, Double_t vl);

    // Euclid
    //virtual void  WriteEuclid(const char*, const char*, Int_t, Int_t);
		               
    // get methods
    virtual Int_t VolId(const Text_t* volName) const;
    virtual const char* VolName(Int_t id) const;
    virtual Int_t NofVolumes() const;
    virtual Int_t NofVolDaughters(const char* volName) const;
    virtual const char*  VolDaughterName(const char* volName, Int_t i) const;
    virtual Int_t        VolDaughterCopyNo(const char* volName, Int_t i) const;
    virtual Int_t VolId2Mate(Int_t id) const;

  private:
    Double_t* CreateDoubleArray(Float_t* array, Int_t size) const;
    void     Vname(const char *name, char *vname) const;
   
    TGeoMCGeometry(const TGeoMCGeometry& rhs);
    TGeoMCGeometry& operator=(const TGeoMCGeometry& /*rhs*/) {return (*this);}
    Bool_t  fG3CompatibleVolumeNames;   // option to convert volumes names to
                                        // be compatible with G3

    static TGeoMCGeometry*  fgInstance; // singleton instance

  ClassDef(TGeoMCGeometry,2)  // VMC TGeo Geometry builder
};

#endif //ROOT_TGeoMCGeometry
