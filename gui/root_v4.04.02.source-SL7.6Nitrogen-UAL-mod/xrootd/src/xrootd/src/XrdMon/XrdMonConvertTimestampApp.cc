/*****************************************************************************/
/*                                                                           */
/*                        XrdMonConvertTimestampApp.cc                       */
/*                                                                           */
/* (c) 2004 by the Board of Trustees of the Leland Stanford, Jr., University */
/*                            All Rights Reserved                            */
/*       Produced by Jacek Becla for Stanford University under contract      */
/*              DE-AC02-76SF00515 with the Department of Energy              */
/*****************************************************************************/

// $Id: XrdMonConvertTimestampApp.cc,v 1.2 2005/03/07 22:42:38 becla Exp $

#include "XrdMon/XrdMonUtils.hh"
#include <stdio.h>
#include <iostream>
#include <string>
using std::cerr;
using std::cout;
using std::endl;
using std::string;

int main(int argc, char* argv[])
{
    if ( argc < 2 ) {
        cerr << "Expected arg: timestamp" << endl;
        return 1;
    }
    for ( int i=1 ; i<argc ; ++i ) {
        kXR_int32 t;
        sscanf(argv[i], "%ld", &t);
        cout << t << "  --> " << timestamp2string(t) << endl;
    }
    
    return 0;
}


