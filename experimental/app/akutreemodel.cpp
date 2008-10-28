 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 

#include "akutreemodel.h"
#include <QVector>
#include <QStringList>

class AkuTreeModel::Private
{
public:
    Private(AkuTreeModel *q):
                        q(q)
                        {}

    AkuTreeModel *q;
    QVector<QStringList> sourceData;
};

AkuTreeModel::AkuTreeModel(QVector<QStringList> fileList, QObject *parent) : 
                                               QAbstractItemModel(parent),
                                               d(new Private(this))
{
    d->sourceData = fileList;
}

AkuTreeModel::~AkuTreeModel()
{}

int AkuTreeModel::rowCount(const QModelIndex & parent) const
{
    if (d->sourceData.isEmpty()) {
        return 0;
    }

    return d->sourceData.size();
}

int AkuTreeModel::columnCount(const QModelIndex & parent) const
{
    if (d->sourceData.isEmpty()) {
        return 0;
    }

    return static_cast<QStringList>(d->sourceData.first()).count();
}
