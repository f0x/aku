 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#ifndef AKUTREEITEM_H
#define AKUTREEITEM_h

#include <QStringList>

class AkuTreeItem
{
    public:
        AkuTreeItem(const QStringList &data, AkuTreeItem *parent = 0);
        ~AkuTreeItem();

        void appendChild(AkuTreeItem *);

        AkuTreeItem* child();
        int childCount() const;
        int columnCount() const;
        QString data(int column) const;
        int row() const;
        AkuTreeItem* parent();

    private:
        QList<AkuTreeItem*> childItems;
        QStringList itemData;
        AkuTreeItem *parentItem;
        
};

#endif
