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
#include <QDir>

#include <KLocale>
#include <KIcon>
#include <KDebug>
#include <KMimeType>

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

    int rows = static_cast<AkuTreeNode*>(parent.internalPointer())->childCount();
    return rows;
}

int AkuTreeModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent)

    if (d->sourceData.isEmpty()) {
        return 0;
    }

    return d->sourceData.first().count() + 1;
   
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


    if (role == Qt::DecorationRole && index.column() == 0) {
        AkuTreeNode *node = static_cast<AkuTreeNode*>(index.internalPointer());
        if (node->isFolder()) {
            return KIcon("inode-directory");
        } else {
            KMimeType::Ptr mimeType = KMimeType::findByPath(node->data(0));
            return KIcon(mimeType->iconName());
        }
    }

    if (role == Qt::DisplayRole) {
        return static_cast<AkuTreeNode*>(index.internalPointer())->data(index.column());
    }

    return QVariant();

}

QModelIndex AkuTreeModel::parent(const QModelIndex &index) const
{
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
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (row > rowCount(parent) || column > columnCount(parent)) {
        return QModelIndex();
    }

    AkuTreeNode *parentNode;

    if (!parent.isValid()) {
        parentNode = d->rootNode;
    } else {
        parentNode = static_cast<AkuTreeNode*>(parent.internalPointer());
    }

    AkuTreeNode *childNode = parentNode->child(row);
    if (childNode) {
        return createIndex(row, column, childNode);
    } else {
        return QModelIndex();
    }
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
    QVector<QStringList> &source = sourceData;
    for (int i = 0; i < source.size(); i++) {

        //NOTE: Since the first string of the list is in the 
        // form folder/folder/folder/file
        // then we need to split it to create nodes.

        QString path = source[i][0];
        source[i].removeAt(0);
        QStringList pathNodes = path.split(QDir::separator());

        AkuTreeNode *parentNode = rootNode;

        for (int j = 0; j < pathNodes.size(); j++) {

            // NOTE: we should put attributes only on last item
            if (j == pathNodes.size() - 1) {
                AkuTreeNode *node = new AkuTreeNode(QStringList()<<pathNodes[j]<<source[i], parentNode);
                node->setFolder(false);
                parentNode->appendChild(node);
                continue;
            }

            // here we avoid duplication of nodes
            AkuTreeNode *existingNode = parentNode->findChildFolder(pathNodes[j]);
            if (existingNode) {
                parentNode = existingNode;
                continue;
            }

            AkuTreeNode *node = new AkuTreeNode(QStringList()<<pathNodes[j], parentNode);
            parentNode->appendChild(node);
            parentNode = node;
        }
    }
}

void AkuTreeModel::setSourceData(const QVector<QStringList> &source)
{
    delete d->rootNode;
    d->initData();
    d->sourceData = source;
    d->generateNodes();
    reset();
}

QVector<QStringList> AkuTreeModel::sourceData()
{
    return d->sourceData;
}
