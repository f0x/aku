 /* This file is part of the KDE project

   Copyright (C) 2008 Francesco Grieco <fgrieco@gmail.com>
   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#include "akutreeview.h"
#include "akutreemodel.h"

#include <KLocale>
#include <KDebug>

#include <QVector>
#include <QHeaderView>

AkuTreeView::AkuTreeView(QWidget *parent) : QTreeView(parent)
{
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    header()->setResizeMode(QHeaderView::ResizeToContents);
}

AkuTreeView::~AkuTreeView()
{
}

QStringList AkuTreeView::selectedPaths()
{
    QStringList paths;
    foreach (const QModelIndex &index, selectedIndexes()) {

        if (index.column() > 0) {
            continue;
        }

        QString path;
        path = index.data().toString();

        QModelIndex parent = index.parent();
        while (parent.isValid()) {
            path.prepend(parent.data().toString() + '/');
            parent = parent.parent();
        }
        paths << path;
    }

    return paths;
}
