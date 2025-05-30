/****************************************************************************
** $Id: qt/connection.h   3.3.4   edited Nov 14 2003 $
**
** Copyright (C) 1992-2002 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

// Enter your connection info here

#define DB_SALES_DRIVER	    "QSQLITE"
#define DB_SALES_DBNAME	    ":memory:"
#define DB_SALES_USER	    ""
#define DB_SALES_PASSWD	    ""
#define DB_SALES_HOST	    ""

#define DB_ORDERS_DRIVER    "QSQLITE"
#define DB_ORDERS_DBNAME    ":memory:"
#define DB_ORDERS_USER	    ""
#define DB_ORDERS_PASSWD    ""
#define DB_ORDERS_HOST	    ""

bool createConnections();
