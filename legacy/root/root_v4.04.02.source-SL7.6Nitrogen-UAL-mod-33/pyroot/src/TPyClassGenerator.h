// @(#)root/pyroot:$Name:  $:$Id: TPyClassGenerator.h,v 1.2 2005/03/04 07:44:11 brun Exp $
// Author: Wim Lavrijsen   May 2004

#ifndef ROOT_TPyClassGenerator
#define ROOT_TPyClassGenerator

// ROOT
#ifndef ROOT_TClassGenerator
#include "TClassGenerator.h"
#endif


class TPyClassGenerator : public TClassGenerator {
public:
   virtual TClass* GetClass( const char* name, Bool_t load );
   virtual TClass* GetClass( const type_info& typeinfo, Bool_t load );
};

#endif // !ROOT_TPyClassGenerator
