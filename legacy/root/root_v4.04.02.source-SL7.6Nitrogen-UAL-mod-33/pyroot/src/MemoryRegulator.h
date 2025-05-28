// @(#)root/pyroot:$Name:  $:$Id: MemoryRegulator.h,v 1.6 2005/03/04 07:44:11 brun Exp $
// Author: Wim Lavrijsen, Apr 2004

#ifndef PYROOT_MEMORYREGULATOR_H
#define PYROOT_MEMORYREGULATOR_H

// ROOT
#include "TObject.h"

// Standard
#include <map>


namespace PyROOT {

/** Communicate object destruction across ROOT/CINT/PyROOT/
      @author  WLAV
      @date    11/23/2004
      @version 2.0
 */

   class ObjectProxy;

   class MemoryRegulator : public TObject {
   public:
      MemoryRegulator();

   // callback for ROOT/CINT
      virtual void RecursiveRemove( TObject* object );

   // add a python object to the table of managed objects
      static void RegisterObject( ObjectProxy* pyobj, TObject* object );

   // new reference to python object corresponding to object, or 0 on failure
      static PyObject* RetrieveObject( TObject* object );

   // callback when weak refs to managed objects are destroyed
      static PyObject* ObjectEraseCallback( PyObject*, PyObject* pyref );

   private:
      typedef std::map< TObject*, PyObject* > ObjectMap_t;
      static ObjectMap_t fgObjectTable;
   };

} // namespace PyROOT

#endif // !PYROOT_MEMORYREGULATOR_H
