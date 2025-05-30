/****************************************************************************
** $Id: qt/qnetwork.cpp   3.3.4   edited May 27 2003 $
**
** Implementation of qInitNetworkProtocols function.
**
** Created : 970521
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of the network module of the Qt GUI Toolkit.
**
** This file may be distributed under the terms of the Q Public License
** as defined by Trolltech AS of Norway and appearing in the file
** LICENSE.QPL included in the packaging of this file.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid Qt Enterprise Edition licenses may use this
** file in accordance with the Qt Commercial License Agreement provided
** with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
**   information about Qt Commercial License Agreements.
** See http://www.trolltech.com/qpl/ for QPL licensing information.
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include "qnetwork.h"

#ifndef QT_NO_NETWORK

#include "qnetworkprotocol.h"

// protocols
#include "qftp.h"
#include "qhttp.h"

/*! \file qnetwork.h */
/*!
    \relates QUrlOperator

    This function registers the network protocols for FTP and HTTP.
    You must call this function before you use QUrlOperator for
    these protocols.

    This function is declared in \l qnetwork.h.
*/
void qInitNetworkProtocols()
{
#ifndef QT_NO_NETWORKPROTOCOL_FTP
    QNetworkProtocol::registerNetworkProtocol( "ftp", new QNetworkProtocolFactory< QFtp > );
#endif
#ifndef QT_NO_NETWORKPROTOCOL_HTTP
    QNetworkProtocol::registerNetworkProtocol( "http", new QNetworkProtocolFactory< QHttp > );
#endif
}

#endif // QT_NO_NETWORK
