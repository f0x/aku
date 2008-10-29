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
    void generateNodes();
};

AkuTreeModel::AkuTreeModel(QVector<QStringList> fileList, QObject *parent) : 
                                               QAbstractItemModel(parent),
                                               d(new Private(this))
{
    d->sourceData = fileList;
    d->initData();
    d->generateNodes();
}

AkuTreeModel::~AkuTreeModel()
{}

int AkuTreeModel::rowCount(const QModelIndex & parent) const
{
    if (d->sourceData.isEmpty()) {
        return 0;
    }

    if (!parent.isValid()) {
        return d->rootNode->childCount();
    }

    return static_cast<AkuTreeNode*>(parent.internalPointer())->childCount();
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
        return static_cast<AkuTreeNode*>(index.internalPointer())->data(index.column());
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
        return createIndex(node->parent()->row(), 0, node->parent());
    }

    return QModelIndex();
}

QModelIndex AkuTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    // TODO: fill in with correct code
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (row > rowCount() || column > columnCount()) {
        return QModelIndex();
    }

    if (parent.isValid()) {
        // TODO: return the subnode correct index
    }

    return createIndex(row, column, d->rootNode->child(row));
//     return QModelIndex();
}

void AkuTreeModel::Private::initData()
{
    QStringList header;
    header << i18n("File Name") << i18n("Size") << i18n("Packed Size") << i18n("Ratio") << i18n("Modified")
           << i18n("Attributes") << i18n("Owner/Group") << i18n("CRC") << i18n("Method") << i18n("Version") << i18n("Mimetype");

    rootNode = new AkuTreeNode(header);
}

void AkuTreeModel::Private::generateNodes()
{
// TODO: generate parents/children
    const QVector<QStringList> &source = sourceData;
    for (int i = 0; i < source.size(); i++) {
        AkuTreeNode *node = new AkuTreeNode(source[i], rootNode);
        rootNode->appendChild(node);
        nodes<<node;
    }
}

void AkuTreeModel::setSourceData(const QVector<QStringList> &source)
{
    d->sourceData = source;
    d->generateNodes();
    reset();
}
