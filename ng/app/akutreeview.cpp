/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "akutreeview.h"
#include "akutreemodel.h"

#include <QVector>
#include <QHeaderView>

#include <KLocale>
#include <KDebug>

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
