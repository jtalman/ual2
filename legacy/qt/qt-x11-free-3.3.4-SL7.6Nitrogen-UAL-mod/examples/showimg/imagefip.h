/****************************************************************************
** $Id: qt/imagefip.h   3.3.4   edited May 27 2003 $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef IMAGEFIP_H
#define IMAGEFIP_H

#include <qfiledialog.h>
#include <qstrlist.h>
#include <qpixmap.h>

class ImageIconProvider : public QFileIconProvider
{
    Q_OBJECT
    QStrList fmts;
    QPixmap imagepm;

public:
    ImageIconProvider( QWidget *parent=0, const char *name=0 );
    ~ImageIconProvider();

    const QPixmap * pixmap( const QFileInfo &fi );
};


#endif // IMAGEFIP_H
