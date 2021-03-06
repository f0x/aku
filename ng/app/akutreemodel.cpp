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

#include "akutreemodel.h"
#include "akutreenode.h"

#include <QFont>
#include <QLinearGradient>
#include <QVector>
#include <QDir>

#include <KGlobal>
#include <KGlobalSettings>
#include <KLocale>
#include <KIcon>
#include <KMimeType>
#include <KDebug>

class AkuTreeModel::Private
{
    public:
       Private(AkuTreeModel *q):
                           q(q)
                           {}
       ~Private()

       { delete rootNode; nodes.clear(); sourceData.clear();}

       AkuTreeModel *q;
       QVector<QStringList> sourceData;
       QList<AkuTreeNode*> nodes;
       AkuTreeNode *rootNode;
       QStringList additionalHeaders;

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
{
    delete d;
}

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

int AkuTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if (d->sourceData.isEmpty()) {
        return 0;
    }

    return d->sourceData.first().count(); //  + 1;
   
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
            KMimeType::Ptr mimeType = KMimeType::mimeType(node->mimeType());
            //kDebug() << mimeType->iconName();
            return KIcon(mimeType->iconName());
        }

    } else if (role == Qt::DisplayRole) {
        AkuTreeNode *node = static_cast<AkuTreeNode*>(index.internalPointer());
        if (index.column() == 0 && node->name().endsWith('*')) {
            QString item = node->name();
            item.chop(2);
            return item;
        }
        if (index.column() == 1) {
            if (node->isFolder()) {
                return QString();
            }
            return KGlobal::locale()->formatByteSize(node->size());
        }
        if (index.column() == 2) {
            if (node->isFolder()) {
                return QString();
            }
            return KGlobal::locale()->formatByteSize(node->packedSize());
        }     
        return node->data(index.column());
    } else if (role == NodeRole) {
        return qVariantFromValue(static_cast<AkuTreeNode*>(index.internalPointer()));
    }

    if (role == Qt::FontRole && index.column() == 0) {
        AkuTreeNode *node = static_cast<AkuTreeNode*>(index.internalPointer());
        if (node->name().endsWith("*")) {
            QFont font;
            font.setItalic(true);
            font.setBold(true);
            return QVariant(QFont(font));
        }
    }
    
    if (role == Qt::FontRole && index.column() == 3) {
        QFont font = KGlobalSettings::smallestReadableFont();
        return QVariant(QFont(font));
    }

    if (role == Qt::BackgroundRole && index.column() == 3) {
        AkuTreeNode *node = static_cast<AkuTreeNode*>(index.internalPointer());
        if (node->isFolder()) {
            return QVariant();
        }
        int ratio = node->ratio();
        QLinearGradient gradient(0, 5, 15, 25);
        gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
        if (ratio < 0) {
            gradient.setColorAt(0, QColor::fromRgbF(0.9, 0.01, 0.4, 1));
        } else if (ratio < 20) {
            gradient.setColorAt(0, QColor::fromRgbF(1, 0, 0, 1));
        } else if (ratio < 40) {
            gradient.setColorAt(0, QColor::fromRgbF(1, 0.5, 0, 1));
        } else if (ratio < 60) {
            gradient.setColorAt(0, QColor::fromRgbF(1, 1, 0, 1));
        } else if (ratio < 80) {
            gradient.setColorAt(0, QColor::fromRgbF(0, 1, 0, 1));
        //} else if (ratio <= 100) {
        //    gradient.setColorAt(0, QColor::fromRgbF(0, 0, 1, 1));
        } else {
            gradient.setColorAt(0, QColor::fromRgbF(0, 0, 1, 1));
        }

        return QVariant(QBrush(gradient));
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
    header << i18n("File Name") << i18n("Size") << i18n("Packed Size");
    header << additionalHeaders;

    rootNode = new AkuTreeNode(header);
}

void AkuTreeModel::Private::generateNodes()
{
    QVector<QStringList> &source = sourceData;
    foreach (QStringList file, source) {

        //NOTE: Since the first string of the list is in the 
        // form folder/folder/folder/file
        // then we need to split it to create nodes.

        QString path = file[0];
        file.removeAt(0);
        QStringList pathNodes = path.split(QDir::separator());

        AkuTreeNode *parentNode = rootNode;

        for (int j = 0; j < pathNodes.size(); j++) {

            // NOTE: we should put attributes only on last item
            if (j == pathNodes.size() - 1) {
                AkuTreeNode *node = new AkuTreeNode(QStringList() << pathNodes[j] << file, parentNode);

                // TODO: make a real check to see whether it is a folder or not
                if (parentNode->findChildFolder(pathNodes[j])) {
                    continue;
                }
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

            AkuTreeNode *node = new AkuTreeNode(QStringList() << pathNodes[j], parentNode);
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

void AkuTreeModel::setAdditionalHeaders(const QStringList &headers)
{
    d->additionalHeaders = headers;
}
