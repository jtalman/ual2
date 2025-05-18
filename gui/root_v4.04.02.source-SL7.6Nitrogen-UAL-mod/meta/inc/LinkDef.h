/* @(#)root/meta:$Name:  $:$Id: LinkDef.h,v 1.15 2005/03/20 19:35:50 brun Exp $ */

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifdef __CINT__

#pragma link C++ nestedtypedefs;
#pragma link C++ nestedclasses;

#pragma link C++ namespace ROOT;

#pragma link C++ enum EProperty;

#pragma link C++ class TBaseClass;
#pragma link C++ class TClass;
#pragma link C++ class TClassStreamer+;
#pragma link C++ class TMemberStreamer+;
#pragma link C++ class TClassRef+;
#pragma link C++ class TClassGenerator+;
#pragma link C++ class TDataMember;
#pragma link C++ class TDataType;
#pragma link C++ class TDictionary;
#pragma link C++ class TFunction;
#pragma link C++ class TGlobal;
#pragma link C++ class TMethod;
#pragma link C++ class TMethodArg;
#pragma link C++ class TMethodCall;
#pragma link C++ class TCint;
#pragma link C++ class TInterpreter;
#pragma link C++ class TClassMenuItem;

#pragma link C++ class TStreamerBase-;
#pragma link C++ class TStreamerBasicPointer-;
#pragma link C++ class TStreamerLoop-;
#pragma link C++ class TStreamerBasicType-;
#pragma link C++ class TStreamerObject-;
#pragma link C++ class TStreamerObjectAny-;
#pragma link C++ class TStreamerObjectPointer-;
#pragma link C++ class TStreamerObjectAnyPointer-;
#pragma link C++ class TStreamerString-;
#pragma link C++ class TStreamerSTL-;
#pragma link C++ class TStreamerSTLstring-;
#pragma link C++ class TStreamerElement-;
#pragma link C++ class TStreamerInfo-;
#pragma link C++ class TToggle;
#pragma link C++ class TToggleGroup;

#endif
