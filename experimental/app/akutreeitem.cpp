 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#include "akutreeitem.h"

AkuTreeItem::AkuTreeItem(const QStringList &data, AkuTreeItem *parent) :
                                                    itemData(data),
                                                    parentItem(parent)
{
}

AkuTreeItem::~AkuTreeItem()
{}

void AkuTreeItem::appendChild(AkuTreeItem *child)
{
    childItems << child;
}

int AkuTreeItem::childCount() const
{
    childItems.size();
}

int AkuTreeItem::columnCount() const
{
    itemData.size();
}

AkuTreeItem* parent()
{
    return parentItem;
}

QString data(int column) const
{
    if (itemData.isEmpty()) {
        return QString();
    }

    if (itemData.size() < column) {
        return QString();
    }

    return itemData[count];
}
