 /* This file is part of the KDE project

   Copyright (C) 2008 Francesco Grieco <fgrieco@gmail.com>
   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#include "akuiconview.h"
#include "akutreemodel.h"

#include <QVector>
#include <QFontMetrics>
#include <QApplication>

#include <KGlobalSettings>

AkuIconView::AkuIconView(QWidget *parent) : QListView(parent)
{
    setViewMode(QListView::IconMode);
    setWrapping(false);
    setSpacing(2);
    setIconSize(64);
    setWrapping(true);
    setLayoutMode(QListView::Batched);
    setSelectionRectVisible(true);
    setResizeMode(QListView::Adjust);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    if (KGlobalSettings::singleClick()) {
        connect(this, SIGNAL(clicked(const QModelIndex &)), this, SLOT(enterItem(const QModelIndex &)));
    } else {
        connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(enterItem(const QModelIndex &)));
    }
}

AkuIconView::~AkuIconView()
{}

AkuTreeModel* AkuIconView::model()
{
    return static_cast<AkuTreeModel*>(QListView::model());
}

void AkuIconView::setIconSize(int size)
{
    QListView::setIconSize(QSize(size, size));
    m_iconSize = size;
    QFontMetrics fm(QApplication::font());

    setGridSize(QSize(size + fm.height()*2, size + fm.height()*2));
    m_gridSize = fm.height()*2 + size;
}

int AkuIconView::viewItemSize()
{
    return m_gridSize;
}

void AkuIconView::enterItem(const QModelIndex &index)
{
    if (!index.model()->hasChildren(index)) {
        return;
    }

    setRootIndex(index);
}

void AkuIconView::browseUp()
{
    setRootIndex(rootIndex().parent());
}
