// @(#)root/pyroot:$Name:  $:$Id: Utility.h,v 1.12 2005/04/28 07:33:55 brun Exp $
// Author: Wim Lavrijsen, Apr 2004

#ifndef PYROOT_UTILITY_H
#define PYROOT_UTILITY_H

// ROOT
#include "DllImport.h"

// Standard
#include <map>
#include <string>


namespace PyROOT {

   extern bool gDictLookupActive;

   namespace Utility {

   // convenience functions
      bool AddToClass( PyObject* pyclass, const char* label, PyCFunction cfunc,
                       int flags = METH_VARARGS );
      bool AddToClass( PyObject* pyclass, const char* label, const char* func );

      bool InitProxy( PyObject* module, PyTypeObject* pytype, const char* name );

   // data/return types
      const int kPtrMask = 0x10000000;

      enum EDataType {
         kBool        = 0x00000001,
         kChar        = 0x00000002,
         kShort       = 0x00000004,
         kInt         = 0x00000008,
         kUInt        = 0x00000010,
         kLong        = 0x00000020,
         kULong       = 0x00000040,
         kFloat       = 0x00000080,
         kDouble      = 0x00000100,
         kVoid        = 0x00000200,
         kOther       = 0x00000400,
         kLongLong    = 0x00000800,
         kEnum        = 0x00001000,
         kSTLString   = 0x00002000,
         kString      = kPtrMask | kChar,
         kDoublePtr   = kPtrMask | kDouble,
         kFloatPtr    = kPtrMask | kFloat,
         kLongPtr     = kPtrMask | kLong,
         kIntPtr      = kPtrMask | kInt,
         kVoidPtr     = kPtrMask | kVoid
      };

   // data/operator mappings
      typedef std::map< std::string, std::string > TC2POperatorMapping_t;
      R__EXTERN TC2POperatorMapping_t gC2POperatorMapping;

   // meta information
      EDataType effectiveType( const std::string& name );
      int isPointer( const std::string& name );   // 0 = no, 1 = yes, 2 = ref

   } // namespace Utility

} // namespace PyROOT

#endif // !PYROOT_UTILITY_H
