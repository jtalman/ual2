/****************************************************************************
** $Id: qt/ftpviewitem.h   3.3.4   edited May 27 2003 $
**
** Copyright (C) 1992-2002 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef FTPVIEWITEM_H
#define FTPVIEWITEM_H

#include <qlistview.h>
#include <qdatetime.h>

class FtpViewItem : public QListViewItem
{
public:
    enum Type {
	Directory,
	File
    };

    FtpViewItem( QListView *parent, Type t, const QString &name, const QString &size, const QString &lastModified );

    int compare( QListViewItem * i, int col, bool ascending ) const;

    bool isDir()
    { return type==Directory; }

private:
    Type type;
};

#endif
