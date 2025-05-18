/* @(#)root/netx:$Name:  $:$Id: LinkDef.h,v 1.3 2004/12/16 19:23:18 rdm Exp $ */

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class TXNetFile-;
#pragma link C++ class TXNetConn-;
#pragma link C++ class TXSocket;
#pragma link C++ class TXMessage;
#pragma link C++ class TXUrl;
#pragma link C++ class TXConnectionMgr;
#pragma link C++ class TXLogConnection;
#pragma link C++ class TXPhyConnection;
#pragma link C++ class TXInputBuffer;

#pragma link C++ class TXAbsNetCommon;

#endif
