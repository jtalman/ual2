// @(#)root/vmc:$Name:  $:$Id: TVirtualMCApplication.h,v 1.2 2003/09/23 14:02:38 brun Exp $
// Author: Ivana Hrivnacova, 23/03/2002

#ifndef ROOT_TVirtualMCApplication
#define ROOT_TVirtualMCApplication
//
// Class TVirtualMCApplication
// ---------------------------
// Interface to a user Monte Carlo application.
//

#include "TNamed.h"

class TVirtualMCApplication : public TNamed {

  public:
    TVirtualMCApplication(const char *name, const char *title);
    TVirtualMCApplication();
    virtual ~TVirtualMCApplication();
  
    // static access method
    static TVirtualMCApplication* Instance(); 

    // methods
    virtual void ConstructGeometry() = 0;
    virtual void InitGeometry() = 0;
    virtual void AddParticles() {}
    virtual void GeneratePrimaries() = 0;
    virtual void BeginEvent() = 0;
    virtual void BeginPrimary() = 0;
    virtual void PreTrack() = 0;
    virtual void Stepping() = 0;
    virtual void PostTrack() = 0;
    virtual void FinishPrimary() = 0;
    virtual void FinishEvent() = 0;
    
    virtual Double_t TrackingRmax() const { return DBL_MAX; }
    virtual Double_t TrackingZmax() const { return DBL_MAX; } 
    virtual void     Field(const Double_t* x, Double_t* b) const = 0;

  private:
    // static data members
    static TVirtualMCApplication* fgInstance; //singleton instance  

  ClassDef(TVirtualMCApplication,1)  //Interface to MonteCarlo application
};

// inline methods
inline TVirtualMCApplication* TVirtualMCApplication::Instance() 
{ return fgInstance; }

#endif //ROOT_TVirtualMCApplication

