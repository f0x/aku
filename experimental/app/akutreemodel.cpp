 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 

#include "akutreemodel.h"
#include "akutreenode.h"

#include <QVector>
#include <QStringList>

#include <KLocale>
#include <KDebug>

class AkuTreeModel::Private
{
public:
    Private(AkuTreeModel *q):
                        q(q)
                        {}

    AkuTreeModel *q;
    QVector<QStringList> sourceData;
    QList<AkuTreeNode*> nodes;
    AkuTreeNode *rootNode;

    void initData();
    void generateNodes(const QVector<QStringList> &source);
};

AkuTreeModel::AkuTreeModel(QVector<QStringList> fileList, QObject *parent) : 
                                               QAbstractItemModel(parent),
                                               d(new Private(this))
{
    d->sourceData = fileList;
    d->initData();
    d->generateNodes(d->sourceData);
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
    Q_UNUSED(parent)

    if (d->sourceData.isEmpty()) {
        return 0;
    }

    return static_cast<QStringList>(d->sourceData.first()).count();
}

QVariant AkuTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
     return d->rootNode->data(section);
    }

    return QVariant();
}

QVariant AkuTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        return d->nodes[index.row()]->data(index.column());
    }

    // TODO: fill in with correct code
    return QVariant();
}

QModelIndex AkuTreeModel::parent(const QModelIndex &index) const
{
    // TODO: fill in with correct code
    if (!index.isValid()) {
        return QModelIndex();
    }

    AkuTreeNode *node = static_cast<AkuTreeNode*>(index.internalPointer());
    if (node->parent() != d->rootNode) {
        kDebug()<<"parent root != rootNode";
        return createIndex(node->parent()->row(), 0, node->parent());
    }

    kDebug()<<"not parented";
    return QModelIndex();
}

QModelIndex AkuTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    // TODO: fill in with correct code
    if (row > rowCount() || column > columnCount()) {
        return QModelIndex();
    }

    return createIndex(row, column, d->nodes[row]);
//     return QModelIndex();
}

void AkuTreeModel::Private::initData()
{
    QStringList header;
    header << i18n("File Name") << i18n("Size") << i18n("Packed Size") << i18n("Ratio") << i18n("Modified")
           << i18n("Attributes") << i18n("Owner/Group") << i18n("CRC") << i18n("Method") << i18n("Version") << i18n("Mimetype");

    rootNode = new AkuTreeNode(header);
}

void AkuTreeModel::Private::generateNodes(const QVector<QStringList> &source)
{
// TODO: generate parents/children

    for (int i = 0; i < source.size(); i++) {
        AkuTreeNode *node = new AkuTreeNode(source[i], rootNode);
        nodes<<node;
    }
}
